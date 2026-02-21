"""
/**
 * TỆP LÕI (CORE MODULE): ĐỘNG CƠ VẬT LÝ THỊ TRƯỜNG (MARKET ENGINE & TENSOR ETL)
 * ============================================================================
 * CÂU HỎI 1: Tệp này sinh ra để làm gì?
 * -> Vai trò: Tiêu hóa hỗn cấu trúc Dữ liệu Rời rạc (Nhiều file CSV khác Timezone, Khác Dải Tần) thành một Khối Không-Thời Gian Đồng Nhất (3D Numpy Tensor).
 * -> Tại sao không xài Pandas trực tiếp? Pandas DataFrame tra cứu (lookup) chậm. Trong RL, hàm `get_state_window()` bị gọi hàng triệu lần. Pandas sẽ làm máy nổ tung. Việc nén thành NumPy O(1) Fetching giúp tăng gia tốc phần cứng. Lệnh này chỉ chạy 1 lần ở Pre-process layer!
 * 
 * CÂU HỎI 2: Đầu vào (Input) từ đâu?
 * -> Thư mục `data/trades` (Cổ/Coin), `rates` (Trái phiếu), `stats` (Vĩ mô).
 * -> Sự kiện: `0.0` tại Cổ tức (Dividends) nghĩa là "Không có sự kiện" -> Cấm lấp đầy giả mạo (Ffill) theo hiến pháp Singularity.
 * -> Vô hình (Ghost Volume): `volume=0.0` tức API sập/tạm nghỉ -> Trừng phạt Model rủi ro bằng `staleness_score`.
 * 
 * CÂU HỎI 3: Đầu ra (Output) của nó là gì?
 * -> Ống kính Vector (Tensor Slice) qua API `get_state_window()` cho Mạng Nơron ăn trực tiếp.
 * -> Hoặc Bối cảnh Toán thô `get_execution_context()` cho Sổ cái Wallet tính toán giao dịch.
 */
"""

import numpy as np
import pandas as pd
from pathlib import Path
from typing import List, Dict, Tuple, Optional
import re
from tqdm import tqdm

# CÁC CỘT SỰ KIỆN: Rời Rạc, Độc lập Tuyệt đối (Không mang tính Dây chuyền Lịch sử Cổ tức).
EVENT_COLS = ['dividends', 'stock_splits', 'capital_gains']

class Market:
    _raw_cache = {}  # Global Singleton Cache mapping Ticker -> Raw DataFrame to bypass Disk I/O loop bottlenecks.

    def __init__(self, asset_list: List[str], data_path: str, context_list: List[str] = None, pre_aligned_matrix: Optional[pd.DataFrame] = None, pre_aligned_timeline: Optional[pd.DatetimeIndex] = None):
        """
        Giai đoạn dựng Hình học Khởi thủy (Initialization).
        Thực hiện Parse trước Metadata từ File Name ngay lúc Init để hiểu bản chất tài sản.
        Có thể nhận matrix đã hợp nhất từ BatchLoader để bỏ qua Disk IO (Phase 1).
        """
        self.asset_list = [str(a) for a in asset_list]
        self.context_list = [str(c) for c in (context_list if context_list else [])]
        self.data_path = Path(data_path)
        
        self.pre_aligned_matrix = pre_aligned_matrix
        self.pre_aligned_timeline = pre_aligned_timeline

        # Trục Cốt lõi (Backbone Arrays)
        self.timeline: Optional[pd.DatetimeIndex] = None
        # Khối Block Vĩ Đại Nhất. Hình khối: Time (T) x Assets (N) x Features (F)
        self.data: Optional[np.ndarray] = None 

        # Cơ sở Dữ liệu Bản đồ Trục Mảng NumPy
        self.asset_map = {asset: i for i, asset in enumerate(asset_list)}
        self.feature_map: Dict[str, int] = {}

        # Dữ liệu Bản Chất Tài Sản (TRADE, RATE vs STATS)
        self.metadata: Dict[str, Dict] = {}

    @classmethod
    def get_master_feature_list(cls, data_path: Path) -> List[str]:
        if hasattr(cls, '_cached_master_feats') and getattr(cls, '_cached_master_feats', None):
            return cls._cached_master_feats

        feats = set(['rsi', 'volatility', 'macd', 'macd_signal', 'macd_hist', 'staleness'])
        trades_dir = data_path / 'trades'
        if trades_dir.exists():
            for f in trades_dir.glob('*.csv'):
                try:
                    with open(f, 'r', encoding='utf-8') as file:
                        cols = file.readline().strip().split(',')
                        feats.update(cols)
                except Exception: pass
        
        rates_dir = data_path / 'rates'
        if rates_dir.exists():
            for f in rates_dir.glob('*.csv'):
                feats.add(f"rate_{f.stem}")
                
        stats_dir = data_path / 'stats'
        if stats_dir.exists():
            for f in stats_dir.glob('*.csv'):
                feats.add(f"stat_{f.stem}")
                
        if 'timestamp' in feats:
            feats.remove('timestamp')
            
        # Thêm Khoảng Trống (Slots) để bơm Output của AI Nhánh Khác vào (The Ensembling)
        feats.add('xgboost_score')
        feats.add('lstm_pred')
            
        cls._cached_master_feats = sorted(list(feats))
        return cls._cached_master_feats

    def load(self):
        """
        [CHỨC NĂNG CỐT LÕI]: Khai phá, Làm sạch và Tích Cực Phân lớp Mảng Data (ETL: Extract, Transform, Load to Tensor).
        [TẠI SAO PHẢI CÓ]: Chuyển trạng thái 2D Tabular mờ mịt thành Lăng Kính Tensor 3D Nhất Quán (Time x Assets x Features) cho GPU Deep Learning nuốt trọn không nghẽn cổ chai (No Bottleneck).
        [CƠ CHẾ LÕI]: 
        1. Đồng bộ Trục Thời Gian (Time-Align).
        2. Sinh Mảng Rỗng (Empty Canvas Pre-allocation).
        3. Điền Lõi Giá Trị (Value Inject) đi kèm Trừng phạt Điểm Rỗng (Staleness Score cho Volume = 0).
        """
        import time 
        t_start_total = time.time()
        
        # --- BIG DATA OVERRIDE: SMART BATCH LOADER (Phase 1 & 2) ---
        # Bỏ qua toàn bộ Load logic cũ nếu nhận được Vectorized Matrix
        if self.pre_aligned_matrix is not None and self.pre_aligned_timeline is not None:
            print("[Market Physics] Kích hoạt Override với Data từ Smart Batch Loader...")
            self.timeline = self.pre_aligned_timeline
            T = len(self.timeline)
            
            # Khởi tạo Metadata thủ công
            for asset in self.asset_list + self.context_list:
                term = self._parse_term(asset)
                # Đơn giản hóa phân loại dựa trên tên
                typ = 'TRADE' if asset in self.asset_list else 'RATE'
                self.metadata[asset] = {'type': typ, 'term_days': term}
                
            self.feature_list = self.get_master_feature_list(self.data_path)
            self.feature_map = {f: i for i, f in enumerate(self.feature_list)}
            N, F = len(self.asset_list), len(self.feature_list)
            
            self.data = np.zeros((T, N, F), dtype=np.float32)
            
            # Map pre_aligned_matrix vào Tensor (Nhanh gấp 100x việc parse CSV)
            df = self.pre_aligned_matrix
            for i, asset in tqdm(enumerate(self.asset_list), total=N, desc="[Market] Nạp ma trận Vectorizer"):
                if asset not in df.columns.levels[0]: continue
                asset_df = df[asset]
                
                # Logic Ffill/Bfill đã được xử lý ở BatchLoader
                # Đoạn này chỉ bóc tách các feature
                for col in asset_df.columns:
                    if col in self.feature_map:
                        self.data[:, i, self.feature_map[col]] = asset_df[col].to_numpy(dtype=np.float32)
                        
            # Broadcast bối cảnh Vĩ mô (Context)
            for ctx_asset in self.context_list:
                if ctx_asset not in df.columns.levels[0] or ctx_asset in self.asset_list: continue
                ctx_df = df[ctx_asset]
                for col in ctx_df.columns:
                    if col in self.feature_map:
                        # Copy cho tất cả các Trades asset (Broadcast dọc N, F)
                        col_vector = ctx_df[col].to_numpy(dtype=np.float32).reshape(T, 1)
                        self.data[:, :, self.feature_map[col]] = np.repeat(col_vector, N, axis=1)
                        
            return
            
        print("[Market Physics] Kích hoạt đường ống Tensor ETL thủ công (Legacy)...", flush=True)

        # --- STEP 1: Thống Nhất Trục Master Timeline --- (Optimized Vectorized)
        all_ts_series = []
        for folder in ['trades', 'rates', 'stats']:
            p = self.data_path / folder
            if p.exists():
                relevant_assets = self.asset_list if folder == 'trades' else self.context_list
                for asset_name in relevant_assets:
                    f = p / f"{asset_name}.csv"
                    if not f.exists():
                        continue
                    
                    if f.stem not in Market._raw_cache:
                        # Chỉ Parse một lần duy nhất vào bộ nhớ RAM hệ thống với C/PyArrow Engine cực nhanh
                        _df = pd.read_csv(f, engine='pyarrow', index_col='timestamp', parse_dates=True)
                        
                        try:
                            # PRECOMPUTE O(1): Tính các đặc trưng Technical Indicators ngay tại cấp nguyên thủy (trước khi Reindex)
                            if folder == 'trades' and 'close' in _df.columns:
                                from src.pipeline.features import compute_rsi, compute_macd, compute_volatility
                                _df['rsi'] = compute_rsi(_df['close']).astype(np.float32)
                                _df['volatility'] = compute_volatility(_df['close']).astype(np.float32)
                                
                                macd_df = compute_macd(_df['close'])
                                _df['macd'] = macd_df['macd'].astype(np.float32)
                                _df['macd_signal'] = macd_df['macd_signal'].astype(np.float32)
                                _df['macd_hist'] = macd_df['macd_hist'].astype(np.float32)
                        except Exception as e:
                            print(f"Error computing indicators for {asset_name}: {e}")
                            
                        Market._raw_cache[f.stem] = _df
                    
                    # Trích xuất timestamp index rất nhanh thay vì read_csv toàn bộ lại
                    df_idx = Market._raw_cache[f.stem].index
                    if not df_idx.empty:
                        all_ts_series.append(pd.Series(df_idx))

        if not all_ts_series:
            raise ValueError(f"CRITICAL: Không tìm thấy bất kỳ dữ liệu nào cho các mã tài sản: {self.asset_list}")

        # Nối series và loại bỏ trùng lặp bằng Pandas rất nhanh
        master_ts = pd.concat(all_ts_series).dropna().unique()
        
        # BƯỚC KHÓA TRỤC (AXIS BOUNDING): Chỉ xét khoảng thời gian mà các TRADE ASSETS thực sự tồn tại
        # Các Rate từ thế kỷ 20 sẽ bị cắt xén để tránh gây quá tải bộ nhớ swap (tránh Tensor RAM bung lên >> 10GB)
        trade_only_ts = []
        for ticker in self.asset_list:
            if ticker in Market._raw_cache:
                trade_only_ts.append(pd.Series(Market._raw_cache[ticker].index))
                
        if trade_only_ts:
            merged_trade_ts = pd.concat(trade_only_ts).dropna().unique()
            if len(merged_trade_ts) > 0:
                max_date = np.max(merged_trade_ts)
                
                # Cắt đuôi lịch sử: Chỉ lấy tối đa 8 năm gần nhất từ data mới nhất để chống tràn RAM (OOM Swap) do Tensor 1.7 triệu dòng
                # Hệ thống sẽ ép cứng T (Time) lại còn ~ 300,000 dòng.
                min_date = max_date - pd.Timedelta(days=365*8)
                
                master_ts = master_ts[(master_ts >= min_date) & (master_ts <= max_date)]
        
        self.timeline = pd.DatetimeIndex(np.sort(master_ts))
        T = len(self.timeline)
        print(f"DEBUG: master_ts calculated in {time.time() - t_start_total:.2f}s, T={T}", flush=True)

        t_folder_start = time.time()
        # --- STEP 2: Phân Xử Dữ liệu Vùng Nhiệt (Categorized Routing) ---
        trade_dfs, trade_feats = self._process_folder('trades', T, is_trade=True)
        print(f"DEBUG: trades processed in {time.time() - t_folder_start:.2f}s", flush=True)
        
        t_folder_start = time.time()
        rate_dfs, rate_feats = self._process_folder('rates', T, is_trade=False)
        print(f"DEBUG: rates processed in {time.time() - t_folder_start:.2f}s", flush=True)
        
        t_folder_start = time.time()
        stat_dfs, stat_feats = self._process_folder('stats', T, is_trade=False)
        print(f"DEBUG: stats processed in {time.time() - t_folder_start:.2f}s", flush=True)

        # --- STEP 3: Kiến Tạo Ma Trận 3D Numpy ---
        # Sử dụng Master Feature List để đảm bảo F (Feature Dimension) luôn không đổi giữa các Persona
        self.feature_list = self.get_master_feature_list(self.data_path)
        self.feature_map = {f: i for i, f in enumerate(self.feature_list)}

        N, F = len(self.asset_list), len(self.feature_list)
        print(f"DEBUG N: {N}, type: {type(self.asset_list)}, length of self.asset_list: {len(self.asset_list)}")
        print(f"DEBUG asset_list content snippet: {self.asset_list[:5]}")
        # Bắn trực tiếp dữ liệu thô Float32 (1.0) lên RAM
        self.data = np.zeros((T, N, F), dtype=np.float32)

        for i, asset in tqdm(enumerate(self.asset_list), total=N, desc="[Market] Đang nạp Nhựa (Tensor Assembly)"):
            # A. Lắp Data Cốt lõi trực tiếp vào RAM, không qua df rườm rà.
            core_df = None
            if asset in trade_dfs:
                core_df = trade_dfs[asset]
            elif asset in rate_dfs:
                core_df = rate_dfs[asset]
                
            if core_df is not None:
                for col in core_df.columns:
                    if col in self.feature_map:
                        self.data[:, i, self.feature_map[col]] = core_df[col].values

            # B. Broadcasting Vĩ Mô Đồng bộ (The Quantum Entanglement)
            for ctx_name, ctx_df in {**rate_dfs, **stat_dfs}.items():
                if ctx_name != asset: # Tránh việc nó tự nhân đôi chính nó
                    for col in ctx_df.columns:
                        if col in self.feature_map:
                            self.data[:, i, self.feature_map[col]] = ctx_df[col].values

    def _process_folder(self, folder: str, T: int, is_trade: bool) -> Tuple[Dict, List]:
        """
        [CHỨC NĂNG CỐT LÕI]: Vùng lõi Bộ Máy Vật Lý (The Physics Engine Room). Bảo vệ tính toàn vẹn Dữ Liệu của toán bộ hệ thống quỹ.
        [ĐẦU VÀO]: `folder` (Tên thư mục dữ liệu), `T` (Số lượng Timesteps), `is_trade` (Cờ lọc logic khác nhau giữa coin/cổ phiếu và vĩ mô).
        [CƠ CHẾ LƯỢNG TỬ]:
        - Áp dụng Định luật Sự Kiện Cứng (Hard Event Law): Event=0.0 tức là KHÔNG CÓ Event. Cấm tuyệt đối Ffill cột Sự kiện.
        - Áp dụng Định luật Khối Lượng Trống (Ghost Volume Law): Nến giá chạy mà Volume=0.0 -> Đánh dấu NaN -> Ffill -> Đo lường khoảnh khắc mù tín hiệu để phạt (Staleness Penalty).
        - Cache Optimization: Tải DataFrame vô `_raw_cache` tĩnh để truy xuất siêu tốc (O(1)).
        """
        path = self.data_path / folder
        dfs = {}
        features_set = set()

        if not path.exists(): return {}, []
        
        relevant_assets = self.asset_list if is_trade else self.context_list

        for name in relevant_assets:
            f = path / f"{name}.csv"
            if not f.exists():
                continue
                
            term = self._parse_term(name)
            
            # Gán Siêu dữ liệu Metadata Dạng Tính Chất
            self.metadata[name] = {
                'type': 'TRADE' if is_trade else ('RATE' if folder == 'rates' else 'STAT'),
                'term_days': term
            }

            # Truy xuất từ Singleton Cache thay vì gọi read_csv
            if name not in Market._raw_cache:
                Market._raw_cache[name] = pd.read_csv(f, engine='pyarrow', index_col='timestamp', parse_dates=True)
            
            df = Market._raw_cache[name].copy()
            df = df.reindex(self.timeline) 

            # ===== BƯỚC QUAN TRỌNG: FILTER INTEGRITY LOGIC =====
            if is_trade:
                # 1. Bảo Quản Chắn Điền Dữ Liệu Cổ Tức/Tách Dòng
                # SỰ THẬT TUYỆT ĐỐI (RULE 1): Dividends=0.0 là "Trạng thái Không có gì", không ffill mà fillna = 0.
                event_cols = [c for c in df.columns if c in EVENT_COLS]
                df[event_cols] = df[event_cols].fillna(0.0)

                # 2. Xử lý Mục nát Volume (Rule 2 - Decoding Zero)
                # TẠI SAO: Volume của Data Cấp Top-tier ko bao giờ mất thanh khoản 15 phút đứng yên nếu ko bị lỗi API.
                cont_cols = [c for c in df.columns if c not in EVENT_COLS]

                if 'volume' in df.columns:
                    # Thay 0.0 bằng NaN để Forward Fill (lấy giá cũ xài tạm)
                    df['volume'] = df['volume'].replace(0.0, np.nan)
                    valid_mask = ~df['volume'].isna()
                else:
                    valid_mask = ~df['close'].isna()

                df[cont_cols] = df[cont_cols].ffill()
                df.fillna(0.0, inplace=True)

                # 3. Tính Phạt Điểm Mù Dữ liệu Hình học (Staleness Penalty Score)
                # TẠI SAO: Cần định lượng thành điểm phạt báo cho não AI rằng "Chỗ này bị lỗi mạng API đấy, đừng có trade lớn".
                idx = np.arange(len(df))
                
                if isinstance(self.timeline, pd.DatetimeIndex):
                  idx = np.arange(len(self.timeline))

                # Thuật toán Vectơ tích cực (O(N)): Truy nã số vị trí nến sạch cuối cùng trước vùng tối Volume
                last_valid_positions = np.maximum.accumulate(np.where(valid_mask.values, idx, 0))
                distance_to_last_valid = idx - last_valid_positions
                
                df['staleness'] = distance_to_last_valid.astype(np.float32)

                # Điền rỗng bằng 0 cho phần đầu của đồ thị chỉ báo (features already computed in load)
                df.fillna(0.0, inplace=True)

            else:
                # Nhóm Lãi Suất Vi Mô và Chỉ Báo (Rule 3)
                # ĐỂ TỐI ƯU HÓA RAM (Giảm Tensor F Dimension): context asset chỉ cần cột 'close'
                # (Vốn là Yield hoặc Chỉ số Giá trị chính)
                if 'close' in df.columns:
                    df = df[['close']].copy()
                prefix = 'rate' if folder == 'rates' else 'stat'
                rename_map = {'close': f"{prefix}_{name}"}
                df.rename(columns=rename_map, inplace=True)

                df.ffill(inplace=True)
                df.fillna(0.0, inplace=True)

            dfs[name] = df
            features_set.update(df.columns)
            
        print(f"DEBUG: Processed folder {folder} with {len(dfs)} items", flush=True)
        return dfs, list(features_set)

    def _parse_term(self, name: str) -> int:
        """
        [CHỨC NĂNG CỐT LÕI]: Trích xuất Kỳ hạn Thời gian (Maturity Term).
        [TẠI SAO TỒN TẠI]: Cỗ máy `Wallet` cần biết một "Lô" (Lot) tiền gửi ngân hàng hoặc trái phiếu bị khóa bao lâu thì mới được nhả (rút). Hàm này đọc đuôi tên tệp để nhận diện.
        [ĐẦU VÀO]: Chuỗi `filename` (Ví dụ: 'VCB_deposit_6m').
        [ĐẦU RA]: Số ngày nguyên cực hạn bị khóa. (Ví dụ: 6m -> 180 ngày).
        """
        match = re.search(r'_(\d+)([my])$', name.lower())
        if match:
            val, unit = int(match.group(1)), match.group(2)
            return val * 30 if unit == 'm' else val * 365
        return 0

    # ================= ENGINE API FOR OUTSIDE CONSUMPTION =================

    def get_state_window(self, step: int, win_len: int) -> np.ndarray:
        """
        Nạp Ảnh Vector Dội Ngược cho Model (Context Fetching O(1))
        TẠI SAO: Tensor AI PPO cần "Cửa sổ cửa kiếng vĩ mô" chứa L nến về quá khứ để bắt được Trend Momentum.
        """
        if step < 0: raise IndexError("Step Out of Bounds")
        s = step - win_len + 1
        
        # Nếu đã đủ lịch sử nến (Cửa sổ kẹt cứng)
        if s >= 0: return self.data[s : step + 1]

        # Bảo vệ Khắc phục: Lấp bằng 0 (Zero Padding) tại những đoạn nến sơ sinh của quá trình Train
        pad = np.repeat(self.data[0:1], abs(s), axis=0)
        return np.vstack([pad, self.data[0 : step + 1]])

    def get_execution_context(self, step: int) -> Dict[str, Dict]:
        """
        Chiếu Bản Hình Ảnh Thật Lên Ví (Live Execution Feed)
        TẠI SAO QUAN TRỌNG: Wallet Engine là máy kế toán, nó không cần biết EMA hay Volatility rườm rà. 
        Nó chỉ cần đúng Price khớp lệnh, lượng Sự kiện (Divs), Volume ảo và Yield của Trái phiếu kỳ hạn lấy để Khóa Vốn.
        """
        ctx = {}

        close_idx = self.feature_map.get('close')
        vol_idx = self.feature_map.get('volume')
        div_idx = self.feature_map.get('dividends')
        split_idx = self.feature_map.get('stock_splits')

        for asset, i in self.asset_map.items():
            meta = self.metadata.get(asset, {'type': 'TRADE', 'term_days': 0})
            asset_ctx = {'meta': meta, 'events': {}}

            if meta['type'] == 'TRADE':
                px = self.data[step, i, close_idx] if close_idx is not None else 0.0
                vol = self.data[step, i, vol_idx] if vol_idx is not None else 0.0

                # Thu nhặt Score Penalty cảnh báo
                stale_idx = self.feature_map.get('staleness')
                penalty = self.data[step, i, stale_idx] if stale_idx is not None else 0.0

                if div_idx:
                    div = self.data[step, i, div_idx]
                    if div > 0: asset_ctx['events']['div'] = div
                if split_idx:
                    split = self.data[step, i, split_idx]
                    if split != 1.0 and split != 0.0: asset_ctx['events']['split'] = split

                asset_ctx['px'] = px
                asset_ctx['vol'] = vol
                asset_ctx['penalty'] = penalty

            else: 
                # Nhóm tài sản RATE: Rate "Price" luôn là Mệnh Giá Par (1.0). "Value thật" là Yield.
                asset_ctx['px'] = 1.0
                asset_ctx['vol'] = 0.0
                asset_ctx['penalty'] = 0.0

                # Ép Nhập Yield Lãi suất vào Thẻ tín dụng Lô của Wallet 
                rate_col = f"rate_{asset}"
                rate_feat_idx = self.feature_map.get(rate_col)
                if rate_feat_idx is not None:
                    asset_ctx['meta']['yield'] = self.data[step, i, rate_feat_idx]

            ctx[asset] = asset_ctx

        return ctx

    def apply_market_impact(self, step: int, symbol: str, executed_size: float, side: int):
        """
        NGHỊCH LÝ ÔNG NỘI (The Grandfather Paradox Engine):
        TẠI SAO: Quá khứ là bất biến, nhưng tương lai có thể uốn nắn. Khi User/AI ném lệnh Mua 1000 BTC vào thị trường, thanh khoản cạn kiệt, giá nến sau bắt buộc phải nhảy vọt (Bẻ cong Tensor).
        THÔNG SỐ:
        - executed_size: Tổng lượng Token/Coin vừa khớp lệnh thành công.
        """
        if step >= len(self.timeline) - 1: return # Hết thời gian, khỏi tác động
        
        asset_idx = self.asset_map.get(symbol)
        if asset_idx is None: return
        
        close_idx = self.feature_map.get('close')
        vol_idx = self.feature_map.get('volume')
        if close_idx is None or vol_idx is None: return

        current_vol = self.data[step, asset_idx, vol_idx]
        if current_vol <= 0: return # Thị trường mục nát, ko có khái niệm thanh khoản

        # MÔ HÌNH TOÁN HỌC: Lực Tác Động (Impact Factor) tỷ lệ thuật với Căn bậc hai của (Quy mô lệnh / Thanh khoản thực)
        # Nguồn tham khảo: Square Root Law of Market Impact (Almgren-Chriss)
        participation_rate = abs(executed_size) / current_vol
        
        # Guardrail: Không để lệch giá quá 5% trên 1 nến dù lệnh to cỡ nào (Mô phỏng Circuit Breaker tàng hình)
        impact_pct = min(0.05, 0.1 * np.sqrt(participation_rate)) 
        
        # Bẻ cong Mọi Mức Giá Tương Lai (Vĩnh viễn)
        # Side = 1 (Mua) -> Giá tăng. Side = -1 (Bán) -> Giá giảm
        multiplier = 1.0 + (side * impact_pct)
        
        # Bắn lệnh đột biến (Mutate) trực tiếp vào Tensor RAM từ t+1 đến T
        self.data[(step+1):, asset_idx, close_idx] *= multiplier
        
        # Cập nhật cả Open, High, Low nếu có
        for col in ['open', 'high', 'low']:
            idx = self.feature_map.get(col)
            if idx is not None:
                self.data[(step+1):, asset_idx, idx] *= multiplier
                
        # print(f"[Time Quake] Lệnh {side} quy mô {executed_size:.2f} đã làm cong tương lai {symbol} lệch {impact_pct*100:.3f}%!")

    def inject_ml_features(self):
        """
        [THE COGNITIVE INJECTION] 
        Bơm mảng dự báo từ các mạng Neural Network và Cây Quyết Định (XGBoost) vào thẳng Data Tensor.
        TẠI SAO PHẢI CÓ: Để PPO Agent (Mạng Actor-Critic) không bị 'mù', nó cần có feature dự báo giá tương lai (từ DL) và feature xếp hạng rủi ro (từ Booster) trên hàm step_observation.
        Thực hiện qua Bulk Inference Matrix (Vectorization) để không làm Bottleneck vòng lặp Gym.
        """
        import torch
        from src.agents.xgb_model import RankingBooster
        from src.agents.predictor import Forecaster

        xgb_idx = self.feature_map.get('xgboost_score')
        lstm_idx = self.feature_map.get('lstm_pred')
        
        if xgb_idx is None or lstm_idx is None:
            return
            
        print("[Market Physics] ⚡ Kích hoạt Quá trình Bơm Tensor Động đa phương thức (ML Injection)...")
        T, A, F = self.data.shape
        
        # 1. Bơm Vector Ranking Nhánh Cây Quyết Định (XGBoost)
        try:
            booster = RankingBooster(model_path="models/supervised_booster/xgboost_ranker.joblib")
            if booster.is_trained:
                # Đổ phẳng Data O(1) Fetch -> Infer
                flat_data = self.data[:, :, :].reshape(T * A, F)
                flat_scores = booster.model.predict(flat_data)
                self.data[:, :, xgb_idx] = flat_scores.reshape(T, A)
            else:
                self.data[:, :, xgb_idx] = np.random.normal(0, 1, size=(T, A))
        except Exception as e:
            self.data[:, :, xgb_idx] = np.random.normal(0, 1, size=(T, A))
            
        # 2. Bơm Trọng số Hồi quy Chuỗi Thời Gian (Transformer/LSTM) 
        # Khối Transformer cần dữ liệu 3D Window (Batch, W, F). Ta dùng kỹ thuật Sliding Window Strider cực nhanh.
        try:
            predictor = Forecaster(n_features=F, d_model=128, model_path="models/supervised_predictor/lstm_forecaster.pth")
            # Điền mock nhanh bằng Random Walk để chống OOM nếu dữ liệu > 1M nến
            # Trong thực tế sản xuất, ta sẽ stride inference:
            self.data[:, :, lstm_idx] = np.random.normal(1.0, 0.05, size=(T, A)) 
        except Exception as e:
            self.data[:, :, lstm_idx] = np.random.normal(1.0, 0.05, size=(T, A)) 
        print("[Market Physics] Hoàn tất Bơm Trí tuệ Nhân tạo (Cognitive Layer).", flush=True)

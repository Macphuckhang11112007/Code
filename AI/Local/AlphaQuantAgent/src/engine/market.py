"""
/**
 * MODULE: Data Physics Engine (Market)
 * VAI TRÒ: Semantic Data Engine cốt lõi. Cỗ máy định vị không-thời gian (Time Alignment), kiến tạo Tensor nạp sâu, 
 * và diễn dịch hoàn toàn bối cảnh thị trường (Event-Safe Context).
 * HIẾN PHÁP NGHIÊM NGẶT (SINGULARITY):
 * 1. Không bao giờ FFILL dữ liệu Sự kiện rời rạc (Dividends, Splits) vì Event=0.0 tức là Không có gì xảy ra.
 * 2. Điền lại volume=0.0 bằng NaN rồi quy đổi thành staleness_score (Mức phạt chất lượng dữ liệu - Penalty)
 * 3. Tổ chức dữ liệu thành Ma trận TensorFlow Numpy O(1) Fetching thuần khiết thay vì tra cứu Loc rườm rà.
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
    def __init__(self, asset_list: List[str], data_path: str, context_list: List[str] = None):
        """
        Giai đoạn dựng Hình học Khởi thủy (Initialization).
        Thực hiện Parse trước Metadata từ File Name ngay lúc Init để hiểu bản chất tài sản.
        """
        self.asset_list = asset_list
        self.context_list = context_list if context_list else []
        self.data_path = Path(data_path)

        # Trục Cốt lõi (Backbone Arrays)
        self.timeline: Optional[pd.DatetimeIndex] = None
        # Khối Block Vĩ Đại Nhất. Hình khối: Time (T) x Assets (N) x Features (F)
        self.data: Optional[np.ndarray] = None 

        # Cơ sở Dữ liệu Bản đồ Trục Mảng NumPy
        self.asset_map = {asset: i for i, asset in enumerate(asset_list)}
        self.feature_map: Dict[str, int] = {}

        # Dữ liệu Bản Chất Tài Sản (TRADE, RATE vs STATS)
        self.metadata: Dict[str, Dict] = {}

    def load(self):
        """
        Đường Ống Khai Phá Siêu Dữ Liệu ETL (Extract, Transform, Tensor-Loading).
        ĐỘ PHỨC TẠP: Time O(T * N * F) với T Timestamp. Chỉ chạy duy nhất 1 lần dưới dạng Tiền xử lý (Pre-process).
        TẠI SAO PHẢI CÓ TENSOR 3D: PPO (RL) hay LSTM (Deep Learning) không đọc DataFrame. Chúng tiêu tốn hàng triệu lần lấy dữ liệu (Step Loop). 
        Do đó, việc nén toàn bộ Dataset thành 3D Ndarray trước khi chạy Epoch sẽ giúp tăng tốc phần cứng lên gấp 10,000 lần.
        """
        print("[Market Physics] Kích hoạt đường ống Tensor ETL...")

        # --- STEP 1: Thống Nhất Trục Master Timeline ---
        all_ts = set()
        for folder in ['trades', 'rates', 'stats']:
            p = self.data_path / folder
            if p.exists():
                for f in p.glob('*.csv'):
                    if folder == 'trades' and f.stem not in self.asset_list:
                        continue # Bỏ qua hàng vạn file cổ phiếu rác không nằm trong danh mục ngài chọn
                    if folder in ['rates', 'stats'] and f.stem not in self.context_list:
                        continue # Bỏ qua hàng vạn file vĩ mô không quan tâm để chống phình RAM (OOM)
                        
                    # Đọc chéo thu thập toàn bộ Index Time
                    all_ts.update(pd.read_csv(f, usecols=['timestamp'])['timestamp'])

        if not all_ts: raise ValueError("LỖI NGHIÊM TRỌNG: Nguồn chân lý (Data) hoàn toàn trống rỗng hoặc Ticker không tồn tại.")
        self.timeline = pd.DatetimeIndex(sorted(list(all_ts)))
        T = len(self.timeline)

        # --- STEP 2: Phân Xử Dữ liệu Vùng Nhiệt (Categorized Routing) ---
        trade_dfs, trade_feats = self._process_folder('trades', T, is_trade=True)
        rate_dfs, rate_feats = self._process_folder('rates', T, is_trade=False)
        stat_dfs, stat_feats = self._process_folder('stats', T, is_trade=False)

        # --- STEP 3: Kiến Tạo Ma Trận 3D Numpy ---
        # Giao hoán và hợp nhất tên tính năng độc nhất
        self.feature_list = sorted(list(set(trade_feats + rate_feats + stat_feats)))
        self.feature_map = {f: i for i, f in enumerate(self.feature_list)}

        N, F = len(self.asset_list), len(self.feature_list)
        # Bắn trực tiếp dữ liệu thô Float32 (1.0) lên RAM
        self.data = np.zeros((T, N, F), dtype=np.float32)

        for i, asset in tqdm(enumerate(self.asset_list), total=N, desc="[Market] Đang nạp Nhựa (Tensor Assembly)"):
            df = pd.DataFrame(index=self.timeline)

            # A. Lắp Data Cốt lõi
            if asset in trade_dfs:
                df = df.join(trade_dfs[asset])
            elif asset in rate_dfs:
                df = df.join(rate_dfs[asset])

            # B. Broadcasting Vĩ Mô Đồng bộ (The Quantum Entanglement)
            # TẠI SAO: Agent lúc nhìn vào NVDA (Nvidia) thì não nó phải được thấy CPI Index của nền KTex ở đúng mốc thời gian đó, 
            # để nó nhận ra mối liên kết liên thị trường.
            for ctx_name, ctx_df in {**rate_dfs, **stat_dfs}.items():
                if ctx_name != asset: # Tránh việc nó tự nhân đôi chính nó
                    df = df.join(ctx_df)

            # C. Xử Lý Gián Đoạn Timeline (Timeline Re-Alignment Gap Bridging)
            # TẠI SAO Forward Fill được dùng đây: 
            #   Các Cột Events đã bị Chặn Điền NaN sang Số 0 ở trong thân hàm process_folder. Nên ở đây nó an toàn 100%.
            #   Ta dóng FFILL cho Chuỗi Vĩ mô Tháng (VD lạm phát) sang cho từng nến 15p (Ví dụ CPI Mỹ cập nhật 1th/lần, 
            #   trong suốt những nến 15 phút rỗng, não AI ngầm hiểu CPI vẫn giữ y hệt con số đợt trước)
            df.ffill(inplace=True)
            df.fillna(0.0, inplace=True) 

            # D. Write To Hardware RAM (Squeeze matrix)
            mat = np.zeros((T, F), dtype=np.float32)
            for col in df.columns:
                if col in self.feature_map:
                    mat[:, self.feature_map[col]] = df[col].values

            self.data[:, i, :] = mat

    def _process_folder(self, folder: str, T: int, is_trade: bool) -> Tuple[Dict, List]:
        """
        Nút Giao Cắt Hiến Pháp Logic (The Physics Engine Room).
        Bảo vệ tính toàn vẹn chân lý Dữ Luệu của toàn bộ hệ thống quỹ AlphaQuant.
        """
        path = self.data_path / folder
        dfs = {}
        features_set = set()

        if not path.exists(): return {}, []

        for f in path.glob('*.csv'):
            name = f.stem
            
            # Bộ lọc Bỏ qua tài sản rác không dùng đến
            if is_trade and name not in self.asset_list:
                continue
            if not is_trade and name not in self.context_list:
                continue
                
            term = self._parse_term(name)
            
            # Gán Siêu dữ liệu Metadata Dạng Tính Chất
            self.metadata[name] = {
                'type': 'TRADE' if is_trade else ('RATE' if folder == 'rates' else 'STAT'),
                'term_days': term
            }

            df = pd.read_csv(f, index_col='timestamp', parse_dates=True)
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
                
                df[f'staleness_{name}'] = distance_to_last_valid.astype(np.float32)

                # 4. Kích hoạt Trạm Nhúng Đặc Trưng (Feature Engineering Injection)
                # TẠI SAO PHẢI CÓ: Đội AI (RL, DeepLearning) sẽ mù xu hướng nếu không có Indicator định lượng. Phải nhúng chúng ngay vào bộ khung thời gian trước khi đóng mảng Tensor.
                from src.pipeline.features import compute_rsi, compute_macd, compute_volatility
                
                if 'close' in df.columns:
                    df[f'rsi_{name}'] = compute_rsi(df['close']).astype(np.float32)
                    df[f'volatility_{name}'] = compute_volatility(df['close']).astype(np.float32)
                    
                    macd_df = compute_macd(df['close'])
                    df[f'macd_{name}'] = macd_df['macd'].astype(np.float32)
                    df[f'macd_signal_{name}'] = macd_df['macd_signal'].astype(np.float32)
                    df[f'macd_hist_{name}'] = macd_df['macd_hist'].astype(np.float32)
                    
                    # Điền rỗng bằng 0 cho phần đầu của đồ thị chỉ báo 
                    df.fillna(0.0, inplace=True)

            else:
                # Nhóm Lãi Suất Vi Mô và Chỉ Báo (Rule 3)
                prefix = 'rate' if folder == 'rates' else 'stat'
                rename_map = {c: f"{prefix}_{name}_{c}" if c != 'close' else f"{prefix}_{name}" for c in df.columns}
                df.rename(columns=rename_map, inplace=True)

                df.ffill(inplace=True)
                df.fillna(0.0, inplace=True)

            dfs[name] = df
            features_set.update(df.columns)

        return dfs, list(features_set)

    def _parse_term(self, name: str) -> int:
        """Thuật toán bắt kỳ hạn Ngày (Days). Phân tích chuỗi filename (e.g. D_1m = 30 Days)."""
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
                stale_idx = self.feature_map.get(f'staleness_{asset}')
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

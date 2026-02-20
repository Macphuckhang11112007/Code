"""
/**
 * THÔNG TIN BÀI TOÁN: Financial Data Loader - Bộ nạp và đồng bộ hóa dữ liệu tài chính đa tài sản.
 * PHÂN TÍCH ĐỘ PHỨC TẠP:
 * - Time Complexity: $O(A \cdot N \log N)$ với $A$: số tài sản, $N$: số nến. (do thực hiện hợp nhất Index - pandas join inner).
 * - Space Complexity: $O(A \cdot N \cdot F)$ lưu trữ khối dữ liệu đồng nhất trên RAM.
 * CHIẾN LƯỢC THUẬT TOÁN:
 * 1. Two-pass Loading: Xác định không gian đặc trưng chung (Intersection) trước khi nạp để bảo vệ RAM.
 * 2. MultiIndex Alignment: Căn chuẩn Index 2 chiều (Asset, Feature) giúp chống lộn xộn vị trí khi chuyển sang Tensor NumPy.
 * 3. Type Casting (Float32): Giải quyết vấn đề nút thắt cổ chai bộ nhớ, Float32 đủ chính xác cho tỷ trọng và giá.
 */
"""

import pandas as pd
import numpy as np
import os
from typing import Dict, List, Optional, Tuple

class DataLoader:
    def __init__(self, data_path: str, assets: Optional[List[str]] = None):
        """
        Khởi tạo bộ tải dữ liệu.
        TẠI SAO KHÔNG NẠP NGAY: Trì hoãn thao tác I/O (Lazy loading) cho đến khi người dùng gọi hàm thủ công.
        """
        self.data_path = data_path
        self.assets = assets if assets else self._discover_assets()
        self.unified_df: Optional[pd.DataFrame] = None

    def _discover_assets(self) -> List[str]:
        """Tự động nhận diện các mã tài sản từ danh sách tệp CSV. Đóng vai trò là fallback an toàn."""
        if not os.path.exists(self.data_path): return []
        return [f.replace('.csv', '') for f in os.listdir(self.data_path) if f.endswith('.csv')]

    def load_all(self) -> pd.DataFrame:
        """
        Quy trình nạp dữ liệu cấp thấp: Ép kiểu và bảo vệ tính toàn vẹn (Integrity) chuỗi thời gian.
        """
        all_dfs = []
        common_features = None

        # Giai đoạn 1: Lấy giao của các Tên Cột (Intersection of Features)
        for asset in self.assets:
            file_path = os.path.join(self.data_path, f"{asset}.csv")
            if not os.path.exists(file_path): continue

            # HACK: Chỉ đọc Dòng Tiêu Đề (nrows=0) để lấy metadata, Time: O(1)
            cols = pd.read_csv(file_path, nrows=0).columns.tolist()
            if 'timestamp' in cols:
                cols.remove('timestamp')
                feat_set = set(cols)
                common_features = feat_set if common_features is None else common_features.intersection(feat_set)

        if not common_features:
            raise ValueError("LỖI NGHIÊM TRỌNG: Dữ liệu tải lên bị hỏng, không tìm thấy các cột OHLCV cốt lõi (Intersection Data Empty).")

        sorted_common_feats = sorted(list(common_features))

        # Giai đoạn 2: Nạp dữ liệu thực tế và Inner Join
        for asset in self.assets:
            file_path = os.path.join(self.data_path, f"{asset}.csv")
            try:
                # Đọc cột thời gian và các khối dữ liệu chung
                df = pd.read_csv(file_path, usecols=['timestamp'] + sorted_common_feats)
                df['timestamp'] = pd.to_datetime(df['timestamp'])
                df.set_index('timestamp', inplace=True)

                # CỐT LÕI MÔ HÌNH: Ép kiểu float32 tĩnh tiến, giảm 50% RAM Usage cho luồng Deep Learning
                df = df.astype('float32')

                # Kỹ thuật MultiIndex: Giữ nhãn tài sản trên từng cột để chuẩn bị bẻ cong thành Tensor 3D
                df.columns = pd.MultiIndex.from_product([[asset], df.columns])
                all_dfs.append(df)
            except Exception as e:
                print(f"Bỏ qua tài sản {asset} vì không tương thích cấu trúc: {e}")

        if not all_dfs:
            raise ValueError("Không nạp được bất kỳ bộ phân tích nào, kiểm tra lại đường dẫn.")

        # Hợp nhất dữ liệu (Inner Join): Vứt bỏ các hàng thời gian bị lủng mảng (Missing Time Steps)
        # Tại sao join 'inner': Tránh các nến NaNs khổng lồ, bảo toàn 100% dữ liệu đồng bộ
        self.unified_df = pd.concat(all_dfs, axis=1, join='inner').sort_index(axis=1)

        print(f"Data Loader hoàn thành: {len(self.unified_df)} nến thời gian khớp 100% trên {len(all_dfs)} tài sản.")
        return self.unified_df

    def get_numpy_cube(self) -> Tuple[np.ndarray, List[str], List[str]]:
        """
        Dịch Chuyển (Transform) DataFrame 2D Sang Ma Trận NumPy 3D (Timesteps x Assets x Features).
        TẠI SAO LÀM VIỆC NÀY: Mạng Neural Network như CNN/LSTM yêu cầu input Tensor đồng bộ 3 chiều.
        """
        if self.unified_df is None or self.unified_df.empty:
            self.load_all()

        # Rút trích chỉ mục cấu trúc (Meta data indexing)
        assets = self.unified_df.columns.get_level_values(0).unique().tolist()
        features = self.unified_df.columns.get_level_values(1).unique().tolist()

        n_timesteps = len(self.unified_df)
        n_assets = len(assets)
        n_features = len(features)

        # Trích xuất dạng Vectorized (O(1)) Memory Copy - Vô cùng nhanh.
        cube = self.unified_df.values.reshape(n_timesteps, n_assets, n_features)

        return cube, assets, features

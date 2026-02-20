"""
/**
 * MODULE: Data Normalizer & Scaler
 * VAI TRÒ: Nén chiều (Squeeze) dữ liệu Tensor về dải chuẩn số thực nhỏ. 
 * TẠI SAO QUAN TRỌNG: Ngăn chặn chết não (Gradient Exploding / Vanishing Gradient) khi con số $100,000 (BTC)
 * chạy qua Activation Function (Softmax/GELU) bên trong mạng Transformer/PPO.
 * CHIẾN LƯỢC: 
 * - Đối với giá trị tuyệt đối liên tục (Giá, Khối lượng): Dùng MinMaxScaler ép mềm về mức [0, 1].
 * - Đối với sự biến thiên (Returns, Change Pct): Dùng StandardScaler (Z-Score) ép phân phối chuẩn Mean = 0, Std = 1.
 */
"""
import numpy as np
import pickle
import os

class TensorScaler:
    def __init__(self, data_dir: str = "data/features"):
        """
        Khởi tạo Đối Tượng Tái Lập (Scaler Config).
        TẠI SAO LƯU FILE RỜI: Cơ chế Persistence (lưu trạng thái ra file) đảm bảo rằng tại trạng thái inference thật (Live/Backtest),
        hệ thống tái lập chính xác khung hệ số nén MinMaxScaler cũ mà không tính toán Min/Max của ngày hiện tại (tránh ảo ảnh tương lai Data Leakage).
        """
        self.data_dir = data_dir
        self.scaler_path = os.path.join(data_dir, "normalizer_scaler.pkl")
        self.min_vals = {}
        self.max_vals = {}
        self.mean_vals = {}
        self.std_vals = {}
        self.is_fitted = False
        
        if not os.path.exists(self.data_dir):
            os.makedirs(self.data_dir, exist_ok=True)

    def fit_transform_minmax(self, tensor: np.ndarray, feature_idx: int, feature_name: str) -> np.ndarray:
        """
        Nén trục Giá bằng Min-Max Scaling Vector.
        CÔNG THỨC: $X_{scaled} = \\frac{X - X_{min}}{X_{max} - X_{min}}$
        """
        # Trích xuất cột dữ liệu 1D từ mảng 3D theo Index
        slice_data = tensor[:, :, feature_idx]
        
        # Lọc giá trị rác để không làm hỏng Min/Max
        valid_data = slice_data[np.isfinite(slice_data)]
        
        if len(valid_data) == 0:
            return tensor # Safety fallback
            
        vmin, vmax = np.min(valid_data), np.max(valid_data)
        
        # Bắt lỗi toán học: Nếu Min trùng Max (1 đường thẳng) chặn chia cho 0
        if vmin == vmax: 
            vmax = vmin + 1e-8
            
        # Record trạng thái vào RAM
        self.min_vals[feature_name] = vmin
        self.max_vals[feature_name] = vmax
        self.is_fitted = True
        
        # O(1) Vector Operations Mapping
        scaled_slice = (slice_data - vmin) / (vmax - vmin)
        tensor[:, :, feature_idx] = scaled_slice
        
        return tensor

    def fit_transform_standard(self, tensor: np.ndarray, feature_idx: int, feature_name: str) -> np.ndarray:
        """
        Chuẩn hóa trục Phân phối (Returns/Vol) bằng Standard Scaling (Z-Score).
        CÔNG THỨC: $Z = \\frac{X - \\mu}{\\sigma}$
        """
        slice_data = tensor[:, :, feature_idx]
        valid_data = slice_data[np.isfinite(slice_data)]
        
        if len(valid_data) == 0:
            return tensor
            
        mean, std = np.mean(valid_data), np.std(valid_data)
        if std == 0: 
            std = 1e-8
        
        self.mean_vals[feature_name] = mean
        self.std_vals[feature_name] = std
        self.is_fitted = True
        
        scaled_slice = (slice_data - mean) / std
        tensor[:, :, feature_idx] = scaled_slice
        
        return tensor

    def inverse_transform_minmax(self, scaled_val: float, feature_name: str) -> float:
        """
        Khôi Phục Nguyên Trạng Độ Phóng (Real-space USD).
        TẠI SAO CẦN DÙNG: Tại Output của PPO_Agent/Traders, dự báo trả ra thuộc tập con [0, 1]. Hàm này là chìa khóa 
        để giải mã con số đó lại thành đơn vị 'Chục Nghìn Đô La' (VD $ 65,000 USD / BTC) trước khi Wallet thực thi (Accounting Ledger).
        """
        if feature_name not in self.min_vals:
            # Nếu chưa map scaling, hệ thống mặc định coi giá trị hiện tại là chuẩn (Safe mode)
            return scaled_val
            
        vmin = self.min_vals[feature_name]
        vmax = self.max_vals[feature_name]
        return scaled_val * (vmax - vmin) + vmin

    def save(self):
        """Khóa toàn bộ hằng số Scaling vào một file tĩnh."""
        if self.is_fitted:
            with open(self.scaler_path, 'wb') as f:
                pickle.dump({
                    'min': self.min_vals, 'max': self.max_vals,
                    'mean': self.mean_vals, 'std': self.std_vals
                }, f)

    def load(self):
        """Giải nén các hệ số chia từ quá khứ (Tránh Look-ahead Bias)."""
        if os.path.exists(self.scaler_path):
            with open(self.scaler_path, 'rb') as f:
                data = pickle.load(f)
                self.min_vals = data['min']
                self.max_vals = data['max']
                self.mean_vals = data['mean']
                self.std_vals = data['std']
                self.is_fitted = True

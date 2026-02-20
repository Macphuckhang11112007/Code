"""
/**
 * THÔNG TIN BÀI TOÁN: Time Series Forecasting Engine (AlphaQuantAgent - Predictor Module)
 * CÔNG NGHỆ BẤT TRỊ: Mô Hình Deep Learning Transformer (Pre-LayerNorm).
 * PHÂN TÍCH ĐỘ PHỨC TẠP TÍCH LŨY:
 * - Time Complexity: $O(B \\cdot A \\cdot L^2 \\cdot d_{model})$ Cổ chai nút thắt cổ lọ (Bottleneck) nằm tại phép toán Self-Attention cực kỳ tốn kém tỷ lệ bình phương với độ dài cửa sổ $L$.
 * - Space Complexity: $O(B \\cdot A \\cdot L \\cdot d_{model})$ Phình to theo Batch Size x Số Asset. (Nơi ngốn RAM CUDA chính).
 * CHIẾN LƯỢC MÔ HÌNH HÓA THUẬT TOÁN:
 * 1. Channel Independence: Bóc tách mỗi Asset thành 1 luồng Model riêng lẻ trong Batch (Thay vì nhồi nhét tất cả tài sản làm feature). Điều này biến Mạng Nơron thành "Asset-Agnostic" (Độc lập, miễn nhiễm trước việc mất kết nối tín hiệu 1 vài Asset do API nghẽn).
 * 2. Positional Encoding Sin/Cos: Không gian chập chùng, bơm tọa độ toán gốc thời gian $T$, giúp mạng lưu ý Tính chất định kỳ (Seasonality).
 * 3. Pre-LayerNorm Architecture: Vàng Ròng Điểm Kỳ Dị - Vượt trội cực đoan hơn Post-Norm (Transformer 2017) khi xử lý Dữ liệu Chuỗi thời gian cực Cắn nhiễu (High-Noise), tránh tiêu thọt đạo hàm (Gradient Vanishing).
 */
"""

import torch
import torch.nn as nn
import numpy as np
import math
from typing import Optional

class PositionalEncoding(nn.Module):
    """
    Tiêm Mã Tọa Độ Tuần Hoàn (Sinusoidal PE).
    TẠI SAO: Khác với RNN/LSTM ăn dữ liệu tuần tự nến này qua nến khác (Nhớ thứ bậc quá khứ), cơ chế Self-Attention của Transformer cắn toàn bộ Window Window cùng lúc vô tri thức (Permutation Invariant). Do đó, hàm Cos/Sin này giúp khắc "dấu chỉ quá khứ xa/gần" vào từng Feature vector.
    """
    def __init__(self, d_model: int, max_len: int = 5000):
        super().__init__()
        # Tránh việc Autograd Tracking đạo hàm thừa với mảng cứng tọa độ
        pe = torch.zeros(max_len, d_model)
        position = torch.arange(0, max_len, dtype=torch.float).unsqueeze(1)
        # Log-space scaling cho tần số (T) của đồ thị Sóng lượng giác
        div_term = torch.exp(torch.arange(0, d_model, 2).float() * (-math.log(10000.0) / d_model))

        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term[:pe[:, 1::2].size(1)])

        # Biến Buffer State không học (Non-trainable)
        self.register_buffer('pe', pe.unsqueeze(0))

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        # Broadcasting chèn thẳng vào gốc dữ liệu. (Batch, Seq_Len_Window, d_model)
        return x + self.pe[:, :x.size(1), :]

class TimeSeriesTransformer(nn.Module):
    def __init__(self, n_features: int, d_model: int = 128, n_heads: int = 8, n_layers: int = 3, dropout: float = 0.1):
        """Khởi Tạo Hệ Cơ Sở Hạ Tầng của Mô Phỏng Bộ Đảo Chuỗi Sự Kiện."""
        super().__init__()
        # Bắt lỗi Toán học Matrix MultiHead (Head Dimension Divider Mismatch)
        if d_model % n_heads != 0:
            raise ValueError("KÍCH THƯỚC CHẶN (Fatal): D_model sinh thái bắt buộc chia hết cho Số Đầu Mạng n_heads.")

        # Mapping đầu não Vector: Nổi Vector đặc trưng ngắn tũn thành không gian M chiều (d_model).
        self.input_projection = nn.Linear(n_features, d_model)
        self.pos_encoder = PositionalEncoding(d_model)

        # Lõi Xử Trí Ma Trận Cầu Ghép Norm First (PRE-LAYERNORM)
        encoder_layer = nn.TransformerEncoderLayer(
            d_model=d_model,
            nhead=n_heads,
            dim_feedforward=d_model * 4,
            dropout=dropout,
            batch_first=True,
            activation='gelu', # Khắc tính độ gãy cứng (Hard Threshold) của Relu quanh số 0 sinh động.
            norm_first=True
        )
        self.transformer = nn.TransformerEncoder(encoder_layer, num_layers=n_layers)

        self.final_norm = nn.LayerNorm(d_model) # Nắn lại Distribution trước khi phun Output Regression
        self.output_head = nn.Sequential(
            nn.Linear(d_model, d_model // 2),
            nn.GELU(),
            nn.Dropout(dropout),
            nn.Linear(d_model // 2, 1) # Đầu ra là 1 Con Số (Float): Giá dự đoán tương lai $Return_{t+1}$
        )

        self._initialize_weights()

    def _initialize_weights(self):
        """
        Khảm Tướng Nhận Diện Trọng Số Phân Nhánh Uniform (Xavier/Glorot Initialization).
        TẠI SAO: Giải quyết vấn đề Phương Sai của Output đầu không bị bùng nổ khi vượt qua lớp Deep network (Bảo toàn tỷ lệ scale).
        """
        for m in self.modules():
            if isinstance(m, nn.Linear):
                nn.init.xavier_uniform_(m.weight)
                if m.bias is not None:
                    nn.init.constant_(m.bias, 0)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        """
        Đường Xích Vi Xử Lý Truyền Thẳng Chân Chuyên Sâu Của Mạng Não D-Model.
        [B, Window_Step, Asset_Qty, Indicator_Qty]
        """
        B, W, A, F = x.shape

        # MỘT QUY TẮC HIẾN PHÁP CHÉO RÃNH (Channel Independence): 
        # Tách Asset Dimension đập mạnh kết nối vào Batch Dimension.
        # Nghĩa là thay vì model phải suy nghĩ việc học chép cặp ETH_BTC, thì nay Model chỉ học mỗi giá trị Đơn chất Time Series Thuần theo W.
        # Size Biến Chuyển: (B, W, A, F) -> Transpose (B, A, W, F) -> Ép Nhấn Kẹp (B*A, Window_T, Features_O)
        x = x.transpose(1, 2).reshape(B * A, W, F)

        # Kỹ Thuật Đúc Khuôn Bản RevIN (Reversible Instance Normalization) 
        # Cực kỳ Quyết đoán: Nó cưỡng bách giá đóng gói Tĩnh của Data Window bay thẳng về Normalize Standard Normal, cạo trọc toàn bộ Base Line Noise.
        mean = x.mean(dim=1, keepdim=True)
        std = x.std(dim=1, keepdim=True) + 1e-8
        x = (x - mean) / std

        # Map vào Core Space Linear
        x = self.input_projection(x)
        x = self.pos_encoder(x)

        # Chạy Qua Động Cơ Mạng Transformer Encoder
        x = self.transformer(x)

        # Chỉ trích Lấy Vị Trí Nhận Diện Nến Thông Minh Cuối (Contextualized Embeddings of "Last Know Timestep")
        x = x[:, -1, :] 
        x = self.final_norm(x)

        # Hồi Biến Dự Đoán Về Vector Tài Sản
        out = self.output_head(x)
        return out.view(B, A)

class Forecaster:
    """Mấu Kết (Wrapper Engine) Bọc Kín Logic AI Cấu Hình Phỏng Inference Python Gọi Ra. Dùng chuẩn của BaseAgent về sau."""
    def __init__(self, n_features: int, d_model: int = 128, model_path: Optional[str] = None):
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model = TimeSeriesTransformer(n_features=n_features, d_model=d_model).to(self.device)

        if model_path:
            try:
                self.model.load_state_dict(torch.load(model_path, map_location=self.device))
                print(f"[Core Predictor] Tiêm Dịch Mã Bộ Chú Model từ Trụ Lôi {model_path}.")
            except Exception as e:
                print(f"[Core Predictor Cảnh Báo Lõi] Mất Liên Kết File Model PT. Reset Về Non-Trained State. {e}")

        self.model.eval() # Ép Hard Constraint Tắt chế độ Dropout/Báckprop khi Inference

    def predict(self, observation: np.ndarray) -> np.ndarray:
        """Kênh truyền Nạp Numpy Thực Thể Vào Vector D-tensor. O(Window) Query."""
        with torch.no_grad():
            x_tensor = torch.from_numpy(observation).float().unsqueeze(0).to(self.device)
            predictions = self.model(x_tensor)
            return predictions.squeeze(0).cpu().numpy()

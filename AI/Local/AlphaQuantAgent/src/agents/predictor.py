"""
/**
 * TỆP AI (AI MODULE): THE TRANSFORMER FORECASTER (BỘ TIÊN TRI CHUỖI THỜI GIAN)
 * ============================================================================
 * CÂU HỎI 1: Tệp này có lý do gì để tồn tại?
 * -> Vai trò: Mạng Transformer sử dụng Pre-LayerNorm Architecture để dự báo chuỗi thời gian nhiễu cực cao (High-Noise Time Series). Nó giải quyết nhược điểm "mù phương hướng trong dài hạn" của RL PPO. PPO chỉ ra Lệnh, nhưng Transformer thì dự báo Tương Lai.
 * 
 * CÂU HỎI 2: Đầu vào (Input) của hệ thống là gì?
 * -> Tuple: `(Batch_B, Window_W, Assets_A, Features_F)`. Một khối không thời gian trượt (Sliding Window Tensor). Tức là Transformer được phép "nhìn lại" quá khứ `W` nến.
 * 
 * CÂU HỎI 3: Đầu ra (Output) xuất đi đâu?
 * -> Xuất ra Mảng Điểm Số (Score Array). Điểm này tương tự XGBoost, được Market Engine thu thập và tiêm thẳng vào khối Tensor (feature `lstm_pred`) trước khi PPO nhúng tay vào ra quyết định.
 * -> Complexity: Trọng tâm O(B * A * W^2 * D_Model), rất nặng VRAM do cơ chế Self-Attention.
 */
"""

import torch
import torch.nn as nn
import numpy as np
import math
from typing import Optional

class PositionalEncoding(nn.Module):
    """
    [CHỨC NĂNG CỐT LÕI]: Tiêm Mã Tọa Độ Tuần Hoàn (Sinusoidal Positional Encoding).
    [TẠI SAO TỒN TẠI]: Cơ chế Self-Attention là "vô tri" về mặt thứ tự (Permutation Invariant). Nếu không có hàm lượng giác Cos/Sin chèn vào, dữ liệu nến số 1 và nến số 100 sẽ được xem là như nhau đối với Transformer.
    [CƠ CHẾ]: Tự động sinh tọa độ hình sin cộng (add) đè lên dữ liệu Data (T, F).
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
        [CHỨC NĂNG CỐT LÕI]: Forward Propagation (Tiến truyền).
        [ĐẦU VÀO]: `x` hình khối `(Batch_size, Window_Length, Num_Assets, Features)`.
        [QUY TẮC ĐỘC LẬP KÊNH (Channel Independence)]: 
        Ép dẹp `Assets` vào chung với `Batch` -> Biến hình thành `(B*A, Window, Features)`. Hệ quả: Model biến thành "Kẻ mù đặc" về từng mã riêng biệt (như BTC hay ETH). Nó chỉ hiểu bản chất của Dòng Thời Gian Trìu tượng (Pure Time Series), miễn nhiễm hoàn toàn sự đứt gãy tương quan cục bộ.
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

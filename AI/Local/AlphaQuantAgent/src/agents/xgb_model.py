"""
/**
 * MODULE: System Booster (Kẻ Xếp Hạng Ánh Sáng)
 * VAI TRÒ: Ứng dụng Cây Quyết định Tối ưu Hóa Gradient (XGBoost/LightGBM). Chuyên gia đánh giá dữ liệu bảng (Tabular/Macro Stats) 
 * mà Mạng Neural thường xử lý kém cỏi. Nó trả về "Ranking Score" sức mạnh của tài sản.
 * CƠ CHẾ: Đặt tính năng này cạnh chuỗi TimeSeries để Tác tử RL (PPO) nhúng vào như một chỉ báo cực mạnh.
 */
"""
import numpy as np
import xgboost as xgb
import os
from typing import Optional, Any
from src.agents.base_agent import BaseAgent

class RankingBooster(BaseAgent):
    def __init__(self, model_path: Optional[str] = None):
        """Khởi tạo Hạt nhân Boosting bằng thuật toán Gradient."""
        # Dùng XGBRegressor để chấm điểm hồi quy (Regression Score - Tượng trưng cho Alpha rủi ro)
        self.model = xgb.XGBRegressor(
            objective='reg:squarederror',
            max_depth=3,          # Cây Nông: Tránh Học vẹt (Overfitting) dữ liệu nhiễu cao
            learning_rate=0.05,   # Adagrad Bước Nhảy Nhỏ: Giúp cây hội tụ chậm nhưng bền vững
            n_estimators=100
        )
        self.model_path = model_path
        self.is_trained = False
        
        if model_path and os.path.exists(model_path):
            self.load(model_path)

    def predict(self, observation: np.ndarray) -> np.ndarray:
        """
        Giao thức truy vấn.
        TÓM TẮT: Observation của ta hiện đang là 3D (Window_L, Asset_N, Feat_F).
        Tuy nhiên XGBoost chỉ ăn Mảng Vô hướng 2D. Do đó, ta chỉ trích xuất hàng Nến mới nhất (Last Timestep).
        """
        # Cắt lấy nhát dao thời gian cuối cùng: (N, F)
        latest_slice = observation[-1, :, :]
        
        if not self.is_trained:
            # Fallback ngây thơ: Trả về Rank trung tính 0.0 nếu chưa Train
            return np.zeros(latest_slice.shape[0])

        # Dự phóng (Inference)
        scores = self.model.predict(latest_slice)
        return scores

    def save(self, path: str):
        """Cố định rễ cây (Tree State)."""
        self.model.save_model(path)

    def load(self, path: str):
        """Đánh thức các Nút quyết định (Decision Knots) từ Json/Bin."""
        self.model.load_model(path)
        self.is_trained = True
        
    def fit(self, X_train: np.ndarray, y_train: np.ndarray):
        """Hàm huấn luyện ngoại lệ chuyên biệt (Off-policy Training Mode)."""
        self.model.fit(X_train, y_train)
        self.is_trained = True

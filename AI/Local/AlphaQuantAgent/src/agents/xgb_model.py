"""
/**
 * TỆP AI (AI MODULE): THE XGBOOST RANKING BOOSTER (HỆ XẾP HẠNG)
 * =================================================================================
 * CÂU HỎI 1: Tệp này có lý do gì để tồn tại?
 * -> Vai trò: Các bảng dữ liệu (Tabular/Macro Stats) chứa nhiễu khổng lồ. Mạng Nơron (RL/LSTM) thường bị Overfitting khi học dạng này. XGBoost với thuật toán Gradient Tree Boosting là sát thủ số 1 với dữ liệu Tabular. Nó tồn tại để chấm một điểm số "Ranking" dự phòng rủi ro tài sản.
 * 
 * CÂU HỎI 2: Đầu vào (Input) của hệ thống là gì?
 * -> Nhát cắt Dữ liệu 2D (Chỉ xét dòng Mới Nhất của Tensor) vì Cây Quyết Định không có trí nhớ tuần tự như LSTM.
 * 
 * CÂU HỎI 3: Đầu ra (Output) xuất đi đâu?
 * -> Xuất ra một Vector 1D (Điểm số từ -1 đến 1 cho mỗi Tài sản).
 * -> Điểm số này sẽ được `market.py` bơm ngược lại vào Tensor 3D gốc thành Feature `xgboost_score` để Trí tuệ PPO đọc trên mỗi step!
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
        [CHỨC NĂNG CỐT LÕI]: Dự phóng (Inference) điểm số hồi quy.
        [ĐẦU VÀO]: Quan sát 3D đầy đủ `(L, N, F)`. Cắt lấy nhát dao thời gian cuối cùng `[-1, :, :]`.
        [ĐẦU RA]: Mảng 1D array dự báo chiều hướng (Alpha). Nếu mô hình chưa học (is_trained=False), hệ thống ngầm định mảng Zero trung tính để PPO không bị nhiễu do điểm rác.
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

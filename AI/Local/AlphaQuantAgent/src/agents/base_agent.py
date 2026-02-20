"""
/**
 * MODULE: AI Base Abstract Structure
 * VAI TRÒ: Giao thức Tiêu chuẩn Cốt lõi (The Interface Protocol).
 * TẠI SAO PHẢI CÓ: Áp dụng chữ D (Dependency Inversion Principle) trong SOLID. 
 * Ép buộc tất cả các Đơn vị Trí tuệ (Predictor, Optimizer, Booster, Trader) phải tuân theo 
 * một hợp đồng chung (Contract). Framework Engine (như Simulator) chỉ nói chuyện với BaseAgent,
 * nó không cần biết bên dưới là PyTorch (LSTM), Scikit (XGBoost) hay Stable-Baselines (PPO).
 */
"""
from abc import ABC, abstractmethod
import numpy as np
from typing import Any

class BaseAgent(ABC):
    
    @abstractmethod
    def predict(self, observation: np.ndarray) -> Any:
        """
        Giao lộ Inference Hành vi.
        Trị số truyền vào luôn luôn là Numpy Array (State Tensor).
        Trả về kết quả (Weights, Dự đoán giá, hoặc Action).
        """
        pass

    @abstractmethod
    def save(self, path: str):
        """Khóa cứng tri thức (Dumb Weights/Pickle/PyTorch pth) và xuất ra ổ cứng đĩa vật lý."""
        pass
        
    @abstractmethod
    def load(self, path: str):
        """Khôi phục Ký ức (Awakening). Bơm Weights từ tệp tĩnh vào lại mầm Mạng Nơ-ron ảo."""
        pass

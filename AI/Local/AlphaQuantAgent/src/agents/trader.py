"""
/**
 * MODULE: System Execution Trader (Central Brain)
 * VAI TRÒ: Tác tử Học Tăng Cường (Reinforcement Learning - PPO). Đại diện cho ý chí cốt tủy đưa ra lệnh phán quyết cuối cùng (Mua/Bán bao nhiêu).
 * CƠ CHẾ KHẮC KHỔ: Không trực tiếp dự báo giá, RL Trader nhìn vào Vector Dự báo của Forecaster, Vector Chấm điểm Ranking của Booster, 
 * và Bản đồ Tỷ trọng Tiêu chuẩn của HRP Optimizer để "Pha trộn" (Blend) ra Action Weights Tối Ưu, mục đích tối đa hóa Hàm Phần thưởng Reward.
 */
"""
import numpy as np
import os
from typing import Optional, Any
from stable_baselines3 import PPO
from src.agents.base_agent import BaseAgent
from src.engine.env import AlphaQuantEnv

class RLTrader(BaseAgent):
    def __init__(self, env: AlphaQuantEnv, model_path: Optional[str] = None):
        """Khắc Nhập Môi Trường Gymnasium."""
        self.env = env
        self.model_path = model_path
        
        # Proxy Policy Setup: Mạng Nơron diễn dịch Đa lớp Nhận Thức (MLP)
        policy_kwargs = dict(
            net_arch=dict(pi=[128, 128], vf=[128, 128]) # Tầng Nhận Diện (Actor) và Tầng Định Giá (Critic)
        )
        
        # PPO: Proximal Policy Optimization - Giải thuật Clipped Surrogate siêu việt cân bằng giữa tốc độ và độ bền vững.
        self.model = PPO(
            "MlpPolicy",
            self.env,
            learning_rate=0.0003,
            n_steps=2048,
            batch_size=64,
            gamma=0.99, # Hệ số chiết khấu phần thưởng: Nhìn về lợi ích tương lai
            clip_range=0.2, # Lõi Toán PPO: Chặn ko cho Model thay đổi não bộ quá vội vàng trong 1 đợt backprop
            tensorboard_log=None, # Tắt cơ chế tự đẻ thư mục _1, _2 của SB3
            policy_kwargs=policy_kwargs,
            device="auto"
        )
        
        # CHỐT CHẶN: Ép SB3 ghi đè vào duy nhất 1 thư mục 'ppo', cấm tự sinh index
        from stable_baselines3.common.logger import configure
        new_logger = configure("logs/training/tensorboard/ppo", ["stdout", "tensorboard"])
        self.model.set_logger(new_logger)
        
        self.is_trained = False
        if model_path and os.path.exists(model_path):
            self.load(model_path)

    def predict(self, observation: np.ndarray) -> np.ndarray:
        """
        Thực thi Chính sách Chết (Deterministic Policy).
        LƯU Ý: Tại trạng thái Deploy Backtest, deterministic=True KHÓA cứng Exploration (Thăm dò ngẫu nhiên), ép Model xuất chiêu mà nó tin là mạnh nhất.
        """
        # Nếu chưa học thì ngây thơ trích Default Action Space (0.5 cho tiện)
        if not self.is_trained:
            action = self.env.action_space.sample()
            return action

        # PPO.predict trả về 1 Tuple (Action Array, Tọa độ State Phụ Trợ)
        action, _states = self.model.predict(observation, deterministic=True)
        return action

    def learn(self, total_timesteps: int, callbacks: Optional[list] = None):
        """Mệnh lệnh đắm chìm vào kỷ nguyên huấn luyện. Kích hoạt Backpropagation."""
        self.model.learn(total_timesteps=total_timesteps, callback=callbacks)
        self.is_trained = True

    def save(self, path: str):
        """Khóa ZIP các lớp PyTorch Checkpoint Weight."""
        self.model.save(path)

    def load(self, path: str):
        """Load bằng cơ chế Override của Library Class."""
        self.model = PPO.load(path, env=self.env)
        self.is_trained = True

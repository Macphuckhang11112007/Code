import os
import json
import numpy as np
from stable_baselines3.common.callbacks import BaseCallback

class QuantTelemetryCallback(BaseCallback):
    def __init__(self, log_path: str, verbose=1):
        super(QuantTelemetryCallback, self).__init__(verbose)
        self.log_path = log_path
        self.training_history = []
        os.makedirs(os.path.dirname(self.log_path), exist_ok=True)
        # Tạo file rỗng nếu chưa có
        with open(self.log_path, 'w') as f:
            json.dump([], f)

    def _on_step(self) -> bool:
        # Chỉ ghi log sau mỗi X steps (ví dụ: mỗi 100 steps) để tránh nặng máy
        if self.num_timesteps % 100 == 0:
            # RÚT RUỘT LOGGER CỦA SB3 BẰNG EXACT KEYS:
            # Bắt buộc dùng self.logger.name_to_value để moi Loss thực tế!
            policy_loss = self.logger.name_to_value.get("train/policy_gradient_loss", 0.0)
            value_loss = self.logger.name_to_value.get("train/value_loss", 0.0)
            entropy = self.logger.name_to_value.get("train/entropy_loss", 0.0)
            mean_reward = self.logger.name_to_value.get("rollout/ep_rew_mean", 0.0)

            # RÚT DATA TỪ ENV (thông qua mảng infos)
            # locals() chứa biến 'infos' từ hàm step của env
            infos = self.locals.get("infos", [{}])
            latest_info = infos[0] if len(infos) > 0 else {}
            
            actions_stats = latest_info.get("action_history", {"buy": 0, "sell": 0, "hold": 0})
            penalties = latest_info.get("penalty_stats", {"illegal_moves": 0, "inactivity_hits": 0})

            # Đóng gói 1 Data Point
            data_point = {
                "step": self.num_timesteps,
                "metrics": {
                    "policyLoss": float(policy_loss),
                    "valueLoss": float(value_loss),
                    "entropy": float(entropy),
                    "meanReward": float(mean_reward)
                },
                "actions": actions_stats,
                "penalties": penalties
            }
            
            self.training_history.append(data_point)

            # Ghi nối tiếp vào file JSON an toàn
            try:
                with open(self.log_path, 'w') as f:
                    json.dump(self.training_history, f)
            except Exception as e:
                pass # Bỏ qua nếu đang khóa file

        return True

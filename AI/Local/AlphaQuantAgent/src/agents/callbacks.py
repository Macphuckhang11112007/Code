"""
/**
 * MODULE: Training Supervisor Callbacks
 * VAI TRÒ: "Giám thị Kỷ luật" (Supervisor). Can thiệp ngang xương vào quy trình học để Ghi hình TensorBoard và Buộc Model dừng học (Early Stopping).
 * TẠI SAO: Giải Tỏa Nút Thắt - Chặn đứng quá trình Train Vô hạn khi Hàm Loss đã bão hòa để tiết kiệm Điện phần cứng chống Overfitting.
 */
"""
import os
import numpy as np
from stable_baselines3.common.callbacks import BaseCallback

class EarlyStoppingAndLogging(BaseCallback):
    """Callback chọc thủng vòng lặp Learn loop ngàn kiếp của RL."""
    def __init__(self, check_freq: int, log_dir: str, model_save_path: str = "models/rl_agent/best_model.zip", verbose: int = 1):
        super(EarlyStoppingAndLogging, self).__init__(verbose)
        self.check_freq = check_freq
        self.log_dir = log_dir
        self.save_path = model_save_path
        self.best_mean_reward = -np.inf
        
        if not os.path.exists(self.log_dir):
            os.makedirs(self.log_dir, exist_ok=True)
            
        save_dir = os.path.dirname(self.save_path)
        if not os.path.exists(save_dir):
            os.makedirs(save_dir, exist_ok=True)

    def _init_callback(self) -> None:
        """Kịch bản khởi động Logging."""
        pass

    def _on_step(self) -> bool:
        """
        Đánh giá Hiệu Quỳ tại mỗi N Bước thời gian. 
        PHỨC TẠP: Xử lý O(1) Fetch Queue RAM trong Gym.
        """
        if self.n_calls % self.check_freq == 0:
            # Tra cứu bảng History Reward của 100 Episodes gần nhất
            ep_info_buffer = self.model.ep_info_buffer
            if len(ep_info_buffer) > 0:
                mean_reward = np.mean([ep_info["r"] for ep_info in ep_info_buffer])
            else:
                # ÉP BUỘC GHI LOG: Nếu episode chưa kết thúc (vì số epochs quá ngắn), lấy tạm reward của step hiện tại
                rewards = self.locals.get("rewards", [0.0])
                mean_reward = np.mean(rewards)
                
            # So sánh Đỉnh Toán Học
            if mean_reward > self.best_mean_reward:
                self.best_mean_reward = mean_reward
                # Ghi nháp Snapshot tạm (Checkpoint)
                self.model.save(self.save_path)
                
                if self.verbose > 0:
                    print(f"| GIÁM SÁT | Bước {self.n_calls} - Đã ghi nhận Đỉnh Reward Mới: {mean_reward:.4f}. Xuất File !")
            
            # Bắt buộc Ghi Log Metrics ra CSV để UI Learning Monitor đọc được
            csv_path = os.path.join(self.log_dir, 'training_metrics.csv')
            write_header = not os.path.exists(csv_path)
            with open(csv_path, 'a') as f:
                if write_header: f.write("step,reward\n")
                f.write(f"{self.n_calls},{mean_reward}\n")
            
            # Ghi Log chi tiết AI Dynamics cho UI Matrix
            dyn_path = os.path.join("logs/trading", "ai_dynamics_log.csv")
            os.makedirs(os.path.dirname(dyn_path), exist_ok=True)
            dyn_header = not os.path.exists(dyn_path)
            
            # Trích xuất từ logger của SB3
            p_loss = self.logger.name_to_value.get("train/policy_gradient_loss", 0.0)
            v_loss = self.logger.name_to_value.get("train/value_loss", 0.0)
            ent_loss = self.logger.name_to_value.get("train/entropy_loss", 0.0)
            
            with open(dyn_path, 'a') as f:
                if dyn_header: f.write("step,policy_loss,value_loss,entropy\n")
                f.write(f"{self.n_calls},{p_loss},{v_loss},{ent_loss}\n")
                    
        return True # Trả True để vòng lặp Learn không bị Crash ngang

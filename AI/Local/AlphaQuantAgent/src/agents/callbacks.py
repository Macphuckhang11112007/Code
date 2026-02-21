"""
/**
 * MODULE: Training Supervisor Callbacks
 * VAI TRÒ: "Giám thị Kỷ luật" (Supervisor). Can thiệp ngang xương vào quy trình học để Ghi hình TensorBoard và Buộc Model dừng học (Early Stopping).
 * TẠI SAO: Giải Tỏa Nút Thắt - Chặn đứng quá trình Train Vô hạn khi Hàm Loss đã bão hòa để tiết kiệm Điện phần cứng chống Overfitting.
 */
"""
import os
import time
import datetime
import numpy as np
import json
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
            
        self.start_time = None
        self.stream_log_path = "logs/trading/training_stream.log"
        os.makedirs(os.path.dirname(self.stream_log_path), exist_ok=True)

    def _init_callback(self) -> None:
        """Kịch bản khởi động Logging."""
        self.start_time = time.time()
        # Reset stream log file
        with open(self.stream_log_path, 'w') as f:
            f.write("Khởi động quá trình huấn luyện AlphaQuant...\n")

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
            
            # Extract metrics
            p_loss = self.logger.name_to_value.get("train/policy_gradient_loss", 0.0)
            v_loss = self.logger.name_to_value.get("train/value_loss", 0.0)
            ent_loss = self.logger.name_to_value.get("train/entropy_loss", 0.0)
            total_loss = p_loss + v_loss
            
            with open(dyn_path, 'a') as f:
                if dyn_header: f.write("step,policy_loss,value_loss,entropy\n")
                f.write(f"{self.n_calls},{p_loss},{v_loss},{ent_loss}\n")
                
            # --- REAL TRAINING HISTORY JSON FOR UI ---
            history_path = os.path.join("logs/trading", "training_history.json")
            history_data = []
            if os.path.exists(history_path):
                try:
                    with open(history_path, 'r', encoding='utf-8') as f:
                        history_data = json.load(f)
                except Exception:
                    pass
            
                
            # --- REAL-TIME LOGGING FORMAT AS REQUIRED ---
            total_steps = self.locals.get("total_timesteps")
            if total_steps is None:
                # Fallback if not available
                total_steps = getattr(self.model, "num_timesteps", getattr(self.model, "_total_timesteps", self.n_calls * 10))

            # ETA Calculation
            elapsed = time.time() - self.start_time
            steps_left = total_steps - self.n_calls
            if self.n_calls > 0:
                time_per_step = elapsed / self.n_calls
                eta_seconds = steps_left * time_per_step
                eta_str = str(datetime.timedelta(seconds=int(eta_seconds)))
            else:
                eta_str = "Calculating..."
                
            # Trích xuất dữ liệu sâu từ Info Buffer
            win_rate = 50.0
            avg_pnl = 0.0
            avg_step_illegal = 0
            avg_inactivity = 0
            
            # Tính toán Action Distribution từ bộ nhớ Action (Nếu đang trong episode)
            buy_rate, sell_rate, hold_rate = 0.0, 0.0, 100.0 

            # Chúng ta dùng infos từ tự nhiên Environment Gym bắn lên
            infos = self.locals.get("infos", [])
            
            if len(infos) > 0:
                # Bóc tách Data từ dictionary info mà simulator.py vừa trả về
                last_info = infos[-1] # Info của env 0 tại step gần nhất
                
                avg_pnl = last_info.get('realized_nav_pct', 0.0) * 100.0
                total_illegal = last_info.get('illegal_moves_total', 0)
                avg_inactivity = last_info.get('inactivity_hits', 0)
                
                trade_counts = last_info.get('trade_counts', {'buy': 0, 'sell': 0, 'hold': 0})
                
                total_trades = trade_counts['buy'] + trade_counts['sell'] + trade_counts['hold']
                if total_trades > 0:
                    buy_rate = (trade_counts['buy'] / total_trades) * 100
                    sell_rate = (trade_counts['sell'] / total_trades) * 100
                    hold_rate = (trade_counts['hold'] / total_trades) * 100
            
            # Tính WinRate từ Episodic Buffer
            if len(ep_info_buffer) > 0:
                wins = sum(1 for ep in ep_info_buffer if ep["r"] > 0)
                win_rate = (wins / len(ep_info_buffer)) * 100

            # Append new data point to JSON History
            history_data.append({
                "epoch": self.n_calls,
                "mean_reward": float(mean_reward),
                "policy_loss": float(p_loss),
                "value_loss": float(v_loss),
                "entropy": float(ent_loss),
                "action_probabilities": {
                    "time": datetime.datetime.now().strftime("%H:%M"),
                    "buy": float(buy_rate),
                    "sell": float(sell_rate),
                    "hold": float(hold_rate)
                }
            })
            
            # Keep the history reasonably sized
            if len(history_data) > 2000:
                history_data = history_data[-2000:]
                
            with open(history_path, 'w', encoding='utf-8') as f:
                json.dump(history_data, f, indent=4)

            # In bảng Thống Kê Ultra-Detailed (The Omniverse Log)
            clear_screen = "\033[2J\033[H" if self.n_calls % (self.check_freq * 10) == 0 else "" # Clean up terminal rarely to prevent scrolling mess
            
            log_block = f"""
{clear_screen}--------------------------------------------------------------------
[Epoch {self.n_calls}/{total_steps}] | ETA: {eta_str}
► Performance : Net PnL: {avg_pnl:+.4f}% | Step Reward: {mean_reward:+.4f} | WinRate: {win_rate:.0f}%
► Actions     : Buy: {buy_rate:.1f}% | Sell: {sell_rate:.1f}% | Hold/Cash: {hold_rate:.1f}%
► Penalties   : Illegal Moves: {total_illegal if len(infos)>0 else 0} | Inactivity Hits: {avg_inactivity}
► Brain Stats : Value Loss: {v_loss:.3f} | Policy Loss: {p_loss:.3f} | Entropy: {ent_loss:.3f}
--------------------------------------------------------------------"""
            
            # In đè lên màn hình Terminal
            print(log_block, flush=True)
            
            # Stream to file (Lưu log ngang thay vì block để tiết kiệm ổ cứng)
            with open(self.stream_log_path, 'a') as f:
                squashed_log = f"[Epoch {self.n_calls}/{total_steps}] PnL: {avg_pnl:+.2f}% | Rwd: {mean_reward:+.2f} | B/S/H: {buy_rate:.0f}/{sell_rate:.0f}/{hold_rate:.0f} | Illegal: {total_illegal if len(infos)>0 else 0} | Inact: {avg_inactivity} | VLoss: {v_loss:.3f} | Ent: {ent_loss:.3f}"
                f.write(squashed_log + "\n")
                    
        return True # Trả True để vòng lặp Learn không bị Crash ngang

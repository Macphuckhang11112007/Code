"""
/**
 * MODULE: Engine Wrapper (Gymnasium Environment)
 * VAI TRÒ: Chuẩn hóa thế giới vật lý thành Giao diện API Mở chuẩn để thích ứng (Compatible) với các thư viện AI mã nguồn mở truyền thống (như Stable Baselines3).
 * CƠ CHẾ: Bọc lớp TradingSimulator lại bên trong các khái niệm action_space và observation_space.
 * TẠI SAO PHẢI CÓ MODULE NÀY: Thuật toán Tối ưu Hóa Chính sách Cận kề (PPO) hoàn toàn không hiều Wallet tỷ giá, 
 * Market Spread trượt giá là cái gì. Nó chỉ biết giao tiếp bằng Vector Toán học dạng Enum Box(Low, High, Shape).
 */
"""
import gymnasium as gym
import numpy as np
from gymnasium import spaces
from typing import Dict, List, Any
from src.engine.simulator import TradingSimulator

class AlphaQuantEnv(gym.Env):
    """
    Khuôn Đúc Môi Trường RL (Custom Environment Gymnasium).
    Kế thừa kiến trúc API Tiêu chuẩn Công nghiệp từ tổ chức OpenAI.
    """
    def __init__(self, simulator: TradingSimulator):
        super(AlphaQuantEnv, self).__init__()
        self.sim = simulator
        
        # Mapping Index (Int) sang Symbol Ticker (String)
        self.asset_list = self.sim.market.asset_list
        num_assets = len(self.asset_list)
        
        # ================= KHÔNG GIAN HÀNH ĐỘNG (ACTION SPACE) =================
        # Một Khối Hình (Box) Box Vector có chiều dài bằng tổng số mã tài sản.
        # Đội AI tạo ra tỷ trọng trong dải liên tục. Softmax sẽ được kẹp ở layer tiếp theo vào khoảng (0, 1).
        self.action_space = spaces.Box(low=0.0, high=1.0, shape=(num_assets,), dtype=np.float32)

        # ================= KHÔNG GIAN NHẬN THỨC (OBSERVATION SPACE) =================
        # Hình phẳng 3D Tensor Cửa sổ Không - Thời Gian:
        # Số Lượng (Window Timesteps) x Cột Cấu Trúc (Assets Mã) x Trục Sâu (Feature Indicators)
        obs_shape = (
            self.sim.window_size,
            num_assets,
            len(self.sim.market.feature_map)
        )
        # Giới hạn an toàn từ Âm Vô Cực đến Dương Vô Cực. (Data thực tế đã được ép (Scale) qua Z-Score nên dao động quanh -5 đến 5).
        self.observation_space = spaces.Box(low=-np.inf, high=np.inf, shape=obs_shape, dtype=np.float32)

    def reset(self, seed=None, options=None):
        """Tiến hành Reseed lại ma trận ngẫu nhiên, đưa vòng đời RL agent bắt đầu lại từ nến khởi điểm."""
        super().reset(seed=seed)
        obs = self.sim.reset()
        
        # Gymnasium >0.26 API yêu cầu trả kết hợp Tuple (Observation, Thêm Error Params/Info Dict)
        return obs, {}

    def step(self, action: np.ndarray) -> tuple:
        """
        Giao Cắt Định Mệnh (The Collision Point) Giữa Ma Trận AI Và Logic Tài Chính Kế Toán.
        TÓM TẮT: AI truyền Array (Tỷ trọng thô) -> Hàm kẹp nó thành xác suất Softmax chuẩn -> Từ điển action_dict -> Xuống Simulator -> Return Reward + Penalty.
        """
        # CỞ TRỊ VĂN BẢN (Clip Bounds): Chống vụ nổ đạo hàm lỡ PPO sinh con số bàng hoang (-99) ngoài bounds chặn Box
        action = np.clip(action, 0.0, 1.0)
        
        # Xác suất Hóa (Softmax Scaling Mechanism): Triệt tiêu hoàn toàn trường hợp AI output mảng trọng số có tổng khác 1
        sum_action = np.sum(action)
        if sum_action > 0:
            action = action / sum_action
            
        action_dict = {
            self.asset_list[i]: float(action[i]) for i in range(len(self.asset_list))
        }

        obs, reward, done, info = self.sim.step(action_dict)

        # Báo hiệu ép TimeLimit của RL API Mới nhất (Gym >= 0.26) (Truncated nếu epoch hết giờ chứ ko chết)
        truncated = False 
        return obs, reward, done, truncated, info

    def render(self, mode="human"):
        """Giao diện mô phỏng 2D cho nhà nghiên cứu. Tạm thời Bỏ qua (Pass) vì ta đã trích báo cáo ra Streamlit Cockpit Web UI tĩnh điện."""
        pass

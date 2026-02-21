"""
/**
 * MODULE: Engine Wrapper (Gymnasium Environment)
 * VAI TRÒ: Chuẩn hóa thế giới vật lý thành Giao diện API Mở chuẩn để thích ứng (Compatible) với các thư viện AI mã nguồn mở truyền thống (như Stable Baselines3).
 * CƠ CHẾ: Bọc lớp TradingSimulator lại bên trong các khái niệm action_space và observation_space. CHỈ ĐẠO CỐT LÕI: REWARD SHAPING TÀN NHẪN.
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
        # Một Khối Hình (Box) Box Vector có chiều dài bằng tổng số mã tài sản + 1 (Tiền mặt / Cash).
        # AI sẽ phải ra quyết định phân bổ tỷ trọng TẤT CẢ TÀI SẢN bao gồm cả Cash.
        self.action_space = spaces.Box(low=0.0, high=1.0, shape=(num_assets,), dtype=np.float32)

        # ================= KHÔNG GIAN NHẬN THỨC (OBSERVATION SPACE) =================
        # Hình phẳng 3D Tensor Cửa sổ Không - Thời Gian:
        # Số Lượng (Window Timesteps) x Cột Cấu Trúc (Assets Mã) x Trục Sâu (Feature Indicators)
        ts_shape = (
            self.sim.window_size,
            num_assets,
            len(self.sim.market.feature_map) if hasattr(self.sim.market, 'feature_map') else 25 # Fallback 25 features
        )
        
        # Biến số tĩnh của Persona là 27 + metrics + allocations for each asset
        static_shape = (29 + num_assets,)
        
        self.observation_space = spaces.Dict({
            'ts_data': spaces.Box(low=-np.inf, high=np.inf, shape=ts_shape, dtype=np.float32),
            'static_data': spaces.Box(low=-np.inf, high=np.inf, shape=static_shape, dtype=np.float32)
        })

        # Theo dõi các mốc phạt để debug
        self.total_penalties = 0.0
        self.current_step = 0
        self.net_worth = getattr(self.sim, 'initial_capital', 100000)
        self.action_history = {"buy": 0, "sell": 0, "hold": 0}
        self.penalty_stats = {"illegal_moves": 0, "inactivity_hits": 0}
        self.portfolio_history = [] # Lưu mảng giá trị tài sản ròng

    def reset(self, seed=None, options=None):
        """Tiến hành Reseed lại ma trận ngẫu nhiên, đưa vòng đời RL agent bắt đầu lại từ nến khởi điểm."""
        super().reset(seed=seed)
        obs = self.sim.reset()
        
        info = {
            "final_net_worth": getattr(self, 'net_worth', getattr(self.sim, 'initial_capital', 100000)),
            "action_history": self.action_history.copy() if hasattr(self, 'action_history') else {"buy": 0, "sell": 0, "hold": 0},
            "penalty_stats": self.penalty_stats.copy() if hasattr(self, 'penalty_stats') else {"illegal_moves": 0, "inactivity_hits": 0},
            "portfolio_history": self.portfolio_history.copy() if hasattr(self, 'portfolio_history') else []
        }
        
        self.total_penalties = 0.0
        self.current_step = 0
        self.net_worth = getattr(self.sim, 'initial_capital', 100000)
        self.action_history = {"buy": 0, "sell": 0, "hold": 0}
        self.penalty_stats = {"illegal_moves": 0, "inactivity_hits": 0}
        self.portfolio_history = [] # Lưu mảng giá trị tài sản ròng
        
        # Gymnasium >0.26 API yêu cầu trả kết hợp Tuple (Observation, Thêm Error Params/Info Dict)
        return obs, info

    def step(self, action: np.ndarray) -> tuple:
        """
        Giao Cắt Định Mệnh (The Collision Point) Giữa Ma Trận AI Và Logic Tài Chính Kế Toán.
        TÓM TẮT: Áp dụng REWARD SHAPING cực độ tàn nhẫn để triệt tiêu The Do-Nothing Paradox.
        """
        # 1. BIẾN ĐỔI HÀNH ĐỘNG (Action Transformation)
        action = np.clip(action, 0.0, 1.0)
        
        # Softmax Chuẩn Xác (Có Temperature nếu cần) thay vì chia tổng ngây ngô
        # Điều này chống trường hợp AI tống toàn bộ action = 0.0
        exp_a = np.exp(action - np.max(action)) 
        action_weights = exp_a / exp_a.sum()
        
        # Kiểm tra Persona Penalties (Lấy từ Simulator nếu có, nếu không lấy mặc định mạnh)
        persona = getattr(self.sim, 'persona', {})
        inactivity_penalty_rate = persona.get('inactivity_penalty', 0.005)
        concentration_penalty_rate = persona.get('drawdown_penalty', 1.0) * 0.01 
        max_weight = persona.get('max_weight_per_asset', 0.8)
        
        action_dict = {
            self.asset_list[i]: float(action_weights[i]) for i in range(len(self.asset_list))
        }

        # Kiểm tra xem AI có giao dịch không (Dựa vào sự thay đổi tỷ trọng hoặc volume)
        # Tạm định nghĩa: Lười biếng là khi action weights cực kỳ nghiêng về 0 cho tất cả,
        # nhưng vì Softmax nên lúc nào tổng cũng = 1.
        self.current_step += 1
        
        # 1. Đếm tỷ lệ Action thực tế
        action_idx = np.argmax(action_weights)
        if action_idx == len(self.asset_list) - 1: # Giả sử Hold (Cash)
            self.action_history["hold"] += 1
        elif action_weights[action_idx] > 0.6: 
            self.action_history["buy"] += 1
        else:
            self.action_history["sell"] += 1

        # Đưa action cho Simulator
        obs, base_reward, done, info = self.sim.step(action_dict)
        
        if hasattr(self.sim, 'wallet'):
            metrics = self.sim.wallet.get_metrics()
            self.net_worth = metrics.get('Total NAV_USD', self.net_worth)

        # 2. Xử lý phạt (Penalty Logic - Bắt buộc code thật)
        current_asset_price = 1.0
        if isinstance(obs, dict) and 'ts_data' in obs:
            current_asset_price = obs['ts_data'][-1][0][3] if len(obs['ts_data']) > 0 else 1.0
            
        illegal_move = False
        if getattr(self, 'net_worth', 100000) < current_asset_price and action_weights[0] > 0.8: # Không đủ tiền mua
            illegal_move = True
            self.penalty_stats["illegal_moves"] += 1
            base_reward -= 0.05 # Trừ điểm thẳng tay

        # 2. REWARD SHAPING (ĐIÊU KHẮC HÀM PHẦN THƯỞNG)
        shaped_reward = base_reward
        penalty = 0.0
        
        # A. Trừng Phạt Trì Trệ (Inactivity Penalty - The Do-Nothing Paradox)
        # Giả sử Simulator trả về thông số "volume_traded" trong tick này
        volume_traded = info.get('turnover_executed', 0.0) 
        if volume_traded < 1e-4:
            # Nếu khối lượng giao dịch gần như bằng 0 (Hoặc ko nắm giữ gì)
            penalty += inactivity_penalty_rate
            
        # B. Trừng Phạt Tập Trung Quá Đáng (Concentration / Overfitting Penalty)
        # Không cho phép AI cược toàn bộ 100% vào 1 tài sản trừ khi max_weight cho phép
        max_alloc = np.max(action_weights)
        if max_alloc > max_weight:
            penalty += (max_alloc - max_weight) * concentration_penalty_rate * 10.0 # x10 độ gắt
            
        # C. Trừng Phạt Biến Động Phân Bổ (Illegal/Erratic Moves)
        # Nếu AI thay đổi trọng số quá gắt liên tục -> Bị phạt phí giao dịch ở trong Simulator rồi.
        
        shaped_reward -= penalty
        self.total_penalties += penalty
        info['penalty'] = penalty
        
        # 3. Cuối hàm step, trước khi return
        self.portfolio_history.append(self.net_worth)
        
        info['action_history'] = self.action_history
        info['penalty_stats'] = self.penalty_stats
        info['portfolio_history'] = self.portfolio_history
        
        truncated = False 
        return obs, shaped_reward, done, truncated, info

    def render(self, mode="human"):
        pass

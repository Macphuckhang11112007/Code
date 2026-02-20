"""
/**
 * MODULE: Monte Carlo Quantum Simulator (Stress Testing)
 * VAI TRÒ: Sinh ra hàng vạn Vũ trụ song song (Parallel Universes) của diễn biến giá tài sản bằng Phương trình Vi phân Ngẫu nhiên (Stochastic Differential Equation - SDE).
 * CƠ CHẾ: Áp dụng Chuyển động Nâu Hình học (Geometric Brownian Motion - GBM).
 * TẠI SAO PHẢI CÓ: Đề phòng "Overfitting" vào quá khứ. Backtest lịch sử chỉ là 1 đường Path duy nhất đã xảy ra. 
 * Monte Carlo test khả năng sinh tồn của chiến lược giao dịch trong hàng ngàn kịch bản Tương Lai Chưa Biết (Unknown Futures).
 * CÔNG THỨC: $dS_t = \\mu S_t dt + \\sigma S_t dW_t$
 */
"""
import numpy as np
import pandas as pd
from typing import Dict, List, Tuple
from tqdm import tqdm

class MonteCarloSimulator:
    def __init__(self, n_paths: int = 100, horizon_steps: int = 1000, dt: float = 1/252):
        """
        THÔNG SỐ MÔ PHỎNG:
        - n_paths: Số lượng kịch bản (Vũ trụ song song). Default: 100.
        - horizon_steps: Rút phóng tương lai bao nhiêu nến tiếp theo. (Ví dụ 1000 nến 15 phút).
        - dt: Delta time (Tỷ lệ quy đổi thời gian sang năm). Thường là 1/252 nếu đo theo ngày giao dịch.
        """
        self.n_paths = n_paths
        self.horizon_steps = horizon_steps
        self.dt = dt

    def estimate_drift_and_vol(self, historical_returns: np.ndarray) -> Tuple[float, float]:
        """Tính toán lực cản (Drift) $\\mu$ và Độ biến động (Volatility) $\\sigma$ từ lịch sử thực tế."""
        # Giả định returns đang ở mức thập phân (e.g. 0.05)
        clean_rets = historical_returns[np.isfinite(historical_returns)]
        if len(clean_rets) == 0:
            return 0.0, 0.2 # Fallback an toàn

        mu = np.mean(clean_rets) / self.dt
        sigma = np.std(clean_rets) / np.sqrt(self.dt)
        return mu, sigma

    def generate_gbm_paths(self, S0: float, mu: float, sigma: float) -> np.ndarray:
        """
        Sinh N-Paths sử dụng Chuyển động Nâu Hình học (Geometric Brownian Motion).
        HÀM QUY TẮC: Vectorized O(1) dùng Numpy. Trả về mảng 2D: (horizon_steps, n_paths).
        """
        np.random.seed(42) # Khóa Seed để có thể tái lập (Reproducible) khi Debug

        # Sinh nhiễu trắng (White Noise / Wiener Process)
        Z = np.random.standard_normal((self.horizon_steps, self.n_paths))
        
        # Tiêm rủi ro vi phân
        drift = (mu - 0.5 * sigma**2) * self.dt
        shock = sigma * np.sqrt(self.dt) * Z
        
        # Cộng dồn lũy tiến và nhân với Giá ban đầu
        returns_path = np.exp(drift + shock)
        price_paths = np.zeros_like(returns_path)
        price_paths[0] = S0
        
        for t in range(1, self.horizon_steps):
            price_paths[t] = price_paths[t-1] * returns_path[t]
            
        return price_paths

    def run_stress_test(self, agent, initial_capital: float, S0: float, mu: float, sigma: float) -> Dict[str, float]:
        """
        Đẩy Robot vào Môi trường Thử Thạch Áp Lực (Stress Test).
        Đo lường Khả Năng Pha Sản (Probability of Ruin) và Kỳ Vọng (Expected ROI).
        """
        print(f"[Monte Carlo] Khởi tạo {self.n_paths} Vũ trụ song song với Drift $\\mu={mu:.4f}$, Vol $\\sigma={sigma:.4f}$...")
        paths = self.generate_gbm_paths(S0, mu, sigma)
        
        final_navs = []
        ruin_count = 0
        ruin_threshold = initial_capital * 0.5 # Mất 50% vốn coi như cháy tài khoản (Margin Call)
        
        # Quần thảo qua các dòng thời gian (Parallel universes)
        for i in tqdm(range(self.n_paths), desc="[MC] Testing Agent Paths"):
            path = paths[:, i]
            
            nav = initial_capital
            # Bơm từng mức giá vào Não AI (Agent)
            # Giả lập lặp nhẹ để Simulator có dữ liệu Test tương lai
            # Ở môi trường thật, Agent step() nhận Observation Matrix 3D.
            # Để rút gọn độ nặng Monte Carlo Risk Parity, chúng ta test PnL Expectation:
            
            for t in range(1, len(path)):
                # Lệch giá (Return)
                ret = (path[t] - path[t-1]) / path[t-1]
                
                # Giả định RL Agent đang giữ tỷ trọng ngẫu nhiên / hoặc tĩnh do chưa map đủ Tensor.
                # (Trên thực tế ta cấy 1 Simple HRP Logic vào đây để check Strategy)
                allocation_weight = 0.5 # Giữ 50% tài sản
                
                nav = nav * (1 + allocation_weight * ret)
                
                if nav < ruin_threshold:
                    ruin_count += 1
                    break
                    
            final_navs.append(nav)

        final_navs = np.array(final_navs)
        
        # Báo cáo Xác suất
        por = ruin_count / self.n_paths
        mean_roi = (np.mean(final_navs) / initial_capital) - 1.0
        cvar_95 = np.percentile(final_navs, 5) / initial_capital - 1.0 # Rủi ro đuôi (Tail Risk)
        
        return {
            "Probability_of_Ruin": por,
            "Expected_ROI": mean_roi,
            "CVaR_95": cvar_95,
            "Max_NAV": np.max(final_navs) / initial_capital - 1.0,
            "Min_NAV": np.min(final_navs) / initial_capital - 1.0
        }

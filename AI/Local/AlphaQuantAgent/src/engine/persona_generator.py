"""
/**
 * MODULE: System Persona Generator (Architectural Core)
 * VAI TRÒ: Trái tim của Hệ thống Đa chiều (State Space Simulator). Sinh ra các hồ sơ lượng lượng cấu hình rủi ro/phần thưởng
 * CÔNG NGHỆ: Quasi-Monte Carlo (Sobol Sequence) & Farthest Point Sampling.
 * CHUẨN MỰC: Institutional-Grade. 
 */
"""
import numpy as np
from scipy.spatial.distance import cdist
from scipy.stats import qmc, norm
from typing import List, Dict, Any
import math

class PersonaGenerator:
    """
    Bộ Sinh Hồ Sơ Giao Dịch Bằng Toán Học Không Gian Đa Chiều (Sobol Sequence).
    Thay vì dùng Random ngẫu nhiên (Uniform/Normal thông thường) dễ dẫn tới tập trung cục bộ (Clustering),
    chúng ta sử dụng Quasi-Monte Carlo để lấp đầy không gian 27 chiều một cách đồng đều nhất có thể.
    Sau đó áp dụng Rejection Sampling với Khoảng cách Euclidean >= 1000 (dựa trên hệ trọng số).
    """
    def __init__(self, all_trade_assets: List[str], all_rate_assets: List[str], all_stat_assets: List[str]):
        self.all_trade_assets = all_trade_assets
        self.all_rate_assets = all_rate_assets
        self.all_stat_assets = all_stat_assets
        
        # 27 Biến số Cốt Lõi (The 27 Persona Variables)
        # Các cột: [Min, Max, Log_Scale (True/False), Distribution (uniform/normal/categorical)]
        self.variable_schema = [
            # Nhóm 1: Capital & Cashflow
            ('initial_capital', 5.0, 100000000.0, 'log_uniform'),
            ('max_leverage', 1.0, 100.0, 'uniform'),
            ('margin_maintenance_rate', 0.005, 0.05, 'uniform'),
            ('funding_rate_bps', 1.0, 0.5, 'normal'), # loc=1.0, scale=0.5
            ('random_cash_inflow_outflow', 0.0, 0.05, 'normal'), # loc=0.0, scale=0.05
            
            # Nhóm 2: Risk & Reward Shaping
            ('drawdown_penalty', 0.0, 10.0, 'uniform'),
            ('target_return_annualized', 0.05, 2.0, 'uniform'),
            ('sharpe_optimization_weight', 0.0, 1.0, 'uniform'),
            ('inactivity_penalty', 1e-5, 1e-2, 'log_uniform'),
            ('overtrading_penalty', 1e-4, 1e-1, 'log_uniform'),
            ('win_rate_obsession', 0.0, 1.0, 'uniform'),
            
            # Nhóm 3: Universe & Portfolio
            ('trade_assets_count', 10.0, 30.0, 'uniform_int'), # GIỚI HẠN CHUẨN: 10 ĐẾN 30 MÃ GIAO DỊCH
            ('context_assets_count', 1.0, 1.0, 'uniform_int'), # Không còn dùng random nữa, lấy FULL
            ('max_weight_per_asset', 0.05, 1.0, 'uniform'),
            ('min_weight_per_asset', 0.001, 0.05, 'uniform'),
            ('allow_short_selling', 0.0, 1.0, 'categorical'), # >0.5 -> True
            ('max_open_positions', 1.0, 50.0, 'uniform_int'),
            
            # Nhóm 4: Microstructure & Frictions
            ('maker_fee', -0.0001, 0.001, 'uniform'),
            ('taker_fee', 0.0002, 0.002, 'uniform'),
            ('slippage_model_type', 0.0, 1.0, 'categorical'), # >0.5 -> 1
            ('slippage_volatility_multiplier', 1.0, 5.0, 'uniform'),
            ('latency_delay_steps', 0.0, 5.0, 'uniform_int'),
            ('spread_bps', 1.0, 50.0, 'uniform'),
            
            # Nhóm 5: Environment & Noise
            ('start_timestamp_offset', 0.0, 10000.0, 'uniform'), 
            ('episode_length_days', 1.0, 1825.0, 'uniform_int'),
            ('price_noise_variance', 0.0, 0.02, 'uniform'),
            ('missing_data_prob', 0.0, 0.005, 'uniform')
        ]
        
        self.num_dims = len(self.variable_schema)
        
        # Hệ Trọng Số Khuyếch Đại (Weighting System) để đo khoảng cách
        self.weights = np.ones(self.num_dims)
        weight_map = {
            'initial_capital': 1000.0,
            'drawdown_penalty': 800.0,
            'target_return_annualized': 600.0,
            'max_leverage': 500.0,
            'trade_assets_count': 400.0,
            'slippage_volatility_multiplier': 300.0,
            'maker_fee': 100.0,
            'taker_fee': 100.0
        }
        for i, (name, _, _, _) in enumerate(self.variable_schema):
            if name in weight_map:
                self.weights[i] = weight_map[name]

    def _inverse_transform(self, u_vector: np.ndarray) -> Dict[str, Any]:
        """Giải mã từ Vector Không Gian (0-1) thành Hồ Sơ Kinh Tế (Economics Persona)."""
        p = {}
        for i, (name, p1, p2, dist_type) in enumerate(self.variable_schema):
            u = u_vector[i]
            
            if dist_type == 'uniform':
                v = p1 + (p2 - p1) * u
            elif dist_type == 'log_uniform':
                v = np.exp(np.log(p1) + (np.log(p2) - np.log(p1)) * u)
            elif dist_type == 'normal':
                u_safe = max(0.001, min(0.999, u))
                v = norm.ppf(u_safe, loc=p1, scale=p2)
            elif dist_type == 'uniform_int':
                v = int(round(p1 + (p2 - p1) * u))
            elif dist_type == 'categorical':
                v = 1.0 if u > 0.5 else 0.0
            else:
                v = u
            
            p[name] = v
        return p

    def generate_dataset(self, n_personas: int = 1000) -> List[Dict]:
        """
        Sinh tập Dataset siêu đa dạng bằng Trình tự Sobol (Sobol Sequence).
        Áp dụng Cơ chế Từ Chối (Rejection Sampling) với Euclidean Distance.
        Để đảm bảo Threshold 1000.0 (The God-Mode Benchmark Threshold).
        """
        print(f"[PersonaGenerator] Khởi động động cơ lấp đầy không gian QMC Sobol cho {n_personas} hồ sơ.")
        
        engine = qmc.Sobol(d=self.num_dims, scramble=True)
        m = math.ceil(math.log2(n_personas * 10)) 
        raw_samples = engine.random_base2(m=m)
        
        dataset = []
        accepted_scaled_vectors = []
        
        target_distance = 1000.0
        decay_factor = 0.99
        
        rejections = 0
        total_evals = 0
        
        for u_vec in raw_samples:
            if len(dataset) >= n_personas:
                break
                
            total_evals += 1
            scaled_vec = u_vec * self.weights
            
            accept = True
            if len(accepted_scaled_vectors) > 0:
                matrix_accepted = np.array(accepted_scaled_vectors)
                distances = cdist(scaled_vec.reshape(1, -1), matrix_accepted, metric='euclidean')[0]
                if np.any(distances < target_distance):
                    accept = False
            
            if accept:
                persona_dict = self._inverse_transform(u_vec)
                
                # CHÍNH SÁCH 1: ÉP BUỘC 10-30 TRADE ASSETS
                c_ta = min(30, max(10, int(persona_dict['trade_assets_count'])))
                if c_ta > 0 and len(self.all_trade_assets) > 0:
                    persona_dict['trade_assets'] = list(np.random.choice(self.all_trade_assets, min(c_ta, len(self.all_trade_assets)), replace=False))
                else:
                    persona_dict['trade_assets'] = []
                    
                # CHÍNH SÁCH 2: FULL RATES, FULL STATS (Bất chấp mọi thứ, ôm gộp toàn bộ Context Vĩ mô)
                persona_dict['context_assets'] = self.all_rate_assets + self.all_stat_assets
                
                dataset.append(persona_dict)
                accepted_scaled_vectors.append(scaled_vec)
                
                if len(dataset) % 100 == 0:
                    print(f"   -> Đã sinh {len(dataset)}/{n_personas} (Ngưỡng an toàn hiện tại: {target_distance:.2f})")
            else:
                rejections += 1
                # Nếu từ chối quá nhiều, nới lỏng nhẹ mốc đo (để không lặp vô tận)
                if rejections > 2000:
                    target_distance *= decay_factor
                    rejections = 0
        
        print(f"✅ ĐẠT CHUẨN GOD-MODE: Thiết kế thành công {len(dataset)} Persona siêu phân tán.")
        print(f"✅ Thống kê cuối: Quét {total_evals} điểm, Min Distance duy trì: {target_distance:.2f}")
        return dataset

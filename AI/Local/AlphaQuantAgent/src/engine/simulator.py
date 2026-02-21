"""
/**
 * TỆP VẬN HÀNH (OPERATIONAL MODULE): MÔI TRƯỜNG GIẢ LẬP KHÔNG THỜI GIAN (THE SIMULATOR)
 * ====================================================================================
 * CÂU HỎI 1: Tệp này sinh ra để làm gì?
 * -> Vai trò: Vòng lặp For-Loop cốt lõi (Core Engine). Nó gắn kết Mạng Nơron (RL Agent), Thị trường (Market), và Túi tiền (Wallet) vào một trục thời gian đồng nhất.
 * -> Lý do tồn tại: Agent học RL (Reinforcement Learning) dựa trên chuẩn OpenAI Gym/Gymnasium. Nó không hiểu "tài sản" hay "tiền", nó chỉ biết Vector Quan sát (Observation Array) và Mảng Hành Động (Action). Simulator dịch các ngôn ngữ này qua lại.
 * 
 * CÂU HỎI 2: Đầu vào (Input) của hàm Step là gì?
 * -> Mảng Action Dictionaries chứa Tỷ trọng Phân bổ vốn lý thuyết. Ví dụ: {'BTC_USDT': 0.5, 'NVDA': 0.3}. 
 * 
 * CÂU HỎI 3: Đầu ra (Output) tác động thế nào đến hệ thống?
 * -> Kích hoạt `wallet.execute()`. 
 * -> Tính toán Hàm Phần thưởng (Reward Function) trừng phạt hoặc khen ngợi Agent dựa trên quy mô Drawdown, Hành vi Lười biếng (Inactivity), hoặc Mất kiểm soát (Illegal moves).
 * -> Trả về Observation mới nhất + Điểm Reward + Trạng thái Game Over (Done).
 */
"""

import numpy as np
from typing import Dict, Tuple
from src.engine.market import Market
from src.engine.wallet import Wallet

class TradingSimulator:
    def __init__(self, market_engine: Market, wallet_engine: Wallet, window_size: int = 16, allowed_trade_assets: list = None, persona: Dict = None):
        """
        Khởi tạo Không gian Môi trường Ảo.
        THÔNG SỐ:
        - window_size: Kích thước cửa sổ Tensor nạp vào cho Trí tuệ (RL) (số nến quá khứ Agent được phép nhìn thấy).
        - allowed_trade_assets: Danh sách hợp lệ cho Persona hiện tại (Action Masking).
        """
        self.market = market_engine
        self.wallet = wallet_engine
        self.window_size = window_size
        self.allowed_trade_assets = allowed_trade_assets if allowed_trade_assets else market_engine.asset_list
        self.current_step = 0
        self.max_steps = len(self.market.timeline) - 1
        
        # Biến đếm Tracking cho Reward Shaping
        self.consecutive_inactivity = 0
        self.illegal_moves = 0
        self.trade_counts = {'buy': 0, 'sell': 0, 'hold': 0}
        self.inactivity_limit = 10 # Ngưỡng lười biếng (10 steps)
        
        self.persona = persona if persona is not None else {}
        self._build_static_context()

    def _build_static_context(self):
        """Scale and encode the persona constants and wallet status into a flat 1D vector (approx 29 features)"""
        # We need a robust static array. For now let's just lay down the items from persona mapped flatly.
        static_vars = []
        keys = ['initial_capital', 'max_leverage', 'margin_maintenance_rate', 'funding_rate_bps', 
                'random_cash_inflow_outflow', 'drawdown_penalty', 'target_return_annualized', 
                'sharpe_optimization_weight', 'inactivity_penalty', 'overtrading_penalty', 
                'win_rate_obsession', 'trade_assets_count', 'context_assets_count', 
                'max_weight_per_asset', 'min_weight_per_asset', 'allow_short_selling', 
                'max_open_positions', 'maker_fee', 'taker_fee', 'slippage_model_type', 
                'slippage_volatility_multiplier', 'latency_delay_steps', 'spread_bps', 
                'start_timestamp_offset', 'episode_length_days', 'price_noise_variance', 
                'missing_data_prob']
                
        for k in keys:
            static_vars.append(float(self.persona.get(k, 0.0)))
            
        self.base_static_vec = np.array(static_vars, dtype=np.float32)

    def reset(self) -> Dict[str, np.ndarray]:
        """Đưa thế giới về Thuở Hồng Hoang (Timestep 0) với tiền vốn khởi tạo nguyên vẹn."""
        self.current_step = self.window_size # Tránh index bị âm ở các nến lùi sâu trong quá khứ
        self.wallet.reset()
        self.consecutive_inactivity = 0
        self.illegal_moves = 0
        self.trade_counts = {'buy': 0, 'sell': 0, 'hold': 0}
        self.last_nav = self.wallet.cash # Track NAV from previous step
        return self._get_observation()

    def _get_observation(self) -> Dict[str, np.ndarray]:
        """
        [CHỨC NĂNG]: Trích xuất Camera cho Agent RL.
        [ĐẦU VÀO]: `self.current_step` (Thời điểm hiện tại).
        [ĐẦU RA]: Lăng kính hỗn hợp chứa:
        1. Tuýp Thời Gian Trượt (Sliding Window Tensor - Quá khứ H-nến của mảng Market 3D).
        2. Vector Trạng Thái Tĩnh (Persona Configs + Ví tiền hiện thời) giúp Agent đa ý thức (Context-Aware).
        """
        ts_data = self.market.get_state_window(self.current_step, self.window_size)
        
        # Realtime wallet dynamics for context
        # Extract precise total nav for weight calculations
        prices = {sym: ctx['px'] for sym, ctx in self.market.get_execution_context(self.current_step).items()}
        total_nav, _, current_allocs = self.wallet.mark_to_market(prices, current_ts=str(self.market.timeline[self.current_step]))
            
        cash_ratio = self.wallet.cash / total_nav if total_nav > 0 else 1.0
        
        # Calculate exactly the weighting of each allowed trade asset
        alloc_weights = []
        for sym in self.market.asset_list:
            alloc_val = current_allocs.get(sym, 0.0)
            weight = alloc_val / total_nav if total_nav > 0 else 0.0
            alloc_weights.append(weight)
            
        dynamic_static = np.array([cash_ratio, float(self.consecutive_inactivity)] + alloc_weights, dtype=np.float32)
        full_static_data = np.concatenate([self.base_static_vec, dynamic_static])
        
        return {
            'ts_data': ts_data,
            'static_data': full_static_data
        }

    def step(self, actions: Dict[str, float]) -> Tuple[Dict[str, np.ndarray], float, bool, Dict]:
        """
        [CHỨC NĂNG CỐT LÕI]: Dịch chuyển dòng thời gian - Mô phỏng cơ học vi cấu trúc 1 nến (15 phút).
        [ĐẦU VÀO]:
        - `actions`: Mảng Tỷ trọng (Ví dụ: Trọng số của tài sản A muốn mua là 20% NAV).
        [CƠ CHẾ KHỚP LỆNH NGẦM (THE INVISIBLE HAND)]:
        1. Hứng Lệnh: Cắt lớp thông tin Market tại Tick T.
        2. Định Giá (Mark-To-Market): Lấy tổng tài sản Wallet.
        3. Tái Cơ Cấu (Rebalancing): Trừ phần trăm NAV với tiền mặt hiện tại để ép ra lệnh Tăng/Giảm (Buy/Sell Quantity).
        4. Tòa Án Định Lượng (The Reward Tribunal): Tính toán phần thưởng (Reward) với 4 trụ cột Phạt:
           + Phạt lười biếng (Ôm cash quá lâu).
           + Phạt hoảng loạn (Drawdown quá giới hạn).
           + Phạt vi hiến (Chạm điểm mù thanh khoản / Mua vượt giới hạn).
           + Thưởng/Phạt Delta NAV (Thắng làm vua).
        [ĐẦU RA]: Observation mới cho Step t+1, Reward (float), Hoàn tất (Done bool), Dữ liệu Vi mô (Info Dict).
        """
        if self.current_step >= self.max_steps:
            return self._get_observation(), 0.0, True, {}

        # 1. Thu thập Ticker Tape (Thông tin thật của Universe tại Tick_t)
        market_context = self.market.get_execution_context(self.current_step)
        timestamp = self.market.timeline[self.current_step]
        
        # 2. Xử lý các sự kiện Cổ tức, Lãi suất và Thời gian trôi qua hằng ngày (Maturity decay)
        # Giả định Tick là 15 phút -> dt_days = 15 / (24 * 60)
        dt_days = 15.0 / (24.0 * 60.0)
        
        rates_map = {sym: ctx['meta'].get('yield', 0.0) for sym, ctx in market_context.items() if ctx['meta']['type'] == 'RATE'}
        self.wallet.on_time_step(rates_map=rates_map, dt_days=dt_days)

        # Trực tiếp tác động chia tách (split) và cổ tức vào sổ cái theo sự kiện thực
        for sym, ctx in market_context.items():
            if 'div' in ctx['events']: self.wallet.on_dividend(str(timestamp), sym, ctx['events']['div'])
            if 'split' in ctx['events']: self.wallet.on_split(str(timestamp), sym, ctx['events']['split'])

        # Reset Tracking for this step
        step_illegal_moves = 0
        executed_trades = 0

        # Lấy giá trị NAV hiện tại để phân mảnh số vốn rebalance
        prices = {sym: ctx['px'] for sym, ctx in market_context.items()}
        total_nav, _, alloc = self.wallet.mark_to_market(prices, current_ts=str(timestamp))

        reward_penalty = 0.0

        # 3. Chuyển Đổi Tỷ Trọng Hành Động (Action Weights) -> Lệnh Kế Toán Mua/Bán (Rebalancing)
        for sym, target_weight in actions.items():
            if target_weight < 0: target_weight = 0.0 # Strict No-shorting constraint tại Engine Simulator
            
            # Action Masking / Penalty logic: Nếu AI phân bổ vốn vào tài sản không nằm trong danh sách cho phép của Persona
            if sym not in self.allowed_trade_assets and target_weight > 0.01:
                reward_penalty -= 0.1 # Phạt vì đầu tư ngoài danh mục ủy thác (Out of bounds allocation)
                target_weight = 0.0
            
            target_value = total_nav * target_weight
            current_value = alloc.get(sym, 0.0)
            diff_value = target_value - current_value
            
            # Khử nhiễu: Bỏ qua chi phí vi mô sinh ra do phí sàn
            if abs(diff_value) < 1.0:
                continue

            ctx = market_context[sym]
            px, vol, penalty = ctx['px'], ctx['vol'], ctx['penalty']
            
            if px <= 0: continue # API Data failure bypass

            side = 1 if diff_value > 0 else -1
            size_qty = abs(diff_value) / px
            
            # Phạt Trí tuệ Nhân tạo nếu nó cố gắng đâm đầu mua vào vùng tài sản có Điểm mù thanh khoản (staleness penalty cao do Volume 0.0)
            if side == 1 and penalty > 0:
                reward_penalty -= (penalty * 0.01)

            success, msg = self.wallet.execute(
                ts=str(timestamp), symbol=sym, side=side, size=size_qty,
                px=px, vol=vol, penalty=penalty, meta=ctx['meta']
            )

            if success:
                executed_trades += 1
                if ctx['meta']['type'] == 'TRADE':
                    self.market.apply_market_impact(self.current_step, sym, size_qty, side)
                if side == 1: self.trade_counts['buy'] += 1
                if side == -1: self.trade_counts['sell'] += 1
            else:
                # GIAI ĐOẠN 3: Hình Phạt ILLEGAL MOVES Cực Hạn (Đặt lệnh lỗi vượt vốn)
                reward_penalty -= 5.0
                self.illegal_moves += 1
                step_illegal_moves += 1

                # RÚT KINH NGHIỆM: Phạt Đậm 5.0 Reward nếu RL cố tình rút Rate Asset (VCB) khi nó chưa tới ngày đáo hạn Maturity.
                if msg == "LOCKED_MATURITY":
                    reward_penalty -= 5.0 

        if executed_trades == 0:
            self.trade_counts['hold'] += 1

        # MÓC NỐI NAV MỚI
        new_nav, unrealized_pnl, _ = self.wallet.mark_to_market(prices, current_ts=str(timestamp))
        
        # YÊU CẦU 1: GỌT GIŨA NET PNL THAY VÌ LẠI CỘNG DỒN
        delta_nav_pct = ((new_nav - self.last_nav) / self.last_nav) if self.last_nav > 0 else 0.0
        self.last_nav = new_nav # Update for next step
        
        # YÊU CẦU 1: THE DO-NOTHING PARADOX - Phạt Lười Biếng (Inactivity Penalty)
        # Giả sử AI ôm 100% cash
        if self.wallet.cash >= total_nav * 0.99 and executed_trades == 0:
            self.consecutive_inactivity += 1
        else:
            self.consecutive_inactivity = 0
            
        if self.consecutive_inactivity > self.inactivity_limit:
            reward_penalty -= 0.5 # Giai đoạn 3: Phạt nặng liên tục mỗi bước nếu lười biếng quá giới hạn

        # YÊU CẦU 1: Hình Phạt Drawdown (Cắt lỗ sớm)
        is_drawdown = False
        is_ruined = False
        if new_nav < self.wallet.high_water_mark:
            drawdown_pct = (self.wallet.high_water_mark - new_nav) / self.wallet.high_water_mark
            
            # Giai đoạn 4: Risk of Ruin Termination (Cháy 50% tài khoản)
            if drawdown_pct >= 0.50:
                is_ruined = True
                reward_penalty -= 100.0
            elif drawdown_pct > 0.02: # Âm nặng hơn 2%
                reward_penalty -= (drawdown_pct * 0.5)
                is_drawdown = True

        # Kết hợp Công Thức Reward Cốt Lõi (Omniverse Core Equation)
        # R_t = (w1 * Delta_Nav) - (w2 * Inactivity) - (w3 * Illegal) - (w4 * Drawdown) + previous custom penatlies
        final_reward = float((delta_nav_pct * 100.0) + reward_penalty) # Scale relative Return for stabilization
        if is_drawdown and final_reward < 0:
            final_reward *= 1.5 # Nhân đôi/gấp rưỡi nỗi đau drawdown

        self.current_step += 1
        done = (self.current_step >= self.max_steps) or is_ruined

        # Đóng hộp Dữ liệu để Xuất Log Xuyên Thấu ra Callbacks.py
        info = {
            'nav': new_nav,
            'timestamp': str(timestamp),
            'realized_nav_pct': delta_nav_pct,
            'illegal_moves_total': self.illegal_moves,
            'step_illegal_moves': step_illegal_moves,
            'inactivity_hits': self.consecutive_inactivity,
            'trade_counts': self.trade_counts.copy()
        }

        return self._get_observation(), final_reward, done, info

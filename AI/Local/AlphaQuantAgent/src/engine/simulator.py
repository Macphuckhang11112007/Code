"""
/**
 * MODULE: The Core Simulator
 * VAI TRÒ: Vòng lặp For-Loop không-thời gian cốt lõi. Gắn kết Trí tuệ (Agent), Thế giới Thực (Market), và Kế toán (Wallet).
 * CƠ CHẾ: Đóng vai "Chúa", điều khiển đồng hồ thời gian của toàn hệ thống (tick-tock) qua từng nến 15 phút.
 * TẠI SAO PHẢI CÓ MODULE NÀY: Mạng PPO Agent không nhận diện được khái niệm Ví và Giao dịch. Nó chỉ tương tác qua hàm Step() 
 * truyền thống - nhận Array quan sát, sau đó suy diễn hành động Hành động (Weights Array). 
 * Simulator sẽ hứng mảng tỉ trọng đó, đối chiếu với danh mục tài sản Market ở timestep này, rồi tự động dịch ra các lệnh Buy/Sell.
 */
"""

import numpy as np
from typing import Dict, Tuple
from src.engine.market import Market
from src.engine.wallet import Wallet

class TradingSimulator:
    def __init__(self, market_engine: Market, wallet_engine: Wallet, window_size: int = 16):
        """
        Khởi tạo Không gian Môi trường Ảo.
        THÔNG SỐ:
        - window_size: Kích thước cửa sổ Tensor nạp vào cho Trí tuệ (RL) (số nến quá khứ Agent được phép nhìn thấy).
        """
        self.market = market_engine
        self.wallet = wallet_engine
        self.window_size = window_size
        self.current_step = 0
        self.max_steps = len(self.market.timeline) - 1

    def reset(self) -> np.ndarray:
        """Đưa thế giới về Thuở Hồng Hoang (Timestep 0) với tiền vốn khởi tạo nguyên vẹn."""
        self.current_step = self.window_size # Tránh index bị âm ở các nến lùi sâu trong quá khứ
        self.wallet.reset()
        return self._get_observation()

    def _get_observation(self) -> np.ndarray:
        """Trích xuất Ống Nhòm Thời Gian (Tensor 3D Slice) từ Market Data cung cấp cho Mạng NN."""
        return self.market.get_state_window(self.current_step, self.window_size)

    def step(self, actions: Dict[str, float]) -> Tuple[np.ndarray, float, bool, Dict]:
        """
        Dịch Chuyển Trạng Thái Không-Thời Gian (Mô phỏng 1 Khoảng 15 phút).
        THÔNG SỐ:
        - actions: Mapping {Tên Tài Sản (Asset Ticker): Tỷ trọng phân bổ của con AI (Scale 0-1)}.
        CƠ CHẾ HOẠT ĐỘNG KHỚP LỆNH:
        1. Thu Thập Context Market hiện thời.
        2. Dựa vào tỷ trọng AI chỉ định, tái cấu trúc (Rebalancing) danh mục.
        3. Phạt Reward mỏng nếu AI lạng lách bất hợp lý.
        4. Trả kết quả sinh tồn cho Mạng Nhựa (NN).
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

        # Lấy giá trị NAV hiện tại để phân mảnh số vốn rebalance
        prices = {sym: ctx['px'] for sym, ctx in market_context.items()}
        total_nav, _, alloc = self.wallet.mark_to_market(prices, current_ts=str(timestamp))

        reward_penalty = 0.0

        # 3. Chuyển Đổi Tỷ Trọng Hành Động (Action Weights) -> Lệnh Kế Toán Mua/Bán (Rebalancing)
        for sym, target_weight in actions.items():
            if target_weight < 0: target_weight = 0.0 # Strict No-shorting constraint tại Engine Simulator
            
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

            if success and ctx['meta']['type'] == 'TRADE':
                self.market.apply_market_impact(self.current_step, sym, size_qty, side)

            # RÚT KINH NGHIỆM: Phạt 0.5 Reward nếu RL cố tình rút (Bán) Rate Asset (VCB) khi nó chưa tới ngày đáo hạn Maturity. (Ép AI học quy luật thời gian khóa vốn)
            if not success and msg == "LOCKED_MATURITY":
                reward_penalty -= 0.5 

        # 4. Tính Toán Tổng PnL (Tăng trưởng NAV) -> Reward Chính cho Neural Network 
        new_nav, _, _ = self.wallet.mark_to_market(prices, current_ts=str(timestamp))
        step_reward = ((new_nav - total_nav) / total_nav) if total_nav > 0 else 0.0
        
        # Kết hợp Hàm Mục Tiêu Cuối Cùng (Objective Function)
        final_reward = float(step_reward + reward_penalty)

        self.current_step += 1
        done = (self.current_step >= self.max_steps)

        info = {
            'nav': new_nav,
            'timestamp': str(timestamp),
            'msg': 'Step Complete'
        }

        return self._get_observation(), final_reward, done, info

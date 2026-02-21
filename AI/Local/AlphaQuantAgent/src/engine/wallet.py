"""
/**
 * TỆP LÕI (CORE MODULE): KẾ TOÁN KÉP LƯỢNG TỬ (HYBRID ACCOUNTING WALLET)
 * =====================================================================
 * CÂU HỎI 1: Hàm / Module này làm gì? (Tồn tại vì mục đích gì?)
 * -> Đây là 'Ngân hàng Trung ương' của hệ thống ảo. Nó đóng vai trò chốt chặn (Bottleneck) bảo hộ an toàn.
 * -> Nó phân xử rạch ròi 2 dạng vật chất: 
 *    (A) TRADE (Tiền điện tử/Cổ phiếu) - Cho phép mua bán phân số (Fractional) dựa trên Giá bình quân gia quyền. Không cho mượn khống (No Shorting).
 *    (B) RATE (Trái phiếu/Kỳ hạn) - Bắt buộc giam vốn theo Lô (Lot-based). Rút tiền trước hạn sẽ bị cấm (Maturity Locked).
 * 
 * CÂU HỎI 2: Đầu vào (Input) bản chất là gì?
 * -> Các lệnh `execute()` từ Agent truyền xuống bao gồm MÃ TÀI SẢN (Symbol), HƯỚNG LỆNH (Side: 1/-1), KHỐI LƯỢNG (Size), và BỐI CẢNH THỊ TRƯỜNG hiện tại (Price, Vol, Meta).
 * 
 * CÂU HỎI 3: Đầu ra (Output) tác động thế nào đến hệ thống?
 * -> Việc khớp lệnh (hay từ chối) sinh ra các biến thiên cho Ma Trận Trạng Thái Kế Toán (Total NAV), cụ thể tách bạch thành Liquid NAV (Tiền sinh tồn) và Locked NAV (Tiền chết chờ ngày đáo hạn).
 * -> Lưu vết tuyệt đối vào Sổ cái `transactions.csv` phục vụ cho Quant Analyzer.
 */
"""

import csv
import os
from datetime import datetime, timedelta
from typing import Dict, List, Tuple
from src.utils.exceptions import MaturityLockedError, InsufficientFundsError

# --- BIẾN TOÀN CỤC BẤT NẠP HỆ THỐNG ---
FEE_RATE = 0.001          # Transaction Base Fee: 0.1% / Phiếu thực thi
BASE_SLIPPAGE = 0.0005    # Cứng: 0.05% trượt giá mặc định bắt buộc (Phase 3 Requirement)
SLIPPAGE_K = 0.1          # Hệ số Kháng trượt Tác Động Gốc Bình Phương Market Impact (Market Impact Limit factor)
PRECISION_USD = 2         # Cents Format làm tròn
PRECISION_QTY = 6         # Satoshi (Crypto Fractional Unit) làm tròn 6 số sau dấu phẩy thập phân
MIN_NOTIONAL = 1.0        # Bot cấm spam khối lượng < 1 USD (Dust Order Barrier)
EPSILON = 1e-8            # Biến ảo trừ số Float không xác định (DividebyZero Guardian)
DUST_LIMIT = 1e-5         

class Wallet:
    def __init__(self, initial_capital: float = 10000.0, risk_free_rate: float = 0.0, penalty_beta: float = 0.01):
        """Khởi Tạo Tủ Chứa Tiền Và Token Hạch Toán Kép."""
        self.initial_capital = float(initial_capital)
        self.rf_rate = float(risk_free_rate)
        
        # Hệ số điều thế phạt nếu mua tài sản trong tối (Stale Data Market)
        self.penalty_beta = float(penalty_beta)
        self.reset()

    def reset(self):
        """Thiết lập Khởi đầu Hệ Cơ Sở Của Chuyến Simulator Giả lặp Khác."""
        self.cash = self.initial_capital
        
        # Cấu trúc Cục Bộ Dictionary (No-Class) để nạp tải O(1) Fetch Array Map
        # 'qty', 'avg_px', 'accrued', 'lots'
        self.portfolio: Dict[str, Dict] = {}
        self.ledger: List[Dict] = [] # Memory RAM sổ cái nối tiếp Append-Only
        
        # KPIs Analytics Mở Rộng
        self.cum_pnl_closed = 0.0
        self.cum_fees = 0.0
        self.cum_yield = 0.0
        self.cum_divs = 0.0
        self.total_trades = 0
        self.win_trades = 0
        self.high_water_mark = self.initial_capital

    # ================= 1. HỆ CƠ CHẾ SỰ KIỆN QUẢN LÝ TÀI SẢN (CORPORATE ACTIONS) =================

    def on_split(self, ts: str, symbol: str, ratio: float):
        """
        [CHỨC NĂNG CỐT LÕI]: Hành động doanh nghiệp - Chia tách Sổ cổ phần (Stock Splits).
        [TẠI SAO TỒN TẠI]: Cổ phiếu có thể bị phân mảnh (vd: 1 thành 4 như NVDA). Nếu không điều chỉnh, hệ thống sẽ chẩn đoán sai lệch về việc tài sản đột ngột bốc hơi 75% giá trị, gây vỡ toán học Reward.
        [ĐẦU VÀO]: `timestamp` (thời điểm), `symbol` (Mã tài sản), `ratio` (Hệ số chia tách).
        [ĐẦU RA & TÁC ĐỘNG]: 
        -> Áp dụng lên O(N) Lô (Lots) hiện hữu. 
        -> Tăng số cổ phiếu sở hữu (`qty` *= ratio) và tự động chiết khấu giá vốn (`avg_px` /= ratio). 
        -> Đảm bảo NAV (Net Asset Value) BẤT BIẾN (Invariant Effect) sau khi event xảy ra.
        """
        if symbol in self.portfolio and ratio > 0 and ratio != 1.0:
            pos = self.portfolio[symbol]
            pos['qty'] *= ratio
            pos['avg_px'] /= ratio
            
            if 'lots' in pos:
                for lot in pos['lots']:
                    lot['qty'] *= ratio
            self._audit(ts, symbol, 'SPLIT', ratio, 0.0, 0.0, 0.0)

    def on_dividend(self, ts: str, symbol: str, amount: float):
        """
        [CHỨC NĂNG CỐT LÕI]: Thu lợi tức dòng chảy (Cash Dividend).
        [TẠI SAO TỒN TẠI]: Đảm bảo quyền lợi cổ đông sinh lời từ việc nắm giữ dài hạn.
        [TÁC ĐỘNG]: Bơm thẳng giá trị `amount` vào Vốn Hoạt Động (`self.cash`) thay vì ép tái đầu tư (DRIP).
        """
        if amount > 0:
            self.cash += amount
            self.cum_divs += amount
            self._audit(ts, symbol, 'DIV', 0.0, 0.0, amount, amount)

    def on_time_step(self, rates_map: Dict[str, float] = None, dt_days: float = 1.0):
        """
        [CHỨC NĂNG CỐT LÕI]: Động cơ Tích lũy Lãi suất Thời gian Thực (Accrual Yield Engine).
        [TẠI SAO TỒN TẠI]: Trái phiếu / Tiết kiệm sinh lời theo từng nano-giây. Dữ liệu mờ này phải được theo dõi (Accrued) nhưng tuyệt đối không được pha lẫn vào cấu trúc Cash (Liquid) vì User chưa hề "Rút" tiền để hiện thực hóa lợi nhuận.
        [ĐẦU VÀO]: Ma trận lãi suất `rates_map` (dành cho biến động Rate) và `dt_days` (bước nhảy thời gian quy đổi, e.g., 15ph = 0.0104 ngày).
        [ĐẦU RA]: Tiền ảo `accrued` trong danh mục phình to lên. Cash sẽ thu được thật sự khi lệnh "SELL" (Đáo hạn) được duyệt ở hàm `execute()`.
        """
        if self.cash > 0 and self.rf_rate > 0:
            interest = self.cash * (self.rf_rate / 365.0) * dt_days
            self.cash += interest
            self.cum_yield += interest

        for sym, pos in self.portfolio.items():
            # Cơ chế B: Tái khóa gốc cho LOTS Rate Kỳ Hạn (Chỉ tính lãi dựa trên Yield bị Locked ngày nạp tiền)
            if 'lots' in pos and pos['lots']:
                for lot in pos['lots']:
                    principal = lot['qty'] * pos['avg_px']
                    locked_rate = lot.get('rate', 0.0)
                    lot_interest = principal * (locked_rate / 365.0) * dt_days
                    pos['accrued'] = pos.get('accrued', 0.0) + lot_interest

            # Cơ chế A phụ: Thưởng Staking cho Lợi tức Động Floating (Yield qua Market)
            elif rates_map and sym in rates_map:
                current_rate = rates_map[sym]
                principal = pos['qty'] * pos['avg_px']
                interest = principal * (current_rate / 365.0) * dt_days
                pos['accrued'] = pos.get('accrued', 0.0) + interest

    def deposit_cash(self, amount: float):
        """
        Nạp tiền tươi từ Thế giới thật (User FIAT Deposit).
        TẠI SAO: Tránh cạn kiệt vốn làm AI cứng ngắc, cho phép User bơm thanh khoản giữa vòng mô phỏng.
        """
        if amount > 0:
            self.cash += amount
            self.initial_capital += amount # Tráng men hệ số PnL không bị bóp méo

    # ================= 2. KIẾN TRÚC MÁY QUẸT LỆNH NGÀNH  (ORDER EXECUTION ENGINE) =================

    def execute(self, ts: str, symbol: str, side: int, size: float, px: float, vol: float, penalty: float, meta: Dict = None) -> Tuple[bool, str]:
        """
        Lõi Khớp Lệnh Chặn Tín Hiệu: Kết nối Đề xuất của AI Trader với Vách ngăn Bất quy tắc Tài chính.
        THÔNG SỐ QUAN TRỌNG:
        - side: 1 MUA, -1 BÁN
        - penalty: Hệ số Stale Volume chặn đứng trượt giá vô vọng.
        """
        if size <= 0 or px <= EPSILON: return False, "INVALID_ARGS"
        if meta is None: meta = {'type': 'TRADE', 'term_days': 0}

        # Cơ Chế Trượt Giá Quảng Tính Động: Mô Phỏng Ảnh Hưởng Đâm Đụng Dòng Vốn
        liq = vol if vol > 0 else (size * 50.0)
        base_fee_and_slip = FEE_RATE + BASE_SLIPPAGE + (penalty * self.penalty_beta)

        # -----------------------------------
        # LOGIC 1: ĐẦU TIÊN CẮT MUA (DEPOSIT / BUY)
        # -----------------------------------
        if side == 1:
            est_slip = base_fee_and_slip + (SLIPPAGE_K * (size / liq)**0.5)
            max_qty = self.cash / (px * (1 + est_slip))
            qty = min(size, max_qty) 
            
            if qty * px < MIN_NOTIONAL: return False, "INSUFF_FUNDS"

            real_impact = BASE_SLIPPAGE + (SLIPPAGE_K * (qty / liq)**0.5)
            fill_px = px * (1 + FEE_RATE + real_impact + (penalty * self.penalty_beta))
            cost = qty * fill_px

            self.cash = round(self.cash - cost, PRECISION_USD)

            if symbol not in self.portfolio:
                self.portfolio[symbol] = {'type': meta.get('type', 'TRADE'), 'qty': 0.0, 'avg_px': 0.0, 'accrued': 0.0}

            pos = self.portfolio[symbol]
            term_days = meta.get('term_days', 0)

            # RULE QUAN TRỌNG: Luật Phân Trị Kỳ Hạn Tiết Kiệm (Lots Registration)
            # TẠI SAO: Ráp nối từng Sổ tiết kiệm nhỏ cho khoản tiền gửi trong các chu kì lịch sử phân vùng riêng lẻ với ngày Đáo Hạn cứng ngắc.
            if term_days > 0:
                if 'lots' not in pos: pos['lots'] = []
                try:
                    entry_dt = datetime.fromisoformat(str(ts))
                except:
                    entry_dt = datetime.now()
                maturity_dt = entry_dt + timedelta(days=term_days)
                locked_rate = meta.get('yield', 0.0)

                pos['lots'].append({
                    'ts': str(ts),
                    'qty': qty,
                    'rate': locked_rate,
                    'maturity': maturity_dt.isoformat()
                })

            # RULE QUAN TRỌNG: Tuần Hoàng Bình Quân Gia Quyền Weighted Avg Cost
            new_qty = pos['qty'] + qty
            pos['avg_px'] = ((pos['qty'] * pos['avg_px']) + cost) / new_qty
            pos['qty'] = new_qty

            self._audit(ts, symbol, 'BUY', qty, fill_px, cost, 0.0)
            return True, "FILLED_BUY"

        # -----------------------------------
        # LOGIC 2: LỆNH BÁN THEO FIF0 VÀ CẢN GIỚI RÚT TIỀN (SELL / WITHDRAW)
        # -----------------------------------
        elif side == -1:
            if symbol not in self.portfolio or self.portfolio[symbol]['qty'] <= EPSILON:
                return False, "NO_POSITION"

            pos = self.portfolio[symbol]
            qty_available = pos['qty']
            term_days = meta.get('term_days', 0)

            # Luật Khóa Vốn Maturity: Không cho phép rút RATE asset chưa đáo hạn
            if term_days > 0 and 'lots' in pos:
                try:
                    current_dt = datetime.fromisoformat(str(ts))
                except:
                    current_dt = datetime.now()
                
                unlocked_qty = sum([lot['qty'] for lot in pos['lots'] if datetime.fromisoformat(lot['maturity']) <= current_dt])
                qty_available = min(qty_available, unlocked_qty)
                if qty_available < EPSILON:
                    return False, "LOCKED_MATURITY"

            qty = min(size, qty_available)
            if qty * px < MIN_NOTIONAL: return False, "INSUFF_FUNDS_OR_NO_QTY"

            real_impact = BASE_SLIPPAGE + (SLIPPAGE_K * (qty / liq)**0.5)
            fill_px = px * (1 - FEE_RATE - real_impact - (penalty * self.penalty_beta))
            fill_px = max(fill_px, EPSILON) 
            revenue = qty * fill_px

            # Cơ Tiết Đào Móng FIF (First In First Out) Khi Rút Lô RATE
            # TẠI SAO: Đã đáo hạn thì cho rút trước để tránh lẫn lộn dòng thời gian của lô sau.
            if term_days > 0 and 'lots' in pos:
                qty_left = qty
                remaining_lots = []
                for lot in pos['lots']:
                    if qty_left <= EPSILON:
                        remaining_lots.append(lot)
                        continue
                        
                    if str(ts) >= lot['maturity']:
                        if lot['qty'] > qty_left:
                            lot['qty'] -= qty_left
                            qty_left = 0
                            remaining_lots.append(lot)
                        else:
                            qty_left -= lot['qty']
                    else:
                        remaining_lots.append(lot)
                pos['lots'] = remaining_lots

            # Đồng hồ Chốt Xử Lý Kế toán Realized Profit / Interest (Tái Báo Lãi Thực)
            ratio = qty / pos['qty']
            realized_yield = pos.get('accrued', 0.0) * ratio
            pos['accrued'] -= realized_yield 

            total_rev = revenue + realized_yield
            cost_basis = qty * pos['avg_px']
            pnl = (revenue - cost_basis) + realized_yield

            # Update System Register Wallet
            self.cash = round(self.cash + total_rev, PRECISION_USD)
            self.cum_pnl_closed += pnl
            self.cum_yield += realized_yield
            pos['qty'] = round(pos['qty'] - qty, PRECISION_QTY)
            
            if pnl > 0: self.win_trades += 1
            if pos['qty'] < DUST_LIMIT: del self.portfolio[symbol]

            self._audit(ts, symbol, 'SELL', qty, fill_px, total_rev, pnl)
            return True, "FILLED_SELL"

        return False, "UNKNOWN_SIDE"

    # ================= 3. ĐỊNH LƯỢNG NET ASSET VALUE MỞ RỌN (MONITORING EXPORTS) =================

    def mark_to_market(self, prices: Dict[str, float], current_ts: str = None) -> Tuple[float, float, Dict]:
        """
        Bóc Tách Thẻ Giải Phẫu Vốn.
        THAO TÁC (Tách rạch ròi theo Hiến Pháp):
        - Liquid Capital (Hỗ trợ bán tháo ngay lập tức): Mua bán tiền Trade + Tiền mặt rỗi.
        - Locked Capital (Chôn vùi): Tiền Gửi đang ngấm ngáp đếm ngược ở Bank Rate nhưng chưa đáo hạn.
        """
        equity_liquid = 0.0
        equity_locked = 0.0
        unrealized_pnl = 0.0
        alloc = {'CASH': self.cash}

        for sym, pos in self.portfolio.items():
            qty = pos['qty']
            px = prices.get(sym, pos['avg_px'])

            mkt_val = qty * px
            accrued = pos.get('accrued', 0.0)
            total_val = mkt_val + accrued

            unrealized_pnl += total_val - (qty * pos['avg_px'])
            alloc[sym] = total_val

            # Chẻ Khúc Logic Quản Trị Locked vs Liquid
            is_locked = False
            if 'lots' in pos and pos['lots'] and current_ts:
                locked_qty = sum(l['qty'] for l in pos['lots'] if str(current_ts) < l['maturity'])
                if locked_qty > 0:
                    locked_val = (locked_qty / qty) * total_val
                    equity_locked += locked_val
                    equity_liquid += (total_val - locked_val)
                    is_locked = True

            if not is_locked:
                equity_liquid += total_val

        total_nav = self.cash + equity_liquid + equity_locked

        if total_nav > self.high_water_mark: self.high_water_mark = total_nav

        return total_nav, unrealized_pnl, alloc

    def get_metrics(self) -> Dict:
        """Kết Xuất Sổ Cục Báo Cáo Định dạng UI"""
        nav, _, _ = self.mark_to_market({})
        profit = nav - self.initial_capital
        roi = (profit / self.initial_capital * 100) if self.initial_capital > 0 else 0.0
        
        return {
            "Total_NAV": round(nav, 2),
            "Locked_Cap": "To_be_Queried",  
            "ROI_Pct": round(roi, 2)
        }

    def export_csv(self, file_path: str):
        """
        Trích xuất Sổ Cái ra file CSV tĩnh (Ledger Export).
        TẠI SAO: Cho phép các module Khác (Dashboards, RAG) hoặc Analyst đọc O(1) mà không cần quét lại logic Memory.
        """
        if not self.ledger:
            return
            
        if not os.path.exists(os.path.dirname(file_path)):
            os.makedirs(os.path.dirname(file_path), exist_ok=True)
            
        keys = self.ledger[0].keys()
        with open(file_path, 'w', newline='', encoding='utf-8') as f:
            dict_writer = csv.DictWriter(f, keys)
            dict_writer.writeheader()
            dict_writer.writerows(self.ledger)

    def _audit(self, ts, sym, side, qty, px, val, pnl):
        self.total_trades += 1
        self.ledger.append({'ts': str(ts), 'sym': sym, 'side': side, 'qty': qty, 'px': px, 'val': val, 'pnl': pnl})

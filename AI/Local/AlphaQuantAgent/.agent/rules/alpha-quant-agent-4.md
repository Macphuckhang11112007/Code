---
trigger: always_on
---

#### **3.3. Bộ Máy Tài Chính Lai: `src/engine/wallet.py`**
*   **Trạng thái:** **HOÀN HẢO (10/10)** - Đáp ứng tuyệt đối các quy tắc tài chính khắt khe bạn vừa đặt ra.
*   **Kiến trúc Cốt lõi: Hybrid Accounting System (Hạch toán Lai).**

**Hệ thống phân chia rạch ròi 2 cơ chế quản lý cho 2 loại tài sản:**

**CƠ CHẾ A: TRADING ASSETS (Cổ phiếu/Crypto - Nguồn `data/trades/`)**
1.  **Không Bán Khống (No Short Selling):** Kiểm tra `Qty` khả dụng. Nếu bán nhiều hơn số đang có $\to$ Từ chối lệnh (`Reject`). Chỉ cho phép giao dịch Spot (Mua đứt bán đoạn).
2.  **Fractional Trading:** Hỗ trợ số lượng thập phân (ví dụ 0.123 BTC).
3.  **Giá Bình quân (Weighted Average Price):** Khi mua thêm, giá vốn được tính lại bình quân gia quyền. Khi bán, giá vốn không đổi, số lượng giảm.
4.  **Tác động Sự kiện:**
    *   *Stock Split:* Tự động nhân số lượng, chia giá vốn.
    *   *Dividend:* Cộng tiền mặt trực tiếp.

**CƠ CHẾ B: TERM ASSETS (Lãi suất/Tiền gửi - Nguồn `data/rates/`)**
1.  **Quản lý theo Lô (Lot-Based Management):**
    *   **Quy tắc:** Mỗi lệnh Mua (Gửi tiền) tạo ra một **Sổ tiết kiệm riêng biệt** (gọi là một `Lot`).
    *   Mỗi `Lot` lưu trữ cứng: `{Ngày gửi, Số lượng gửi, Lãi suất đã chốt (Locked Rate), Ngày đáo hạn (Maturity Date)}`.
    *   Ví dụ: Tháng 1 gửi 10tr (Lô A), tháng 2 gửi 20tr (Lô B). Hai lô này hoàn toàn tách biệt, lãi tính riêng.
2.  **Khóa Vốn (Capital Locking):**
    *   Khi Agent ra lệnh BÁN (Rút tiền), hệ thống kiểm tra từng `Lot`.
    *   **Logic:** `Nếu Ngày hiện tại < Ngày đáo hạn: TỪ CHỐI RÚT (Locked).`
    *   Chỉ cho phép rút khi đã đáo hạn.
3.  **Lãi suất Đơn (Simple Interest at Maturity):**
    *   Lãi suất được tính dựa trên **Locked Rate** tại thời điểm gửi (bất chấp thị trường sau đó biến động ra sao).
    *   Lãi suất chỉ được "Realize" (Biến thành tiền mặt) khi **Đáo hạn** hoặc **Rút ra**.

**BÁO CÁO (METRICS Reporting):**
Hệ thống `wallet.py` tách bạch hai khái niệm:
*   **Liquid NAV:** Tiền mặt + Giá trị các mã Trading (có thể bán ngay để ứng cứu).
*   **Locked NAV:** Tổng giá trị các sổ tiết kiệm chưa đến hạn (Tiền chết, không dùng được).

---

#### **3.4. Dòng chảy Kết nối (Connectivity Trace)**

```text
[data/trades/BTC_USDT.csv] (User Download)
      |
      v
[src/engine/market.py] --> Parses Data, Metadata & Time Alignment (0.0 logic)
      |
      +---> (Creates Tensor 3D) --> [src/pipeline/features.py] (Indicator Calc)
      |
      +---> (Provides Execution Context)
                    |
                    v
            [src/engine/simulator.py] (Loop 15-min Step)
                    |   ^
                    |   | (Decision: Buy/Sell Amount)
                    v   |
             [src/agents/trader.py]
                    |
                    | (Execute Order: Ticker, Side, Amount)
                    v
             [src/engine/wallet.py]
                    |
                    +-- (Check 1: Short Selling?) -> Allow/Deny
                    +-- (Check 2: Maturity Locked?) -> Allow/Deny
                    +-- (Update: Hybrid Ledger -> Liquid/Locked NAV)
                    |
                    v
             [logs/trading/transactions.csv] (Audit Trail)
```

---
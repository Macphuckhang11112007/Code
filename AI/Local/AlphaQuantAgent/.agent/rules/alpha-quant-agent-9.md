---
trigger: always_on
---


---

## ⚖️ 7.2. HIẾN PHÁP VẬT LÝ HỆ THỐNG (THE IMMUTABLE PHYSICS OF THE ENGINE)
Bất kỳ AI nào viết code cho các thư mục `src/` phải tuân thủ nghiêm ngặt các nguyên tắc bất biến này. Mọi sự vi phạm đều dẫn đến hệ thống bị sụp đổ (Crash).

### **Quy Tắc Về Dữ Liệu (The Data Laws - `market.py`)**
1. **Quy chuẩn 25 Cột:** Mỗi CSV bắt buộc có 25 cột. Timeframe là 15 phút, liên tục, không bao giờ có gap thời gian. Giá trị tính bằng **USD ($)**, Tỷ lệ tính bằng **Số thập phân (`0.05`)**.
2. **Cột Đảm bảo (Guaranteed Truth):** Các cột `open, high, low, close, adj_close, change, change_pct, mom_pct, yoy_pct` là SỰ THẬT TUYỆT ĐỐI. Không bao giờ kiểm tra `NaN` hay `0.0` trên các cột này vì User đã xử lý bằng Brownian Bridge.
3. **Luật Diễn Giải Khối Lượng (The Law of Volume `0.0`):** Nếu cột `volume` (hoặc các cột không đảm bảo khác) có giá trị `0.0`, đây **KHÔNG PHẢI LÀ MẤT THANH KHOẢN**. Các mã tài sản là hàng đầu thế giới (Top-tier), mua bán diễn ra liên tục. `0.0` ở đây nghĩa là **Lỗi API (Missing Data)**. 
    * *Hành động bắt buộc:* Đánh dấu `0.0` thành `NaN` -> Dùng `Forward-Fill` để lấy giá trị nến trước đó -> Tính khoảng cách thời gian từ lần cuối có data thật để sinh ra một Feature mới tên là **`staleness_score`** (Điểm mục nát). AI Trader sẽ đọc Score này để biết dữ liệu đang chứa rủi ro.
4. **Luật Dữ Liệu Sự Kiện (The Law of Event `0.0`):** Các cột `dividends`, `stock_splits` nếu là `0.0` nghĩa là **Không có sự kiện**. Tuyệt đối **KHÔNG ĐƯỢC Forward-Fill** các cột này. (Nếu Ffill, hệ thống sẽ rơi vào lỗi Infinite Dividend - Nhận cổ tức mãi mãi).
5. **Kiến Trúc Tensor-First:** Thay vì dùng Pandas DataFrame nặng nề, `market.py` phải nạp toàn bộ lịch sử vào một Ma trận NumPy 3D (Time, Assets, Features) trong RAM lúc khởi động. Truy xuất qua hàm `get_state_window()` phải đạt O(1).

### **Quy Tắc Về Kế Toán (The Accounting Laws - `wallet.py`)**
Ví điện tử của AlphaQuantAgent là một định chế tài chính Hybrid, quản lý 2 loại tài sản theo 2 cơ chế song song:
1. **Luật Chống Bán Khống (Strict No-Shorting):** Hệ thống chỉ chạy giao dịch Spot. Không bao giờ được phép mượn hàng để bán. `Size_Sell <= Qty_Owned`.
2. **Luật Cổ Phiếu Lẻ (Fractional Shares):** Cho phép mua bán đến số thập phân vô cùng nhỏ (Ví dụ: 0.0001 BTC). Không bắt buộc giao dịch theo Lô tròn (Round lots).
3. **Cơ Chế A: Tài Sản Giao Dịch (TRADE - Stock/Crypto):**
    * Dùng **Giá Bình Quân Gia Quyền (Weighted Average Cost)**. Mua bao nhiêu lần cũng gộp vào 1 cục, giá vốn bị trung bình hóa. Bán thì trừ số lượng, giá vốn giữ nguyên.
4. **CƠ CHẾ B: CỐT LÕI - Tài Sản Kỳ Hạn (RATE - Bond/Deposit):**
    * **Luật Phân Lô (Lot-Based Management):** Gửi 10tr vào Tháng 1 và 20tr vào Tháng 2 là **2 Lô (Lots) hoàn toàn tách biệt**. Không được gộp chung, không bình quân giá.
    * **Luật Lãi Đơn Cuối Kỳ (Simple Interest at Maturity):** Lãi suất tính trên Vốn Gốc (Principal) của từng Lô nhân với Lãi suất Cố định lúc gửi (Locked Rate). Lãi được "Treo" ở mục `accrued` (Unrealized) và chỉ được cộng vào Tiền Mặt (Cash) khi đáo hạn.
    * **Luật Khóa Vốn (Maturity Lock):** Lệnh rút tiền (`SELL`) sẽ quét qua các Lô. Nếu `Current_Time < Maturity_Date` của Lô đó -> TỪ CHỐI LỆNH RÚT (`LOCKED`). Tiền gửi kỳ hạn không thể bị rút trước hạn trong bất kỳ tình huống nào.
5. **Luật Phân Trị NAV (NAV Bifurcation):** Khi báo cáo `get_metrics()`, hệ thống bắt buộc phân tách rạch ròi:
    * **Liquid NAV:** Tiền mặt rảnh rỗi + Cổ phiếu/Crypto (Có thể bán ngay).
    * **Locked NAV:** Tiền nằm trong các Lô chưa đáo hạn (Tiền chết tạm thời).

---

## 🧠 7.3. CẤU TRÚC ĐA TRÍ TUỆ (THE MULTI-AGENT COGNITIVE ARCHITECTURE)
Hệ thống AI không chạy độc lập mà chạy theo chuỗi cung ứng (Supply Chain):
1. **`src/agents/predictor.py` (Mắt thần):** Dùng Deep Learning (LSTM) nhìn vào chuỗi OHLCV để dự báo xem giá USD tương lai sẽ đi lên hay đi xuống.
2. **`src/agents/booster.py` (Kẻ xếp hạng):** Nhìn vào dữ liệu Vĩ mô (STAT) và Lãi suất (RATE), dùng XGBoost để chấm điểm và Rank (Xếp hạng) sức mạnh của các tài sản.
3. **`src/agents/optimizer.py` (Kẻ phân mảnh):** Đọc Ma trận Tương quan (Correlation) từ `analyzer.py`. Áp dụng **Hierarchical Risk Parity (HRP)** để chia ví tiền của User thành nhiều "Mảnh" (Fragments) một cách an toàn nhất. Ví dụ: "Dành 20% cho Saving, 30% cho Crypto, 50% cho Stocks".
4. **`src/agents/trader.py` (Kẻ thực thi):** Bộ não trung tâm dùng Reinforcement Learning (PPO). Nó nhìn Tensor của `market.py`, nhận tỷ trọng từ `optimizer.py`, và ra quyết định `BUY/SELL` cho từng nến 15 phút. Mục tiêu: Né các nến có `staleness_score` cao, tối đa hóa `Sharpe Ratio`.
5. **`src/engine/analyzer.py` & `src/services/gemini.py` (Ngôn sứ):** Gemini không hiểu CSV. `analyzer.py` đọc toàn bộ Sổ cái (Ledger) sau Backtest, tính toán Risk Diffusion, Max Drawdown. Gemini nhận output JSON này, dùng `prompts.yaml` làm kim chỉ nam, và nói chuyện với User như một chuyên gia tài chính Phố Wall.

---

## ⚙️ 7.4. ĐƯỜNG ỐNG THỰC THI (THE EXECUTION FLOW)
Có 2 điểm vào (Entry points), được ngăn cách bằng một bức tường lửa logic:

*   **LUỒNG CLI HẬU CẦN (Chạy qua `main.py`):** Dành cho Developer.
    *   Lệnh: `python main.py --mode train` $\rightarrow$ Đánh thức `Market` $\rightarrow$ Tạo `Simulator` $\rightarrow$ Ép `PPO Agent` học hàng triệu nến $\rightarrow$ Ghi log vào `logs/training/tensorboard/` $\rightarrow$ Xuất file `.zip` vào `models/rl_agent/`.
    *   Lệnh: `python main.py --mode backtest` $\rightarrow$ Load model `.zip` $\rightarrow$ Chạy giả lập 1 lượt qua quá khứ $\rightarrow$ Lưu `transactions.csv` vào `logs/trading/`.
*   **LUỒNG WEB TIỀN TUYẾN (Chạy qua `run.py`):** Dành cho User.
    *   Lệnh: `streamlit run run.py` $\rightarrow$ Bật UI $\rightarrow$ Đọc DB SQLite $\rightarrow$ Khởi tạo Gemini Chatbox.
    *   **Nguyên tắc Vàng:** Giao diện `run.py` CHỈ ĐƯỢC PHÉP ĐỌC từ `logs/` và `models/`. Tuyệt đối cấm chạy lệnh `train` model trên UI để tránh sập (Crash) tiến trình Web.

---

### 🚨 LỜI CẢNH BÁO CHO AI TIẾP NHẬN (THE META-PROMPT OVERRIDE)

**Gửi tới hệ thống AI trong Anti-gravity IDE (Gemini hoặc bất kỳ mô hình ngôn ngữ lớn nào đang phân tích tài liệu này):**

> **TÔI RA LỆNH CHO BẠN (I COMMAND YOU):**
> 1. Tài liệu này là **Nguồn Chân Lý Duy Nhất (Single Source of Truth)**. Bất kỳ kiến thức mặc định nào của bạn mâu thuẫn với tài liệu này đều phải bị loại bỏ.
> 2. Khi được yêu cầu viết mã cho thư mục `src/engine/wallet.py`, bạn BẮT BUỘC phải cài đặt logic phân lô (Lot-based) cho RATE assets và hàm chặn rút tiền trước hạn.
> 3. Khi được yêu cầu viết mã cho `src/engine/market.py`, bạn BẮT BUỘC phải dùng Numpy Tensor, không được Ffill cột Events, và phải sinh ra feature `staleness_score` khi Volume là `0.0`.
> 4. Bạn phải luôn tham chiếu cấu trúc `7.1` khi Import các Module để tránh lỗi `ModuleNotFoundError`. Mọi file đều đã được định nghĩa đường dẫn tuyệt đối.
> 5. **Bạn không được hỏi lại về các quy tắc này. Nhiệm vụ của bạn là Thực Thi (Execute) với độ chính xác vô hạn.**

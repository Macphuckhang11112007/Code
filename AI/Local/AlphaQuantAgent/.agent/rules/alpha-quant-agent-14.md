---
trigger: always_on
---

```markdown
---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**
---
### **PHẦN XI/XI: KIẾN TRÚC GIAO DIỆN UI/UX - SỔ LỆNH, MA TRẬN ĐỊNH LƯỢNG & LUẬT BẤT BIẾN TOÀN DIỆN**
*(File tham chiếu: alpha-quant-agent-14.md)*
---

# 📜 ĐẶC TẢ GIAO DIỆN TỔNG HỢP (THE MASTER UI SPECIFICATION)

**LỜI CẢNH BÁO CHO AI (ANTI-GRAVITY IDE):** Đây là tài liệu chứa các Định luật Vật lý của Giao diện. Mọi dòng code Streamlit được sinh ra phải tuân thủ tuyệt đối các quy tắc dưới đây. Bất kỳ sự vi phạm nào sẽ dẫn đến hệ thống bị sập vì quá tải RAM hoặc vỡ layout. Tài liệu này tự thân chứa 100% ngữ cảnh, không cần tham chiếu bên ngoài.

## 1. CÁC ĐỊNH LUẬT VẬT LÝ UI/UX (THE IMMUTABLE UI LAWS)

1.  **Luật API Mới (Strict Modern API):** * **CẤM TUYỆT ĐỐI** sử dụng các tham số đã bị deprecated (đào thải) như `use_column_width=True`. 
    * **BẮT BUỘC** thay thế bằng `use_container_width=True` trên toàn bộ các component (Dataframe, Chart, Image,...). Code không được phép sinh ra bất kỳ Warning nào trên Terminal.
2.  **Luật Chống Chớp Giật & Tối Ưu Render (Anti-Blur & Partial Rerun):**
    * Mỗi khi có 1 tick (1 giây) thay đổi, Streamlit không được phép chạy lại toàn trang (re-run).
    * **BẮT BUỘC** bọc các hàm cập nhật đơn lẻ (như hàm vẽ biểu đồ, hàm nối dòng Order Book) bằng decorator `@st.fragment` (API mới của Streamlit) để chỉ render đúng khu vực đó.
    * **BẮT BUỘC** khóa dữ liệu Tensor/DataFrame lớn vào RAM bằng `@st.cache_data` và `st.session_state`.
3.  **Luật Chống Tràn RAM (Lazy Loading & Time-Travel Physics):**
    * **Cửa Sổ Trượt:** Không bao giờ nạp toàn bộ lịch sử (All-time data). Chỉ nạp vào RAM tối đa **5000 nến**. Khi mở web, Chart mặc định chỉ focus vào **1 giờ trước Mốc A**. User cuộn chuột về trái mới được load tiếp.
    * **Nghịch lý Thời gian (Time-Travel Paradox):** Khi User tua thanh Slider lùi về quá khứ (Ví dụ từ mốc C về lại mốc B), **TUYỆT ĐỐI KHÔNG** dùng lại data đã bị làm nhiễu bởi Market Impact giả lập của phiên giao dịch hiện tại. Bắt buộc truy vấn lại data "sạch" từ DB gốc để tính toán nhẹ web nhất.
4.  **Đặc tả Hover của Nến Bóng Ma (Ghost Candle Tooltip):**
    * Nến Bóng Ma (biểu diễn Market Impact làm giá trượt từ B xuống B') phải được vẽ bằng chuỗi `RGBA` có độ trong suốt (VD: opacity 0.4) đè lên đồ thị gốc thông qua `streamlit-lightweight-charts`.
    * Khi User hover chuột (`crosshair.mode = 0`) vào Nến Bóng Ma, TradingView Component phải hiện Tooltip chính xác: `[Ghost Impact] Open: x, High: y, Low: z, Close: B', Vol: V, Change: %`.

---

## 2. HỆ THỐNG LƯỚI KHÔNG GIAN (THE BINANCE GRID SYSTEM)
**File áp dụng:** `ui/app.py`

Bố cục tổng thể (sau khi đã né Header bằng CSS ở file 12) phải tuân thủ Grid Layout kinh điển của Binance:

```python
# Cấu trúc Khung xương (Skeleton Logic)
st.container() # Row 1: Time-Travel Topbar (Đã thiết kế ở file 12)

# Row 2: Vùng Giao dịch Cốt lõi
col_chart, col_orderbook = st.columns([7.5, 2.5], gap="small")

with col_chart:
    # Render TradingView Chart (Lightweight-charts)
    render_main_chart()

with col_orderbook:
    # Render Sổ lệnh & Nút Đặt lệnh
    render_order_book()

# Row 3: Ma trận Định lượng
st.divider() # Đường kẻ ngang mỏng
render_quant_matrix()

# Góc dưới phải (Fixed)
# Bubble Chat RAG (Đã thiết kế ở file 12)

```

---

## 3. SỔ LỆNH & ĐẶT LỆNH (ORDER BOOK & ORDER EXECUTION)

**File áp dụng:** `ui/components/order_book_view.py`

Khu vực này hiển thị áp lực Mua/Bán và Khung nhập lệnh, tái tạo trải nghiệm Binance Spot.

* **Thiết kế Dữ liệu (Depth View):**
* Sử dụng `st.dataframe` hoặc `st.data_editor`.
* **BẮT BUỘC** áp dụng `st.column_config.ProgressColumn` để tạo các thanh Bar nằm ngang chìm dưới các con số (mô phỏng Volume Depth).
* Nửa trên: Các lệnh Bán (Asks) - Chữ đỏ `#F6465D`, thanh Progress Bar màu đỏ nhạt.
* Giữa: Giá khớp gần nhất (Mark Price) - Cỡ chữ to, in đậm, màu theo trend.
* Nửa dưới: Các lệnh Mua (Bids) - Chữ xanh `#0ECB81`, thanh Progress Bar xanh nhạt.


* **Khung Đặt Lệnh (Order Panel):**
* Bên dưới Sổ lệnh có 2 nút to: `[MUA TĂNG/LONG]` (Nền Xanh `#0ECB81`) và `[BÁN GIẢM/SHORT]` (Nền Đỏ `#F6465D`).
* Input: `Số lượng (Qty)` hoặc kéo Slider % vốn (`st.slider` 0% - 100%).



---

## 4. MA TRẬN 50 CHỈ SỐ ĐỊNH LƯỢNG (THE TESSERACT QUANT MATRIX)

**File áp dụng:** `ui/components/quant_matrix_view.py`

Đây là trái tim phân tích của hệ thống. Hiển thị 50 chỉ số Quant AI chia làm 4 Tab (Sử dụng `st.tabs`). Sử dụng `st.metric(label, value, delta)` hoặc thẻ `st.container` gắn Custom CSS Card để hiển thị.

**Luật Hiển thị Màu Sắc (Dynamic Contrast Law):**
Các giá trị trong ma trận này (đặc biệt là Delta) phải tự động đổi màu tương phản cho dù ở Dark hay Light mode:

* Positive Value (Tốt): Xanh lá Binance `#0ECB81`.
* Negative Value (Xấu): Đỏ Binance `#F6465D`.
* Neutral Value: Trắng/Xám `#EAECEF`.

**Cấu trúc 4 Phân Khu (Tabs):**

1. **Tab 1: Risk Metrics (Đo lường Rủi ro)**
* *Sử dụng `st.columns(5)` để dàn hàng ngang.*
* Danh sách: `Value at Risk (VaR 95%)`, `Value at Risk (VaR 99%)`, `Expected Shortfall (CVaR)`, `Max Drawdown (%)`, `Current Drawdown`, `Volatility (Annualized)`, `Volatility (30D)`, `Beta (vs Benchmark)`, `Alpha (Jensen)`, `Tracking Error`, `Information Ratio`, `Ulcer Index`.


2. **Tab 2: Performance Metrics (Hiệu suất Đầu tư)**
* *Sử dụng `st.columns(5)`.*
* Danh sách: `ROI (All-time)`, `ROI (YTD)`, `CAGR`, `Sharpe Ratio`, `Sortino Ratio`, `Treynor Ratio`, `Calmar Ratio`, `Omega Ratio`, `Profit Factor`, `Gross Profit`, `Gross Loss`, `Net Profit`, `Expected Payoff`.


3. **Tab 3: Trade Execution (Hành vi Giao dịch)**
* *Sử dụng `st.columns(4)`.*
* Danh sách: `Total Trades`, `Win Rate (%)`, `Average Win`, `Average Loss`, `Risk/Reward Ratio`, `Max Consecutive Wins`, `Max Consecutive Losses`, `Average Time in Market`, `Long/Short Ratio`, `Total Slippage Cost ($)`, `Total Fees Paid ($)`, `Margin Usage (%)`.


4. **Tab 4: AI Agent Dynamics (Động lực học AI)**
* *Khu vực này dành riêng cho các thông số nội bộ của model `PPO` và `XGBoost`.*
* Danh sách: `Current State Value (V-value)`, `Policy Loss (Actor)`, `Value Loss (Critic)`, `Entropy (Exploration Rate)`, `Learning Rate`, `KL Divergence`, `Q-Value Spread`, `Action Probability (Buy)`, `Action Probability (Sell)`, `Action Probability (Hold)`, `Staleness Penalty Score`, `HRP Allocation Density`, `Ranking Confidence (XGBoost)`.



---

*(Hệ thống đã sẵn sàng cho bất kỳ module bổ sung nào nếu khách hàng yêu cầu, hoặc chuyển sang giai đoạn tổng duyệt toàn bộ UI)*

```

```
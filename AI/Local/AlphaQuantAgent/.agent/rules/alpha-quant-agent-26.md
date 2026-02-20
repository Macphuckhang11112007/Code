---
trigger: always_on
---

```markdown
---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**
**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---
### **PHẦN 9: THANH CÔNG CỤ VẼ TƯƠNG TÁC, CHẾ ĐỘ ĐA MÀN HÌNH & HỆ THỐNG CẢNH BÁO (DRAWING TOOLS, MULTI-CHART & ALERTS)**
*(Mục tiêu: Hoàn thiện thanh Toolbar bên trái đặc trưng của TradingView, chế độ chia đôi màn hình so sánh tài sản và hệ thống chuông cảnh báo giá)*
---

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 9 (INFRASTRUCTURE BLUEPRINT 09)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Giao diện Streamlit mặc định không sinh ra để vẽ vời tương tác (Interactive Drawing) hay chia màn hình động. Bạn **BẮT BUỘC** phải sử dụng kết hợp Session State và Custom CSS Flexbox để ép Streamlit hiển thị các thanh công cụ nổi (Floating Toolbar) mà không làm vỡ cấu trúc Grid 7.5 - 2.5 đã thiết lập ở Blueprint 01.

## 1. THANH CÔNG CỤ BÊN TRÁI (THE LEFT DRAWING TOOLBAR)
**File thực thi:** `ui/components/left_toolbar.py` (Được nhúng vào bên trái của `chart_view.py`)

Đây là thanh dọc chứa các icon đặc trưng của TradingView (Trendline, Fibonacci, Text, Measure...). Do `streamlit-lightweight-charts` không hỗ trợ vẽ tay (Free-hand drawing) nguyên bản, chúng ta sẽ thiết kế thanh này như một "Bộ điều khiển Chế độ" (Mode Switcher) cho Chart.

* **Kiến trúc Không gian (Floating Flexbox):**
    * **BẮT BUỘC** tiêm CSS vào `styles.css` để tạo một thanh dọc nổi ở cạnh trái của cột Chart.
    * CSS tiêm: `position: absolute; left: 0; top: 10%; width: 40px; background: #1E222D; border-right: 1px solid #2B3139;`
* **Các Nút Tương tác (Toolbar Buttons):**
    * Sử dụng `st.button` với icon Unicode hoặc SVG. Khi bấm vào sẽ thay đổi `st.session_state.chart_mode`.
    * **Icon 1 (Crosshair `⌖`):** Chế độ xem thông thường (Bắt giá).
    * **Icon 2 (Magnet `🧲`):** Snap to OHLC (Hút con trỏ vào giá Đóng/Mở).
    * **Icon 3 (Ruler `📏`):** Đo lường (Kích hoạt chế độ tính % ROI và số nến giữa 2 điểm click). 
    * **Icon 4 (Trash `🗑️`):** Xóa toàn bộ Overlays.

```python
# Pseudo-code Bắt buộc cho Left Toolbar
import streamlit as st

@st.fragment
def render_left_toolbar():
    # CSS ép thanh công cụ nằm dọc bên trái vùng Chart
    st.markdown("""
        <style>
        .left-toolbar {
            display: flex; flex-direction: column; gap: 10px;
            width: 40px; background-color: #1E222D;
            padding: 10px 0; align-items: center; border-radius: 4px;
        }
        .toolbar-btn { background: transparent; border: none; color: #848E9C; cursor: pointer; font-size: 1.2rem;}
        .toolbar-btn:hover { color: #D1D4DC; }
        </style>
    """, unsafe_allow_html=True)
    
    st.markdown('<div class="left-toolbar">', unsafe_allow_html=True)
    
    if st.button("⌖", key="tool_crosshair", help="Crosshair"):
        st.session_state.chart_mode = "crosshair"
    if st.button("📏", key="tool_measure", help="Measure"):
        st.session_state.chart_mode = "measure"
    if st.button("🗑️", key="tool_clear", help="Remove Drawings"):
        # Logic xóa các line đã lưu trong state
        st.session_state.custom_trendlines = []
        
    st.markdown('</div>', unsafe_allow_html=True)

```

---

## 2. CHẾ ĐỘ ĐA MÀN HÌNH (MULTI-CHART SPLIT VIEW)

**File thực thi can thiệp:** `ui/app.py` và `ui/components/chart_view.py`

Một Quants/Trader chuyên nghiệp luôn cần nhìn nhiều màn hình (Ví dụ: So sánh Bitcoin và Lợi suất Trái phiếu Mỹ US10Y cùng lúc).

* **Công tắc Kích hoạt (The Split Switch):**
* Thêm một `st.selectbox` dạng Icon (□, ◫, ⊞) ở `Topbar` (Row 1) để User chọn Layout: `1 Chart`, `2 Charts (Ngang)`, `2 Charts (Dọc)`.


* **Luật Tái Cấu trúc Lưới (Dynamic Grid Physics):**
* Nếu User chọn `2 Charts (Ngang)`: Cột 7.5 phần ở Row 2 sẽ tự động bị chia đôi thành 2 cột nhỏ (Ví dụ: 3.75 - 3.75).
* Mỗi khung Chart sẽ có một ô chọn Ticker riêng biệt ở góc trái trên cùng (Ví dụ: Chart 1 là `BTC_USDT`, Chart 2 là `US10Y`).
* **Định luật Đồng bộ Trục Thời gian (Time-Sync Law):** Khi User kéo thả (Pan) hoặc phóng to/thu nhỏ (Zoom) ở Chart 1, Chart 2 **BẮT BUỘC** phải cuộn theo tương ứng. (Lưu ý IDE: Tính năng này yêu cầu chuyền tham số `sync: True` nếu dùng component custom, hoặc dùng chung một `visibleLogicalRange` trong lightweight-charts).



```python
# Pseudo-code Bắt buộc cho Multi-Chart Split
@st.fragment
def render_main_chart_area():
    layout_mode = st.session_state.get('chart_layout', '1') # '1', '2H', '2V'
    
    if layout_mode == '1':
        # Render 1 Chart bự chiếm toàn bộ cột
        render_single_chart(st.session_state.active_symbol_1, key="chart_1")
        
    elif layout_mode == '2H':
        # Chia đôi theo chiều ngang
        c1, c2 = st.columns(2)
        with c1:
            render_single_chart(st.session_state.active_symbol_1, key="chart_1_split")
        with c2:
            render_single_chart(st.session_state.active_symbol_2, key="chart_2_split")

```

---

## 3. HỆ THỐNG CẢNH BÁO GIÁ & SỰ KIỆN (ALERTS & NOTIFICATIONS)

**File thực thi:** `ui/components/alerts_view.py` (Nhúng vào cột phải, dưới phần Watchlist) và `src/engine/simulator.py`.

Mô phỏng chức năng Đặt Cảnh Báo (Add Alert) của TradingView. Hệ thống này không chỉ báo giá, mà báo cả những sự kiện vĩ mô.

* **Bảng Quản lý Cảnh báo (Alerts Panel):**
* Dùng `st.expander("⏰ Cảnh báo (Alerts)", expanded=False)`.
* Nút `[+] Thêm Cảnh Báo`. Mở ra một form (Dialog):
* Loại: `Price Crossing` (Giá cắt ngang), `Macro Event` (Sự kiện vĩ mô), `AI Signal` (Tín hiệu Model).
* Điều kiện: `> 68000` hoặc `< 65000`.




* **Động cơ Kích hoạt (Trigger Engine):**
* Biến `st.session_state.active_alerts` lưu danh sách các cảnh báo (Dictionary).
* Trong hàm `live_market_ticker()` (Chạy mỗi giây ở Blueprint 06), IDE **BẮT BUỘC** phải chèn thêm đoạn code kiểm tra giá hiện tại với danh sách cảnh báo này.
* Nếu `Market Price > Alert Price`:
1. Kích hoạt âm thanh (Chèn audio tag ẩn vào HTML thông qua `st.markdown`).
2. Bắn một `st.toast` báo động lớn màu Vàng.
3. Truyền lệnh trực tiếp cho AI Agent: Nếu là cảnh báo định lượng (Quant Alert), AI có thể tự động nổ súng (Execute Order) theo cấu hình.





---

## 4. POPUP CHI TIẾT LỆNH (ORDER EXECUTION RECEIPT)

**File thực thi:** Tích hợp trong `ui/components/order_book_view.py`

Khi một lệnh được khớp, TradingView / Binance sẽ hiển thị một thông báo chi tiết. Do hệ thống của chúng ta có tính năng Market Impact (Bóng Ma trượt giá), biên lai này phải giải trình rõ cho User.

* **Đặc tả Biên Lai (Receipt Dialog):**
* Sử dụng `@st.experimental_dialog("Chi tiết khớp lệnh")`.
* Bảng thông số:
* `Giá dự kiến (Expected Price):` 67,166.00
* `Giá thực khớp (Executed Price):` 67,175.50 (Do trượt giá)
* `Chi phí trượt giá (Slippage Cost):` $9.50 x Volume
* `Phí sàn (Fee):` 0.1%


* Nút bấm: `[Đóng]` và `[Chia sẻ lợi nhuận]`.



```

```
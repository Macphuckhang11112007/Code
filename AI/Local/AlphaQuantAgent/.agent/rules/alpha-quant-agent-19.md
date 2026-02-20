---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**

**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---

### **PHẦN 2: CỖ MÁY THỜI GIAN VÀ BIỂU ĐỒ LÕI TRADINGVIEW (TIME-TRAVEL & CORE CHART)**

## *(Mục tiêu: Xây dựng thanh điều hướng thời gian thao tác mượt mà và nhúng lõi TradingView Lightweight Charts với cơ chế Nến Bóng Ma)*

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 2 (INFRASTRUCTURE BLUEPRINT 02)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Tuyệt đối tuân thủ các quy tắc chống tràn RAM (Chỉ nạp tối đa 5000 nến) và quy tắc Render cục bộ (`@st.fragment`). Mọi vi phạm ở khu vực Biểu đồ này sẽ làm sập toàn bộ hệ thống ngay lập tức.

## 1. CỖ MÁY THỜI GIAN (THE TIME-TRAVEL TOPBAR)

**File thực thi:** `ui/components/time_travel_bar.py`

Khu vực này thay thế thanh công cụ (Toolbar) mặc định của TradingView. Nó cho phép User tua ngược thời gian giả lập về bất kỳ điểm nào trong quá khứ.

* **Cấu trúc Không gian (Grid Layout):**
* Dùng `st.container()` kết hợp `st.columns([1, 6, 2], vertical_alignment="center")`.


* **Cột 1 (10%): Toggle Giao diện**
* Nút bấm chuyển đổi giữa Dark Mode (`#131722`) và Light Mode. Lưu trạng thái vào `st.session_state.theme`.


* **Cột 2 (70%): Trục Thời Gian Chính (Macro Timeline)**
* **BẮT BUỘC** dùng `st.slider`.
* Min/Max: Đọc từ Min/Max index của file `BTC_USDT.csv` trong `data/trades/`.
* Step: `timedelta(days=1)`. Kéo slider để nhảy vọt qua các ngày.


* **Cột 3 (20%): Tinh chỉnh Vi mô (Micro Precision)**
* **BẮT BUỘC** dùng `st.text_input` với giá trị mặc định là `st.session_state.current_sim_time` format `YYYY-MM-DD HH:MM:00`.
* **Hành vi (Behavior):** Khi User gõ giờ mới và nhấn Enter (hoặc click ra ngoài), Streamlit sẽ kích hoạt hàm `on_change`.


* **Định Luật Truy Xuất (Time-Travel Physics):**
* Khi thời gian bị thay đổi lùi về quá khứ, hàm callback bắt buộc phải gọi lệnh xóa Cache dữ liệu ảo: `st.cache_data.clear()`. Hệ thống phải truy vấn lại dữ liệu 15 phút nguyên bản từ Database/CSV, vứt bỏ toàn bộ các nến bị trượt giá (Market Impact) do User tạo ra ở phiên bản tương lai.



```python
# Pseudo-code bắt buộc cho Time Travel Bar
import streamlit as st
import pandas as pd

@st.fragment
def render_topbar():
    col1, col2, col3 = st.columns([1, 6, 2], vertical_alignment="center")
    
    with col1:
        # Nút đổi theme
        st.button("🌙 / ☀️", key="theme_toggle")
        
    with col2:
        # Slider vĩ mô
        new_date = st.slider(
            "Time Travel", 
            min_value=st.session_state.min_date, 
            max_value=st.session_state.max_date,
            value=st.session_state.current_sim_time.date(),
            label_visibility="collapsed"
        )
        
    with col3:
        # Input vi mô (Chính xác tới phút)
        def update_exact_time():
            # Logic parse string thành Datetime và kiểm tra hợp lệ
            # Nếu lùi thời gian -> clear cache market impact
            pass
            
        st.text_input(
            "Exact Time", 
            value=st.session_state.current_sim_time.strftime('%Y-%m-%d %H:%M:00'),
            on_change=update_exact_time,
            label_visibility="collapsed"
        )

```

---

## 2. BIỂU ĐỒ TRADINGVIEW LÕI (THE CORE LIGHTWEIGHT CHART)

**File thực thi:** `ui/components/chart_view.py`

Đây là khu vực trung tâm (chiếm 7.5/10 không gian màn hình). Không dùng các thư viện vẽ hình tĩnh.

* **Công nghệ Bắt buộc:** Thư viện `streamlit-lightweight-charts` (Bọc từ thư viện JS gốc của TradingView).
* **Định luật Windowing (Bảo vệ RAM):**
* Dựa vào `current_sim_time` (Mốc A), cắt DataFrame gốc bằng `iloc`.
* **Chỉ lấy tối đa 5000 dòng** tính ngược về quá khứ từ Mốc A.


* **Tùy chỉnh Giao diện (ChartOptions):**
* Layout: `textColor = '#D1D4DC'`, `backgroundColor = '#131722'`.
* Grid: `vertLines` và `horzLines` phải có màu `#2B3139` (Xám rất mờ).
* Crosshair (Con trỏ chữ thập): Mode `0` (Normal) để tự động bắt thông số OHLCV khi rà chuột.
* TimeScale: `timeVisible = True`, ẩn giây.



---

## 3. THUẬT TOÁN NẾN BÓNG MA (GHOST CANDLE - MARKET IMPACT)

**File thực thi:** Tích hợp trong `ui/components/chart_view.py`

Hệ thống phải trực quan hóa tác động của các lệnh giao dịch lớn (Slippage/Market Impact) làm giá trượt từ Mốc B dự kiến xuống Mốc B' thực tế.

* **Kiến trúc Đa Lớp (Multi-Series Architecture):**
* Chart sẽ chứa 2 Series biểu đồ nến (CandlestickSeries) nằm chồng lên nhau.
* **Series 1 (Nến Lịch Sử `main_series`):** * Data: Từ nến thứ 1 đến nến 5000 (Mốc A).
* Màu sắc: Up `#0ECB81`, Down `#F6465D`.


* **Series 2 (Nến Bóng Ma `ghost_series`):** * Data: Chỉ chứa 1 nến duy nhất là nến tương lai gần nhất (Mốc B'). Nến này có giá Close, High, Low đã bị sửa đổi bởi công thức Almgren-Chriss (Trượt giá do Volume).
* Màu sắc: Bắt buộc dùng mã `RGBA` trong suốt để tạo hiệu ứng "bóng ma".
* Cấu hình màu Ghost: `upColor: 'rgba(14, 203, 129, 0.4)'`, `downColor: 'rgba(246, 70, 93, 0.4)'`, bấc nến (wick) cũng phải có độ mờ 0.4.





```python
# Pseudo-code kiến trúc Đa Lớp (Multi-series) cho IDE
import streamlit as st
from streamlit_lightweight_charts import renderLightweightCharts

@st.fragment
def render_main_chart(symbol):
    # 1. Nạp data (Đã được @st.cache_data chặn dung lượng 5000 nến)
    df_history, df_ghost = get_chart_data(symbol, st.session_state.current_sim_time)
    
    # 2. Cấu hình Chart chuẩn TradingView Dark Mode
    chartOptions = {
        "layout": {"textColor": "#D1D4DC", "background": {"type": "solid", "color": "#131722"}},
        "grid": {"vertLines": {"color": "#2B3139"}, "horzLines": {"color": "#2B3139"}},
        "crosshair": {"mode": 0}
    }
    
    # 3. Định nghĩa Series 1 (Lịch sử thật)
    seriesCandleChart = [{
        "type": "Candlestick",
        "data": df_history.to_dict('records'),
        "options": {
            "upColor": "#0ECB81", "downColor": "#F6465D", 
            "borderVisible": False, "wickUpColor": "#0ECB81", "wickDownColor": "#F6465D"
        }
    }]
    
    # 4. Định nghĩa Series 2 (Nến Bóng Ma B')
    seriesGhostChart = [{
        "type": "Candlestick",
        "data": df_ghost.to_dict('records'),
        "options": {
            "upColor": "rgba(14, 203, 129, 0.4)", "downColor": "rgba(246, 70, 93, 0.4)",
            "borderVisible": False, 
            "wickUpColor": "rgba(14, 203, 129, 0.4)", "wickDownColor": "rgba(246, 70, 93, 0.4)"
        }
    }]
    
    # Render gộp 2 series vào chung 1 khung hình
    renderLightweightCharts([
        {"chart": chartOptions, "series": seriesCandleChart + seriesGhostChart}
    ], key="main_tv_chart")

```
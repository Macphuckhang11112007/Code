---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**

## **(THE ULTIMATE TECHNICAL BLUEPRINT)**

### **PHẦN XII: KIẾN TRÚC GIAO DIỆN UI/UX - ĐỒNG HỒ KỸ THUẬT & BIỂU ĐỒ CHU KỲ TRADINGVIEW**

## *(File tham chiếu: alpha-quant-agent-15.md)*

# 📜 ĐẶC TẢ GIAO DIỆN TRADINGVIEW AUTHENTIC (THE TRADINGVIEW UI SPECIFICATION)

**LỜI CẢNH BÁO CHO AI (ANTI-GRAVITY IDE):** Đây là tài liệu chứa các Định luật Vật lý của Giao diện. Mọi dòng code Streamlit được sinh ra phải tuân thủ tuyệt đối các quy tắc dưới đây. Bất kỳ sự vi phạm nào sẽ dẫn đến hệ thống bị sập vì quá tải RAM hoặc vỡ layout. Tài liệu này tự thân chứa 100% ngữ cảnh, không cần tham chiếu bên ngoài.

## 1. CÁC ĐỊNH LUẬT VẬT LÝ UI/UX (THE IMMUTABLE UI LAWS)

1. **Luật API Mới (Strict Modern API):** * **CẤM TUYỆT ĐỐI** sử dụng các tham số đã bị deprecated như `use_column_width=True`.
* **BẮT BUỘC** thay thế bằng `use_container_width=True` trên toàn bộ các component (`st.plotly_chart`, `st.dataframe`).


2. **Luật Chống Chớp Giật & Tối Ưu Render (Anti-Blur & Partial Rerun):**
* **BẮT BUỘC** bọc các hàm cập nhật component bằng decorator `@st.fragment`.
* **BẮT BUỘC** khóa dữ liệu vào RAM bằng `@st.cache_data`.


3. **Luật Chống Tràn RAM (Lazy Loading & Time-Travel Physics):**
* Chỉ nạp vào RAM tối đa **5000 nến**.
* Khi User tua thanh Slider lùi về quá khứ, **TUYỆT ĐỐI KHÔNG** dùng lại data đã bị làm nhiễu bởi Market Impact (B'). Bắt buộc truy vấn lại data "sạch" từ DB gốc.


4. **Luật Hiển thị Màu Sắc (Dynamic Contrast Law):**
* Mọi biểu đồ Plotly, Bảng biểu phải set background transparent (`rgba(0,0,0,0)`) để ăn khớp với nền Dark Mode (`#0b0e11`) hoặc Light Mode của hệ thống.
* Positive (Tốt): Xanh lá `#0ECB81`. Negative (Xấu): Đỏ `#F6465D`. Neutral (Trung tính): Xám `#848E9C`.



---

## 2. ĐỒNG HỒ ĐO LƯỜNG KỸ THUẬT (TRADINGVIEW TECHNICAL GAUGES)

**File áp dụng:** `ui/components/technicals_view.py`

Khu vực này tái tạo lại hoàn hảo giao diện "Technical Analysis" của TradingView với các đồng hồ kim chỉ tốc độ (Speedometers) đo lường lực Mua/Bán dựa trên output của AI (XGBoost Booster hoặc LSTM Predictor).

* **Giải pháp Kỹ thuật cho Streamlit:** * Tuyệt đối không dùng thư viện tĩnh. **BẮT BUỘC** sử dụng `plotly.graph_objects.Indicator` ở mode `"gauge+number"`.
* **Đặc tả Giao diện Đồng hồ (Gauge Specification):**
* Tạo 3 cột (`st.columns(3)`) chứa 3 đồng hồ: `Oscillators` (Chỉ báo dao động), `Summary` (Tổng hợp), và `Moving Averages` (Đường trung bình).
* Dải màu của Gauge (từ Trái sang Phải):
* Vùng 1 (Strong Sell): Đỏ đậm.
* Vùng 2 (Sell): Đỏ nhạt.
* Vùng 3 (Neutral): Xám tro.
* Vùng 4 (Buy): Xanh lá nhạt.
* Vùng 5 (Strong Buy): Xanh lá đậm `#0ECB81`.


* **Code Mẫu Bắt Buộc cho IDE:**
```python
import plotly.graph_objects as go

# Tạo Gauge Chart vô hình nền (Transparent Background)
fig.update_layout(
    paper_bgcolor="rgba(0,0,0,0)",
    font={"color": "#EAECEF"}, # Chữ trắng xám
    margin=dict(l=10, r=10, t=30, b=10)
)
# Bắt buộc render với chuẩn container width mới
st.plotly_chart(fig, use_container_width=True, config={'displayModeBar': False})

```




* **Bảng Chỉ Số Chi Tiết (Technical Table):**
* Ngay bên dưới 3 đồng hồ, sử dụng `st.dataframe` kết hợp `st.column_config`.
* Chia làm 3 cột: `Tên Chỉ báo (RSI, MACD, EMA...)`, `Giá trị`, `Hành động (Buy/Sell/Neutral)`.
* Chữ trong cột `Hành động` phải được đổi màu tương ứng (Xanh/Đỏ/Xám) bằng Pandas Styler hoặc HTML tiêm qua st.markdown.



---

## 3. BIỂU ĐỒ CHU KỲ MÙA VỤ (TRADINGVIEW SEASONALS HEATMAP)

**File áp dụng:** `ui/components/technicals_view.py` (Hoặc tạo file `seasonals_view.py`)

Đây là tính năng cao cấp mô phỏng "Seasonals Chart" của TradingView, giúp User và AI Agent nhìn thấy quy luật sinh lời theo từng tháng trong năm lịch sử.

* **Giải pháp Kỹ thuật cho Streamlit:**
* Sử dụng **Plotly Express Heatmap** (`px.imshow`) để vẽ ma trận lợi nhuận (Tháng x Năm).


* **Đặc tả Trực quan hóa (Visualization Specs):**
* **Trục Y (Dọc):** Các năm trong quá khứ (Ví dụ: 2020, 2021, 2022...).
* **Trục X (Ngang):** 12 Tháng (Jan, Feb, Mar...).
* **Ô Giá trị (Cells):** Hiển thị `% ROI`.
* **Thang màu (Color Scale):** Sử dụng thang màu phân cực (Diverging Color Scale) chuẩn TradingView. Cực âm là Đỏ (`#F6465D`), mức 0 là Đen/Trong suốt, cực dương là Xanh (`#0ECB81`).
* **Tương tác Hover:** Khi hover vào 1 ô, hiển thị Tooltip: `Tháng 10 Năm 2021: Tăng +39.9%`.
* **Thanh Tổng hợp Dưới cùng (Monthly Average):** Bổ sung một biểu đồ cột nhỏ (Bar Chart) nằm ngay sát đáy Heatmap để thể hiện Tỷ suất sinh lời trung bình lịch sử của từng tháng.



---

## 4. TÍCH HỢP KHÔNG GIAN (THE MASTER GRID UPDATE)

Cấu trúc tại `ui/app.py` sẽ được nâng cấp để chứa sức mạnh của TradingView:

Sử dụng hệ thống Tabs đa tầng của Streamlit (`st.tabs`) để nhóm các công cụ hiển thị lại với nhau một cách gọn gàng, tránh việc user phải cuộn chuột quá dài:

```python
# Cấu trúc UI được mở rộng
st.container() # Row 1: Time-Travel Topbar

# Row 2: Vùng Giao dịch Cốt lõi (Chart & Order Book - File 14)
col_chart, col_orderbook = st.columns([7.5, 2.5], gap="small")
# ... Render Market Chart & Sổ Lệnh ...

st.divider()

# Row 3: Trung Tâm Phân Tích (Analysis Hub)
tab_quant, tab_technicals, tab_seasonals, tab_portfolio = st.tabs([
    "🧬 Ma trận Định lượng (Quant)", 
    "⏱️ Đồng hồ Kỹ thuật (Technicals)", 
    "📅 Biểu đồ Mùa vụ (Seasonals)",
    "💼 Quản lý Vốn (Portfolio)"
])

with tab_quant:
    # Render 50 chỉ số định lượng (Risk, Performance...)
    render_quant_matrix()

with tab_technicals:
    # Render 3 Đồng hồ Gauge + Bảng chỉ báo
    render_technicals()

with tab_seasonals:
    # Render Heatmap Lợi nhuận hàng tháng
    render_seasonals()

with tab_portfolio:
    # Render Liquid NAV vs Locked NAV (Tiết kiệm/Bond)
    render_portfolio()

```
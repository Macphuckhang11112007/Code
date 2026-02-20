---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**

**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---

### **PHẦN 4: ĐỒNG HỒ KỸ THUẬT, BẢN ĐỒ MÙA VỤ & TRÌNH LỌC TÀI SẢN (TECHNICALS & SCREENER)**

## *(Mục tiêu: Tái tạo giao diện Technical Analysis, Seasonals Chart và ETF Screener của TradingView)*

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 4 (INFRASTRUCTURE BLUEPRINT 04)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Mọi biểu đồ Plotly được sử dụng ở phần này bắt buộc phải thiết lập nền trong suốt (`paper_bgcolor="rgba(0,0,0,0)"`) để không làm vỡ nền Dark Mode `#131722` của hệ thống. Phải dùng `@st.fragment` để cách ly sự kiện render.

## 1. ĐỒNG HỒ KỸ THUẬT & TÍN HIỆU (TRADINGVIEW TECHNICALS)

**File thực thi:** `ui/components/technicals_view.py`

Khu vực này tương ứng với Tab "Đồng hồ Kỹ thuật" ở `Row 3` của Layout tổng, mô phỏng trang Technical Analysis của TradingView.

* **Kiến trúc Đồng hồ (Speedometer Gauges):**
* Dùng `st.columns(3)` để đặt 3 đồng hồ hàng ngang: `Oscillators` (Dao động), `Summary` (Tổng hợp), `Moving Averages` (Trung bình động).
* **BẮT BUỘC** dùng `plotly.graph_objects.Indicator` với cấu hình `mode="gauge+number+delta"`.
* **Thang điểm Gauge:** Từ -100 (Strong Sell) đến +100 (Strong Buy). Mức 0 là Neutral.
* **Dải màu TradingView (Gauge Steps):**
* `[-100, -50]`: Đỏ đậm `#F6465D`.
* `[-50, -10]`: Đỏ nhạt `rgba(246, 70, 93, 0.5)`.
* `[-10, 10]`: Xám Neutral `#848E9C`.
* `[10, 50]`: Xanh nhạt `rgba(14, 203, 129, 0.5)`.
* `[50, 100]`: Xanh đậm `#0ECB81`.




* **Bảng Chi tiết Chỉ báo (Technical Indicators Table):**
* Nằm ngay dưới 3 đồng hồ.
* Dùng `st.dataframe` hoặc `st.table`.
* Cột 1: Tên chỉ báo (Ví dụ: RSI(14), MACD, EMA(20)).
* Cột 2: Giá trị (Value).
* Cột 3: Hành động (Action - Buy/Sell/Neutral). **Phải dùng `st.column_config.TextColumn` kết hợp Pandas Styler** để tô màu chữ: Chữ Buy màu Xanh `#0ECB81`, chữ Sell màu Đỏ `#F6465D`.



```python
# Pseudo-code Bắt buộc cho Gauge Chart
import plotly.graph_objects as go
import streamlit as st

@st.fragment
def render_technicals(symbol):
    st.markdown("<h4 style='color:#D1D4DC;'>Technical Analysis</h4>", unsafe_allow_html=True)
    col1, col2, col3 = st.columns(3)
    
    # Logic nạp data và tính toán AI (XGBoost/Predictor)
    # ...
    
    def create_gauge(title, value):
        fig = go.Figure(go.Indicator(
            mode = "gauge+number",
            value = value,
            title = {'text': title, 'font': {'color': '#848E9C', 'size': 16}},
            number = {'font': {'color': '#D1D4DC'}},
            gauge = {
                'axis': {'range': [-100, 100], 'tickwidth': 1, 'tickcolor': "#2B3139"},
                'bar': {'color': "#D1D4DC", 'thickness': 0.2}, # Kim chỉ
                'bgcolor': "rgba(0,0,0,0)",
                'steps': [
                    {'range': [-100, -50], 'color': "#F6465D"},
                    {'range': [-50, -10], 'color': "rgba(246, 70, 93, 0.3)"},
                    {'range': [-10, 10], 'color': "#2B3139"},
                    {'range': [10, 50], 'color': "rgba(14, 203, 129, 0.3)"},
                    {'range': [50, 100], 'color': "#0ECB81"}
                ]
            }
        ))
        fig.update_layout(paper_bgcolor="rgba(0,0,0,0)", margin=dict(t=30, b=10, l=10, r=10), height=250)
        return fig

    with col2: # Ví dụ đặt Summary ở giữa
        st.plotly_chart(create_gauge("Summary", 45), use_container_width=True, config={'displayModeBar': False})

```

---

## 2. BẢN ĐỒ CHU KỲ MÙA VỤ (SEASONALS HEATMAP)

**File thực thi:** Tích hợp vào `ui/components/technicals_view.py` hoặc tạo file riêng.

Tái tạo biểu đồ tính mùa vụ (Seasonals Chart) để xem quy luật sinh lời theo tháng.

* **Cấu trúc Dữ liệu (Matrix Pivot):**
* IDE phải viết hàm Pandas dùng `pivot_table` để biến chuỗi Time-series gốc thành Ma trận 2D: Index là `Năm`, Columns là `Tháng` (Jan -> Dec), Values là `% ROI`.


* **Trực quan hóa bằng Plotly Heatmap (`px.imshow`):**
* Bắt buộc dùng Thang màu phân cực (Diverging Color Scale). Điểm 0% cố định là màu đen/trong suốt. Dương là dải Xanh lá, Âm là dải Đỏ.
* Text (Annotations): Hiển thị trực tiếp con số `%` trên từng ô của Heatmap.
* Trục Y: Đảo ngược (Năm mới nhất nằm trên cùng).



---

## 3. TRÌNH LỌC TÀI SẢN & SO SÁNH (MARKETS & ETF SCREENER)

**File thực thi:** `ui/components/screener_view.py`

Mô phỏng trang "Markets: Exchanges & Key Data" và "Bitcoin ETFs List" của TradingView. Cho phép User quét toàn bộ các file CSV trong thư mục `data/` thay vì chỉ xem 1 mã.

* **Bố cục (Screener Layout):**
* Nằm ở Tab "Trình lọc Ticker".
* Thanh công cụ trên cùng: Dùng `st.text_input` làm ô Search Ticker, và `st.selectbox` để lọc (All / Chỉ Crypto / Chỉ Rate-Banks / Chỉ Vĩ mô).


* **Bảng Siêu Dữ Liệu (The Master Screener Table):**
* Dùng `st.dataframe` quét toàn bộ Asset.
* **Cột "Ticker":** In đậm.
* **Cột "7D Trend" (Sparkline):** **BẮT BUỘC** dùng `st.column_config.LineChartColumn`. Chuyền array giá 7 ngày vào để vẽ đường biểu đồ nhỏ ngay trong bảng.
* **Cột "Capital Allocated" (Vốn đang phân bổ):** Dùng `st.column_config.ProgressColumn` để vẽ thanh Bar thể hiện mức độ tỷ trọng NAV đang nằm ở mã này.
* **Hành vi Tương tác (Interactivity):** Bật `on_select="rerun"`. Khi User click vào dòng `NVDA` trong bảng, hệ thống lập tức cập nhật `st.session_state.active_symbol = "NVDA"` và load lại Main Chart ở phía trên.


* **Biểu đồ Dòng Tiền (Fund Flows Treemap):**
* Nằm bên cạnh Bảng Screener (Chia cột 7-3).
* Dùng `plotly.express.treemap`. Các khối đại diện cho các Ticker. Kích thước khối = Tổng vốn phân bổ. Màu sắc = Biến động giá 24h. Giúp User nhìn nhanh xem tiền đang tập trung ở đâu.
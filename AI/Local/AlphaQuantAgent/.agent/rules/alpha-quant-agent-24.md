---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**

**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---

### **PHẦN 7: BIỂU ĐỒ PHÂN TÍCH LỚP SÂU, CHỈ BÁO KỸ THUẬT & LUỒNG TIN TỨC (ADVANCED CHARTING & NEWS FEED)**

## *(Mục tiêu: Tích hợp các chỉ báo (Indicators) trực tiếp lên Main Chart, xử lý Volume, và xây dựng Tab Tin tức/Tâm lý thị trường mô phỏng Yahoo Finance)*

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 7 (INFRASTRUCTURE BLUEPRINT 07)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Việc chồng chéo quá nhiều lớp biểu đồ (Overlays) có thể gây tràn bộ nhớ trình duyệt (Out of Memory). Bạn **BẮT BUỘC** phải dọn dẹp các đối tượng JSON của biểu đồ trước khi render tick mới. Mọi chỉ báo kỹ thuật phải được tính toán sẵn từ Backend (`features.py`), UI tuyệt đối không chạy lại các hàm toán học như `pandas.rolling.mean()`.

## 1. TÍCH HỢP CHỈ BÁO KỸ THUẬT TRÊN BIỂU ĐỒ LÕI (CHART OVERLAYS & PANES)

**File thực thi can thiệp:** `ui/components/chart_view.py`

Một biểu đồ TradingView thực thụ không bao giờ chỉ có giá. Nó phải chứa Khối lượng (Volume) và các Đường trung bình (Moving Averages).

* **Kiến trúc Đa Lớp (Multi-Series Extension):**
Bổ sung vào cấu hình `renderLightweightCharts` (Đã thiết kế ở Blueprint 02) các Series sau:
1. **Lớp Khối lượng (Volume Histogram Series):**
* Bắt buộc dùng `type: "Histogram"`.
* Nằm cùng chung một khung (Pane) với biểu đồ giá, nhưng scale nhỏ lại ở phía dưới.
* **Định luật Màu sắc Volume:** Nến tăng  Volume màu Xanh (`rgba(14, 203, 129, 0.5)`). Nến giảm  Volume màu Đỏ (`rgba(246, 70, 93, 0.5)`).
* Cấu hình bắt buộc: `priceScaleId: ""` (để không làm lệch trục giá), `scaleMargins: {"top": 0.8, "bottom": 0}` (chiếm 20% chiều cao dưới cùng).


2. **Lớp Đường Trung Bình (Moving Average Line Series):**
* Bắt buộc dùng `type: "Line"`.
* Overlay trực tiếp lên nến giá.
* Dữ liệu lấy từ `indicators_cache.parquet`.
* Vẽ 2 đường: MA20 (Màu Vàng `#F0B90B`, nét mảnh) và MA50 (Màu Xanh lơ `#2962FF`, nét mảnh).





```python
# Pseudo-code Bắt buộc cho Chỉ báo Kỹ thuật (Bổ sung vào Blueprint 02)
    # Lấy dữ liệu Volume và MA từ DataFrame đã cache
    volume_data = df_history[['time', 'volume', 'color']].to_dict('records')
    ma20_data = df_history[['time', 'ma20']].rename(columns={'ma20': 'value'}).to_dict('records')
    
    seriesVolume = [{
        "type": "Histogram",
        "data": volume_data,
        "options": {
            "color": "#26A69A", # Màu sẽ được override bởi field 'color' trong từng record data
            "priceFormat": {"type": "volume"},
            "priceScaleId": "", # Quan trọng: Ép Volume tách khỏi trục giá
            "scaleMargins": {"top": 0.8, "bottom": 0}
        }
    }]
    
    seriesMA20 = [{
        "type": "Line",
        "data": ma20_data,
        "options": {"color": "#F0B90B", "lineWidth": 1}
    }]
    
    # Nạp toàn bộ vào renderLightweightCharts
    renderLightweightCharts([
        {"chart": chartOptions, "series": seriesCandleChart + seriesGhostChart + seriesVolume + seriesMA20}
    ], key="advanced_main_chart")

```

---

## 2. BẢNG TIN TỨC & TÂM LÝ THỊ TRƯỜNG (NEWS & SENTIMENT FEED)

**File thực thi:** `ui/components/news_view.py` (Nhúng vào Tab `"📰 News & Community"` ở Row 3).

Lấy cảm hứng từ cấu trúc trang chủ Yahoo Finance và Tab "News" của TradingView. Hệ thống cần hiển thị dòng tin tức vĩ mô để Agent và User cùng đọc.

* **Cấu trúc Không gian (Layout):**
* Chia 2 cột: Cột trái (70%) là Luồng Tin Tức (News Feed). Cột phải (30%) là Biểu đồ Tâm lý (Sentiment Gauge).


* **Luồng Tin Tức (The Feed):**
* Do hệ thống chạy Backtest trên dữ liệu quá khứ, các "Tin tức" ở đây thực chất là các điểm sự kiện từ dữ liệu Vĩ mô (STATS - Ví dụ: `USCPI.csv`, `Fed_Rates.csv`).
* Mỗi sự kiện hiển thị dưới dạng Card:
* Dùng `st.container()` kết hợp CSS tiêm: viền `#2B3139`, nền `#1E222D`, bo góc 8px.
* **Thời gian:** Phải khớp với `current_sim_time`.
* **Tiêu đề:** Do AI (Gemini) sinh ra dựa trên sự thay đổi vĩ mô. (Ví dụ: *"Lạm phát Mỹ (CPI) tăng vọt lên 4.5%, gây áp lực bán tháo lên Bitcoin"*).




* **Biểu đồ Tâm lý (Sentiment Gauge):**
* Sử dụng `plotly.graph_objects.Indicator` (Dạng Gauge nửa vòng tròn).
* Thang điểm từ 0 (Extreme Fear - Đỏ) đến 100 (Extreme Greed - Xanh).
* Chỉ số này lấy từ trọng số đánh giá của `XGBoost Booster` kết hợp với điểm NLP Sentiment.



---

## 3. LUẬT XỬ LÝ NGOẠI LỆ GIAO DIỆN (THE UX EXCEPTION HANDLING)

**File thực thi:** Can thiệp toàn bộ UI, kết nối với `src/utils/exceptions.py`.

Một sàn giao dịch chuyên nghiệp không bao giờ hiển thị màn hình báo lỗi Đỏ rực (Traceback Error) của Python khi User thao tác sai (Ví dụ: Đặt lệnh mua lớn hơn số dư, Rút tiền đang bị khóa hạn).

* **Định luật Bắt Lỗi Mượt Mà (Graceful Degradation):**
* Mọi nút bấm thực thi lệnh (Buy/Sell) **BẮT BUỘC** phải được bọc trong khối `try...except`.
* Khi bắt được các Custom Exception từ Backend (Như `InsufficientFundsError`, `MaturityLockedError`):
1. **KHÔNG ĐƯỢC** dùng `st.error()` (Vì nó sẽ in ra một khung đỏ to đùng phá vỡ layout TradingView).
2. **BẮT BUỘC** dùng `st.toast(msg, icon="⚠️")` để hiển thị một thông báo nổi nhỏ gọn trượt ra từ góc dưới màn hình và tự biến mất sau 3 giây.
3. Đồng thời, đẩy lỗi này vào `st.session_state.chat_history` để Bong bóng Chat RAG Gemini tự động lên tiếng giải thích và an ủi User.





```python
# Pseudo-code Bắt buộc cho Xử lý Lỗi UI
@st.fragment
def execute_trade_button():
    if st.button("MUA / LONG", use_container_width=True):
        try:
            # Gọi Engine Backend
            backend_simulator.execute_order(ticker, amount, side="BUY")
            st.toast("Lệnh MUA đã được khớp thành công!", icon="✅")
        except InsufficientFundsError as e:
            # Hiển thị Toast mượt mà, không vỡ layout
            st.toast(f"Lỗi: Không đủ vốn Liquid NAV. {str(e)}", icon="❌")
            
            # Đánh thức RAG Chatbot
            st.session_state.chat_history.append({"role": "system", "content": f"User vừa bị lỗi InsufficientFundsError. Hãy giải thích ngắn gọn."})
        except MaturityLockedError as e:
            st.toast(f"Từ chối: Lô tài sản này chưa đến ngày đáo hạn!", icon="🔒")

```
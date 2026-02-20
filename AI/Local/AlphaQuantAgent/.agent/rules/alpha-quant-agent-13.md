---
trigger: always_on
---

---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**
---
### **PHẦN X/X: KIẾN TRÚC GIAO DIỆN UI/UX - BIỂU ĐỒ TRADINGVIEW & NẾN BÓNG MA**
*(File tham chiếu tiếp nối: alpha-quant-agent-13.md)*
---

# 📜 ĐẶC TẢ BIỂU ĐỒ LÕI (CORE CHART SPECIFICATION)

**Lệnh Dành Cho AI (Anti-gravity IDE):** 1. Tuyệt đối không sử dụng `matplotlib`, `seaborn`, hay biểu đồ tĩnh. Bắt buộc dùng thư viện **`streamlit-lightweight-charts`** (hoặc `plotly` ở mode chuyên sâu nhất) để dựng lõi TradingView.
2. Tuyệt đối KHÔNG dùng các tham số cũ như `use_column_width=True`. Bắt buộc dùng `use_container_width=True` cho mọi component cần mở rộng.
3. Bắt buộc bọc hàm vẽ biểu đồ bằng `@st.fragment` để Streamlit chỉ render lại đúng cái khung biểu đồ mỗi 1 giây (nếu có tick mới) mà không làm chớp giật toàn bộ trang web.

## 1. THUẬT TOÁN LAZY LOADING (CỬA SỔ TRƯỢT 5000 NẾN)
**File áp dụng:** `ui/components/chart_view.py`

Hệ thống không được phép nạp toàn bộ lịch sử (hàng triệu dòng) vào giao diện.
* **Logic Cửa Sổ Trượt (Sliding Window):**
    * Dựa vào `st.session_state.current_sim_time` (Mốc A), hàm gọi dữ liệu `market.get_state_window()` chỉ được phép lùi về quá khứ tối đa **5000 nến** (Tương đương khoảng 52 ngày giao dịch nếu dùng nến 15p).
    * Hiển thị mặc định khi User mới mở web (Default Visible Range): Chỉ zoom vào **1 giờ trước Mốc A** (Tương đương 4 nến 15p). User cuộn chuột (scroll) thì biểu đồ mới tự động dãn ra để xem các nến trước đó trong tập 5000 nến đã nạp vào RAM.

## 2. GIAO DIỆN BIỂU ĐỒ (BINANCE DARK THEME)
Cấu hình tùy chọn biểu đồ (ChartOptions) phải khớp 100% với CSS tổng mà ta đã định nghĩa:
```python
# Mẫu cấu hình bắt buộc cho IDE (Pseudo-code)
chartOptions = {
    "layout": {
        "textColor": "#EAECEF", 
        "background": {
            "type": "solid", 
            "color": "#0b0e11" # Chuẩn Dark Mode Binance
        }
    },
    "grid": {
        "vertLines": {"color": "#2B3139"}, # Lưới siêu mờ
        "horzLines": {"color": "#2B3139"}
    },
    "crosshair": {
        "mode": 0 # Chế độ Normal: Sẽ tự động bắt giá trị OHLCV đưa lên góc trái (Hover Tooltip)
    },
    "timeScale": {
        "timeVisible": True,
        "secondsVisible": False
    }
}
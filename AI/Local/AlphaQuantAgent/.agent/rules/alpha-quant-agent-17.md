---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**

## **(THE ULTIMATE TECHNICAL BLUEPRINT)**

### **PHẦN XIV: KIẾN TRÚC GIAO DIỆN UI/UX - TRÌNH LỌC TÀI SẢN & SO SÁNH DÒNG TIỀN (TRADINGVIEW ETFS & SCREENER)**

## *(File tham chiếu: alpha-quant-agent-17.md)*

# 📜 ĐẶC TẢ GIAO DIỆN TRADINGVIEW SCREENER & ETFS (THE ASSET SCREENER SPECIFICATION)

**LỜI CẢNH BÁO CHO AI (ANTI-GRAVITY IDE):** Đây là tài liệu chứa các Định luật Vật lý của Giao diện. Mọi dòng code Streamlit được sinh ra phải tuân thủ tuyệt đối các quy tắc dưới đây. Bất kỳ sự vi phạm nào sẽ dẫn đến hệ thống bị sập vì quá tải RAM hoặc vỡ layout. Tài liệu này tự thân chứa 100% ngữ cảnh.

## 1. CÁC ĐỊNH LUẬT VẬT LÝ UI/UX (THE IMMUTABLE UI LAWS)

1. **Luật API Mới (Strict Modern API):** **CẤM TUYỆT ĐỐI** sử dụng `use_column_width=True`. **BẮT BUỘC** dùng `use_container_width=True` cho mọi component.
2. **Luật Chống Chớp Giật & Tối Ưu Render (Anti-Blur & Partial Rerun):** **BẮT BUỘC** bọc các hàm cập nhật bảng biểu bằng decorator `@st.fragment`. **BẮT BUỘC** khóa dữ liệu vào RAM bằng `@st.cache_data`.
3. **Luật Chống Tràn RAM:** Chỉ truy vấn và tính toán trên tập dữ liệu đã được thu gọn (Lazy Loading). Tua thời gian lùi về quá khứ phải load data sạch từ DB gốc.
4. **Luật Typography & Màu Sắc TradingView:**
* Font chữ chuẩn, không dùng serif.
* Positive: Xanh lá `#0ECB81`. Negative: Đỏ `#F6465D`. Neutral: Xám `#848E9C`.



---

## 2. BẢNG TRÌNH LỌC TÀI SẢN CÙNG NHÓM (THE COHORT/ETF SCREENER)

**File áp dụng:** `ui/components/screener_view.py`

Lấy cảm hứng từ giao diện "Bitcoin ETFs List" của TradingView, hệ thống cần một bảng điều khiển cho phép so sánh các tài sản có chung đặc tính (Ví dụ: So sánh tất cả các mã Crypto trong danh mục, hoặc so sánh tất cả các Lô tiền gửi/Trái phiếu).

* **Đặc tả UI Component (`st.dataframe` kết hợp `st.column_config`):**
* **Tên Cột & Định dạng Bắt buộc:**
* `Ticker`: Ký hiệu tài sản (VD: BTC_USDT, VCB_6M). In đậm.
* `Price`: Giá khớp gần nhất.
* `Change %`: Biến động giá (Đổi màu Xanh/Đỏ).
* `AUM / Capital Allocated`: Tổng số vốn hệ thống đang phân bổ vào mã này. Dùng `st.column_config.ProgressColumn` để trực quan hóa tỷ trọng so với tổng danh mục.
* `Volume`: Khối lượng giao dịch 24h.
* `Flows (Net)`: Dòng tiền Mua ròng/Bán ròng của Agent trên mã này trong 7 ngày qua.
* `Volatility`: Độ lệch chuẩn hoặc rủi ro (VaR).




* **Tính năng Tương tác:**
* **Sắp xếp (Sorting):** Cho phép User click vào tiêu đề cột để sắp xếp (Ví dụ: Lọc mã có lợi nhuận cao nhất).
* **Tìm kiếm (Search):** Tích hợp `st.text_input` ở ngay góc trên bảng để gõ tên Ticker và filter Real-time.



---

## 3. BIỂU ĐỒ PHÂN BỔ DÒNG TIỀN (FUND FLOW & VOLUME DISTRIBUTION)

**File áp dụng:** `ui/components/screener_view.py`

Thay vì chỉ nhìn số liệu thô, User cần thấy rõ cách dòng tiền đang dịch chuyển giữa các tài sản, tương tự cách TradingView phân tích dòng tiền đổ vào các quỹ ETF.

* **Đặc tả Trực quan hóa (Visualization Specs):**
* **Sử dụng Plotly Bar Chart dạng Stacked (`barmode='stack'`):**
* Trục X: Thời gian (Ngày/Tuần).
* Trục Y: Giá trị dòng tiền (USD).
* Mỗi màu trên cột đại diện cho một Ticker. Điều này giúp nhìn rõ trong một ngày, hệ thống bơm bao nhiêu tiền vào Crypto, rút bao nhiêu tiền khỏi Bank/Bonds.


* **Biểu đồ Treemap (Bản đồ Cây):**
* Sử dụng `plotly.express.treemap` để hiển thị tỷ trọng AUM (Assets Under Management) hiện tại.
* Các khối (Block) lớn đại diện cho tài sản chiếm tỷ trọng cao. Màu sắc của khối dựa trên `Change %` (Xanh/Đỏ/Xám), diện tích khối dựa trên `Capital Allocated`.
* Background của biểu đồ Plotly bắt buộc set `paper_bgcolor="rgba(0,0,0,0)"`.





---

## 4. TÍCH HỢP KHÔNG GIAN VÀO TỔNG THỂ (APP INTEGRATION)

Cập nhật cấu trúc điều hướng tại `ui/app.py` để bổ sung Module Screener/ETFs này mà không làm vỡ Grid hiện tại:

* **Tích hợp vào Thanh Điều Hướng Bên Trái (Sidebar) hoặc Tab mới:**
* Nếu dùng Tabs ở Row 3: Bổ sung tab thứ 5 có tên `"🔍 Trình lọc & Dòng tiền (Screener)"`.
* Bên trong Tab này, chia layout làm 2 cột:
* Cột trái (70%): Bảng `st.dataframe` chi tiết các mã (ETF-style list).
* Cột phải (30%): Biểu đồ Treemap tổng quan tỷ trọng và Biểu đồ Stacked Bar dòng tiền.
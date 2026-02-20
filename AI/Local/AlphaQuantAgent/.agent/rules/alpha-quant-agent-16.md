---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**

## **(THE ULTIMATE TECHNICAL BLUEPRINT)**

### **PHẦN XIII: KIẾN TRÚC GIAO DIỆN UI/UX - DỮ LIỆU THỊ TRƯỜNG & TỔNG QUAN TÀI SẢN (TRADINGVIEW MARKETS)**

## *(File tham chiếu: alpha-quant-agent-16.md)*

# 📜 ĐẶC TẢ GIAO DIỆN TRADINGVIEW MARKETS (THE MARKETS & KEY DATA SPECIFICATION)

**LỜI CẢNH BÁO CHO AI (ANTI-GRAVITY IDE):** Đây là tài liệu chứa các Định luật Vật lý của Giao diện. Mọi dòng code Streamlit được sinh ra phải tuân thủ tuyệt đối các quy tắc dưới đây. Bất kỳ sự vi phạm nào sẽ dẫn đến hệ thống bị sập vì quá tải RAM hoặc vỡ layout. Tài liệu này tự thân chứa 100% ngữ cảnh.

## 1. CÁC ĐỊNH LUẬT VẬT LÝ UI/UX (THE IMMUTABLE UI LAWS)

1. **Luật API Mới (Strict Modern API):** **CẤM TUYỆT ĐỐI** sử dụng `use_column_width=True`. **BẮT BUỘC** dùng `use_container_width=True`.
2. **Luật Chống Chớp Giật & Tối Ưu Render (Anti-Blur & Partial Rerun):** **BẮT BUỘC** bọc các hàm cập nhật component bằng decorator `@st.fragment`. **BẮT BUỘC** khóa dữ liệu vào RAM bằng `@st.cache_data`.
3. **Luật Chống Tràn RAM:** Chỉ nạp tối đa **5000 nến**. Tua thời gian lùi về quá khứ phải load data sạch từ DB gốc, loại bỏ Market Impact.
4. **Luật Typography & Màu Sắc TradingView:**
* Font chữ gọn gàng, ưu tiên Sans-serif.
* Positive (Tốt): Xanh lá `#0ECB81`. Negative (Xấu): Đỏ `#F6465D`. Neutral: Xám `#848E9C`.



---

## 2. BẢNG TỔNG QUAN TÀI SẢN CỐT LÕI (KEY DATA PANEL)

**File áp dụng:** `ui/components/watchlist.py` hoặc đưa vào cột bên cạnh Chart.

Lấy cảm hứng từ trang "Markets: Exchanges & Key Data" của TradingView, hệ thống cần một dải hiển thị các thông số sống còn của tài sản đang được chọn (Ví dụ: BTC_USDT).

* **Bố cục (Layout):** Sử dụng `st.columns` chia làm 4-5 cột nhỏ nằm ngay trên đỉnh của Main Chart hoặc vắt ngang dưới thanh Time-Travel.
* **Các Metrics Bắt Buộc:**
* **Giá Hiện Tại (Last Price):** Kích thước chữ lớn nhất, in đậm.
* **Thay Đổi 24h (24h Change):** Hiển thị số tuyệt đối và % (Ví dụ: `-1,234.50 (-2.5%)`). Đổi màu Xanh/Đỏ tương ứng.
* **Khối Lượng 24h (24h Volume):** Định dạng số ngắn gọn (Ví dụ: `45.2M`, `1.2B`).
* **Điểm Mục Nát Dữ Liệu (Staleness Score):** Lấy từ `market.py`. Hiển thị cảnh báo màu vàng/đỏ nếu API bị đứt quãng (Volume = 0.0 liên tục).
* **Thanh Khoản (Liquidity/Spread):** Ước tính độ trượt giá dự kiến nếu Agent đi lệnh lớn.



---

## 3. DANH SÁCH THỊ TRƯỜNG & TÀI SẢN (THE WATCHLIST / MARKETS TABLE)

**File áp dụng:** `ui/components/watchlist.py`

Khu vực này thay thế cho danh sách Market mặc định, giúp User và AI có cái nhìn toàn cảnh về toàn bộ các file CSV trong `data/trades/` và `data/rates/`.

* **Đặc tả UI Component:**
* Sử dụng `st.dataframe` với tính năng `st.column_config` cao cấp của Streamlit.
* **Cột 1: Tên Tài Sản (Symbol).** Kèm icon hoặc text phân loại (TRADE vs RATE).
* **Cột 2: Giá (Price).**
* **Cột 3: Thay đổi % (Change %).** Dùng `st.column_config.TextColumn` kết hợp custom HTML/Styler để đổi màu Xanh/Đỏ.
* **Cột 4: Biểu Đồ Mini 7 Ngày (7D Trend Sparkline):** **BẮT BUỘC** sử dụng `st.column_config.LineChartColumn`. Nạp chuỗi giá (array) của 7 ngày gần nhất vào đây để vẽ ra một đường line nhỏ ngay trong bảng.
* **Cột 5: Đề xuất AI (AI Action):** Hiển thị tín hiệu từ mô hình (Buy/Sell/Hold) hoặc điểm Ranking từ XGBoost.


* **Tương tác (Interactivity):**
* Bật tham số `on_select="rerun"` (hoặc dùng session state callback) trên `st.dataframe` để khi User click vào một mã (VD: NVDA), toàn bộ hệ thống (Main Chart, Order Book, Quant Matrix) lập tức chuyển đổi ngữ cảnh sang mã NVDA.



---

## 4. BẢNG SO SÁNH LÃI SUẤT (RATES & YIELD CURVE)

**File áp dụng:** Tích hợp vào `ui/components/portfolio_view.py` hoặc tạo tab riêng.

Vì hệ thống quản lý cả tài sản RATE (Ví dụ: `VCB_deposit_6m`, `US10Y`), ta cần một giao diện giống danh sách Trái phiếu (Bond Yields) của TradingView.

* **Hiển thị Đường Cong Lợi Suất (Yield Curve):**
* Trục X: Kỳ hạn (1m, 6m, 12m, 10y).
* Trục Y: Lợi suất (%).
* Vẽ bằng `plotly.graph_objects.Scatter` (dạng line). Đường cong lộn ngược (Inverted Yield Curve) sẽ là tín hiệu vĩ mô cho AI Agent.


* **Bảng Lãi Suất:** Chỉ hiển thị Lợi suất cố định (Locked Rate) và ngày cập nhật. Đánh dấu rõ trạng thái tài sản này **CẤM BÁN KHỐNG** và **QUẢN LÝ THEO LÔ** (Lot-based).
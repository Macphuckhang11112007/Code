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
### **PHẦN 10: SỔ CÁI GIAO DỊCH, TRÌNH CHẠY BACKTEST & HỆ THỐNG XUẤT BÁO CÁO (LEDGER, RUNNER & REPORTING)**
*(Mục tiêu: Xây dựng bảng lịch sử lệnh chi tiết như Binance, giao diện điều khiển kích hoạt Model AI và chức năng xuất báo cáo PDF/CSV cho quỹ)*
---

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 10 (INFRASTRUCTURE BLUEPRINT 10)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Đây là phân hệ Kế toán và Vận hành. Tuyệt đối không được tính toán sai lệch PnL (Profit and Loss) trên UI. Dữ liệu bảng (DataFrame) phải được phân trang (Pagination) hoặc giới hạn dòng nạp để không làm sập Streamlit khi quỹ có hàng chục ngàn giao dịch lịch sử.

## 1. SỔ CÁI GIAO DỊCH CHI TIẾT (THE TRADE LEDGER & PNL HISTORY)
**File thực thi:** `ui/components/trade_ledger.py` (Nhúng vào Tab `"🧾 Lịch sử Giao dịch"` ở Row 3)

Mô phỏng chính xác Tab "Lịch sử Giao dịch" (Trade History) của Binance/TradingView. Nơi đây ghi nhận mọi quyết định của AI.

* **Kiến trúc Bảng Kế Toán (The Ledger Table):**
    * Dùng `st.dataframe` kết hợp `st.column_config`.
    * Giới hạn hiển thị: Chỉ nạp 100 lệnh gần nhất vào RAM. Bổ sung nút `[Tải thêm dữ liệu cũ]`.
    * **Các Cột Bắt Buộc:**
        * `Thời gian (Time)`: Định dạng `YYYY-MM-DD HH:MM:SS`.
        * `Mã (Symbol)`: In đậm (Ví dụ: `BTC_USDT`, `VCB_6M`).
        * `Loại lệnh (Side)`: Chữ `BUY` (Màu xanh), `SELL` (Màu đỏ).
        * `Khối lượng (Qty)`: Số lượng tài sản giao dịch.
        * `Giá Khớp (Exec. Price)`: Giá thực tế sau khi đã tính trượt giá (Slippage).
        * `Phí (Fee)`: Tiền phí sàn.
        * `Realized PnL`: Lợi nhuận đã chốt. Cột này **BẮT BUỘC** tô màu nền Đỏ/Xanh nhạt tùy vào số âm/dương bằng Pandas Styler.
        * `Người thực thi (Executor)`: AI (PPO) / AI (XGBoost) / Manual (User).

```python
# Pseudo-code Bắt buộc cho Sổ Cái Giao Dịch
import streamlit as st
import pandas as pd

@st.fragment
def render_trade_ledger():
    st.markdown("<h4 style='color:#D1D4DC;'>Lịch sử Khớp Lệnh</h4>", unsafe_allow_html=True)
    
    # Hàm đọc log giao dịch từ Backend
    df_trades = get_trade_history_logs(limit=100)
    
    # Hàm tô màu PnL
    def color_pnl(val):
        color = 'rgba(14, 203, 129, 0.2)' if val > 0 else 'rgba(246, 70, 93, 0.2)' if val < 0 else 'transparent'
        return f'background-color: {color}'
    
    styled_df = df_trades.style.map(color_pnl, subset=['Realized_PnL'])
    
    st.dataframe(
        styled_df,
        use_container_width=True,
        hide_index=True,
        column_config={
            "Side": st.column_config.TextColumn("Side"),
            "Exec_Price": st.column_config.NumberColumn("Giá Khớp ($)", format="%.2f"),
            "Realized_PnL": st.column_config.NumberColumn("PnL ($)", format="%.2f")
        }
    )

```

---

## 2. BẢNG ĐIỀU KHIỂN CHIẾN LƯỢC VÀ BACKTEST (THE STRATEGY LAUNCHER)

**File thực thi:** `ui/components/strategy_runner.py`

Thay vì phải chạy lệnh gõ `python run_backtest.py` trên Terminal, hệ thống cho phép User cấu hình và chạy quá trình huấn luyện/backtest trực tiếp trên giao diện.

* **Kiến trúc Không gian (Control Panel):**
* Dùng `st.expander` hoặc Sidebar để chứa form cài đặt.
* **Khối 1: Chọn Động cơ (Engine Selector):** Dùng `st.radio` chọn `[Huấn luyện PPO mới]`, `[Chạy Backtest lịch sử]`, hoặc `[Giao dịch Mock Live]`.
* **Khối 2: Cấu hình Dữ liệu:** * Mốc bắt đầu (Start Date), Mốc kết thúc (End Date).
* Vốn khởi tạo (Initial Balance).


* **Khối 3: Nút Kích Hoạt (The Big Red Button):** * Nút to, in đậm: `st.button("🚀 KHỞI CHẠY CHIẾN LƯỢC", type="primary")`.


* **Luật Xử lý Đa luồng (Threading/Subprocess Law):**
* Việc chạy Train AI tốn hàng giờ đồng hồ. Nếu chạy trực tiếp trên luồng của Streamlit, web sẽ bị đơ hoàn toàn.
* **BẮT BUỘC:** Khi User bấm nút khởi chạy, IDE phải dùng thư viện `subprocess.Popen` hoặc `threading` để đẩy tiến trình AI xuống chạy ngầm ở Backend.
* Sau khi đẩy xuống ngầm, UI hiển thị `st.success("Tiến trình đã được đẩy xuống Backend. Vui lòng sang Tab [🧠 AI Training Monitor] để theo dõi tiến độ.")`.



---

## 3. TRÌNH XUẤT BÁO CÁO (THE REPORT EXPORTER)

**File thực thi:** `ui/components/export_tools.py`

Quỹ đầu tư cần báo cáo gửi cho cổ đông. Hệ thống phải cho phép xuất dữ liệu ra file vật lý.

* **Cấu trúc Nút bấm (Download Buttons):**
* Đặt ở góc phải trên cùng của Tab `Ma trận Định lượng` hoặc `Lịch sử Giao dịch`.
* Nút 1: `st.download_button(label="📥 Tải CSV Giao dịch", data=csv_data, file_name="trades.csv")`.
* Nút 2: `[Xuất PDF Báo cáo]`. (Vì Streamlit không có nút tải PDF trực tiếp, IDE phải dùng thư viện như `pdfkit` hoặc `FPDF` tạo file PDF ngầm ở backend, sau đó dùng `st.download_button` để trả file về cho User).


* **Nội dung PDF Báo cáo Định lượng:**
* Chứa Logo AlphaQuant.
* Snapshot của Ma trận 50 chỉ số định lượng.
* Biểu đồ Heatmap Mùa vụ (Seasonals) và Biểu đồ Tròn danh mục (Portfolio Sunburst).



---

## 4. LUẬT PHẢN HỒI THIẾT BỊ DI ĐỘNG (MOBILE RESPONSIVENESS)

**File thực thi can thiệp:** `ui/styles.css`

Mặc dù đây là một trạm giao dịch chuyên nghiệp trên Desktop, nhưng hệ thống vẫn không được "vỡ nát" nếu User mở bằng điện thoại.

* **Định luật Chuyển đổi Lưới (CSS Media Queries):**
* Bổ sung đoạn CSS sau vào `styles.css` để định nghĩa lại hành vi của các cột khi màn hình thu hẹp (Mobile/Tablet):



```css
/* Pseudo-code Bắt buộc cho Responsive Mobile trong styles.css */
@media screen and (max-width: 768px) {
    /* Ép tất cả các cột của Streamlit xếp chồng lên nhau theo chiều dọc */
    [data-testid="column"] {
        width: 100% !important;
        flex: 1 1 100% !important;
        min-width: 100% !important;
    }
    
    /* Thu nhỏ font size của dải Key Stats */
    .stat-value { font-size: 1rem !important; }
    
    /* Đẩy Chat RAG xuống hẳn, không trôi nổi che biểu đồ nữa */
    .floating-chat-container {
        position: relative; 
        width: 100%;
        bottom: 0; right: 0;
    }
}

```

---

**TỔNG KẾT BẢN THIẾT KẾ KỸ THUẬT (MASTER BLUEPRINT CONCLUSION)**

Đến đây, TẤT CẢ các thành phần từ Hệ thống Lõi Backend, Động cơ AI, Toán học Định lượng, cho đến Kiến trúc UI/UX giả lập TradingView đỉnh cao đã được phác thảo toàn diện. Không còn bất kỳ một ngóc ngách nào của dự án bị bỏ sót.

*Bản thiết kế này chính thức KHÉP LẠI (DONE).* Hệ thống đã sẵn sàng 100% để giao cho "AI Code Web" bắt tay vào viết code thực tế.

```

```
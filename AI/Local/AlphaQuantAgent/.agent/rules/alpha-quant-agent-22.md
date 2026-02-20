---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**

**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---

### **PHẦN 5: BỘ NỐI DỮ LIỆU BACKEND, TAB MODEL AI ĐỘC QUYỀN & QUẢN LÝ DANH MỤC (AI TAB & PORTFOLIO)**

## *(Mục tiêu: Xây dựng Tab Phân tích Model AI độc quyền không có trên TradingView, ép buộc Backend xuất dữ liệu nâng cao, và vẽ Biểu đồ Danh mục Hybrid)*

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 5 (INFRASTRUCTURE BLUEPRINT 05)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** TradingView truyền thống không có các chỉ số của AI Quant Model. Do đó, khu vực này là ĐỘC QUYỀN. Nếu dữ liệu để tính toán 50 chỉ số định lượng chưa tồn tại, **BẮT BUỘC** IDE phải viết bổ sung mã nguồn vào tầng Backend (`src/engine/analyzer.py` và `src/agents/callbacks.py`) để xuất ra các file dữ liệu này trước khi code UI. Nếu UI đọc file không thấy data, ứng dụng sẽ sập.

## 1. ĐỊNH LUẬT ĐỒNG BỘ DỮ LIỆU BACKEND-FRONTEND (THE DATA BRIDGE LAWS)

**File thực thi can thiệp:** `src/engine/analyzer.py` và `src/agents/trader.py`

Để Tab Model AI trên Streamlit có thể hiển thị 50 thống kê nâng cao (như Entropy, Q-Value Spread, VaR, CVaR), phần Train/Backtest bắt buộc phải lưu trữ thêm các tệp sau vào thư mục `logs/trading/`:

1. **`advanced_quant_metrics.json`:**
* Chứa các cấu trúc Dictionary phẳng.
* Ví dụ: `{"VaR_95": -12500.50, "Sharpe": 2.1, "Max_DD": -15.4, "Sortino": 3.2}`.
* *Nhiệm vụ của IDE:* Thêm hàm tính toán VaR (Historical/Parametric), Beta, Alpha vào `analyzer.py` và dump ra file JSON này sau mỗi lần backtest.


2. **`ai_dynamics_log.csv`:**
* Ghi lại trạng thái não bộ của AI sau mỗi Epoch/Step.
* Cột bắt buộc: `Step`, `Policy_Loss`, `Value_Loss`, `Entropy`, `Action_Prob_Buy`, `Action_Prob_Sell`, `Action_Prob_Hold`.
* *Nhiệm vụ của IDE:* Trong `callbacks.py` hoặc hàm `train()` của PPO, phải trích xuất các mảng gradient/loss này từ thư viện `stable_baselines3` và ghi ra CSV.


3. **`covariance_matrix.pkl` (Tùy chọn cho HRP):**
* Xuất ma trận hiệp phương sai từ `optimizer.py` để UI có thể vẽ Heatmap tương quan tài sản.



---

## 2. TAB ĐỘC QUYỀN: QUẢN TRỊ MODEL AI & QUANT (THE AI MODEL TAB)

**File thực thi:** `ui/components/quant_matrix_view.py`

Khu vực này được đặt tại Row 3 của giao diện tổng (`ui/app.py`), mang tên Tab `"🤖 AI Model & Quant Analytics"`. Nó kết hợp tính thẩm mỹ của TradingView với độ sâu của một hệ thống Quant chuyên nghiệp.

* **Kiến trúc Layout Tầng 1 (The 50-Metric Grid):**
* Đã được định nghĩa ở Bản thiết kế số 3 (Chia 4 Sub-tabs: Risk, Performance, Execution, AI Dynamics).
* **BẮT BUỘC** đọc dữ liệu trực tiếp từ `advanced_quant_metrics.json`. Không để Streamlit tự tính toán lại Toán học phức tạp nhằm bảo vệ CPU/RAM.


* **Kiến trúc Layout Tầng 2 (AI Visualizations):**
* Nằm ngay bên dưới ma trận 50 con số. Cần trực quan hóa quá trình "Học" của AI.
* Chia 2 cột `st.columns(2)`:
* **Biểu đồ 1: Policy vs Value Loss (Động lực học).** Đọc từ `ai_dynamics_log.csv`. Vẽ bằng `plotly.express.line`. Trục X là Step, Trục Y là Loss. **BẮT BUỘC** background `rgba(0,0,0,0)` và màu chữ `#D1D4DC`.
* **Biểu đồ 2: Phân bổ Hành động (Action Distribution).** Vẽ biểu đồ miền (Area Chart) hoặc Stacked Bar thể hiện sự thay đổi tỷ lệ xác suất Chọn Mua/Bán/Giữ của AI theo thời gian.





---

## 3. TAB QUẢN LÝ DANH MỤC HYBRID (PORTFOLIO & WALLET VIEW)

**File thực thi:** `ui/components/portfolio_view.py`

Hệ thống của chúng ta có một định luật kế toán lai (Hybrid Accounting): Khoang TRADE (Liquid NAV) và Khoang RATE (Locked NAV). TradingView không có khái niệm khóa vốn (Maturity Lock), nên ta phải thiết kế một biểu đồ đặc thù cho việc này.

* **Biểu đồ Vòng Sáng Mặt Trời (Sunburst Chart Bắt Buộc):**
* **Công nghệ:** Dùng `plotly.express.sunburst`.
* **Cấu trúc dữ liệu phân cấp (Hierarchy):**
* Tầng lõi (Center): `Tổng Tài Sản (Total NAV)`.
* Tầng 1 (Vòng trong): Chia làm 2 nhánh: `Liquid NAV` (Màu Xanh/Trắng) và `Locked NAV` (Màu Vàng/Cam cảnh báo không thể rút).
* Tầng 2 (Vòng ngoài cùng): Chi tiết từng mã. (Nhánh Liquid chỉ ra `Cash`, `BTC`, `NVDA`. Nhánh Locked chỉ ra từng `Lot` riêng biệt như `VCB_6M_Lot1`, `US10Y_Lot2`).


* **Hover Tooltip:** Khi hover vào một `Lot` khóa, bắt buộc hiển thị: `Ngày đáo hạn (Maturity Date)`, `Lãi suất chốt (Locked Rate)`, `Lãi dự kiến (Accrued)`.



```python
# Pseudo-code Bắt buộc cho Portfolio Sunburst
import plotly.express as px
import streamlit as st

@st.fragment
def render_portfolio():
    # Giả lập IDE đọc hàm get_metrics() từ wallet.py
    # df_portfolio = ... 
    
    fig = px.sunburst(
        df_portfolio,
        path=['Type', 'Asset'], # Phân cấp: Type (Liquid/Locked) -> Asset (BTC/VCB)
        values='Value_USD',
        color='Type',
        color_discrete_map={'Liquid': '#0ECB81', 'Locked': '#F0B90B'} # Vàng Binance cho tiền bị khóa
    )
    fig.update_layout(
        paper_bgcolor="rgba(0,0,0,0)",
        margin=dict(t=0, l=0, r=0, b=0),
        font=dict(color="#D1D4DC")
    )
    st.plotly_chart(fig, use_container_width=True, config={'displayModeBar': False})

```

---

## 4. BONG BÓNG CHAT RAG (THE FLOATING GEMINI BUBBLE)

**File thực thi:** `ui/components/chat_box.py`

Streamlit không hỗ trợ "nút trôi nổi" (Floating Action Button) một cách tự nhiên. Để tạo khung chat RAG tư vấn nằm gọn ở góc dưới cùng bên phải màn hình (không làm vỡ layout TradingView):

* **Kỹ thuật Tiêm CSS (CSS Injection):**
* Tạo một `st.container()` và gán cho nó một Class mỏ neo thông qua HTML.
* Trong `styles.css` (ở Blueprint 01), tiêm đoạn mã sau để ép container này nổi lên trên mọi thành phần khác.



```css
/* Mã CSS Bắt buộc nạp vào styles.css cho Chat Bubble */
.floating-chat-container {
    position: fixed;
    bottom: 20px;
    right: 20px;
    width: 380px;
    max-height: 600px;
    background-color: #1E222D;
    border: 1px solid #2B3139;
    border-radius: 12px;
    box-shadow: 0 8px 24px rgba(0,0,0,0.5);
    z-index: 10000;
    display: flex;
    flex-direction: column;
}

```

* **Logic Giao diện Chat:**
* Sử dụng `st.chat_message` và `st.chat_input` bên trong container này.
* **Trí nhớ:** Lấy từ `st.session_state.chat_history`.
* Nếu User ra lệnh (Ví dụ: "Phân tích rủi ro của danh mục"), gọi `gemini.py`, RAG engine sẽ bóc tách ma trận `advanced_quant_metrics.json` và trả lời chuyên sâu bằng ngôn ngữ tự nhiên. Mọi thông báo lỗi (như hết tiền) phải được `formatter.py` xử lý mượt mà.
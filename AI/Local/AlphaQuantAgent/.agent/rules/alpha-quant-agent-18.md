---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**

**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---

### **PHẦN 1: NỀN MÓNG KIẾN TRÚC, ĐỊNH LUẬT VẬT LÝ VÀ BỘ KHUNG CSS (CORE FOUNDATION & CSS)**

## *(Mục tiêu: Thiết lập hạ tầng Streamlit chống giật lag, giao diện Dark Mode chuẩn TradingView, xóa bỏ hoàn toàn giới hạn hiển thị mặc định)*

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 1 (INFRASTRUCTURE BLUEPRINT 01)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Bạn đang thực thi nhiệm vụ xây dựng một hệ thống giả lập thị trường Real-time (Market Simulator). Giao diện này không phải là một trang web tĩnh, nó là một **Buồng lái (Cockpit)**. Mọi dòng code Streamlit bạn sinh ra phải tuân thủ tuyệt đối các "Định luật Vật lý" dưới đây. Nếu vi phạm, hệ thống sẽ sụp đổ vì tràn RAM hoặc vỡ Layout.

## 1. CÁC ĐỊNH LUẬT VẬT LÝ UI/UX (THE IMMUTABLE UI LAWS)

1. **Luật Hàm Hiện Đại (Strict Modern API):** * **CẤM TUYỆT ĐỐI:** Sử dụng `use_column_width=True` hay bất kỳ tham số nào đã bị Streamlit gắn mác "Deprecated".
* **BẮT BUỘC:** Thay thế bằng `use_container_width=True` trên TOÀN BỘ các component (Dataframe, Plotly Chart, Image, Metrics).


2. **Luật Chống Chớp Giật (Anti-Blur & Partial Rerun):**
* Streamlit có điểm yếu là re-run toàn bộ file `app.py` mỗi khi có thay đổi state. Để đạt tốc độ 1 tick/giây như TradingView:
* **BẮT BUỘC:** Sử dụng decorator `@st.fragment` (hoặc `@st.experimental_fragment` tùy phiên bản lõi) bọc xung quanh các hàm cập nhật đơn lẻ (Ví dụ: `def render_chart():`, `def render_order_book():`). Điều này ép Streamlit chỉ render lại đúng cái khung đó, phần còn lại đứng im.


3. **Luật Quản Trị Bộ Nhớ (Stateful Memory & Lazy Loading):**
* **TUYỆT ĐỐI KHÔNG** dùng lệnh đọc file CSV (`pd.read_csv`) đặt trực tiếp trong các hàm render UI.
* **BẮT BUỘC:** Mọi thao tác I/O hoặc tính toán Tensor phải được bọc trong hàm có decorator `@st.cache_data` để khóa chặt vào RAM.


4. **Luật Bảng Màu (The TradingView Color Palette):**
* Nền tổng thể (Background): Đen sâu `#0b0e11` hoặc `#131722`.
* Chữ (Text/Foreground): Trắng xám `#EAECEF` hoặc `#D1D4DC`.
* Màu Lưới (Grid/Border): Xám mờ `#2B3139`.
* Màu Tăng (Positive/Bull): Xanh lá `#0ECB81` hoặc `#26A69A`.
* Màu Giảm (Negative/Bear): Đỏ `#F6465D` hoặc `#EF5350`.



---

## 2. CẤU TRÚC THƯ MỤC UI TỔNG THỂ (THE UI DIRECTORY TREE)

IDE phải tạo chính xác cấu trúc sau trong thư mục `ui/`:

```text
ui/
├── app.py                      # File Root (Entry point), chứa logic cấu trúc Grid tổng.
├── styles.css                  # Mã CSS can thiệp sâu vào DOM của Streamlit.
└── components/                 # Các khối UI module hóa
    ├── __init__.py
    ├── time_travel_bar.py      # Thanh tua thời gian ở topbar.
    ├── chart_view.py           # Biểu đồ nến TradingView lõi (Lightweight Charts).
    ├── order_book_view.py      # Sổ lệnh và panel đặt lệnh.
    ├── quant_matrix_view.py    # Ma trận 50 chỉ số rủi ro/hiệu suất.
    ├── technicals_view.py      # Đồng hồ đo Gauge và Heatmap mùa vụ.
    ├── screener_view.py        # Trình lọc tài sản và dòng tiền.
    ├── portfolio_view.py       # Biểu đồ tròn quản lý Liquid vs Locked NAV.
    └── chat_box.py             # Bong bóng chat RAG Gemini.

```

---

## 3. GIẢI PHẪU CSS LÕI (OVERRIDING STREAMLIT DOM)

**File thực thi:** `ui/styles.css`

Mặc định, Streamlit có padding rất lớn, header màu trắng che mất nội dung, và lớp sương mù (blur) khi đang load. IDE bắt buộc sao chép chính xác đoạn CSS sau để biến nó thành màn hình Full-width của TradingView.

```css
/* 1. Xóa bỏ hoàn toàn lớp Blur và Icon Load (Running...) gây chớp giật */
[data-testid="stAppViewBlockContainer"] {
    filter: none !important;
    opacity: 1 !important;
    transition: none !important;
}
.stSpinner > div {
    display: none !important; 
}

/* 2. Quy hoạch lại Header Mặc định của Streamlit (Không xóa để giữ menu Settings) */
header[data-testid="stHeader"] {
    background-color: transparent !important;
    background-image: none !important;
    box-shadow: none !important;
    z-index: 9999;
}

/* 3. Mở rộng không gian làm việc tối đa (Full-width no-padding) */
.block-container {
    padding-top: 2rem !important; /* Đẩy nội dung xuống một chút để né Header */
    padding-bottom: 0rem !important;
    padding-left: 0.5rem !important;
    padding-right: 0.5rem !important;
    max-width: 100% !important; /* Ép Full-width */
}

/* 4. Khung nền Dark Theme Tuyệt đối (TradingView Style) */
.stApp {
    background-color: #131722 !important; 
    color: #D1D4DC !important; 
}

/* 5. Tùy chỉnh các khối Tabs cho giống TradingView */
[data-baseweb="tab-list"] {
    background-color: #1E222D !important;
    border-radius: 4px;
    padding: 2px;
}
[data-baseweb="tab"] {
    color: #848E9C !important;
    border: none !important;
    background-color: transparent !important;
}
[data-baseweb="tab"][aria-selected="true"] {
    color: #EAECEF !important;
    background-color: #2B3139 !important;
    border-radius: 4px;
}

/* 6. Ẩn thanh cuộn (Scrollbar) xấu xí của trình duyệt */
::-webkit-scrollbar {
    width: 6px;
    height: 6px;
}
::-webkit-scrollbar-track {
    background: #131722; 
}
::-webkit-scrollbar-thumb {
    background: #2B3139; 
    border-radius: 3px;
}
::-webkit-scrollbar-thumb:hover {
    background: #848E9C; 
}

```

---

## 4. TỆP TIN ENTRY POINT KHOẢNG KHÔNG GIAN (THE APP.PY BOOTSTRAPPER)

**File thực thi:** `ui/app.py`

Đây là tệp gốc. IDE phải code chính xác logic khởi tạo trang web và tiêm CSS vào hệ thống. Cấu trúc Layout sẽ áp dụng hệ thống Grid mô phỏng TradingView.

```python
import streamlit as st
import os

# 1. Cấu hình trang bắt buộc phải nằm dòng đầu tiên
st.set_page_config(
    page_title="AlphaQuant TradingView",
    page_icon="📈",
    layout="wide", # Bắt buộc
    initial_sidebar_state="collapsed" # Giấu sidebar mặc định
)

# 2. Khởi tạo Trí nhớ Hệ thống (Session State Initialization)
def init_session_state():
    if 'current_sim_time' not in st.session_state:
        # Thời gian gốc của User (Mốc A)
        st.session_state.current_sim_time = None 
    if 'active_symbol' not in st.session_state:
        # Ticker mặc định khi mở app
        st.session_state.active_symbol = "BTC_USDT"
    if 'chat_history' not in st.session_state:
        # Bộ nhớ của bong bóng Chat RAG
        st.session_state.chat_history = []

init_session_state()

# 3. Tiêm CSS vào DOM
def load_css():
    css_file = os.path.join(os.path.dirname(__file__), "styles.css")
    if os.path.exists(css_file):
        with open(css_file, "r") as f:
            st.markdown(f'<style>{f.read()}</style>', unsafe_allow_html=True)

load_css()

# ==========================================
# KHUNG XƯƠNG GIAO DIỆN (THE MASTER GRID)
# ==========================================

# Nhập các component (Sẽ được thiết kế ở các file MD tiếp theo)
# from components.time_travel_bar import render_topbar
# from components.chart_view import render_main_chart
# from components.order_book_view import render_order_book
# from components.quant_matrix_view import render_quant_matrix

def main():
    # ROW 1: Topbar (Chứa Logo, Mốc thời gian, Nút tua nhanh)
    # render_topbar()
    st.markdown("---") # Kẻ ngang mỏng
    
    # ROW 2: Trading Core (Biểu đồ ở giữa 7.5 phần, Sổ lệnh bên phải 2.5 phần)
    col_main_chart, col_side_panel = st.columns([7.5, 2.5], gap="small")
    
    with col_main_chart:
        st.write("Vùng chứa Chart Lightweight")
        # render_main_chart(st.session_state.active_symbol)
        
    with col_side_panel:
        st.write("Vùng chứa Sổ lệnh & Nút Đặt Lệnh")
        # render_order_book(st.session_state.active_symbol)
        
    # ROW 3: Trung tâm Phân tích (Chia thành các Tabs)
    st.markdown("---")
    main_tabs = st.tabs([
        "🧬 Ma trận Định lượng", 
        "⏱️ Đồng hồ Kỹ thuật", 
        "📅 Tính Mùa vụ", 
        "🔍 Trình lọc Ticker", 
        "💼 Quản lý Vốn (NAV)"
    ])
    
    # Render các module vào từng tab tương ứng...

if __name__ == "__main__":
    main()

```
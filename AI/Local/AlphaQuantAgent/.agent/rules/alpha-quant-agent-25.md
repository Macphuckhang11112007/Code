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
### **PHẦN 8: TRÍ TUỆ NGÔN NGỮ (RAG CHAT), GIÁM SÁT HUẤN LUYỆN VÀ QUẢN TRỊ TRẠNG THÁI (STATE MANAGEMENT)**
*(Mục tiêu: Hoàn thiện Bong bóng Chat Gemini RAG, Tích hợp màn hình giám sát TensorBoard và Thiết lập luồng dữ liệu trung tâm của Streamlit)*
---

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 8 (INFRASTRUCTURE BLUEPRINT 08)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Đây là các thành phần "Động" phức tạp nhất của hệ thống. Luồng dữ liệu Chat RAG và TensorBoard yêu cầu đọc file/database liên tục. **BẮT BUỘC** phải xử lý bất đồng bộ (hoặc giả lập bất đồng bộ qua `@st.fragment`) để UI không bị "đóng băng" khi AI đang suy nghĩ hoặc Model đang train.

## 1. GIAO DIỆN CHAT RAG ĐỘC LẬP (THE FLOATING RAG BUBBLE)
**File thực thi:** `ui/components/chat_box.py`

Như đã quy định ở Blueprint 05, Chatbox phải là một thành phần trôi nổi (Floating UI) ở góc dưới cùng bên phải. Nó đóng vai trò là "Chuyên gia Giải trình" (The Explainer) của quỹ đầu tư.

* **Kiến trúc UI/UX Cốt lõi:**
    * **State:** Quản lý trạng thái đóng/mở bằng `st.session_state.is_chat_open` (Boolean). Nút icon bong bóng chat `💬` sẽ toggle biến này.
    * **Lịch sử hội thoại:** Render bằng vòng lặp qua mảng `st.session_state.chat_history`.
    * **BẮT BUỘC** dùng `st.chat_message("user")` và `st.chat_message("assistant")`.
    * **Định luật Màu sắc Chat:** Nền của bong bóng chat là `#1E222D` (TradingView Panel). Tin nhắn User nền màu `#2962FF` (Xanh lơ TradingView), tin nhắn Assistant nền màu `#2B3139` (Xám đen).
* **Hiệu ứng Khóa Giao diện (The Thinking Spinner):**
    * Khi User gõ lệnh và nhấn Enter, hệ thống **BẮT BUỘC** hiển thị `st.spinner("AlphaQuant đang phân tích...")` hoặc `st.status("Đang truy vấn ma trận định lượng...")` TRƯỚC KHI gửi payload qua `gemini.py`.
    * Trong lúc chờ API Google Gemini trả về, không được để User bấm thêm nút Mua/Bán trên UI chính.

```python
# Pseudo-code Bắt buộc cho Chat RAG
import streamlit as st

@st.fragment
def render_chat_bubble():
    # Khởi tạo trạng thái
    if "is_chat_open" not in st.session_state:
        st.session_state.is_chat_open = False
        
    # Nút bấm Floating (CSS đã tiêm ở Blueprint 01)
    if st.button("💬 Chat với AlphaQuant", key="toggle_chat"):
        st.session_state.is_chat_open = not st.session_state.is_chat_open
        
    if st.session_state.is_chat_open:
        st.markdown('<div class="floating-chat-container">', unsafe_allow_html=True)
        st.markdown("<h4 style='color:#D1D4DC; text-align:center; padding-top:10px;'>Trợ lý Định lượng RAG</h4>", unsafe_allow_html=True)
        
        # Render lịch sử
        for msg in st.session_state.chat_history:
            with st.chat_message(msg["role"]):
                st.markdown(msg["content"])
                
        # Khung nhập liệu
        if prompt := st.chat_input("Nhập câu hỏi hoặc lệnh giao dịch..."):
            # Thêm tin User vào UI
            st.session_state.chat_history.append({"role": "user", "content": prompt})
            st.rerun() # Hoặc dùng Fragment auto-refresh
            
            with st.chat_message("assistant"):
                with st.spinner("Đang trích xuất dữ liệu..."):
                    # Gọi Backend NLP (Parser -> RAG -> Gemini)
                    # response = gemini_service.process(prompt, current_portfolio, quant_matrix)
                    pass
            # Cập nhật phản hồi
            # st.session_state.chat_history.append({"role": "assistant", "content": response})
        st.markdown('</div>', unsafe_allow_html=True)

```

---

## 2. BẢNG THEO DÕI HUẤN LUYỆN (THE LEARNING MONITOR / TENSORBOARD UI)

**File thực thi:** `ui/components/learning_monitor.py`

Để hệ thống thực sự "Pro", người dùng không cần phải gõ lệnh `tensorboard --logdir logs/` trong Terminal. Chúng ta sẽ nhúng trực tiếp dữ liệu học tập của Agent vào UI Streamlit.

* **Kiến trúc Không gian:** Đặt ở một Tab riêng trong `Row 3` (Ví dụ: Tab `"🧠 AI Training Monitor"`).
* **Cơ chế Đọc Log (Binary Parser):**
* Lõi Streamlit không đọc được file `.tfevents` trực tiếp. IDE **BẮT BUỘC** phải viết một hàm backend dùng thư viện `tensorboard.backend.event_processing.event_accumulator` để dịch file nhị phân thành Pandas DataFrame.
* Cache hàm đọc này bằng `@st.cache_data(ttl=5)` để nó làm mới mỗi 5 giây nếu đang trong quá trình Training.


* **Đặc tả Trực quan hóa (Training Charts):**
* Vẽ 2 biểu đồ Line Chart lớn (Dùng Plotly, `use_container_width=True`, nền trong suốt).
* **Biểu đồ 1: Đường cong Phần thưởng (Reward Curve):** Trục Y: `rollout/ep_rew_mean`. Trục X: `timesteps`. Màu Line: Xanh `#0ECB81`.
* **Biểu đồ 2: Hàm Mất mát (Loss Curve):** Trục Y: `train/loss`. Trục X: `timesteps`. Màu Line: Đỏ `#F6465D`.
* Hiển thị bảng siêu tham số (`hyperparams.yaml`) bên cạnh biểu đồ bằng component `st.json`.



---

## 3. MA TRẬN TRẠNG THÁI TOÀN CẦU (GLOBAL STATE MANAGEMENT)

**File thực thi:** Mọi file trong `ui/` và `ui/components/`

Streamlit là một hệ thống Stateless (mất trí nhớ sau mỗi lần chạy lại). Để kết nối Chart, Sổ Lệnh, Chat RAG và Cỗ máy Thời gian lại với nhau thành một vòng lặp kín, IDE **BẮT BUỘC** phải duy trì Ma trận State này tại file `app.py` trước khi render bất kỳ component nào:

```python
# Danh sách Biến Session State Bắt Buộc (The Global Registry)
# Bất kỳ module nào thiếu biến này sẽ gây lỗi KeyError trên Streamlit.

def initialize_global_state():
    # --- 1. Nhóm Thời gian & Hệ trục (Time Physics) ---
    st.session_state.setdefault('current_sim_time', None)  # Mốc A hiện tại
    st.session_state.setdefault('is_traveling_past', False) # Cờ báo hiệu đang tua slider
    
    # --- 2. Nhóm Tương tác Bề mặt (UI Focus) ---
    st.session_state.setdefault('active_symbol', 'BTC_USDT') # Mã tài sản đang xem
    st.session_state.setdefault('theme', 'dark') # Giao diện hiện hành
    
    # --- 3. Nhóm Giao dịch & Kế toán (Ledger & Orders) ---
    st.session_state.setdefault('pending_orders', []) # Lệnh đang chờ khớp ở tick tiếp theo
    st.session_state.setdefault('portfolio_snapshot', None) # Cache của get_metrics() từ wallet
    
    # --- 4. Nhóm Bộ nhớ LLM (NLP Context) ---
    st.session_state.setdefault('chat_history', []) # Bộ nhớ hội thoại cho Gemini
    st.session_state.setdefault('quant_matrix_cache', None) # Cache 50 chỉ số để RAG đọc

```

* **Định luật Truyền dẫn (Event Propagation Law):**
* Khi User chọn 1 Ticker mới từ bảng **Watchlist** (`screener_view.py`), nó cập nhật biến `active_symbol`.
* Ngay lập tức, `chart_view.py` và `order_book_view.py` (được bọc bởi fragment) phải đọc biến `active_symbol` này để tự động nạp Tensor của mã mới từ Backend (`market.py`). Không được phép truyền Ticker qua các biến cục bộ (Local variables) để tránh xung đột vòng đời (Lifecycle conflict) của Streamlit.



```

```
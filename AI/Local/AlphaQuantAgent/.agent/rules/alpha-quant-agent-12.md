---
trigger: always_on
---

---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**
---
### **PHẦN IX/X: KIẾN TRÚC GIAO DIỆN UI/UX - NỀN TẢNG & CỖ MÁY THỜI GIAN**
*(File tham chiếu tiếp nối: alpha-quant-agent-12.md)*
---

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG (INFRASTRUCTURE UI SPECIFICATION)

**Lệnh Dành Cho AI (Anti-gravity IDE):** Bạn phải tuân thủ nghiêm ngặt từng dòng CSS và cấu trúc component dưới đây. Không được sử dụng các hàm mặc định của Streamlit nếu chúng phá vỡ giao diện Modern Dark Mode kiểu Binance/Yahoo Finance.

## 1. GIẢI PHẪU CSS LÕI VÀ VẤN ĐỀ HEADER (THE CORE CSS & HEADER BYPASS)
**File áp dụng:** `ui/styles.css` và được gọi thông qua `st.markdown('<style>...</style>', unsafe_allow_html=True)` trong `ui/app.py`.

* **Vấn đề:** Streamlit có một Header mặc định (chứa nút Deploy, Menu 3 chấm) nằm đè lên nội dung, và màn hình hay bị "blur" (mờ) kèm icon xoay xoay khi đang load data.
* **Giải pháp (Bắt buộc Code đúng CSS này):**
    ```css
    /* 1. Xóa bỏ hoàn toàn lớp Blur và Icon Load gây khó chịu */
    [data-testid="stAppViewBlockContainer"] {
        filter: none !important;
        opacity: 1 !important;
    }
    .stSpinner > div {
        display: none !important; /* Giấu spinner mặc định, ta sẽ dùng loading custom */
    }
    
    /* 2. Quy hoạch lại Header Mặc định của Streamlit */
    /* KHÔNG ẩn header đi vì mất menu hữu ích, nhưng làm nó trong suốt và không chiếm diện tích */
    header[data-testid="stHeader"] {
        background-color: transparent !important;
        z-index: 9999;
    }
    
    /* 3. Đẩy nội dung của chúng ta xuống dưới Header, dán sát mép màn hình */
    .block-container {
        padding-top: 3rem !important; /* Né cái header ra */
        padding-bottom: 0rem !important;
        padding-left: 1rem !important;
        padding-right: 1rem !important;
        max-width: 100% !important; /* Full width như Binance */
    }

    /* 4. Khung nền Dark Theme Tuyệt đối (Binance Style) */
    .stApp {
        background-color: #0b0e11 !important; /* Đen sâu */
        color: #EAECEF !important; /* Trắng xám dễ đọc */
    }
    ```

## 2. CỖ MÁY THỜI GIAN (THE TIME-TRAVEL TOPBAR)
**File áp dụng:** Sẽ được gọi ngay đầu file `ui/app.py`.

Đây là thanh công cụ trên cùng của màn hình. Nó chứa Bộ điều khiển Ngày/Giờ theo yêu cầu của User: Slider kéo bằng ngày, Text Input chỉnh đến từng phút.

* **Logic Hoạt Động Cốt Lõi (State Management):**
    * Sử dụng `st.session_state.current_sim_time` để lưu mốc thời gian hiện tại (Mốc A).
    * Sử dụng tính năng `on_change` của `st.text_input`. Khi user nhập text rồi nhấn `Enter`, hoặc click chuột ra ngoài (hành vi `blur`), Streamlit tự động trigger hàm callback để cập nhật biểu đồ.
    * **Định luật Vật lý (Luật chống tràn RAM):** Khi tua ngược thời gian (Back in time), hệ thống CHỈ truy vấn lại từ file `CSV` (Data thô gốc). Xóa bỏ mọi cache về `Market Impact` (Tác động thị trường) của những lệnh mua/bán giả lập trước đó.

* **Đặc Tả UI/UX Component:**
    1.  Tạo một vùng chứa `st.container()` có viền mỏng phía dưới làm Topbar.
    2.  Chia 3 cột (`st.columns([1, 6, 2])`):
        * **Cột 1 (Logo/Mode):** Nút Toggle Trắng/Đen (Sử dụng biểu tượng ☀️/🌙).
        * **Cột 2 (Slider):** Dùng `st.slider`. 
            * Mốc Min: Ngày đầu tiên của Dataset.
            * Mốc Max: Ngày cuối cùng của Dataset.
            * Step: `timedelta(days=1)`.
        * **Cột 3 (Precision Input):** Dùng `st.text_input`.
            * Hiển thị chuỗi định dạng: `YYYY-MM-DD HH:MM:00`.
            * Hàm Callback: `def update_time_from_input(): ...` sẽ parse chuỗi này, nếu hợp lệ thì gán vào `st.session_state.current_sim_time`. Nếu nhập sai định dạng, tự động revert về giờ cũ, không được Crash.

## 3. CHATBOX BONG BÓNG (THE FLOATING RAG BUBBLE)
**File áp dụng:** `ui/components/chat_box.py`

* **Yêu cầu Thiết kế:** Không dùng layout chia đôi màn hình thô kệch. Khung chat Gemini phải là một nút hình tròn (Bong bóng) trôi nổi ở góc dưới cùng bên phải màn hình (Bottom-Right Fixed Position).
* **Hành vi UI:**
    * Mặc định: Chỉ hiện icon Chat (💬).
    * Khi Click: Mở pop-up lên thành một khung chat (Kích thước ~ 350px width, 500px height).
    * **Trí nhớ (Memory):** Sử dụng `st.session_state.chat_history` lưu toàn bộ đoạn chat. Khi tắt pop-up mở lại, đoạn chat vẫn nằm nguyên đó.
* **Kỹ thuật Code (Cho AI):** Streamlit không có hàm "Floating button" mặc định. Phải dùng HTML/CSS tiêm vào qua `st.components.v1.html` để tạo nút nổi, và dùng Javascript để lắng nghe sự kiện click nhằm bật/tắt một `st.container` chứa `st.chat_message`.

---
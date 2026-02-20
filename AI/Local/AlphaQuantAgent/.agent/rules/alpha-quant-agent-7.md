---
trigger: always_on
---

---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**

---

### **PHẦN VI/VI: ĐIỂM TRUY CẬP VÀ CHIẾN LƯỢC THỰC THI (ENTRY POINTS & EXECUTION STRATEGY)**

Phần này quy định cách thức con người tương tác với mã nguồn, tách biệt rõ ràng giữa các tác vụ nặng (Heavy Calculation) và các tác vụ hiển thị (Visualization/Interaction).

#### **6.1. Triết lý Thiết kế: "Tách biệt Mối quan tâm" (Separation of Concerns)**
Hệ thống có hai trạng thái hoạt động riêng biệt:
1.  **Chế độ Phòng Lab (The Lab Mode):** Nơi máy tính làm việc cật lực để học (`Training`), tính toán đặc trưng (`Feature Engineering`), hoặc kiểm thử lại (`Backtesting`). Đây là lãnh địa của **Backend/CLI**.
2.  **Chế độ Buồng lái (The Cockpit Mode):** Nơi con người nhìn thấy kết quả, ra quyết định và tương tác. Đây là lãnh địa của **Frontend/GUI**.

Chúng ta sử dụng 2 điểm truy cập (Entry Points) riêng biệt để quản lý 2 trạng thái này, đảm bảo UI không bao giờ bị treo khi AI đang training.

---

#### **6.2. Chỉ huy Hậu cần: `main.py` (Backend Entry Point)**

*   **Vị trí:** Root Directory.
*   **Bản chất:** Giao diện Dòng lệnh (CLI - Command Line Interface).
*   **Công nghệ:** Sử dụng thư viện `argparse` hoặc `click` để nhận tham số.
*   **Nhiệm vụ Cốt lõi:** Điều phối các module trong `src/` để thực hiện các quy trình dài hạn (Long-running processes).

**Các Chế độ Vận hành (Operational Modes):**

1.  **Mode: `train` (Huấn luyện AI)**
    *   **Lệnh:** `python main.py --mode train --agent ppo --epochs 1000`
    *   **Quy trình:**
        1.  Gọi `market.load()` để xây Tensor.
        2.  Khởi tạo `simulator` làm môi trường (Environment).
        3.  Khởi tạo Agent (PPO/SAC) từ `src/agents/trader.py`.
        4.  Bắt đầu vòng lặp `Learn`.
        5.  Lưu định kỳ: Model vào `models/rl_agent/`, Log vào `logs/training/`.

2.  **Mode: `backtest` (Kiểm thử quá khứ)**
    *   **Lệnh:** `python main.py --mode backtest --model best_model.zip --start 2023-01-01`
    *   **Quy trình:**
        1.  Nạp `system.yaml` để lấy cấu hình vốn/phí.
        2.  Nạp Model đã train từ `models/`.
        3.  Chạy `simulator` với `deterministic=True` (Hành động quyết đoán, không ngẫu nhiên).
        4.  Ghi sổ cái vào `logs/trading/transactions.csv`.

3.  **Mode: `features` (Kỹ thuật đặc trưng)**
    *   **Lệnh:** `python main.py --mode features`
    *   **Quy trình:**
        1.  Gọi `market.load()`.
        2.  Chuyển Tensor sang `src/pipeline/features.py`.
        3.  Tính toán RSI, MACD, Volatility.
        4.  Lưu cache vào `data/features/*.parquet` (để các lần chạy sau nhanh hơn).

---

#### **6.3. Cổng Giao diện: `run.py` (Frontend Entry Point)**

*   **Vị trí:** Root Directory.
*   **Bản chất:** Script khởi động Server Web (Streamlit Bootstrap).
*   **Công nghệ:** Streamlit.
*   **Nhiệm vụ Cốt lõi:** Kết nối người dùng với kết quả đã được `main.py` tạo ra và dịch vụ AI (Gemini).

**Cấu trúc Logic của `run.py`:**

```python
# Pseudo-code logic của run.py
import streamlit as st
from ui.app import main_dashboard
from src.utils.config_loader import load_env

if __name__ == "__main__":
    # 1. Nạp biến môi trường (API Key của Gemini)
    load_env(".env")

    # 2. Cấu hình trang (Title, Layout, Icon)
    st.set_page_config(page_title="AlphaQuant AI", layout="wide")

    # 3. Khởi tạo Session State (Bộ nhớ tạm của trình duyệt)
    if 'user_session' not in st.session_state:
        st.session_state.user_session = init_memory()

    # 4. Chuyển quyền điều khiển cho Module UI chính
    main_dashboard()
```

**Các Thành phần UI được `run.py` kích hoạt (`ui/`):**

1.  **Dashboard Quan sát:**
    *   Đọc file `logs/trading/performance.json` để vẽ biểu đồ ROI.
    *   Đọc `logs/trading/nav_curve.csv` để vẽ đường cong tài sản.
    *   Đọc trạng thái `Wallet` (Locked vs Liquid) để vẽ biểu đồ tròn Sunburst.
2.  **Trung tâm Tương tác (Chat Interface):**
    *   Khung chat kết nối với `src/services/gemini.py`.
    *   Hiển thị spinner "AI đang suy nghĩ..." khi RAG pipeline đang chạy.

---

#### **6.4. Mối quan hệ "Nhà Sản xuất - Người Tiêu thụ" (Producer-Consumer Relationship)**

Để hệ thống không bị xung đột, chúng ta thiết lập quy tắc truyền dữ liệu giữa 2 file này:

1.  **`main.py` là NHÀ SẢN XUẤT (Producer):**
    *   Nó đọc dữ liệu thô (`data/`).
    *   Nó tạo ra mô hình (`models/`).
    *   Nó sinh ra log giao dịch (`logs/`).
    *   *Nó KHÔNG BAO GIỜ vẽ biểu đồ giao diện.*

2.  **`run.py` là NGƯỜI TIÊU THỤ (Consumer):**
    *   Nó đọc log giao dịch (`logs/`).
    *   Nó đọc mô hình (chỉ để hiển thị thông tin metadata).
    *   Nó đọc cấu hình.
    *   *Nó KHÔNG BAO GIỜ huấn luyện model (tránh treo giao diện).*
    *   *Ngoại lệ:* Nó có thể kích hoạt một `Backtest` nhanh (Lightweight Backtest) thông qua việc gọi `main.py` dưới dạng *Subprocess* (Tiến trình con) nếu người dùng yêu cầu "Test chiến lược này ngay lập tức".

---

#### **6.5. Bảo mật & Biến Môi trường (.env)**

Cả `main.py` và `run.py` đều phải đi qua chốt kiểm soát bảo mật này trước khi chạy.

*   **Tệp tin:** `.env` (Không được commit lên Git).
*   **Nội dung bắt buộc:**
    *   `GEMINI_API_KEY`: Để Chatbot hoạt động.
    *   `DB_URL`: Đường dẫn đến SQLite (mặc định là local file).
*   **Loader:** `src/utils/config_loader.py` sẽ chịu trách nhiệm `load_dotenv()` ngay dòng đầu tiên của cả 2 file entry point.

---

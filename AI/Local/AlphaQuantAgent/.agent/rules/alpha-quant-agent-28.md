---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (MASTER ARCHITECTURE)**

**(THE ULTIMATE SYSTEM BLUEPRINT)**

---

### **PHẦN 11: KIẾN TRÚC THƯ MỤC TỔNG THỂ VÀ SƠ ĐỒ ĐIỀU PHỐI (MASTER DIRECTORY TREE & ORCHESTRATION)**

## *(Mục tiêu: Chốt cứng cấu trúc thư mục cuối cùng, vá các lỗ hổng namespace và định nghĩa rõ ràng nhiệm vụ của từng file để AI lập trình không thể sai sót)*

# 📜 ĐẶC TẢ KIẾN TRÚC HỆ THỐNG (THE MASTER DIRECTORY SPECIFICATION)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Đây là sơ đồ phả hệ của toàn bộ dự án AlphaQuant. Mọi file được tạo ra phải nằm ĐÚNG vị trí này. Tuyệt đối không được tự ý đổi tên file, đặc biệt là các file trong thư mục `src/agents/` để tránh lỗi đụng độ thư viện (Namespace Collision).

## 1. CẤU TRÚC THƯ MỤC GỐC (THE ROOT DIRECTORY)

Cấp cao nhất của dự án chứa các tệp cấu hình môi trường và bộ khởi động tổng thể.

* `AlphaQuant/` (Thư mục gốc)
* `.env`: Chứa các API Key nhạy cảm (Google Gemini API, Binance API nếu có). **Tuyệt đối không hardcode key vào code.**
* `.gitignore`: Chặn Git đẩy các file rác, file `.env`, và thư mục `__pycache__` lên mạng.
* `docker-compose.yml` & `Dockerfile`: Cấu hình container hóa để có thể deploy lên cloud dễ dàng.
* `requirements.txt`: Chứa phiên bản cố định của thư viện (VD: `streamlit==1.32.0`, `stable-baselines3`, `xgboost`, `streamlit-lightweight-charts`).
* **`run_pipeline.py` (MỚI):** Bộ điều phối tối cao (Orchestrator). Thay thế cho `main.py` và `run.py` cũ. File này sẽ cung cấp CLI (Command Line Interface) để User chọn chạy: `train`, `backtest`, hoặc `ui`.



---

## 2. CÁC THƯ MỤC LƯU TRỮ VÀ CẤU HÌNH (STORAGE & CONFIGS)

Nơi chứa dữ liệu tĩnh, trọng số mô hình và siêu tham số.

* `configs/`
* `system.yaml`: Cấu hình vốn khởi tạo, phí giao dịch, độ trượt giá (Slippage K).
* `models.yaml`: Siêu tham số cho PPO, LSTM, XGBoost (Learning rate, gamma, max_depth...).


* `data/`
* `raw/`: Dữ liệu thô tải từ Yahoo Finance / Binance.
* `processed/`: Dữ liệu đã làm sạch, chuẩn hóa.
* `features/`: Dữ liệu đã được thêm các chỉ báo kỹ thuật (MA, RSI, MACD).


* `logs/`
* `trading/`: Chứa file `advanced_quant_metrics.json` và `ai_dynamics_log.csv` (Để UI Tab AI đọc).
* `tensorboard/`: Chứa file nhị phân `.tfevents` của Stable Baselines3.


* `models/`
* `ppo_weights.zip`, `xgb_booster.json`: Nơi lưu trữ bộ não của Agent sau khi train xong.



---

## 3. LÕI BACKEND - TRÁI TIM ĐỊNH LƯỢNG (THE BACKEND CORE - `src/`)

Đây là tầng xử lý logic ngầm. Streamlit UI chỉ được phép *gọi* dữ liệu từ đây, không được phép *tính toán* logic ở UI.

* `src/agents/` (Bộ não AI)
* `base.py`: Lớp trừu tượng gốc cho mọi Agent.
* `ppo.py`: Mô hình Deep Reinforcement Learning cốt lõi.
* **`xgb_model.py` (ĐÃ ĐỔI TÊN):** Mô hình XGBoost cho Portfolio Ranking. (Đổi từ `xgboost.py` để không làm sập thư viện Python).
* **`callbacks.py` (MỚI BỔ SUNG):** Chứa các class Hook can thiệp vào quá trình train của PPO để trích xuất `Loss`, `Entropy` xuất ra file CSV cho UI đọc.


* `src/data/` (Kỹ sư Dữ liệu)
* `fetcher.py`: Tải data.
* `cleaner.py`: Làm sạch data.
* **`features.py` (MỚI BỔ SUNG):** Trạm tính toán chỉ báo kỹ thuật hạng nặng. Mọi đường MA, Volume, RSI hiển thị trên UI phải được file này tính sẵn.


* `src/engine/` (Động cơ Mô phỏng)
* `simulator.py`: Cỗ máy thời gian giả lập Live Tick 1 giây bằng Brownian Bridge, xử lý lệnh mua bán và Market Impact.
* `analyzer.py`: Tính toán 50 chỉ số định lượng (VaR, Sharpe, Max Drawdown).
* `wallet.py`: Quản lý kế toán lai (Liquid NAV và Locked NAV).


* `src/nlp/` (Trí tuệ Ngôn ngữ)
* `parser.py`: Bộ phân tách ý định lệnh của User.
* `gemini.py`: Giao tiếp API với Google Gemini, thực hiện RAG (Retrieval-Augmented Generation).
* `formatter.py`: Định dạng câu trả lời văn bản.


* `src/utils/` (Công cụ Tiện ích)
* `exceptions.py`: Định nghĩa các lỗi Custom (VD: `InsufficientFundsError`, `MaturityLockedError`) để UI bắt và hiển thị Toast mượt mà.



---

## 4. GIAO DIỆN HIỂN THỊ CHUẨN TRADINGVIEW (THE UI COCKPIT - `ui/`)

Nơi chứa mã nguồn Streamlit. Phải tuân thủ tuyệt đối các Định luật UI/UX đã đề ra (Không tràn RAM, không chớp giật).

* `ui/app.py`: Tệp gốc khởi chạy giao diện, định nghĩa Grid Layout 7.5 - 2.5 và khởi tạo Global Session State.
* `ui/styles.css`: Mã CSS can thiệp sâu vào DOM, xóa padding, tiêm màu Dark Theme `#131722`.
* `ui/components/` (Các mô-đun UI độc lập, tất cả phải dùng `@st.fragment`):
* `time_travel_bar.py`: Thanh topbar tua thời gian.
* `left_toolbar.py`: Thanh công cụ dọc bên trái (Crosshair, Measure, Clear).
* `chart_view.py`: Biểu đồ nến lõi đa lớp (Nến thật, Nến Bóng Ma Market Impact, Volume, MA).
* `order_book_view.py`: Bảng độ sâu thị trường và form đặt lệnh Buy/Sell.
* `watchlist.py`: Danh sách các mã tài sản bên cột phải (Click để đổi Chart).
* `alerts_view.py`: Form thiết lập và quản lý cảnh báo giá/sự kiện vĩ mô.
* `quant_matrix_view.py`: Ma trận 50 con số định lượng và biểu đồ AI Dynamics.
* `technicals_view.py`: Đồng hồ kim đo tốc độ tín hiệu (Gauges) và Bảng Mùa vụ Heatmap.
* `screener_view.py`: Trình lọc tài sản dạng ETF và bản đồ Treemap dòng tiền.
* `news_view.py`: Dòng thời gian tin tức vĩ mô (Macro Sentiment) và điểm sợ hãi/tham lam.
* `portfolio_view.py`: Biểu đồ Sunburst chia tỷ trọng tài sản Liquid (Tiền/Coin) và Locked (Trái phiếu).
* `trade_ledger.py`: Bảng sao kê lịch sử khớp lệnh chi tiết (PnL, Phí).
* `strategy_runner.py`: Bảng điều khiển bấm nút khởi chạy Train/Backtest.
* `learning_monitor.py`: Bảng đọc file `.tfevents` vẽ biểu đồ Loss/Reward của quá trình huấn luyện AI.
* `export_tools.py`: Nút tải báo cáo định lượng PDF / CSV.
* `chat_box.py`: Bong bóng chat RAG Gemini trôi nổi góc dưới cùng bên phải.
* `config_editor.py`: Form chỉnh sửa các file YAML trực tiếp trên nền web.



---

*(Bản thiết kế hệ thống chính thức được khóa chặt. Sẵn sàng tiến hành lập trình)*


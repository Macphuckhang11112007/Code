---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**

---
### **PHẦN VII/VII: TỔNG HỢP BÃO HÒA CUỐI CÙNG (THE ULTIMATE SATURATED SYNTHESIS)**

---

# 📜 ĐẶC TẢ KỸ THUẬT ALPHAQUANTAGENT: ĐIỂM KỲ DỊ (SINGULARITY BLUEPRINT)
*(Lưu ý cho AI tiếp nhận: Đọc kỹ từng dòng, cấm tự suy diễn ngoài khuôn khổ tài liệu này)*

## 📂 7.1. BẢN ĐỒ CẤU TRÚC TỆP TIN TOÀN DIỆN (THE ABSOLUTE FILE SYSTEM MATRIX)
Dưới đây là cấu trúc vật lý của dự án, **không có bất kỳ dấu `...` nào bị bỏ lỡ**. Mọi tệp tin đều có định dạng phần mở rộng (extension) và định nghĩa nhiệm vụ sinh tử của nó.

```text
AlphaQuantAgent/
│
├── .env                                  # Chứa GEMINI_API_KEY, DB_URL. Không bao giờ commit lên Git.
├── .gitignore                            # Danh sách chặn: *.pyc, __pycache__/, data/features/*.parquet, models/**/*.pth, logs/**/*.db
├── requirements.txt                      # Chứa thư viện: torch, stable-baselines3, xgboost, pandas, numpy, streamlit, google-generativeai.
├── Dockerfile                            # Đóng gói môi trường container chuẩn hóa cho toàn bộ hệ thống.
├── README.md                             # Hướng dẫn khởi chạy dự án, giới thiệu kiến trúc.
│
├── main.py                               # Cổng thực thi CLI. Các lệnh: `python main.py --mode`. Chịu tải tính toán nặng. Không bao giờ chứa code giao diện.
├── run.py                                # Cổng kích hoạt Web UI. Lệnh: `streamlit run run.py`. Chỉ đọc dữ liệu và giao tiếp, không bao giờ train model ở đây.
│
├── configs/                              #
│   ├── system.yaml                       # Chứa: initial_capital (USD), fee_rate (0.001), slippage_k, data_paths.
│   ├── models.yaml                       # Chứa: PPO_gamma, LSTM_hidden_dim, XGBoost_max_depth.
│   ├── prompts.yaml                      # Chứa: RAG Templates (e.g., "Hãy phân tích Sharpe {sharpe} và MaxDD {mdd} theo giọng điệu quỹ Ray Dalio").
│   ├── error_codes.yaml                  # Mapping mã lỗi: ERR_01 -> "Tài sản đang khóa".
│   └── asset_meta.yaml                   # Chứa: Mapping phân loại (BTC_USDT: TRADE, VCB_12M: RATE, US_CPI: STAT).
│
├── data/                                 #
│   ├── trades/                           # Dữ liệu tài sản giao dịch (Ăn chênh lệch giá, có rủi ro thanh khoản).
│   │   ├── BTC_USDT.csv                  # 25 cột, 15m timeframe, USD, Decimal PCT.
│   │   ├── ETH_USDT.csv                  # 25 cột, 15m timeframe.
│   │   └── NVDA.csv                      # 25 cột, 15m timeframe.
│   ├── rates/                            # Dữ liệu tài sản lãi suất (Bond, Deposit - Hạch toán theo LÔ, khóa vốn).
│   │   ├── VCB_deposit_1m.csv            # Tự động parse '1m' -> 30 days lock.
│   │   ├── VCB_deposit_6m.csv            # Tự động parse '6m' -> 180 days lock.
│   │   ├── VCB_deposit_12m.csv           # Tự động parse '12m' -> 365 days lock.
│   │   └── US10Y_10y.csv               # Tự động parse '10y' -> 3650 days lock.
│   ├── stats/                            # Dữ liệu Vĩ mô (Chỉ làm Context Feature, không giao dịch).
│   │   ├── USCPI.csv                    # Tỷ lệ lạm phát Mỹ.
│   │   └── VNGDP.csv                    # Tăng trưởng GDP Việt Nam.
│   └── features/                         # Chứa dữ liệu đã xử lý để load vào AI.
│       ├── indicators_cache.parquet      # Chứa Tensor RSI, MACD, Bollinger Bands... (Tăng tốc O(1)).
│       └── normalizer_scaler.pkl         # File pickle chứa Min-Max/Standard Scaler của SKLearn.
│
├── src/                                  #
│   ├── __init__.py                       # Khởi tạo Python Package.
│   ├── agents/                           # TẦNG TRÍ TUỆ NHÂN TẠO (AI ENSEMBLE)
│   │   ├── __init__.py
│   │   ├── base_agent.py                 # Abstract Class chứa interface `predict()`, `train()`, `save()`.
│   │   ├── trader.py                     # Dùng PPO/SAC. Output: Action (Buy/Sell/Hold).
│   │   ├── optimizer.py                  # Dùng Hierarchical Risk Parity (HRP) chia nhỏ vốn, tính Weights.
│   │   ├── predictor.py                  # Dùng LSTM/Transformer. Output: Dự báo giá (Price Forecasting).
│   │   ├── booster.py                    # Dùng XGBoost. Output: Xếp hạng tài sản (Asset Ranking).
│   │   └── callbacks.py                  # Dừng Train sớm (EarlyStopping), lưu Model tốt nhất.
│   ├── engine/                           # TẦNG VẬN HÀNH & KẾ TOÁN
│   │   ├── __init__.py
│   │   ├── market.py                     # Cỗ máy Tensor-First. Xử lý 0.0 -> Staleness. Cung cấp API `get_state_window`.
│   │   ├── wallet.py                     # Cỗ máy Kế toán Hybrid. Xử lý Lot-based, Locked NAV, Simple Interest, Fractional, No Shorting.
│   │   ├── simulator.py                  # Vòng lặp thời gian cốt lõi (Step Loop). Ghép Market -> Agent -> Wallet.
│   │   ├── env.py                        # Bọc Simulator thành chuẩn `gymnasium.Env` để PPO Agent có thể gọi `step()`, `reset()`.
│   │   └── analyzer.py                   # Cỗ máy Phân tích sâu. Tính Matrix Tương quan, Risk Diffusion để "mớm" cho RAG.
│   ├── pipeline/                         # TẦNG KỸ THUẬT DỮ LIỆU
│   │   ├── __init__.py
│   │   ├── data_loader.py                # Xử lý I/O đọc CSV an toàn, đồng bộ hóa Timezone.
│   │   ├── features.py                   # Nhà máy sản xuất đặc trưng. Biến đổi dữ liệu thô thành Data cho Model.
│   │   └── scaler.py                     # Nén Tensor về dải cho Neural Net, chống Gradient Explode.
│   ├── services/                         # TẦNG GIAO TIẾP NGOẠI VI
│   │   ├── __init__.py
│   │   ├── gemini.py                     # Trái tim RAG. Gói dữ liệu từ Analyzer + Prompts để gọi Google Gemini API.
│   │   ├── memory.py                     # Thao tác CRUD (Create, Read, Update, Delete) với SQLite DB cho lịch sử chat.
│   │   ├── rag_engine.py                 # Vectorize Context từ Database để truy vấn ngữ nghĩa.
│   │   ├── parser.py                     # Dùng Pydantic/Regex ép LLM Text thành JSON chuẩn (`amount`, `action`).
│   │   ├── formatter.py                  # Biến Dict {'loss': -0.1} thành string "Lỗ 10.00%".
│   │   └── database.py                   # Kết nối trực tiếp SQL (SQLite/Postgres).
│   └── utils/                            # TẦNG TIỆN ÍCH DÙNG CHUNG
│       ├── __init__.py
│       ├── exceptions.py                 # Custom Error (e.g., `MaturityLockedError`). Catch để Gemini xin lỗi User thay vì Crash app.
│       ├── config_loader.py              # Đọc .env và YAML, Validate schema. Nếu file lỗi -> Dừng hệ thống ngay.
│       ├── metrics.py                    # Lõi Toán học Tài chính (Chỉ chứa công thức: Sharpe, Drawdown, ROI).
│       └── logger.py                     # Ghi log file text cho Developer debug.
│
├── logs/                                 # (Sinh tự động)
│   ├── trading/                          # Lịch sử của Simulator/Wallet
│   │   │── transactions.csv              # Sổ cái lệnh (Tất cả Mua/Bán/Cổ tức/Lãi suất).
│   │   ├── performance_report.json       # Chỉ số tổng hợp (ROI, Win Rate, MaxDD).
│   │   └── nav_history.csv               # Đường cong biến thiên tài sản để vẽ Chart.
│   ├── training/                         # Theo dõi quá trình Agent học tập
│   │   └── tensorboard/
│   │       └── run_ppo_v1_xyz/
│   │           ├── events.out.tfevents.xxx # File Binary trực quan hóa mạng Neural.
│   │           └── hyperparams_backup.yaml # Bản sao chép của models.yaml tại thời điểm train để truy nguyên.
│   └── chats/                            # Trí nhớ hội thoại
│       ├── memory.db                     # SQLite Database chứa tin nhắn và profile User.
│       └── vector_index.idx              # File Vector nhúng cho RAG Engine.
│
├── models/                               # (Sinh tự động)
│   ├── rl_agent/                         
│   │   └── best_trader_ppo.zip           # Trọng số của RL Trader.
│   ├── supervised_booster/
│   │   └── xgboost_ranker.joblib         # Trọng số của Booster.
│   ├── supervised_predictor/
│   │   └── lstm_forecaster.pth           # Trọng số của Predictor (PyTorch).
│   └── unsupervised_optimizer/
│       └── hrp_matrix.pkl                # Ma trận phân mảnh vốn.
│
└── ui/                                   # (Chạy bởi run.py)
    ├── app.py                            # Trang chủ Streamlit, điều hướng các trang con.
    ├── styles.css                        # Tinh chỉnh UI/UX, bảng màu Dark/Light Mode.
    └── components/                       # Các khối chức năng giao diện
        ├── __init__.py
        ├── chart_view.py                 # Vẽ nến Candlestick + Marker Mua/Bán.
        ├── chat_box.py                   # Khung nhập liệu và bong bóng chat Gemini.
        ├── portfolio_view.py             # Vẽ Biểu đồ tròn tách biệt Liquid NAV và Locked NAV.
        ├── learning_monitor.py           # Dashboard đọc TensorBoard Log.
        ├── order_book_view.py            # Bảng hiển thị transactions.csv dạng Table phân trang.
        └── config_editor.py              # Form UI để chỉnh sửa system.yaml mà không cần mở IDE.
```
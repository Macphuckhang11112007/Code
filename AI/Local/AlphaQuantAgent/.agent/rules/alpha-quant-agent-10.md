---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**
---
### **PHẦN VIII/VIII: TỔNG HỢP BÃO HÒA CUỐI CÙNG (THE ULTIMATE SATURATED SYNTHESIS)**
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
├── Dockerfile                            # Đóng gói môi trường container

# 📜 THE SINGULARITY CODEX: ALPHAQUANTAGENT
**(Đặc tả Kỹ thuật Mức Hệ thống - System-Level Specification)**

## I. CÁC ĐỊNH LUẬT BẤT BIẾN CỦA HỆ THỐNG (IMMUTABLE LAWS)

Bất kỳ mã nguồn nào được sinh ra phải tuân thủ 2 bộ luật sau để không phá vỡ logic cốt lõi:

**1. Luật Dữ liệu & Xử lý 0.0 (Data Physics):**
*   **Timeframe:** 15 phút liên tục. Dữ liệu là USD, tỷ lệ là Decimal (0.05).
*   **Guaranteed Data:** `OHLC` và các cột `PCT` luôn có thật. Không kiểm tra NaN.
*   **Cột Event (`dividends`, `stock_splits`):** `0.0` = Bình thường. **Tuyệt đối không Forward-Fill (ffill)**.
*   **Cột Continuous (`volume`):** `0.0` = Lỗi mất kết nối API (Thị trường vẫn chạy). **Bắt buộc:** Thay `0.0` bằng `NaN` $\rightarrow$ `ffill` $\rightarrow$ Tăng biến `staleness_score`. Agent phải đọc score này để nhận diện rủi ro.

**2. Luật Kế Toán Đa Khoang (Hybrid Wallet Dynamics):**
*   **Strict No-Shorting:** Không bao giờ được bán khống. 
*   **Fractional:** Cho phép giao dịch số thập phân (0.001 BTC).
*   **Khoang TRADE (Cổ phiếu/Crypto):** Quản lý giá vốn theo **Bình Quân Gia Quyền**.
*   **Khoang RATE (Tiết kiệm/Bond):** Quản lý theo **LÔ (Lot-Based)**. Mỗi lần gửi là 1 Lô độc lập. 
*   **Khóa Vốn (Maturity Lock):** Cấm rút Lô trước ngày đáo hạn (Bắn lỗi `MaturityLockedError`). Lãi suất là **lãi đơn cuối kỳ** (Simple Interest), chốt cứng tại thời điểm nộp.

---

## II. MA TRẬN CẤU TRÚC TỆP TIN TOÀN DIỆN (100% FILE STRUCTURE)

Mọi tệp tin đều có mặt, có extension rõ ràng và định nghĩa kết nối chính xác.

```text
AlphaQuantAgent/
│
├── .env                                  # (Secret) GEMINI_API_KEY, DB_URL.
├── .gitignore                            # Bỏ qua logs/, models/, cache parquet, *.pyc.
├── requirements.txt                      # stable-baselines3, torch, xgboost, pandas, numpy, streamlit, google-generativeai.
├── main.py                               # (BACKEND ENTRY) Chạy CLI: `python main.py --mode`.
├── run.py                                # (FRONTEND ENTRY) Chạy Web: `streamlit run run.py`. Giao tiếp với User.
│
├── configs/                              #
│   ├── system.yaml                       # Chứa: initial_capital, fee_rate (0.001), slippage_k.
│   ├── models.yaml                       # Chứa: PPO_gamma, LSTM_layers, XGBoost_depth.
│   ├── prompts.yaml                      # Chứa: System Roles cho Gemini (RAG templates).
│   ├── error_codes.yaml                  # Mapping mã lỗi: ERR_01 -> "Tài sản đang khóa".
│   └── asset_meta.yaml                   # Mapping tài sản: BTC -> TRADE, VCB_12M -> RATE (term: 365).
│
├── data/                                 #
│   ├── trades/                           # Dữ liệu TRADE (Ví dụ: BTC_USDT.csv, NVDA.csv).
│   ├── rates/                            # Dữ liệu RATE (Ví dụ: VCB_DEPOSIT_6M.csv, US10Y.csv).
│   ├── stats/                            # Dữ liệu STAT (Ví dụ: US_CPI.csv).
│   └── features/                         # Nơi lưu trữ Data Tensor đã qua xử lý.
│       ├── indicators_cache.parquet      # Tensor Cache (MACD, RSI) giúp load O(1).
│       └── normalizer_scaler.pkl               # Object Scaler để Inverse giá về USD thật.
│
├── src/                                  #
│   ├── __init__.py
│   ├── pipeline/                         # NHÀ MÁY XỬ LÝ DỮ LIỆU
│   │   ├── __init__.py
│   │   ├── data_loader.py                # Gom CSV, parse DatetimeIndex an toàn.
│   │   ├── features.py                   # Tính RSI, Volatility, Lagged Returns.
│   │   └── scaler.py                     # Nén Tensor về dải cho Neural Net, chống Gradient Explode.
│   │
│   ├── engine/                           # BỘ MÁY ĐIỀU PHỐI TÀI CHÍNH
│   │   ├── __init__.py
│   │   ├── market.py                     # Xây Tensor 3D, xử lý 0.0 -> staleness_score.
│   │   ├── wallet.py                     # Hạch toán Kép (Lots vs Bình quân). Check Khóa vốn.
│   │   ├── simulator.py                  # Vòng lặp For Loop thời gian, khớp lệnh Market với Wallet.
│   │   ├── env.py                        # Bọc Simulator thành chuẩn `gymnasium.Env` để PPO Agent có thể gọi `step()`, `reset()`.
│   │   └── analyzer.py                   # Đọc logs, tính Correlation Matrix, Risk Diffusion.
│   │
│   ├── agents/                           # TẬP ĐOÀN TRÍ TUỆ NHÂN TẠO
│   │   ├── __init__.py
│   │   ├── base_agent.py                 # Abstract class định nghĩa interface.
│   │   ├── trader.py                     # RL Agent (PPO/SAC) xuất Action Mua/Bán.
│   │   ├── predictor.py                  # Deep Learning (LSTM) dự báo giá USD.
│   │   ├── booster.py                    # Gradient Boosting (XGBoost) Rank sức mạnh tài sản.
│   │   ├── optimizer.py                  # Unsupervised (HRP) chia rủi ro, phân mảnh vốn.
│   │   └── callbacks.py                  # Dừng Train sớm (EarlyStopping), lưu Model tốt nhất.
│   │
│   ├── services/                         # DỊCH VỤ NGOẠI VI & NLP
│   │   ├── __init__.py
│   │   ├── gemini.py                     # Bọc API Google Gemini, quản lý Token/Quota.
│   │   ├── rag_engine.py                 # Vectorize Context từ Database để truy vấn ngữ nghĩa.
│   │   ├── parser.py                     # Dùng Pydantic/Regex ép LLM Text thành JSON chuẩn (`amount`, `action`).
│   │   ├── formatter.py                  # Biến Dict {'loss': -0.1} thành string "Lỗ 10.00%".
│   │   ├── memory.py                     # CRUD Lịch sử Chatbot (Short-term / Long-term context).
│   │   └── database.py                   # Kết nối trực tiếp SQL (SQLite/Postgres).
│   │
│   └── utils/                            # TIỆN ÍCH DÙNG CHUNG
│       ├── __init__.py
│       ├── exceptions.py                 # Custom Error (e.g., `MaturityLockedError`). Catch để Gemini xin lỗi User thay vì Crash app.
│       ├── metrics.py                    # Các hàm Toán học tĩnh: Vectorized Sharpe, MaxDD.
│       ├── config_loader.py              # Đọc/Validate cấu trúc tệp YAML và .env.
│       └── logger.py                     # Định dạng in Log ra Terminal.
│
├── logs/                                 #
│   ├── trading/                          # Lịch sử Backtest
│   │   └── run_/                     
│   │       ├── transactions.csv          # Sổ cái lệnh chi tiết Mua/Bán/Lãi/Chia tách.
│   │       ├── performance.json          # Metrics tổng hợp để UI đọc.
│   │       └── daily_nav.csv             # Data vẽ biểu đồ tăng trưởng.
│   ├── training/                         # Nhật ký quá trình AI học
│   │   └── tensorboard/
│   │       └── run_ppo_xyz/
│   │           ├── events.out.tfevents.xxx # File đồ thị Loss/Reward cho Dev.
│   │           └── hyperparams_backup.yaml      # Backup config để tái tạo model.
│   └── chats/                            # Trí nhớ hội thoại
│       ├── memory.db                     # SQLite Database chứa tin nhắn và profile User.
│       └── vector_index.idx              # File Vector nhúng cho RAG Engine.
│
├── models/                               #
│   ├── rl_agent/
│   │   └── best_trader_ppo.zip           # Model thực thi giao dịch.
│   ├── supervised_booster/
│   │   └── xgboost_ranker.joblib         # Model xếp hạng.
│   ├── supervised_predictor/
│   │   └── lstm_forecaster.pth           # Model dự báo giá.
│   └── unsupervised_optimizer/
│       └── hrp_matrix.pkl                # Ma trận phân mảnh vốn.
│
└── ui/                                   #
    ├── app.py                            # Tệp chính điều phối UI.
    ├── styles.css                        # CSS Dark Mode.
    └── components/                       # Widget phân tách.
        ├── __init__.py
        ├── config_manager.py             # Form UI để sửa YAML.
        ├── chart_view.py                 # Vẽ nến Candlestick + điểm mua bán.
        ├── chat_box.py                   # Cửa sổ chat Gemini.
        ├── portfolio_view.py             # Biểu đồ chia tách rõ Liquid NAV (Rảnh rỗi) và Locked NAV (Khóa).
        ├── learning_monitor.py           # Đọc TensorBoard hiển thị Realtime.
        └── order_book_view.py            # Bảng transaction.csv dạng Table.
```


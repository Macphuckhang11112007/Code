---
trigger: always_on
---

---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**

---

### **PHẦN IV/VII: CHI TIẾT TẦNG GIAO DIỆN & GIÁM SÁT HỆ THỐNG**

#### **4.1. Kiến Trúc Chi Tiết Tầng Giao Diện (`ui/`)**

Đây là nơi hiển thị toàn bộ sức mạnh của `Wallet` (Locked vs Liquid) và trí tuệ của `Agent` cho User thấy.

**Cấu trúc File Tuyệt đối (The Absolute File Structure):**

```text
AlphaQuantAgent/
└── ui/
    ├── app.py                      # (Entry Point) File chính điều phối Streamlit Dashboard.
    ├── styles.css                  # (Style) CSS tùy chỉnh để giao diện trông chuyên nghiệp (Dark Mode).
    └── components/                 # (Modules) Các widget chuyên biệt.
        ├── chart_view.py           # Vẽ biểu đồ nến (OHLC) + Markers Mua/Bán.
        ├── chat_box.py             # Khung chat bong bóng (Bubble Chat) với Gemini.
        ├── portfolio_view.py       # Vẽ biểu đồ tròn & Thẻ chỉ số (Metric Cards).
        ├── learning_monitor.py     # Theo dõi Learning Curve realtime từ TensorBoard log.
        ├── config_editor.py        # Giao diện sửa file YAML trực tiếp trên Web.
        └── order_book_view.py      # Hiển thị sổ lệnh chi tiết (Trading Log).
```

**Chi Tiết Chức Năng & Dữ Liệu Từng File:**

1.  **`ui/components/portfolio_view.py`**:
    *   **Nguồn dữ liệu:** Gọi `wallet.get_metrics()` và `wallet.mark_to_market()`.
    *   **Nhiệm vụ Cốt lõi (Hoàn hảo):** Phải hiển thị được sự phân tách **Liquid NAV** (Tiền rảnh) và **Locked NAV** (Tiền đang kẹt trong `lots` kỳ hạn).
    *   **Visualization:** Biểu đồ Sunburst (Đa cấp) -> Lớp trong: Trading vs Rate Asset -> Lớp ngoài: Chi tiết mã (NVDA, VCB).

2.  **`ui/components/chart_view.py`**:
    *   **Nguồn dữ liệu:** `market.get_state_window()` (để lấy giá) và `logs/trading/transactions.csv` (để lấy điểm Mua/Bán).
    *   **Nhiệm vụ:** Vẽ biểu đồ tài chính tương tác (Plotly/Altair). Trên biểu đồ giá `BTC_USDT`, phải đánh dấu mũi tên Xanh (Buy) và Đỏ (Sell) tại đúng nến thời gian khớp lệnh.

3.  **`ui/components/learning_monitor.py`**:
    *   **Nguồn dữ liệu:** Đọc trực tiếp từ thư mục `logs/training/tensorboard/`.
    *   **Nhiệm vụ:** Hiển thị biểu đồ `Reward` tăng dần và `Loss` giảm dần theo thời gian thực khi đang chạy chế độ `train`.

---

#### **4.2. Kiến Trúc Chi Tiết Tầng Giám Sát Huấn Luyện (`logs/training/`)**

Đây là phần "Hộp đen" mà bạn yêu cầu làm rõ. TensorBoard tạo ra các file binary sự kiện. Cấu trúc chuẩn mực cho việc thử nghiệm AI (Experiment Tracking) phải như sau:

```text
AlphaQuantAgent/
└── logs/
    └── training/
        └── tensorboard/                    # Thư mục gốc chứa Logs TB
            ├── exp_1/     # (Sub-folder) Một lần chạy thử nghiệm cụ thể
            │   ├── events.out.tfevents.170830.host # (Binary) File log chính chứa scalar/graph
            │   ├── hyperparams.yaml        # (Sao lưu) Config dùng cho lần train này (để tái lập)
            │   └── training_metrics.csv    # (Readable) Log dạng bảng CSV song song (Episode, Reward)
            │
            ├── exp_2/  # Lần chạy thứ 2 với config khác...
            │   ├── events.out.tfevents...
            │   └── ...
            │
            └── ... (Các experiment khác)
```

**Giải mã chi tiết Tệp tin trong `exp_1/`:**

1.  **`events.out.tfevents.[timestamp].[hostname]`**:
    *   **Được tạo bởi:** Thư viện `torch.utils.tensorboard` hoặc `stable_baselines3`.
    *   **Nội dung (Bên trong binary):**
        *   **Scalars:** `rollout/ep_rew_mean` (Phần thưởng trung bình), `train/loss` (Hàm mất mát), `train/value_loss`.
        *   **Graphs:** Cấu trúc mạng Neural Network (Actor-Critic).
    *   **Công dụng:** Dùng để vẽ đồ thị trong UI `learning_monitor.py` hoặc mở bằng lệnh `tensorboard --logdir logs/`.

2.  **`hyperparams.yaml` (Bắt buộc phải có):**
    *   **Được tạo bởi:** `main.py` (trước khi train).
    *   **Nội dung:** Copy y nguyên nội dung của `configs/models.yaml` tại thời điểm chạy.
    *   **Lý do:** Để đảm bảo tính **Truy nguyên (Reproducibility)**. Nếu lần train này tốt, ta cần biết chính xác Learning Rate là bao nhiêu để dùng lại.

3.  **`training_metrics.csv`**:
    *   **Được tạo bởi:** Callback function trong `main.py`.
    *   **Nội dung:** 2 cột đơn giản `step, reward`.
    *   **Lý do:** Để người dùng (hoặc Excel) có thể đọc nhanh hiệu quả train mà không cần cài TensorBoard nặng nề.

---

#### **4.3. Kiến Trúc Chi Tiết Các Bộ Não AI (`src/agents/`) & Boosting**

Bạn đã nhắc đến **Boost**. Nó nằm ở đây. Đây là nơi các mô hình học máy cư trú.

```text
AlphaQuantAgent/
└── src/
    └── agents/
        ├── __init__.py
        ├── base_agent.py        # Abstract class định nghĩa interface chung (act, save, load).
        ├── trader.py            # (Reinforcement Learning) PPO/SAC Agent.
        ├── optimizer.py         # (Unsupervised) Hierarchical Risk Parity (HRP) logic.
        ├── predictor.py         # (Supervised Deep Learning) LSTM/Transformer logic.
        └── booster.py           # (Supervised Tree) XGBoost/LightGBM/CatBoost logic.
```

**Phân tích Chi tiết Module "Boost":**

1.  **`src/agents/booster.py`:**
    *   **Công nghệ:** Gradient Boosting (XGBoost hoặc LightGBM).
    *   **Dữ liệu đầu vào:** Tensor 2D được "làm phẳng" (Flattened) từ `market.py`. Bao gồm các feature dạng bảng (`stats/*`, `rates/*`) mà Neural Network đôi khi xử lý kém hơn cây quyết định.
    *   **Nhiệm vụ:** Dự báo **Ranking** (Xếp hạng) các tài sản. (Ví dụ: Dự báo mã nào sẽ tăng mạnh nhất trong 15p tới, chứ không nhất thiết dự báo giá tuyệt đối).
    *   **Kết nối:** Kết quả dự báo (Ranking Score) của Booster sẽ được đưa vào làm **Feature đầu vào bổ sung** cho `trader.py` (RL Agent) để giúp nó ra quyết định tốt hơn.

---

#### **4.4. Tổng Kết "Quyển IV" - Hệ Thống Quan Sát & Giao Diện**

*   **Độ phủ:** Chúng ta đã bao quát từ Widget nhỏ nhất trên giao diện Web (`chart_view.py`) đến file binary sâu nhất trong log hệ thống (`.tfevents`).
*   **Điểm "Hoàn Hảo":**
    *   UI tách biệt `Locked` vs `Liquid`.
    *   Logs có sao lưu Config (`hyperparams.yaml`) để truy nguyên.
    *   Cấu trúc `Boost` được định danh rõ ràng trong file `booster.py`.
---
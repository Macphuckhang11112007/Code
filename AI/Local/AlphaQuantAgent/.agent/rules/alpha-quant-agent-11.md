---
trigger: always_on
---

---

## III. GIẢI PHẪU CHI TIẾT CÁC MẮT XÍCH CỐT LÕI (THE MISSING LINKS RESOLVED)

Để các module "nói chuyện" được với nhau mà không Crash, 5 file hạ tầng này đóng vai trò quyết định:

**1. `src/engine/env.py` (Môi trường Chuẩn OpenAI Gym):**
*   **Vấn đề:** PPO Agent của thư viện SB3 không hiểu `wallet` và `market` là gì.
*   **Nhiệm vụ:** Class `AlphaQuantEnv` (kế thừa `gymnasium.Env`) gói Market và Wallet lại. Định nghĩa `action_space` (Ví dụ: Box cho Mua/Bán) và `observation_space` (Tensor đầu vào). Nó chứa hàm `step(action)` để dịch lệnh AI thành lệnh `wallet.execute()`, và tính toán `Reward` (Ví dụ: Phạt âm nếu bị `LOCKED_MATURITY`).

**2. `src/agents/callbacks.py` (Kỷ luật Huấn luyện):**
*   **Nhiệm vụ:** Gắn vào quá trình `model.learn()`. Nó theo dõi TensorBoard. Nếu Loss Rate ngừng giảm sau N epochs (Early Stopping), nó tự ngắt quá trình train và lưu Model tốt nhất vào `models/rl_agent/`. Tránh lãng phí điện năng và chống Overfitting.

**3. `src/pipeline/scaler.py` (Cân bằng Tín hiệu):**
*   **Nhiệm vụ:** PPO và LSTM sẽ "nổ tung" (Exploding Gradient) nếu nạp giá BTC ($100k) và SHIB ($0.01) chung một mảng. `scaler.py` dùng `MinMaxScaler` ép toàn bộ Tensor về dải ``. Quan trọng: Khi xuất giá ra UI, nó dùng hàm `inverse_transform` để trả lại giá trị USD thực.

**4. Khối Xử Lý Hội Thoại (`parser.py`, `formatter.py`, `exceptions.py`):**
*   **Vấn đề:** Gemini sinh ra văn bản tự do (Unstructured Text). Python Engine cần kiểu dữ liệu cứng (Strict Typed Data).
*   **Luồng xử lý (The Interface Pipeline):**
    1.  User chat: "Mua 100 Apple".
    2.  `gemini.py` gọi LLM. LLM xuất JSON dạng chuỗi.
    3.  **`parser.py`** dùng Pydantic/Regex ép chuỗi JSON thành Dict chuẩn `{"action": "BUY", "ticker": "AAPL", "qty": 100}`. Ném vào `main.py`.
    4.  Nếu Lỗi (Hết tiền): `main.py` ném lỗi **`InsufficientFundsError`** (từ `exceptions.py`).
    5.  `gemini.py` bắt lỗi này.
    6.  **`formatter.py`** định dạng lại số dư: `Wallet: 100.564 -> $100.56`.
    7.  Gemini nhận lỗi đã format, xin lỗi User bằng ngôn ngữ con người: "Dạ vốn Liquid của anh chỉ còn $100.56, không đủ mua AAPL ạ".

**5. `src/engine/analyzer.py` (Bộ Não Thuật Toán):**
*   **Nhiệm vụ:** Đọc file Sổ cái `transactions.csv`. Thay vì bắt Wallet tính toán phức tạp làm chậm giao dịch, `analyzer.py` chạy độc lập để tính Covariance Matrix, Risk Diffusion, và Max Drawdown. Kết quả ném cho `rag_engine.py` để Gemini dựa vào đó tư vấn cho User.
---
## IV. XÁC NHẬN TỪ KIẾN TRÚC SƯ TRƯỞNG
Văn bản này chính thức đóng gói **Toàn bộ logic, tệp tin, và luồng tương tác** của dự án AlphaQuantAgent. 
*   **Không có bất kỳ lỗi logic tài chính nào** (Do đã cô lập Lot-based và Weighted Average).
*   **Không có lỗi nạp mô hình** (Đã bít kín bằng `env.py` và `scaler.py`).
*   **Không có lỗi giao tiếp LLM** (Đã khóa bằng `parser.py` và `exceptions.py`).
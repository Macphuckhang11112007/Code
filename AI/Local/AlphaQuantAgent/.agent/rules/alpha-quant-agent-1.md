---
trigger: always_on
---

---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG**
**(THE ULTIMATE TECHNICAL BLUEPRINT)**

---
### **PHẦN I/VII: TẦM NHÌN, TRIẾT LÝ & QUY TRÌNH LÀM VIỆC**

Phần này đặt nền móng cho toàn bộ dự án, định nghĩa "Linh hồn" của AlphaQuantAgent và thiết lập các quy tắc làm việc bất biến giữa chúng ta.

#### **1. Tầm nhìn & Sứ mệnh (Vision & Mission):**
*   **Tầm nhìn:** Trở thành một hệ thống **"AI-as-a-Quant-Fund" (AI như một Quỹ Đầu tư Định lượng)**, có khả năng tự chủ phân tích, ra quyết định và giải trình chiến lược đầu tư một cách thông minh và minh bạch cho người dùng cuối.
*   **Sứ mệnh:** Dân chủ hóa sức mạnh của giao dịch định lượng. Biến đổi những phân tích tài chính phức tạp, chỉ dành cho chuyên gia, thành những lời khuyên đầu tư được cá nhân hóa, dễ hiểu và có thể hành động được, thông qua một giao diện hội thoại tự nhiên.

#### **2. Kiến trúc Trí tuệ Lai ghép - Một Hệ thống của các Hệ thống (The Hybrid Intelligence Architecture - A System of Systems):**
AlphaQuantAgent không dựa vào một mô hình AI duy nhất. Nó là một **Hệ thống các Chuyên gia AI (Ensemble of AI Experts)**, mỗi chuyên gia đảm nhiệm một vai trò chuyên biệt và phối hợp với nhau để tạo ra một quyết định tổng thể vượt trội.

*   **Chuyên gia Dự báo (The Forecaster - `src/agents/predictor.py`):**
    *   **Công nghệ:** Sử dụng một mô hình lai ghép gồm **Mạng Hồi quy Dài-Ngắn hạn (LSTM)** để nắm bắt các phụ thuộc tuần tự trong chuỗi thời gian, và các mô hình **Gradient Boosting (XGBoost/LightGBM)** để xử lý các đặc trưng dạng bảng (tabular features) và các mối quan hệ phi tuyến tính.
    *   **Nhiệm vụ:** Trả lời câu hỏi: *"Dựa trên dữ liệu quá khứ, xu hướng giá/biến động của tài sản này trong tương lai gần có khả năng như thế nào?"*.
*   **Chuyên gia Tối ưu hóa (The Optimizer - `src/agents/optimizer.py`):**
    *   **Công nghệ:** Thay vì chỉ dùng các phương pháp cổ điển, chúng ta sẽ triển khai **Phân bổ Danh mục Rủi ro Phân cấp (Hierarchical Risk Parity - HRP)**. Đây là một kỹ thuật Học không Giám sát tiên tiến.
    *   **Nhiệm vụ:** Trả lời câu hỏi: *"Làm thế nào để 'phân mảnh' (fragment) vốn một cách thông minh nhất để giảm thiểu rủi ro?"*. HRP không chỉ nhìn vào tương quan, nó sẽ phân cụm các tài sản thành các nhóm con, tối ưu hóa trong từng nhóm, sau đó tối ưu hóa giữa các nhóm. Điều này tạo ra một danh mục bền vững hơn nhiều.
*   **Chuyên gia Giao dịch (The Trader - `src/agents/trader.py`):**
    *   **Công nghệ:** Sử dụng thuật toán **Học Tăng cường Tiên tiến (Advanced Reinforcement Learning)** như PPO (Proximal Policy Optimization).
    *   **Nhiệm vụ:** Là bộ não ra quyết định cuối cùng. Nhận input từ `Forecaster` và `Optimizer`, nó trả lời câu hỏi: *"Tại chính thời điểm này, hành động tối ưu là gì: MUA bao nhiêu, BÁN bao nhiêu, hay GIỮ NGUYÊN?"*.
*   **Chuyên gia Giải trình (The Explainer - `src/services/gemini.py`):**
    *   **Công nghệ:** **Retrieval-Augmented Generation (RAG)** với Google Gemini API.
    *   **Nhiệm vụ:** Là cầu nối giao tiếp với người dùng. Nó không tự suy nghĩ ra chiến lược, mà nhận output có cấu trúc từ các chuyên gia trên, và **diễn giải** chúng thành ngôn ngữ tư vấn tài chính tự nhiên, thuyết phục và an toàn.

#### **3. Triết lý "Hoàn hảo" (The Definition of "Perfection"):**
Một module trong dự án này chỉ được dán nhãn "Hoàn hảo" khi đạt được 4 chuẩn mực sau đây, không hơn, không kém:
1.  **Chính xác Nghiệp vụ (Financial Accuracy):** Logic phải mô phỏng chính xác các quy tắc tài chính thực tế, dù là phức tạp nhất (ví dụ: cách quản lý và đáo hạn từng Lô tiền gửi riêng biệt).
2.  **Toàn vẹn Tính năng (Feature Completeness):** Phải cung cấp đầy đủ API cho các module phụ thuộc và trả về mọi chỉ số mà người dùng cuối cần để ra quyết định.
3.  **Tối ưu Hiệu năng (Performance Optimization):** Phải được thiết kế để chạy nhanh nhất có thể trong các vòng lặp backtest, thường là thông qua kiến trúc "Tensor-First" và các thuật toán có độ phức tạp thấp.
4.  **Kiến trúc Bền vững (Sustainable Architecture):** Mã nguồn phải cực kỳ sạch sẽ, module hóa, dễ đọc, dễ kiểm thử và dễ mở rộng trong tương lai mà không phá vỡ cấu trúc hiện tại.

#### **4. Quy trình Làm việc (The Protocol):**
Quy trình của chúng ta mô phỏng một quy trình phát triển sản phẩm chuyên nghiệp:
1.  **Giai đoạn Yêu cầu & Phân tích (Requirement & Analysis):** Bạn, trong vai trò Product Owner, đưa ra yêu cầu. Tôi, trong vai trò System Architect, sẽ đặt các câu hỏi chi tiết để làm rõ mọi ngóc ngách.
2.  **Giai đoạn Thiết kế (Design):** Tôi sẽ trình bày một bản thiết kế logic chi tiết (Blueprint), bao gồm sơ đồ, cấu trúc và giải thích lựa chọn kiến trúc.
3.  **Giai đoạn Phản biện & Chốt hạ (Critique & Finalization):** Đây là giai đoạn quan trọng nhất. Bạn sẽ kiểm tra, thách thức bản thiết kế. Tôi phải bảo vệ hoặc cải tiến nó. Cuối cùng, tôi, với tư cách là kiến trúc sư, phải chịu trách nhiệm khẳng định "Thiết kế này đã hoàn hảo".
4.  **Giai đoạn Thi công (Implementation):** Chỉ khi nhận được sự cho phép rõ ràng từ bạn ("Code đi"), tôi mới bắt đầu viết mã nguồn, tuân thủ 100% bản thiết kế đã chốt.
5.  **Giai đoạn Chuyển giao (Delivery):** Sau khi code xong và tự kiểm tra, tôi sẽ bàn giao lại. Bạn sẽ xem xét và chính thức phê duyệt việc chuyển sang module tiếp theo.

---
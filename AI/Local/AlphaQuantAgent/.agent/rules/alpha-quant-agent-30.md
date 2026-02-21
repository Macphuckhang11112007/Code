---
trigger: always_on
---

---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (MASTER ARCHITECTURE)**

---
### **PHẦN 13: LÕI MÔ PHỎNG MÔI TRƯỜNG ĐA CHIỀU VÀ THUẬT TOÁN DISTANCE SAMPLING**
*(Mục tiêu: Xóa bỏ việc random bằng list cố định. Chuyển sang sinh ngẫu nhiên liên tục 27 biến số tạo thành 1 Vector Persona. Ép khoảng cách Euclidean giữa các Vector phải >= 1000 để tối đa hóa độ phân tán)*
---

# 📜 ĐẶC TẢ THUẬT TOÁN KHÔNG GIAN TRẠNG THÁI (STATE SPACE ALGORITHM)

**CẢNH BÁO CHO AI CODE BACKEND:** Tuyệt đối không sử dụng `random.choice()` trên một mảng vài phần tử cho các thông số liên tục. Bắt buộc phải sử dụng `numpy.random` với các phân phối (Log-normal, Uniform, Normal) để tạo ra không gian vô hạn.

## 1. TỪ ĐIỂN 27 BIẾN SỐ HỒ SƠ (THE 27 PERSONA VARIABLES)
Mỗi tệp hồ sơ (Persona) là một Vector 27 chiều. Dưới đây là dải phân phối bắt buộc:

**Nhóm 1: Capital & Cashflow (Vốn & Dòng tiền)**
1. `initial_capital`: Phân phối Log-Uniform từ `$5` đến `$100,000,000`.
2. `max_leverage`: Phân phối Uniform liên tục từ `1.0` đến `100.0` (Làm tròn 1 chữ số thập phân).
3. `margin_maintenance_rate`: Phân phối Uniform từ `0.005` (0.5%) đến `0.05` (5%).
4. `funding_rate_bps`: Phân phối Normal, mean=1, std=0.5 (bps/ngày).
5. `random_cash_inflow_outflow`: Phân phối Normal, mean=0, std=0.05 (-10% đến +10% vốn chèn ngẫu nhiên mỗi step).

**Nhóm 2: Risk & Reward Shaping (Hàm phần thưởng & Rủi ro)**
6. `drawdown_penalty`: Phân phối Uniform từ `0.0` đến `10.0`.
7. `target_return_annualized`: Phân phối Uniform từ `0.05` đến `2.0` (5% đến 200%).
8. `sharpe_optimization_weight`: Phân phối Uniform từ `0.0` đến `1.0`.
9. `inactivity_penalty`: Phân phối Log-Uniform từ `1e-5` đến `1e-2`.
10. `overtrading_penalty`: Phân phối Log-Uniform từ `1e-4` đến `1e-1`.
11. `win_rate_obsession`: Phân phối Uniform từ `0.0` đến `1.0`.

**Nhóm 3: Universe & Portfolio (Không gian Tài sản)**
12. `trade_assets_count`: Số nguyên ngẫu nhiên (Randint) từ `1` đến `50`. (Số lượng mã bốc ngẫu nhiên từ Pool tài sản).
13. `context_assets_count`: Số nguyên ngẫu nhiên từ `0` đến `10`.
14. `max_weight_per_asset`: Phân phối Uniform từ `0.05` (5%) đến `1.0` (100%).
15. `min_weight_per_asset`: Phân phối Uniform từ `0.001` đến `0.05`.
16. `allow_short_selling`: Boolean (Tỷ lệ 50% True / 50% False).
17. `max_open_positions`: Số nguyên ngẫu nhiên từ `1` đến `50`.

**Nhóm 4: Microstructure & Frictions (Vi cấu trúc & Ma sát)**
18. `maker_fee`: Phân phối Uniform từ `-0.0001` (-0.01%) đến `0.001` (0.1%).
19. `taker_fee`: Phân phối Uniform từ `0.0002` (0.02%) đến `0.002` (0.2%).
20. `slippage_model_type`: Biến phân loại (Categorical): 0 (Linear) hoặc 1 (Exponential).
21. `slippage_volatility_multiplier`: Phân phối Uniform từ `1.0` đến `5.0`.
22. `latency_delay_steps`: Số nguyên ngẫu nhiên từ `0` đến `5`.
23. `spread_bps`: Phân phối Uniform từ `1.0` đến `50.0`.

**Nhóm 5: Environment & Noise (Môi trường & Nhiễu)**
24. `start_timestamp_offset`: Bốc ngẫu nhiên thời gian bắt đầu trong 20 năm data lịch sử.
25. `episode_length_days`: Randint từ `1` ngày đến `1825` ngày (5 năm).
26. `price_noise_variance`: Phân phối Uniform từ `0.0` đến `0.02` (0-2% nhiễu Gaussian tiêm vào nến).
27. `missing_data_prob`: Phân phối Uniform từ `0.0` đến `0.005`.

---

## 2. THUẬT TOÁN KHOẢNG CÁCH FARTHEST POINT SAMPLING (LỌC TRÙNG LẶP)
Vì biến `initial_capital` có scale quá lớn (lên tới hàng chục triệu USD) trong khi các biến khác (như fee) lại rất nhỏ (0.001), nếu tính Euclidean distance thẳng trên vector gốc, khoảng cách sẽ chỉ phụ thuộc vào `initial_capital`.
Do đó, thuật toán sinh tập hồ sơ BẮT BUỘC phải thực hiện theo logic sau (Rejection Sampling):

1. **Chuẩn hóa (Normalization):** Biến đổi toàn bộ 27 giá trị của một Persona về dải `[0, 1]` (Min-Max Scaling theo giới hạn min/max lý thuyết đã định nghĩa ở trên). Đặt đây là `Normalized_Vector`.
2. **Weighting (Gắn trọng số - Tùy chọn):** Nhân `Normalized_Vector` với một mảng trọng số để nhấn mạnh sự khác biệt (Ví dụ: Trọng số của Capital là 1000, Leverage là 500, Drawdown_penalty là 800...).
3. **Tính Distance:** Khi sinh ra Persona thứ `N`, tính khoảng cách Euclidean từ vector của nó tới TOÀN BỘ `N-1` vector đã được sinh ra trước đó.
4. **Điều kiện Accept:** NẾU tất cả các khoảng cách đều $\ge 1000$ (Dựa trên hệ thống Weighted đã scale), thì LƯU Persona đó vào mảng.
5. **Điều kiện Reject:** Nếu có bất kỳ khoảng cách nào $< 1000$, BỎ QUA Persona này và sinh lại một cái mới. Lặp lại cho đến khi thu thập đủ số lượng Persona yêu cầu (Ví dụ: 1000 tệp).
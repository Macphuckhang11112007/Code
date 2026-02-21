---
trigger: always_on
---

---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**
**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---
### **PHẦN 12: SIÊU MA TRẬN THỐNG KÊ LƯỢNG TỬ VÀ TRÍ TUỆ NHÂN TẠO (THE ULTIMATE QUANT AI MODEL MATRIX)**
*(Mục tiêu: Đặc tả chi tiết 100% các chỉ số đo lường AI, Quản trị rủi ro, và Toán học danh mục hiển thị trong Tab "Quant AI Model" - Không bao gồm phân tích tin tức)*
---

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 12 (INFRASTRUCTURE BLUEPRINT 12)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Đây là phần cốt lõi học thuật nhất của toàn bộ dự án. Giao diện Streamlit **BẮT BUỘC** phải chia Tab `Quant AI Model` thành **6 Sub-tabs (Tab con)**. Mọi con số ở đây phải được đọc từ file `logs/trading/advanced_quant_metrics.json`. Tuyệt đối không tính toán lại các công thức này trên UI để tránh sập RAM. Dữ liệu sẽ được Backend (Pandas/NumPy/SciPy) tính toán trước.

---

## CẤU TRÚC GIAO DIỆN CHÍNH: `ui/components/quant_matrix_view.py`

Sử dụng `st.tabs()` để chia không gian màn hình thành 6 khu vực chuyên sâu:
`t1, t2, t3, t4, t5, t6 = st.tabs(["🧠 AI Brain", "🌪️ Risk & Volatility", "👑 Advanced Ratios", "📊 Returns", "⏱️ Execution", "🌐 Portfolio & Survival"])`

---

### TAB 1: 🧠 AI BRAIN & LEARNING DYNAMICS (TRẠNG THÁI NÃO BỘ AI)
*Mục đích: Giám sát "bên trong" mạng nơ-ron và các thuật toán Machine Learning.*

**Khối 1: Reinforcement Learning (PPO/DQN Core)**
* `Mean Episodic Reward`: Phần thưởng trung bình đạt được mỗi chu kỳ (Dạng line chart).
* `Policy Loss (Actor Loss)`: Sai số trong quyết định xác suất hành động (Càng hội tụ về 0 càng tốt).
* `Value Loss (Critic Loss)`: Sai số trong việc dự báo giá trị tương lai của trạng thái.
* `Entropy`: Độ nhiễu/Mức độ khám phá. (Giá trị cao: AI đang mò mẫm; Giá trị thấp: AI đã tự tin).
* `KL Divergence`: Độ chênh lệch giữa chính sách cũ và mới (Giới hạn cập nhật tệp trọng số).
* `Advantage Estimate`: Điểm ước lượng lợi thế của hành động hiện tại so với mức trung bình.
* `Clip Fraction`: Tỷ lệ cắt xén gradient bảo vệ PPO khỏi việc học lệch.
* `Q-Value Spread`: Độ chênh lệch giá trị kỳ vọng giữa hành động Tốt nhất và Tệ nhất.

**Khối 2: Supervised Learning & Trees (XGBoost/LSTM Core)**
* `Feature Importance`: Biểu đồ Bar Chart nằm ngang (Top 10 chỉ báo AI đang dựa vào nhiều nhất, ví dụ: RSI > MA50 > MACD).
* `SHAP Values`: Đóng góp của từng đặc trưng vào quyết định mua/bán (Force plot).
* `Prediction Accuracy (RMSE / Log-Loss)`: Sai số dự báo của mô hình phụ trợ.

**Khối 3: Action Dynamics (Động lực học hành vi)**
* `Action Probability Distribution`: Biểu đồ miền (Area chart) hiển thị % xác suất AI muốn `[Buy, Sell, Hold]` theo thời gian thực.
* `Exploration vs Exploitation Ratio`: Tỷ lệ Thử nghiệm ngẫu nhiên so với Khai thác kiến thức cũ.

---

### TAB 2: 🌪️ RISK & VOLATILITY (QUẢN TRỊ RỦI RO & BIẾN ĐỘNG)
*Mục đích: Đo lường rủi ro đuôi (Tail Risk) và các kịch bản thiên nga đen.*

**Khối 1: Volatility Metrics (Độ biến động)**
* `Historical Volatility (HV)`: Độ lệch chuẩn của lợi nhuận (Tính theo năm).
* `Downside Volatility`: Độ biến động chỉ xét trên các ngày thua lỗ (Dùng cho Sortino).
* `Upside Volatility`: Độ biến động chỉ xét trên các ngày có lãi.
* `Parkinson Volatility`: Biến động dựa trên khoảng cách (Đỉnh - Đáy) trong ngày.
* `Garman-Klass Volatility`: Biến động bao gồm cả (Đỉnh, Đáy, Mở, Đóng).
* `EWMA Volatility`: Độ biến động trung bình trượt hàm mũ (Nhạy cảm với cú sốc gần nhất).

**Khối 2: Drawdown Profiling (Hồ sơ Sụt giảm)**
* `Max Drawdown (MDD)`: Mức sụt giảm vốn lớn nhất từ đỉnh cao nhất (All-time).
* `Average Drawdown`: Trung bình của tất cả các đợt sụt giảm lớn hơn 1%.
* `Current Drawdown`: Tỷ lệ sụt giảm tính từ đỉnh gần nhất đến giá trị tài khoản hiện tại.
* `Time Under Water (Drawdown Duration)`: Thời gian kẹt trong vùng lỗ (Đo bằng ngày hoặc tick).
* `Recovery Time`: Số ngày trung bình để tài khoản vượt lên đỉnh cũ sau khi chạm đáy Drawdown.

**Khối 3: Tail Risk & Distribution (Rủi ro Đuôi cực đoan)**
* `Value at Risk (VaR 95% & 99%)`: Mức thiệt hại tối đa trong 1 khoảng thời gian với độ tin cậy 95%/99%.
* `Conditional VaR (CVaR / Expected Shortfall)`: Thiệt hại trung bình nếu kịch bản tồi tệ vượt qua mốc VaR xảy ra.
* `Skewness (Độ lệch)`: Sự bất đối xứng của phân phối lợi nhuận (Skew < 0 là cực kỳ rủi ro).
* `Kurtosis (Độ nhọn)`: Khả năng xảy ra Thiên nga đen (Kurtosis > 3 (Leptokurtic) nghĩa là rủi ro đuôi rất dày).
* `Ulcer Index`: Chỉ số đo lường độ căng thẳng, tính toán dựa trên độ sâu và độ dài của Drawdown.

---

### TAB 3: 👑 ADVANCED INSTITUTIONAL RATIOS (HỆ SỐ QUỸ ĐẦU TƯ CẤP CAO)
*Mục đích: Các công thức toán học định lượng đánh giá chất lượng sinh lời.*

* `Sharpe Ratio`: (Lợi nhuận vượt rủi ro / Tổng rủi ro). Chuẩn công nghiệp.
* `Sortino Ratio`: Phiên bản nâng cấp của Sharpe, chỉ trừng phạt rủi ro giảm giá (Downside Volatility).
* `Calmar Ratio`: (Lợi nhuận kép hàng năm / Max Drawdown). Rất quan trọng với quỹ Trend-following.
* `Treynor Ratio`: (Lợi nhuận vượt rủi ro / Beta).
* `Information Ratio (IR)`: Đo lường kỹ năng vượt trội so với chỉ số tham chiếu (Benchmark).
* `Omega Ratio`: Tỷ lệ giữa hàm phân phối lợi nhuận dương so với lợi nhuận âm (Thay thế Sharpe khi phân phối không chuẩn).
* `Sterling Ratio`: (Lợi nhuận trung bình / Lỗi sụt giảm vốn trung bình + 10%).
* `Burke Ratio`: Trừng phạt các hệ thống có nhiều đợt Drawdown sâu liên tiếp.
* `K-Ratio`: Đo lường sự nhất quán và mượt mà của đường cong vốn (Equity Curve).
* `Kappa Ratio`: Đo lường rủi ro bất đối xứng của toàn bộ danh mục.

---

### TAB 4: 📊 PROFITABILITY & RETURNS (HỒ SƠ SINH LỜI)
*Mục đích: Bản báo cáo kế toán dòng tiền.*

* `Total / Cumulative Return`: Tổng % lợi nhuận từ lúc bắt đầu.
* `CAGR (Compound Annual Growth Rate)`: Lợi nhuận gộp quy năm.
* `Arithmetic Mean Return`: Lợi nhuận trung bình cộng.
* `Geometric Mean Return`: Lợi nhuận trung bình nhân (Thực tế hơn khi tính lãi kép).
* `Rolling Returns`: Lợi nhuận cuộn (1 Tháng, 3 Tháng, 6 Tháng, 1 Năm) - Hiển thị dạng Bar chart so sánh.
* `YTD (Year-to-Date)`: Lợi nhuận từ ngày 1/1 đến nay.
* `Gross Profit`: Tổng số tiền kiếm được từ lệnh thắng.
* `Gross Loss`: Tổng số tiền mất từ lệnh thua.
* `Net Profit`: Lợi nhuận ròng tuyệt đối.

---

### TAB 5: ⏱️ EXECUTION & MICROSTRUCTURE (VI CẤU TRÚC KHỚP LỆNH)
*Mục đích: Đánh giá chi phí chìm và cách hệ thống đấm lệnh vào sàn.*

**Khối 1: Trade Statistics (Thống kê lệnh)**
* `Win Rate`: Tỷ lệ lệnh thắng (%) / `Loss Rate`: Tỷ lệ lệnh thua (%).
* `Profit Factor`: Tổng lãi / Tổng lỗ. (Tuyệt đối phải > 1.2 để sinh tồn).
* `Risk/Reward Ratio (R:R)`: Lãi trung bình / Lỗ trung bình mỗi lệnh.
* `Payoff Ratio`: Tỷ số hoàn vốn.
* `Total Trades`: Tổng số lệnh đã thực thi.
* `Max Consecutive Wins / Losses`: Chuỗi thắng / Chuỗi thua dài nhất liên tiếp.
* `Average Holding Time`: Thời gian cầm lệnh trung bình (Phân tách rõ Long vs Short).
* `Long/Short Ratio`: Tỷ lệ mở vị thế Mua so với Bán khống.

**Khối 2: Slippage & Order Book Microstructure (Chi phí & Vi cấu trúc Sổ lệnh)**
* `Total Slippage Cost`: Tổng thiệt hại tiền mặt do hiệu ứng trượt giá (Market Impact).
* `Execution Shortfall`: Độ hụt giá khớp lệnh thực tế so với giá tín hiệu của mô hình.
* `Total Commissions Paid`: Tổng phí giao dịch đã nộp cho sàn.
* `Margin Utilization`: Tỷ lệ đòn bẩy trung bình đang sử dụng (Ký quỹ).
* `Order Book Imbalance (OBI)`: Trung bình độ lệch giữa Bid/Ask tại thời điểm đặt lệnh.
* `VPIN (Volume-Synchronized Probability of Informed Trading)`: Đo lường rủi ro thanh khoản từ các lệnh nội gián.
* `Bid-Ask Spread Variance`: Độ giãn chênh lệch giá lúc khớp lệnh.

---

### TAB 6: 🌐 PORTFOLIO MATH & SURVIVAL (TOÁN DANH MỤC & SINH TỒN)
*Mục đích: Kiểm định sức chịu đựng của thuật toán và phân bổ vốn liên thị trường.*

**Khối 1: Macro & Correlation Math (Toán Vĩ mô & Tương quan)**
* `Alpha (Jensen's Alpha)`: Khả năng tự tạo ra tiền của AI không phụ thuộc vào thị trường chung.
* `Beta (Market Exposure)`: Hệ số rủi ro hệ thống (Sự nhạy cảm với sập giá chung).
* `R-Squared (R²)`: Mức độ tương quan định tuyến với Benchmark.
* `Tracking Error`: Mức độ sai lệch quỹ đạo so với tài sản neo.
* `Correlation Matrix`: Ma trận Heatmap Tương quan (Pearson) giữa mọi cặp tài sản trong Watchlist.
* `Covariance Matrix`: Ma trận Hiệp phương sai đo lường rủi ro lây nhiễm chéo.
* `Hurst Exponent`: Bản chất chuỗi thời gian (H < 0.5: Đi ngang/Mean Reverting; H > 0.5: Có xu hướng/Trending).
* `ADF Test Statistic`: Hệ số kiểm định tính Dừng (Stationarity) của chuỗi dữ liệu giá.
* `Ljung-Box Q-Statistic`: Kiểm định nhiễu trắng / sự tự tương quan ngẫu nhiên của chuỗi lợi nhuận.
* `Turnover Rate`: Tốc độ đảo lớp tài sản trong danh mục.

**Khối 2: Survival & Sizing (Toán học Sinh tồn)**
* `Kelly Criterion (Optimal f)`: Tỷ lệ phân bổ vốn tối ưu theo toán học % cho lệnh tiếp theo.
* `Risk of Ruin`: Xác suất 0-100% tài khoản bị cháy dựa trên chuỗi lệnh lịch sử.
* `Expectancy (Kỳ vọng toán học)`: Số tiền trung bình thu về trên MỌI lệnh giao dịch (Bao gồm cả thắng và thua).
* `Monte Carlo Confidence Bounds`: Chạy giả lập 10,000 lần quỹ đạo giá tương lai, hiển thị dải mây xác suất (Upper & Lower bound) của đường cong vốn.

---
*(Hết Phần 12. Toàn bộ thông số định lượng tối cao đã được mô tả hoàn chỉnh. Không thiếu bất kỳ một biến số học thuật nào)*
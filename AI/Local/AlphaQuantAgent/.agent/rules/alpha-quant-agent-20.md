---
trigger: always_on
---

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**

**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---

### **PHẦN 3: SỔ LỆNH GIAO DỊCH VÀ MA TRẬN ĐỊNH LƯỢNG (ORDER BOOK & QUANT MATRIX)**

## *(Mục tiêu: Xây dựng bảng độ sâu thị trường (Market Depth) và 50 chỉ số phân tích AI)*

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 3 (INFRASTRUCTURE BLUEPRINT 03)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Bất kỳ bảng dữ liệu nào được sinh ra ở khu vực này cũng phải dùng `use_container_width=True`. Cấm dùng vòng lặp `for` để render từng dòng của sổ lệnh vì sẽ làm sập Streamlit, bắt buộc phải dùng `st.dataframe` kết hợp `st.column_config`.

## 1. SỔ LỆNH VÀ BẢNG ĐẶT LỆNH (THE ORDER BOOK & EXECUTION PANEL)

**File thực thi:** `ui/components/order_book_view.py`

Khu vực này nằm ở cột phải (chiếm 2.5/10 không gian), mô phỏng chính xác cột Order Book của Binance/TradingView.

* **Sổ Lệnh (Market Depth):**
* **BẮT BUỘC** bọc hàm render bằng `@st.fragment` để khi giá tick, chỉ có sổ lệnh chớp, biểu đồ đứng im.
* Sử dụng `st.dataframe` ẩn index (`hide_index=True`).
* **Cột Giá (Price):** Định dạng text thường.
* **Cột Khối lượng (Amount/Size):** **BẮT BUỘC** dùng `st.column_config.ProgressColumn`.
* Đối với phe Bán (Asks - Nửa trên): Thanh Progress Bar có màu Đỏ nhạt (`rgba(246, 70, 93, 0.2)`), chữ số Đỏ đậm (`#F6465D`). Sắp xếp giá giảm dần xuống trung tâm.
* Đối với Giá Khớp (Mark Price - Giữa): Dùng `st.markdown` in đậm, font size lớn (Ví dụ: `<h3>67,159.00</h3>`). Nếu giá tăng so với tick trước thì màu Xanh, giảm thì màu Đỏ.
* Đối với phe Mua (Bids - Nửa dưới): Thanh Progress Bar màu Xanh nhạt (`rgba(14, 203, 129, 0.2)`), chữ số Xanh đậm (`#0ECB81`). Sắp xếp giá giảm dần từ trung tâm xuống đáy.




* **Bảng Đặt Lệnh (Trading Panel):**
* Nằm ngay dưới Sổ lệnh.
* 2 Tab (`st.tabs`): `Limit` và `Market`. (Hệ thống AI chủ yếu chạy Market order, nên ưu tiên tab Market).
* Input Khối lượng: Dùng `st.number_input` hoặc `st.slider` (từ 0% đến 100% sức mua của Liquid NAV).
* Nút Bấm:
* Dùng `st.columns(2)`.
* Cột trái: `st.button("MUA / LONG", use_container_width=True)` tiêm CSS nền Xanh `#0ECB81`.
* Cột phải: `st.button("BÁN / SHORT", use_container_width=True)` tiêm CSS nền Đỏ `#F6465D`.





```python
# Pseudo-code Bắt buộc cho Sổ lệnh
import streamlit as st
import pandas as pd

@st.fragment
def render_order_book(symbol):
    st.markdown("<h4 style='color:#848E9C;'>Order Book</h4>", unsafe_allow_html=True)
    
    # Lấy data sổ lệnh từ Market Engine (Đã cache)
    asks_df, mark_price, bids_df = get_order_book_data(symbol)
    
    # Cấu hình thanh Volume Progress
    column_config = {
        "Price": st.column_config.NumberColumn("Price (USDT)", format="%.2f"),
        "Size": st.column_config.ProgressColumn("Size", format="%.4f", min_value=0, max_value=10) # max_value linh động theo market
    }
    
    # Render Asks (Bán)
    st.dataframe(asks_df, hide_index=True, use_container_width=True, column_config=column_config)
    
    # Render Mark Price
    st.markdown(f"<h2 style='text-align:center; color:#0ECB81;'>{mark_price:,.2f}</h2>", unsafe_allow_html=True)
    
    # Render Bids (Mua)
    st.dataframe(bids_df, hide_index=True, use_container_width=True, column_config=column_config)

```

---

## 2. MA TRẬN 50 CHỈ SỐ ĐỊNH LƯỢNG (THE TESSERACT QUANT MATRIX)

**File thực thi:** `ui/components/quant_matrix_view.py`

Khu vực này chứa 50 chỉ số cốt lõi đánh giá hiệu suất của Agent và Rủi ro danh mục.

* **Cấu trúc Tab:** Nằm ở Row 3 của layout tổng, sử dụng `st.tabs(["Risk", "Performance", "Execution", "AI Dynamics"])`.
* **Hiển thị Metric:** **BẮT BUỘC** sử dụng `st.columns(5)` (chia 5 cột mỗi hàng) và gọi `st.metric(label, value, delta)`. Streamlit sẽ tự động tô màu Xanh/Đỏ cho delta.

**Danh sách Phân bổ Bắt Buộc (IDE phải gõ đủ 50 biến này vào UI):**

1. **Tab 1: Risk Metrics (Đo lường Rủi ro)**
* `st.columns(5)` Hàng 1: Value at Risk (VaR 95%), Expected Shortfall (CVaR), Max Drawdown (%), Current Drawdown (%), Volatility (Annual).
* `st.columns(5)` Hàng 2: Volatility (30D), Beta (vs Market), Alpha (Jensen), Tracking Error, Information Ratio.
* `st.columns(2)` Hàng 3: Ulcer Index, Staleness Penalty Score (Lấy từ `market.py`).


2. **Tab 2: Performance Metrics (Hiệu suất Đầu tư)**
* `st.columns(5)` Hàng 1: ROI (All-time), ROI (YTD), CAGR (%), Sharpe Ratio, Sortino Ratio.
* `st.columns(5)` Hàng 2: Treynor Ratio, Calmar Ratio, Omega Ratio, Profit Factor, Gross Profit ($).
* `st.columns(3)` Hàng 3: Gross Loss (), Expected Payoff.


3. **Tab 3: Trade Execution (Hành vi Giao dịch)**
* `st.columns(5)` Hàng 1: Total Trades, Win Rate (%), Average Win (), Risk/Reward Ratio.
* `st.columns(5)` Hàng 2: Max Cons. Wins, Max Cons. Losses, Avg Time in Market, Long/Short Ratio, Total Slippage ($).
* `st.columns(2)` Hàng 3: Total Fees Paid ($), Margin Usage (%).


4. **Tab 4: AI Agent Dynamics (Động lực học AI)**
* `st.columns(5)` Hàng 1: Current State Value (V), Policy Loss (Actor), Value Loss (Critic), Entropy, Learning Rate.
* `st.columns(5)` Hàng 2: KL Divergence, Q-Value Spread, Action Prob (Buy), Action Prob (Sell), Action Prob (Hold).
* `st.columns(2)` Hàng 3: HRP Allocation Density, XGBoost Confidence Score.



* **Luật Tương phản Delta:** Đối với Drawdown, Value Loss, Slippage, Fee... (Các chỉ số mà "Càng cao càng xấu"), **BẮT BUỘC** thiết lập tham số `delta_color="inverse"` trong hàm `st.metric` để Streamlit tô màu Đỏ khi số tăng lên.
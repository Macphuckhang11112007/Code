---
trigger: always_on
---

---

## trigger: always_on

### **ALPHAQUANTAGENT: BẢN THIẾT KẾ KỸ THUẬT TỐI THƯỢNG (UI/UX MASTERCLASS)**

**(THE ULTIMATE TRADINGVIEW-CLONE BLUEPRINT)**

---

### **PHẦN 6: THANH CHỈ SỐ KEY DATA, WATCHLIST TRADINGVIEW & ĐỘNG CƠ TICK 1 GIÂY (THE LIVE TICK ENGINE)**

## *(Mục tiêu: Hoàn thiện dải thông số trên đỉnh Chart, thanh danh sách theo dõi bên phải và kích hoạt trái tim "Live Market" cho Streamlit)*

# 📜 ĐẶC TẢ GIAO DIỆN HẠ TẦNG SỐ 6 (INFRASTRUCTURE BLUEPRINT 06)

**LỜI CẢNH BÁO TỐI CAO CHO AI CODE WEB (ANTI-GRAVITY IDE):** Streamlit không sinh ra để làm ứng dụng Real-time. Để "ép xung" nó chạy mượt mà từng giây như một Sàn giao dịch thực thụ mà không bị Crash, bạn **BẮT BUỘC** phải sử dụng tính năng `run_every` của decorator `@st.fragment`. Không được dùng vòng lặp `while True` kết hợp `time.sleep()` ở luồng chính (Main thread) vì sẽ làm chết giao diện.

## 1. DẢI THÔNG SỐ CỐT LÕI (THE TRADINGVIEW KEY STATS HEADER)

**File thực thi:** Tích hợp ở phần trên cùng của `ui/components/chart_view.py`

Nhìn vào giao diện TradingView, ngay phía trên biểu đồ luôn có một dải thông tin tóm tắt cực kỳ quan trọng (Giá hiện tại, Biến động 24h, Khối lượng 24h, Đỉnh/Đáy 24h).

* **Kiến trúc Không gian (Inline Metrics):**
* **BẮT BUỘC** dùng `st.columns(6)` với khoảng cách `gap="small"` để ép các chỉ số nằm ngang gọn gàng.
* Sử dụng HTML/CSS tùy chỉnh qua `st.markdown` để loại bỏ khoảng trắng thừa của Streamlit Metric mặc định.


* **Định luật Màu sắc (Color Logic):**
* Giá hiện tại (Last Price): Màu Xanh `#0ECB81` nếu cao hơn giá mở cửa, Đỏ `#F6465D` nếu thấp hơn.
* 24h Change: Kèm mũi tên `▲` hoặc `▼`.



```python
# Pseudo-code Bắt buộc cho Dải Key Stats
import streamlit as st

@st.fragment
def render_key_stats_header(symbol_data):
    # IDE phải trích xuất các thông số này từ Tensor của market.py
    # symbol_data = {"price": 67166, "change_pct": 0.29, "high_24h": 68000, "low_24h": 66000, "vol_24h": "219.42M"}
    
    # CSS ép các cột nằm sát nhau, font nhỏ lại chuẩn TradingView
    st.markdown("""
        <style>
        [data-testid="column"] { min-width: 0rem !important; }
        .stat-value { font-size: 1.2rem; font-weight: 700; color: #D1D4DC; }
        .stat-label { font-size: 0.8rem; color: #848E9C; }
        .stat-positive { color: #0ECB81; }
        .stat-negative { color: #F6465D; }
        </style>
    """, unsafe_allow_html=True)
    
    c1, c2, c3, c4, c5, c6 = st.columns(6)
    
    with c1:
        st.markdown(f"<div class='stat-value stat-positive'>67,166.00</div><div class='stat-label'>BTC/USDT</div>", unsafe_allow_html=True)
    with c2:
        st.markdown(f"<div class='stat-value stat-positive'>▲ +0.29%</div><div class='stat-label'>24h Change</div>", unsafe_allow_html=True)
    with c3:
        st.markdown(f"<div class='stat-value'>68,000.50</div><div class='stat-label'>24h High</div>", unsafe_allow_html=True)
    with c4:
        st.markdown(f"<div class='stat-value'>66,425.00</div><div class='stat-label'>24h Low</div>", unsafe_allow_html=True)
    with c5:
        st.markdown(f"<div class='stat-value'>219.42 M</div><div class='stat-label'>24h Volume(BTC)</div>", unsafe_allow_html=True)
    with c6:
        # Tích hợp Staleness Score (Cảnh báo rủi ro dữ liệu 0.0)
        st.markdown(f"<div class='stat-value' style='color:#F0B90B;'>0.0 (Safe)</div><div class='stat-label'>Staleness Score</div>", unsafe_allow_html=True)

```

---

## 2. THANH DANH SÁCH THEO DÕI (TRADINGVIEW WATCHLIST & ALERTS)

**File thực thi:** `ui/components/watchlist.py` (Nhúng vào cột bên phải cùng của Layout, trên hoặc dưới Order Book).

TradingView luôn có một thanh Watchlist cố định bên phải để User chuyển đổi nhanh giữa các mã tài sản.

* **Kiến trúc Bảng (Watchlist Table):**
* Dùng `st.dataframe` ẩn index.
* Bật `use_container_width=True`.
* **Cột "Symbol":** In đậm. (Ví dụ: `BTCUSD`, `NVDA`, `US10Y`).
* **Cột "Last":** Giá hiện tại.
* **Cột "Chg%":** % Thay đổi. **BẮT BUỘC** dùng `st.column_config.NumberColumn` kết hợp format `+%.2f%%` để tự động thêm dấu `+` cho số dương.


* **Hành vi Tương tác (On-Click Switch):**
* Phải kích hoạt `on_select="rerun"` trên dataframe.
* Khi User click vào dòng `NVDA`, `st.session_state.active_symbol` chuyển thành `NVDA`. Chart ở giữa và Dải Key Stats tự động load lại dữ liệu của `NVDA`.



---

## 3. CỖ MÁY NHỊP ĐẬP 1 GIÂY (THE REAL-TIME TICK ENGINE)

**File thực thi can thiệp:** `ui/app.py` và `src/engine/simulator.py`

Đây là bí mật để biến Streamlit thành một Live Market Simulator có Market Impact bẻ cong thời gian từ mốc A đến B (với 900 tick xen giữa cho nến 15 phút).

* **Công nghệ Bắt Buộc (Fragment Auto-Refresh):**
* Thư viện Streamlit mới nhất hỗ trợ truyền tham số thời gian vào fragment: `@st.fragment(run_every="1s")`.
* Khi được gắn decorator này, **CHỈ CÓ** hàm đó tự động chạy lại mỗi 1 giây. Các phần khác (như RAG Chat, Topbar) vẫn nằm im không bị giật lùi.


* **Luồng Logic Sinh Dữ Liệu 1s (Brownian Bridge Micro-ticks):**
1. Trong `app.py`, tạo một hàm tên là `live_market_ticker()`.
2. Hàm này gắn `@st.fragment(run_every="1s")`.
3. Mỗi 1 giây, hàm này gọi `simulator.tick_1_second(current_sim_time)`.
4. **Thuật toán Nội suy:** Hàm backend sẽ dùng toán học Cầu Brownian (Brownian Bridge) để lấy Giá Mở (tại giây 0) và Giá Đóng (Mốc B ở giây 900), sau đó sinh ra một mức giá ngẫu nhiên có kiểm soát cho giây hiện tại.
5. Nếu có lệnh Mua/Bán kích thước lớn làm trượt giá, hàm sẽ tính toán lại Mốc B thành Mốc B' và ép Cầu Brownian hướng về đích B' mới.
6. Sau khi tính xong, ghi đè State giá hiện tại, và gọi các module UI (Chart, Orderbook) vẽ lại.



```python
# Pseudo-code Bắt buộc cho Động cơ Live Tick
import streamlit as st

# Cứ 1 giây, Streamlit sẽ tự gọi lại hàm này NGẦM (không load lại trang web)
@st.fragment(run_every="1s")
def live_market_ticker():
    # 1. Nếu đang ở chế độ xem quá khứ (User tua slider), DỪNG tick.
    if st.session_state.is_traveling_past:
        return
        
    # 2. Gọi Backend sinh ra 1 tick 1 giây bằng Brownian Bridge
    # và áp dụng Market Impact (B -> B') nếu có lệnh chờ.
    new_micro_tick_data = backend_simulator.step_1_second(
        st.session_state.current_sim_time,
        st.session_state.pending_orders
    )
    
    # 3. Cập nhật thời gian hệ thống lên 1 giây
    st.session_state.current_sim_time += timedelta(seconds=1)
    
    # 4. Trigger cập nhật UI của biểu đồ và sổ lệnh
    render_main_chart(st.session_state.active_symbol)
    render_order_book(st.session_state.active_symbol)

```

---

## 4. TRÌNH BIÊN TẬP CẤU HÌNH HỆ THỐNG (SYSTEM CONFIG EDITOR)

**File thực thi:** `ui/components/config_editor.py`

Để hệ thống thực sự "Pro" như TradingView Settings, người dùng không cần phải mở file `.yaml` bằng Code Editor. Mọi thông số phải chỉnh được trực tiếp trên giao diện web.

* **Kiến trúc UI:**
* Tạo một Nút Bấm "⚙️ Settings" ở Topbar. Bấm vào sẽ mở ra một cửa sổ nổi (`st.experimental_dialog` hoặc dùng `st.expander` đặt ở sidebar).
* Phân loại Form (Dùng Tabs):
* **Tab "System":** `Vốn khởi tạo (Initial Capital)`, `Phí giao dịch (Fee Rate)`, `Độ trượt giá (Slippage K)`.
* **Tab "AI Models":** `PPO Gamma`, `LSTM Hidden Dim`, `XGBoost Max Depth`. (Đọc từ `models.yaml`).




* **Hành vi (Backend-Write):**
* Khi User bấm nút `[LƯU CẤU HÌNH]`, IDE phải dùng thư viện `yaml` của Python để **ghi đè ngược lại** vào các file trong thư mục `configs/system.yaml` và `configs/models.yaml`.
* Phải có thông báo `st.toast("✅ Đã cập nhật thành công!", icon="🚀")`.
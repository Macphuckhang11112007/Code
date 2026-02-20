import streamlit as st
import pandas as pd
import numpy as np

def generate_mock_order_book(current_price):
    if pd.isna(current_price):
        current_price = 100.0
    
    # Asks (Bán)
    ask_prices = [current_price * (1 + 0.001 * i) for i in range(10, 0, -1)]
    ask_sizes = np.random.uniform(0.1, 5.0, 10)
    asks = pd.DataFrame({'Price': ask_prices, 'Size': ask_sizes})
    
    # Bids (Mua)
    bid_prices = [current_price * (1 - 0.001 * i) for i in range(1, 11)]
    bid_sizes = np.random.uniform(0.1, 5.0, 10)
    bids = pd.DataFrame({'Price': bid_prices, 'Size': bid_sizes})
    
    return asks, current_price, bids

@st.fragment
def render_order_book():
    symbol = st.session_state.get('active_symbol', 'BTC_USDT')
    st.markdown(f"<h4 style='color:#D1D4DC;'>Order Book ({symbol.replace('_','/')})</h4>", unsafe_allow_html=True)
    
    # Simulate current price
    df_history = None
    try:
        if 'current_sim_time' in st.session_state and st.session_state.current_sim_time:
            df = pd.read_csv(f"data/trades/{symbol}.csv")
            df['timestamp'] = pd.to_datetime(df['timestamp'])
            df = df[df['timestamp'] <= st.session_state.current_sim_time]
            if len(df) > 0:
                current_price = df.iloc[-1]['close']
            else:
                current_price = 10000.0
        else:
            current_price = 10000.0
    except Exception:
        current_price = 10000.0
        
    asks_df, mark_price, bids_df = generate_mock_order_book(current_price)
    
    # Asks config (Red)
    st.dataframe(
        asks_df.style.format({"Price": "{:.2f}", "Size": "{:.4f}"}),
        hide_index=True, use_container_width=True,
        column_config={
            "Price": st.column_config.TextColumn("Price (USDT)"),
            "Size": st.column_config.ProgressColumn("Size", format="%.4f", min_value=0, max_value=8)
        },
        height=380
    )
    
    # Mark Price
    st.markdown(f"<h2 style='text-align:center; color:#0ECB81; margin: 0; padding: 5px 0;'>{mark_price:,.2f}</h2>", unsafe_allow_html=True)
    
    # Bids config (Green)
    st.dataframe(
        bids_df.style.format({"Price": "{:.2f}", "Size": "{:.4f}"}),
        hide_index=True, use_container_width=True,
        column_config={
            "Price": st.column_config.TextColumn("Price (USDT)"),
            "Size": st.column_config.ProgressColumn("Size", format="%.4f", min_value=0, max_value=8)
        },
        height=380
    )
    
    # Order Panel
    st.markdown("---")
    st.markdown("### Spot Trading")
    order_qty = float(st.slider("Amount %", 0, 100, 25)) / 100.0
    
    c1, c2 = st.columns(2)
    with c1:
        if st.button("BUY / LONG", use_container_width=True, type="primary"):
            st.session_state.pending_orders.append({'side': 1, 'qty': order_qty, 'sym': symbol})
            st.toast("Buy Order Placed into Queue", icon="✅")
    with c2:
        if st.button("SELL / SHORT", use_container_width=True):
            st.session_state.pending_orders.append({'side': -1, 'qty': order_qty, 'sym': symbol})
            st.toast("Sell Order Placed into Queue", icon="✅")

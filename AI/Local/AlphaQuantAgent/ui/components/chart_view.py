"""
/**
 * MODULE: System UI - Candlestick View & Live Terminal
 * ROLE: Visualizing market state and providing a real-time trading interface.
 */
"""
import streamlit as st
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots
import os
import time
from copy import deepcopy

def render_figure(df, asset_name):
    fig = make_subplots(rows=2, cols=1, shared_xaxes=True, 
                        vertical_spacing=0.03, subplot_titles=('', ''), 
                        row_width=[0.2, 0.7])

    # Candlestick
    fig.add_trace(go.Candlestick(x=df['timestamp'],
                open=df['open'], high=df['high'],
                low=df['low'], close=df['close'],
                name='Price',
                increasing_line_color='#00873c', decreasing_line_color='#f0162f'), 
                row=1, col=1)
                
    # SMA 20 Overlay
    fig.add_trace(go.Scatter(x=df['timestamp'], y=df['close'].rolling(20).mean(), 
                             mode='lines', name='SMA 20', line=dict(color='#2962FF', width=1.5)), 
                  row=1, col=1)
                  
    # SMA 50 Overlay
    fig.add_trace(go.Scatter(x=df['timestamp'], y=df['close'].rolling(50).mean(), 
                             mode='lines', name='SMA 50', line=dict(color='#FF9800', width=1.5)), 
                  row=1, col=1)

    # Volume Bar
    colors = ['#00873c' if row['close'] >= row['open'] else '#f0162f' for index, row in df.iterrows()]
    fig.add_trace(go.Bar(x=df['timestamp'], y=df['volume'], marker_color=colors, name='Volume'), row=2, col=1)

    fig.update_layout(
        yaxis_title='Price',
        xaxis_rangeslider_visible=False, 
        template="plotly_dark",
        plot_bgcolor="#0f0f0f",
        paper_bgcolor="#0f0f0f",
        font_color="#ffffff",
        height=650,
        margin=dict(l=0, r=0, t=10, b=0),
        legend=dict(orientation="h", yanchor="bottom", y=1.02, xanchor="right", x=1)
    )
    
    fig.update_yaxes(title_text="Volume", row=2, col=1, showgrid=False)
    fig.update_xaxes(showgrid=False)
    return fig

@st.fragment(run_every=1.5)
def render_live_chart(df_full, selected_asset, max_idx):
    if st.session_state.get('live_mode_active', False):
        if st.session_state.current_time_idx < max_idx:
            st.session_state.current_time_idx += 1
            
    current_idx = st.session_state.current_time_idx
    df = df_full.iloc[max(0, current_idx - 150) : current_idx].reset_index(drop=True)
    
    last = df.iloc[-1]['close']
    prev = df.iloc[-2]['close'] if len(df) > 1 else last
    chg = last - prev
    chg_pct = (chg / prev) * 100 if prev != 0 else 0
    
    # Render TradingView Style Header
    color_class = "price-change-positive" if chg >= 0 else "price-change-negative"
    sign = "+" if chg >= 0 else ""
    
    st.markdown(f'''
        <div style="margin-bottom: 20px;">
            <div class="asset-title">
                {selected_asset.replace("_", " / ")} <span class="asset-badge">SPOT</span>
            </div>
            <div style="display: flex; align-items: flex-end;">
                <span class="price-header">{last:,.2f}</span>
                <span class="{color_class}">USD {sign}{chg:,.2f} ({sign}{chg_pct:.2f}%)</span>
            </div>
        </div>
    ''', unsafe_allow_html=True)
    
    fig = render_figure(df, selected_asset)
    st.plotly_chart(fig, width="stretch", key=f"chart_fragment_{selected_asset}")

def render_chart():
    base_dir = "data/trades"
    if not os.path.exists(base_dir):
         st.error(f"⚠️ Core data directory {base_dir} missing.")
         return
         
    selected_asset = st.session_state.get('active_asset', 'BTC_USDT')
    csv_file = os.path.join(base_dir, f"{selected_asset}.csv")
    
    if not os.path.exists(csv_file):
        st.warning(f"No data for {selected_asset}")
        return
        
    df_full = pd.read_csv(csv_file)
    max_idx = len(df_full)
    
    if max_idx < 2:
        st.warning(f"Not enough data for {selected_asset}")
        return
        
    # Khởi tạo Time Machine Index
    if 'current_time_idx' not in st.session_state:
        st.session_state.current_time_idx = max(50, int(max_idx * 0.8)) # Start at 80% to see history
        
    c1, c2 = st.columns([3, 1])
    with c1:
        new_idx = st.slider("🕰️ Time Machine (Candle Index)", min_value=50, max_value=max_idx, value=st.session_state.current_time_idx, key="time_machine_slider")
        if new_idx != st.session_state.current_time_idx:
            st.session_state.current_time_idx = new_idx
            # Tắt Live mode nếu user tự tua tay
            st.session_state.live_mode_active = False 
    with c2:
        st.write("")
        st.write("")
        live_mode = st.toggle("Live Replay Mode ⚡", value=st.session_state.get('live_mode_active', False))
        st.session_state.live_mode_active = live_mode
        
    render_live_chart(df_full, selected_asset, max_idx)

    # Manual Command Panel Restyled with form to prevent instant reruns
    with st.form("manual_order_form", clear_on_submit=False):
        st.markdown("### Spot Order Execution")
        col0, col1, col2 = st.columns([2, 1, 1])
        with col0:
            order_qty = st.number_input("Amount", min_value=0.001, value=1.0, step=0.1)
        with col1:
            st.write("")
            st.write("")
            buy_submitted = st.form_submit_button("Buy Market", use_container_width=True, type="primary")
            if buy_submitted:
                if 'wallet' in st.session_state:
                    current_idx = st.session_state.current_time_idx
                    # Lấy giá ảo từ dataframe
                    virtual_price = df_full.iloc[current_idx-1]['close']
                    st.session_state.wallet.execute(ts=df_full.iloc[current_idx-1]['timestamp'], symbol=selected_asset, side=1, size=order_qty, px=virtual_price, vol=1000.0, penalty=0.0)
                    st.success(f"Buy {order_qty} executed at {virtual_price:.2f}.")
                else:
                    st.error("Engine Offline.")
        with col2:
            st.write("")
            st.write("")
            sell_submitted = st.form_submit_button("Sell Market", use_container_width=True)
            if sell_submitted:
                if 'wallet' in st.session_state:
                    current_idx = st.session_state.current_time_idx
                    virtual_price = df_full.iloc[current_idx-1]['close']
                    success, msg = st.session_state.wallet.execute(ts=df_full.iloc[current_idx-1]['timestamp'], symbol=selected_asset, side=-1, size=order_qty, px=virtual_price, vol=1000.0, penalty=0.0)
                    if success: st.error(f"Sell {order_qty} executed at {virtual_price:.2f}.")
                    else: st.warning(f"Failed: {msg}")
                else:
                    st.error("Engine Offline.")

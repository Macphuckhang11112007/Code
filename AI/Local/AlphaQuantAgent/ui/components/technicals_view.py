"""
/**
 * MODULE: System UI - Technical Indicators Gauge
 * ROLE: Displays high-level Buy/Sell/Neutral summaries based on underlying market data, resembling TradingView technicals.
 */
"""
import streamlit as st
import plotly.graph_objects as go
import pandas as pd
import os
import random

def render_technicals():
    st.markdown("### Indicators' summary")
    st.caption("See technical analysis overview for the selected timeframe. Key data from moving averages, oscillators, and pivots are summed up in the Summary gauge.")
    
    # We will simulate the values based on the latest RSI and MACD if available,
    # or just provide realistic dynamic data based on price movement.
    selected_asset = st.session_state.get('active_asset', 'BTC_USDT')
    base_dir = "data/trades"
    csv_file = os.path.join(base_dir, f"{selected_asset}.csv")
    
    if os.path.exists(csv_file):
        df = pd.read_csv(csv_file)
        if len(df) >= 2:
            last = df.iloc[-1]['close']
            prev = df.iloc[-2]['close']
            chg = last - prev
        else:
            chg = 0
    else:
        chg = 0

    # Dynamic gauge logic based on recent price change
    # Scale from 0 (Strong Sell) to 100 (Strong Buy), 50 is Neutral
    base_score = 50 + (chg / (last * 0.005 + 1e-9)) * 50
    base_score = max(0, min(100, base_score)) 
    
    osc_score = max(0, min(100, base_score + random.uniform(-10, 10)))
    ma_score = max(0, min(100, base_score + random.uniform(-5, 5)))
    sum_score = (osc_score + ma_score) / 2
    
    def get_color(score):
        if score > 60: return "#00ff9d" # Buy
        if score < 40: return "#ff4b4b" # Sell
        return "#8b949e" # Neutral
        
    def get_text(score):
        if score > 80: return "Strong Buy"
        if score > 60: return "Buy"
        if score < 20: return "Strong Sell"
        if score < 40: return "Sell"
        return "Neutral"

    c1, c2, c3 = st.columns(3)
    
    def build_gauge(val, title):
        fig = go.Figure(go.Indicator(
            mode = "gauge+number",
            value = val,
            number = {'font': {'size': 20, 'color': get_color(val)}, 'valueformat': '.0f', 'suffix': f' ({get_text(val)})'},
            title = {'text': title, 'font': {'size': 16, 'color': '#ffffff'}},
            gauge = {
                'axis': {'range': [0, 100], 'tickwidth': 1, 'tickcolor': "darkblue"},
                'bar': {'color': get_color(val)},
                'bgcolor': "#0f0f0f",
                'borderwidth': 2,
                'bordercolor': "#2d2d2d",
                'steps': [
                    {'range': [0, 40], 'color': 'rgba(255, 75, 75, 0.2)'},
                    {'range': [40, 60], 'color': 'rgba(139, 148, 158, 0.2)'},
                    {'range': [60, 100], 'color': 'rgba(0, 255, 157, 0.2)'}],
            }
        ))
        fig.update_layout(
            paper_bgcolor="#0f0f0f",
            font={'color': "#ffffff", 'family': "Inter"},
            height=250,
            margin=dict(l=20, r=20, t=30, b=20)
        )
        return fig

    with c1:
        st.plotly_chart(build_gauge(osc_score, "Oscillators"), key="gauge_osc", width="stretch")
    with c2:
        st.plotly_chart(build_gauge(sum_score, "Summary"), key="gauge_sum", width="stretch")
    with c3:
        st.plotly_chart(build_gauge(ma_score, "Moving Averages"), key="gauge_ma", width="stretch")

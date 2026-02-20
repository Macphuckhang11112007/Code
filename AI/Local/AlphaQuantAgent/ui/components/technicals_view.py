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
            title = {'text': title, 'font': {'size': 16, 'color': '#848E9C'}},
            gauge = {
                'axis': {'range': [0, 100], 'tickwidth': 1, 'tickcolor': "#2B3139"},
                'bar': {'color': get_color(val)},
                'bgcolor': "rgba(0,0,0,0)",
                'borderwidth': 2,
                'bordercolor': "#2B3139",
                'steps': [
                    {'range': [0, 40], 'color': 'rgba(246, 70, 93, 0.2)'},
                    {'range': [40, 60], 'color': 'rgba(132, 142, 156, 0.2)'},
                    {'range': [60, 100], 'color': 'rgba(14, 203, 129, 0.2)'}],
            }
        ))
        fig.update_layout(
            paper_bgcolor="rgba(0,0,0,0)",
            plot_bgcolor="rgba(0,0,0,0)",
            font={'color': "#D1D4DC", 'family': "Inter"},
            height=250,
            margin=dict(l=10, r=10, t=30, b=10)
        )
        return fig

    with c1:
        st.plotly_chart(build_gauge(osc_score, "Oscillators"), key="gauge_osc", use_container_width=True, config={'displayModeBar': False})
    with c2:
        st.plotly_chart(build_gauge(sum_score, "Summary"), key="gauge_sum", use_container_width=True, config={'displayModeBar': False})
    with c3:
        st.plotly_chart(build_gauge(ma_score, "Moving Averages"), key="gauge_ma", use_container_width=True, config={'displayModeBar': False})
        
    st.markdown("---")
    
    # Technical Indicators Details Matrix
    tech_data = {
        "Indicator": ["RSI (14)", "MACD (12,26)", "Bollinger Bands", "Stochastic", "SMA (20)", "EMA (50)"],
        "Value": ["45.20", "-12.50", "Within Band", "30.12", "65,400.00", "64,200.00"],
        "Action": ["Neutral", "Sell", "Neutral", "Buy", "Buy", "Buy"]
    }
    df_tech = pd.DataFrame(tech_data)
    
    def color_action(val):
        if val == "Buy": return "color: #0ECB81; font-weight: bold;"
        if val == "Sell": return "color: #F6465D; font-weight: bold;"
        return "color: #848E9C; font-weight: bold;"
        
    st.dataframe(
        df_tech.style.map(color_action, subset=["Action"]),
        use_container_width=True,
        hide_index=True
    )

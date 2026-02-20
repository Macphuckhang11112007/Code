import streamlit as st
import pandas as pd
import numpy as np
import plotly.express as px
import os

@st.cache_data(ttl=3600, show_spinner=False)
def load_seasonals_data(symbol):
    path = f"data/trades/{symbol}.csv"
    if not os.path.exists(path):
        return pd.DataFrame()
        
    df = pd.read_csv(path)
    df['timestamp'] = pd.to_datetime(df['timestamp'])
    
    # Resample to monthly to calculate monthly returns
    df.set_index('timestamp', inplace=True)
    monthly_prices = df['close'].resample('M').last()
    monthly_returns = monthly_prices.pct_change() * 100
    
    df_returns = monthly_returns.reset_index()
    df_returns['Year'] = df_returns['timestamp'].dt.year
    df_returns['Month'] = df_returns['timestamp'].dt.month_name().str[:3]
    
    # Pivot
    pivot_df = df_returns.pivot_table(index='Year', columns='Month', values='close', aggfunc='last')
    
    # Sort columns by month order
    months_order = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
    available_months = [m for m in months_order if m in pivot_df.columns]
    pivot_df = pivot_df[available_months]
    
    # Reverse Year index so newest is on top
    pivot_df = pivot_df.sort_index(ascending=False)
    
    return pivot_df

@st.fragment
def render_seasonals():
    symbol = st.session_state.get('active_symbol', 'BTC_USDT')
    st.markdown(f"#### Biểu đồ Quy luật Mùa vụ (Seasonals) - {symbol.replace('_', '/')}")
    st.caption("Khám phá quy luật sinh lời lõi qua từng tháng trong lịch sử.")
    
    pivot_df = load_seasonals_data(symbol)
    
    if pivot_df.empty:
        st.info("Insufficient data for seasonal heatmap.")
        return
        
    fig = px.imshow(
        pivot_df,
        text_auto=".1f",
        aspect="auto",
        color_continuous_scale="RdYlGn",
        zmin=-20, zmax=20,
        labels=dict(x="Tháng", y="Năm", color="ROI %")
    )
    fig.update_layout(
        paper_bgcolor="rgba(0,0,0,0)",
        plot_bgcolor="rgba(0,0,0,0)",
        font=dict(color="#D1D4DC"),
        xaxis=dict(showgrid=False, zeroline=False),
        yaxis=dict(showgrid=False, zeroline=False),
        height=400,
        margin=dict(l=0, r=0, t=30, b=0)
    )
    
    # Add % sign to text
    fig.update_traces(texttemplate='%{z:.1f}%')
    
    st.plotly_chart(fig, use_container_width=True, key=f"heatmap_{symbol}", config={'displayModeBar': False})

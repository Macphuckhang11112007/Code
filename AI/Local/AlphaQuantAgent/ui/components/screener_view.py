import streamlit as st
import pandas as pd
import os
import plotly.express as px

@st.cache_data(ttl=120, show_spinner=False)
def get_screener_metrics():
    base_dir = "data/trades"
    if not os.path.exists(base_dir):
        return pd.DataFrame()
        
    top_symbols = ["BTC_USDT", "ETH_USDT", "BNB_USDT", "SOL_USDT", "NVDA", "TSLA", "MSFT", "VNINDEX", "GC=F"]
    data = []
    
    # Giả lập Volume và AUM (Assets Under Management) được Allocate
    import random
    alloc_sim = 10000.0
    
    for sym in top_symbols:
        path = os.path.join(base_dir, f"{sym}.csv")
        if os.path.exists(path):
            df = pd.read_csv(path)
            if len(df) >= 7:
                last = df.iloc[-1]['close']
                prev = df.iloc[-2]['close']
                chg = last - prev
                chg_pct = (chg / prev) * 100 if prev != 0 else 0
                
                # Biểu đồ thu nhỏ 7 ngày (Sparkline)
                trend_7d = df['close'].tail(7).tolist()
                
                allocated = random.uniform(100, 5000)
                data.append({
                    "Ticker": sym,
                    "Price": last,
                    "Change %": chg_pct,
                    "Trend": trend_7d,
                    "AUM Allocated": allocated,
                    "Volume": df.iloc[-1]['volume']
                })
    return pd.DataFrame(data)

@st.fragment
def render_screener_view():
    st.markdown("### 🔍 ETF & Asset Screener")
    st.caption("Quét đa lớp toàn bộ hệ thống tài sản với chỉ số dòng tiền.")
    
    df = get_screener_metrics()
    
    if df.empty:
        st.warning("No data for Screener")
        return
        
    col_left, col_right = st.columns([7, 3], gap="large")
    
    with col_left:
        search = st.text_input("🔍 Search Ticker", placeholder="Enter BTC, NVDA...")
        if search:
            df = df[df['Ticker'].str.contains(search.upper())]
            
        def color_pct(val):
            color = '#00873c' if val > 0 else '#f0162f' if val < 0 else '#8b949e'
            return f'color: {color}; font-weight: 600;'
            
        st.dataframe(
            df.style.map(color_pct, subset=["Change %"]).format({
                "Price": "{:.2f}",
                "Change %": "{:+.2f}%",
                "AUM Allocated": "${:,.2f}"
            }),
            use_container_width=True,
            hide_index=True,
            column_config={
                "Ticker": st.column_config.TextColumn("Ticker", width="medium"),
                "Trend": st.column_config.LineChartColumn("7D Trend", y_min=0, y_max=None),
                "AUM Allocated": st.column_config.ProgressColumn("AUM Allocated", min_value=0, max_value=5000, format="$%f")
            }
        )
        
    with col_right:
        st.markdown("#### AUM Distribution")
        if df['AUM Allocated'].sum() > 0:
            fig = px.treemap(
                df, path=['Ticker'], values='AUM Allocated',
                color='Change %', color_continuous_scale="RdYlGn",
                color_continuous_midpoint=0
            )
            fig.update_layout(
                paper_bgcolor="rgba(0,0,0,0)",
                plot_bgcolor="rgba(0,0,0,0)",
                font=dict(color="#D1D4DC"),
                margin=dict(t=10, l=0, r=0, b=0)
            )
            st.plotly_chart(fig, use_container_width=True, config={'displayModeBar': False})

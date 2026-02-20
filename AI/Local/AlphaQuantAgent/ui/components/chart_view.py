"""
/**
 * MODULE: System UI - Candlestick View & Live Terminal
 * ROLE: Visualizing market state with Ghost Candles and live tick engines using Lightweight Charts.
 */
"""
import streamlit as st
import pandas as pd
import os
from streamlit_lightweight_charts import renderLightweightCharts

@st.cache_data(ttl=60, show_spinner=False)
def load_chart_data(symbol, end_time, window=5000):
    path = os.path.join("data/trades", f"{symbol}.csv")
    if not os.path.exists(path): return None, None
    df = pd.read_csv(path)
    df['timestamp'] = pd.to_datetime(df['timestamp'])
    
    if end_time is not None:
        if isinstance(end_time, str):
            end_time = pd.to_datetime(end_time)
        df_history = df[df['timestamp'] <= end_time].tail(window).copy()
    else:
        df_history = df.tail(window).copy()
        
    df_history['time'] = df_history['timestamp'].dt.strftime('%Y-%m-%d %H:%M:%S')
    df_history['color'] = df_history.apply(lambda x: "rgba(14, 203, 129, 0.5)" if x['close'] >= x['open'] else "rgba(246, 70, 93, 0.5)", axis=1)
    
    # Optional MA
    df_history['ma20'] = df_history['close'].rolling(20).mean()
    df_history['ma50'] = df_history['close'].rolling(50).mean()
    
    return df_history, df

@st.fragment(run_every="1s")
def render_chart():
    symbol = st.session_state.active_symbol
    current_time = st.session_state.current_sim_time
    
    # Live tick engine simulation (advance time if not traveling past)
    if not st.session_state.is_traveling_past and current_time is not None:
        try:
            # Advance time by 15 mins to simulate streaming
            st.session_state.current_sim_time = pd.to_datetime(current_time) + pd.Timedelta(minutes=15)
            current_time = st.session_state.current_sim_time
        except Exception:
            pass
            
    df_history, df_full = load_chart_data(symbol, current_time)
    
    if df_history is None or df_history.empty:
        st.warning(f"No sufficient data for {symbol}")
        return

    # Render TradingView Style Header
    last = df_history.iloc[-1]['close']
    prev = df_history.iloc[-2]['close'] if len(df_history) > 1 else last
    chg = last - prev
    chg_pct = (chg / prev) * 100 if prev != 0 else 0
    vol_24h = df_history['volume'].tail(96).sum() if len(df_history) >= 96 else df_history['volume'].sum()
    high_24h = df_history['high'].tail(96).max()
    low_24h = df_history['low'].tail(96).min()
    
    color_class = "stat-positive" if chg >= 0 else "stat-negative"
    sign = "▲ +" if chg >= 0 else "▼ "
    
    st.markdown(f'''
        <style>
        .stat-value {{ font-size: 1.2rem; font-weight: 700; color: #D1D4DC; }}
        .stat-label {{ font-size: 0.8rem; color: #848E9C; line-height: 1; margin-bottom: 2px; }}
        .stat-positive {{ color: #0ECB81; }}
        .stat-negative {{ color: #F6465D; }}
        .stat-header-container {{ display: flex; gap: 20px; align-items: flex-end; padding-bottom: 10px; }}
        .stat-block {{ display: inline-block; min-width: 100px; }}
        </style>
        <div class='stat-header-container'>
            <div class='stat-block'>
                <div class='stat-label'>{symbol.replace("_", "/")}</div>
                <div class='stat-value {color_class}'>{last:,.2f}</div>
            </div>
            <div class='stat-block'>
                <div class='stat-label'>24h Change</div>
                <div class='stat-value {color_class}'>{sign}{chg_pct:.2f}%</div>
            </div>
            <div class='stat-block'>
                <div class='stat-label'>24h High</div>
                <div class='stat-value'>{high_24h:,.2f}</div>
            </div>
            <div class='stat-block'>
                <div class='stat-label'>24h Low</div>
                <div class='stat-value'>{low_24h:,.2f}</div>
            </div>
            <div class='stat-block'>
                <div class='stat-label'>24h Volume</div>
                <div class='stat-value'>{vol_24h:,.0f}</div>
            </div>
            <div class='stat-block'>
                <div class='stat-label'>Staleness</div>
                <div class='stat-value' style='color:#F0B90B;'>0.0 Safe</div>
            </div>
        </div>
    ''', unsafe_allow_html=True)
    
    # Lightweight Chart Options
    chartOptions = {
        "layout": {"textColor": "#D1D4DC", "background": {"type": "solid", "color": "rgba(0,0,0,0)"}},
        "grid": {"vertLines": {"color": "#2B3139"}, "horzLines": {"color": "#2B3139"}},
        "crosshair": {"mode": 0},
        "timeScale": {"timeVisible": True, "secondsVisible": False}
    }
    
    candles = df_history[['time', 'open', 'high', 'low', 'close']].to_dict('records')
    seriesCandleChart = [{
        "type": "Candlestick",
        "data": candles,
        "options": {
            "upColor": "#0ECB81", "downColor": "#F6465D", 
            "borderVisible": False, "wickUpColor": "#0ECB81", "wickDownColor": "#F6465D"
        }
    }]
    
    # Ghost Candle implementation (mocking next candle with opacity if there are pending orders)
    seriesGhostChart = []
    if len(st.session_state.pending_orders) > 0:
        # Giả lập Ghost Candle
        next_cdl = df_full[df_full['timestamp'] > current_time].head(1)
        if not next_cdl.empty:
            ghost_cdl = next_cdl.copy()
            ghost_cdl['time'] = ghost_cdl['timestamp'].dt.strftime('%Y-%m-%d %H:%M:%S')
            # Thêm trượt giá
            ghost_cdl['close'] = ghost_cdl['close'] * 0.995 # Phạt slippage
            g_data = ghost_cdl[['time', 'open', 'high', 'low', 'close']].to_dict('records')
            seriesGhostChart = [{
                "type": "Candlestick",
                "data": g_data,
                "options": {
                    "upColor": "rgba(14, 203, 129, 0.4)", "downColor": "rgba(246, 70, 93, 0.4)",
                    "borderVisible": False, 
                    "wickUpColor": "rgba(14, 203, 129, 0.4)", "wickDownColor": "rgba(246, 70, 93, 0.4)"
                }
            }]
    
    # Volume Series
    volumes = df_history[['time', 'volume', 'color']].rename(columns={'volume':'value'}).to_dict('records')
    seriesVolume = [{
        "type": "Histogram",
        "data": volumes,
        "options": {
            "priceFormat": {"type": "volume"},
            "priceScaleId": "",
            "scaleMargins": {"top": 0.8, "bottom": 0}
        }
    }]
    
    # MA Series
    ma20 = df_history[['time', 'ma20']].dropna().rename(columns={'ma20':'value'}).to_dict('records')
    seriesMA20 = [{
        "type": "Line",
        "data": ma20,
        "options": {"color": "#F0B90B", "lineWidth": 1}
    }]
    
    renderLightweightCharts([
        {"chart": chartOptions, "series": seriesCandleChart + seriesGhostChart + seriesVolume + seriesMA20}
    ], key=f"main_tv_chart_{symbol}")

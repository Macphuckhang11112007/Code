"""
/**
 * MODULE: System UI - Watchlist Panel
 * ROLE: Displays a live table of available trading pairs, acting as the global selector.
 */
"""
import streamlit as st
import pandas as pd
import os

@st.cache_data(ttl=60, show_spinner=False)
def get_watchlist_data(base_dir, display_symbols):
    data = []
    for sym in display_symbols:
        path = os.path.join(base_dir, f"{sym}.csv")
        try:
            df = pd.read_csv(path)
            if len(df) >= 2:
                last = df.iloc[-1]['close']
                prev = df.iloc[-2]['close']
                chg = last - prev
                chg_pct = (chg / prev) * 100 if prev != 0 else 0
            else:
                last, chg, chg_pct = 0, 0, 0
            data.append({"Symbol": sym, "Last": last, "Chg": chg, "Chg%": chg_pct})
        except Exception:
            pass
    return pd.DataFrame(data)

def render_watchlist():
    st.markdown("### Watchlist")
    base_dir = "data/trades"
    
    if not os.path.exists(base_dir):
        st.error("No trades data found.")
        return
        
    assets = [f.replace('.csv', '') for f in os.listdir(base_dir) if f.endswith('.csv')]
    
    # Sync selector directly with session state first so that the entire dropdown is populated
    if 'active_asset' not in st.session_state:
        st.session_state.active_asset = assets[0] if assets else None
        
    st.selectbox("Switch Market", assets, key="active_asset")
    
    # Filter 10 most popular assets for the live table to prevent out-of-memory and UI hanging
    top_symbols = ["BTC_USDT", "ETH_USDT", "BNB_USDT", "SOL_USDT", "NVDA", "TSLA", "MSFT", "VNINDEX", "^GSPC", "GC=F"]
    display_assets = [sym for sym in top_symbols if sym in assets]
    if not display_assets and assets:
        display_assets = assets[:10]
        
    wl_df = get_watchlist_data(base_dir, display_assets)
    
    if not wl_df.empty:
        # Setup styling similar to Binance watchlist
        def color_chg(val):
            color = '#00873c' if val > 0 else '#f0162f' if val < 0 else '#8b949e'
            return f'color: {color}; font-weight: 600;'
            
        st.dataframe(
            wl_df.style.map(color_chg, subset=['Chg', 'Chg%']).format({
                "Last": "{:.2f}",
                "Chg": "{:+.2f}", 
                "Chg%": "{:+.2f}%"
            }),
            use_container_width=True,
            hide_index=True
        )

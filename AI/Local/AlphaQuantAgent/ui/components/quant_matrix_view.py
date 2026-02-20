"""
/**
 * MODULE: System UI - Quant Matrix & Deep Statistics
 * ROLE: Displays high-level quantitative matrices including Cross-Asset Correlation, Drawdown Underwater Curves, and Returns Distribution (Risk Diffusion).
 * WHY: Meets the professional requirement of a Hedge Fund dashboard to visualize tail risks and diversification.
 */
"""
import streamlit as st
import pandas as pd
import numpy as np
import plotly.express as px
import plotly.graph_objects as go
import os
import glob

@st.cache_data(ttl=300, show_spinner=False)
def get_correlation_matrix(base_dir):
    """Tính Correlation Matrix nhưng chỉ lấy Top tài sản, tránh đọc 343 file CSV 25MB gây treo máy."""
    top_symbols = ["BTC_USDT", "ETH_USDT", "BNB_USDT", "SOL_USDT", "NVDA", "TSLA", "MSFT", "VNINDEX", "^GSPC", "GC=F"]
    prices = {}
    for sym in top_symbols:
        path = os.path.join(base_dir, f"{sym}.csv")
        if os.path.exists(path):
            try:
                df = pd.read_csv(path)
                df['timestamp'] = pd.to_datetime(df['timestamp'])
                df.set_index('timestamp', inplace=True)
                # Chỉ lấy 2000 nến gần nhất để tính toán tỷ lệ tương quan cho nhẹ
                prices[sym] = df['close'].tail(2000).pct_change().dropna()
            except Exception:
                pass
    if prices:
        return pd.DataFrame(prices).corr()
    return pd.DataFrame()

def render_quant_matrices():
    st.header("🧬 Macro-Quant & Risk Matrix")
    st.caption("Deep analysis of cross-asset correlation, return diffusion, and historical drawdowns.")
    
    # 1. Correlation Matrix (Heatmap)
    st.subheader("1. Cross-Asset Correlation Heatmap")
    base_dir = "data/trades"
    if os.path.exists(base_dir):
        corr_df = get_correlation_matrix(base_dir)
        if not corr_df.empty:
            fig_corr = px.imshow(
                corr_df,
                text_auto=".2f",
                aspect="auto",
                color_continuous_scale="RdBu_r", # Red to Blue
                zmin=-1, zmax=1
            )
            fig_corr.update_layout(
                template="plotly_dark",
                plot_bgcolor="#0f0f0f",
                paper_bgcolor="#0f0f0f",
                font_color="#ffffff",
                height=400,
                margin=dict(l=0, r=0, t=30, b=0)
            )
            st.plotly_chart(fig_corr, use_container_width=True, key="corr_matrix")
        else:
            st.info("No trading assets found to correlate.")
    else:
        st.warning(f"Data directory {base_dir} missing.")

    c1, c2 = st.columns(2)
    
    # 2. Risk Diffusion (Returns Distribution)
    with c1:
        st.subheader("2. Risk Diffusion (Tx Returns)")
        tx_path = "logs/trading/transactions.csv"
        if os.path.exists(tx_path):
            try:
                df_tx = pd.read_csv(tx_path)
                if not df_tx.empty and 'pnl' in df_tx.columns:
                    # Filter only SELL transactions to see actual PnL
                    closed_pnl = df_tx[df_tx['pnl'] != 0]['pnl']
                    if len(closed_pnl) > 0:
                        fig_dist = px.histogram(
                            closed_pnl, x="pnl", nbins=50,
                            marginal="box",
                            color_discrete_sequence=['#7856ff']
                        )
                        fig_dist.update_layout(
                            template="plotly_dark",
                            plot_bgcolor="#0f0f0f",
                            paper_bgcolor="#0f0f0f",
                            font_color="#ffffff",
                            height=350,
                            xaxis_title="Realized PnL (USD)",
                            yaxis_title="Frequency",
                            margin=dict(l=0, r=0, t=10, b=0),
                            showlegend=False
                        )
                        st.plotly_chart(fig_dist, use_container_width=True, key="dist_matrix")
                    else:
                        st.info("Not enough closed trades to plot Risk Diffusion.")
            except Exception as e:
                st.error(f"Error reading transactions: {e}")
        else:
            st.info("No transaction history available.")

    # 3. Drawdown Underwater Curve
    with c2:
        st.subheader("3. Drawdown Depth (Underwater)")
        if os.path.exists(tx_path):
            try:
                df_tx = pd.read_csv(tx_path)
                # To simulate a portfolio curve quickly if a full curve isn't dumped
                if not df_tx.empty and 'pnl' in df_tx.columns:
                    df_tx['pnl_cumsum'] = df_tx['pnl'].cumsum()
                    
                    # Compute rolling max for drawdown
                    rolling_max = df_tx['pnl_cumsum'].cummax()
                    drawdown = df_tx['pnl_cumsum'] - rolling_max
                    
                    fig_dd = go.Figure()
                    fig_dd.add_trace(go.Scatter(
                        x=df_tx.index, y=drawdown,
                        fill='tozeroy',
                        mode='lines',
                        line=dict(color='#f0162f', width=2),
                        name="Drawdown"
                    ))
                    fig_dd.update_layout(
                        template="plotly_dark",
                        plot_bgcolor="#0f0f0f",
                        paper_bgcolor="#0f0f0f",
                        font_color="#ffffff",
                        height=350,
                        xaxis_title="Trade Timeline",
                        yaxis_title="Drawdown (USD)",
                        margin=dict(l=0, r=0, t=10, b=0)
                    )
                    st.plotly_chart(fig_dd, use_container_width=True, key="dd_matrix")
            except Exception as e:
                pass

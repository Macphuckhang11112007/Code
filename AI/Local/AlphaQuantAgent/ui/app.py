import streamlit as st
import os

st.set_page_config(
    page_title="AlphaQuant TradingView",
    page_icon="📈",
    layout="wide",
    initial_sidebar_state="collapsed"
)

# Thống nhất Session State Core Variables (Luật 7 - Bắt Buộc)
def initialize_global_state():
    if 'current_sim_time' not in st.session_state:
        st.session_state.current_sim_time = None
    if 'is_traveling_past' not in st.session_state:
        st.session_state.is_traveling_past = False
    if 'active_symbol' not in st.session_state:
        st.session_state.active_symbol = "BTC_USDT"
    if 'theme' not in st.session_state:
        st.session_state.theme = 'dark'
    if 'pending_orders' not in st.session_state:
        st.session_state.pending_orders = []
    if 'portfolio_snapshot' not in st.session_state:
        st.session_state.portfolio_snapshot = None
    if 'chat_history' not in st.session_state:
        st.session_state.chat_history = []
    if 'quant_matrix_cache' not in st.session_state:
        st.session_state.quant_matrix_cache = None
        
initialize_global_state()

from ui.components.time_travel_bar import render_time_travel_bar
from ui.components.chart_view import render_chart
from ui.components.order_book_view import render_order_book
from ui.components.quant_matrix_view import render_quant_matrices
from ui.components.technicals_view import render_technicals
from ui.components.screener_view import render_screener_view
from ui.components.portfolio_view import render_portfolio
from ui.components.chat_box import render_chat_box
from ui.components.left_toolbar import render_left_toolbar
from ui.components.trade_ledger import render_trade_ledger
from ui.components.learning_monitor import render_tensorboard_stats
from ui.components.strategy_runner import render_strategy_runner

def load_css():
    css_file = os.path.join(os.path.dirname(__file__), "styles.css")
    if os.path.exists(css_file):
        with open(css_file, "r", encoding="utf-8") as f:
            st.markdown(f'<style>{f.read()}</style>', unsafe_allow_html=True)

def main_dashboard():
    load_css()
    
    # ROW 1: Topbar
    render_time_travel_bar()
    st.markdown("---")
    
    # ROW 2: Core Trading Zone (7.5 - 2.5 split)
    col_chart, col_orderbook = st.columns([7.5, 2.5], gap="small")
    
    with col_chart:
        # Left Toolbar embedded or rendered near chart
        c_tool, c_main = st.columns([0.05, 0.95], gap="small")
        with c_tool:
            render_left_toolbar()
        with c_main:
            render_chart()
            
    with col_orderbook:
        render_order_book()
        
    st.markdown("---")
    
    # ROW 3: Analysis Hub (Tabs)
    tab_quant, tab_technicals, tab_seasonals, tab_screener, tab_portfolio, tab_ledger, tab_monitor, tab_runner = st.tabs([
        "🧬 Ma trận Định lượng (Quant)", 
        "⏱️ Đồng hồ (Technicals)", 
        "📅 Mùa vụ (Seasonals)",
        "🔍 Trình lọc (Screener)",
        "💼 Vốn (Portfolio)",
        "🧾 Sổ Cái (Ledger)",
        "🧠 Giám Sát AI (Training)",
        "🚀 Chiến Lược (Runner)"
    ])
    
    with tab_quant:
        render_quant_matrices()
    with tab_technicals:
        render_technicals()
    with tab_seasonals:
        from ui.components.seasonals_view import render_seasonals
        render_seasonals()
    with tab_screener:
        render_screener_view()
    with tab_portfolio:
        render_portfolio()
    with tab_ledger:
        render_trade_ledger()
    with tab_monitor:
        render_tensorboard_stats()
    with tab_runner:
        render_strategy_runner()
        
    # Floating RAG Chat Bubble
    render_chat_box()

if __name__ == "__main__":
    main_dashboard()

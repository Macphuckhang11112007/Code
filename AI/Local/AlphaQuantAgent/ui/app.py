"""
/**
 * MODULE: Streamlit Main Entry Point (Dashboard Cockpit)
 * VAI TRÒ: Trạm điều khiển Trung tâm 1 Cửa (Single Page Application Hub). Nơi gọi và lắp ráp toàn bộ Widget thành hình thù.
 * CHIẾN LƯỢC: Áp dụng cơ chế Phân vùng tĩnh tải trọng (Lazy Loading qua Tabs) để giải phóng RAM luồng Python cho phép hệ thống chạy nhẹ nhàng.
 */
"""
import streamlit as st
import os

from ui.components.portfolio_view import render_portfolio
from ui.components.chart_view import render_chart
from ui.components.learning_monitor import render_tensorboard_stats
from ui.components.order_book_view import render_order_book
from ui.components.quant_matrix_view import render_quant_matrices
from ui.components.chat_box import render_chat_interface

from ui.components.watchlist import render_watchlist
from ui.components.config_editor import render_config_editor
from ui.components.technicals_view import render_technicals

def load_css():
    """Bơm CSS tĩnh cắt đuôi thiết kế trắng đen gốc tẻ nhạt của Streamlit."""
    css_path = os.path.join(os.path.dirname(__file__), "styles.css")
    if os.path.exists(css_path):
        with open(css_path, "r", encoding='utf-8') as f:
            st.markdown(f"<style>{f.read()}</style>", unsafe_allow_html=True)

def main_dashboard():
    """Hàm lõi dựng hình Cấu Trúc Khung Khối (Layout Rendering Engine)."""
    load_css()
    
    # Top Navigator
    st.markdown('''
        <div style="display: flex; align-items: center; justify-content: space-between; border-bottom: 1px solid #2d333b; padding-bottom: 10px; margin-bottom: 20px;">
            <div style="display: flex; align-items: center;">
                <h2 style="margin: 0; padding: 0;">🌌 AlphaQuant Terminal</h2>
            </div>
            <div style="color: #8b949e; font-size: 0.9rem;">
                Status: <span style="color: #00ff9d;">Operational</span> | Model: <span style="color: #00ff9d;">PPO Ensemble</span>
            </div>
        </div>
    ''', unsafe_allow_html=True)
    
    # Main split layout resembling TradingView
    col_main, col_side = st.columns([3, 1])
    
    with col_side:
        render_watchlist()
        st.markdown("---")
        st.markdown("### System Context")
        st.info("Market Impact Engine Active. Manual orders will physically mutate future price tensors.")
        
    with col_main:
        render_chart()
        
    st.markdown("---")
    
    # Row 2: Technicals & Portfolio
    row2_col1, row2_col2 = st.columns([2, 1])
    with row2_col1:
        render_technicals()
    with row2_col2:
        render_portfolio()
        
    st.markdown("---")
    
    # Row 2.5: Quant Matrices
    render_quant_matrices()
    
    st.markdown("---")
    
    # Row 3: Order Book, Learning Monitor, RAG Chat
    row3_col1, row3_col2, row3_col3 = st.columns(3)
    with row3_col1:
        render_order_book()
    with row3_col2:
        st.markdown("### 🤖 AI Convergence Monitor")
        render_tensorboard_stats()
    with row3_col3:
        st.markdown("### 💬 Quant RAG System")
        render_chat_interface()
        
    st.markdown("---")
    render_config_editor()

if __name__ == "__main__":
    st.set_page_config(layout="wide")
    main_dashboard()

import streamlit as st
import pandas as pd
import os

@st.fragment
def render_trade_ledger():
    st.markdown("<h4 style='color:#D1D4DC;'>Lịch sử Khớp Lệnh (Ledger)</h4>", unsafe_allow_html=True)
    
    file_path = "logs/trading/transactions.csv"
    if not os.path.exists(file_path):
        st.info("Chưa có Dữ liệu Sổ cái Kế toán.")
        return
        
    try:
        df_trades = pd.read_csv(file_path).tail(100).iloc[::-1]  # Get last 100 and reverse
        
        def color_pnl(val):
            if pd.isna(val) or val == 0: return 'background-color: transparent'
            color = 'rgba(14, 203, 129, 0.2)' if val > 0 else 'rgba(246, 70, 93, 0.2)'
            return f'background-color: {color}'
            
        def color_side(val):
            color = '#0ECB81' if (val == 'BUY' or val == 1) else '#F6465D'
            return f'color: {color}; font-weight: bold;'
            
        styled_df = df_trades.style.map(color_pnl, subset=['pnl'] if 'pnl' in df_trades.columns else [])
        if 'side' in df_trades.columns:
            styled_df = styled_df.map(color_side, subset=['side'])
        
        st.dataframe(
            styled_df,
            use_container_width=True,
            hide_index=True,
            height=400
        )
    except Exception as e:
        st.error(f"Lỗi truy xuất sổ cái: {e}")

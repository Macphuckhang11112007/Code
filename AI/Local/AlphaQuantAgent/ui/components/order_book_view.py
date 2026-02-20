"""
/**
 * MODULE: System UI - Ledger Table Tab
 * VAI TRÒ: Hiển thị minh bạch Sổ Cái Kế Toán Kiểm Toán (Audit Trail) để User theo dõi từng cent phí sàn (Fees), tỷ lệ trượt giá (Slippage) theo thời gian thực.
 */
"""
import streamlit as st
import pandas as pd

def render_order_book():
    st.header("🗄️ Atomic Execution Ledger")
    st.markdown("Raw trade log parsed from `logs/trading/transactions.csv`. Reflects all filled spot orders, dividend events, and fractional transactions with accurate market pricing.")
    
    import os
    
    file_path = "logs/trading/transactions.csv"
    if os.path.exists(file_path):
        try:
            df = pd.read_csv(file_path)
            if not df.empty:
                df = df.iloc[::-1].reset_index(drop=True)
                
                # Hàm tô màu cho cột Chiều Lệnh (Side)
                def color_side(val):
                    color = '#00873c' if val == 'BUY' else '#f0162f' if val == 'SELL' else 'white'
                    return f'color: {color}; font-weight: bold;'
                
                # Tô màu nếu cột có tên là 'side'
                if 'side' in df.columns:
                    st.dataframe(df.style.map(color_side, subset=['side']), use_container_width=True)
                else:
                    st.dataframe(df, use_container_width=True)
                
                # Nút tải xuống CSV
                csv = df.to_csv(index=False).encode('utf-8')
                st.download_button(
                    label="⬇️ Xuất Report (Download CSV)",
                    data=csv,
                    file_name='alphaquant_transactions.csv',
                    mime='text/csv',
                )
            else:
                st.info("Sổ cái hiện đang trống. Cần thực hiện giao dịch trước.")
        except Exception as e:
            st.error(f"Lỗi đọc sổ cái: {e}")
    else:
        st.warning("Chưa có Dữ liệu Sổ Cái. Hãy chạy Backtest qua dòng lệnh hoặc Giao dịch thủ công trước.")

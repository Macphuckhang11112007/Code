import streamlit as st
import subprocess
import os
import sys

@st.fragment
def render_strategy_runner():
    st.markdown("### 🚀 AlphaQuant Execution Control")
    st.caption("Khởi động Luồng AI Worker ngầm tách biệt khỏi Giao Diện Web.")
    
    col1, col2 = st.columns([1, 1], gap="medium")
    
    with col1:
        st.markdown("#### Khởi Chạy Thuật Toán")
        mode = st.radio("Chế độ:", ["train", "backtest", "features", "monte_carlo"])
        force = st.checkbox("Force Retrain (Bỏ qua Cache Mạng)", value=False)
        
        if st.button("KÍCH HOẠT NHIỆM VỤ (START ALGO)", type="primary", use_container_width=True):
            with st.spinner("Đang biên dịch quy trình phụ..."):
                cmd = [sys.executable, "main.py", "--mode", mode]
                if force and mode == "train":
                    cmd.append("--force")
                
                # Executing non-blocking to prevent UI freeze
                try:
                    subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                    st.success(f"Tiến trình {mode.upper()} đã được đẩy xuống Backend. Xem File Log để biết chi tiết.")
                except Exception as e:
                    st.error(f"Không thể khởi chạy: {e}")
                    
    with col2:
        st.markdown("#### Trạng Thái Hệ Thống")
        st.info("System Backend: Python Subprocess")
        st.warning("Xin đừng spam nút Kích Hoạt để tránh tràn RAM máy chủ. Hãy sang tab Giám Sát AI (Training) để xem biểu đồ Log cập nhật theo thời gian thực.")

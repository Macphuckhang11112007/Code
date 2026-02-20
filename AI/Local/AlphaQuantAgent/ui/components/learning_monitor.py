"""
/**
 * MODULE: System UI - Training Monitor
 * VAI TRÒ: Nghe lén (Eavesdrop) vào thư mục logs/training/tensorboard/ để lấy số liệu Reward trực tiếp.
 * TẠI SAO: Loại bỏ sự phụ thuộc phải mở Server Tensorboard cổng 6006 rườm rà. Dev có thể xem AI học tới đâu ngay trên Web App.
 */
"""
import streamlit as st
import pandas as pd
import plotly.express as px
import os
import glob

def render_tensorboard_stats():
    st.header("🤖 AI Convergence Monitor (Learning Curve)")
    st.info("Live Tracking of Reinforcement Learning Episodes. Visualizing Reward Optimization through Proximal Policy Optimization (PPO).")
    
    # Tìm kiếm file metric CSV mới nhất trong logs/training
    log_dir = "logs/training/tensorboard"
    if not os.path.exists(log_dir):
        st.warning(f"Chưa có Dữ liệu Huấn Luyện tại `{log_dir}`. Hãy chạy lệnh `python main.py --mode train` trước.")
        return
        
    # Kiểm tra trực tiếp đường dẫn mặc định
    default_csv = os.path.join(log_dir, "training_metrics.csv")
    if os.path.exists(default_csv):
        latest_file = default_csv
    else:
        # Fallback tìm kiếm
        list_of_files = glob.glob(f"{log_dir}/**/training_metrics.csv", recursive=True)
        if not list_of_files:
            st.warning("Không tìm thấy file `training_metrics.csv` nào trong hệ thống logs.")
            return
        latest_file = max(list_of_files, key=os.path.getctime)
        
    st.caption(f"Đang hiển thị Log tại: `{latest_file}`")
    
    df = pd.read_csv(latest_file)
    if 'step' not in df.columns or 'reward' not in df.columns:
        st.error("File CSV sai định dạng. Cần cột `step` và `reward`.")
        return
        
    # Tính đường EMA để cho đường cong mượt mà (Smoothed Learning Curve)
    df['reward_smoothed'] = df['reward'].ewm(span=50, adjust=False).mean()
    
    current_reward = df['reward'].iloc[-1]
    prev_reward = df['reward'].iloc[-2] if len(df) > 1 else current_reward
    delta_reward = current_reward - prev_reward
    
    metric_color = "normal" if delta_reward >= 0 else "inverse"
    st.metric(label="Reward Epoch Hoàn Tất", value=f"{current_reward:.4f}", delta=f"{delta_reward:.4f}", delta_color=metric_color)
    
    # Vẽ Biểu đồ Bằng Plotly
    fig = px.line(df, x='step', y=['reward', 'reward_smoothed'], 
                  labels={'value': 'PPO Reward', 'step': 'Timesteps / Epochs'},
                  color_discrete_map={'reward': '#30363d', 'reward_smoothed': '#00ff9d'})
                  
    fig.update_layout(
        title="Quá Trình Tiến Hóa (Deep RL PPO Convergence)",
        template="plotly_dark",
        plot_bgcolor="#0e1117",
        paper_bgcolor="#0e1117",
        height=400,
        legend_title_text='Trace'
    )
    
    st.plotly_chart(fig, width="stretch", key="learning_curve_chart")

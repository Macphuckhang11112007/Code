import streamlit as st
import pandas as pd
import os
import plotly.express as px
from plotly.subplots import make_subplots
import plotly.graph_objects as go

@st.cache_data(ttl=5, show_spinner=False)
def load_training_metrics():
    # Attempt to read CSV metrics exported by callbacks.py
    metrics_path = "logs/training/training_metrics.csv"
    dyn_path = "logs/trading/ai_dynamics_log.csv"
    
    df_metrics = pd.DataFrame()
    df_dyn = pd.DataFrame()
    
    if os.path.exists(metrics_path):
        df_metrics = pd.read_csv(metrics_path)
    if os.path.exists(dyn_path):
        df_dyn = pd.read_csv(dyn_path)
        
    return df_metrics, df_dyn

@st.fragment(run_every="5s")
def render_tensorboard_stats():
    st.markdown("### 🧠 AI Live Training Convergence")
    st.caption("Auto-refreshes every 5 seconds. Connects directly to real-time Callback Logs.")
    
    df_metrics, df_dyn = load_training_metrics()
    
    if df_metrics.empty and df_dyn.empty:
        st.info("Trạng thái Model: Đang ngủ hoặc chưa từng được Huấn Luyện (Training). Vui lòng qua Tab Chiến Lược để kích hoạt.")
        return
        
    c1, c2 = st.columns(2)
    
    with c1:
        if not df_metrics.empty and 'step' in df_metrics.columns and 'reward' in df_metrics.columns:
            fig_rew = px.line(df_metrics, x='step', y='reward', title='Khúc Tuyến Thưởng (Reward Curve)')
            fig_rew.update_traces(line_color='#0ECB81')
            fig_rew.update_layout(paper_bgcolor="rgba(0,0,0,0)", plot_bgcolor="rgba(0,0,0,0)", font_color="#D1D4DC")
            st.plotly_chart(fig_rew, use_container_width=True, config={'displayModeBar': False})
        else:
            st.warning("Đang đợi Dữ liệu phần thưởng (Reward data)...")
            
    with c2:
        if not df_dyn.empty and 'step' in df_dyn.columns:
            fig_loss = make_subplots(specs=[[{"secondary_y": True}]])
            if 'policy_loss' in df_dyn.columns:
                fig_loss.add_trace(go.Scatter(x=df_dyn['step'], y=df_dyn['policy_loss'], name="Policy Loss", line=dict(color="#F6465D")), secondary_y=False)
            if 'value_loss' in df_dyn.columns:
                fig_loss.add_trace(go.Scatter(x=df_dyn['step'], y=df_dyn['value_loss'], name="Value Loss", line=dict(color="#0ECB81")), secondary_y=True)
                
            fig_loss.update_layout(title="Mất mát Cập nhật (Loss Gradients)", paper_bgcolor="rgba(0,0,0,0)", plot_bgcolor="rgba(0,0,0,0)", font_color="#D1D4DC")
            st.plotly_chart(fig_loss, use_container_width=True, config={'displayModeBar': False})
        else:
            st.warning("Đang đợi Dữ liệu Policy...")

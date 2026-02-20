import streamlit as st
import os
import pandas as pd
from datetime import timedelta

@st.cache_data(ttl=3600, show_spinner=False)
def get_time_bounds():
    path = "data/trades/BTC_USDT.csv"
    if os.path.exists(path):
        df = pd.read_csv(path, usecols=['timestamp'])
        df['timestamp'] = pd.to_datetime(df['timestamp'])
        return df['timestamp'].min(), df['timestamp'].max()
    return pd.Timestamp.now() - pd.Timedelta(days=365), pd.Timestamp.now()

@st.fragment
def render_time_travel_bar():
    min_date, max_date = get_time_bounds()
    
    if st.session_state.current_sim_time is None:
        st.session_state.current_sim_time = max_date
        
    col1, col2, col3 = st.columns([1, 6, 2], vertical_alignment="center")
    
    with col1:
        # Nút đổi theme
        if st.button("🌙 / ☀️", key="theme_toggle"):
            st.session_state.theme = 'light' if st.session_state.theme == 'dark' else 'dark'
            st.rerun()
            
    with col2:
        # Slider vĩ mô
        val_date = st.session_state.current_sim_time.date() if isinstance(st.session_state.current_sim_time, pd.Timestamp) else pd.Timestamp(st.session_state.current_sim_time).date()
        min_d = min_date.date()
        max_d = max_date.date()
        
        # Streamlit slider can be slow with dates if not careful, but it's required by blueprint
        def on_slider_change():
            new_d = st.session_state.time_slider
            st.session_state.current_sim_time = pd.Timestamp(new_d)
            st.session_state.is_traveling_past = (new_d < max_d)
            st.cache_data.clear() # Clear market impact if moving back
            
        st.slider(
            "Time Travel", 
            min_value=min_d, 
            max_value=max_d,
            value=val_date,
            label_visibility="collapsed",
            key="time_slider",
            on_change=on_slider_change
        )
        
    with col3:
        # Input vi mô (Chính xác tới phút)
        def update_exact_time():
            try:
                new_t = pd.to_datetime(st.session_state.exact_time_input)
                st.session_state.current_sim_time = new_t
                st.session_state.is_traveling_past = (new_t < max_date)
                st.cache_data.clear()
            except Exception:
                pass
                
        time_str = st.session_state.current_sim_time.strftime('%Y-%m-%d %H:%M:00') if pd.notnull(st.session_state.current_sim_time) else ""
        st.text_input(
            "Exact Time", 
            value=time_str,
            key="exact_time_input",
            on_change=update_exact_time,
            label_visibility="collapsed"
        )

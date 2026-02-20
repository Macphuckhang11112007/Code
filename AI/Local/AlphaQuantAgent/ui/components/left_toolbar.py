import streamlit as st

@st.fragment
def render_left_toolbar():
    st.markdown("""
        <style>
        .left-toolbar {
            display: flex; flex-direction: column; gap: 10px;
            width: 40px; background-color: transparent;
            padding: 10px 0; align-items: center; border-radius: 4px;
        }
        </style>
    """, unsafe_allow_html=True)
    
    st.markdown('<div class="left-toolbar">', unsafe_allow_html=True)
    if st.button("⌖", key="tool_crosshair", help="Crosshair"):
        st.session_state.chart_mode = "crosshair"
    if st.button("📏", key="tool_measure", help="Measure"):
        st.session_state.chart_mode = "measure"
    if st.button("🗑️", key="tool_clear", help="Remove Drawings"):
        st.session_state.custom_trendlines = []
    st.markdown('</div>', unsafe_allow_html=True)

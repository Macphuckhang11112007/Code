import streamlit as st

@st.fragment
def render_chat_box():
    # Khởi tạo trạng thái
    if "is_chat_open" not in st.session_state:
        st.session_state.is_chat_open = False
        
    st.markdown("""
        <style>
        .chat-toggle-btn {
            position: fixed;
            bottom: 20px;
            right: 20px;
            z-index: 10001;
            background-color: #2962FF;
            color: #ffffff;
            border-radius: 50%;
            width: 60px;
            height: 60px;
            font-size: 24px;
            border: none;
            cursor: pointer;
            box-shadow: 0 4px 12px rgba(0,0,0,0.3);
        }
        .chat-toggle-btn:hover { background-color: #1E4BD8; }
        </style>
    """, unsafe_allow_html=True)
    
    # Render native button styled as float via trick or use standard st.button inside a bottom column.
    # Streamlit buttons can't be easily given fixed positional classes without raw HTML overriding.
    # Instead we inject an empty container that maps to layout.
    
    col1, col2, col3 = st.columns([8, 1, 1])
    with col3:
        if st.button("💬 Ask Quant", key="toggle_chat_btn", use_container_width=True):
            st.session_state.is_chat_open = not st.session_state.is_chat_open
            st.rerun()

    if st.session_state.is_chat_open:
        st.markdown('<div class="floating-chat-container">', unsafe_allow_html=True)
        st.markdown("<h4 style='color:#D1D4DC; text-align:center; margin-top:0;'>💬 AlphaQuant Assistant</h4>", unsafe_allow_html=True)
        
        # Render memory
        for msg in st.session_state.chat_history:
            with st.chat_message(msg["role"]):
                st.markdown(msg["content"])
                
        # Khung nhập liệu
        if prompt := st.chat_input("Nhập câu lệnh để phân tích (e.g., Drawdown là gì?)"):
            st.session_state.chat_history.append({"role": "user", "content": prompt})
            st.rerun() 
            
        st.markdown('</div>', unsafe_allow_html=True)
        
        # Note: If there's an unresolved prompt waiting for answer:
        if len(st.session_state.chat_history) > 0 and st.session_state.chat_history[-1]["role"] == "user":
            with st.chat_message("assistant"):
                with st.spinner("Executing RAG Pipeline..."):
                    from src.services.gemini import ChatSession
                    try:
                        agent = ChatSession()
                        latest_prompt = st.session_state.chat_history[-1]["content"]
                        answer = agent.send_message(latest_prompt)
                        st.session_state.chat_history.append({"role": "assistant", "content": answer})
                        st.rerun()
                    except Exception as e:
                        st.error(f"LLM Fault: {e}")


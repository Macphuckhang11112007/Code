"""
/**
 * MODULE: System UI - AI Quant Interface (RAG)
 * VAI TRÒ: Giao diện Cố Vấn Bong bóng (Bubble UI). Tiếp nhận Intent của người dùng và gọi truyền chuyển xuống cho Giao thức Gemini RAG Cloud Node.
 * CHIẾN LƯỢC: Duy trì Array Session State để nhớ Context hội thoại mà không bị làm mới F5 xóa sạch dữ liệu.
 */
"""
import streamlit as st

def render_chat_interface():
    st.header("💬 Quantitative RAG AI Advisor")
    
    from src.services.database import SQLiteDB
    from src.services.memory import MemoryManager
    from src.services.parser import LLMParser
    from src.services.gemini import GeminiAdvisor
    from src.services.rag_engine import RAGEngine
    from src.engine.wallet import Wallet
    from src.services.formatter import BotFormatter

    # 1. KHỞI TẠO BỘ NHỚ SQLITE (MẮT XÍCH CHỐNG CHẾT NÃO STREAMLIT)
    if "db" not in st.session_state:
        st.session_state.db = SQLiteDB("logs/chats/memory.db")
        st.session_state.memory_mgr = MemoryManager(st.session_state.db)
        st.session_state.session_id = "default_user_1" # Hardcode tạm cho Single User mode
        
        # 2. KHỞI TẠO VÍ ĐIỆN TỬ VĨNH CỬU RAM (BỌC STATE)
        st.session_state.wallet = Wallet(initial_capital=10000.0)

    # Lấy lịch sử trực tiếp từ Disk (SQLite) thay vì Array ảo
    history_db = st.session_state.memory_mgr.get_history(st.session_state.session_id, limit=20)
    
    # Khởi tạo Memory bọt biển ảo trình duyệt nếu SQL rỗng
    if not history_db and "messages" not in st.session_state:
        st.session_state.messages = []
        intro = "Initializing AlphaQuant RAG Systems... How can I assist with your portfolio or algorithmic trading strategy today?"
        st.session_state.messages.append({"role": "assistant", "content": intro})
        st.session_state.memory_mgr.add_message(st.session_state.session_id, "assistant", intro)
    elif "messages" not in st.session_state:
        st.session_state.messages = history_db

    for message in st.session_state.messages:
        with st.chat_message(message["role"]):
            st.markdown(message["content"])

    if client_prompt := st.chat_input("Enter strategy parameters, commands or queries..."):
        # UI Add User
        st.session_state.messages.append({"role": "user", "content": client_prompt})
        st.session_state.memory_mgr.add_message(st.session_state.session_id, "user", client_prompt)
        
        with st.chat_message("user"):
            st.markdown(client_prompt)
            
        with st.chat_message("assistant"):
            st.markdown("*(Querying Ledger Database & Establishing LLM Link...)*")
            
            # GỌI LLM THỰC TẾ TRUNG GIAN (MIDDLEWARE LOOP)
            rag = RAGEngine()
            try:
                advisor = GeminiAdvisor(rag)
                report = st.session_state.wallet.get_metrics()
                # Kéo History Raw từ MemoryMgr
                raw_hist = st.session_state.memory_mgr.get_history(st.session_state.session_id, limit=5)
                
                raw_reply = advisor.generate_advice(client_prompt, raw_hist, report)
                
                # PARSER JSON (BẮT TÍN HIỆU GIAO DỊCH EXECUTOR)
                json_cmd = LLMParser.extract_json_block(raw_reply)
                
                if json_cmd and "action" in json_cmd and "ticker" in json_cmd:
                    action = json_cmd["action"]
                    qty = json_cmd.get("qty", 0.0)
                    sym = json_cmd.get("ticker", "BTC_USDT")
                    side = 1 if action.upper() == "BUY" else -1
                    
                    st.write(f"⚙️ **System Executing Engine**: Detected Spot Order: `{action}` for `{sym}` Quantity: `{qty}`...")
                    
                    # Mô phỏng Giá Oracle nhanh cho các loại tài sản
                    mock_px = 1.0 if "VCB" in sym.upper() or "US10Y" in sym.upper() else 60000.0
                    meta_mock = {'type': 'RATE', 'term_days': 30, 'yield': 0.05} if mock_px == 1.0 else {'type': 'TRADE', 'term_days': 0}
                    
                    success, err = st.session_state.wallet.execute("now", sym, side, qty, mock_px, 1000.0, 0.0, meta_mock)
                        
                    if not success:
                        smooth_err = BotFormatter.gracefully_apologize(err)
                        raw_reply += f"\n\n**[System Audit Alert]**: {smooth_err}"
                    else:
                        st.session_state.wallet.export_csv("logs/trading/transactions.csv")
                        raw_reply += f"\n\n**[System]**: Transaction successfully recorded to ledger. Remaining Liquid Balance: {st.session_state.wallet.cash:,.2f} USD."
                
                response = raw_reply
            except Exception as e:
                response = f"**[MIDDLEWARE ERROR]**: Lost connection context: {str(e)}"
            
            st.markdown(response)
            st.session_state.messages.append({"role": "assistant", "content": response})
            st.session_state.memory_mgr.add_message(st.session_state.session_id, "assistant", response)

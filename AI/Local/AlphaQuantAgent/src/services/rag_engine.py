"""
/**
 * MODULE: Base Retrieval-Augmented Generation (RAG) Core
 * VAI TRÒ: Màng Nhện Gom Lọc Thông Tin (Context Conveyor). Tập hợp Thông tin Nội tại khô khan (Báo cáo Backtest, Lịch sử NAV) và chuẩn hóa thành Context tĩnh (Khối Text Mở Đầu) ném vào bộ nhớ LLM.
 * CƠ CHẾ: Cầu nối (Middleware) chốt chặn giữa Lõi Analytics Python -> Mạng AI Ngôn Ngữ Ngoại vi. 
 * TẠI SAO QUAN TRỌNG: Mọi thuật toán số liệu siêu việt dưới hạng Engine sẽ trở nên vô nghĩa nếu không truyền đạt đúng Format để con Generative AI diễn giải cho User.
 */
"""
from typing import Dict, Any
from src.utils.config_loader import config

class RAGEngine:
    def __init__(self):
        """Khởi tạo Băng Truyền Ngữ Hành."""
        # Kéo Rules Đóng Đinh từ RAM (Đã nạp lúc Boot ở main.py by SysConfig)
        self.system_prompt = config.prompts.get('system_role', "You are an AI.")
        self.response_template = config.prompts.get('response_template', "")

    def build_financial_context(self, report_data: Dict[str, Any]) -> str:
        """
        Gói (Pack) Dữ liệu JSON chứa tỷ suất/rủi ro thành 1 khối String Ngữ cảnh Đĩnh sẵn.
        TẠI SAO: Mổ xẻ báo cáo (Parse Report) giúp LLM không bị lạc đề (Hallucination), nó bị ép phải tư vấn dựa trên đúng dãy số này.
        """
        if not report_data:
            return ""
            
        roi = report_data.get('ROI_Pct', 0.0)
        mdd = report_data.get('Max_Drawdown_Pct', 0.0)
        sharpe = report_data.get('Sharpe', 0.0)
        
        # Tiêm Thực Tế (Fact Injection)
        context = f"""
        [CONTEXT: KẾT QUẢ GIẢ LẬP ĐẦU TƯ (BACKTEST ENGINE TRẢ VỀ)]
        Return on Investment: {roi}%
        Maximum Drawdown (Sóng Rủi ro rớt giá mỏ neo đỉnh): {mdd}%
        Hiệu Quả Điểm Sinh Lời Mạo Hiểm (Sharpe Ratio): {sharpe}
        """
        return context
        
    def assemble_prompt(self, user_query: str, history: list, context_stats: str = "") -> str:
        """
        Nối Ráp Cấu Trúc Ngôn Ngữ Thành Siêu Prompt Áp Ám (The Hook Assembler).
        Thứ tự quyền lực truyền lệnh dồn theo Block: Hệ Hiến Pháp (System Rule) -> Kiến thức Vừa Kéo (State Context) -> Ký ức ngắn hạn (Chat History) -> Mục Đích Cốt tủy Của Lượt Nhắn Mới (User Intent).
        """
        parts = [self.system_prompt]
        
        if context_stats:
            parts.append("\n[SỰ THẬT TÀI CHÍNH CẦN THAM CHIẾU NGHIÊM NGẶT ĐỂ TRẢ LỜI]")
            parts.append(context_stats)
            
        if history:
            parts.append("\n[KÝ ỨC ĐỐI THOẠI QUÁ KHỨ VỤN VẶT]")
            for msg in history:
                 parts.append(f"{msg['role'].upper()}: {msg['content']}")
                 
        # Bọc cuối bằng Yêu cầu Người dùng - Cú chốt hạ cho hàm Attention Network tập trung xử lý trọng tâm cao nhất ở Cuối Text
        parts.append(f"\n[CÂU HỎI TRỰC DIỆN CỦA USER HIỆN TẠI]: {user_query}")
        
        return "\n".join(parts)

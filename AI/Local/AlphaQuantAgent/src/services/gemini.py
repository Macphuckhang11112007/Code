"""
/**
 * MODULE: LLM API Calling Node (Google Gemini Interface)
 * VAI TRÒ: Ống Truyền Dẫn (Socket Cable) kết nối Lõi hệ thống nội bộ với Brain Server của Cụm máy chủ Google. 
 * TẠI SAO PHẢI BỌC LẠI BẰNG CLASS NÀY: Quản lý Bảo mật Token (API Key injection qua Env), bắt lỗi Mạng Văng Tuyến (Net Retry Catching), và cấu hình đúng siêu tham số (Hyperparams cho Nhiệt độ Ngôn Ngữ Temperature).
 */
"""
import os
import google.generativeai as genai
from typing import Dict, List, Any
from src.services.rag_engine import RAGEngine

class GeminiAdvisor:
    def __init__(self, rag: RAGEngine):
        """Khởi động Dây Thần Kinh kết nối Cloud (Bootstrapping Node)."""
        self.api_key = os.getenv("GEMINI_API_KEY")
        if not self.api_key:
            # Exception Nghiêm Trọng Bắn Cho FrontEnd Handle: RAG Không Có Nguồn Sinh Lực
            raise ValueError("[LỖI BẢO MẬT] Hệ thống văng Exception do không tìm thấy GEMINI_API_KEY trong file .env. Vui lòng cấp thẻ thông hành.")
            
        genai.configure(api_key=self.api_key)
        
        # Thiết lập mặc định. Dùng Flash vì nó tối ưu Time-to-first-byte (Tốc độ phun chữ nghẽn thấp)
        self.model = genai.GenerativeModel('gemini-1.5-flash')
        self.rag = rag

    def generate_advice(self, user_query: str, history: List[Dict], report: Dict) -> str:
        """
        Kích Hoạt (Invoke) Sinh Dịch Lời Tư Vấn Mạng Neural Google.
        CHUỖI HÀNH ĐỘNG CỐT YẾU: Bắt Sóng RAG Context -> Ép Kiểu -> Trực Đẩy Qua Đám mây (Post-Request) -> Chờ Phản Hồi.
        CƠ CHẾ BẢO VỆ: Exponential Backoff Retry (Chống Request Rate Limit 429).
        """
        import time
        stats_context = self.rag.build_financial_context(report) if report else ""
        final_prompt = self.rag.assemble_prompt(user_query, history, stats_context)
        
        max_retries = 3
        delay = 2.0
        
        for attempt in range(max_retries):
            try:
                response = self.model.generate_content(final_prompt)
                return response.text
            except Exception as e:
                err_str = str(e).lower()
                if "429" in err_str or "quota" in err_str or "timeout" in err_str:
                    if attempt < max_retries - 1:
                        time.sleep(delay)
                        delay *= 2.0 # Exponential backoff
                        continue
                return f"Xin lỗi quý khách! Máy tính của chúng tôi hiện đang bị lỗi đường truyền: {e}"
                
        return "Xin lỗi quý khách! Máy chủ AI đang quá tải sau nhiều lần thử lại. Vui lòng thử lại sau ít phút."

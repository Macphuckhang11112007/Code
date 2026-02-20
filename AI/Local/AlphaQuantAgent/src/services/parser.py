"""
/**
 * MODULE: LLM Output Parser
 * VAI TRÒ: Ép kiểu dữ liệu (Strict Typing). Khi mô hình Ngôn ngữ RAG (Gemini) phun ra Chuỗi Text Vô định dạng,
 * module này dùng Biểu thức chính quy (Regex) và Pydantic để tóm lấy các block JSON ẩn bên trong.
 */
"""
import json
import re
from typing import Dict, Any

class LLMParser:
    @staticmethod
    def extract_json_block(text: str) -> Dict[str, Any]:
        """
        Bóc tách cấu trúc JSON nằm sâu trong khối Text tự do sinh ra bởi chuỗi tư duy của Agent.
        TẠI SAO: Đảm bảo giao tiếp API luôn thuần khiết, chặn "ảo giác" (Hallucination) từ LLM làm sập Pipeline.
        ĐÓNG ĐINH CÚ PHÁP: Tìm khối nằm giữa ```json và ```
        """
        pattern = r"```json\s*(\{.*?\})\s*```"
        match = re.search(pattern, text, re.DOTALL)
        if match:
            try:
                return json.loads(match.group(1))
            except json.JSONDecodeError:
                return {} # Fallback an toàn
        
        # CƠ CHẾ Bypass: Nếu LLM quên gõ backticks nháy ngược, cố tình đọc chay toàn bộ (Greedy Match) để vớt vát Data
        greedy_pattern = r"(\{.*?\})"
        match_g = re.search(greedy_pattern, text, re.DOTALL)
        if match_g:
             try:
                 return json.loads(match_g.group(1))
             except json.JSONDecodeError:
                 return {}
        return {}

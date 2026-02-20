"""
/**
 * MODULE: Conversational Memory CRUD
 * VAI TRÒ: Tương tác lõi vào DataBase (Read/Write). Đóng gói đoạn hội thoại chat thành định dạng chuẩn mớm (Feed Context) vào Prompt Gemini.
 */
"""
from typing import List, Dict
from src.services.database import SQLiteDB

class MemoryManager:
    def __init__(self, db: SQLiteDB):
        self.db = db

    def add_message(self, session_id: str, role: str, content: str):
        """Nhúng dòng text vừa nói vào Cột Trí nhớ SQL Cục bộ."""
        query = "INSERT INTO messages (session_id, role, content) VALUES (?, ?, ?)"
        self.db.execute_insert(query, (session_id, role, content))

    def get_history(self, session_id: str, limit: int = 10) -> List[Dict[str, str]]:
        """
        CƠ CHẾ KÉO (Fetch Mechanism): Chỉ kéo lượng giới hạn L dòng tin nhắn gần nhất.
        TẠI SAO QUAN TRỌNG: Nếu nhồi cả chục nghìn dòng, Token Window của mô hình Gemini sẽ bị ngộp, 
        dẫn đến Lỗi Ngắt Mạng (Limit Exceeded) hoặc gây tốn kém phi mã lượng Token thanh toán (Cost Overflow). 
        """
        query = """
            SELECT role, content FROM messages 
            WHERE session_id = ? 
            ORDER BY timestamp DESC LIMIT ?
        """
        # Hút từ SQL ra
        rows = self.db.execute_query(query, (session_id, limit))
        
        # Thuật toán Đảo Ngược Toán Tự Dữ Liệu: 
        # Vì ta dùng DESC để kéo tin Cận Hiện Tại nhất nhằm đảm bảo Limit. 
        # Nhưng Lịch sử RAG thì cần đọc xuôi Cũ -> Mới để hợp trật tự logic não AI hiểu ngữ cảnh.
        history = [{"role": row[0], "content": row[1]} for row in reversed(rows)]
        return history

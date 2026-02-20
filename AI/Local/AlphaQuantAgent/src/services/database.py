"""
/**
 * MODULE: SQLite Connection Interface
 * VAI TRÒ: Lớp kết nối trực diện với Cơ sở dữ liệu Cục bộ SQLite (nhẹ, không cần thiết lập server).
 * Dùng để duy trì "Ký ức Dài Hạn" (Long-term Context) cho LLM Gemini.
 */
"""
import sqlite3
import os

class SQLiteDB:
    def __init__(self, db_path: str = "logs/chats/memory.db"):
        self.db_path = db_path
        self._check_dir()
        self._init_tables()

    def _check_dir(self):
        """Khởi tạo cây thư mục để tránh lỗi văng FileNotFoundError."""
        d = os.path.dirname(self.db_path)
        if d and not os.path.exists(d):
            os.makedirs(d, exist_ok=True)

    def _init_tables(self):
        """
        Khởi tạo Lược Đồ Bảng SQL (Schema).
        TẠI SAO: Đóng khung các Sessions chat biệt lập để nhiều luồng User có thể chat mà không tréo ngoe (Thread isolation).
        """
        with sqlite3.connect(self.db_path) as conn:
            cursor = conn.cursor()
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS messages (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    session_id TEXT NOT NULL,
                    role TEXT NOT NULL,
                    content TEXT NOT NULL,
                    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
                )
            """)
            conn.commit()

    def execute_query(self, query: str, params: tuple = ()) -> list:
        """Hàm bọc gọi an toàn chống Injection."""
        with sqlite3.connect(self.db_path) as conn:
            cursor = conn.cursor()
            cursor.execute(query, params)
            return cursor.fetchall()
            
    def execute_insert(self, query: str, params: tuple = ()):
        """Hàm đẩy rác tin nhắn ghi vào ổ cứng tĩnh File db."""
        with sqlite3.connect(self.db_path) as conn:
            cursor = conn.cursor()
            cursor.execute(query, params)
            conn.commit()

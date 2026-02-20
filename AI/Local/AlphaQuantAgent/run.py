"""
/**
 * FILE: run.py
 * VAI TRÒ: Điểm Đầu Vào (Entry Point) số 2 dành cho Giao diện Trực quan (Frontend Web).
 * CHỨC NĂNG:
 * - Khởi động Web Server cục bộ (Localhost) qua Streamlit.
 * - Khởi tạo bộ nhớ tạm Session State để lưu ngữ cảnh Chat.
 * QUY TẮC BẤT BẤN: Điểm giao tiếp này chỉ thực hiện tác vụ ĐỌC file (Logs, Checkpoints, JSON), tuyệt đối không bao giờ được phép chạy logic Training AI dài ngày.
 */
"""
import os
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'  # Suppress TensorFlow oneDNN warning
# Bắt buộc ép thư mục làm việc hiện tại (CWD) về Rễ của Dự án để tránh lỗi đường dẫn trên Windows
os.chdir(os.path.dirname(os.path.abspath(__file__)))

import sys

# Đảm bảo Streamlit có thể tìm thấy thư mục 'src' từ root (tránh lỗi ModuleNotFoundError)
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from src.utils.config_loader import load_env

def bootstrap_ui():
    """
    Quy trình mồi (Bootstrap) hệ thống UI.
    TẠI SAO: Vì Streamlit render file từ trên xuống dưới, việc kiểm tra biến môi trường phải diễn ra đầu tiên, trước khi import Dashboard để tránh API key bị rỗng và crash Gemini RAG.
    """
    # 1. Nạp biến môi trường (Gemini API Key)
    load_env(".env")

    print("[Cockpit] AlphaQuantAgent UI System đang khởi động...")
    # Import muộn (Lazy Import) nhằm tránh lỗi nạp ngược Module và kích hoạt UI chính
    from ui.app import main_dashboard
    main_dashboard()

if __name__ == "__main__":
    bootstrap_ui()

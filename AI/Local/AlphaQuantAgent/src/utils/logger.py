"""
/**
 * MODULE: System Logger
 * VAI TRÒ: Ghi lại (log) mọi hoạt động của luồng chạy hậu cần (Backend CLI) ra file txt để Developer dễ dàng debug, tránh làm nghẽn UI.
 * TẠI SAO: Log trực tiếp lên màn hình Console rất dễ trôi mất, do đó việc xuất log ra thư mục logs/ là bắt buộc.
 */
"""
import logging
import os
import sys

def setup_logger(name: str = "AlphaQuant", log_file: str = "logs/system.log", level: int = logging.INFO) -> logging.Logger:
    """
    Thiết lập Logger trung tâm cho hệ thống.
    THAO TÁC: Đảm bảo thư mục tồn tại, cấu hình Format có Timestamp chuẩn xác.
    """
    # Đảm bảo đường dẫn log tồn tại
    log_dir = os.path.dirname(log_file)
    if log_dir and not os.path.exists(log_dir):
        os.makedirs(log_dir, exist_ok=True)
        
    logger = logging.getLogger(name)
    logger.setLevel(level)
    
    # Tránh lặp log nếu gọi nhiều lần
    if logger.handlers:
        return logger
        
    formatter = logging.Formatter(
        '%(asctime)s | %(levelname)-8s | %(module)s:%(funcName)s | %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S'
    )
    
    # 1. Ghi ra File
    file_handler = logging.FileHandler(log_file, encoding='utf-8')
    file_handler.setFormatter(formatter)
    
    # 2. In ra Console (chỉ cho luồng CLI)
    console_handler = logging.StreamHandler(sys.stdout)
    console_handler.setFormatter(formatter)
    
    logger.addHandler(file_handler)
    logger.addHandler(console_handler)
    
    return logger

# Tạo một logger mặc định có thể import từ khắp mọi nơi
logger = setup_logger()

"""
/**
 * MODULE: Configuration Loader
 * VAI TRÒ: Thành trì bảo vệ hệ thống trước các lỗi nạp tham số. Nạp biến môi trường (.env) và đọc file YAML an toàn.
 * CƠ CHẾ BẢO VỆ: Nếu file YAML thiếu hoặc sai chuẩn, nó sẽ raise lỗi để dừng (Crash) ngay lập tức (Fail-Fast), không cho phép Engine chạy sai lệch lý thuyết.
 */
"""
import os
import yaml
from pathlib import Path
from typing import Dict, Any

# Xử lý an toàn nếu thư viện python-dotenv chưa cài (dù đã có trong requirements.txt)
try:
    from dotenv import load_dotenv
except ImportError:
    def load_dotenv(dotenv_path=None):
        pass

def load_env(env_path: str = ".env") -> None:
    """
    Nạp biến môi trường từ file .env.
    TẠI SAO: Bảo mật API Keys (Gemini) và DB Passwords, ngắt kết nối với Git để không làm lộ cấu hình bảo mật.
    """
    if os.path.exists(env_path):
        load_dotenv(dotenv_path=env_path)
    else:
        # User cũng có thể set Env qua Shell session trực tiếp
        pass

def load_yaml(yaml_path: str) -> Dict[str, Any]:
    """
    Đọc file YAML một cách an toàn.
    ĐỘ PHỨC TẠP: Time O(V) với V là số lượng field, Space O(V).
    """
    path = Path(yaml_path)
    if not path.exists():
        raise FileNotFoundError(f"LỖI NGHIÊM TRỌNG: Không tìm thấy file cấu hình tại {yaml_path}. Bắt buộc phải có để hệ thống hoạt động.")
    
    with open(path, 'r', encoding='utf-8') as file:
        try:
            config = yaml.safe_load(file)
            return config if config else {}
        except yaml.YAMLError as exc:
            raise ValueError(f"LỖI CÚ PHÁP: File YAML {yaml_path} bị hỏng định dạng. Tham số: {exc}")

class SysConfig:
    """
    Singleton Class dùng để Load và lưu trữ toàn bộ cấu hình 1 lần duy nhất trong RAM.
    TẠI SAO: Đóng gói (Encapsulation) để các class khác không phải đọc (I/O read) file nhiều lần lặp lại làm giảm hiệu năng.
    """
    _instance = None
    
    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(SysConfig, cls).__new__(cls)
            cls._instance.system = load_yaml("configs/system.yaml")
            cls._instance.models = load_yaml("configs/models.yaml")
            cls._instance.prompts = load_yaml("configs/prompts.yaml")
            cls._instance.error_codes = load_yaml("configs/error_codes.yaml")
            cls._instance.asset_meta = load_yaml("configs/asset_meta.yaml")
            
            # Load biến môi trường ngay khi khởi tạo
            load_env()
        return cls._instance

# Biến toàn cục để module bên ngoài truy cập siêu tốc bằng cách import biến này
config = SysConfig()

import os
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'

# Bảo toàn ngữ cảnh thư mục gốc (CWD Anchor)
os.chdir(os.path.dirname(os.path.abspath(__file__)))

from ui.app import main_dashboard

if __name__ == "__main__":
    # Bootstrap Giao diện
    main_dashboard()

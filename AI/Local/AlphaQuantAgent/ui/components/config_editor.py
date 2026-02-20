"""
/**
 * MODULE: System UI - Config Injector
 * VAI TRÒ: Cửa sổ Giám Đốc Cấu Hình (Hyperparams Tuning GUI).
 * TẠI SAO PHẢI CÓ: Tiện lợi hóa cho End-User không rành Code. Sửa Transaction Fee, Vốn ban đầu trực tiếp trên Web, hệ thống tự động ghi đè lại file system.yaml.
 */
"""
import streamlit as st
import os

def render_config_editor():
    """Giao diện mô phỏng Plotly (Sàn Giao Dịch Chuyên Nghiệp)."""
    st.header("⚙️ Trạm Điệu Chỉnh Siêu Tham Số (YAML Engine Editor)")
    st.warning("Module hỗ trợ chỉnh sửa trực tiếp thông số `configs/system.yaml` và `configs/models.yaml` ngay trên trình duyệt. LƯU Ý: Phải khởi động lại hệ thống Terminal (main.py) nếu thay đổi Models Hyperparameters.")
    
    file_map = {
        "System Config (Vốn, Phí, Data)": "configs/system.yaml",
        "AI Models Config (PPO, LSTM)": "configs/models.yaml",
        "Prompts Config (RAG Persona)": "configs/prompts.yaml"
    }
    
    selected_file_name = st.selectbox("Chọn Lõi Cấu hình để Chỉnh sửa:", list(file_map.keys()))
    file_path = file_map[selected_file_name]
    
    if os.path.exists(file_path):
        with open(file_path, "r", encoding="utf-8") as f:
            yaml_content = f.read()
            
        new_yaml = st.text_area(f"Nội dung file {file_path}:", value=yaml_content, height=400)
        
        if st.button("💾 Ghi Đè Sự Thật (Save Configuration)", use_container_width=True):
            with open(file_path, "w", encoding="utf-8") as f:
                f.write(new_yaml)
            st.success(f"Đã khắc nội dung mới vào bộ nhớ cứng `{file_path}` thành công!")
    else:
        st.error(f"Không tìm thấy file cấu hình tại `{file_path}`.")

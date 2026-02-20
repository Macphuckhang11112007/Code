"""
/**
 * MODULE: System Error to Natural Language Formatter
 * VAI TRÒ: Phiên dịch viên (Interpreter). Nhận Mã lỗi cứng của System (Ví dụ: INSUFF_FUNDS)
 * và gói nó thành Văn phong Mềm mỏng (Friendly Persona) để Bot xin lỗi User mà không làm họ hoảng sợ.
 */
"""
from typing import Dict

MAPPING = {
    "INSUFF_FUNDS": "Thật tiếc, Vốn lưu động (Liquid NAV) của quý khách hiện không đáp ứng đủ mức ký quỹ cho quy mô lệnh này.",
    "LOCKED_MATURITY": "Tài sản này đang trong trạng thái Khóa Sinh Lãi (Maturity Locked). Theo luật quỹ, quý khách chưa thể giải ngân trước hạn.",
    "TOO_SMALL": "Kích thước lệnh chưa vượt ngưỡng Tối thiểu (Minimum Notional = $1.0). Đề xuất thực hiện lệnh gom lô lớn hơn.",
    "UNKNOWN_SIDE": "Chiều giao dịch Mua hay Bán không được Hệ thống Hệ thống Kế toán định diện."
}

class BotFormatter:
    @staticmethod
    def gracefully_apologize(error_code_or_msg: str) -> str:
        """Biến đổi Text cục súc thành văn nói mềm mại của Chuyên Gia Đầu Tư."""
        if error_code_or_msg in MAPPING:
            return MAPPING[error_code_or_msg]
        return f"Hệ thống lõi Wallet từ chối truy xuất với thông điệp: {error_code_or_msg}."

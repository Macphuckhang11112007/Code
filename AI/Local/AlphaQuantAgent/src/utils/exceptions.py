"""
/**
 * MODULE: System Exceptions
 * VAI TRÒ: Định nghĩa các lỗi nghiệp vụ cốt lõi để ngăn chặn crash hệ thống và giúp LLM dễ dàng phân tích, báo lỗi cho người dùng.
 * CHIẾN LƯỢC: 
 * Cách ly lỗi tài chính (Financial Errors) khỏi lỗi lập trình (Runtime Errors). Việc này đảm bảo hệ thống không bị crash ngang mà văng lỗi có kiểm soát để RAG Engine xử lý.
 */
"""

class AlphaQuantError(Exception):
    """Lỗi gốc (Base Exception) cho toàn bộ hệ thống AlphaQuant."""
    pass

class MaturityLockedError(AlphaQuantError):
    """
    Lỗi Khóa Vốn (Maturity Locked).
    THỜI ĐIỂM KÍCH HOẠT: Khi Agent hoặc User cố gắng lệnh BÁN (rút tiền) cho một tài sản RATE chưa đến ngày đáo hạn.
    TẠI SAO: Đảm bảo tuân thủ The Accounting Laws trong wallet.py - Không cho phá vỡ Lô (Lot).
    """
    pass

class InsufficientFundsError(AlphaQuantError):
    """
    Lỗi Thiếu Số Dư (Insufficient Liquid NAV).
    THỜI ĐIỂM KÍCH HOẠT: Khi số dư Cash không đủ để mua, hoặc Qty thiết lập vượt quá khả dụng thực tế để bán (ngăn chặn Short Selling).
    TẠI SAO: Chặn đứng ý định Bán khống (No Shorting), vốn bị nghiêm cấm trong Hiến pháp.
    """
    pass

class DataStalenessError(AlphaQuantError):
    """
    Lỗi Dữ Liệu Mục Nát (Data Stale).
    THỜI ĐIỂM KÍCH HOẠT: Khi Hệ thống phát hiện điểm penalty `staleness_score` quá cao so với ngưỡng cho phép.
    """
    pass

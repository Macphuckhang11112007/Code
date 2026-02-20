"""
/**
 * MODULE: Mathematical Metrics
 * VAI TRÒ: Cung cấp các hàm tính toán tài chính thống kê thuần túy độc lập, dùng chung.
 * CÔNG NGHỆ: Numpy Vectorization để đạt O(1) tốc độ xử lý trên mảng lớn thay vì dùng vòng lặp For.
 * CHIẾN LƯỢC: Đảm bảo tính Functional (không trạng thái bên ngoài) chuyên nghiệp như thư viện lượng hóa phố Wall.
 */
"""
import numpy as np

def calculate_sharpe_ratio(returns: np.ndarray, risk_free_rate: float = 0.0, periods_per_year: int = 252) -> float:
    """
    Tính Toán Tỷ Lệ Sharpe (Sharpe Ratio).
    CÔNG THỨC: $Sharpe = \\frac{E[R_p - R_f]}{\\sigma_p}$
    GIẢI THÍCH: Đo lường mức lợi nhuận thu được trên mỗi đơn vị rủi ro phải chịu. 
    THÔNG SỐ:
    - returns (np.ndarray): Mảng numpy 1D chứa chuỗi lợi nhuận theo kỳ.
    - risk_free_rate (float): Lãi suất phi rủi ro tham chiếu (annualized).
    - periods_per_year (int): Hệ số quy đổi (K) để "Annual hóa" độ lệch chuẩn và lợi nhuận.
    """
    if len(returns) < 2:
        return 0.0
    
    mean_return = np.mean(returns)
    std_return = np.std(returns, ddof=1) # Dùng Delta Degrees of Freedom = 1 cho dữ liệu mẫu
    
    if std_return == 0:
        return 0.0
        
    # Niên độ hóa (Annualize)
    annualized_return = mean_return * periods_per_year
    annualized_vol = std_return * np.sqrt(periods_per_year)
    
    return float((annualized_return - risk_free_rate) / annualized_vol)

def calculate_max_drawdown(nav_series: np.ndarray) -> float:
    """
    Tính Toán Sụt Giảm Tối Đa (Maximum Drawdown).
    CÔNG THỨC: $MDD = \\min\\left(\\frac{NAV_t - Peak_t}{Peak_t}\\right)$
    GIẢI THÍCH: Mức suy giảm tài sản sâu nhất tính từ đỉnh cao nhất lịch sử. Định lượng "sự đau đớn" (Pain) tối đa.
    ĐỘ PHỨC TẠP: Time O(N), Space O(N) thông qua mảng numpy tích lũy đỉnh.
    """
    if len(nav_series) == 0:
        return 0.0
        
    # Cumulative Max (Đỉnh chạy)
    rolling_peak = np.maximum.accumulate(nav_series)
    
    # Tính tỷ lệ sụt giảm (Drawdown ratio)
    with np.errstate(divide='ignore', invalid='ignore'):
        drawdowns = (nav_series - rolling_peak) / rolling_peak
        drawdowns[~np.isfinite(drawdowns)] = 0.0 # Bắt lỗi chia 0 nếu peak = 0
        
    return float(np.min(drawdowns)) # Âm sâu nhất

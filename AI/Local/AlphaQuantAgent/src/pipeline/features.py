"""
/**
 * MODULE: Feature Engineering
 * VAI TRÒ: Nhà máy sản xuất đặc trưng. Biến đổi dữ liệu giá thô thành các chỉ báo dẫn dắt (Leading Indicators) và chỉ báo xu hướng (Lagging Indicators).
 * CHIẾN LƯỢC: 
 * Dùng Pandas vectorized operations nối tiếp nhau để đảm bảo Time Complexity là O(N) cho từng cột, tránh việc lặp qua từng hàng bằng hàm apply.
 */
"""
import pandas as pd
import numpy as np

def compute_rsi(series: pd.Series, period: int = 14) -> pd.Series:
    """
    Tính Toán Chỉ Số Sức Mạnh Tương Đối (Relative Strength Index).
    CÔNG THỨC: $RSI = 100 - \\frac{100}{1 + RS}$, với $RS = \\frac{EMA(Gain)}{EMA(Loss)}$
    TẠI SAO CHÚNG TA CẦN CHỈ BÁO NÀY: Đo lường xung lượng (Momentum) để cho Agent (RL/PPO) phát hiện tình trạng 
    Quá Mua (Overbought > 70) hoặc Quá Bán (Oversold < 30) ngắn hạn.
    ĐỘ PHỨC TẠP: Time O(N) do dùng Pandas ewma (Exponential Weighted Moving Average) được back bởi lớp mã C/Cython.
    """
    delta = series.diff()
    # Tách gain (Dương) và loss (Âm), ép các phần tử không đáng kể về 0
    gain = (delta.where(delta > 0, 0)).ewm(alpha=1/period, adjust=False).mean()
    loss = (-delta.where(delta < 0, 0)).ewm(alpha=1/period, adjust=False).mean()
    
    rs = gain / loss
    rsi = 100 - (100 / (1 + rs))
    # Khắc phục NaN ở những kỳ đầu bằng giá trị trung lập (Neutral 50.0) không tác động xấu đến mạng Neural
    return rsi.fillna(50.0) 

def compute_macd(series: pd.Series, fast: int = 12, slow: int = 26, signal: int = 9) -> pd.DataFrame:
    """
    Tính Toán Đường Trung Bình Động Hội Tụ Phân Kỳ (MACD).
    CÔNG THỨC: $MACD = EMA_{fast} - EMA_{slow}$ và $Signal = EMA_{signal}(MACD)$
    TẠI SAO: Rất linh nghiệm khi phát hiện sự đảo chiều xu hướng dựa vào điểm giao cắt Zero-Line hoặc Signal-Line,
    nuôi cấy (feed) cảm giác chu kỳ cực mạnh cho RNN/LSTM predictor.
    """
    ema_fast = series.ewm(span=fast, adjust=False).mean()
    ema_slow = series.ewm(span=slow, adjust=False).mean()
    macd = ema_fast - ema_slow
    sig = macd.ewm(span=signal, adjust=False).mean()
    hist = macd - sig
    
    return pd.DataFrame({'macd': macd, 'macd_signal': sig, 'macd_hist': hist})

def compute_volatility(series: pd.Series, period: int = 20) -> pd.Series:
    """
    Tính Toán Độ Biến Động Định Kỳ (Rolling Volatility).
    TẠI SAO CHÚNG TA CẦN CHỈ BÁO NÀY: Dành riêng cho Optimizer sử dụng thuật toán Risk Parity (HRP). 
    Quy luật bất biến: Tài sản có biên độ dao động càng mạnh sẽ bị triệt giảm tỷ trọng phân bổ vốn.
    """
    returns = series.pct_change()
    vol = returns.rolling(window=period).std()
    return vol.fillna(0.0)

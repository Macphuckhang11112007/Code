import pandas as pd
import numpy as np
import os

def compute_rsi(data: pd.Series, window: int = 14) -> pd.Series:
    delta = data.diff()
    gain = (delta.where(delta > 0, 0)).fillna(0)
    loss = (-delta.where(delta < 0, 0)).fillna(0)
    avg_gain = gain.rolling(window=window, min_periods=1).mean()
    avg_loss = loss.rolling(window=window, min_periods=1).mean()
    rs = avg_gain / avg_loss
    return 100 - (100 / (1 + rs))

def compute_macd(data: pd.Series, fast: int = 12, slow: int = 26, signal: int = 9):
    exp1 = data.ewm(span=fast, adjust=False).mean()
    exp2 = data.ewm(span=slow, adjust=False).mean()
    macd_line = exp1 - exp2
    signal_line = macd_line.ewm(span=signal, adjust=False).mean()
    macd_hist = macd_line - signal_line
    return macd_line, signal_line, macd_hist

def compute_and_cache_indicators(market_data_dir: str = "data/trades", output_dir: str = "data/features"):
    """
    Tính toán các chỉ báo kỹ thuật hạng nặng (MA20, MA50, RSI, MACD) cho toàn bộ file CSV
    và lưu trực tiếp ra định dạng .parquet để Streamlit UI load nhanh.
    """
    os.makedirs(output_dir, exist_ok=True)
    
    if not os.path.exists(market_data_dir):
        return

    for file_name in os.listdir(market_data_dir):
        if not file_name.endswith(".csv"):
            continue
            
        sym = file_name.replace(".csv", "")
        df = pd.read_csv(os.path.join(market_data_dir, file_name))
        
        if 'close' not in df.columns:
            continue
            
        # Calculation
        df['ma20'] = df['close'].rolling(window=20, min_periods=1).mean()
        df['ma50'] = df['close'].rolling(window=50, min_periods=1).mean()
        df['rsi'] = compute_rsi(df['close'], window=14)
        
        macd_line, sig_line, macd_hist = compute_macd(df['close'])
        df['macd'] = macd_line
        df['macd_signal'] = sig_line
        df['macd_hist'] = macd_hist
        
        out_path = os.path.join(output_dir, f"{sym}_indicators.parquet")
        df.to_parquet(out_path, index=False)

import os
import pandas as pd
import numpy as np
from datetime import datetime, timedelta

TRADE_SYMBOLS = ['NVDA', 'AAPL', 'TSLA', 'SPY', 'BTC_USDT', 'ETH_USDT', 'SOL_USDT', 'EUR_USD', 'XAU_USD', 'DXY']
RATE_SYMBOLS = ['US10Y_10y', 'VCB_deposit_6m', 'VCB_deposit_1m']
STAT_SYMBOLS = ['US_CPI', 'VN_GDP']

os.makedirs('data/trades', exist_ok=True)
os.makedirs('data/rates', exist_ok=True)
os.makedirs('data/stats', exist_ok=True)

def generate_mock_csv(path, base_px, volatility):
    if os.path.exists(path):
        return
    print(f"Generating {path}...")
    start = datetime(2023, 1, 1)
    # 52 weeks * 5 days * 24 hours * 4 quarters = a lot. Let's just do 3 months (approx 8000 bars)
    periods = 8000
    times = [start + timedelta(minutes=15 * i) for i in range(periods)]
    
    # Random walk
    np.random.seed(42)
    returns = np.random.normal(0, volatility, periods)
    prices = base_px * np.exp(np.cumsum(returns))
    
    df = pd.DataFrame({
        'timestamp': times,
        'open': prices * (1 + np.random.normal(0, 0.001, periods)),
        'high': prices * (1 + abs(np.random.normal(0, 0.002, periods))),
        'low': prices * (1 - abs(np.random.normal(0, 0.002, periods))),
        'close': prices,
        'adj_close': prices,
        'volume': np.random.uniform(1000, 1000000, periods),
        'turnover': np.random.uniform(1000, 1000000, periods)*prices,
        'change': np.zeros(periods),
        'change_pct': np.zeros(periods),
        'mom_pct': np.zeros(periods),
        'yoy_pct': np.zeros(periods),
        'dividends': np.zeros(periods),
        'stock_splits': np.zeros(periods)
    })
    
    # Fill remaining columns up to 25 to satisfy market.py strict requirement (usually expects 25)
    for i in range(14, 26):
        df[f'feature_{i}'] = 0.0
        
    # Calculate simple changes
    df['change'] = df['close'].diff().fillna(0)
    df['change_pct'] = df['close'].pct_change().fillna(0)
    
    df.to_csv(path, index=False)

def main():
    for sym in TRADE_SYMBOLS:
        vol = 0.005 if 'META' in sym or 'NVDA' in sym or 'USDT' in sym else 0.002
        generate_mock_csv(f'data/trades/{sym}.csv', 100.0, vol)
        
    for sym in RATE_SYMBOLS:
        generate_mock_csv(f'data/rates/{sym}.csv', 5.0, 0.0001) # Rates change slowly
        
    for sym in STAT_SYMBOLS:
        generate_mock_csv(f'data/stats/{sym}.csv', 100.0, 0.001)
        
if __name__ == '__main__':
    main()

import os
import pandas as pd
import numpy as np
from typing import List, Dict, Tuple, Iterator

from src.pipeline.features import compute_rsi, compute_macd, compute_volatility

class SmartBatchLoader:
    def __init__(self, data_path: str, batch_size: int = 20):
        self.data_path = data_path
        self.batch_size = batch_size
        
        # Load universal context assets (Rates & Stats)
        self.rate_assets = self._scan_dir('rates')
        self.stat_assets = self._scan_dir('stats')
        self.trade_assets = self._scan_dir('trades')
        
    def _scan_dir(self, folder: str) -> List[str]:
        p = os.path.join(self.data_path, folder)
        if not os.path.exists(p):
            return []
        return [f.replace('.csv', '') for f in os.listdir(p) if f.endswith('.csv')]
        
    def _create_master_timeline(self, asset_files: List[str]) -> pd.DatetimeIndex:
        all_indices = []
        for file in asset_files:
            try:
                # Use usecols and parse_dates for memory efficiency when just extracting the index
                df = pd.read_csv(file, usecols=['timestamp'], parse_dates=['timestamp'])
                all_indices.append(df['timestamp'])
            except Exception as e:
                print(f"Error reading timeline from {file}: {e}")
                
        if not all_indices:
            return pd.DatetimeIndex([])
            
        master_series = pd.concat(all_indices)
        # Drop NaN and get unique sorted timestamps
        master_ts = pd.DatetimeIndex(master_series.dropna().unique()).sort_values()
        
        # Bound the timeline to 8 years max to prevent RAM explosion (borrowed from market.py logic)
        if len(master_ts) > 0:
            max_date = master_ts.max()
            min_date = max_date - pd.Timedelta(days=365*8)
            master_ts = master_ts[(master_ts >= min_date) & (master_ts <= max_date)]
            
        return master_ts

    def _load_and_align_batch(self, batch_trade_assets: List[str]) -> Tuple[pd.DataFrame, pd.DatetimeIndex]:
        """
        Loads the batch of trades + all universal rates/stats.
        Resolves timeframe gaps using ffill/bfill safely (Phase 1).
        """
        all_targets = []
        for sym in batch_trade_assets:
            all_targets.append((sym, os.path.join(self.data_path, 'trades', f"{sym}.csv")))
        for sym in self.rate_assets:
            all_targets.append((sym, os.path.join(self.data_path, 'rates', f"{sym}.csv")))
        for sym in self.stat_assets:
            all_targets.append((sym, os.path.join(self.data_path, 'stats', f"{sym}.csv")))
            
        file_paths = [path for _, path in all_targets]
        master_timeline = self._create_master_timeline(file_paths)
        
        if len(master_timeline) == 0:
            return pd.DataFrame(), master_timeline
            
        aligned_dfs = {}
        for sym, path in all_targets:
            try:
                df = pd.read_csv(path, index_col='timestamp', parse_dates=True)
                # Reindex aligns to the master timeline, introducing NaNs for missing spots
                df = df.reindex(master_timeline)
                
                df.ffill(inplace=True)
                df.bfill(inplace=True)
                
                # Phase 2: Bắt buộc nhồi thêm động lượng (Momentum) cho TRADE assets
                if sym in batch_trade_assets:
                    # Giao diện đóng mở nến của crypto/stock
                    base_price = df['close'] if 'close' in df.columns else df.iloc[:, 0]
                    
                    df['rsi'] = compute_rsi(base_price, period=14)
                    
                    macd_df = compute_macd(base_price)
                    df['macd'] = macd_df['macd']
                    df['macd_signal'] = macd_df['macd_signal']
                    df['macd_hist'] = macd_df['macd_hist']
                    
                    df['volatility'] = compute_volatility(base_price, period=20)
                
                # Add a prefix to columns to avoid collision in the mega matrix, except we'll use MultiIndex
                # Actually, dict of DataFrames is cleaner before concatenating onto axis 1
                aligned_dfs[sym] = df
            except Exception as e:
                print(f"Error processing {sym} for batch matrix: {e}")
                
        # Concat along columns. Keys become the top level of a MultiIndex
        mega_matrix = pd.concat(aligned_dfs, axis=1)
        return mega_matrix, master_timeline
        
    def get_batches(self) -> Iterator[Tuple[int, List[str], pd.DataFrame, pd.DatetimeIndex]]:
        """
        Generator that chunks the trade assets and yields a massive pre-aligned DataFrame.
        """
        total_assets = len(self.trade_assets)
        for i in range(0, total_assets, self.batch_size):
            batch_id = i // self.batch_size + 1
            batch_symbols = self.trade_assets[i:i + self.batch_size]
            
            print(f"[SmartBatchLoader] Assembling Batch {batch_id} with {len(batch_symbols)} trade assets...")
            mega_matrix, master_timeline = self._load_and_align_batch(batch_symbols)
            
            # Yield the heavy memory object
            yield batch_id, batch_symbols, mega_matrix, master_timeline

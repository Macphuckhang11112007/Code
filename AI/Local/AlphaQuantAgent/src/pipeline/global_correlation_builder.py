import os
import pandas as pd
import json
import numpy as np

def build_correlation_matrix():
    print("Building Global Correlation Matrix from all assets...")
    data_dirs = ["data/trades", "data/rates", "data/stats"]
    
    # Dictionary to hold the close price series for each asset
    # Key: symbol, Value: Pandas Series
    close_series_dict = {}
    
    for d in data_dirs:
        if os.path.exists(d):
            files = [f for f in os.listdir(d) if f.endswith(".csv")]
            for file in files:
                sym = file.replace(".csv", "")
                filepath = os.path.join(d, file)
                try:
                    df = pd.read_csv(filepath)
                    if 'timestamp' in df.columns and 'close' in df.columns:
                        # Set timestamp as index and extract the close column
                        df['timestamp'] = pd.to_datetime(df['timestamp'])
                        df.set_index('timestamp', inplace=True)
                        # Drop duplicates if any
                        df = df[~df.index.duplicated(keep='last')]
                        close_series_dict[sym] = df['close']
                except Exception as e:
                    print(f"Error reading {file}: {e}")
                    
    if not close_series_dict:
        print("No valid CSV files found.")
        return
        
    print(f"Loaded {len(close_series_dict)} assets. Concatenating into Mega DataFrame...")
    # Concat all series into one massive Dataframe along columns
    mega_df = pd.concat(close_series_dict, axis=1)
    
    print("Resolving time misalignment using forward/backward fill...")
    # Fill NA to resolve alignment issues over time between macro vs trades
    mega_df.ffill(inplace=True)
    mega_df.bfill(inplace=True)
    
    print("Calculating Percentage Changes...")
    # Calculate percentage change for correlation
    pct_df = mega_df.pct_change(fill_method=None)
    
    print("Calculating Pearson Correlation Matrix...")
    # Calculate Pearson correlation matrix
    corr_matrix = pct_df.corr()
    
    # Fill NaN values in correlation matrix (if any exist due to zero variance) with 0
    corr_matrix.fillna(0, inplace=True)
    
    print("Exporting matrix to nested JSON...")
    # Convert DataFrame to nested dictionary
    corr_dict = {}
    for col in corr_matrix.columns:
        # Get the correlation values for this column, drop self correlation (1.0)
        col_corr = corr_matrix[col].drop(index=col)
        # Convert to dictionary {symbol: correlation_value}
        corr_dict[col] = col_corr.to_dict()
        
    output_path = "data/features/global_correlation.json"
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    with open(output_path, 'w', encoding='utf-8') as f:
        json.dump(corr_dict, f, indent=4)
        
    print(f"Done! Global correlation matrix saved to {output_path}")

if __name__ == "__main__":
    build_correlation_matrix()

import pandas as pd
import numpy as np

# Generate mock data for 50 assets
print(f"Creating mock dataframe for 50 distinct assets...")
np.random.seed(42)
dates = pd.date_range('20230101', periods=100)
df = pd.DataFrame(np.random.randn(100, 50), index=dates, columns=[f'ASSET_{i}' for i in range(50)])

print(f"Calculating dynamic Pearson correlation matrix...")
corr_df = df.corr().fillna(0).round(2)

print(f"Converting array to nested recursive dictionary...")
correlation_matrix_data = corr_df.to_dict()

# Checking shape and type safety
keys_count = len(correlation_matrix_data.keys())
inner_keys_count = len(correlation_matrix_data['ASSET_0'].keys())

print(f"Outer Key Count (Columns mapping): {keys_count}")
print(f"Inner Key Count (Rows mapping): {inner_keys_count}")

assert keys_count == 50
assert inner_keys_count == 50

print("SUCCESS: Python natively handles massive correlations via Pandas corr() mapping direct to nested dictionaries.")

import pandas as pd
import numpy as np
import traceback
import sys
from src.engine.quant_analyzer import calculate_advanced_metrics

with open('test_analyzer_res.txt', 'w') as err_file:
    sys.stdout = err_file
    sys.stderr = err_file
    try:
        print("Testing calculate_advanced_metrics with Extreme Adversarial Data...")
        df = pd.DataFrame({"BTC_USDT": [100, 100, 100], "ETH_USDT": [np.nan, np.nan, np.nan]})
        res = calculate_advanced_metrics(
            portfolio_history=[100000, 100000, 100000],
            trade_history=[],
            price_data_df=df,
            feature_map_keys=["close", "volume"]
        )
        print("SUCCESS")
    except Exception:
        traceback.print_exc(file=err_file)

import os
import sys

# Wrap the backtest command
print("Starting Backtest Wrapper")
os.system(f"{sys.executable} main.py --mode backtest --sims 1")
print("Finished Backtest")

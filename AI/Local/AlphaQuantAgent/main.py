"""
/**
 * FILE: main.py
 * VAI TRÒ: Điểm Đầu Vào (Entry Point) số 1 dành cho Tiến trình Hậu cần (Backend CLI).
 * CHỨC NĂNG:
 * - Điều phối quy trình nạp dữ liệu (ETL).
 * - Huấn luyện các Agent học máy (Training).
 * - Kiểm thử lịch sử giao dịch (Backtesting).
 * QUY TẮC BẤT BẤN: Không bao giờ được phép chứa code UI/Giao diện tại đây. Mọi xử lý đều ở mức hệ thống / Terminal.
 */
"""
import os
os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'  # Suppress TensorFlow oneDNN warning
# Bắt buộc ép thư mục làm việc hiện tại (CWD) về Rễ của Dự án để tránh lỗi đường dẫn trên Windows
os.chdir(os.path.dirname(os.path.abspath(__file__)))

import argparse
import sys
from src.utils.logger import logger
from src.utils.config_loader import config

def run_train(args):
    """Quy trình huấn luyện mạng Neural và PPO Agent."""
    logger.info(f"Khởi động tiến trình TRAINING cho agent: {args.agent}")
    from src.engine.market import Market
    from src.engine.simulator import TradingSimulator
    from src.engine.wallet import Wallet
    from src.engine.env import AlphaQuantEnv
    from src.agents.trader import RLTrader
    from src.agents.callbacks import EarlyStoppingAndLogging
    import os

    model_path = "models/rl_agent/best_model.zip"
    if os.path.exists(model_path) and not getattr(args, 'force', False):
        logger.info(f"Phát hiện Model Cache tại {model_path}. Bỏ qua huấn luyện để tiết kiệm thời gian 15-30 phút.")
        logger.info("Mẹo: Thêm cờ `--force` khi chạy `python main.py --mode train --force` nếu bạn thực sự muốn train lại từ đầu (Retrain).")
        return

    config_sys = config.system
    data_dir = config_sys.get('data_paths', {}).get('base_dir', 'data/')
    market = Market(asset_list=["BTC_USDT"], context_list=["US10Y_10y", "US_CPI"], data_path=data_dir)
    market.load()

    wallet = Wallet(initial_capital=config_sys.get('initial_capital', 10000.0), penalty_beta=config.models.get('penalty_beta', 0.01))
    sim = TradingSimulator(market, wallet, window_size=config.models.get('window_size', 16))
    env = AlphaQuantEnv(sim)

    agent = RLTrader(env)
    
    cb = EarlyStoppingAndLogging(check_freq=50, log_dir="logs/training/tensorboard/")
    agent.learn(total_timesteps=args.epochs, callbacks=[cb])
    
    agent.save("models/rl_agent/best_model.zip")
    logger.info("Huấn luyện hoàn tất và đã lưu Model Best.")

def run_backtest(args):
    """Quy trình giả lập quá khứ một cách quyết đoán (Deterministic)."""
    logger.info("Khởi động tiến trình BACKTESTING.")
    from src.engine.market import Market
    from src.engine.simulator import TradingSimulator
    from src.engine.wallet import Wallet
    from src.engine.env import AlphaQuantEnv
    from src.agents.trader import RLTrader
    import os
    
    config_sys = config.system
    data_dir = config_sys.get('data_paths', {}).get('base_dir', 'data/')
    market = Market(asset_list=["BTC_USDT"], context_list=["US10Y_10y", "US_CPI"], data_path=data_dir)
    market.load()

    wallet = Wallet(initial_capital=config_sys.get('initial_capital', 10000.0))
    sim = TradingSimulator(market, wallet, window_size=config.models.get('window_size', 16))
    env = AlphaQuantEnv(sim)

    model_path = "models/rl_agent/best_model.zip"
    agent = RLTrader(env, model_path=model_path if os.path.exists(model_path) else None)
    
    obs, _ = env.reset()
    done = False
    
    logger.info("Bắt đầu vòng lặp thời gian cốt lõi...")
    while not done:
        action = agent.predict(obs)
        obs, reward, done, truncated, info = env.step(action)
        
    logger.info("Backtest kết thúc. Trích xuất thành quả File Sổ cái (Ledger Export)...")
    wallet.export_csv("logs/trading/transactions.csv")
    metrics = wallet.get_metrics()
    logger.info(f"Kết quả NAV: {metrics}")

def run_features(args):
    """Quy trình sản xuất Đặc trưng (Feature Engineering)."""
    logger.info("Khởi động tiến trình FEATURES COMPUTATION.")
    from src.engine.market import Market
    config_sys = config.system
    data_dir = config_sys.get('data_paths', {}).get('base_dir', 'data/')
    market = Market(asset_list=["BTC_USDT"], context_list=["US10Y_10y", "US_CPI"], data_path=data_dir)
    market.load()
    logger.info(f"Hoạt động tạo Đặc trưng Tensor Vector đã nạp thành công: {market.data.shape}")

def run_monte_carlo(args):
    """Quy trình thử nghiệm Áp lực Đa Vũ Trụ (Monte Carlo Stress Test)."""
    logger.info("Khởi động tiến trình MONTE CARLO (Quantum Audit).")
    from src.engine.market import Market
    from src.engine.monte_carlo import MonteCarloSimulator
    import numpy as np
    
    config_sys = config.system
    data_dir = config_sys.get('data_paths', {}).get('base_dir', 'data/')
    market = Market(asset_list=["BTC_USDT"], context_list=["US10Y_10y", "US_CPI"], data_path=data_dir)
    market.load()

    # Nhổ lấy Lịch sử Returns thực tế để mồi (seed) cho Brownian Motion
    close_idx = market.feature_map.get('close')
    if close_idx is None:
        logger.error("Không tìm thấy giá Close. MC Hủy bỏ.")
        return
        
    hist_prices = market.data[:, 0, close_idx]
    hist_prices = hist_prices[hist_prices > 0] # Lọc rác
    returns = np.diff(hist_prices) / hist_prices[:-1]
    
    mc = MonteCarloSimulator(n_paths=1000, horizon_steps=252*4) # Lấy mốc 4 năm (15m candles scale)
    mu, sigma = mc.estimate_drift_and_vol(returns)
    
    # S0 là giá trị chốt phiên cuối cùng của lịch sử
    report = mc.run_stress_test(agent=None, initial_capital=10000.0, S0=hist_prices[-1], mu=mu, sigma=sigma)
    
    logger.info("============== BÁO CÁO TOÁN HỌC LƯỢNG TỬ (MONTE CARLO) ==============")
    logger.info(f"Kỳ vọng Lợi nhuận (Expected ROI): {report['Expected_ROI'] * 100:.2f} %")
    logger.info(f"Xác Suất Phá Sản (Margin Call): {report['Probability_of_Ruin'] * 100:.2f} %")
    logger.info(f"Rủi Ro Đuôi (CVaR 95%): {report['CVaR_95'] * 100:.2f} %")
    logger.info("=====================================================================")

def main():
    parser = argparse.ArgumentParser(description="AlphaQuantAgent: Backend CLI Engine")
    parser.add_argument("--mode", type=str, required=True, choices=["train", "backtest", "features", "monte_carlo"],
                        help="Chế độ chạy (train | backtest | features | monte_carlo)")
    parser.add_argument("--agent", type=str, default="ppo",
                        help="Tên agent để train (ppo/lstm/booster)")
    parser.add_argument("--epochs", type=int, default=1000,
                        help="Số vòng lặp huấn luyện tối đa (ép Early Stopping nếu cần)")
    parser.add_argument("--force", action="store_true",
                        help="Ép buộc train lại từ đầu bỏ qua cache mô hình")
                        
    args = parser.parse_args()
    
    logger.info(f"System Boot: Memory configured with {config.system.get('initial_capital', 'N/A')} USD initial capital.")
    
    if args.mode == "train":
        run_train(args)
    elif args.mode == "backtest":
        run_backtest(args)
    elif args.mode == "features":
        run_features(args)
    elif args.mode == "monte_carlo":
        run_monte_carlo(args)

if __name__ == "__main__":
    main()

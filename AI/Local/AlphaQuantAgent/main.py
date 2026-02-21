"""
/**
 * FILE: run_pipeline.py
 * VAI TRÒ: Điểm Đầu Vào (Entry Point) Duy Nhất dành cho cả Backend CLI và Giao diện UI (Streamlit).
 * CHỨC NĂNG:
 * - Điều phối quy trình nạp dữ liệu (ETL).
 * - Huấn luyện các Agent học máy (Training).
 * - Kiểm thử lịch sử giao dịch (Backtesting).
 * - Khởi chạy Giao diện Trực quan (UI).
 */
"""
import os
import sys

# MONKEY PATCH TENSORBOARD NATIVE KERNEL: BẺ GÃY LUẬT LƯU FILE CÓ HOSTNAME/TIMESTAMP
try:
    import tensorboard.summary.writer.event_file_writer as tb_efw
    import time
    
    class CleanEventFileWriter(tb_efw.EventFileWriter):
        def __init__(self, logdir, max_queue_size=10, flush_secs=120, filename_suffix=""):
            self._logdir = logdir
            
            # Tên tĩnh tuyệt đối. Nếu gọi nhiều lần trong 1 giây, ta thêm timestamp siêu ngắn thay vì Desktop ID dài thượt.
            # Hoặc ép thành "events.out.tfevents.alphaquant"
            self._file_name = os.path.join(logdir, "events.out.tfevents.alphaquant")
            
            # Nếu chép đè thì xóa rác cũ
            if os.path.exists(self._file_name):
                try: os.remove(self._file_name)
                except: pass
                
            # The RecordWriter expects a file-like object
            self._file_writer = open(self._file_name, "wb")
            self._ev_writer = tb_efw.RecordWriter(self._file_writer)
            self._async_writer = tb_efw._AsyncWriter(self._ev_writer, max_queue_size, flush_secs)
            self._closed = False
            
    # Ép ghi đè toàn hệ thống Python hiện tại
    tb_efw.EventFileWriter = CleanEventFileWriter
except ImportError:
    pass

os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'  # Suppress TensorFlow oneDNN warning
# Bắt buộc ép thư mục làm việc hiện tại (CWD) về Rễ của Dự án để tránh lỗi đường dẫn trên Windows
os.chdir(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

import argparse

def get_assets(dir_path):
    if not os.path.exists(dir_path): return []
    return [f.split('.')[0] for f in os.listdir(dir_path) if f.endswith('.csv')]

def main():
    from src.utils.logger import logger
    from src.utils.config_loader import config
    
    parser = argparse.ArgumentParser(description="AlphaQuantAgent: Master Orchestrator (The Singularity)")
    parser.add_argument("--epochs", type=int, default=1000,
                        help="Số vòng lặp huấn luyện tối đa (ép Early Stopping nếu cần)")
    parser.add_argument("--sims", type=int, default=1,
                        help="Số lượng persona cho Backtest (mặc định 1)")
    parser.add_argument("--force", action="store_true",
                        help="Ép buộc train lại từ đầu bỏ qua cache mô hình")
    parser.add_argument("--test-mode", action="store_true",
                        help="Kích hoạt chế độ test pipeline siêu tốc")
                        
    args, unknown = parser.parse_known_args()
    
    logger.info("=========================================================================")
    logger.info("                  ALPHAQUANT MASTER PIPELINE INITIATED                   ")
    logger.info("=========================================================================")

    # BƯỚC 1: KHỞI TẠO PERSONA (PERSONA GENERATION)
    logger.info("\n>>> [STEP 1/5] KHỞI TẠO VÀ CHUẨN BỊ PERSONA...")
    
    model_path = "models/rl_agent/best_model.zip"
    if args.force and os.path.exists(model_path):
        try: os.remove(model_path)
        except: pass

    config_sys = config.system
    data_dir = config_sys.get('data_paths', {}).get('base_dir', 'data/')
    
    trade_assets = get_assets(os.path.join(data_dir, 'trades'))
    rate_assets = get_assets(os.path.join(data_dir, 'rates'))
    stat_assets = get_assets(os.path.join(data_dir, 'stats'))
    context_assets = rate_assets + stat_assets

    import numpy as np
    from src.engine.persona_generator import PersonaGenerator
    from src.pipeline.batch_loader import SmartBatchLoader
    
    is_test_mode = getattr(args, 'test_mode', False)
    if is_test_mode:
        total_personas_to_gen = 2
        batch_size = 5
        epochs_total = 5000
        n_simulations = 1
    else:
        total_personas_to_gen = 50
        batch_size = 20
        epochs_total = args.epochs
        n_simulations = args.sims

    generator = PersonaGenerator(trade_assets, rate_assets, stat_assets)
    dataset = generator.generate_dataset(n_personas=total_personas_to_gen)
    np.random.shuffle(dataset)

    # Chia tập Train và tập Backtest (Validation)
    train_personas = dataset[:-n_simulations] if len(dataset) > n_simulations else dataset
    eval_persona = dataset[0] # Đại diện 1 người cho EvalCallback
    backtest_personas = dataset[-n_simulations:] if len(dataset) >= n_simulations else dataset

    # BƯỚC 2: HUẤN LUYỆN (TRAINING)
    logger.info(f"\n>>> [STEP 2/5] BẮT ĐẦU HUẤN LUYỆN PPO TRÊN {len(train_personas)} PERSONAS...")
    
    from src.engine.market import Market
    from src.engine.simulator import TradingSimulator
    from src.engine.wallet import Wallet
    from src.engine.env import AlphaQuantEnv
    from src.agents.trader import RLTrader
    from src.agents.callbacks import EarlyStoppingAndLogging
    
    from stable_baselines3.common.vec_env import SubprocVecEnv
    from stable_baselines3.common.callbacks import EvalCallback
    from stable_baselines3.common.callbacks import CallbackList
    import gc

    def make_env(persona, mega_matrix, master_timeline):
        def _init():
            import shutup
            shutup.please() 
            p_market = Market(
                asset_list=persona['trade_assets'] + rate_assets, 
                context_list=context_assets, 
                data_path=data_dir,
                pre_aligned_matrix=mega_matrix,
                pre_aligned_timeline=master_timeline
            )
            p_market.load()
            p_market.inject_ml_features()
            proxy_penalty_beta = persona['drawdown_penalty'] / 100.0  
            wallet = Wallet(initial_capital=persona['initial_capital'], penalty_beta=proxy_penalty_beta)
            sim = TradingSimulator(p_market, wallet, window_size=config.models.get('window_size', 16), allowed_trade_assets=persona['trade_assets'], persona=persona)
            env = AlphaQuantEnv(sim)
            return env
        return _init

    has_agent_instantiated = False
    
    if not os.path.exists(model_path) or args.force:
        loader = SmartBatchLoader(data_dir, batch_size=batch_size)
        for batch_id, batch_symbols, mega_matrix, master_timeline in loader.get_batches():
            if mega_matrix.empty: continue
            
            active_personas = [p for p in train_personas if set(p['trade_assets']).intersection(set(batch_symbols))]
            if not active_personas: continue
            active_personas = active_personas[:8]  # Giới hạn số luồng (an toàn cho RAM)
            
            logger.info(f"    -> Đang Train Batch {batch_id} với {len(active_personas)} Vector Môi trường đa chiều...")
            
            env_fns = [make_env(p, mega_matrix, master_timeline) for p in active_personas]
            vec_env = SubprocVecEnv(env_fns)
            
            eval_env_fn = make_env(eval_persona, mega_matrix, master_timeline)
            eval_env = SubprocVecEnv([eval_env_fn])
            
            eval_callback = EvalCallback(eval_env, best_model_save_path='models/rl_agent/',
                                         log_path='logs/training/tensorboard/ppo_eval/', eval_freq=100 if is_test_mode else 1000,
                                         deterministic=True, render=False)
            omni_cb = EarlyStoppingAndLogging(check_freq=50, log_dir="logs/training/tensorboard/ppo/")
            callbacks = CallbackList([eval_callback, omni_cb])
            
            if not has_agent_instantiated:
                agent = RLTrader(vec_env)
                has_agent_instantiated = True
            else:
                agent.model.set_env(vec_env)
                
            try:
                agent.learn(total_timesteps=epochs_total, callbacks=callbacks)
                agent.save(model_path)
            finally:
                vec_env.close()
                eval_env.close()
                del vec_env, eval_env, mega_matrix, master_timeline
                Market._raw_cache.clear()
                gc.collect()
            
            if is_test_mode: break
    else:
        logger.info("    -> Đã tìm thấy model được huấn luyện sẵn trong bộ nhớ. Bỏ qua Training PPO để tiết kiệm thời gian.")

    # BƯỚC 3: KIỂM THỬ GIAO DỊCH (BACKTESTING)
    logger.info(f"\n>>> [STEP 3/5] CHẠY KIỂM THỬ TRÊN TẬP VALIDATION ({len(backtest_personas)} PERSONAS)...")
    
    global_trade_history = []
    global_assets_traded = set()
    global_feature_keys = []
    last_portfolio_history = []
    
    for sim_idx, p in enumerate(backtest_personas):
        logger.info(f"    -> Đang mô phỏng giao dịch cho Validation Persona {sim_idx+1}/{len(backtest_personas)}...")
        global_assets_traded.update(p['trade_assets'])
        
        p_market = Market(asset_list=p['trade_assets'] + rate_assets, context_list=context_assets, data_path=data_dir)
        p_market.load()
        p_market.inject_ml_features()
        
        if not global_feature_keys and hasattr(p_market, 'feature_map'):
            global_feature_keys = list(p_market.feature_map.keys())
        
        wallet = Wallet(initial_capital=p['initial_capital'], penalty_beta=p['drawdown_penalty']/100.0)
        sim = TradingSimulator(p_market, wallet, window_size=config.models.get('window_size', 16), allowed_trade_assets=p['trade_assets'])
        env = AlphaQuantEnv(sim)

        agent = RLTrader(env, model_path=model_path if os.path.exists(model_path) else None)
        obs, _ = env.reset()
        done = False
        nav_history = []
        
        while not done:
            action = agent.predict(obs)
            obs, reward, done, truncated, info = env.step(action)
            nav_history.append(info['nav'])
            
        if sim_idx == len(backtest_personas) - 1:
            wallet.export_csv("logs/trading/transactions.csv")
            
        global_trade_history.extend(wallet.ledger)
        last_portfolio_history = nav_history
        
        env.close()
        del agent, env, sim, wallet, p_market
        gc.collect()

    import pandas as pd
    from src.engine.quant_analyzer import calculate_advanced_metrics
    
    df_market = pd.DataFrame()
    for asset in global_assets_traded:
        if asset in Market._raw_cache:
            df_asset = Market._raw_cache[asset]
            if 'close' in df_asset.columns:
                if df_market.empty:
                    df_market[asset] = df_asset['close']
                else:
                    df_market = df_market.join(df_asset['close'].rename(asset), how='outer')
    df_market = df_market.ffill().fillna(0)

    # BƯỚC 4: MONTE CARLO STRESS TEST
    logger.info("\n>>> [STEP 4/5] CHẠY MÔ PHỎNG LƯỢNG TỬ MONTE CARLO TRÊN LỊCH SỬ THỰC TẾ...")
    from src.engine.monte_carlo import MonteCarloSimulator
    mc_report = None
    
    if len(last_portfolio_history) > 2:
        returns = pd.Series(last_portfolio_history).pct_change().dropna().values
        mc = MonteCarloSimulator(n_paths=1000, horizon_steps=252*4) 
        mu, sigma = mc.estimate_drift_and_vol(returns)
        S0 = last_portfolio_history[-1]
        try:
            mc_report = mc.run_stress_test(agent=None, initial_capital=S0, S0=S0, mu=mu, sigma=sigma)
            logger.info("    -> Monte Carlo hoàn tất. Tính toán rủi ro thành công.")
        except Exception as e:
            logger.error(f"    -> Lỗi khi chạy Monte Carlo: {e}")
    else:
        logger.warning("    -> Không đủ dữ liệu sinh lời để chạy Monte Carlo.")

    # BƯỚC 5: TÓM GỌN CHỈ SỐ JSON (THE GOLDEN DUMP)
    logger.info("\n>>> [STEP 5/5] TỔNG HỢP SIÊU CHỈ SỐ ĐỊNH LƯỢNG (THE GOLDEN DUMP)...")
    calculate_advanced_metrics(last_portfolio_history, global_trade_history, df_market, feature_map_keys=global_feature_keys, mc_report=mc_report)
    
    Market._raw_cache.clear()
    gc.collect()
            
    logger.info("\n=========================================================================")
    logger.info("🎉 ĐƯỜNG ỐNG DỮ LIỆU HOÀN TẤT. [advanced_quant_metrics.json] ĐÃ SẴN SÀNG CHO REACT/NODE!")
    logger.info("=========================================================================\n")

if __name__ == "__main__":
    main()

import traceback
import sys

with open('test_persona_res.txt', 'w') as err_file:
    sys.stdout = err_file
    sys.stderr = err_file
    try:
        from src.engine.persona_generator import PersonaGenerator
        import time
        import numpy as np

        print("Testing Persona Generator with Farthest Point Sampling...")
        trade_assets = [f"TRADE_{i}" for i in range(50)]
        rate_assets = [f"RATE_{i}" for i in range(10)]
        stat_assets = [f"STAT_{i}" for i in range(5)]
        
        start = time.time()
        generator = PersonaGenerator(trade_assets, rate_assets, stat_assets)
        dataset = generator.generate_dataset(n_personas=500)
        
        print(f"Completed generation of {len(dataset)} personas in {time.time() - start:.2f}s")
        
        # Assert constraints
        for i, p in enumerate(dataset):
            assert 10 <= p['trade_assets_count'] <= 30, f"Violation of trade_assets_count limits at index {i}"
            assert len(p['context_assets']) == 15, "Violation of context assets inclusion limits"
            assert p['initial_capital'] >= 5.0 and p['initial_capital'] <= 100000000.0, "Capital out of bounds"
            
        print("All constraints perfectly maintained. Blueprint 13 fully respected.")
    except Exception as e:
        traceback.print_exc(file=err_file)

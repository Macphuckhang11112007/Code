import traceback
import sys

with open('error_log.txt', 'w') as f:
    try:
        import src.engine.quant_analyzer
        f.write("Import successful")
    except BaseException as e:
        traceback.print_exc(file=f)

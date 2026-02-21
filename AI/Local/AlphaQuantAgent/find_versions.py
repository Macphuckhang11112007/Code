import os
import re

keywords = ['ultimate', 'final', 'version', 'ver', 'v1', 'v2', 'v3', 'patched']
pattern = re.compile(r'\b(' + '|'.join(keywords) + r')\b', re.IGNORECASE)

for root, _, files in os.walk('.'):
    if any(exclude in root for exclude in ['node_modules', '.git', 'venv', '__pycache__', 'logs', 'dist', 'brain', 'tmp']):
        continue
    for file in files:
        if not file.endswith(('.js', '.jsx', '.py', '.yaml', '.html', '.css')):
            continue
        path = os.path.join(root, file)
        try:
            with open(path, 'r', encoding='utf-8') as f:
                lines = f.readlines()
            for i, line in enumerate(lines):
                if pattern.search(line):
                    print(f'{path}:{i+1}: {line.strip()[:100]}')
        except Exception:
            pass

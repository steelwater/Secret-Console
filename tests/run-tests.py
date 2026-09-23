#!/usr/bin/env python3
"""Compile the real sketch with lightweight display/button/audio test doubles."""
import os
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
sketch = (root / 'arduboy/SecretConsole/SecretConsole.ino').read_text()
# Arduino generates these declarations before compiling an .ino file.
prototypes = re.findall(r'^(?:void|bool|uint8_t|char|const __FlashStringHelper \*)\s*\w+\([^\n]*\) \{', sketch, re.M)
marker = 'void setup() {'
sketch = sketch.replace(marker, '\n'.join(p[:-2] + ';' for p in prototypes) + '\n\n' + marker, 1)
with tempfile.TemporaryDirectory(prefix='secret-console-tests-') as tmp:
    source = Path(tmp) / 'tests.cpp'
    source.write_text(sketch + '\n' + (root / 'tests/behavior.cpp').read_text())
    binary = Path(tmp) / 'tests'
    subprocess.run([os.environ.get('CXX', 'c++'), '-std=c++11', '-Wall', '-Wextra', '-Werror', '-I', str(root / 'tests/stubs'), str(source), '-o', str(binary)], check=True)
    subprocess.run([str(binary)], check=True)

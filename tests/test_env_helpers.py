"""Run: python tests/test_env_helpers.py"""

import os
import sys
import types

SRC = os.path.join(os.path.dirname(__file__), "..", "python", "extension.py")

# extension.py imports krita at top; stub it so only the env helpers load.
krita = types.ModuleType("krita")
krita.Extension = type("Extension", (), {})
krita.Krita = type("Krita", (), {})
sys.modules.setdefault("krita", krita)

with open(SRC, encoding="utf-8") as f:
    head = f.read().split("class PixelToolsExtension")[0]

ns = {"__name__": "test_env_helpers"}
exec(compile(head, SRC, "exec"), ns)
_env_add_path = ns["_env_add_path"]
_restore_env = ns["_restore_env"]

VAR = "PIXELTOOLS_TEST_VAR"
os.environ.pop(VAR, None)

prev = _env_add_path(VAR, "C:\\x", "", "D:\\y")
assert os.environ[VAR] == "C:\\x" + os.pathsep + "D:\\y", os.environ[VAR]
_restore_env(VAR, prev)
assert VAR not in os.environ, "PATH prepend leaked into environment"

os.environ[VAR] = "ORIG"
prev = _env_add_path(VAR, "C:\\x")
assert os.environ[VAR] == "C:\\x" + os.pathsep + "ORIG"
_restore_env(VAR, prev)
assert os.environ[VAR] == "ORIG"

prev = _env_add_path(VAR)
assert os.environ[VAR] == "ORIG"

os.environ.pop(VAR, None)
print("OK - env helper checks passed")

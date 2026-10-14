import ctypes
import os
import sys
from krita import Extension, Krita
from pathlib import Path

if sys.platform in ["win32", "cygwin", "msys"]:
    platform = "windows"
elif sys.platform == "linux":
    platform = "linux"
elif sys.platform == "darwin":
    platform = "macos"
else:
    raise RuntimeError(f"Unsupported platform: {sys.platform}")


_UNSET = object()


def _env_add_path(var, *paths):
    prev = os.environ.get(var, _UNSET)
    added = os.pathsep.join(str(p) for p in paths if str(p))
    if added:
        base = "" if prev is _UNSET else prev
        os.environ[var] = f"{added}{os.pathsep}{base}" if base else added
    return prev


def _restore_env(var, prev):
    if prev is _UNSET:
        os.environ.pop(var, None)
    else:
        os.environ[var] = prev


class PixelToolsExtension(Extension):
    """Loads the native library; the tools themselves live in C++."""

    def __init__(self, parent):
        super().__init__(parent)

        ext = {"windows": ".dll", "linux": ".so", "macos": ".dylib"}[platform]
        lib_dir = Path(__file__).parent / "lib"
        lib_file = lib_dir / f"kitapixeltools{ext}"

        executable_dir = Path(sys.executable).parent
        bin_paths = []
        if platform == "windows":
            bin_paths = [lib_dir, executable_dir]
        env_path = _env_add_path("PATH", *bin_paths)

        ld_paths = []
        if platform == "linux":
            ld_paths.append(lib_dir.resolve())
        if platform == "linux" and "APPDIR" in os.environ:  # for AppImage
            ld_paths.append(Path(os.environ["APPDIR"]) / "usr" / "lib")
        ld_path = _env_add_path("LD_LIBRARY_PATH", *ld_paths)

        try:
            lib = ctypes.CDLL(str(lib_file.resolve()))
            lib.load_pixel_tools_plugin()
        except OSError as e:
            raise RuntimeError(
                f"Failed to load PixelTools library from {lib_file}: {e}"
            )
        finally:
            _restore_env("PATH", env_path)
            _restore_env("LD_LIBRARY_PATH", ld_path)

    def setup(self):
        pass

    def shutdown(self):
        pass

    def createActions(self, window):
        pass


Krita.instance().addExtension(PixelToolsExtension(Krita.instance()))

import platform

_system = platform.system().lower()

if _system == 'windows':
    from .win import repl_
else:
    from .nix import repl_

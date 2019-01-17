import shlex

import _pybasic

_bvm_ins = _pybasic.get_bytecodes()

from .compiler import pyb_compile
from .core import Interpreter
from .repl import repl_ as run_repl

_pybasic.set_tokenizer(pyb_compile)

__version__ = '0.2.0'
__author__ = 'mental'

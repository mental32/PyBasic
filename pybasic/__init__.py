import shlex

import _pybasic

_bvm_ins = _pybasic.get_bytecodes()

from . import parser
from .core import Interpreter

_pybasic.set_tokenizer(parser.tokenize)

__version__ = '0.1.0'
__author__ = 'mental'

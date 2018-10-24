import shlex

import _pybasic

def _tokenize(source):
    return [list(iter(shlex.shlex(line).get_token, '')) for line in source.split('\n')]

_pybasic.set_tokenizer(_tokenize)

from . import parser
from .core import Interpreter

__version__ = '0.1.0'
__author__ = 'mental'

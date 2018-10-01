import re
import shlex

import _pybasic

RE_DIGIT = r'((\d|\d_)\d)'

class Interpreter(_pybasic.ByteCodeInterpreter):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._code = {}
        self._data = {}

    def __repr__(self):
        return '<pybasic.Interpreter: state="%s">' % 'IDLE'

    def __enter__(self):
        return self

    def __exit__(self, *args):
        pass

    def __tokenize(self, source):
        return shlex.split(source)

    def run_simple(self, source):
        ln, *tokens = self._tokenize(source)

        self._code[ln] = tokens;

        # print(_pybasic.is_integer(ln))

    def exec_line(self, line_number):
        pass

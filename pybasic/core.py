import re
import shlex

import _pybasic

from .errors import BadSyntax

# Bytecodes are defined in pybasic_c/_pyb_bytecode.c

RE_DIGIT = r'((\d|\d_)\d)'

RE_PRINT = r'(print|PRINT) (\(.*\)|.*)'
RE_GOTO = r'(goto|GOTO) (\d{1,3})'
RE_LET = r'(let|LET) ([A-Za-z_]*)( = |=)(.*)'
RE_IF = r'(if|IF) (.*) (goto|GOTO) (\d{1,3})'
RE_FOR = r'(for|FOR) (.*) (to|TO) (.*) ((step|STEP) \d{1,9}|)'

patterns = {
    RE_PRINT: 'print',
    RE_LET: 'let',
    RE_IF: 'if',
}


class Interpreter(_pybasic.ByteCodeInterpreter):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._code = {}
        self._data = {}
        self._constants = []
        self._stack = []

    def __repr__(self):
        return '<pybasic.Interpreter: state="%s">' % 'IDLE'

    def _tokenize(self, source):
        return list(iter(shlex.shlex(source).get_token, ''))

    def run_simple(self, source):
        ln, *tokens = self._tokenize(source)
        source_no_ln = source[len(ln):].strip()
        self._code[ln] = tokens

        if not _pybasic.is_integer(ln):
            raise ValueError("Invalid line number.")

        bytecode = []

        for pattern, name in patterns.items():
            match = re.fullmatch(pattern, source_no_ln)
            if match:
                bytecode += self._handle(name, match)

    def exec_line(self, line_number):
        pass

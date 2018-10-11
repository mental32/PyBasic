import re
import shlex

import _pybasic
from .errors import BadSyntax

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
        return '<pybasic.Interpreter@%s>' % hex(id(self)).upper()

    def _tokenize(self, source):
        return list(iter(shlex.shlex(source).get_token, ''))

    def run_source(self, source):
        for line in source.split('\n'):
            ln, *tokens = self._tokenize(line)

            if not _pybasic.is_integer(ln):
                raise ValueError("Invalid line number.")

            source_no_ln = line[len(ln):].strip()
            self._code[ln] = tokens

            for pattern, name in patterns.items():
                match = re.fullmatch(pattern, source_no_ln)
                if match:
                    bytecode += self._compile_match(name, match)

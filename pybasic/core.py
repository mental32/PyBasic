import re
import shlex

import _pybasic

# Bytecodes are defined in pybasic_c/_pyb_bytecode.c
#
# enum BYTECODE {
#     STORE,
#     LOAD,
#     LOAD_KEYWRD,
#     LOAD_GLOBAL,
#     LOAD_ARGS,
#     CALL,
#     JUMP
# };

RE_DIGIT = r'((\d|\d_)\d)'

keywords = {
    'let': [],
}


class Interpreter(_pybasic.ByteCodeInterpreter):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._code = {}
        self._data = {}

    def __repr__(self):
        return '<pybasic.Interpreter: state="%s">' % 'IDLE'

    def _tokenize(self, source):
        return list(iter(shlex.shlex(source).get_token, ''))

    def _compile_tokens(self, tokens):
        bytecode = []

        for token in tokes:
            value = keywords.get(token)

            if value is not None:
                bytecode += val
                continue

        return bytecode

    def run_simple(self, source):
        ln, *tokens = self._tokenize(source)

        if not _pybasic.is_integer(ln):
            raise ValueError("Invalid line number.")

        self._code[ln] = tokens

        # print(_pybasic.is_integer(ln))

    def exec_line(self, line_number):
        pass

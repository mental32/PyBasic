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

# 10 let x = 7
# 

RE_DIGIT = r'((\d|\d_)\d)'


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
        constants = tuple({t for t in tokens if _pybasic.is_constant(t)})
        bytecode = []
        i_tokens = iter(tokens)

        for token in i_tokens:
            pass

        return bytecode

    def run_simple(self, source):
        ln, *tokens = self._tokenize(source)

        if not _pybasic.is_integer(ln):
            raise ValueError("Invalid line number.")

        self._code[ln] = self._compile_tokens(tokens)

        # print(_pybasic.is_integer(ln))

    def exec_line(self, line_number):
        pass

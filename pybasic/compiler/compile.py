import struct

from .grammar import SYNTAX_BASE
from .evaluate import evaluate
from .tokenize import tokenize
from .tokens import _token_string
from .bytecode import (
    _ins_store,
    _ins_print,
    _ins_return,
    _ins_jmpt,
    _ins_jmp,
)

class Metadata:
    __slots__ = ('constants', 'symt')

    def __init__(self, const, symt):
        self.constants = const
        self.symt = symt

    @property
    def fsymt(self):
        return (*self.symt.keys(),)


def pyb_compile(source):
    encode_const = lambda word: bytes(word, encoding='ascii') + b'\x00'

    str_const = []
    symbol_table = {}

    bytecode = bytearray()
    metadata = Metadata(str_const, symbol_table)

    branching, labels = {}, {}

    # Rebinding a refrence to a local
    # name means less time wasted on lookup.
    #
    # If the source is very large this leads
    # to more lookups.
    tree = _tree = SYNTAX_BASE

    for tokline in tokenize(source):
        tokend = len(tokline) - 1

        # Begin semantic validation of the line

        for ti, token in enumerate(tokline):
            # satisfy the blueprint then evolve the expression
            dat, tp = token

            # Faster than dict.get in my testing 
            # ">>> (tree[tp] if tp in tree else {})"
            #
            # Note: LYBL approach is the style here
            # since this isnt a threaded process.
            _t, tree = tree, (tree[tp] if tp in tree else {})

            if (not tree and 'eol' not in _t) or (ti == tokend and (tree and 'eol' not in tree)):
                # Ha! the programmer didnt follow the grammatical rules.
                # throw a syntax error and abort.
                raise SyntaxError(f'\n===>\t{tokline},\n===>\t{ti},\n===>\t{token}\n')

            # we are validated lets analyse it

            if tp == _token_string and dat not in str_const:
                str_const.append(dat)

        tree = _tree

        # Analyse the validated line

        if tokline[1][1] == 'let_stmt':
            if tokline[2][0] in symbol_table:
                continue

            symbol_table[tokline[2][0]] = {
                'declared_on': [tokline[0][0]],
            }


        # Construct the bytecode

        ln = int(tokline[0][0])

        if ln in branching:
            for pos in branching[ln]:
                bytecode[pos + 1:pos + 3] = struct.pack('=h', len(bytecode) - pos - 1)

        labels[ln] = pos = len(bytecode)

        print(ln, bytecode)
        print(labels, branching)

        dat, tp = tokline[1]

        if tp == 'let_stmt':
            bytecode += evaluate(metadata, tokline[4:])
            bytecode += _ins_store
            bytecode += struct.pack('=h', metadata.fsymt.index(tokline[2][0]))

        elif tp == 'if_stmt':
            then_stmt = tokline.index(('then', 'then_stmt'))

            _jmp_ln = int(tokline[then_stmt + 2][0])
            _jmp = 0

            bytecode += evaluate(metadata, tokline[2:then_stmt])
            bytecode += _ins_jmpt
            bytecode += struct.pack('=h', -3)

            if _jmp_ln < ln:
                _jmp = labels[_jmp_ln] - len(bytecode) - 2

            elif _jmp_ln not in branching:
                branching[_jmp_ln] = [len(bytecode) - 2]

            else:
                branching[_jmp_ln].append(len(bytecode) - 2)

            bytecode += struct.pack('=h', _jmp)


        elif tp == 'end_stmt':
            bytecode += _ins_return

        elif tp == 'print_stmt':
            bytecode += evaluate(metadata, tokline[2:])
            bytecode += _ins_print

        elif tp == 'goto_stmt':
            _jmp_ln = int(tokline[2][0])
            _jmp = 0

            bytecode += _ins_jmp

            if _jmp_ln < ln:
                _jmp = labels[_jmp_ln] - len(bytecode)

            elif _jmp_ln not in branching:
                branching[_jmp_ln] = [pos]

            else:
                branching[_jmp_ln].append(len(bytecode))

            bytecode += struct.pack('=h', _jmp)

        else:
            bytecode += evaluate(metadata, tokline)

    # TODO: Bytecode optimization

    _str_const = b''.join(encode_const(word) for word in str_const)
    header = bytearray() + (struct.pack('=H', len(_str_const) + 2) + _str_const)

    return header + bytecode + _ins_return

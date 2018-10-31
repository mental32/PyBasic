import shlex
import struct
import pprint

import _pybasic

from pybasic import _bvm_ins

is_string = lambda token: (len(token) > 2 and (token[0], token[-1]) == ('"', '"'))

_reserved = [
    'PRINT',
    'LET',
    'GOTO',
    'END',
    'IF'
]

class Operator:
    __bvm_ops = {
        '+': _bvm_ins['binary_add'],
        '-': _bvm_ins['binary_sub'],
        '*': _bvm_ins['binary_mul'],
        '/': _bvm_ins['binary_div'],
    }

    def __init__(self, data):
        self.data = data

    @property
    def ins(self):
        return self.__bvm_ops[self.data]


class Logic(Operator):
    pass


def _get_concrete(token):
    ttype = _pybasic.type(token)

    if ttype == 'I':
        return int(token)

    elif token in ('!=', '=='):
        return Logic(token)

    elif token in '+-*/':
        return Operator(token)

    elif token in ('!', '=', 'GOTO'):
        return None

    return token

def evaluate(data, tokens):
    flatten = lambda stream: list(p[0] for p in stream)

    expr = bytearray()
    constants, varnames = data

    if not tokens:
        return expr

    types = {_pybasic.type(t) for t in tokens}
    typed_tokens = tuple((t, _pybasic.type(t)) for t in tokens)

    if 'S' not in types and 'V' not in types:
        # Expression appears to be static (containing no refrences)

        # "Using eval is cheating!" 
        # I'm using eval because I can and I know it's absolutely safe!
        # Ergo to not use eval would be the silly answer
        rv = eval(''.join(tokens))
    else:
        stream = iter(reversed(typed_tokens))

        value, ttype = next(stream)
        rv = _get_concrete(value)

    while True:
        if isinstance(rv, int):
            if 255 > rv >= 0:
                expr.append(_bvm_ins['load_byte'])
                expr.extend(struct.pack('=B', rv))
            else:
                expr.append(_bvm_ins['load_long'])
                expr.extend(struct.pack('=l', rv))

        elif isinstance(rv, bool):
            expr.append(_bvm_ins['load_byte'])
            expr.extend(struct.pack('=?', rv))

        elif isinstance(rv, Logic):
            expr.extend(evaluate(data, flatten(stream)))

            if rv.data[0] == '!':
                expr.append(_bvm_ins['cmp'])
                expr.append(_bvm_ins['not'])
            else:
                expr.append(_bvm_ins['cmp'])

        elif isinstance(rv, Operator):
            expr.extend(evaluate(data, flatten(stream)))
            expr.append(rv.ins)

        elif isinstance(rv, str):

            if ttype == 'S':
                expr.append(_bvm_ins['load_const'])

                rv = rv[1:-1]

                if rv not in constants:
                    constants.append(rv)

                expr.append(constants.index(rv))
            else:
                expr.append(_bvm_ins['load_name'])
                expr.append(varnames.index(rv))

        if 'V' not in types:
            break

        try:
            value, ttype = next(stream)
            rv = _get_concrete(value)
        except StopIteration:
            break

    return expr

def tokenize(source):
    _header = bytearray()
    _bytecode = bytearray()
    parsed = {}
    labels = {}
    constants = []
    varnames = []

    # While creating the parsed tokens
    # Extract metadata and perform analysis
    # On the source.
    for line in source.strip().split('\n'):
        row = []

        it = iter(shlex.shlex(line.strip()).get_token, '')
        ln = int(next(it))
        lt = None

        for token in it:
            if token not in _reserved and token not in '!=*/+-{}[]@':
                if is_string(token) and token not in constants:
                    constants.append(token[1:-1])

                elif token.isalpha() and token not in varnames:
                    varnames.append(token)

            elif token == '=' and lt == '!':
                row.pop()
                row.append('!=')
                lt = token
                continue

            row.append(token)
            lt = token

        parsed[ln] = row

    parsed = {k: parsed[k] for k in sorted(parsed)}

    byte_consts = (bytes(c, encoding='utf8') + b'\x00' for c in constants)
    for value in byte_consts:
        _header += value

    _header = (bytearray() + struct.pack('=H', len(_header) + 2)) + _header

    for ln, src in parsed.items():
        labels[ln] = [len(_bytecode)]

        if src[0] == 'LET':
            _bytecode.extend(evaluate((constants, varnames), src[3:]))

            _bytecode.append(_bvm_ins['load_name'])
            _bytecode.append(varnames.index(src[1]))

            _bytecode.append(_bvm_ins['store'])

        elif src[0] == 'IF':
            *e, _g_ln = src[1:]

            _bytecode.extend(evaluate((constants, varnames), e[:-1]))

            _bytecode.append(_bvm_ins['pop_jmp_true'])
            _bytecode.extend(struct.pack('=h', (labels[int(_g_ln)][0]) - len(_bytecode)))

        elif src[0] == 'END':
            _bytecode.append(_bvm_ins['return'])

        elif src[0] == 'PRINT':
            _bytecode.extend(evaluate((constants, varnames), src[1:]))
            _bytecode.append(_bvm_ins['print'])

        elif len(src) >= 3 and src[1] == '=' and src[2] != '=':
            # re assignment
            _bytecode.extend(evaluate((constants, varnames), src[2:]))

            _bytecode.append(_bvm_ins['load_name'])
            _bytecode.append(varnames.index(src[0]))

            _bytecode.append(_bvm_ins['store'])
        else:
            _bytecode.extend(evaluate((constants, varnames), src))

        labels[ln].append(len(_bytecode) - labels[ln][0])

    # pprint.pprint(labels)

    # pprint.pprint((parsed, constants))

    # litecode = list(_bytecode)
    # for ln, data in labels.items():
    #     start, size = data
    #     print(ln, litecode[start:start + size])

    _bytecode = _header + _bytecode

    # pprint.pprint(_bytecode)

    return _bytecode

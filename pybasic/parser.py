import sys
import shlex
import struct

import _pybasic

from pybasic import _bvm_ins

is_string = lambda token: (len(token) > 2 and (token[0], token[-1]) == ('"', '"'))
flatten = lambda stream: list(p[0] for p in stream)

reserved_words = [
    'print',
    'goto',
    'end',
    'let',
    'if'
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


def encode_const(word):
    return bytes(word, encoding='utf8') + b'\x00'

def create_header(data):
    return bytearray() + (struct.pack('=H', len(data) + 2) + data)

def resolve(token):
    ttype = _pybasic.type(token)

    if ttype == 'I':
        return int(token)

    elif token in ('!=', '=='):
        return Logic(token)

    elif token in '+-*/':
        return Operator(token)

    elif token in reserved_words:
        return None

    return token

def evaluate(metadata, tokens):
    expression = bytearray()
    constants, varnames = metadata

    if not tokens:
        return expression

    types = {_pybasic.type(t) for t in tokens}
    ttypes = tuple((t, _pybasic.type(t)) for t in tokens)

    if 'S' not in types and 'V' not in types:
        real = eval(''.join(tokens))
    else:
        stream = iter(reversed(ttypes))
        value, value_type = next(stream)
        real = resolve(value)

    while True:
        if isinstance(real, int):
            if 254 >= real >= 0:
                expression += _bvm_ins['load_byte'].to_bytes(1, sys.byteorder)
                expression += struct.pack('=B', real)
            else:
                expression += _bvm_ins['load_long'].to_bytes(1, sys.byteorder)
                expression += struct.pack('=l', real)

        elif isinstance(real, bool):
            expression += _bvm_ins['load_byte'].to_bytes(1, sys.byteorder)
            expression += struct.pack('=?', real)

        elif isinstance(real, Logic):
            expression += evaluate(metadata, flatten(stream))
            expression += _bvm_ins['cmp'].to_bytes(1, sys.byteorder)

            if real.data[0] == '!':
                expression += _bvm_ins['not'].to_bytes(1, sys.byteorder)

        elif isinstance(real, Operator):
            expression += evaluate(metadata, flatten(stream))
            expression += real.ins.to_bytes(1, sys.byteorder)

        elif isinstance(real, str):
            if value_type == 'S':
                expression += _bvm_ins['load_const'].to_bytes(1, sys.byteorder)
                real = real[1:-1]

                if real not in constants:
                    constants.append(real)

                expression += constants.index(real).to_bytes(1, sys.byteorder)
            else:
                expression += _bvm_ins['load_name'].to_bytes(1, sys.byteorder)
                expression += varnames.index(real).to_bytes(1, sys.byteorder)

        if 'V' not in types:
            break

        try:
            value, value_type = next(stream)
            real = resolve(value)
        except StopIteration:
            break

    return expression

def extract(source):
    constants, varnames = [], []
    parsed = {}

    for line_index, line in enumerate(source.strip().split('\n')):
        row = []

        stream = iter(shlex.shlex(line.strip()).get_token, '')
        ln = next(stream)

        if not ln.isdigit():
            raise SyntaxError('\nln:{0:02}| {src}\n{ptr}^'.format(line_index, src=line, ptr=(len(ln) + 4) * '-'))

        lt = None

        for token in stream:
            if token not in reserved_words and token not in '!=*/+-{}[]@:;#~?.><,\\|\"\'£$%^&*()_`¬':
                if is_string(token) and token not in constants:
                    constants.append(token[1:-1])

                elif token.isalpha() and token not in varnames:
                    varnames.append(token)

            elif (lt, token) == ('!', '='):
                row.pop()
                row.append('!=')
                lt = token
                continue

            lt = token
            row.append(token)

        parsed[ln] = row

    parsed = {int(k): parsed[k] for k in sorted(parsed)}
    return (constants, varnames), parsed

def tokenize(source):
    bytecode = bytearray()
    branching, labels = {}, {}

    metadata, parsed = extract(source)

    header = create_header(b''.join(encode_const(word) for word in metadata[0]))

    for ln, src in parsed.items():
        if ln in branching:
            for pos in branching[ln]:
                bytecode[pos:pos + 2] = struct.pack('=h', pos - len(bytecode))

        labels[ln] = len(bytecode)

        if src[0] == 'let':
            bytecode += evaluate(metadata, src[3:])
            bytecode += _bvm_ins['load_name'].to_bytes(1, sys.byteorder)
            bytecode += metadata[1].index(src[1]).to_bytes(1, sys.byteorder)
            bytecode += _bvm_ins['store'].to_bytes(1, sys.byteorder)

        elif src[0] == 'if':
            *e, _g_ln = src[1:]

            bytecode += evaluate(metadata, e[:-1])
            bytecode += _bvm_ins['jmp_true'].to_bytes(1, sys.byteorder)

            if int(_g_ln) > ln:
                branching.setdefault(int(_g_ln), [])
                branching[int(_g_ln)].append(len(bytecode))
                bytecode += struct.pack('=h', 0)
            else:
                print(labels[int(_g_ln)] - len(bytecode))
                struct.pack('=h', labels[int(_g_ln)] - len(bytecode))

        elif src[0] == 'end':
            bytecode += _bvm_ins['return'].to_bytes(1, sys.byteorder)

        elif src[0] == 'print':
            bytecode += evaluate(metadata, src[1:])
            bytecode += _bvm_ins['print'].to_bytes(1, sys.byteorder)

        elif len(src) >= 3 and src[1] == '=' and src[2] != '=':
            bytecode += evaluate(metadata, srd[2:])
            bytecode += _bvm_ins['load_name'].to_bytes(1, sys.byteorder)
            bytecode += metadata[1].index(src[0]).to_bytes(1, sys.byteorder)
            bytecode += _bvm_ins['store'].to_bytes(1, sys.byteorder)

        else:
            bytecode += evaluate(metadata, src)
    return header + bytecode

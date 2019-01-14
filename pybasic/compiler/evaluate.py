from struct import pack

from .bytecode import (
    _ins_load_long,
    _ins_load_const,
    _ins_load_name,
    _ins_bin,
    _ins_cmp,
)

flatten = lambda stream: list(p for p in stream)


def evaluate(metadata, tokens):
    expr = bytearray()

    if not tokens:
        return expr

    tp_group = {tok[1].split('_')[0] for tok in tokens}

    if tp_group == {'bin', 'integer'}:
        # Constant folding occurs here

        value = eval(''.join(t[0] for t in tokens))

        if type(value) is bool:
            value = int(value)

        tp = {int: 'integer', str: 'string'}[type(value)]
        dat = str(value)

    else:
        stream = iter(tokens)
        dat, tp = next(stream)

    while True:
        if tp == 'integer':
            expr += _ins_load_long
            expr += pack('=l', int(dat))

        elif tp == 'identifier':
            expr += _ins_load_name
            expr += pack('=h', metadata.fsymt.index(dat))

        elif tp == 'string':
            expr += _ins_load_const
            expr += pack('=h', metadata.constants.index(dat))

        elif tp in ('eq_neq', 'eq_double'):
            expr += evaluate(metadata, flatten(stream))
            expr += _ins_cmp

            if tp == 'eq_neq':
                expr += _ins_not

        elif tp in ('bin_add', 'bin_sub', 'bin_mul', 'bin_div'):
            expr += evaluate(metadata, flatten(stream))
            expr += _ins_bin[tp]

        elif tp == 'sep_semicolon':
            expr += evaluate(metadata, flatten(stream))

        try:
            dat, tp = next(stream)
        except (StopIteration, NameError):
            break

    return expr

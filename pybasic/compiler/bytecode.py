from sys import byteorder

from pybasic import _bvm_ins

_b = lambda i: i.to_bytes(1, byteorder)

_ins_load_long  = _b(_bvm_ins['load_long'])
_ins_load_const = _b(_bvm_ins['load_const'])
_ins_load_name  = _b(_bvm_ins['load_name'])
_ins_store      = _b(_bvm_ins['store'])
_ins_print      = _b(_bvm_ins['print'])
_ins_return     = _b(_bvm_ins['return'])
_ins_jmpt       = _b(_bvm_ins['jmp_true'])
_ins_jmp        = _b(_bvm_ins['goto'])
_ins_cmp        = _b(_bvm_ins['cmp'])

_ins_bin_add = _b(_bvm_ins['binary_add'])
_ins_bin_sub = _b(_bvm_ins['binary_sub'])
_ins_bin_mul = _b(_bvm_ins['binary_mul'])
_ins_bin_div = _b(_bvm_ins['binary_div'])

_ins_bin = {
    'bin_add': _ins_bin_add,
    'bin_sub': _ins_bin_sub,
    'bin_mul': _ins_bin_mul,
    'bin_div': _ins_bin_div,
}

def dump():
    for v, o in globals().copy().items():
        if v[:4] == '_ins':
            if type(o) is bytes:
                o = f'0x{o.hex()}'

            print(f'{v}\t{o}')

dump()

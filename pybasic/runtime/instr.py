

class Bytecode(IntEnum):
    Load = 0
    LoadConst = 1
    LoadName = 2
    Store = 3
    Return = 4
    JmpTrue = 5
    JmpFalse = 6
    Goto = 7
    Cmp = 8
    Add = 9
    Sub = 10
    Mul = 11
    Div = 12
    Shl = 13
    Shr = 14
    And = 15
    Xor = 16
    Or = 17
    LoadInt = 18

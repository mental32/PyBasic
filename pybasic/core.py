from _pybasic import ByteCodeInterpreter

from pybasic import _tokenize

class Interpreter(ByteCodeInterpreter):
    def __repr__(self):
        return '<pybasic.Interpreter @ %s>' % hex(id(self)).upper()

from _pybasic import ByteCodeInterpreter

class Interpreter(ByteCodeInterpreter):
    def __repr__(self):
        return '<pybasic.Interpreter @ %s>' % hex(id(self)).upper()

import sys

if '.' not in sys.path:
	sys.path.append('.')

from pybasic import Interpreter

if __name__ == '__main__':
    with Interpreter() as pyb:
        pyb.run()

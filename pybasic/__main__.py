import sys

if '.' not in sys.path:
	sys.path.append('.')

from pybasic import Interpreter

if __name__ == '__main__':
	_pybvm = Interpreter()

	with open(sys.argv[1]) as inf:
		_pybvm.run_source(inf.read())

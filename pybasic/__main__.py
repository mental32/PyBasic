import sys

if '.' not in sys.path:
	sys.path.append('.')

from pybasic import Interpreter

if __name__ == '__main__':
	i = Interpreter()

	print('-' * 20)

	with open('test.bas') as inf:
		for line in inf:
			i.run_simple(line.strip())

    # return Interpreter().run()

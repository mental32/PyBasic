import sys
import argparse

if '.' not in sys.path:
	sys.path.append('.')

from pybasic import Interpreter

def main(args):
	_pybvm = Interpreter()

	if args.file is None:
		return _pybvm.run_repl()

	with open(args.file) as inf:
		source = inf.read().strip()
	_pybvm.run_source(source)

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('file', nargs='?', help='read and run program from file.')
	main(parser.parse_args())

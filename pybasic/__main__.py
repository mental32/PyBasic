import sys
import time
import argparse

if '.' not in sys.path:
    sys.path.append('.')

from pybasic import Interpreter

def main(args):
    _timed = args.time
    _pybvm = Interpreter()

    if args.file is None:
        return pybasic.run_repl(_pybvm)

    with open(args.file) as inf:
        source = inf.read().strip()

    if _timed:
        t1 = time.time()

    try:
        _pybvm.run_source(source)
    except RuntimeError as err:
        sys.exit(err)

    if _timed:
        print(time.time() - t1)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-t', dest='time', action='store_true', help='Output the execution time in seconds')
    parser.add_argument('file', nargs='?', help='read and run program from file.')
    main(parser.parse_args())

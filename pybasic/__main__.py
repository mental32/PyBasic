import sys
import time

import click

from . import compile as _pybasic_compile


class Timer:
    __slots__ = ('dummy', 'start_time', 'laps')

    def __init__(self, dummy: bool = False):
        self.dummy = dummy
        self.start_time = time.monotonic_ns()
        self.laps = []

    def __enter__(self):
        if not self.dummy:
            self.laps.append((time.monotonic_ns(), None))

    def __exit__(self, *_):
        if not self.dummy:
            start, _ = self.laps.pop()
            self.laps.append((start, time.monotonic_ns()))

    @property
    def lap(self):
        if not self.dummy:
            if not self.laps:
                return time.monotonic_ns() - self.start_time
            else:
                start, end = self.laps[-1]
                return end - start


@click.command()
@click.argument('file')
@click.option('--timed', '-t', is_flag=True)
def main(file, timed):
    timer = Timer(dummy=(not timed))

    with open(file) as source_file:
        with timer:
            code = _pybasic_compile(source_file)

        if timed:
            LOGGER.info('Compiled source in %s', timer.lap)

        if False:
            with timer:
                pybasic.exec(code)

        if timed:
            LOGGER.info('Executed code in %s', timer.lap)

if __name__ == '__main__':
    main()

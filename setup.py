from distutils.core import setup, Extension

ext_sources = [fp for files in (glob.glob('%s/*.c' % fp) for path in os.walk('.') for fp in (path[0],)) for fp in files if files]

setup(
    name='pybasic',
    version='0.1.0',
    description='CPython BASIC interpreter',
    ext_modules=[Extension('_pybasic', sources=ext_sources)]
)

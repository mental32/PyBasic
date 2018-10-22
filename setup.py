from distutils.core import setup, Extension

extensions = [
    Extension('_pybasic', sources=[
        'pybasic_c/_pybasic.c',
        'pybasic_c/interpreter.c',
        'pybasic_c/parser.c'
    ])
]

setup(
    name='pybasic',
    version='0.1.0',
    description='CPython BASIC interpreter',
    ext_modules=extensions
)

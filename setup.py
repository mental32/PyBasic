from distutils.core import setup, Extension

extensions = [
    Extension('_pybasic', sources=[
        'pybasic_c/_pybasic.c',
        'pybasic_c/_pyb_bytecode.c',
        'pybasic_c/_pyb_helpers.c',
        'pybasic_c/_pyb_parser.c'
    ])
]

setup(
    name='pybasic',
    version='0.1.0',
    description='CPython BASIC interpreter',
    ext_modules=extensions
)

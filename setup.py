from setuptools import setup

setup(
    name='pybasic',
    version='0.3.0',
    requires=['click'],
    description='CPython BASIC interpreter',
    packages=['pybasic', 'pybasic.compiler', 'pybasic.runtime'],
    zip_safe=False,
)

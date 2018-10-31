# PyBASIC

A (dartmouth) BASIC inspired interpreter written in C/Python.

A list of current known issues:
 - There is no REPL
 - All integers are unsigned bytes
 - But all jump addresses are shorts
 - Division will probably not work
 - Floats don't exist
 - There is 0 Garbage collection (nothing gets free'd)
 - If you get infinite loop, control-c will not work you will have to suspend and kill the process
 - when you exit the python interpreter after you instantiate a pybasic one, due to a bug somewhere with the type refrencing python segfaults.

Enjoy!

## Installing

Compiling from source.
- `pip3 install git+https://github.com/mental32/pybasic`

## Usage

Command line usage for pybasic is pretty much
 - `python -m pybasic [option] ... [file] [args] ...`

## Progress

Working:
 - Jumping
 - Printing
 - Assignment
 - Expressions
 - Flow control
 - Conditional loops

Planned:
 - Builtins
 - I/O

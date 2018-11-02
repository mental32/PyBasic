# PyBASIC

A (dartmouth) BASIC inspired interpreter written in C/Python.

A list of current known issues:
 - There is no REPL
 - All integers are either unsigned bytes or unsigned longs
 - Division will probably not work (currently behaves as floor division due to no floats)
 - Floats don't exist (yet)
 - If you get infinite loop, control-c will not work you will have to suspend and kill the process
 - There is 0 Garbage collection (nothing gets free'd)
 - After doing a lot of arithmetic ops due to a bug somewhere. Python randomly segfaults (possibly due with the lack of a GC).

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

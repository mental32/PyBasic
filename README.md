# PyBasic

A (dartmouth) BASIC inspired interpreter written in C/Python.

## Issues
 - There is no REPL
 - All integers are unsigned longs
 - Division currently behaves as floor division due to no floating point integers
 - If you get infinite loop, you will have to suspend and kill the process
 - There is 0 Garbage collection & nothing gets free'd
 - Python randomly segfaults when exiting, possibly due with the lack of a GC.

Enjoy!

## Examples
Hello world with PyBasic
```basic
00 REM Save this as hello.bas
10 print "Hello world!"
20 end
```
Then run with `python -m pybasic hello.bas`.

It should then produce the following
```
$> python -m pybasic hello.bas
Hello world!
```

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

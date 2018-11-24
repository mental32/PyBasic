# PyBasic

A (dartmouth) BASIC inspired interpreter written in C/Python.

# Index
 - [installing](##Installing)
 - [usage](##Usage)
 - [issues](##Issues)
 - [examples](##Examples)
 - [progress](##Progress)
 - [BVM](##BVM)

## Installing

Compiling from source.
- `pip3 install git+https://github.com/mental32/pybasic`

## Usage

Command line usage for pybasic is pretty much
 - `python -m pybasic [option] ... [file] [args] ...`


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

## BVM

PyBasic runs on a bytecode interepreter commonly refered to as the BVM.

PyBasic's runtime completely takes place in the BVM and its source compilation takes place in Python itself.

BVM Opcodes
| bytecode | name | description |
|----------|------|-------------|
| 0x00     | NOP  | A No-op     |
| 0x01     | RETURN | exit successfully from the BVM |
| 0x02     | STORE_NAME | pop the name then value from the stack and store |
| 0x03     | LOAD_NAME | pop and try loading that name from the varspace |
| 0x04     | LOAD_CONST | pop and load a string constant with that index |
| 0x05     | LOAD_LONG | read a signed long from the bytecode, and store it on the stack |
| 0x06     | BINARY_ADD | pop a, pop b, add a and b, push result |
| 0x07     | BINARY_SUB | pop a, pop b, subtract a from b, push result |
| 0x08     | BINARY_MUL | pop a, pop b, multiply a and b, push result |
| 0x09     | BINARY_DIV | pop a, pop b, divide a by b, push result |
| 0x0A     | CMP | pop a, pop b, check a and b are of the same type and value, push result |
| 0x0B     | NOT | pop a, not its value, push result |
| 0x0C     | JMP_TRUE | pop a, if a is true change the instruction pointer by the next two bytes (short) else skip two bytes |
| 0x0D
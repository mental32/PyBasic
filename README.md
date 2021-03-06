# PyBasic

A (dartmouth) BASIC inspired interpreter written in C/Python.

PyBasic has a C bytecode VM for its runtime and a Python module for its source compiler and runtime interface.

# Index
 - [Installing](#installing)
 - [Usage](#usage)
 - [Issues](#issues)
 - [Examples](#examples)
 - [Progress](#progress)
 - [BVM](#bvm)

## Installing

Compiling from source.
- `pip3 install git+https://github.com/mental32/pybasic`

## Usage

Command line usage for pybasic is pretty much
 - `python -m pybasic [option] ... [file] [args] ...`


## Issues
 - All integers are longs, if you overflow them you will get unexpected behaviour.
 - Division currently behaves as floor division due to no floating point integers.
 - If you get infinite loop, you will have to suspend and kill the process.

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
user@host$ python -m pybasic hello.bas
Hello world!
```

## Progress

Working:
 - [x] Jumping
 - [x] Printing
 - [x] Assignment
 - [x] Expressions
 - [x] Flow control
 - [x] Conditional loops
 - [x] GC

Planned:
 - [ ] Builtins
 - [ ] I/O

## BVM

PyBasic runs on a bytecode interepreter commonly refered to as the BVM.

### BVM Opcodes

| bytecode | name       | description                                                             |
|----------|------------|-------------------------------------------------------------------------|
| 0x00     | NOP        | A No-op                                                                 |
| 0x01     | RETURN     | exit successfully from the BVM                                          |
| 0x02     | STORE_NAME | pop the name then value from the stack and store                        |
| 0x03     | LOAD_NAME  | pop and try loading that name from the varspace                         |
| 0x04     | LOAD_CONST | pop and load a string constant with that index                          |
| 0x05     | LOAD_LONG  | read a signed long from the bytecode, and store it on the stack         |
| 0x06     | BINARY_ADD | pop a, pop b, add a and b, push result                                  |
| 0x07     | BINARY_SUB | pop a, pop b, subtract a from b, push result                            |
| 0x08     | BINARY_MUL | pop a, pop b, multiply a and b, push result                             |
| 0x09     | BINARY_DIV | pop a, pop b, divide a by b, push result                                |
| 0x0A     | CMP        | pop a, pop b, check a and b are of the same type and value, push result |
| 0x0B     | NOT        | pop a, not its value, push result                                       |
| 0x0C     | JMP_TRUE   | pop a, if a is truthy jump by the next short                            |
| 0x0D     | GOTO       | change the instruction pointer by the value of the next short           |
| 0x0E     | PRINT      | exhaust the stack, displaying the items to stdout                        |


### Garbage Collection

Currently the BVM implements a refrence counting garbage collection.

Internally these object refrence counts are modified with DECREF and INCREF macros.
```c
/* Increase an objects refrence count (without any checks) */
#define Object_INCREF(o) o->rc++

/* If the object is not null, increase its refrence count. */
#define Object_XINCREF(o) if (o != NULL) o->rc++

/* Decrease an objects refrence count (without any checks) */
#define Object_DECREF(o) o->rc--

/* If the object is not null, decrease its refrence count. */
#define Object_XDECREF(o) if (o != NULL) o->rc--

/* If decreasing the objects refrence count will cause it to be forgotten, it is free'd. */
#define Object_FDECREF(o) if (!--o->rc) Object_free(o)

/* Chains Object_XDECREF (null check) and Object_FDECREF (free if forgotten). */
#define Object_UDECREF(o) if (o != NULL && !(--o->rc)) Object_Free(o)
```

## Bytecode

The bytecode has a simple structure consisting of: the header, constants pool and bytecode.

### The bytecode header

The header is located from bits 0 to 32.<br>
The first 16 bits declare the size of the constants pool (by members)<br>
and the last 16 bits dictates the limit of stored variables.

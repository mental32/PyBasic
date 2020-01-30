# PyBasic
## A GW BASIC compiler toolchain written in Python.


## Index

 - [Abstract](#Abstract)
 - [Installing](#installing)
 - [Usage](#usage)
 - [Examples](#examples)

## Abstract

### Features

 - Source to source transpiling from BASIC to Python
 - CPython codeobjects as a compilation target
 - z80 *(and friends)* as a compilation target

## Installing

Compiling from source.

- `pip3 install git+https://github.com/mental32/pybasic`

## Usage

Command line usage for pybasic is pretty much

 - `$ pybasic [option] ... [file] [args] ...`

### Examples

```BASIC
10 PRINT "Hello, World!"
20 GOTO 10
```

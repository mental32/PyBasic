#ifndef _BASIC_BYTECODE_H
#define _BASIC_BYTECODE_H

enum BYTECODE {
    _INS_NOP,
    _INS_RETURN,

    _INS_STORE_NAME,

    _INS_LOAD_NAME,
    _INS_LOAD_CONST,

    _INS_LOAD_LONG,  // 4 bytes
    _INS_LOAD_SHORT, // 2 bytes
    _INS_LOAD_BYTE,  // 1 byte

    _INS_BINARY_ADD,
    _INS_BINARY_SUB,
    _INS_BINARY_MUL,
    _INS_BINARY_DIV,

    _INS_CMP,
    _INS_NOT,

    _INS_JMP_TRUE,

    _INS_GOTO,
    _INS_PRINT,
};

#endif

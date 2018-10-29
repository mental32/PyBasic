#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "ins.h"

/*

*/
enum ObjectType {
    _obj_tp_long,
    _obj_tp_byte,
    _obj_tp_str,
};

typedef struct {
    uint8_t tp;
    void *ptr;
} Object;

/*

*/
typedef struct {
    uint8_t *ip;
    uint8_t _running;

    size_t insc;
    size_t sp;

    Object **stack;
} VMState;

static const uint16_t PEEK_CONST(uint8_t *bytecode, size_t index) {
    const char *string = bytecode;
    size_t pos = 0;

    uint8_t offset = 0;
    uint8_t size = 0;

    while (1) {
        if (pos == index) {
            while (*string != ':') {
                string++;
                size++;
            }

            break;
        }

        while (*string != ':') {
            string++;
            offset++;
        }

        pos++;
    }

    return (((uint16_t) offset) << 8) | ((uint16_t) size);
}

static inline char *READ_CONST(uint8_t *bytecode, size_t index) {
    char *dest;
    uint16_t data = PEEK_CONST(bytecode, index);
    dest = (char *) malloc(sizeof(char) * ((data & 0xff) + 1));
    memcpy(dest, bytecode + (data >> 8), (data & 0xff));
    dest[(data & 0xff)] = '\0';
    return dest;
}

static inline Object *NewObject(uint8_t tp, void *ptr) {
    Object *obj = malloc(sizeof(Object));
    obj->tp = tp;
    obj->ptr = ptr;
    return obj;
}

/*

*/
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size) {
    int _status = 0;

    if (bytecode_size < 1) {
        return _status;
    }

    VMState *vm = (VMState *) malloc(sizeof(VMState));
    vm->_running = 1;
    vm->insc = 0;

    vm->stack = (Object **) malloc((size_t) 20);
    vm->sp = 0;

    vm->ip = bytecode;

    // Read size of constants pool.
    short DATA_SIZE = *((short*) (vm->ip));

    // Then jump ahead of it.
    vm->ip += DATA_SIZE;

    while (vm->_running) {
        vm->ip++;

        // Explicitly handle a nop instruction (0).
        // Don't increment the instruction counter (insc)
        // Just loop around for the next instruction.
        if (!*vm->ip) {
            continue;
        }

        vm->insc++;

        switch (*vm->ip) {
            case _INS_RETURN: {
                vm->_running = 0;
                _status = 0;
                break;
            }

            case _INS_STORE_NAME: {
            }

            case _INS_LOAD_NAME: {

            }

            case _INS_LOAD_CONST: {
                vm->stack[vm->sp++] = NewObject(_obj_tp_str, bytecode + (PEEK_CONST(bytecode, *vm->ip) >> 8));
            }

            case _INS_LOAD_LONG: {

            }

            case _INS_LOAD_SHORT: {

            }

            case _INS_LOAD_BYTE: {
                vm->stack[vm->sp++] = NewObject(_obj_tp_byte, ((uint8_t *) (vm->ip++)));
            }

            case _INS_BINARY_ADD: {

            }

            case _INS_BINARY_SUB: {

            }

            case _INS_BINARY_MUL: {

            }

            case _INS_BINARY_DIV: {

            }

            case _INS_CMP: {

            }

            case _INS_NOT: {

            }

            case _INS_POP_JMP_TRUE: {

            }

            case _INS_GOTO: {
                vm->ip += *((short*) (vm->ip + 1));
                break;
            }
        }
    }

    return _status;
}

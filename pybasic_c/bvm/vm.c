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

    char **data;
} VMState;

static inline void pushstack(VMState *vm, Object *item) {
    vm->stack[vm->sp++] = item;
}

static inline Object *popstack(VMState *vm) {
    Object *item = vm->stack[--vm->sp];
    vm->stack[vm->sp] = NULL;
    return item;
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

    vm->data = (char **) malloc((size_t) 20);

    vm->stack = (Object **) malloc((size_t) 20);
    vm->sp = 0;

    vm->ip = bytecode;

    // Read size of constants pool.
    short dp = 0, data_size = *((short*) (vm->ip));
    char *p = vm->ip + 2, *end = (char *)(data_size + vm->ip + 2);

    while (p < end) {
        vm->data[dp++] = p;
        p += strlen(p) + 1;
    }

    // Then jump ahead of it.
    vm->ip += data_size;

    while (vm->_running) {
        switch (*vm->ip) {
            case _INS_RETURN: {
                vm->_running = 0;
                _status = 0;
                break;
            }

            case _INS_STORE_NAME: {
                Object *name = popstack(vm);
                Object *value = popstack(vm);
                break;
            }

            case _INS_LOAD_NAME: {
                vm->stack[vm->sp++] = NewObject(_obj_tp_byte, (uint8_t *) vm->ip);
                vm->ip++;
                break;
            }

            case _INS_LOAD_CONST: {
                vm->ip++;
                vm->stack[vm->sp++] = NewObject(_obj_tp_str, vm->data[*vm->ip]);
                break;
            }

            case _INS_LOAD_LONG: {
                break;
            }

            case _INS_LOAD_SHORT: {
                break;
            }

            case _INS_LOAD_BYTE: {
                vm->stack[vm->sp++] = NewObject(_obj_tp_byte, ((uint8_t*) (vm->ip++)));
                break;
            }

            case _INS_BINARY_ADD: {
                break;
            }

            case _INS_BINARY_SUB: {
                break;
            }

            case _INS_BINARY_MUL: {
                break;
            }

            case _INS_BINARY_DIV: {
                break;
            }

            case _INS_CMP: {
                break;
            }

            case _INS_NOT: {
                break;
            }

            case _INS_POP_JMP_TRUE: {
                break;
            }

            case _INS_GOTO: {
                vm->ip += *((short*) (vm->ip + 1));
                break;
            }
        }

        vm->ip++;
        vm->insc++;
    }

    return _status;
}

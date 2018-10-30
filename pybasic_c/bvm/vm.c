#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "ins.h"

enum ObjectType {
    _obj_tp_str = 1,

    _obj_tp_u8,
    _obj_tp_u16,
    _obj_tp_u32,

    _obj_tp_generic_ref,
    _obj_tp_generic_int,
    _obj_tp_generic_str,
    _obj_tp_generic_flt,

    _obj_tp_literal = 128,
};

#define BYTE  (_obj_tp_u8  | _obj_tp_generic_int)
#define SHORT (_obj_tp_u16 | _obj_tp_generic_int)
#define LONG  (_obj_tp_u32 | _obj_tp_generic_int)

#define _BYTE  (BYTE  | _obj_tp_literal)
#define _SHORT (SHORT | _obj_tp_literal)
#define _LONG  (LONG  | _obj_tp_literal)

typedef struct {
    uint8_t tp;
    void *ptr;
} Object;

/*

*/
typedef struct {
    uint8_t *ip;
    uint8_t _running;

    char **data;

    size_t insc;
    size_t sp;

    Object **stack;
    Object **varspace;
} VMState;

static void PrintObject(Object *obj) {
    uint8_t tp = obj->tp;

    if (tp == _obj_tp_str) {
        printf("%s", (char*)obj->ptr);
    } else if (tp == BYTE) {
        printf("%d", *((uint8_t*)obj->ptr));
    } else if (tp == _BYTE) {
        printf("%d", ((uint8_t*)obj->ptr));
    }

    return;
}

static inline void pushstack(VMState *vm, Object *item) {
    vm->stack[vm->sp++] = item;
}

static inline Object *popstack(VMState *vm) {
    Object *item = vm->stack[--vm->sp];
    vm->stack[vm->sp] = NULL;
    return item;
}

static inline Object *resolve(VMState *vm, Object *ref) {
    if (ref->tp == _obj_tp_generic_ref) {
        return vm->varspace[*((uint8_t *)ref->ptr)];
    } else {
        return ref;
    }
}

static inline Object *NewObject(uint8_t tp, void *ptr) {
    Object *obj = malloc(sizeof(Object));
    obj->tp = tp;
    obj->ptr = ptr;
    return obj;
}

static inline Object *RebaseObject(Object *obj, uint8_t tp, void *ptr) {
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

    vm->ip = bytecode;
    vm->_running = 1;

    vm->data =     (char **)   malloc((size_t) 20);

    vm->insc = 0;
    vm->sp = 0;

    vm->stack       = (Object **) malloc((size_t) 20);
    vm->varspace    = (Object **) malloc((size_t) 20);

    // Read size of constants pool.
    short dp = 0, data_size = *((short*) (vm->ip));
    char *p = (char *)vm->ip + 2, *end = (char *)(data_size + vm->ip + 2);

    while (p < end) {
        vm->data[dp++] = p;
        p += strlen(p) + 1;
    }

    p = NULL;

    // Then jump ahead of it.
    vm->ip += data_size;

    while (vm->_running) {
        // printf("! %d\n", *vm->ip);

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
                pushstack(vm, NewObject(_obj_tp_generic_ref, (uint8_t *) (++vm->ip)));
                break;
            }

            case _INS_LOAD_CONST: {
                pushstack(vm, NewObject(_obj_tp_str, vm->data[*++vm->ip]));
                break;
            }

            case _INS_LOAD_LONG: {
                pushstack(vm, NewObject(LONG, ((long*) (++vm->ip))));
                break;
            }

            case _INS_LOAD_SHORT: {
                pushstack(vm, NewObject(SHORT, ((short*) (++vm->ip))));
                break;
            }

            case _INS_LOAD_BYTE: {
                pushstack(vm, NewObject(BYTE, ((uint8_t*) (++vm->ip))));
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

            case _INS_PRINT: {
                while (vm->sp-- != 0) {
                    Object *item = resolve(vm, vm->stack[vm->sp]);
                    vm->stack[vm->sp] = NULL;
                    PrintObject(item);
                }
                printf("\n");
                vm->sp = 0;
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

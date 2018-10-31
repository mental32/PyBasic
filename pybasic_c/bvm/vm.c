#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "ins.h"
#include "types.h"

typedef struct {
    uint8_t tp;
    void *ptr;
} Object;

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

    if (tp == _obj_tp_generic_str) {
        printf("%s", (char*)obj->ptr);
    } else if (tp == BYTE) {
        printf("%d", *((uint8_t*)obj->ptr));
    } else if (tp == _BYTE || tp == _BOOL) {
        printf("%d", ((uint8_t*)obj->ptr));
    } else {
        printf("\n(nil) : Attempted to display an unrecognised value (%d).\n", tp);
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

static inline Object *resolve_once(VMState *vm, Object *ref) {
    if (ref->tp == _obj_tp_generic_ref) {
        return vm->varspace[*((uint8_t *)ref->ptr)];
    } else {
        return ref;
    }
}

static inline Object *resolve(VMState *vm, Object *ref) {
    Object *obj = ref;

    while (obj->tp == _obj_tp_generic_ref) {
        obj = resolve_once(vm, obj);
    }

    return obj;
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

static inline int TypeCheckExact(Object *a, Object *b) {
    if (IS_STR(a) && IS_STR(b)) {
        return _obj_tp_generic_str;
    } else if (IS_INT(a) && IS_INT(b)) {
        return _obj_tp_generic_int;
    }

    return 0;
}

static inline int CompareObjects(Object *a, Object *b) {
    int tp = TypeCheckExact(a, b);

    if (tp & _obj_tp_generic_int) {
        return GetIntValue(a) == GetIntValue(b);
    }

    else if (tp & _obj_tp_generic_str) {
        return !strcmp(((char*)a), ((char*)b));
    }

    return 0;
}

int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size) {
    int _status = 0;

    if (bytecode_size < 1) {
        return _status;
    }

    VMState *vm = (VMState *) malloc(sizeof(VMState));

    vm->ip = bytecode;
    vm->_running = 1;

    vm->data = (char **)malloc((size_t) 20);

    vm->insc = 0;
    vm->sp = 0;

    vm->stack = (Object **)malloc((size_t) 20);
    vm->varspace = (Object **)malloc((size_t) 20);

    // Read size of constants pool.
    short dp = 0, data_size = *((short*) (vm->ip));
    char *p = (char *)vm->ip + 2, *end = (char *)(data_size + vm->ip + 2);

    while (p < end) {
        vm->data[dp++] = p;
        p += strlen(p) + 1;
    }

    p = NULL;

    // Then jump ahead of it.
    vm->ip += data_size - 1;

    while (vm->_running) {
        vm->ip++;
        vm->insc++;

        // printf("! %d\t:: %p\t:: %d\n", *vm->ip, vm->ip, vm->sp);

        switch (*vm->ip) {
            case _INS_RETURN: {
                vm->_running = 0;
                _status = 0;
                break;
            }

            case _INS_STORE_NAME: {
                Object *name = popstack(vm);
                Object *value = popstack(vm);

                vm->varspace[*((uint8_t *)name->ptr)] = value;
                break;
            }

            case _INS_LOAD_NAME: {
                pushstack(vm, NewObject(_obj_tp_generic_ref, (uint8_t *) (++vm->ip)));
                break;
            }

            case _INS_LOAD_CONST: {
                pushstack(vm, NewObject(STRING, vm->data[*++vm->ip]));
                break;
            }

            case _INS_LOAD_LONG: {
                pushstack(vm, NewObject(LONG, ((long*) (++vm->ip))));
                vm->ip += sizeof(long) - 1;
                break;
            }

            case _INS_LOAD_SHORT: {
                pushstack(vm, NewObject(SHORT, ((short*) (++vm->ip))));
                vm->ip += sizeof(short) - 1;
                break;
            }

            case _INS_LOAD_BYTE: {
                pushstack(vm, NewObject(BYTE, ((uint8_t*) (++vm->ip))));
                break;
            }

            case _INS_BINARY_ADD: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & _obj_tp_generic_int && b->tp & _obj_tp_generic_int) {
                    RebaseObject(_ref_a, _BYTE, GetIntValue(a) + GetIntValue(b));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_SUB: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & _obj_tp_generic_int && b->tp & _obj_tp_generic_int) {
                    RebaseObject(_ref_a, _BYTE, GetIntValue(a) - GetIntValue(b));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_MUL: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & _obj_tp_generic_int && b->tp & _obj_tp_generic_int) {
                    RebaseObject(_ref_a, _BYTE, GetIntValue(a) * GetIntValue(b));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_DIV: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & _obj_tp_generic_int && b->tp & _obj_tp_generic_int) {
                    RebaseObject(_ref_a, _BYTE, GetIntValue(a) / GetIntValue(b));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_CMP: {
                Object *left = popstack(vm);
                Object *right = popstack(vm);

                pushstack(vm, NewObject(_BOOL, CompareObjects(left, right)));
                break;
            }

            case _INS_NOT: {
                Object *_ref_obj = popstack(vm);
                Object *obj = resolve(vm, _ref_obj);

                if (obj->tp == _BOOL) {
                    pushstack(vm, RebaseObject(_ref_obj, _BOOL, !(uint8_t)obj->ptr));
                }

                break;
            }

            case _INS_POP_JMP_TRUE: {
                Object *left = popstack(vm);
                Object *right = popstack(vm);

                if (CompareObjects(left, right)) {
                    vm->ip += *((short*) (vm->ip + 1));
                } else {
                    printf("NO POP JMP\n");
                    vm->ip += sizeof(short);
                }

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

            default: {
                _status = 1;
                vm->_running = 0;
            }
        }
    }

    return _status;
}

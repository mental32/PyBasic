#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <string.h>

#include "ins.h"
#include "obj.h"
#include "types.h"

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
    if (!obj) {
        return;
    }

    switch(obj->tp) {
        case generic_str: {
            printf("%s", (char*)obj->ptr);
            break;
        }

        case BYTE: {
            printf("%hhd", *((uint8_t*)obj->ptr));
            break;
        }

        case _BYTE: {
            printf("%d", ((uint8_t)obj->ptr));
            break;
        }

        case _LONG: {
             printf("%lu", ((unsigned long)obj->ptr));
             break;
        }

        default: {
            printf("{(nil)?(%d)}", obj->tp);
        }
    }
}

static inline int pushstack(VMState *vm, Object *item) {
    if (!item || !vm) {
        return 0;
    } else {
        vm->stack[vm->sp++] = item;
    }

    return 1;
}

static inline Object *popstack(VMState *vm) {
    if (!vm) {
        return NULL;
    }

    Object *item = vm->stack[--vm->sp];
    vm->stack[vm->sp] = NULL;
    return item;
}

static inline Object *resolve(VMState *vm, Object *ref) {
    Object *obj = ref;

    while (obj->tp == generic_ref) {
        obj = vm->varspace[*((uint8_t*)obj->ptr)];
    }

    return obj;
}

static inline int GetIntValue(Object *obj) {
    if (obj->tp == _BYTE) {
        return (uint8_t)obj->ptr;
    } else if (obj->tp == _LONG) {
        return (long)obj->ptr;
    } else {
        return *(uint8_t*)obj->ptr;
    }
}

static inline uint8_t ObjectIsTrue(Object *obj) {
    if (IS_INT(obj)) {
        return (uint8_t)(GetIntValue(obj) != 0);
    } else if (IS_STR(obj)) {
        return (uint8_t)(strlen((char*)obj->ptr) != 0);
    }
}

static inline void TailorInteger(Object *o, long value) {
    if (value > 0 && value < 255) {
        RebaseObject(o, _BYTE, (void*)value);
    } else {
        RebaseObject(o, _LONG, (void*)value);
    }
}

static inline int TypeCheckExact(Object *a, Object *b) {
    if (a->tp & generic_str && b->tp & generic_str) {
        return generic_str;
    } else if (IS_INT(a) && IS_INT(b)) {
        return generic_int;
    }

    return 0;
}

static inline int CompareObjects(Object *a, Object *b) {
    int tp = TypeCheckExact(a, b);

    if (tp & generic_int) {
        return GetIntValue(a) == GetIntValue(b);
    }

    else if (tp & generic_str) {
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

    // Map the constants pool to vm->data
    // This is for a lookup speed of O(n)
    // When refrencing constants.
    char *p = (char *)vm->ip + 2, *end = (char *)(data_size + vm->ip + 2);

    while (p < end) {
        vm->data[dp++] = p;
        p += strlen(p) + 1;
    }

    p = NULL;

    // Then jump ahead of it.
    vm->ip += data_size - 1;

    while (vm->_running && !((size_t)(vm->ip - bytecode) >= bytecode_size)) {
        vm->ip++;
        vm->insc++;

        // printf("! %d\t:: %p\t:: %ld\n", *vm->ip, vm->ip, vm->sp);

        switch (*vm->ip) {
            case _INS_RETURN: {
                vm->_running = 0;
                _status = 0;
                break;
            }

            case _INS_STORE_NAME: {
                Object *name = popstack(vm);
                Object *value = popstack(vm);

                vm->varspace[*((uint8_t *)name->ptr)] = resolve(vm, value);
                break;
            }

            case _INS_LOAD_NAME: {
                pushstack(vm, NewObject(generic_ref, (uint8_t *) (++vm->ip)));
                break;
            }

            case _INS_LOAD_CONST: {
                pushstack(vm, NewObject(STRING, vm->data[*++vm->ip]));
                break;
            }

            case _INS_LOAD_LONG: {
                pushstack(vm, NewObject(LONG, ((long*) (++vm->ip))));
                vm->ip += 3;
                break;
            }

            case _INS_LOAD_SHORT: {
                pushstack(vm, NewObject(SHORT, ((short*) (++vm->ip))));
                vm->ip += 1;
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

                if (a->tp & generic_int && b->tp & generic_int) {
                    TailorInteger(_ref_a, (long)(GetIntValue(a) + GetIntValue(b)));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_SUB: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    TailorInteger(_ref_a, (long)(GetIntValue(a) - GetIntValue(b)));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_MUL: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    TailorInteger(_ref_a, (long)(GetIntValue(a) * GetIntValue(b)));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_DIV: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    TailorInteger(_ref_a, (long)(GetIntValue(a) / GetIntValue(b)));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_CMP: {
                if (vm->sp == 1) {
                    pushstack(vm, NewObject(BYTE, ObjectIsTrue(resolve(vm, popstack(vm)))));
                } else {
                    pushstack(vm, NewObject(BYTE, CompareObjects(resolve(vm, popstack(vm)), resolve(vm, popstack(vm)))));
                }

                break;
            }

            case _INS_NOT: {
                Object *_ref_obj = popstack(vm);
                Object *obj = resolve(vm, _ref_obj);

                if (obj->tp == BYTE) {
                    pushstack(vm, RebaseObject(_ref_obj, BYTE, !(uint8_t)obj->ptr));
                }

                else if (IS_INT(obj)) {
                    pushstack(vm, NewObject(BYTE, !GetIntValue(obj)));
                }

                else if (IS_STR(obj)) {
                }

                break;
            }

            case _INS_JMP_TRUE: {
                Object *obj = popstack(vm);

                if (IS_INT(obj) && GetIntValue(obj)) {
                    vm->ip += *((short*) (vm->ip + 1));
                } else if ((bytecode_size - (vm->ip - bytecode)) >= 2) {
                    vm->ip += sizeof(short);
                }

                break;
            }

            case _INS_PRINT: {
                while (vm->sp--) {
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

    if (vm->_running) {
        _status = 2;
    }

    return _status;
}

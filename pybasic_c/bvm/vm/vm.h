#ifndef _BASIC_VMSTATE_H
#define _BASIC_VMSTATE_H

typedef struct {
    uint8_t *ip;
    uint8_t _running;

    char **data;

    size_t insc;
    size_t sp;

    Object **stack;
    Object **varspace;
} VMState;

inline int VMState_pushstack(VMState *vm, Object *item) {
    if (!item || !vm) {
        return 0;
    } else {
        vm->stack[vm->sp++] = item;
    }

    return 1;
}

inline Object *VMState_popstack(VMState *vm) {
    if (!vm) {
        return NULL;
    }

    Object *item = vm->stack[--vm->sp];
    vm->stack[vm->sp] = NULL;
    return item;
}

inline Object *VMState_resolve_ref(VMState *vm, Object *ref) {
    Object *obj = ref;

    while (obj->tp == generic_ref) {
        obj = vm->varspace[*((uint8_t*)obj->ptr)];
    }

    return obj;
}

#endif

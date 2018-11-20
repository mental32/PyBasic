#ifndef _BASIC_VMSTATE_H
#define _BASIC_VMSTATE_H

#include <stddef.h>

#include "../lib/status.h"
#include "../lib/object/base.h"

typedef struct {
    uint8_t *ip;
    uint8_t _running;
    uint8_t __state;

    char **data;

    size_t insc;
    size_t sp;

    Object **stack;
    Object **varspace;
} VMState;

typedef struct {

} ByteCodeHeader;

VMState *VirtualMachine_new(uint8_t *bytecode);
int VirtualMachine_init(VMState *vm);
int VirtualMachine_free(VMState *vm);

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

#ifndef _VMSTATE_H
#define _VMSTATE_H

#include "vm.h"

#define jump(vm) vm->ip += *((short*) (vm->ip + 1))
#define skip(vm) vm->ip += sizeof(short)

/*
push an Object onto the VM stack.

Parameters
----------
 VMState *vm - The VMState to use.
 Objact *item - The Object to push onto the stack.
*/
inline int VMState_pushstack(VMState *vm, Object *item) {
    if (!item || !vm) {
        return 0;
    } else {
        vm->stack[vm->sp++] = item;

        // Object is now being refrenced by the stack
        // Gotta increase the refrence count
        Object_INCREF(item);
    }

    return 1;
}

/*
pop an Object off the stack.

Parameters
----------
 VMState *vm - The VMState stack to use.
*/
inline Object *VMState_popstack(VMState *vm) {
    if (!vm) {
        return NULL;
    }

    Object *item = vm->stack[--vm->sp];
    vm->stack[vm->sp] = NULL;

    // Lower the refrence count of the item we just popped off
    // since its no longer being refrenced by in the stack.
    Object_DECREF(item);

    return item;
}

inline Object *VMState_resolve(VMState *vm, Object *ref) {
    Object *obj = ref;

    while (obj->tp == generic_ref) {
        obj = vm->varspace[(long)obj->ptr];
    }

    return obj;
}

#define store(name, value) vm->varspace[name] = VMState_resolve(vm, value)

#endif

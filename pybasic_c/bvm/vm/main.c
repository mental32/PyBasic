#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "../lib/object.h"
#include "../lib/prelude.h"

#include "ins.h"
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

/*
Entry point for the VM.
At this point its basically one function with a huge switch case statement.

Parameters
----------
 uint8_t *bytecode - A pointer to the base of the bytecode that the VM should read.
 size_t bytecode_size - The size of the bytecode.
*/
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size)
{
    int _status = OK;

    if (bytecode_size < 2) {
        return OK;
    }


    #if __BVM_DEBUG
    printf("! Creating VMState instance.\n");
    #endif

    VMState *vm = VirtualMachine_new(bytecode);

    #define resolve(o) VMState_resolve(vm, o)
    #define stack_item() vm->stack[vm->sp - 1]

    #if __BVM_DEBUG
    printf("! Initializing VMState...");
    #endif

    if (!VirtualMachine_init(vm)) {
        return PANIC;
    }

    #if __BVM_DEBUG
    printf("OK\n! Entering VMState loop\n");
    #endif

    vm->_running = 1;

    while (vm->_running && !((size_t)(vm->ip - bytecode) >= bytecode_size)) {
        vm->ip++;
        vm->insc++;

        #if __BVM_DEBUG
        printf("!-ins=%d :: ip=%p :: sp=%ld\n", *vm->ip, vm->ip, vm->sp);
        #endif

        switch (*vm->ip) {
            case _INS_NOP: {
                break;
            }

            case _INS_RETURN: {
                vm->_running = 0;
                _status = OK;
                break;
            }

            case _INS_STORE_NAME: {
                Object *value = VMState_popstack(vm);
                vm->varspace[*(short*)(vm->ip + 1)] = VMState_resolve(vm, value);
                Object_INCREF(value);
                vm->ip += sizeof(short);
                break;
            }

            case _INS_LOAD_NAME: {
                VMState_pushstack(vm, Object_New(generic_ref, (void*) (*(short*)(++vm->ip))));

                #if __BVM_DEBUG
                printf("! |LOAD_NAME( %d )\n", (short)stack_item()->ptr);
                #endif

                break;
            }

            case _INS_LOAD_CONST: {
                VMState_pushstack(vm, Object_String(vm->const_pool[*((uint8_t*)(++vm->ip))]));

                #if __BVM_DEBUG
                printf("! |LOAD_CONST( %d => \"%s\")\n", *((uint8_t*)(vm->ip)), (char*)stack_item()->ptr);
                #endif

                break;
            }

            case _INS_LOAD_LONG: {
                VMState_pushstack(vm, Object_Integer(*(long*) (vm->ip + 1)));
                vm->ip += sizeof(long);

                #if __BVM_DEBUG
                printf("! |LOAD_LONG( %d )\n", (long)stack_item()->ptr);
                #endif

                break;
            }

            case _INS_BINARY_ADD: {
                Object *a = VMState_resolve(vm, VMState_popstack(vm));
                Object *b = VMState_resolve(vm, VMState_popstack(vm));

                #if __BVM_DEBUG
                printf("! |BINARY_ADD ( %ld + %ld = %ld )\n", (long)a->ptr, (long)b->ptr, (long)a->ptr + (long)b->ptr);
                #endif

                VMState_pushstack(vm, Object_Integer((long)a->ptr + (long)b->ptr));
                break;
            }

            case _INS_BINARY_SUB: {
                Object *a = VMState_resolve(vm, VMState_popstack(vm));
                Object *b = VMState_resolve(vm, VMState_popstack(vm));

                #if __BVM_DEBUG
                printf("! |BINARY_SUB ( %ld - %ld = %ld )\n", (long)a->ptr, (long)b->ptr, (long)a->ptr - (long)b->ptr);
                #endif

                VMState_pushstack(vm, Object_Integer((long)a->ptr - (long)b->ptr));
                break;
            }

            case _INS_BINARY_MUL: {
                Object *a = VMState_resolve(vm, VMState_popstack(vm));
                Object *b = VMState_resolve(vm, VMState_popstack(vm));

                #if __BVM_DEBUG
                printf("! |BINARY_MUL ( %ld * %ld = %ld )\n", (long)a->ptr, (long)b->ptr, (long)a->ptr * (long)b->ptr);
                #endif

                VMState_pushstack(vm, Object_Integer((long)a->ptr * (long)b->ptr));
                break;
            }

            case _INS_BINARY_DIV: {
                break;
            }

            case _INS_CMP: {
                if (vm->sp == 1) {
                    Object *obj = VMState_resolve(vm, VMState_popstack(vm));
                    VMState_pushstack(vm, Object_Bool(Object_bool(obj)));
                } else {
                    Object *left = VMState_resolve(vm, VMState_popstack(vm));
                    Object *right = VMState_resolve(vm, VMState_popstack(vm));

                    VMState_pushstack(vm, Object_Bool(Object_Compare(left, right)));
                }

                break;
            }

            case _INS_NOT: {
                Object *obj = VMState_resolve(vm, VMState_popstack(vm));
                VMState_pushstack(vm, Object_Bool(!Object_bool(obj)));
                break;
            }

            case _INS_JMP_TRUE: {
                Object *obj = VMState_popstack(vm);

                if (Object_bool(obj)) {
                    jump(vm);
                } else if ((bytecode_size - (vm->ip - bytecode)) >= 2){
                    skip(vm);
                }

                break;
            }

            case _INS_PRINT: {
                while (vm->sp--) {
                    Object *item = VMState_resolve(vm, vm->stack[vm->sp]);
                    vm->stack[vm->sp] = NULL;
                    Object_print(item);
                    Object_UDECREF(item);
                }
                vm->sp = 0;
                printf("\n");
                break;
            }

            case _INS_GOTO: {
                vm->ip += *((short*) (vm->ip + 1));
                break;
            }

            default: {
                _status = PANIC;
                vm->_running = 0;
            }
        }
    }

    if (vm->_running)
    {
        err("VM exited while running.\n");
        _status = PANIC;
    }

    VirtualMachine_free(vm);

    return _status;
}

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
        obj = vm->varspace[*((uint8_t*)obj->ptr)];
    }

    return obj;
}

#define store(name, value) vm->varspace[*((uint8_t *)name->ptr)] = VMState_resolve(vm, value)

/*
Entry point for the VM.
At this point its basically one function with a huge switch case statement.
*/
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size)
{
    int _status = OK;

    if (bytecode_size < 2) {
        return OK;
    }

    VMState *vm = VirtualMachine_new(bytecode);
    
    if (!VirtualMachine_init(vm)) {
        return PANIC;
    }

    vm->_running = 1;

    while (vm->_running && !((size_t)(vm->ip - bytecode) >= bytecode_size)) {
        vm->ip++;
        vm->insc++;

        #if __BVM_DEBUG
        printf("! ins=%d\t:: ip=%p\t:: sp=%ld\n", *vm->ip, vm->ip, vm->sp);
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
                Object *name = VMState_popstack(vm);
                Object *value = VMState_popstack(vm);

                store(name, value);
                Object_INCREF(value);
                break;
            }

            case _INS_LOAD_NAME: {
                VMState_pushstack(vm, Object_New(generic_ref, (uint8_t *) (++vm->ip)));
                break;
            }

            case _INS_LOAD_CONST: {
                VMState_pushstack(vm, Object_String(vm->const_pool[*++vm->ip]));
                break;
            }

            case _INS_LOAD_LONG: {
                VMState_pushstack(vm, Object_Integer(*(long*) (vm->ip + 1)));
                vm->ip += sizeof(*((long*) (vm->ip + 1)));
                break;
            }

            case _INS_BINARY_ADD: {
                Object *_ref_a = VMState_popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, VMState_popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    // TailorInteger(_ref_a, (long)(GetIntValue(a) + GetIntValue(b)));
                    VMState_pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_SUB: {
                Object *_ref_a = VMState_popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, VMState_popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    // TailorInteger(_ref_a, (long)(GetIntValue(a) - GetIntValue(b)));
                    VMState_pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_MUL: {
                Object *_ref_a = VMState_popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, VMState_popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    // TailorInteger(_ref_a, (long)((long)GetIntValue(a) * (long)GetIntValue(b)));
                    VMState_pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_DIV: {
                Object *_ref_a = VMState_popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, VMState_popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    // TailorInteger(_ref_a, (long)(GetIntValue(a) / GetIntValue(b)));
                    VMState_pushstack(vm, _ref_a);
                }

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
        _status = PANIC;
    }

    VirtualMachine_free(vm);

    return _status;
}

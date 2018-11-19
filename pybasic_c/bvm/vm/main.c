#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "../lib/object.h"
#include "ins.h"
#include "vm.h"

#define _BASIC_VM_DEBUG 1

#define resolve VMState_resolve_ref

#define jump(vm) vm->ip += *((short*) (vm->ip + 1))
#define skip(vm) vm->ip += sizeof(short)
#define store(name, value) vm->varspace[*((uint8_t *)name->ptr)] = resolve(vm, value)

/*
Entry point for the VM.
At this point its basically one function with a huge switch case statement.
*/
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size)
{
    int _status = 0;

    if (bytecode_size < 2) {
        return _status;
    }

    VMState *vm = VirtualMachine_new(bytecode);
    
    if (!VirtualMachine_init(vm)) {
        return 2;
    }

    while (vm->_running && !((size_t)(vm->ip - bytecode) >= bytecode_size)) {
        vm->ip++;
        vm->insc++;

        #ifdef _BASIC_VM_DEBUG
        printf("! ins=%d\t:: ip=%p\t:: sp=%ld\n", *vm->ip, vm->ip, vm->sp);
        #endif

        switch (*vm->ip) {
            case _INS_NOP: {
                break;
            }

            case _INS_RETURN: {
                vm->_running = 0;
                _status = 0;
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
                VMState_pushstack(vm, Object_String(vm->data[*++vm->ip]));
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
                    Object *obj = resolve(vm, VMState_popstack(vm));
                    VMState_pushstack(vm, Object_Bool(Object_bool(obj)));
                } else {
                    Object *left = resolve(vm, VMState_popstack(vm));
                    Object *right = resolve(vm, VMState_popstack(vm));

                    VMState_pushstack(vm, Object_Bool(Object_Compare(left, right)));
                }

                break;
            }

            case _INS_NOT: {
                Object *obj = resolve(vm, VMState_popstack(vm));
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
                    Object *item = resolve(vm, vm->stack[vm->sp]);
                    vm->stack[vm->sp] = NULL;
                    Object_print(item);
                    Object_DECREF(item);
                }
                printf("\n");
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

    if (vm->_running)
    {
        _status = 2;
    }

    VirtualMachine_free(vm);

exit:
    return _status;
}

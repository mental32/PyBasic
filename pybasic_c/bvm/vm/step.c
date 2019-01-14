#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <inttypes.h>

#include "../lib/object.h"

#include "ins.h"
#include "vm.h"
#include "vm_state.h"

void BytecodeVirtualMachine_step(VMState *vm)
{
    #define resolve(o) VMState_resolve(vm, o)
    #define stack_item() vm->stack[vm->sp - 1]

    switch (*vm->ip) {
        case _INS_NOP: {
            break;
        }

        case _INS_RETURN: {
            vm->_running = 0;
            vm->__state = OK;
            break;
        }

        case _INS_STORE_NAME: {
            Object *value = VMState_popstack(vm);

            #if __BVM_DEBUG
            printf("! |STORE_NAME( %"PRIu16" :: ", *(uint16_t*)(vm->ip + 1));
            Object_print(VMState_resolve(vm, value));
            printf(" :: ");
            Object_print(vm->varspace[*(uint16_t*)(vm->ip + 1)]);
            #endif

            vm->varspace[*(uint16_t*)(vm->ip + 1)] = VMState_resolve(vm, value);

            #if __BVM_DEBUG
            printf(" :: ");
            Object_print(vm->varspace[*(uint16_t*)(vm->ip + 1)]);
            printf(")\n");
            #endif

            Object_INCREF(value);
            vm->ip += 2;
            break;
        }

        case _INS_LOAD_NAME: {
            VMState_pushstack(vm, Object_New(generic_ref, (void*) (*(uint16_t*)(++vm->ip))));

            #if __BVM_DEBUG
            printf("! |LOAD_NAME( %"PRIu16" ) => ", (uint16_t)stack_item()->ptr);
            Object_print(VMState_resolve(vm, stack_item()));
            printf("\n");
            #endif

            vm->ip++;

            Object_INCREF(VMState_resolve(vm, stack_item()));

            break;
        }

        case _INS_LOAD_CONST: {
            VMState_pushstack(vm, Object_String(vm->const_pool[*((uint8_t*)(++vm->ip))]));

            #if __BVM_DEBUG
            printf("! |LOAD_CONST( %"PRIu8" ) => \"%s\"\n", *((uint8_t*)(vm->ip)), (char*)stack_item()->ptr);
            #endif

            break;
        }

        case _INS_LOAD_LONG: {
            VMState_pushstack(vm, Object_Integer(*((int32_t*)(vm->ip + 1))));
            vm->ip += 4;

            #if __BVM_DEBUG
            printf("! |LOAD_LONG( %"PRIi32" )\n", (int32_t)stack_item()->ptr);
            #endif

            break;
        }

        case _INS_LOAD_SHORT: {
            VMState_pushstack(vm, Object_Integer(*((int16_t*)(vm->ip + 1))));
            vm->ip += 2;

            #if __BVM_DEBUG
            printf("! |LOAD_SHORT( %"PRIi16" )\n", (int16_t)stack_item()->ptr);
            #endif

            break;
        }

        case _INS_LOAD_BYTE: {
            VMState_pushstack(vm, Object_Integer(*((int8_t*)(vm->ip + 1))));
            vm->ip += 1;

            #if __BVM_DEBUG
            printf("! |LOAD_BYTE( %"PRIi8" )\n", (int8_t)stack_item()->ptr);
            #endif

            break;
        }

        case _INS_BINARY_ADD: {
            Object *a = VMState_resolve(vm, VMState_popstack(vm));
            Object *b = VMState_resolve(vm, VMState_popstack(vm));

            #if __BVM_DEBUG
            printf("! |BINARY_ADD ( %"PRIu32" + %"PRIu32" = %"PRIu32" )\n", (uint32_t)a->ptr, (uint32_t)b->ptr, (uint32_t)a->ptr + (uint32_t)b->ptr);
            #endif

            VMState_pushstack(vm, Object_Integer((uint32_t)a->ptr + (uint32_t)b->ptr));
            break;
        }

        case _INS_BINARY_SUB: {
            Object *a = VMState_resolve(vm, VMState_popstack(vm));
            Object *b = VMState_resolve(vm, VMState_popstack(vm));

            #if __BVM_DEBUG
            printf("! |BINARY_SUB ( %"PRIu32" - %"PRIu32" = %"PRIu32" )\n", (uint32_t)a->ptr, (uint32_t)b->ptr, (uint32_t)a->ptr - (uint32_t)b->ptr);
            #endif

            VMState_pushstack(vm, Object_Integer((uint32_t)a->ptr - (uint32_t)b->ptr));
            break;
        }

        case _INS_BINARY_MUL: {
            Object *a = VMState_resolve(vm, VMState_popstack(vm));
            Object *b = VMState_resolve(vm, VMState_popstack(vm));

            #if __BVM_DEBUG
            printf("! |BINARY_MUL ( %"PRIu32" * %"PRIu32" = %"PRIu32" )\n", (uint32_t)a->ptr, (uint32_t)b->ptr, (uint32_t)a->ptr * (uint32_t)b->ptr);
            #endif

            VMState_pushstack(vm, Object_Integer((uint32_t)a->ptr * (uint32_t)b->ptr));
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
            } else {
                vm->ip += 2;
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
            vm->__state = PANIC;
            vm->_running = 0;
        }
    }
}

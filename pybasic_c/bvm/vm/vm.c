#include <stdlib.h>
#include <string.h>

#include "vm.h"

BytecodeHeader *BytecodeHeader_New(uint8_t *bytecode)
{
    BytecodeHeader *header = malloc(sizeof(BytecodeHeader));
    header->varspace_size = 0;
}

VMState *VirtualMachine_new(uint8_t *bytecode)
{
    VMState *vm = (VMState *) malloc(sizeof(VMState));
    vm->ip = bytecode;

    vm->_running = 0;
    vm->__state = 0;

    vm->insc = 0;
    vm->sp = 0;
    return vm;
}

int VirtualMachine_init(VMState *vm)
{
    vm->header = BytecodeHeader_New(vm->bytecode);

    vm->varspace = (Object **) malloc(sizeof(Object **) * vm->header->varspace_size);

    for (size_t i = 0; i < vm->header->varspace_size; i++)
    {
        vm->varspace[i] = NULL;
    }

    // Read size of constants pool.
    short dp = 0, data_size = *((short*) (vm->ip));

    if (data_size > 2)
    {
        // Map the constants pool to vm->data
        // This is for a lookup speed of O(n)
        // When refrencing constants.
        char *p = (char *)vm->ip + 2, *end = (char *)(data_size + vm->ip + 2);

        while (p < end) {
            vm->const_pool[dp++] = p;
            p += strlen(p) + 1;
        }

        p = NULL;
    }

    // Then jump ahead of it.
    vm->ip += data_size - 1;


    return 1;
}

int VirtualMachine_free(VMState *vm)
{
    while (vm->sp--)
    {
        Object_UDECREF(vm->stack[vm->sp]);
    }

    for (size_t i = 0; i < vm->header->varspace_size; i++)
    {
        Object_UDECREF(vm->varspace[i]);
    }

    return 1;
}

#include <stdlib.h>
#include <string.h>

#include "vm.h"

/*
Create and return a new BytecodeHeader.

Parameters
----------
 uint8_t *bytecode - The bytecode to initialize the header with.
*/
BytecodeHeader *BytecodeHeader_New(uint8_t *bytecode)
{
    BytecodeHeader *header = malloc(sizeof(BytecodeHeader));
    header->varspace_size = 0;
}

/*
Create a new (uninitialized) VMState.

Parameters
----------
 uint8_t *bytecode - The bytecode to bind to the VMState.
*/
VMState *VirtualMachine_new(uint8_t *bytecode)
{
    VMState *vm = (VMState *) malloc(sizeof(VMState));
    vm->ip = bytecode;
    vm->varspace = NULL;

    vm->_running = 0;
    vm->__state = 0;

    vm->insc = 0;
    vm->sp = 0;
    return vm;
}

/*
initialize a VMState, if called with an already initialized VMState it will be reinitialized.
returns a 1 if successfull otherwise 0.

Parameters
----------
 VMState *vm - The VMState to initialize.
*/
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
        vm->const_pool = (char**) malloc((size_t) (end - p));

        while (p < end) {
            vm->const_pool[dp++] = p;
            p += strlen(p) + 1;
        }
    }

    // Then jump ahead of it.
    vm->ip += data_size - 1;

    return 1;
}

/*
Free and destroy a VMState.
This also clears and free's the stack and the varspace.

Parameters
----------
 VMState *vm - The VMState to free.
*/
void VirtualMachine_free(VMState *vm)
{
    while (vm->sp--)
    {
        if (vm->stack[vm->sp])
            Object_Free(vm->stack[vm->sp]);
    }

    if (vm->varspace != NULL)
    {
        for (size_t i = 0; i < vm->header->varspace_size; i++)
        {
            if (vm->varspace[i] != NULL) {
                Object_Free(vm->varspace[i]);
            }
        }
    }

    free(vm->header);
    free(vm);
}

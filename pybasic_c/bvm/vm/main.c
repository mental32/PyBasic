#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "vm.h"

/*
run the entire bytecode to completion.
NOTE:This is a blocking operation and may not give up control.

Parameters
----------
 uint8_t *bytecode - A pointer to the base of the bytecode that the VM should read.
 size_t bytecode_size - The size of the bytecode.
*/
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size)
{
    if (bytecode_size < 2) {
        return OK;
    }

    #if __BVM_DEBUG
    printf("! Creating VMState instance.\n");
    #endif

    VMState *vm = VirtualMachine_new(bytecode);

    #if __BVM_DEBUG
    printf("! Initializing VMState...");
    #endif

    if (!VirtualMachine_init(vm, bytecode_size)) {
        return PANIC;
    }

    #if __BVM_DEBUG
    printf("OK\n! Entering VMState loop\n");
    #endif

    vm->_running = 1;

    while (vm->_running) {
        vm->ip++;
        vm->insc++;

        #if __BVM_DEBUG
        printf("!-ins=%d :: ip=%p :: sp=%ld\n", *vm->ip, vm->ip, vm->sp);
        #endif

        BytecodeVirtualMachine_step(vm);
    }

    if (vm->_running)
    {
        vm->err = "VM exited while running.\n";
        vm->__state = PANIC;
    }

    return VirtualMachine_free(vm);
}

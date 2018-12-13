#ifndef _BASIC_VMSTATE_H
#define _BASIC_VMSTATE_H

#include <stddef.h>

#include "../lib/status.h"
#include "../lib/object/base.h"

#define __BVM_DEBUG 0

char *BytecodeVirtualMachine_err;

/*
Helper struct for containing bytecode metadata.
*/
typedef struct {
    uint16_t size;
    uint16_t varspace_size;
} BytecodeHeader;

/*
implementation of a VirtualMachine state.
*/
typedef struct {
    uint8_t *ip;
    uint8_t *bytecode;

    uint8_t _running;
    uint8_t __state;

    char **const_pool;

    size_t insc;
    size_t sp;

    BytecodeHeader *header;

    Object **stack;
    Object **varspace;
} VMState;

VMState *VirtualMachine_new(uint8_t *bytecode);
int VirtualMachine_free(VMState *vm);
int VirtualMachine_init(VMState *vm);

void BytecodeVirtualMachine_step(VMState *vm);

#endif

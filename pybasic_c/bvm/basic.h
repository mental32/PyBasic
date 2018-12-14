#ifndef _BASIC_BVM_H
#define _BASIC_BVM_H

#include <stddef.h>
#include <stdint.h>

char *BytecodeVirtualMachine_err;
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size);

#endif

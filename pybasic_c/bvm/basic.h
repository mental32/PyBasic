#ifndef _BASIC_BVM_H
#define _BASIC_BVM_H

#include "lib/status.h"

char *BytecodeVirtualMachine_err;
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size);

#endif

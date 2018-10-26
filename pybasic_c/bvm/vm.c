#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>

#include "ins.h"

/*

*/
enum ObjectType {
    _obj_tp_long,
    _obj_tp_str,
};

typedef struct {
    uint8_t tp;
    void *ptr;
} Object;

/*

*/
typedef struct {
    uint8_t *ip;
    uint8_t _running;
    size_t insc;
} VMState;

/*

*/
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size) {
    int _status = 0;

    if (!bytecode_size) {
        return _status;
    }    

    VMState *vm = (VMState *) malloc(sizeof(VMState));
    vm->ip = bytecode;
    vm->_running = 1;
    vm->insc = 0;

    while (vm->_running) {
        vm->ip++;

        // Explicitly handle a nop instruction (0).
        // Don't increment the instruction counter (insc)
        // Just loop around for the next instruction.
        if (!vm->ip) {
            continue;
        }

        vm->insc++;

        switch (*vm->ip) {
            case _INS_RETURN: {
                vm->_running = 0;
                _status = 1;
                break;
            }

            case _INS_GOTO: {
                vm->ip += *((short*) (vm->ip + 1));
                break;
            }

            case _INS_PRINT: {
                break;
            }
        }
    }

    return _status;
}

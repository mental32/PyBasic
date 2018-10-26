#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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
    uint8_t _status;

    size_t insc;
    size_t sp;

    Object **stack;
} VMState;

static inline void BVM_EXIT_OK(VMState *vm) {
    vm->_running = 0;
    vm->_status = 1;
    return;
}

static inline void BVM_EXIT_ERR(VMState *vm) {
    vm->_running = 0;
    vm->_status = 0;
    return;
}

/*

*/
int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size) {
    if (bytecode_size < 1) {
        return 0;
    }

    VMState *vm = (VMState *) malloc(sizeof(VMState));
    vm->_running = 1;
    vm->_status = 0;
    vm->insc = 0;

    vm->stack = (Object *) malloc(sizeof(Object) * 20);
    long sp = 0;

    vm->sp = 0;

    vm->ip = bytecode;

    // Read through data until we hit DATA_END (255)
    size_t DATA_SIZE = 0;
    for (uint8_t *size_count = vm->ip; *size_count++ != (uint8_t) _INS_DATA_END; DATA_SIZE++);

    // Jump ahead of data
    vm->ip += DATA_SIZE;

    while (vm->_running) {
        vm->ip++;

        // Explicitly handle a nop instruction (0).
        // Don't increment the instruction counter (insc)
        // Just loop around for the next instruction.
        if (!*vm->ip) {
            continue;
        }

        vm->insc++;

        switch (*vm->ip) {
            case _INS_RETURN: {
                BVM_EXIT_OK(vm);
                break;
            }

            case _INS_GOTO: {
                vm->ip += *((short*) (vm->ip + 1));
                break;
            }

            case _INS_LOAD_CONST :{

            }

            case _INS_STORE_NAME : {

            }

            case _INS_LOAD_NAME: {

            }

            case _INS_PRINT: {

            }

            // default: {
            //     printf("Unknown bytecode %d\n", *vm->ip);
            //     BVM_EXIT_ERR(vm);
            // }
        }
    }

    vm->_status = 1;

    // free(vm->stack);

    return vm->_status;
}

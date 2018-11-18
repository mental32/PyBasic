VMState *VirtualMachine_new(uint8_t *bytecode)
{
    VMState *vm = (VMState *) malloc(sizeof(VMState));
    vm->ip = bytecode;
    vm->_running = 0;

    // vm->data = (char **)malloc((size_t) 20);

    vm->insc = 0;
    vm->sp = 0;

    // vm->stack = (Object **)malloc((size_t) 20);
    // vm->varspace = (Object **)malloc((size_t) 20);
}

int VirtualMachine_init(void)
{
    // Read size of constants pool.
    short dp = 0, data_size = *((short*) (vm->ip));

    // Map the constants pool to vm->data
    // This is for a lookup speed of O(n)
    // When refrencing constants.
    char *p = (char *)vm->ip + 2, *end = (char *)(data_size + vm->ip + 2);

    while (p < end) {
        vm->data[dp++] = p;
        p += strlen(p) + 1;
    }

    p = NULL;

    // Then jump ahead of it.
    vm->ip += data_size - 1;
}

int VirtualMachine_free(VMState *vm)
{
    
}

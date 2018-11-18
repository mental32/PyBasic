int BytecodeVirtualMachine_main(uint8_t *bytecode, size_t bytecode_size)
{
    int _status = 0;

    if (bytecode_size < 2) {
        return _status;
    }

    VMState *vm = VirtualMachine_new(bytecode);

    while (vm->_running && !((size_t)(vm->ip - bytecode) >= bytecode_size)) {
        vm->ip++;
        vm->insc++;

        #ifdef _BASIC_VM_DEBUG
        printf("! %d\t:: %p\t:: %ld\n", *vm->ip, vm->ip, vm->sp);
        #endif

        switch (*vm->ip) {
            case _INS_NOP: {
                break;
            }

            case _INS_RETURN: {
                vm->_running = 0;
                _status = 0;
                break;
            }

            case _INS_STORE_NAME: {
                Object *name = popstack(vm);
                Object *value = popstack(vm);

                vm->varspace[*((uint8_t *)name->ptr)] = resolve(vm, value);
                break;
            }

            case _INS_LOAD_NAME: {
                pushstack(vm, NewObject(generic_ref, (uint8_t *) (++vm->ip)));
                break;
            }

            case _INS_LOAD_CONST: {
                pushstack(vm, NewObject(STRING, vm->data[*++vm->ip]));
                break;
            }

            case _INS_LOAD_LONG: {
                pushstack(vm, NewObject(LONG, ((long*) (vm->ip + 1))));
                vm->ip += sizeof(*((long*) (vm->ip + 1)));
                break;
            }

            case _INS_LOAD_SHORT: {
                pushstack(vm, NewObject(SHORT, ((short*) (++vm->ip))));
                vm->ip += 1;
                break;
            }

            case _INS_LOAD_BYTE: {
                pushstack(vm, NewObject(BYTE, ((uint8_t*) (++vm->ip))));
                break;
            }

            case _INS_BINARY_ADD: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    TailorInteger(_ref_a, (long)(GetIntValue(a) + GetIntValue(b)));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_SUB: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    TailorInteger(_ref_a, (long)(GetIntValue(a) - GetIntValue(b)));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_MUL: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    TailorInteger(_ref_a, (long)((long)GetIntValue(a) * (long)GetIntValue(b)));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_BINARY_DIV: {
                Object *_ref_a = popstack(vm);

                Object *a = resolve(vm, _ref_a);
                Object *b = resolve(vm, popstack(vm));

                if (a->tp & generic_int && b->tp & generic_int) {
                    TailorInteger(_ref_a, (long)(GetIntValue(a) / GetIntValue(b)));
                    pushstack(vm, _ref_a);
                }

                break;
            }

            case _INS_CMP: {
                if (vm->sp == 1) {
                    pushstack(vm, NewObject(BYTE, ObjectIsTrue(resolve(vm, popstack(vm)))));
                } else {
                    pushstack(vm, NewObject(_BYTE, CompareObjects(resolve(vm, popstack(vm)), resolve(vm, popstack(vm)))));
                }

                break;
            }

            case _INS_NOT: {
                Object *obj = resolve(vm, popstack(vm));

                if (IS_INT(obj)) {
                    pushstack(vm, NewObject(obj->tp, !GetIntValue(obj)));
                }

                else if (IS_STR(obj)) {
                }

                break;
            }

            case _INS_JMP_TRUE: {
                Object *obj = popstack(vm);

                if (IS_INT(obj) && GetIntValue(obj)) {
                    vm->ip += *((short*) (vm->ip + 1));
                } else if ((bytecode_size - (vm->ip - bytecode)) >= 2) {
                    vm->ip += sizeof(short);
                }

                break;
            }

            case _INS_PRINT: {
                while (vm->sp--) {
                    Object *item = resolve(vm, vm->stack[vm->sp]);
                    vm->stack[vm->sp] = NULL;
                    PrintObject(item);
                }
                printf("\n");
                vm->sp = 0;
                break;
            }

            case _INS_GOTO: {
                vm->ip += *((short*) (vm->ip + 1));
                break;
            }

            default: {
                _status = 1;
                vm->_running = 0;
            }
        }
    }

    if (vm->_running) {
        _status = 2;
    }

    VirtualMachine_free(vm);

    return _status;
}

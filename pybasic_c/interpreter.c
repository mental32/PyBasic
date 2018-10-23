#include <Python.h>
#include <stdint.h>
#include <stdbool.h>
#include <structmember.h>

#include "interpreter.h"
#include "instructions.h"
#include "object.h"

int bytecode_parse_source(ByteCodeInterpreter *vm, uint8_t *buf, const char *source);

static PyObject *ByteCodeInterpreter_run_source(ByteCodeInterpreter *self, PyObject *args);
static PyObject *ByteCodeInterpreter_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static void ByteCodeInterpreter_dealloc(ByteCodeInterpreter *self);

static PyMemberDef ByteCodeInterpreter_members[] = {
    {"_constants", T_OBJECT_EX, offsetof(ByteCodeInterpreter, _constants), 0, "source constants"},
    {"_data", T_OBJECT_EX, offsetof(ByteCodeInterpreter, _data), 0, "interpreter namespace"},
    {NULL}  /* Sentinel */
};

static PyMethodDef ByteCodeInterpreter_methods[] = {
    {"run_source", (PyCFunction) ByteCodeInterpreter_run_source, METH_VARARGS, "Run the interpreter."},
    {NULL}  /* Sentinel */
};

PyTypeObject ByteCodeInterpreterType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_pybasic.ByteCodeInterpreter",
    .tp_basicsize = sizeof(ByteCodeInterpreter),
    .tp_itemsize = 0,
    .tp_new = ByteCodeInterpreter_new,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_dealloc = (destructor) ByteCodeInterpreter_dealloc,
    .tp_members = ByteCodeInterpreter_members,
    .tp_methods = ByteCodeInterpreter_methods,
};

static void
ByteCodeInterpreter_dealloc(ByteCodeInterpreter *self)
{
    // Recipe for deallocation is just
    // Py_XDECREF(self->attribute);
    // for every attribute.

    Py_XDECREF(self->_data);
    Py_XDECREF(self->_constants);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
ByteCodeInterpreter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    ByteCodeInterpreter *self;
    self = (ByteCodeInterpreter *) type->tp_alloc(type, 0);

    if (self != NULL) {
        self->_data = PyDict_New();

        if (self->_data == NULL) {
            Py_DECREF(self->_data);
            return NULL;
        }

        self->_constants = PyList_New(0);

        if (self->_constants == NULL) {
            Py_DECREF(self->_constants);
            return NULL;
        }

        self->ip = 0;
        self->_running = false;
    }

    return (PyObject *) self;
}


static PyObject *
ByteCodeInterpreter_run_source(ByteCodeInterpreter *self, PyObject *args)
{
    const char *source;
    PyObject *risp = PyLong_FromLong(-1);

    if (!PyArg_ParseTuple(args, "s", &source)) {
        return NULL;
    }

    // TODO: Bytecode parsing
    // example of hello world
    //
    // {_INS_LOAD_CONST, 0x01, _INS_RETURN}
    // {_INS_BUILD_STR, 0x0D, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, _INS_RETURN}
    // {_INS_LABEL_INT, 0x05, _INS_LOAD_CONST, 1, _INS_PRINT, _INS_GOTO, 0x05}

    long insc = 0, lnsp = 0, nsp = 0, sp = 0;
    uint8_t bytecode[1024];
    Object *stack[512];

    Binding *namespace[512];
    Label *labels[512];

    if (!bytecode_parse_source(self, bytecode, source)) {
        return NULL;
    }

    self->_running = true;
    while (self->_running) {
        uint8_t instruction = bytecode[self->ip++];

        // Explicitly handle a nop instruction.
        // Don't increment the instruction counter (insc)
        // Just loop around for the next instruction.
        if (!instruction) {
            continue;
        }

        insc++;

        switch (instruction) {
            case _INS_RETURN: {
                self->_running = false;
                risp = PyLong_FromLong((long) self->ip);

                // Free the stack and it's objects.
                while(sp >= 0) {
                    delObject(stack[--sp]);
                    stack[sp] = NULL;
                }

                // free(namespace);
                // free(labels);
                break;
            }
            case _INS_LABEL_INT: {
                labels[lnsp] = (Label *) malloc(sizeof(Label));
                labels[lnsp]->ip = self->ip + 1;
                labels[lnsp++]->name = bytecode[self->ip++];
                break;
            }
            case _INS_GOTO: {
                for (int k = 0; k < lnsp; k++) {
                    if (labels[k]->name == bytecode[self->ip++]) {
                        self->ip = labels[k]->ip;
                        break;
                    }
                }
                break;
            }
            case _INS_PRINT: {
                while (sp--) {
                    printf("%ld :: %d :: %s\n", insc, sp, stack[sp]->ptr);
                }
                sp = 0;
                break;
            }
            case _INS_STORE: {
                namespace[nsp++] = newBinding(stack[--sp], PyList_GetItem(self->_constants, bytecode[self->ip++]));
                break;
            }
            case _INS_LOAD_CONST: {
                stack[sp++] = newObject(_obj_tp_str, PyUnicode_AsUTF8(PyList_GetItem(self->_constants, bytecode[self->ip++])));
                break;
            }
            case _INS_BUILD_STR: {
                size_t offset = 0, size = 0;

                while (bytecode[self->ip + offset] != 0)
                    size++;

                char* string = malloc(size + 1);
                memcpy(string, &bytecode[self->ip], size);
                string[size] = '\0';
                self->ip += size;
                stack[sp++] = newObject(_obj_tp_str, (void *)string);
                break;
            }
        }
    }

    Py_INCREF(risp);
    return risp;
}

#include <Python.h>
#include "structmember.h"
#include "stdint.h"
#include "stdbool.h"

#include "_pybasic.h"
#include "instructions.h"

typedef struct {
    PyObject_HEAD
    PyObject *_data;
    uint16_t ip;
    bool _running;
} ByteCodeInterpreter;


inline PyObject *
Py_RETURN_From_PyBVM(ByteCodeInterpreter *self) {
    self->_running = false;
    PyObject *insp = PyLong_FromLong((long) self->ip);
    Py_INCREF(insp);
    return insp;
}

static void
ByteCodeInterpreter_dealloc(ByteCodeInterpreter *self)
{
    // Recipe for deallocation is just
    // Py_XDECREF(self->attribute);
    // for every attribute.

    Py_XDECREF(self->_data);
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

        self->ip = 0;
        self->_running = false;
    }

    return (PyObject *) self;
}

static PyObject *
ByteCodeInterpreter_run_source(ByteCodeInterpreter *self, PyObject *args)
{
    const char *source;

    // TODO: Bytecode parsing
    // example of hello world
    // {_INS_BUILD_STR, 0x0D, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, _INS_RETURN}

    uint8_t bytecode[1024];
    uint8_t stack[512];

    long sp = 0;

    if (!PyArg_ParseTuple(args, "s", &source)) {
        return NULL;
    }

    self->_running = true;

    // Just in case we manage to fill up the bytecode buffer
    // exit from the virtual machine unconditionally.
    bytecode[1023] = _INS_RETURN;

    // _parse_source_to_bytecode(bytecode, source);

    while (self->_running) {
        uint8_t instruction = bytecode[self->ip++];

        switch (instruction) {
            case _INS_RETURN: {
                Py_RETURN_From_PyBVM(self);
            }
            case _INS_LITERAL: {
                stack[sp++] = bytecode[self->ip++];
                break;
            }
            case _INS_BUILD_STR: {
                stack[sp++] = bytecode[self->ip++];

                size_t i = 0, _str_size = stack[sp - 1] + 1;
                char *string = (char *) malloc(_str_size);

                while (i < _str_size) {
                    string[i] = bytecode[self->ip++];
                    i++;
                }

                string[i] = '\0';

                sp = 0;
                break;
            }
        }
    }

    Py_RETURN_NONE;
}

// def run_source(self, source):
//     for line in source.split('\n'):
//         ln, *tokens = self._tokenize(line)

//         if not _pybasic.is_integer(ln):
//             raise ValueError("Invalid line number.")

//         source_no_ln = line[len(ln):].strip()
//         self._code[ln] = tokens

//         print(ln, tokens)

//         for pattern, name in patterns.items():
//             match = re.fullmatch(pattern, source_no_ln)
//             if match:
//                 print(match)
//                 print(self._compile_match(name, match))
//                 break

static PyMemberDef ByteCodeInterpreter_members[] = {
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

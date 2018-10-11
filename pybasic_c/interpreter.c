#include <Python.h>
#include "structmember.h"
#include "stdint.h"
#include "stdbool.h"

#include "_pybasic.h"
#include "instructions.h"

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
    }

    return (PyObject *) self;
}

static PyObject *
ByteCodeInterpreter_run_source(ByteCodeInterpreter *self, PyObject *args)
{
    const char *source;
    bool running = true;

    if (!PyArg_ParseTuple(args, "s", &source)) {
        return NULL;
    }

    uint8_t bytecode[1024];

    _parse_source_to_bytecode(bytecode);

    while (running) {
        uint8_t instruction = bytecode[self->ip++];

        switch (instruction) {
            case _INS_RETURN: {
                printf("Exiting PyBASIC VM...(%hu) \n", (unsigned short int) self->ip);
                running = false;
                Py_RETURN_PyLong_From_PyBVM_IP(self);
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

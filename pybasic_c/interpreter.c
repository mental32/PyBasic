#include <Python.h>
#include "structmember.h"
#include "stdint.h"

#include "_pybasic.h"
#include "instructions.h"

typedef struct {
    PyObject_HEAD
} ByteCodeInterpreter;

static void
ByteCodeInterpreter_dealloc(ByteCodeInterpreter *self)
{
    // Recipe for deallocation is just
    // Py_XDECREF(self->attribute);
    // for every attribute.

    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
ByteCodeInterpreter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    ByteCodeInterpreter *self;
    self = (ByteCodeInterpreter *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

static PyObject *
ByteCodeInterpreter_run(ByteCodeInterpreter *self, PyObject *args)
{

    const char *source;
    int16_t *line_number;

    if (!PyArg_ParseTuple(args, "s", &source)) {
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
ByteCodeInterpreter_eval_expression(ByteCodeInterpreter *self, PyObject *args)
{
    const char *expression;

    if (!PyArg_ParseTuple(args, "s", &expression)) {
        return NULL;
    }

    for (size_t i = 0; i < strlen(expression); i++) {}
    Py_RETURN_NONE;
}

static PyObject *
ByteCodeInterpreter__compile_match(PyObject *self, PyObject *args)
{
    PyObject *match;
    const char *name;

    // Bytecode array of 1024 bytes (1KB)
    uint8_t bytecode[1024];

    if (!PyArg_ParseTuple(args, "|sO", &name, &match)) {
        return NULL;
    }

    if (name == "let") {
        bytecode[0] = (uint8_t) _INS_STORE;

        // PyObject *varname = PyList_GetItem(match, 1);
        // PyObject *expression = PyList_GetItem(match, 3);
    }

    Py_RETURN_NONE;
}

// def _handle(self, name, match):
//     if name == 'let':
//         _, varname, _, expr = match.groups()
//         self._data[varname] = expr
//         return []

//     elif name == 'print':
//         _, expr = match.groups()
//         print(match.groups())
//         print(eval(expr, {}, self._data))
//         return []

//     elif name == 'goto':
//         _, ln = match.groups()
//         return [0x03, int(ln), 0x08]

//     else:
//         return []

static PyMemberDef ByteCodeInterpreter_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef ByteCodeInterpreter_methods[] = {
    {"run", (PyCFunction) ByteCodeInterpreter_run, METH_VARARGS, "Run the interpreter."},
    {"eval", (PyCFunction) ByteCodeInterpreter_eval_expression, METH_VARARGS, "Evaluate an expression."},
    {"_compile_match", (PyCFunction) ByteCodeInterpreter__compile_match, METH_VARARGS, "Compile the bytecode for a regex match."},
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

#include <Python.h>
#include "structmember.h"

#include "_pybasic.h"

enum BYTECODE {
    STORE,
    LOAD,
    LOAD_CONST,
    LOAD_INT_CONST,
    LOAD_GLOBAL,
    LOAD_ARGS,
    CALL,
    JUMP
};

typedef enum BYTECODE bytecode_t;

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

    if (!PyArg_ParseTuple(args, "s", &source)) {
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
ByteCodeInterpreter_eval_expression(ByteCodeInterpreter *self, PyObject *args)
{
    const char *expression;
    long type = 1;
    long p_len = 1;

    int speech = 0;

    if (!PyArg_ParseTuple(args, "s", &expression)) {
        return NULL;
    }

    for (size_t i = 0; i < strlen(expression); i++) {
        if (expression[i] == '"') {
            speech = !speech;
        }

        p_len++;
    }
}

static PyObject *
ByteCodeInterpreter__compile_match(PyObject *self, PyObject *args)
{
    PyObject *bytecode = PyList_New((Py_ssize_t *) 0);
    PyObject *match;
    const char *name;

    if (!PyArg_ParseTuple(args, "|sO", &name, &match)) {
        return NULL;
    }

    if (name == "let") {
        PyObject *tmp = PyList_GetItem(match);
        if (!PyObject_SetItem(self->_data, NULL, NULL)) {
            return NULL;
        }
    }

    PyObject *

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

#include <Python.h>
#include "structmember.h"

#include "_pybasic.h"

enum BYTECODE {
    STORE,
    LOAD,
    LOAD_KEYWRD,
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

    if (self != NULL) {

    }

    return (PyObject *) self;
}

static int
ByteCodeInterpreter_init(ByteCodeInterpreter *self, PyObject *args, PyObject *kwds)
{
    return 0;
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

static PyMemberDef ByteCodeInterpreter_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef ByteCodeInterpreter_methods[] = {
    {"run", (PyCFunction) ByteCodeInterpreter_run, METH_VARARGS, "Run the interpreter."},
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
    .tp_init = (initproc) ByteCodeInterpreter_init,
    .tp_members = ByteCodeInterpreter_members,
    .tp_methods = ByteCodeInterpreter_methods,
};

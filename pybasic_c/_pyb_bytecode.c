#include <Python.h>
#include "structmember.h"

#include "_pybasic.h"

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
        // self->first = PyUnicode_FromString("");
        // if (self->first == NULL) {
        //     Py_DECREF(self);
        //     return NULL;
        // }
        // self->last = PyUnicode_FromString("");
        // if (self->last == NULL) {
        //     Py_DECREF(self);
        //     return NULL;
        // }
        // self->number = 0;
    }
    return (PyObject *) self;
}

static int
ByteCodeInterpreter_init(ByteCodeInterpreter *self, PyObject *args, PyObject *kwds)
{
    // static char *kwlist[] = {"source", NULL};
    // PyObject tmp;

    // if (!PyArg_ParseTuple(args, "s", &self->source)) {
    //     return -1;
    // }

    // if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOi", kwlist,
    //                                  &first, &last,
    //                                  &self->number))
    //     return -1;

    // if (first) {
    //     tmp = self->first;
    //     Py_INCREF(first);
    //     self->first = first;
    //     Py_XDECREF(tmp);
    // }

    return 0;
}

static PyObject *
ByteCodeInterpreter_run(ByteCodeInterpreter *self, PyObject *args)
{

    const char *source;

    if (!PyArg_ParseTuple(args, "s", &source)) {
        return NULL;
    }

    // Println(source);
    for (size_t i = 0; i < strlen(source); i++) {
        printf("%d: %c\n", i, source[i]);
    }


    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
ByteCodeInterpreter_run_repl(ByteCodeInterpreter *self)
{
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMemberDef ByteCodeInterpreter_members[] = {
    // {"source", T_STRING, offsetof(ByteCodeInterpreter, source), 0, "source code"},
    {NULL}  /* Sentinel */
};

static PyMethodDef ByteCodeInterpreter_methods[] = {
    {"run", (PyCFunction) ByteCodeInterpreter_run, METH_VARARGS, "Run the interpreter."},
    {"run_repl", (PyCFunction) ByteCodeInterpreter_run_repl, METH_NOARGS, "Enter the PyBasic REPL."},
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

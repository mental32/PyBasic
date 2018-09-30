#include <Python.h>

#include "_pybasic.h"

typedef struct {
    PyObject_HEAD
    const char *input_file;
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
    // static char *kwlist[] = {"first", "last", "number", NULL};
    // PyObject *first = NULL, *last = NULL, *tmp;

    if (!PyArg_ParseTuple(args, "s", &self->input_file)) {
        return -1;
    }

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
    // if (last) {
    //     tmp = self->last;
    //     Py_INCREF(last);
    //     self->last = last;
    //     Py_XDECREF(tmp);
    // }

    return 0;
}

static PyObject *
ByteCodeInterpreter_run(ByteCodeInterpreter *self)
{

    if (self->input_file == NULL) {
        if (!PyObject_CallMethod(self, "run_repl")) {
            return NULL;
        }
    }

    Py_INCREF(Py_None);
    return Py_None;
}

static PyMemberDef ByteCodeInterpreter_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef ByteCodeInterpreter_methods[] = {
    {"run", (PyCFunction) ByteCodeInterpreter_run, METH_NOARGS, "Run the interpreter."},
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
};

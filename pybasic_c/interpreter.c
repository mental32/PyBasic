#include <Python.h>

#include <stdint.h>
#include <structmember.h>

#include "_pybasic.h"
#include "bvm/basic.h"

typedef struct {
    PyObject_HEAD
} ByteCodeInterpreter;

static PyObject *ByteCodeInterpreter_run_source(ByteCodeInterpreter *self, PyObject *args);
static PyObject *ByteCodeInterpreter_run_bytecode(ByteCodeInterpreter *self, PyObject *args);
static PyObject *ByteCodeInterpreter_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static void ByteCodeInterpreter_dealloc(ByteCodeInterpreter *self);

static PyMemberDef ByteCodeInterpreter_members[] = {
    {NULL}  /* Sentinel */
};

static PyMethodDef ByteCodeInterpreter_methods[] = {
    {"run_source", (PyCFunction) ByteCodeInterpreter_run_source, METH_VARARGS, "Run the interpreter."},
    {"run_bytecode", (PyCFunction) ByteCodeInterpreter_run_bytecode, METH_VARARGS, "Run raw bytecode."},
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

    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
ByteCodeInterpreter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    return (PyObject *)((ByteCodeInterpreter *) type->tp_alloc(type, 0));
}

static PyObject *
ByteCodeInterpreter_run_source(ByteCodeInterpreter *self, PyObject *args)
{
    
    PyObject *bytearray = PyObject_CallObject(tokenizer, args);

    if (!bytearray) {
        return NULL;
    }

    size_t size = PyByteArray_GET_SIZE(bytearray);
    uint8_t *bytecode = (uint8_t *) PyByteArray_AS_STRING(bytearray);

    if (BytecodeVirtualMachine_main(bytecode, size) != 0) {
        PyErr_SetString(PyExc_RuntimeError, BytecodeVirtualMachine_err);
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
ByteCodeInterpreter_run_bytecode(ByteCodeInterpreter *self, PyObject *args)
{
    PyObject *bytearray;

    if (!PyArg_ParseTuple(args, "Y", &bytearray)) {
        return NULL;
    }

    size_t size = PyByteArray_GET_SIZE(bytearray);
    uint8_t *bytecode = (uint8_t *) PyByteArray_AS_STRING(bytearray);

    if (BytecodeVirtualMachine_main(bytecode, size) != 0) {
        PyErr_SetString(PyExc_RuntimeError, "BVM aborted execution.");
        return NULL;
    }

    Py_RETURN_NONE;
}

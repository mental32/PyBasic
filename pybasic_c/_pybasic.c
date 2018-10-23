#include <Python.h>

#define PYBASIC_CORE
#include "_pybasic.h"

PyObject *_ins_dict;

static PyObject *
set_tokenizer(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "O", &tokenizer)) {
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
get_bytecode_instruction(PyObject *self, PyObject *args)
{
    PyObject *key, *item;

    if (!PyArg_ParseTuple(args, "O", &key)) {
        return NULL;
    }

    item = PyDict_GetItem(_ins_dict, key);

    if (!item) {
        PyErr_SetString(PyExc_KeyError, "instruction not found.");
        return NULL;
    }
    
    Py_INCREF(item);
    return item;
}

static PyMethodDef BPyBasicMethods[] = {
    {"set_tokenizer", set_tokenizer, METH_VARARGS, "Set the default tokenizer used."},
    {"get_bytecode", get_bytecode_instruction, METH_VARARGS, "Get the bytecode for an instruction name."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef BPyBasicModule = {
    PyModuleDef_HEAD_INIT,
    "_pybasic",   /* name of module */
    NULL,         /* module documentation, may be NULL */
    -1,           /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    BPyBasicMethods
};

PyMODINIT_FUNC
PyInit__pybasic(void)
{
    PyObject *module;
    if (PyType_Ready(&ByteCodeInterpreterType) < 0) {
        return NULL;
    }

    module = PyModule_Create(&BPyBasicModule);
    if (module == NULL) {
        return NULL;
    }

    _ins_dict = PyDict_New();
    if (!_ins_dict) {
        Py_XDECREF(_ins_dict);
        return NULL;
    } else {
        Py_INCREF(_ins_dict);
    }
    

    Py_INCREF(&ByteCodeInterpreterType);
    PyModule_AddObject(module, "ByteCodeInterpreter", (PyObject *) &ByteCodeInterpreterType);
    return module;
}

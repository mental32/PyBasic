#include <Python.h>

#define PYBASIC_CORE
#include "_pybasic.h"

static PyObject *
set_tokenizer(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "O", &tokenizer)) {
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyMethodDef BPyBasicMethods[] = {
    {"set_tokenizer", set_tokenizer, METH_VARARGS, "Set the default tokenizer used."},
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

    Py_INCREF(&ByteCodeInterpreterType);
    PyModule_AddObject(module, "ByteCodeInterpreter", (PyObject *) &ByteCodeInterpreterType);
    return module;
}

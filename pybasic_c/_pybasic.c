#include <Python.h>

#define PYBASIC_CORE
#include "_pybasic.h"

static PyMethodDef BPyBasicMethods[] = {
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

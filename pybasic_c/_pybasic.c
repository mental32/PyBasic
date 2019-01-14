#include <Python.h>

#define PYBASIC_CORE
#include "_pybasic.h"
#include "bvm/vm/ins.h"

static PyObject *
set_tokenizer(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "O", &tokenizer)) {
        return NULL;
    }

    Py_RETURN_NONE;
}

static PyObject *
get_bytecode_instructions(PyObject *self, PyObject *args)
{
    PyObject *dict = PyDict_New();
    Py_INCREF(dict);

#define set(s, l) PyDict_SetItem(dict, PyUnicode_FromString(s), PyLong_FromLong(l))

    set("nop",        _INS_NOP);

    set("return",     _INS_RETURN);
    set("store",      _INS_STORE_NAME);

    set("load_const", _INS_LOAD_CONST);
    set("load_name",  _INS_LOAD_NAME);
    set("load_long",  _INS_LOAD_LONG);

    set("binary_add", _INS_BINARY_ADD);
    set("binary_sub", _INS_BINARY_SUB);
    set("binary_mul", _INS_BINARY_MUL);
    set("binary_div", _INS_BINARY_DIV);

    set("cmp",        _INS_CMP);
    set("not",        _INS_NOT);

    set("jmp_true",   _INS_JMP_TRUE);

    set("goto",       _INS_GOTO);
    set("print",      _INS_PRINT);

    return dict;
}

static PyMethodDef _PyBasicMethods[] = {
    {"set_tokenizer", set_tokenizer, METH_VARARGS, "Set the default tokenizer used."},
    {"get_bytecodes", get_bytecode_instructions, METH_VARARGS, "Get all bytecode instructions as a dict (name -> bytecode)."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef _PyBasicModule = {
    PyModuleDef_HEAD_INIT,
    "_pybasic",   /* name of module */
    NULL,         /* module documentation, may be NULL */
    -1,           /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    _PyBasicMethods
};

PyMODINIT_FUNC
PyInit__pybasic(void)
{
    PyObject *module;
    if (PyType_Ready(&ByteCodeInterpreterType) < 0) {
        return NULL;
    }

    module = PyModule_Create(&_PyBasicModule);
    if (module == NULL) {
        return NULL;
    }

    Py_INCREF(&ByteCodeInterpreterType);
    PyModule_AddObject(module, "ByteCodeInterpreter", (PyObject *) &ByteCodeInterpreterType);
    return module;
}

#include <Python.h>

#define PYBASIC_CORE
#include "_pybasic.h"
#include "bvm/ins.h"

static PyObject *_ins_dict;

static PyObject *
set_tokenizer(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "O", &tokenizer)) {
        return NULL;
    }

    Py_RETURN_NONE;
}

static void populate_ins_dict(PyObject *dict) {
    PyDict_SetItem(dict, PyUnicode_FromString("nop"),        PyLong_FromLong(_INS_NOP));
    PyDict_SetItem(dict, PyUnicode_FromString("return"),     PyLong_FromLong(_INS_RETURN));

    
    PyDict_SetItem(dict, PyUnicode_FromString("store"),      PyLong_FromLong(_INS_STORE_NAME));
    PyDict_SetItem(dict, PyUnicode_FromString("load"),       PyLong_FromLong(_INS_LOAD_NAME));

    PyDict_SetItem(dict, PyUnicode_FromString("goto"),       PyLong_FromLong(_INS_GOTO));
    PyDict_SetItem(dict, PyUnicode_FromString("print"),      PyLong_FromLong(_INS_PRINT));

    PyDict_SetItem(dict, PyUnicode_FromString("data_end"),      PyLong_FromLong(_INS_DATA_END));
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

static PyObject *
get_all_bytecode_instructions(PyObject *self, PyObject *args)
{
    PyObject *dict = PyDict_New();
    Py_INCREF(dict);

    populate_ins_dict(dict);
    return dict;
}

static PyMethodDef BPyBasicMethods[] = {
    {"set_tokenizer", set_tokenizer, METH_VARARGS, "Set the default tokenizer used."},
    {"get_bytecode", get_bytecode_instruction, METH_VARARGS, "Get the bytecode for an instruction name."},
    {"get_all_bytecodes", get_all_bytecode_instructions, METH_VARARGS, "Get all bytecode instructions as a dict (name => bytecode)."},
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

    populate_ins_dict(_ins_dict);


    Py_INCREF(&ByteCodeInterpreterType);
    PyModule_AddObject(module, "ByteCodeInterpreter", (PyObject *) &ByteCodeInterpreterType);
    return module;
}

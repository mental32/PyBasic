#include <Python.h>

#define PYBASIC_CORE
#include "_pybasic.h"
#include "bvm/ins.h"

static void populate_ins_dict(PyObject *dict) {
    PyDict_SetItem(dict, PyUnicode_FromString("nop"),        PyLong_FromLong(_INS_NOP));
    PyDict_SetItem(dict, PyUnicode_FromString("return"),     PyLong_FromLong(_INS_RETURN));

    
    PyDict_SetItem(dict, PyUnicode_FromString("store"),      PyLong_FromLong(_INS_STORE_NAME));

    PyDict_SetItem(dict, PyUnicode_FromString("load_const"), PyLong_FromLong(_INS_LOAD_CONST));
    PyDict_SetItem(dict, PyUnicode_FromString("load_name"),  PyLong_FromLong(_INS_LOAD_NAME));

    PyDict_SetItem(dict, PyUnicode_FromString("load_long"),  PyLong_FromLong(_INS_LOAD_LONG));
    PyDict_SetItem(dict, PyUnicode_FromString("load_short"), PyLong_FromLong(_INS_LOAD_SHORT));
    PyDict_SetItem(dict, PyUnicode_FromString("load_byte"),  PyLong_FromLong(_INS_LOAD_BYTE));

    PyDict_SetItem(dict, PyUnicode_FromString("binary_add"), PyLong_FromLong(_INS_BINARY_ADD));
    PyDict_SetItem(dict, PyUnicode_FromString("binary_sub"), PyLong_FromLong(_INS_BINARY_SUB));
    PyDict_SetItem(dict, PyUnicode_FromString("binary_mul"), PyLong_FromLong(_INS_BINARY_MUL));
    PyDict_SetItem(dict, PyUnicode_FromString("binary_div"), PyLong_FromLong(_INS_BINARY_DIV));

    PyDict_SetItem(dict, PyUnicode_FromString("cmp"),        PyLong_FromLong(_INS_CMP));
    PyDict_SetItem(dict, PyUnicode_FromString("not"),        PyLong_FromLong(_INS_NOT));

    PyDict_SetItem(dict, PyUnicode_FromString("jmp_true"), PyLong_FromLong(_INS_JMP_TRUE));

    PyDict_SetItem(dict, PyUnicode_FromString("goto"),       PyLong_FromLong(_INS_GOTO));
    PyDict_SetItem(dict, PyUnicode_FromString("print"),      PyLong_FromLong(_INS_PRINT));
}

static PyObject *
token_type(PyObject *self, PyObject *args)
{
    PyObject *tp;
    const char *token;
    char *end;

    if (!PyArg_ParseTuple(args, "s", &token)) {
        return NULL;
    }

    size_t length = strlen(token);

    if (length == 1) {
        if (((int) *token) >= 48 && ((int) *token) <= 57) {
            tp = PyUnicode_FromString("I");
        } else {
            tp = PyUnicode_FromString("V");
        }
    }

    else if (token[0] == '"') {
        if (token[length - 1] == '"') {
            tp = PyUnicode_FromString("S");
        } else {
            PyErr_SetString(PyExc_ValueError, "Invalid token.");
            return NULL;
        }
    }

    else if (strtol(token, &end, 10)) {
        tp = PyUnicode_FromString("I");
    }

    else {
        tp = PyUnicode_FromString("U");
    }

    Py_INCREF(tp);
    return tp;
}

static PyObject *
set_tokenizer(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "O", &tokenizer)) {
        return NULL;
    }

    Py_RETURN_NONE;
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
    {"type", token_type, METH_VARARGS, "Get the type of a token."},
    {"set_tokenizer", set_tokenizer, METH_VARARGS, "Set the default tokenizer used."},
    {"get_bytecodes", get_all_bytecode_instructions, METH_VARARGS, "Get all bytecode instructions as a dict (name -> bytecode)."},
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

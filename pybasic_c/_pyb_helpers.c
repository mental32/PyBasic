#include <Python.h>

#include "_pybasic.h"

void *
Println(const char *string)
{
    PyObject *stdout = PySys_GetObject("stdout");

    PyFile_WriteString(string, stdout);
    PyFile_WriteString("\n", stdout);
}

PyObject *
IsInteger(PyObject *self, PyObject *args)
{
    const char *data;
    char *end;
    long tmp;

    if (!PyArg_ParseTuple(args, "s", &data)) {
        return NULL;
    }

    if (!strtol(data, &end, 10)) {
        Py_RETURN_FALSE;
    }

    Py_RETURN_TRUE;
}

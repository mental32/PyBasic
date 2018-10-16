#include <Python.h>

void
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

    if (!PyArg_ParseTuple(args, "s", &data)) {
        return NULL;
    }

    if (!strtol(data, &end, 10)) {
        Py_RETURN_FALSE;
    }

    Py_RETURN_TRUE;
}

PyObject *
IsString(PyObject *self, PyObject *args) {
    Py_RETURN_TRUE;
}

PyObject *
IsConstant(PyObject *self, PyObject *args)
{
    const char *data;
    char *end;

    if (!PyArg_ParseTuple(args, "s", &data)) {
        return NULL;
    }

    if (strtol(data, &end, 10) || PyObject_IsTrue(IsString(self, args))) {
        Py_RETURN_TRUE;
    }

    Py_RETURN_FALSE;
}

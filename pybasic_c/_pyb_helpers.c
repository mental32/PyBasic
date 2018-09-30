#include <Python.h>

#include "_pybasic.h"

void *
Println(const char *string)
{
    PyObject *stdout = PySys_GetObject("stdout");

    PyFile_WriteString(string, stdout);
    PyFile_WriteString("\n", stdout);
}

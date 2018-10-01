#ifndef Py_BASIC_H
#define Py_BASIC_H

#ifdef PYBASIC_CORE
/* This section is used when compiling _pybasic.c */

PyTypeObject ByteCodeInterpreterType;
PyObject *parse_string(PyObject *self, PyObject *args);
PyObject *IsInteger(PyObject *self, PyObject *args);

#else
/* This section is used in modules that use PyBasic's API */

void *Println(const char *string);
PyObject *parse_string(PyObject *self, PyObject *args);

#endif
#endif

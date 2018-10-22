#ifndef PYBASIC_H
#define PYBASIC_H

#include <Python.h>

PyObject *tokenizer;

#ifdef PYBASIC_CORE
/* This section is used when compiling _pybasic.c */

PyTypeObject ByteCodeInterpreterType;
PyObject *IsString(PyObject *self, PyObject *args);
PyObject *IsInteger(PyObject *self, PyObject *args);
PyObject *IsConstant(PyObject *self, PyObject *args);

#else
/* This section is used in modules that use PyBasic's API */

void Println(const char *string);
void _parse_source_to_bytecode(uint8_t *buf, const char *source);

#endif
#endif

#ifndef PYBASIC_H
#define PYBASIC_H

#include <Python.h>

PyObject *tokenizer;

#ifdef PYBASIC_CORE

PyTypeObject ByteCodeInterpreterType;

#else

void Println(const char *string);

#endif
#endif

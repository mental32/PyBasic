#ifndef PYBASIC_MACROS_H
#define PYBASIC_MACROS_H

#include <Python.h>

#define set(dict, name, value) PyDict_SetItem(dict, name, value)
#define string(o) PyUnicode_FromString(o)
#define integer(i) PyLong_FromLong(i)

#endif

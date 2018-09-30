#ifndef Py_BASIC_H
#define Py_BASIC_H

#ifdef PYBASIC_CORE
/* This section is used when compiling _pybasic.c */

PyTypeObject ByteCodeInterpreterType;

#else
/* This section is used in modules that use PyBasic's API */

void *Println(const char *string);

#endif
#endif

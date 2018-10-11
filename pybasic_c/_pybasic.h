#ifndef Py_BASIC_H
#define Py_BASIC_H

#ifdef PYBASIC_CORE
/* This section is used when compiling _pybasic.c */

PyTypeObject ByteCodeInterpreterType;
PyObject *IsString(PyObject *self, PyObject *args);
PyObject *IsInteger(PyObject *self, PyObject *args);
PyObject *IsConstant(PyObject *self, PyObject *args);

#else
/* This section is used in modules that use PyBasic's API */

typedef struct {
    PyObject_HEAD
    PyObject *_data;
    uint16_t ip;
} ByteCodeInterpreter;

void Println(const char *string);
void _parse_source_to_bytecode(uint8_t *buf);

inline PyObject *
Py_RETURN_PyLong_From_PyBVM_IP(ByteCodeInterpreter *self) {
    PyObject *insp = PyLong_FromLong((long) self->ip);
    Py_INCREF(insp);
    return insp;
}

#endif
#endif

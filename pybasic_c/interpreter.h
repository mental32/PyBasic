#ifndef PYBASIC_INTERPRETER_H
#define PYBASIC_INTERPRETER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    PyObject_HEAD
    PyObject *_data;
    PyObject *_constants;
    uint16_t ip;
    bool _running;
} ByteCodeInterpreter;

#endif

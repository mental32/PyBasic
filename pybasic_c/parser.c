#include <stdint.h>

#include "_pybasic.h"
#include "instructions.h"
#include "interpreter.h"

void
Println(const char *string)
{
    PyObject *stdout = PySys_GetObject("stdout");

    PyFile_WriteString(string, stdout);
    PyFile_WriteString("\n", stdout);
}

// RE_DIGIT = r'((\d|\d_)\d)'
// RE_PRINT = r'(print|PRINT) (\(.*\)|.*)'
// RE_GOTO  = r'(goto|GOTO) (\d{1,3})'
// RE_LET   = r'(let|LET) ([A-Za-z_]*)( = |=)(.*)'
// RE_IF    = r'(if|IF) (.*) (goto|GOTO) (\d{1,3})'
// RE_FOR   = r'(for|FOR) (.*) (to|TO) (.*) ((step|STEP) \d{1,9}|)'

int bytecode_parse_source(ByteCodeInterpreter *vm, uint8_t *buf, const char *source)
{
    PyObject *tmp, *line, *rows = PyObject_CallFunctionObjArgs(tokenizer, PyUnicode_FromString(source), NULL);
    const char *token;
    int bufptr = 0;

    for (Py_ssize_t row = 0; row < PySequence_Length(rows); row++) {
        // [10, x, =, 5]
        line = PyList_GetItem(rows, row);
        Py_ssize_t index = 1, length = PySequence_Length(line);

        if (length < 0) {
            continue;
        }

        long label = strtol(PyUnicode_AsUTF8(PyList_GetItem(line, index++)), &token, 10);

        if (label < 0) {
            PyErr_SetString(PyExc_SyntaxError, "PyBasic: Label must be greater than zero.");
            return 0;
        }

        if (label < 255) {
            buf[bufptr++] = _INS_LABEL_BYTE;
            buf[bufptr++] = (uint8_t) label;
        } else if (label < 65535) {
            buf[bufptr++] = _INS_LABEL_SHORT;
            buf[bufptr++] = (uint8_t) (label & 0xff);
            buf[bufptr++] = (uint8_t) (label >> 8);
        } else {
            PyErr_SetString(PyExc_SyntaxError, "PyBasic: Label must be less than 65,535");
            return 0;
        }

        while (index < length) {
            token = PyUnicode_AsUTF8(PyList_GetItem(line, index));
            index++;
        }
    }

    return 1;
}

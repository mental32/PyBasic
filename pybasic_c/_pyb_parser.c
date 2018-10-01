#include <Python.h>
#include <ctype.h>
#include <stdbool.h>

#include "_pybasic.h"

char* read_while(const char* source, size_t* size, bool predicate(char)) {
  *size = 0;
  const char* start = source;
  while (predicate(*source++))
    (*size)++;
  return *size == 0 ? NULL : start;
}

PyObject *
parse_string(PyObject *self, PyObject *args)
{
    const char *source;
    char* end;
    long LN_DEF;

    if (!PyArg_ParseTuple(args, "s", &source)) {
        return NULL;
    }

    
    

    size_t token_size;
    char* token_text = read_while(source, &token_size, isdigit);
    printf("%s\n", token_text);

    // LN_DEF = strtol(source, &end, 10);
    // if (!LN_DEF) {
    //     PyErr_SetString(PyExc_RuntimeError, "Failed to find line number in source");
    //     return NULL;
    // }

    // const int n = snprintf(NULL, 0, "%ld", LN_DEF);
    // char buf[n+1];
    // int c = snprintf(buf, n+1, "%ld", LN_DEF);
    // printf("%ld %ld\n", LN_DEF, strlen(buf));

    // for (size_t i = 0; i < strlen(source); i++) {}
    // printf("%ld\n", LN_DEF);

    Py_RETURN_NONE;
}

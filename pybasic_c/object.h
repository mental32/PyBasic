#ifndef PYBASIC_OBJECT_H
#define PYBASIC_OBJECT_H

#include <stddef.h>
#include <stdint.h>

enum ObjectType {
    _obj_tp_long,
    _obj_tp_str,
};

typedef struct {
    uint8_t tp;
    void *ptr;
} Object;

typedef struct {
    Object *value;
    PyObject *key;
} Binding;

typedef struct {
    long ip;
    long name;
} Label;

Binding *newBinding(Object *value, PyObject *key) {
    Binding *bind = (Binding *) malloc(sizeof(Binding));
    bind->value = value;
    bind->key = key;

    return bind;
}

Object *newObject(uint8_t type, void *ptr) {
    Object *obj = (Object*) malloc(sizeof(Object));
    obj->tp = type;
    obj->ptr = ptr;

    return obj;
}

void delObject(Object *obj) {
    free(obj->ptr);
    obj->ptr = NULL;

    free(obj);
    return;
}

#endif

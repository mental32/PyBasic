#ifndef PY_BASIC_OBJECT_H
#define PY_BASIC_OBJECT_H

typedef struct {
    uint8_t tp;
    void *ptr;
} Object;

enum ObjectType {
    _obj_tp_long,
    _obj_tp_str,
};

Object *newObject(uint8_t type, void *ptr) {
    Object* obj = (Object*) malloc(sizeof(Object));
    obj->tp = type;
    obj->ptr = ptr;

    return obj;
}

void delObject(Object *obj) {
    free(obj->ptr);
    free(obj);
    return;
}

#endif

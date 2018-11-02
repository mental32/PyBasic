#ifndef _BASIC_OBJECT_H
#define _BASIC_OBJECT_H

typedef struct {
    uint8_t tp;
    void *ptr;
} Object;

static inline Object *NewObject(uint8_t tp, void *ptr) {
    Object *obj = malloc(sizeof(Object));
    obj->tp = tp;
    obj->ptr = ptr;
    return obj;
}

static inline Object *RebaseObject(Object *obj, uint8_t tp, void *ptr) {
    obj->tp = tp;
    obj->ptr = ptr;
    return obj;
}

#endif

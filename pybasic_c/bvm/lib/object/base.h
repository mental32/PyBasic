#ifndef __BASIC_OBJECT_DEF_H
#define __BASIC_OBJECT_DEF_H

#include <stdint.h>
#include "../types.h"

typedef struct {
    uint8_t tp;
    uint8_t rc;
    void *ptr;
} Object;

Object *Object_New(uint8_t tp, void *data);
int Object_Free(Object *obj);
Object *Object_Rebase(Object *src, Object *dst);
Object *Object_rebase(Object *dst, uint8_t tp, void *data, uint8_t rc);

#define Object_INCREF(o) o->rc++
#define Object_DECREF(o) o->rc--
#endif 

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

/* Increase an objects refrence count (without any checks) */
#define Object_INCREF(o) o->rc++

/* If the object is not null, increase its refrence count. */
#define Object_XINCREF(o) if (o != NULL) o->rc++

/* Decrease an objects refrence count (without any checks) */
#define Object_DECREF(o) o->rc--

/* If the object is not null, decrease its refrence count. */
#define Object_XDECREF(o) if (o != NULL) o->rc--

/* If decreasing the objects refrence count will cause it to be forgotten, it is free'd. */
#define Object_FDECREF(o) if (!--o->rc) Object_Free(o)

/* Chains Object_XDECREF (null check) and Object_FDECREF (free if forgotten). */
#define Object_UDECREF(o) if (o != NULL && !(--o->rc)) Object_Free(o)

#endif 

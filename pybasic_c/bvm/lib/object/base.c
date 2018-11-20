#include <stdlib.h>

#include "base.h"

/*
Constructor for an Object.
*/
Object *Object_New(uint8_t tp, void *data)
{
    Object *obj = malloc(sizeof(Object));
    obj->tp = tp;
    obj->rc = 0;
    obj->ptr = data;

    return obj;
}

/*
Destructor for an object.
*/
int Object_Free(Object *obj)
{
    if (!obj) {
        return 0;
    }

    free(obj);
    return 1;
}

/*
Mirror the contents of one object onto another (Effectively a memcpy).
NOTE: This will discard the src->ptr, DO NOT LOSE THE src->ptr UNLESS YOU LIKE MEMORY LEAKS.
*/
Object *Object_Rebase(Object *dst, Object *src)
{
    dst->tp = src->tp;
    dst->rc = src->rc;
    dst->ptr = src->ptr;

    return dst;
}

/*
Modify the contents of one object with that properties of an abstract Object.#
NOTE: This will discard the src->ptr, DO NOT LOSE THE src->ptr UNLESS YOU LIKE MEMORY LEAKS.
*/
Object *Object_rebase(Object *dst, uint8_t tp, void *ptr, uint8_t rc)
{
    dst->tp = tp;
    dst->rc = rc;
    dst->ptr = ptr;

    return dst;
}

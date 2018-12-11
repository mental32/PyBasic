#include <stdlib.h>

#include "base.h"

/*
Constructor for an Object.

Parameters
----------
 uint8_t tp - The type of the Object.
 void *data - The data it should represent.
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

Parameters
----------
 Object *obj - The Object to free.
*/
void Object_Free(Object *obj)
{
    if (!obj) {
        return;
    }

    free(obj);
}

/*
Mirror the contents of one object onto another (Effectively a memcpy).
NOTE: This will discard the src->ptr, DO NOT LOSE THE src->ptr UNLESS YOU LIKE MEMORY LEAKS.

Parameters
----------
 Object *dst - The Object we're modifying.
 Object *src - The source Object we're reading.
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

Parameters
----------
 Object *dst - The Object we're writing to.
 uint8_t tp - The type of that object.
 void *ptr - The data the Object should represent.
 uint8_t rc - The refrence count for the Object.
*/
Object *Object_rebase(Object *dst, uint8_t tp, void *ptr, uint8_t rc)
{
    dst->tp = tp;
    dst->rc = rc;
    dst->ptr = ptr;

    return dst;
}

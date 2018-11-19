#ifndef _BASIC_OBJECT_H
#define _BASIC_OBJECT_H

#include <stdlib.h>
#include <stdint.h>

#include "types.h"

typedef struct {
    uint8_t tp;
    uint8_t rc;
    void *ptr;
} Object;

#define Object_INCREF(obj) obj->rc++
#define Object_DECREF(obj) obj->rc--

static inline Object *new_object(uint8_t tp, void *ptr) {
    Object *obj = malloc(sizeof(Object));
    obj->tp = tp;
    obj->ptr = ptr;
    obj->rc = 0;
    return obj;
}

static inline Object *rebase_object(Object *obj, uint8_t tp, void *ptr) {
	if (!obj) {
		return NULL;
	}

    obj->tp = tp;
    obj->ptr = ptr;
    return obj;
}

static inline void free_object(Object *obj) {
	if (!obj) {
		return;
	}

	free(obj);
}

#endif

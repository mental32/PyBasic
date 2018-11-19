#include <stdlib.h>

#include "base.h"

Object *Object_New(uint8_t tp, void *data)
{
    Object *obj = malloc(sizeof(Object));
    obj->tp = tp;
    obj->rc = 0;
    obj->ptr = data;

    return obj;
}

int Object_Free(Object *obj)
{
	if (!obj) {
		return 0;
	}

	free(obj);
	return 1;
}

Object *Object_Rebase(Object *src, Object *dst)
{

}

Object *Object_rebase(uint8_t tp, void *data, uint8_t rc)
{
	
}

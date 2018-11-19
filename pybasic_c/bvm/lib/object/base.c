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
*/
Object *Object_Rebase(Object *src, Object *dst)
{

}

/*
Modify the contents of one object with that properties of an abstract Object.
*/
Object *Object_rebase(uint8_t tp, void *data, uint8_t rc)
{
	
}

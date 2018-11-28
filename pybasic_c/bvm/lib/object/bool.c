#include <string.h>

#include "base.h"

Object *Object_New(uint8_t tp, void *data);
int Object_TypeCheck(Object *left, Object *right);

static const _Bool _true = 1;
static const _Bool _false = 0;
const _Bool *True = &_true;
const _Bool *False = &_false;


/*
Create a new Object with a boolean type.

Parameters
----------
 int n - The value to represent as a bool.
*/
Object *Object_Bool(int n)
{
    if (n)
        return Object_New(generic_bool, True);
    return Object_New(generic_bool, False);
}

/*
Determine if an object is true (1) or false (0)

Parameters
----------
 Object *obj - The Object to examine.
*/
int Object_bool(Object *obj)
{
    if (is_int(obj)) {
        return ((long)obj->ptr) != 0;
    } 

    else if (is_str(obj)) {
        return strlen((char*)obj->ptr) != 0;
    }

    return 0;
}

/*
Compare two objects.
Objects are only equal if they are of the same type and contain the same data.

Parameters
----------
 Object *left - The Object we're comparing
 Object *right - The second Object we're comparing against.
*/
int Object_Compare(Object *left, Object *right)
{
    int type = Object_TypeCheck(left, right);

    if (!type) goto exit;

    if (type & generic_int)
        return (long)left->ptr == (long)right->ptr;

    if (type & generic_str)
        return !strcmp((char*)left->ptr, (char*)right->ptr);

exit:
    return 0;
}

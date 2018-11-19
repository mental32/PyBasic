#include <string.h>

#include "base.h"

Object *Object_New(uint8_t tp, void *data);
int Object_TypeCheck(Object *left, Object *right);

const _Bool _true = 1;
const _Bool _false = 0;
const _Bool *True = &_true;
const _Bool *False = &_false;


/*
Create a new Object with a boolean type.
*/
Object *Object_Bool(int n)
{
    if (n)
        return Object_New(generic_bool, True);
    return Object_New(generic_bool, False);
}

/*
Determine if an object is true (1) or false (0)
*/
int Object_bool(Object *obj)
{
    if (is_int(obj)) {
        return DEREF_INT(obj) != 0;
    } else if (is_str(obj)) {
        return strlen(DEREF_STR(obj)) != 0;
    }

    return 0;
}

/*
Compare two objects.

Objects are only equal if they are of the same type and contain the same data.
*/
int Object_Compare(Object *left, Object *right)
{
    int type = Object_TypeCheck(left, right);

    if (!type) goto exit;

    if (type & generic_int)
    {
        return DEREF_INT(left) == DEREF_INT(right);
    }

    if (type & generic_str)
    {
        return !strcmp(DEREF_STR(left), DEREF_STR(right));
    }

exit:
    return 0;
}

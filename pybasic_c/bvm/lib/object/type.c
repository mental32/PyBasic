#include "base.h"

/*
Perform a strict type check on two Objects.
If their types match return that type flag, otherwise return 0.

Parameters
----------
 Object *left - The Object we're comparing.
 Object *right - The second Object we're comparing against.
*/
int Object_TypeCheck(Object *left, Object *right)
{
    if (!(left->tp - right->tp)) {
        return left->tp;
    }

    return 0;
}

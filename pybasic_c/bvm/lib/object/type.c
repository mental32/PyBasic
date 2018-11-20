#include "base.h"

/*
Perform a strict type check on two Objects.

If their types match return that type flag, otherwise return 0.
*/
int Object_TypeCheck(Object *left, Object *right)
{
    if (!(left->tp - right->tp)) {
        return left->tp;
    }

    return 0;
}

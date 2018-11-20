#include "base.h"

/*
Constructor for an integer.
*/
Object *Object_Integer(long n)
{
    return Object_New(generic_int, (void *) n);
}

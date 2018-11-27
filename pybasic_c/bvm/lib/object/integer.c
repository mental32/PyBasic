#include "base.h"

/*
Constructor for an integer.

Parameters
----------
 long n - The integer to represet.
*/
Object *Object_Integer(long n)
{
    return Object_New(generic_int, (void *) n);
}

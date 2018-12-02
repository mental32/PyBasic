#include "base.h"

/*
Constructor for a string Object.

Parameters
----------
 char *str - The string to represent.
*/
Object *Object_String(char *str)
{
    return Object_New(generic_str, str);
}

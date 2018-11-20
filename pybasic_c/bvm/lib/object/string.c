#include "base.h"

/*
Constructor for a string Object.
*/
Object *Object_String(char *str)
{
    return Object_New(generic_str, str);
}

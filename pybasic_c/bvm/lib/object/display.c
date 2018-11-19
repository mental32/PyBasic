#include <stdio.h>

#include "base.h"

/*
format and write an objects data to stdout (no newline).
*/
void Object_print(Object *obj)
{
    // Gotta check for dem null pointers
    if (!obj) {
        return;
    }

    switch(obj->tp) {
        case generic_str: {
            printf("%s", (char*)obj->ptr);
            break;
        }

        case generic_int: {
            printf("%ld", *((long*)obj->ptr));
            break;
        }

        default: {
            // Should we be doing something else
            // apart from writing garbage?
            printf("{(nil)?(%d)}", obj->tp);
        }
    }
}

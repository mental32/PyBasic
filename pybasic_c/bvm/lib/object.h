#ifndef _BASIC_OBJECT_H
#define _BASIC_OBJECT_H

#include "object/base.h"

#define Object_INCREF(obj) obj->rc++
#define Object_DECREF(obj) obj->rc--

/* object/bool.c */
Object *Object_Bool(int n);
int Object_bool(Object *obj);
int Object_Compare(Object *left, Object *right);

/* object/display.c */
void Object_print(Object *obj);

/* object/float.c */

/* object/integer.c */
Object *Object_Integer(long n);

/* object/string.c */
Object *Object_String(char *str);

/* object/type.c */
int Object_TypeCheck(Object *left, Object *right);

#endif

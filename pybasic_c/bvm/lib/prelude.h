#ifndef _BASIC_PRELUDE_H
#define _BASIC_PRELUDE_H

#include "object.h"

/*
Get the absolute value of a number.
*/
inline unsigned long BASIC_ABS(Object *obj)
{
	if (!is_number(obj)) return NULL;

}

/*
Get the Arctangent value of a number (result in radians).
*/
inline Object *BASIC_ATN(Object *obj)
{

}

/*
Get the Cosine value of a number (argument in radians).
*/
inline float BASIC_COS(Object *obj)
{

}

/*
Get the exponential value.
*/
inline float BASIC_POW(Object *b, Object *x)
{

}

#endif

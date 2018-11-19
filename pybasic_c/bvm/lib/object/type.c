#include "base.h"

/*
*/
int Object_TypeCheck(Object *left, Object *right)
{
	if (!(left->tp - right->tp)) {
		return left->tp;
	}

	return 0;
}

#ifndef __BASIC_OBJECT_DEF_H
#define __BASIC_OBJECT_DEF_H

#include <stdint.h>
#include "../types.h"

typedef struct {
    uint8_t tp;
    uint8_t rc;
    void *ptr;
} Object;

#endif 

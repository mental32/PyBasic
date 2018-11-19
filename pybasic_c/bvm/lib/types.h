#ifndef _BASIC_TYPES_H
#define _BASIC_TYPES_H

enum object_type {
    u8  = 1,
    u16 = 2,
    u32 = 4,

    generic_ref = 8,
    generic_str = 16,
    generic_int = 32,
    generic_flt = 64,
    generic_bool = 128,
};

#define STRING   (generic_str)
#define INTEGER  (u32 | generic_int)

#define is_int(o) ((o->tp) & generic_int)
#define is_str(o) ((o->tp) & generic_str)

#define DEREF_INT(o) (*(long*)o->ptr)
#define DEREF_STR(o) ((char*)o->ptr)

const _Bool _true;
const _Bool _false;
const _Bool *True;
const _Bool *False;

#endif

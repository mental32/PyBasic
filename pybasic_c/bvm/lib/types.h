#ifndef _BASIC_TYPES_H
#define _BASIC_TYPES_H

enum object_type {
    generic_ref = 1,
    generic_str = 2,
    generic_int = 4,
    generic_flt = 8,

    generic_bool = 16,
};

#define STRING   (generic_str)
#define NUMBER  (generic_flt | generic_int)

#define is_int(o) ((o->tp) & generic_int)
#define is_str(o) ((o->tp) & generic_str)
#define is_number(o) (((o->tp) & generic_flt) || is_int(o))

const _Bool *True;
const _Bool *False;

#endif

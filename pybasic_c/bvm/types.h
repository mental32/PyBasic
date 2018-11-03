#ifndef _BASIC_TYPES_H
#define _BASIC_TYPES_H

enum ObjectType {
    u8  = 1,
    u16 = 2,
    u32 = 4,

    generic_ref = 8,
    generic_str = 16,
    generic_int = 32,
    generic_flt = 64,

    _obj_tp_literal = 128,
};

#define STRING (generic_str)

#define BYTE  (u8  | generic_int)
#define SHORT (u16 | generic_int)
#define LONG  (u32 | generic_int)

#define _BYTE  (BYTE  | _obj_tp_literal)
#define _SHORT (SHORT | _obj_tp_literal)
#define _LONG  (LONG  | _obj_tp_literal)

#define IS_INT(o) ((o->tp) | generic_int)
#define IS_STR(o) ((o->tp) | generic_str)

static const _Bool _true = 1;
static const _Bool _false = 0;
static const _Bool *True = &_true;
static const _Bool *False = &_false;

#endif

#ifndef _BASIC_TYPES_H
#define _BASIC_TYPES_H

enum ObjectType {
    _obj_tp_u8 = 1,
    _obj_tp_u16,
    _obj_tp_u32,

    _obj_tp_generic_ref,
    _obj_tp_generic_int,
    _obj_tp_generic_str,
    _obj_tp_generic_flt,
    _obj_tp_generic_bool,

    _obj_tp_literal = 128,
};

#define STRING (_obj_tp_generic_str)
#define BOOL   (_obj_tp_generic_bool)

#define BYTE  (_obj_tp_u8  | _obj_tp_generic_int)
#define SHORT (_obj_tp_u16 | _obj_tp_generic_int)
#define LONG  (_obj_tp_u32 | _obj_tp_generic_int)

#define _BYTE  (BYTE  | _obj_tp_literal)
#define _SHORT (SHORT | _obj_tp_literal)
#define _LONG  (LONG  | _obj_tp_literal)
#define _BOOL  (BOOL  | _obj_tp_literal)

#define IS_LIT(o) ((o->tp) | _obj_tp_literal)
#define IS_INT(o) ((o->tp) | _obj_tp_generic_int)
#define IS_STR(o) ((o->tp) | _obj_tp_generic_str)

#endif
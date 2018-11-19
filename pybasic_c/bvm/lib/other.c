static inline int GetIntValue(Object *obj) {
    if (obj->tp == _BYTE) {
        return (uint8_t)obj->ptr;
    } else if (obj->tp == _LONG) {
        return (long)obj->ptr;
    } else {
        return *(uint8_t*)obj->ptr;
    }
}

static inline uint8_t ObjectIsTrue(Object *obj) {
    if (IS_INT(obj)) {
        return (uint8_t)(GetIntValue(obj) != 0);
    } else if (IS_STR(obj)) {
        return (uint8_t)(strlen((char*)obj->ptr) != 0);
    }
}

static inline void TailorInteger(Object *o, long value) {
    if (value > 0 && value < 255) {
        RebaseObject(o, _BYTE, (void*)value);
    } else {
        RebaseObject(o, _LONG, (void*)value);
    }
}

static inline int TypeCheckExact(Object *a, Object *b) {
    if (a->tp & generic_str && b->tp & generic_str) {
        return generic_str;
    } else if (IS_INT(a) && IS_INT(b)) {
        return generic_int;
    }

    return 0;
}

static inline int CompareObjects(Object *a, Object *b) {
    int tp = TypeCheckExact(a, b);

    if (tp & generic_int) {
        return GetIntValue(a) == GetIntValue(b);
    }

    else if (tp & generic_str) {
        return !strcmp(((char*)a), ((char*)b));
    }

    return 0;
}

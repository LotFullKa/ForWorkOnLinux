#include <stdint.h>

typedef enum {
    PlusZero      = 0x00,
    MinusZero     = 0x01,
    PlusInf       = 0xF0,
    MinusInf      = 0xF1,
    PlusRegular   = 0x10,
    MinusRegular  = 0x11,
    PlusDenormal  = 0x20,
    MinusDenormal = 0x21,
    SignalingNaN  = 0x30,
    QuietNaN      = 0x31
} float_class_t;

enum {
    SHIFT_TO_ZERO = 63,
    BASE_MASK_FOR_EXPONENT = (uint64_t)~0 >> 53,
    SHIFT_TO_EXPONENT = 52,
    BASE_MASK_FOR_MANTIS = (uint64_t)~0 >> 12,
    MASK_FOR_NAN = (uint64_t)1 << 51,
};

typedef union {
    double     real_value;
    uint64_t   uint_value;
} real_or_uint;

extern float_class_t
classify(double *value_ptr){
    real_or_uint u;
    u.real_value = *value_ptr;
    uint64_t bits = u.uint_value;

    const uint64_t sign = (bits & ((uint64_t)1 << SHIFT_TO_ZERO));
    const uint64_t mask_for_exponents = ((uint64_t)BASE_MASK_FOR_EXPONENT << SHIFT_TO_EXPONENT);
    const uint64_t exponent = (bits & mask_for_exponents);
    const uint64_t mantis = bits & BASE_MASK_FOR_MANTIS;

    if (sign == 0 && exponent == 0 && mantis == 0) {
        return PlusZero;
    } else if (sign != 0 && exponent == 0 && mantis == 0) {
        return MinusZero;
    } else if (sign == 0 && exponent == mask_for_exponents && mantis == 0) {
        return PlusInf;
    } else if (sign != 0 && exponent == mask_for_exponents && mantis == 0) {
        return MinusInf;
    } else if (sign == 0 && exponent != mask_for_exponents && exponent != 0) {
        return PlusRegular;
    } else if (sign != 0 && exponent != mask_for_exponents && exponent != 0) {
        return MinusRegular;
    } else if (sign == 0 && exponent == 0) {
        return PlusDenormal;
    } else if (sign != 0 && exponent == 0) {
        return MinusDenormal;
    } else if (exponent == mask_for_exponents && (mantis & MASK_FOR_NAN) == 0) {
        return SignalingNaN;
    } else if (exponent == mask_for_exponents && (mantis & MASK_FOR_NAN) == MASK_FOR_NAN) {
        return QuietNaN;
    }
    return 0;
}


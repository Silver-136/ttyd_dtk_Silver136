#pragma once
#include <dolphin/types.h>

/* Recovered inline float classification and reciprocal-root refinement. */
static inline s32 marioMoveClassify(f32 value) {
    s32 bits = *(s32*)&value;
    switch (bits & 0x7F800000) {
    case 0x7F800000:
        if (bits & 0x7FFFFF) return 1;
        else return 2;
    case 0:
        if (bits & 0x7FFFFF) return 5;
        else return 3;
    default:
        return 4;
    }
}

static inline f32 marioMoveSqrt(f64 value) {
    extern f64 __frsqrte(f64);
    extern f32 __float_nan[];
    extern f32 float_0_80420020;
    f64 guess;
    if (value > float_0_80420020) {
        guess = __frsqrte(value);
        guess = 0.5 * guess * (3.0 - guess * guess * value);
        guess = 0.5 * guess * (3.0 - guess * guess * value);
        guess = 0.5 * guess * (3.0 - guess * guess * value);
        value = (f32)(value * guess);
    } else if (value < 0.0) {
        value = __float_nan[0];
    } else if (marioMoveClassify(value) == 1) {
        value = __float_nan[0];
    }
    return value;
}

#pragma once
#include "dolphin/types.h"

/* Recovered single-precision quadrant polynomials; US constant owners retained. */
static inline f32 evtSubSin(f32 angle) {
    extern const f32 float_3p1416_8041fe5c, float_1p5708_8041fe60, float_4p7124_8041fe64, float_0p00761_8041fe50, float_0p16605_8041fe54, float_0p03705_8041fe6c, float_0p4967_8041fe70, float_1_8041fe58;
    f32 a = float_0p00761_8041fe50;
    f32 b = float_0p16605_8041fe54;
    f32 one = float_1_8041fe58;
    f32 t, square, result;
    if (angle <= float_3p1416_8041fe5c) {
        if (angle <= float_1p5708_8041fe60) {
            t = angle;
            square = t * t;
            result = (a * square - b) * square + one;
            result *= t;
        } else {
            t = float_1p5708_8041fe60 - (angle - float_1p5708_8041fe60);
            square = t * t;
            result = (a * square - b) * square + one;
            result *= t;
        }
    } else {
        if (angle < float_4p7124_8041fe64) {
            t = angle - float_3p1416_8041fe5c;
            square = t * t;
            result = (a * square - b) * square + one;
            result *= t;
            result = -result;
        } else {
            t = float_1p5708_8041fe60 - (angle - float_4p7124_8041fe64);
            square = t * t;
            result = (a * square - b) * square + one;
            result *= t;
            result = -result;
        }
    }
    return result;
}

static inline f32 evtSubCos(f32 angle) {
    extern const f32 float_3p1416_8041fe5c, float_1p5708_8041fe60, float_4p7124_8041fe64, float_0p00761_8041fe50, float_0p16605_8041fe54, float_0p03705_8041fe6c, float_0p4967_8041fe70, float_1_8041fe58;
    f32 a = float_0p03705_8041fe6c;
    f32 b = float_0p4967_8041fe70;
    f32 one = float_1_8041fe58;
    f32 t, square, result;
    if (angle <= float_3p1416_8041fe5c) {
        if (angle <= float_1p5708_8041fe60) {
            t = angle;
            square = t * t;
            result = (a * square - b) * square + one;
        } else {
            t = float_1p5708_8041fe60 - (angle - float_1p5708_8041fe60);
            square = t * t;
            result = (a * square - b) * square + one;
            result = -result;
        }
    } else {
        if (angle < float_4p7124_8041fe64) {
            t = angle - float_3p1416_8041fe5c;
            square = t * t;
            result = (a * square - b) * square + one;
            result = -result;
        } else {
            t = float_1p5708_8041fe60 - (angle - float_4p7124_8041fe64);
            square = t * t;
            result = (a * square - b) * square + one;
        }
    }
    return result;
}


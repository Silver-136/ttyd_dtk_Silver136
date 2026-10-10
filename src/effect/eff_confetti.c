#include "effect/eff_confetti.h"

void* effConfettiEntry(f32 x, f32 y, f32 z, s32 type, s32 timer) {
    extern void* effConfettiN64Entry(f32, f32, f32, f32, s32, s32);
    extern const f32 float_1_80422a18;

    return effConfettiN64Entry(x, y, z, float_1_80422a18, type, timer);
}

const f32 float_1_80422a18 = 1.0f;

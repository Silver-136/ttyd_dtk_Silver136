#include "effect/n64/eff_ripple_n64.h"

void effRippleN64SetRxRz(void* effect, f32 rx, f32 rz) {
    void* work = *(void**)((s32)effect + 0xC);
    *(f32*)((s32)work + 0x2C) = rx;
    *(f32*)((s32)work + 0x30) = rz;
}

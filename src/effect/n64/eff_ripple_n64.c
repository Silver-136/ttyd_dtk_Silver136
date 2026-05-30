#include "effect/n64/eff_ripple_n64.h"

void effRippleN64SetRxRz(void* effect, f32 rx, f32 rz) {
    void* work = *(void**)((s32)effect + 0xC);
    *(f32*)((s32)work + 0x2C) = rx;
    *(f32*)((s32)work + 0x30) = rz;
}


u8 effRippleDisp(int param_1, int param_2) {
    return 0;
}


u8 effRippleMain(void* effEntry) {
    return 0;
}


void* effRippleN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 time) {
    return 0;
}

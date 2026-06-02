#include "cam_shift.h"


void camShiftMain(void* cam, void* mario, f32* out) {
}


void camShiftReset(void) {
    extern void* wp;
    extern f32 float_0_80421fe8;
    extern f32 float_300_80421fec;
    extern f32 float_3_80421ff0;
    extern f32 float_0p002_80421ff4;
    f32 zero = float_0_80421fe8;
    f32 dist = float_300_80421fec;
    f32 three = float_3_80421ff0;
    f32 small = float_0p002_80421ff4;

    *(f32*)((s32)wp + 0x0) = zero;
    *(f32*)((s32)wp + 0x4) = zero;
    *(f32*)((s32)wp + 0x8) = zero;
    *(f32*)((s32)wp + 0xC) = zero;
    *(f32*)((s32)wp + 0x10) = zero;
    *(f32*)((s32)wp + 0x20) = dist;
    *(f32*)((s32)wp + 0x24) = three;
    *(f32*)((s32)wp + 0x28) = small;
    *(f32*)((s32)wp + 0x2C) = zero;
    *(f32*)((s32)wp + 0x30) = zero;
}


void camShiftInit(void) {
    extern void* wp;
    extern f32 float_0_80421fe8;
    extern f32 float_300_80421fec;
    extern f32 float_3_80421ff0;
    extern f32 float_0p002_80421ff4;
    f32 zero = float_0_80421fe8;
    f32 dist = float_300_80421fec;
    f32 three = float_3_80421ff0;
    f32 small = float_0p002_80421ff4;

    *(f32*)((s32)wp + 0x0) = zero;
    *(f32*)((s32)wp + 0x4) = zero;
    *(f32*)((s32)wp + 0x8) = zero;
    *(f32*)((s32)wp + 0xC) = zero;
    *(f32*)((s32)wp + 0x10) = zero;
    *(f32*)((s32)wp + 0x20) = dist;
    *(f32*)((s32)wp + 0x24) = three;
    *(f32*)((s32)wp + 0x28) = small;
    *(f32*)((s32)wp + 0x2C) = zero;
    *(f32*)((s32)wp + 0x30) = zero;
}


void camShiftPostMain(void) {
    extern void* gp;
    extern void* wp;
    void* g;
    void* w;
    s32 temp;

    g = gp;
    w = wp;
    temp = *(s32*)((s32)g + 0x38);
    *(s32*)((s32)w + 0x1C) = *(s32*)((s32)g + 0x3C);
    *(s32*)((s32)w + 0x18) = temp;
}

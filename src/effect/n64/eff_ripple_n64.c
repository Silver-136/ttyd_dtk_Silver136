#include "effect/n64/eff_ripple_n64.h"

typedef struct EffRippleWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
    f32 scale;
    s32 frame;
    s32 lifetime;
    s32 field_20;
    s32 randAngle;
    f32 y2;
    f32 rx;
    f32 rz;
    s32 field_34;
} EffRippleWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
s32 rand(void);

extern const char str_RippleN64_802fbe60[];
extern f32 float_0_80425f20;
extern f32 float_1_80425f2c;
extern f32 float_0p6_80425f3c;
extern f32 float_10_80425f40;

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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effRippleN64Entry(f32 x, f32 y, f32 z, f32 height, s32 type) {
    void* entry;
    EffRippleWork* work;
    f32 one;
    f32 zero;
    s32 zeroInt;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_RippleN64_802fbe60;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x38);
    *(EffRippleWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRippleMain;

    zeroInt = 0;
    zero = float_0_80425f20;
    one = float_1_80425f2c;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->frame = zeroInt;
    work->field_20 = zeroInt;
    work->rx = zero;
    work->rz = zero;
    work->scale = one;
    work->field_34 = 4;

    switch ((u32)type) {
        case 1:
        case 10:
            work->radius = float_0p6_80425f3c;
            break;
        case 2:
        case 20:
            work->radius = float_10_80425f40;
            break;
        case 3:
        case 30:
            work->radius = float_10_80425f40;
            break;
        default:
            work->radius = float_1_80425f2c;
            break;
    }

    work->lifetime = 0x20;
    work->randAngle = rand() % 32;
    work->y2 = y + height;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

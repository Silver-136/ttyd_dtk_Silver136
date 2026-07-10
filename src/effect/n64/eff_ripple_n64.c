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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effRippleMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRippleDisp(void);
    extern void* effRippleN64Entry(s32 type, f32 x, f32 y, f32 z, f32 height);
    extern const Vec3 vec3_802fbe48;
    extern f32 float_0_80425f20;
    extern f32 float_0p3_80425f30;
    extern f32 float_1p05_80425f34;
    extern f32 float_0p1_80425f38;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 timer;
    s32 type;
    void* child;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbe48;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    timer = *(s32*)(work + 0x1C);
    type = *(s32*)work;
    *(s32*)(work + 0x1C) = timer - 1;
    *(s32*)(work + 0x20) += 1;
    if (*(s32*)(work + 0x1C) < 0) {
        effDelete(effect);
        return;
    }
    if (timer > 5) {
        *(s32*)(work + 0x18) = (s32)((f32)(120 - *(s32*)(work + 0x18)) * float_0p3_80425f30 + (f32)*(s32*)(work + 0x18));
    }
    if (((u32)(type - 1) <= 1 || type == 3) && timer == 15) {
        child = effRippleN64Entry(type * 10, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC), float_0_80425f20);
        *(f32*)((s32)*(void**)((s32)child + 0xC) + 0x2C) = *(f32*)(work + 0x2C);
        *(f32*)((s32)*(void**)((s32)child + 0xC) + 0x30) = *(f32*)(work + 0x30);
    }
    if (type == 2 || type == 20) {
        *(f32*)(work + 0x10) *= float_1p05_80425f34;
    }
    if (type == 3 || type == 30) {
        *(f32*)(work + 0x10) *= float_1p05_80425f34;
    }
    if (timer < 0x20) {
        *(s32*)(work + 0x18) = timer << 3;
    }
    *(s32*)(work + 0x24) += 1;
    if (*(s32*)(work + 0x24) >= 0x20) {
        *(s32*)(work + 0x24) -= 0x20;
    }
    *(f32*)(work + 0x10) += float_0p1_80425f38;
    dispEntry(*(s32*)(work + 0x34), 2, effRippleDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

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

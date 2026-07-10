#include "effect/n64/eff_rubble_n64.h"


u8 effRubbleDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma optimize_for_size off

void* effRubbleN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 groundY, f32 vy, f32 gravity, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effRubbleMain(void*);
    extern s32 rand(void);
    extern char str_RubbleN64_802fbee0[];
    extern f32 float_0_80425fbc;
    extern f32 float_20_80425fc0;
    extern f32 float_neg20_80425fc4;
    extern f32 float_2_80425fc8;
    void* entry;
    u8* work;
    f32 zero;
    f32 two;
    f32 twenty;
    f32 negTwenty;
    f32 scaled;

    entry = effEntry();
    zero = float_0_80425fbc;
    *(char**)((s32)entry + 0x14) = str_RubbleN64_802fbee0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x50);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRubbleMain;
    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = groundY;
    *(f32*)(work + 0x18) = vy;
    *(f32*)(work + 0x20) = gravity;
    *(s32*)(work + 0x48) = lifetime;
    *(s32*)(work + 0x4C) = 0;
    *(f32*)(work + 0x14) = zero;
    *(f32*)(work + 0x1C) = zero;
    *(f32*)(work + 0x24) = zero;
    *(f32*)(work + 0x28) = (f32)(rand() % 360);
    *(f32*)(work + 0x2C) = (f32)(rand() % 360);
    two = float_2_80425fc8;
    scaled = two * scale;
    twenty = float_20_80425fc0;
    negTwenty = float_neg20_80425fc4;
    *(f32*)(work + 0x30) = zero;
    *(f32*)(work + 0x38) = twenty;
    *(f32*)(work + 0x34) = negTwenty;
    *(s32*)(work + 0x44) = 0xFF;
    *(f32*)(work + 0x3C) = scaled;
    *(s32*)(work + 0x40) = 0;
    return entry;
}

#pragma optimize_for_size on

#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effRubbleMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRubbleDisp(void);
    extern void* effHokoriN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z);
    extern const Vec3 vec3_802fbec8;
    extern f32 float_0p8_80425fb8;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 timer;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbec8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x48) -= 1;
    *(s32*)(work + 0x4C) += 1;
    timer = *(s32*)(work + 0x48);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (*(f32*)(work + 8) != *(f32*)(work + 0x10)) {
        *(f32*)(work + 0x18) += *(f32*)(work + 0x20);
        *(f32*)(work + 8) += *(f32*)(work + 0x18);
        *(f32*)(work + 0x24) += *(f32*)(work + 0x30);
        *(f32*)(work + 0x2C) += *(f32*)(work + 0x38);
        *(f32*)(work + 0x28) += *(f32*)(work + 0x34);
        if (*(f32*)(work + 8) <= *(f32*)(work + 0x10)) {
            *(f32*)(work + 8) = *(f32*)(work + 0x10);
            effHokoriN64Entry(2, 0x14, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
            *(s32*)(work + 0x48) = 0x1E;
        }
    }
    if (timer < 10) {
        *(s32*)(work + 0x44) = (s32)((f32)*(s32*)(work + 0x44) * float_0p8_80425fb8);
    }
    dispEntry(4, 2, effRubbleDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


#include "effect/n64/eff_pokopi_pcharge_n64.h"


u8 effPokopiPchargeN64Entry(void) {
    return 0;
}


u8 effPokopiPchargeDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effPokopiPchargeMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPokopiPchargeDisp(void);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern const Vec3 vec3_802fbcc0;
    extern f64 double_to_int_802fbcd0;
    extern f32 float_0_80425d2c;
    extern f32 float_180_80425d34;
    extern f32 float_6p2832_80425d38;
    extern f32 float_360_80425d3c;
    extern f32 float_255_80425d40;
    u8* work;
    u8* p;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 frame;
    s32 limit;
    f32 angle;
    f32 zero;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbcc0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 0x3E8) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    if (*(s32*)(work + 0x10) < 0) {
        effDelete(effect);
        return;
    }

    if (*(s32*)(work + 0x10) < 0x10) {
        *(s32*)(work + 0x24) = *(s32*)(work + 0x10) << 4;
    }
    if (*(s32*)(work + 0x14) < 0x10) {
        *(s32*)(work + 0x24) = (*(s32*)(work + 0x14) << 4) + 0xF;
    }

    zero = float_0_80425d2c;
    p = work + 0x48;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, p += 0x48) {
        *(s32*)(p + 0x14) += 1;
        frame = *(s32*)(p + 0x14);
        limit = *(s32*)(p + 0x10);
        if (frame >= limit) {
            *(s32*)(p + 0x14) = 0;
            if (*(f32*)(p + 0x38) < zero) {
                *(f32*)(p + 4) = (f32)(-limit) * *(f32*)(p + 0x38);
            } else {
                *(f32*)(p + 4) = zero;
            }
            *(f32*)(p + 0x3C) = (f32)(rand() % 0x168);
        }

        *(f32*)(p + 4) += *(f32*)(p + 0x38);
        *(f32*)(p + 0x3C) += *(f32*)(p + 0x40);
        angle = (f32)((*(s32*)(p + 0x14) * 0xB4) / *(s32*)(p + 0x10));
        if (angle < zero || angle > float_180_80425d34) {
            *(s32*)(p + 0x24) = 0;
        } else {
            *(s32*)(p + 0x24) = (s32)(float_255_80425d40 * (f32)sin((float_6p2832_80425d38 * angle) / float_360_80425d3c));
        }
    }

    dispEntry(4, 2, effPokopiPchargeDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


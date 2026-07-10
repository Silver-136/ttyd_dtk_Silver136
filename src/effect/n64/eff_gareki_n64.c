#include "effect/n64/eff_gareki_n64.h"


u8 effGarekiDisp(void) {
    return 0;
}


u8 gareki_dl(s32 param_1) {
    return 0;
}


#pragma optimize_for_size off

void* effGarekiN64Entry(f32 x, f32 y, f32 z, f32 floorY) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effGarekiMain(void*);
    extern s32 rand(void);
    extern char str_GarekiN64_802fb010[];
    extern f32 float_neg2_804253e4;
    extern f32 float_0_804253c0;
    extern f32 float_0p25_804253e8;
    extern f32 float_2_804253ec;
    extern f32 float_1_804253f0;
    extern f32 float_0p1_804253f4;
    extern f32 trans_data[];
    extern f32 vx_data[];
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    f32 neg2;
    f32 zero;
    f32 quarter;
    f32 two;
    f32 one;
    f32 tenth;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_GarekiN64_802fb010;
    *(s32*)((s32)entry + 8) = 9;
    work = __memAlloc(3, 0x240);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effGarekiMain;

    neg2 = float_neg2_804253e4;
    zero = float_0_804253c0;
    quarter = float_0p25_804253e8;
    two = float_2_804253ec;
    one = float_1_804253f0;
    tenth = float_0p1_804253f4;

    *(s32*)(work + 0x24) = 0;
    *(f32*)work = x;
    *(f32*)(work + 4) = y;
    *(f32*)(work + 0xC) = floorY;
    *(f32*)(work + 8) = z;
    *(f32*)(work + 0x14) = neg2;
    *(f32*)(work + 0x2C) = zero;
    *(f32*)(work + 0x30) = zero;
    *(f32*)(work + 0x34) = (f32)(rand() % 60 - 30);
    *(s32*)(work + 0x28) = 0xFF;
    *(f32*)(work + 0x38) = (f32)(rand() % 60 - 30);
    *(f32*)(work + 0x3C) = quarter * *(f32*)(work + 0x38);
    *(s32*)(work + 0x20) = 20;

    part = work + 0x40;
    for (i = 1; i < 9; i++) {
        *(f32*)(part + 0x18) = trans_data[(i - 1) * 2];
        *(f32*)(part + 0x1C) = trans_data[(i - 1) * 2 + 1];
        *(f32*)part = zero;
        *(f32*)(part + 4) = zero;
        *(f32*)(part + 8) = zero;
        *(f32*)(part + 0x10) = two * vx_data[i - 1];
        *(f32*)(part + 0x14) = one + two * (tenth * (f32)(rand() % 10));
        *(f32*)(part + 0x2C) = zero;
        *(f32*)(part + 0x34) = zero;
        part += 0x40;
    }

    return entry;
}

#pragma optimize_for_size on

void effGarekiMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effGarekiDisp(void);
    extern Vec3 vec3_802faff8[];
    extern f32 float_neg0p1_804253cc;
    extern f32 float_0p8_804253d0;
    extern f32 float_neg0p2_804253d4;
    extern f32 float_0p9_804253d8;
    extern f32 float_10_804253dc;
    extern f32 float_20_804253e0;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802faff8;
    pos = *base;
    pos.x = *(f32*)work;
    pos.y = *(f32*)(work + 4);
    pos.z = *(f32*)(work + 8);
    dispPos = pos;

    if (*(s32*)(work + 0x24) == 0) {
        *(f32*)(work + 0x14) += float_neg0p1_804253cc;
        *(f32*)(work + 4) += *(f32*)(work + 0x14);
        *(f32*)(work + 0x2C) += *(f32*)(work + 0x38);
        *(f32*)(work + 0x30) += *(f32*)(work + 0x3C);
        if (*(f32*)(work + 4) <= *(f32*)(work + 0xC)) {
            *(s32*)(work + 0x24) = 1;
        }
    }

    if (*(s32*)(work + 0x24) == 1) {
        *(s32*)(work + 0x20) -= 1;
        if (*(s32*)(work + 0x20) < 0) {
            effDelete(effect);
            return;
        }
        if (*(s32*)(work + 0x20) < 10) {
            *(s32*)(work + 0x28) = (s32)((f32)*(s32*)(work + 0x28) * float_0p8_804253d0);
        }
        part = work + 0x40;
        for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
            *(f32*)(part + 0x14) += float_neg0p2_804253d4;
            *(f32*)(part + 0x10) *= float_0p9_804253d8;
            *(f32*)part += *(f32*)(part + 0x10);
            *(f32*)(part + 4) += *(f32*)(part + 0x14);
            *(f32*)(part + 0x2C) += float_10_804253dc;
            *(f32*)(part + 0x34) += float_20_804253e0;
            part += 0x40;
        }
    }

    dispEntry(4, 2, effGarekiDisp, effect, dispCalcZ(&dispPos));
}


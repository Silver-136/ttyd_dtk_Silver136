#include "effect/n64/eff_kameki_tornade_n64.h"


u8 effKamekiTornadeDisp(void) {
    return 0;
}


#pragma optimize_for_size off

void* effKamekiTornadeN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKamekiTornadeMain(void*);
    extern s32 rand(void);
    extern char str_KamekiTornadeN64_802fb198[];
    extern f32 float_0p2_8042559c;
    extern f32 float_1_80425598;
    extern f32 float_0p1_80425564;
    extern f32 float_2_804255a0;
    extern f32 float_0_80425570;
    void* entry;
    u8* work;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_KamekiTornadeN64_802fb198;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x14C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKamekiTornadeMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (timer <= 0) {
        *(s32*)(work + 0x10) = 1000;
    } else {
        *(s32*)(work + 0x10) = timer;
    }
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x34) = scale;
    *(s32*)(work + 0x18) = 0xFF;
    *(s32*)(work + 0x1C) = 0xFF;
    *(s32*)(work + 0x20) = 0xFF;
    *(s32*)(work + 0x28) = 0xFF;
    *(s32*)(work + 0x2C) = 0xFF;
    *(s32*)(work + 0x30) = 0xEB;

    for (i = 0; i < 8; i++) {
        *(f32*)(work + 0x38 + i * 4) = x;
        *(f32*)(work + 0x58 + i * 4) = y;
        *(f32*)(work + 0x78 + i * 4) = z;
        *(f32*)(work + 0x98 + i * 4) = float_1_80425598 + float_0p2_8042559c * (f32)i;
        *(f32*)(work + 0xB8 + i * 4) = (f32)(rand() % 0x168);
        *(f32*)(work + 0xD8 + i * 4) = float_2_804255a0 + float_0p1_80425564 * (f32)(rand() % 100);
        *(f32*)(work + 0xF8 + i * 4) = float_0p1_80425564 * (f32)(rand() % 30);
        *(f32*)(work + 0x118 + i * 4) = float_0_80425570;
    }
    *(f32*)(work + 0x138) = float_0_80425570;
    *(f32*)(work + 0x144) = float_0_80425570;
    *(f32*)(work + 0x13C) = float_0_80425570;
    *(f32*)(work + 0x140) = float_0_80425570;
    *(f32*)(work + 0x148) = float_1_80425598;

    return entry;
}

#pragma optimize_for_size on

void effKamekiTornadeMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKamekiTornadeDisp(void);
    extern Vec3 vec3_802fb168[];
    extern f32 float_4_80425580;
    extern f32 float_5_8042557c;
    extern f32 float_10_8042556c;
    extern f32 float_12p96_8042558c;
    extern f32 float_17p28_80425590;
    extern f32 float_64_80425594;
    extern f32 float_90_80425584;
    extern f32 float_neg90_80425588;
    u8* work;
    Vec3* base;
    Vec3 dispPos;
    Vec3 pos;
    f32 scale;
    f32 delta;
    f32 angle;
    s32 i;
    s32 timer;
    s32 frame;
    u8* p;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb168;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(u32*)effect & 4) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    scale = *(f32*)(work + 0x34);
    if (timer < 0x10) {
        *(s32*)(work + 0x24) = timer << 4;
    }
    frame = *(s32*)(work + 0x14);
    if (frame < 0x10) {
        *(s32*)(work + 0x24) = (frame << 4) + 0xF;
    }

    *(f32*)(work + 0x38) = *(f32*)(work + 4);
    *(f32*)(work + 0x58) = *(f32*)(work + 8);
    *(f32*)(work + 0x78) = *(f32*)(work + 0xC);
    p = work + 0x1C;
    for (i = 0; i < 7; i++) {
        delta = (*(f32*)(p + 0x34) - *(f32*)(p + 0x38)) * float_4_80425580;
        *(f32*)(p + 0x118) = delta;
        *(f32*)(p + 0x38) = *(f32*)(p + 0x34);
        *(f32*)(p + 0x58) = *(f32*)(p + 0x54) + float_5_8042557c * scale;
        *(f32*)(p + 0x78) = *(f32*)(p + 0x74);
        angle = *(f32*)(p + 0x118);
        if (angle > float_90_80425584) {
            *(f32*)(p + 0x118) = float_90_80425584;
        } else if (angle < float_neg90_80425588) {
            *(f32*)(p + 0x118) = float_neg90_80425588;
        }
        p -= 4;
    }

    *(f32*)(work + 0x138) += float_10_8042556c;
    *(f32*)(work + 0x13C) += float_12p96_8042558c;
    *(f32*)(work + 0x140) += float_17p28_80425590;
    if (*(f32*)(work + 0x13C) > float_64_80425594) {
        *(f32*)(work + 0x13C) -= float_64_80425594;
    }
    if (*(f32*)(work + 0x140) > float_64_80425594) {
        *(f32*)(work + 0x140) -= float_64_80425594;
    }

    *(f32*)(work + 0xB8) += *(f32*)(work + 0xD8);
    *(f32*)(work + 0xBC) += *(f32*)(work + 0xDC);
    *(f32*)(work + 0xC0) += *(f32*)(work + 0xE0);
    *(f32*)(work + 0xC4) += *(f32*)(work + 0xE4);
    *(f32*)(work + 0xC8) += *(f32*)(work + 0xE8);
    *(f32*)(work + 0xCC) += *(f32*)(work + 0xEC);
    *(f32*)(work + 0xD0) += *(f32*)(work + 0xF0);
    *(f32*)(work + 0xD4) += *(f32*)(work + 0xF4);
    dispEntry(4, 2, effKamekiTornadeDisp, effect, dispCalcZ(&dispPos));
}


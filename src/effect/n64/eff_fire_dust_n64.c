#include "effect/n64/eff_fire_dust_n64.h"


void effFireDustMain(void* effect) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern s32 rand(void);
    extern double sin(f64 x);
    extern double cos(f64 x);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFireDustDisp(s32 cameraId, void* effect);
    extern f32 scale_data[];
    extern f32 float_0_80425198;
    extern f32 float_0p5_804251a4;
    extern f32 float_6p2832_804251a8;
    extern f32 float_360_804251ac;
    extern f32 float_15_804251b0;
    extern f32 float_0p4_804251b4;
    extern f32 float_2_804251b8;
    extern f32 float_4_804251bc;
    extern f32 float_0p2_804251c0;
    extern f32 float_0p6_804251c4;
    extern f32 float_0p04_804251c8;
    u8* work;
    u8* part;
    LocalVec3 pos;
    f32 angle;
    f32 halfHeight;
    s32 i;
    s32 r;

    work = *(u8**)((s32)effect + 0xC);
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x38) = 0x10;
    }
    if (*(s32*)(work + 0x38) < 1000) {
        (*(s32*)(work + 0x38))--;
    }
    (*(s32*)(work + 0x3C))++;
    if (*(s32*)(work + 0x38) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x38) < 8) {
        *(s32*)(work + 0x4C) = *(s32*)(work + 0x38) << 5;
    }

    halfHeight = float_0p5_804251a4 * *(f32*)(work + 0x14);
    part = work;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        part += 0x6C;
        (*(s32*)(part + 0x30))++;
        if (*(s32*)(part + 0x30) > 0x13) {
            *(s32*)(part + 0x30) = 0;
        }
        if (*(s32*)(part + 0x30) > -1) {
            if (*(s32*)(part + 0x30) == 0) {
                r = rand();
                angle = (float_6p2832_804251a8 * (f32)(r % 0x168)) / float_360_804251ac;
                rand();
                *(f32*)(part + 4) = *(f32*)(work + 0x10) * (f32)sin(angle);
                r = rand();
                *(f32*)(part + 8) = (f32)(r % (s32)*(f32*)(work + 0x14)) - halfHeight;
                *(f32*)(part + 0xC) = *(f32*)(work + 0x10) * (f32)cos(angle);
                *(f32*)(part + 0x18) = float_15_804251b0 * (f32)sin(angle);
                *(f32*)(part + 0x1C) = float_0p4_804251b4 * *(f32*)(part + 8);
                *(f32*)(part + 0x20) = float_15_804251b0 * (f32)cos(angle);
                *(f32*)(part + 0x24) = float_0p5_804251a4 * *(f32*)(part + 0x18);
                *(f32*)(part + 0x28) = float_0p5_804251a4 * *(f32*)(part + 0x1C);
                *(f32*)(part + 0x2C) = float_0p5_804251a4 * *(f32*)(part + 0x20);
                r = rand();
                *(f32*)(part + 0x64) = (f32)(r % 0xF);
                r = rand();
                *(f32*)(part + 0x68) = (f32)(r % 0xF);
                *(f32*)(part + 0x34) = float_0_80425198;
                *(s32*)(part + 0x4C) = 0xFF;
            }

            *(f32*)(part + 0x60) = float_2_804251b8 * scale_data[*(s32*)(part + 0x30)];
            *(f32*)(part + 0x34) += float_4_804251bc;
            *(f32*)(part + 4) += *(f32*)(part + 0x18);
            *(f32*)(part + 8) += *(f32*)(part + 0x1C);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x20);
            *(f32*)(part + 0x64) += float_0p2_804251c0;
            *(f32*)(part + 0x68) += float_0p6_804251c4;
            *(f32*)(part + 0x18) += float_0p04_804251c8 * (*(f32*)(part + 0x24) - *(f32*)(part + 0x18));
            *(f32*)(part + 0x1C) += float_0p04_804251c8 * (*(f32*)(part + 0x28) - *(f32*)(part + 0x1C));
            *(f32*)(part + 0x20) += float_0p04_804251c8 * (*(f32*)(part + 0x2C) - *(f32*)(part + 0x20));
        }
    }

    dispEntry(4, 2, effFireDustDisp, effect, dispCalcZ(&pos));
}

void* effFireDustN64Entry(f32 x, f32 y, f32 z, f32 scaleA, f32 scaleB, s32 type, s32 count, u32 timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effFireDustMain(void* effect);
    extern char str_FireDustN64_802faea0[];
    extern f32 float_1_804251cc;
    extern f32 float_20_804251d0;
    void* entry;
    u8* work;
    u8* part;
    f32 step;
    u32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_FireDustN64_802faea0;
    *(s32*)((s32)entry + 8) = count + 2;
    work = __memAlloc(3, (count + 2) * 0x6C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireDustMain;
    *(u32*)entry |= 2;

    *(s32*)work = type;
    *(s32*)(work + 0x3C) = 0;
    if ((s32)timer < 1) {
        *(s32*)(work + 0x38) = 1000;
    } else {
        *(s32*)(work + 0x38) = timer;
    }
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = scaleA;
    *(f32*)(work + 0x14) = scaleB;
    *(f32*)(work + 0x60) = float_1_804251cc;
    *(s32*)(work + 0x4C) = 0xFF;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    *(s32*)(work + 0x48) = 0xFF;
    *(s32*)(work + 0x50) = 0xFF;
    *(s32*)(work + 0x54) = 0xFF;
    *(s32*)(work + 0x58) = 0x7F;
    *(s32*)(work + 0x5C) = 0xFF;

    if ((s32)timer < 0x14) {
        step = float_20_804251d0 / (f32)(count + 1);
    } else {
        step = (f32)timer / (f32)(count + 1);
    }

    part = work + 0x6C;
    for (i = 1; i < (u32)(count + 2); i++, part += 0x6C) {
        *(s32*)(part + 0x30) = (s32)(-(f32)i * step) - 1;
    }

    return entry;
}

u8 main_dl(void) {
    return 0;
}


u8 effFireDustDisp(int param_1, int param_2) {
    return 0;
}

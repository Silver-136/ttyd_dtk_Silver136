#include "effect/n64/eff_magic_horao_n64.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
s32 rand(void);

extern const char str_MagicHoraoN64_802fb4d0[];
extern f32 float_0_80425ae8;
extern f32 float_1_80425af8;


u8 effMagicHoraoDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagicHoraoMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effMagicHoraoDisp(void);
    extern void* effHokoriN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z);
    extern const Vec3 vec3_802fb4b8;
    extern f32 float_0p02_80425af4;
    extern f32 float_0_80425ae8;
    extern f32 float_1_80425af8;
    extern f32 float_5_80425afc;
    extern f32 float_0p05_80425b00;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 timer;
    s32 state;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb4b8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x40) -= 1;
    *(s32*)(work + 0x44) += 1;
    timer = *(s32*)(work + 0x40);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    state = *(s32*)(work + 0x2C);
    switch (state) {
        case 0:
            *(f32*)(work + 0x24) += *(f32*)(work + 0x28) * (*(f32*)(work + 0x18) - *(f32*)(work + 0x24));
            *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) +
                                          (*(f32*)(work + 0x28) * (f32)(*(s32*)(work + 0x20) - *(s32*)(work + 0x3C))));
            *(f32*)(work + 0x34) -= float_0p02_80425af4;
            if (*(f32*)(work + 0x34) < float_0_80425ae8) {
                *(f32*)(work + 0x34) = float_0_80425ae8;
                *(f32*)(work + 0x10) = float_0_80425ae8;
                *(s32*)(work + 0x30) = 10;
                *(s32*)(work + 0x2C) = 1;
            }
            break;
        case 1:
            *(s32*)(work + 0x30) -= 1;
            if (*(s32*)(work + 0x30) <= 0) {
                *(s32*)(work + 0x2C) = 2;
                *(s32*)(work + 0x30) = 10;
            }
            break;
        case 2:
            *(s32*)(work + 0x30) -= 1;
            if (*(s32*)(work + 0x30) <= 0) {
                *(s32*)(work + 0x2C) = 3;
            }
            break;
        case 3:
            *(f32*)(work + 0x10) -= float_1_80425af8;
            *(f32*)(work + 8) += *(f32*)(work + 0x10);
            if (*(f32*)(work + 8) < float_0_80425ae8) {
                *(f32*)(work + 8) = float_0_80425ae8;
                effHokoriN64Entry(2, 0x1E, *(f32*)(work + 4) + float_5_80425afc, *(f32*)(work + 8), *(f32*)(work + 0xC));
                effHokoriN64Entry(2, 0x1E, *(f32*)(work + 4) - float_5_80425afc, *(f32*)(work + 8), *(f32*)(work + 0xC));
                *(s32*)(work + 0x2C) = 4;
                *(s32*)(work + 0x30) = 0x14;
            }
            break;
        case 4:
            *(s32*)(work + 0x30) -= 1;
            if (*(s32*)(work + 0x30) <= 0) {
                *(s32*)(work + 0x2C) = 5;
            }
            break;
        case 5:
            if (timer < 10) {
                *(s32*)(work + 0x3C) = timer * 0x19;
            }
            *(f32*)(work + 0x34) -= float_0p05_80425b00;
            break;
    }

    dispEntry(4, 2, effMagicHoraoDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma optimize_for_size off

void* effMagicHoraoN64Entry(s32 type, f32 x, f32 y, f32 z, s32 param4, s32 param5, f32 param6, f32 param7, f32 param8, s32 param9) {
    void* entry;
    void* work;
    s32 zero;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_MagicHoraoN64_802fb4d0;
    *(s32*)((s32)entry + 0x8) = 1;

    work = __memAlloc(3, 0x48);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagicHoraoMain;

    *(s32*)((s32)work + 0x0) = type;
    *(f32*)((s32)work + 0x4) = x;
    *(f32*)((s32)work + 0x8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x28) = param8;
    *(f32*)((s32)work + 0x14) = param6;
    *(s32*)((s32)work + 0x1C) = param4;
    *(f32*)((s32)work + 0x18) = param7;
    *(s32*)((s32)work + 0x20) = param5;
    *(f32*)((s32)work + 0x34) = float_1_80425af8;

    zero = 0;
    *(s32*)((s32)work + 0x38) = rand() % 360;
    *(s32*)((s32)work + 0x40) = param9;
    *(s32*)((s32)work + 0x44) = zero;
    *(s32*)((s32)work + 0x3C) = param4;
    *(f32*)((s32)work + 0x24) = param6;
    *(f32*)((s32)work + 0x10) = float_0_80425ae8;
    *(s32*)((s32)work + 0x2C) = zero;

    return entry;
}

#pragma optimize_for_size on

#pragma no_register_save_helpers off

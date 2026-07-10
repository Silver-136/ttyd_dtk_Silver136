#include "effect/n64/eff_stamp_n64.h"


u8 effStampDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effStampN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effStampMain(void*);
    extern char str_StampN64_802fc030[];
    extern f32 float_6p2832_80426198;
    extern f32 float_90_8042619c;
    extern f32 float_360_804261a0;
    extern f32 float_1_8042616c;
    extern f32 float_0_80426168;
    extern f32 float_0p1_80426174;
    extern double cos(double);
    extern double sin(double);
    void* entry;
    u8* work;
    u8* part;
    f32 degrees;
    f32 angle;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_StampN64_802fc030;
    *(s32*)((s32)entry + 8) = 8;
    work = __memAlloc(3, 0x200);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStampMain;
    *(s32*)(work + 0x24) = 0x1E;
    *(s32*)(work + 0x28) = 0;
    *(s16*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x34) = 0xFF;
    if (type == 1) {
        goto type1;
    }
    if (type >= 1) {
        if (type >= 3) {
            goto colors_done;
        }
        goto type2;
    }
    if (type >= 0) {
        goto type0;
    }
    goto colors_done;
type0:
    {
        *(u8*)(work + 0x3B) = 0x14;
        *(u8*)(work + 0x38) = 0x14;
        *(u8*)(work + 0x3C) = 0x15;
        *(u8*)(work + 0x39) = 0x15;
        *(u8*)(work + 0x3D) = 0xF2;
        *(u8*)(work + 0x3A) = 0xF2;
    }
    goto colors_done;
type1:
    {
        *(u8*)(work + 0x3B) = 0xC8;
        *(u8*)(work + 0x38) = 0xC8;
        *(u8*)(work + 0x3C) = 0x15;
        *(u8*)(work + 0x39) = 0x15;
        *(u8*)(work + 0x3D) = 0xD4;
        *(u8*)(work + 0x3A) = 0xD4;
    }
    goto colors_done;
type2:
    {
        *(u8*)(work + 0x3B) = 0xFF;
        *(u8*)(work + 0x38) = 0xFF;
        *(u8*)(work + 0x3C) = 0xB5;
        *(u8*)(work + 0x39) = 0xB5;
        *(u8*)(work + 0x3D) = 0xDA;
        *(u8*)(work + 0x3A) = 0xDA;
    }
colors_done:
    part = work + 0x40;
    for (i = 1; i < 8; i++, part += 0x40) {
        degrees = (f32)(((i - 1) * 360) / 7);
        angle = ((float_90_8042619c + degrees) * float_6p2832_80426198) / float_360_804261a0;
        *(f32*)(part + 4) = float_1_8042616c * (f32)cos(angle);
        *(f32*)(part + 8) = float_1_8042616c * (f32)sin(angle);
        *(f32*)(part + 0xC) = float_0_80426168;
        *(f32*)(part + 0x10) = float_0_80426168;
        *(f32*)(part + 0x14) = float_0_80426168;
        *(f32*)(part + 0x18) = float_0_80426168;
        *(f32*)(part + 0x1C) = float_0p1_80426174;
        *(f32*)(part + 0x20) = float_0p1_80426174;
        *(f32*)(part + 0x30) = float_1_8042616c;
        *(f32*)(part + 0x2C) = degrees;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effStampMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effStampDisp(void);
    extern const Vec3 vec3_802fc018;
    extern f32 float_0p9_80426170;
    extern f32 float_6p2832_80426198;
    extern f32 float_90_8042619c;
    extern f32 float_360_804261a0;
    extern f32 float_1_8042616c;
    extern f32 float_0p1_80426174;
    extern f32 float_2p4_80426178;
    extern f32 float_0p2_8042617c;
    extern f32 float_0p07_80426180;
    extern f32 float_100_80426184;
    extern f32 float_7_8042618c;
    extern f32 float_0p05_80426188;
    extern f32 float_0p6_80426190;
    extern f32 float_10_80426194;
    extern double cos(double);
    extern double sin(double);
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    s32 frame;
    s32 i;
    f32 angle;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fc018;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x28) += 1;
    *(s32*)(work + 0x24) -= 1;
    if (*(s32*)(work + 0x24) < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x28);
    if (frame > 10) {
        *(s32*)(work + 0x34) = (s32)((f32)*(s32*)(work + 0x34) * float_0p9_80426170);
    }
    part = work + 0x40;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x40) {
        if (frame > 10) {
            *(f32*)(part + 0x1C) += float_0p1_80426174 * (float_1_8042616c - *(f32*)(part + 0x1C));
            *(f32*)(part + 0x20) += float_0p1_80426174 * (float_2p4_80426178 - *(f32*)(part + 0x20));
            *(f32*)(part + 0x30) += float_0p2_8042617c * (float_0p07_80426180 * (float_100_80426184 - *(f32*)(part + 0x30)));
        }
        if (frame < 10) {
            *(f32*)(part + 0x1C) += float_0p05_80426188 * (float_7_8042618c - *(f32*)(part + 0x1C));
            *(f32*)(part + 0x20) += float_0p1_80426174 * (float_1_8042616c - *(f32*)(part + 0x20));
            *(f32*)(part + 0x30) += float_0p6_80426190 * (float_0p05_80426188 * (float_10_80426194 - *(f32*)(part + 0x30)));
        }
        angle = ((float_90_8042619c + *(f32*)(part + 0x2C)) * float_6p2832_80426198) / float_360_804261a0;
        *(f32*)(part + 4) = *(f32*)(part + 0x30) * (f32)cos(angle);
        *(f32*)(part + 8) = *(f32*)(part + 0x30) * (f32)sin(angle);
    }
    dispEntry(4, 2, effStampDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


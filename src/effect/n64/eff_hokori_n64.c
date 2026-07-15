#include "effect/n64/eff_hokori_n64.h"


u8 effHokoriDisp(int param_1, int param_2) {
    return 0;
}


#pragma optimize_for_size off

void* effHokoriN64Entry(f32 x, f32 y, f32 z, s32 type, s32 duration) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effHokoriMain(void* effect);
    extern s32 rand(void);
    extern char str_HokoriN64_802fb0e8[];
    extern f32 float_0_80425488;
    extern f32 float_neg1_80425498;
    extern f32 float_1_8042549c;
    extern f32 float_200_804254a0;
    extern f32 float_0p1_804254a4;
    extern f32 float_0p2_804254a8;
    extern f32 float_0p4_804254ac;
    void* entry;
    u8* work;
    u8* part;
    s32 count;
    s32 i;

    entry = effEntry();
    count = type < 2 ? 5 : 4;
    *(char**)((s32)entry + 0x14) = str_HokoriN64_802fb0e8;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 0x48);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHokoriMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x2C) = duration;
    *(s32*)(work + 0x18) = 0xFF;
    *(s32*)(work + 0x30) = 0xA0;
    *(s32*)(work + 0x34) = 0xA0;
    *(s32*)(work + 0x38) = 0x8C;
    *(s32*)(work + 0x3C) = 0xA0;
    *(s32*)(work + 0x40) = 0xA0;
    *(s32*)(work + 0x44) = 0x8C;

    part = work + 0x48;
    for (i = 1; i < count; i++) {
        *(f32*)(part + 4) = (f32)(rand() % 200 - 100);
        *(f32*)(part + 8) = (f32)(rand() % 50);
        *(f32*)(part + 0xC) = float_0_80425488;
        *(f32*)(part + 0x10) = float_neg1_80425498;
        *(f32*)(part + 0x14) = float_0_80425488;
        *(f32*)(part + 0x1C) = float_1_8042549c;
        *(f32*)(part + 0x20) = float_1_8042549c;
        *(f32*)(part + 0x24) = (f32)(rand() % 10) / float_200_804254a0;

        if (type == 1) {
            *(f32*)(part + 0x28) = float_0p2_804254a8 * *(f32*)(part + 0x24);
        } else if (type == 0) {
            *(f32*)(part + 0x28) = float_0p1_804254a4 * *(f32*)(part + 0x24);
        } else {
            *(f32*)(part + 0x24) = float_0p4_804254ac;
            *(f32*)(part + 0x28) = float_0p4_804254ac;
            *(f32*)(part + 0x10) = float_0_80425488;
            *(f32*)(part + 4) = float_0p1_804254a4 * (f32)(rand() % 40 - 20);
            *(f32*)(part + 8) = float_0p1_804254a4 * (f32)(rand() % 40 - 20);
        }
        part += 0x48;
    }
    return entry;
}

#pragma optimize_for_size on

void effHokoriMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effHokoriDisp(s32, s32);
    extern Vec3 vec3_802fb0d0[];
    extern f32 float_0p9_80425490;
    extern f32 float_0p005_80425494;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb0d0;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x2C) -= 1;
    if (*(s32*)(work + 0x2C) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x2C) < 0x18) {
        *(s32*)(work + 0x18) = (s32)((f32)*(s32*)(work + 0x18) * float_0p9_80425490);
    }

    part = work + 0x48;
    if (*(s32*)work <= 1) {
        for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
            *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
            *(f32*)(part + 8) += *(f32*)(part + 0x10);
            *(f32*)(part + 0x24) += float_0p005_80425494;
            *(f32*)(part + 0x1C) += *(f32*)(part + 0x24);
            *(f32*)(part + 0x20) += *(f32*)(part + 0x28);
            part += 0x48;
        }
    } else {
        for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
            *(f32*)(part + 0x10) += *(f32*)(part + 0x14);
            *(f32*)(part + 8) += *(f32*)(part + 0x10);
            *(f32*)(part + 0x1C) += *(f32*)(part + 0x24);
            *(f32*)(part + 0x20) += *(f32*)(part + 0x28);
            part += 0x48;
        }
    }

    dispEntry(4, 2, effHokoriDisp, effect, dispCalcZ(&dispPos));
}


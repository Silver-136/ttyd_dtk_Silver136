#include "effect/n64/eff_onpu_n64.h"


u8 effOnpuDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effOnpuN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effOnpuMain(void*);
    extern s32 rand(void);
    extern char str_OnpuN64_802fbb40[];
    extern f32 float_0_80425b18;
    extern f32 float_1_80425b24;
    extern f32 float_neg5_80425b2c;
    extern f32 float_0p1_80425b30;
    extern f32 float_0p5_80425b34;
    extern f32 float_5_80425b38;
    void* entry;
    u8* work;
    s32 r;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_OnpuN64_802fbb40;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effOnpuMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = float_0_80425b18;
    *(s32*)(work + 0x18) = 0x40;
    *(s32*)(work + 0x14) = 0;
    *(s32*)(work + 0x1C) = 0;
    *(s32*)(work + 0x20) = rand() % 7;

    switch (type) {
        case 0:
            *(f32*)(work + 0x24) = float_0_80425b18;
            *(f32*)(work + 0x2C) = float_0_80425b18;
            *(f32*)(work + 0x28) = float_1_80425b24;
            *(f32*)(work + 0x30) = float_1_80425b24;
            break;
        case 1:
            r = rand() % 11;
            *(f32*)(work + 0x24) = float_neg5_80425b2c + (float_0p1_80425b30 * (f32)r);
            r = rand() % 11;
            *(f32*)(work + 0x28) = float_0p5_80425b34 + (float_0p1_80425b30 * (f32)r);
            *(f32*)(work + 0x2C) = float_0_80425b18;
            *(f32*)(work + 0x30) = float_0_80425b18;
            break;
        default:
            r = rand() % 11;
            *(f32*)(work + 0x24) = float_5_80425b38 - (float_0p1_80425b30 * (f32)r);
            r = rand() % 11;
            *(f32*)(work + 0x28) = float_1_80425b24 + (float_0p1_80425b30 * (f32)r);
            *(f32*)(work + 0x2C) = float_0_80425b18;
            *(f32*)(work + 0x30) = float_0_80425b18;
            break;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effOnpuMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effOnpuDisp(s32, s32);
    extern const Vec3 vec3_802fbb28;
    extern f32 float_0p3_80425b20;
    extern f32 float_1_80425b24;
    extern f32 float_0p04_80425b28;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 timer;
    s32 alpha;
    f32 accel;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbb28;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x18) -= 1;
    *(s32*)(work + 0x1C) += 1;
    timer = *(s32*)(work + 0x18);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer > 5) {
        alpha = *(s32*)(work + 0x14);
        *(s32*)(work + 0x14) = (s32)((f32)alpha + (float_0p3_80425b20 * (f32)(0xFF - alpha)));
        *(f32*)(work + 0x10) += float_0p3_80425b20 * (float_1_80425b24 - *(f32*)(work + 0x10));
    }
    if (timer < 10) {
        *(s32*)(work + 0x14) = timer * 0x19;
    }

    accel = float_0p04_80425b28;
    *(f32*)(work + 4) += *(f32*)(work + 0x24);
    *(f32*)(work + 8) += *(f32*)(work + 0x28);
    *(f32*)(work + 0x24) += accel * (*(f32*)(work + 0x2C) - *(f32*)(work + 0x24));
    *(f32*)(work + 0x28) += accel * (*(f32*)(work + 0x30) - *(f32*)(work + 0x28));
    dispEntry(4, 2, effOnpuDisp, effect, dispCalcZ(&dispPos));
}


#include "effect/n64/eff_shock_n64.h"


u8 effShockDisp(int param_1, int param_2) {
    return 0;
}


void effShockMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effShockDisp(void);
    extern const Vec3 vec3_802fbef0;
    extern f32 float_1p1_80425fe8;
    extern f32 float_0p05_80425fec;
    extern f32 float_8_80425ff0;
    extern f32 float_0p9_80425ff4;
    extern f32 float_0p8_80425ff8;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 timer;
    s32 type;
    s32 frame;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbef0;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    timer = *(s32*)(work + 0x1C);
    type = *(s32*)work;
    if (timer < 100) {
        *(s32*)(work + 0x1C) = timer - 1;
    }
    *(s32*)(work + 0x20) += 1;
    timer = *(s32*)(work + 0x1C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x20);
    if (type == 2) {
        if (frame <= 20) {
            *(f32*)(work + 0x24) *= float_1p1_80425fe8;
        }
    } else {
        *(f32*)(work + 0x24) += float_0p05_80425fec * (float_8_80425ff0 - *(f32*)(work + 0x24));
    }
    *(s32*)(work + 0x14) = (s32)((f32)*(s32*)(work + 0x14) * float_0p9_80425ff4);
    if (timer < 5) {
        *(s32*)(work + 0x18) = (s32)((f32)*(s32*)(work + 0x18) * float_0p8_80425ff8);
    }
    if (frame > 6) {
        *(s32*)(work + 0x10) -= 13;
        if (*(s32*)(work + 0x10) < 0) {
            *(s32*)(work + 0x10) = 0;
        }
    }
    dispEntry(4, 2, effShockDisp, effect, dispCalcZ(&dispPos));
}

void* effShockN64Entry(s32 type, f32 x, f32 y, f32 z, f32 sx, f32 sy) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effShockMain(void*);
    extern char str_ShockN64_802fbf08[];
    extern f32 float_1_80425fe0;
    extern f32 float_0_80425fd8;
    extern f32 float_0p5_80425ffc;
    void* entry;
    u8* work;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ShockN64_802fbf08;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x48);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effShockMain;
    *(s32*)work = type;
    *(s32*)(work + 0x20) = 0;
    *(s32*)(work + 0x1C) = 0x1E;
    *(s32*)(work + 0x10) = 0xFF;
    *(s32*)(work + 0x14) = 0xFF;
    *(s32*)(work + 0x18) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    if (type == 2) {
        *(f32*)(work + 0x24) = float_1_80425fe0;
    } else {
        *(f32*)(work + 0x24) = float_0_80425fd8;
    }
    *(f32*)(work + 0x28) = float_0p5_80425ffc * sx;
    *(f32*)(work + 0x2C) = float_0p5_80425ffc * sy;
    if (type >= 0 && type < 4 && type != 2) {
        *(s32*)(work + 0x30) = 0xFF;
        *(s32*)(work + 0x34) = 0xFF;
        *(s32*)(work + 0x38) = 0;
        *(s32*)(work + 0x3C) = 0xFF;
        *(s32*)(work + 0x40) = 0xFF;
        *(s32*)(work + 0x44) = 0xFF;
    } else {
        *(s32*)(work + 0x30) = 0x46;
        *(s32*)(work + 0x34) = 0xB4;
        *(s32*)(work + 0x38) = 0x78;
        *(s32*)(work + 0x3C) = 0xFF;
        *(s32*)(work + 0x40) = 0xFF;
        *(s32*)(work + 0x44) = 0xFF;
    }
    return entry;
}


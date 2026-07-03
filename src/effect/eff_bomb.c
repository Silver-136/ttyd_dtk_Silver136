#include "effect/eff_bomb.h"


u8 effBombDisp(void) {
    return 0;
}


void* effBombEntry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(s32 type);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* effConfettiEntry(s32 type, s32 count, f32 x, f32 y, f32 z);
    extern u8 effBombMain(int* effect);
    extern double sin(double x);
    extern double cos(double x);
    extern char str_Bomb_804247e8[];
    extern f32 float_0_804247d8;
    extern f32 float_deg2rad_804247c8;
    extern f32 float_10_804247f0;
    extern f32 float_90_804247f4;
    extern f32 float_45_804247f8;
    void* effect;
    u8* work;
    u8* entry;
    f32 radius;
    f32 angle;
    s32 i;

    effect = effEntry(type);
    *(char**)((s32)effect + 0x14) = str_Bomb_804247e8;
    *(s32*)((s32)effect + 8) = 9;
    work = __memAlloc(3, *(s32*)((s32)effect + 8) * 0x28);
    *(u8**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effBombMain;
    radius = float_10_804247f0 * scale;

    *(s32*)(work + 0) = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x18) = scale;
    *(s32*)(work + 0x20) = 0;
    *(s32*)(work + 0x24) = 0;

    entry = work + 0x28;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        angle = float_90_804247f4 * (f32)(i - 1);
        if (i > 4) {
            angle += float_45_804247f8;
        } else {
            angle += float_0_804247d8;
        }
        *(f32*)(entry + 0x1C) = angle;
        *(f32*)(entry + 4) = radius * (f32)sin(float_deg2rad_804247c8 * angle);
        *(f32*)(entry + 8) = radius * (f32)cos(float_deg2rad_804247c8 * angle);
        *(f32*)(entry + 0xC) = float_0_804247d8;
        *(f32*)(entry + 0x10) = float_0_804247d8;
        *(f32*)(entry + 0x14) = float_0_804247d8;
        *(s32*)(entry + 0x24) = 0;
        *(s32*)(entry + 0x20) = 0;
        entry += 0x28;
    }

    effConfettiEntry(4, 0x28, x, y, z);
    return effect;
}

void effBombMain(void* effect) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(void* pos);
    extern void* dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 priority);
    extern u8 effBombDisp(void);
    extern f32 float_100_804247e4;
    extern u8 scale_dt[];
    extern u8 scale_dt2[];
    extern f32 vec3_802f9aa0[3];
    u8* work;
    u8* entry;
    u8* table;
    f32 pos[3];
    f32 dispPos[3];
    s32 oldFrame;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = vec3_802f9aa0[0];
    pos[1] = vec3_802f9aa0[1];
    pos[2] = vec3_802f9aa0[2];
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);
    dispPos[0] = pos[0];
    dispPos[1] = pos[1];
    dispPos[2] = pos[2];

    oldFrame = *(s32*)(work + 0x20);
    *(s32*)(work + 0x20) = oldFrame + 1;
    if (*(s32*)(work + 0x20) > 0x24) {
        effDelete(effect);
        return;
    }

    entry = work + 0x28;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        if (oldFrame < 0x36) {
            if (i < 5) {
                table = scale_dt + oldFrame * 3;
            } else {
                table = scale_dt2 + oldFrame * 3;
            }
            *(u32*)(entry + 0x24) = table[0];
            *(f32*)(entry + 0x10) = (f32)table[1] / float_100_804247e4;
            *(f32*)(entry + 0x14) = (f32)table[2] / float_100_804247e4;
        }
        entry += 0x28;
    }

    dispEntry(4, 2, effBombDisp, effect, dispCalcZ(dispPos));
}


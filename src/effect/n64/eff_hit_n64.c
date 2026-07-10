#include "effect/n64/eff_hit_n64.h"


u8 effHitDisp(void) {
    return 0;
}


void* effHitN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effHitMain(void*);
    extern s32 rand(void);
    extern char str_HitN64_802fb0c8[];
    extern u8 cnt_table[];
    extern f32 float_0_80425468;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 n;

    entry = effEntry();
    n = ((count * 5 + 0x14) / 3) + 1;
    *(char**)((s32)entry + 0x14) = str_HitN64_802fb0c8;
    *(s32*)((s32)entry + 8) = n;
    work = __memAlloc(3, n * 0x44);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHitMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x30) = cnt_table[type];
    *(s32*)(work + 0x34) = 0;
    *(f32*)(work + 0x38) = float_0_80425468;

    switch (type) {
        case 0:
            *(u8*)(work + 0x3C) = 0xFF;
            *(u8*)(work + 0x3D) = 0xFF;
            *(u8*)(work + 0x3E) = 0xFF;
            *(u8*)(work + 0x3F) = 0xFF;
            *(u8*)(work + 0x40) = 0xFF;
            *(u8*)(work + 0x41) = 0xFF;
            break;
        case 1:
            *(u8*)(work + 0x3C) = 0xFF;
            *(u8*)(work + 0x3D) = 0xFF;
            *(u8*)(work + 0x3E) = 0;
            *(u8*)(work + 0x3F) = 0xFF;
            *(u8*)(work + 0x40) = 0;
            *(u8*)(work + 0x41) = 0;
            break;
        case 2:
            *(u8*)(work + 0x3C) = 0xFF;
            *(u8*)(work + 0x3D) = 0xFF;
            *(u8*)(work + 0x3E) = 0;
            *(u8*)(work + 0x3F) = 0xC8;
            *(u8*)(work + 0x40) = 0xFF;
            *(u8*)(work + 0x41) = 0xE6;
            break;
        default:
            *(u8*)(work + 0x3C) = 0;
            *(u8*)(work + 0x3D) = 0xFF;
            *(u8*)(work + 0x3E) = 0xFF;
            *(u8*)(work + 0x3F) = 0;
            *(u8*)(work + 0x40) = 0;
            *(u8*)(work + 0x41) = 0xE6;
            break;
    }
    *(f32*)(work + 0x28) = scale;

    part = work + 0x44;
    for (i = 1; i < n; i++) {
        *(f32*)(part + 4) = float_0_80425468;
        *(f32*)(part + 8) = float_0_80425468;
        *(f32*)(part + 0xC) = float_0_80425468;
        *(f32*)(part + 0x10) = float_0_80425468;
        *(f32*)(part + 0x14) = float_0_80425468;
        *(f32*)(part + 0x20) = float_0_80425468;
        *(s32*)part = 0;
        *(f32*)(part + 0x1C) = (f32)(0x168 / (n - 1));
        *(f32*)(part + 0x24) = (f32)(rand() % 0x168);
        *(f32*)(part + 0x10) = float_0_80425468;
        *(s32*)(part + 0x2C) = 0xFF;
        part += 0x44;
    }

    return entry;
}

u8 main_dl(void) {
    return 0;
}


void effHitMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effHitDisp(void);
    extern Vec3 vec3_802fb0a0[];
    extern u8 cnt_table[];
    extern u8 anim_data[];
    extern u8 radius_data[];
    extern f32 float_0p0039062_80425478;
    extern f32 float_1_8042547c;
    extern f32 float_0_80425468;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;
    s32 frame;
    s32 type;
    s32 idx;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb0a0;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    type = *(s32*)work;
    *(s32*)(work + 0x30) -= 1;
    *(s32*)(work + 0x34) += 1;
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x34);
    if (frame <= cnt_table[type]) {
        *(f32*)(work + 0x38) = (f32)anim_data[frame - 1] * float_0p0039062_80425478;
    } else {
        *(f32*)(work + 0x38) = float_1_8042547c;
    }

    part = work + 0x44;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        if (frame > cnt_table[type]) {
            idx = 0xC;
        } else {
            idx = frame - 1;
        }
        *(f32*)(part + 4) = (f32)radius_data[idx];
        *(f32*)(part + 8) = float_0_80425468;
        *(f32*)(part + 0xC) = float_0_80425468;
        part += 0x44;
    }
    dispEntry(4, 2, effHitDisp, effect, dispCalcZ(&dispPos));
}


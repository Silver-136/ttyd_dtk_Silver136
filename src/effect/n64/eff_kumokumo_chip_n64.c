#include "effect/n64/eff_kumokumo_chip_n64.h"


void* effKumokumoChipN64Entry(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 step, f32 scale, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effKumokumoChipMain(void*);
    extern f32 sqrtf(f32);
    extern char str_KumokumoChipN64_802fb384[];
    void* entry;
    u8* work;
    f32 dx, dy, dz, dist;
    s32 count;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_KumokumoChipN64_802fb384;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x48);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKumokumoChipMain;
    *(u32*)entry |= 2;
    dx = x1 - x0;
    dy = y1 - y0;
    dz = z1 - z0;
    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    count = step > 0.0f ? (s32)(dist / step) : 0;
    *(s32*)work = type;
    *(s32*)(work + 0x20) = 0;
    if (count == 0) {
        *(f32*)(work + 0x10) = 0.0f;
        *(f32*)(work + 0x14) = 0.0f;
        *(f32*)(work + 0x18) = 0.0f;
    } else {
        *(f32*)(work + 0x10) = dx / count;
        *(f32*)(work + 0x14) = dy / count;
        *(f32*)(work + 0x18) = dz / count;
    }
    *(f32*)(work + 4) = x0;
    *(f32*)(work + 8) = y0;
    *(f32*)(work + 0xC) = z0;
    *(f32*)(work + 0x1C) = scale;
    return entry;
}

u8 effKumokumoChipDisp(int param_1, int param_2) {
    return 0;
}


void effKumokumoChipMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effKumokumoChipDisp(s32, s32);
    extern const Vec3 vec3_802fb378;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 type;
    s32 timer;
    s32 frame;
    s32 flags;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb378;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    flags = *(s32*)effect;
    type = *(s32*)work;
    if (flags & 4) {
        *(s32*)effect = flags & ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    timer = *(s32*)(work + 0x1C);
    if (timer < 0x3E8) {
        *(s32*)(work + 0x1C) = timer - 1;
    }
    *(s32*)(work + 0x20) += 1;
    timer = *(s32*)(work + 0x1C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x20);
    if (type == 0 || type == 2) {
        if (frame < 0x10) {
            *(s32*)(work + 0x30) = (frame << 4) + 0xF;
        } else {
            *(s32*)(work + 0x30) = 0xFF;
        }
        if (timer < 0x10) {
            *(s32*)(work + 0x30) = timer << 4;
        }
    } else {
        if (frame < 4) {
            *(s32*)(work + 0x30) = (frame << 6) + 0x3F;
        } else {
            *(s32*)(work + 0x30) = 0xFF;
        }
        if (timer < 4) {
            *(s32*)(work + 0x30) = timer << 6;
        }
    }

    *(f32*)(work + 4) += *(f32*)(work + 0x10);
    *(f32*)(work + 8) += *(f32*)(work + 0x14);
    *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
    dispEntry(4, 2, effKumokumoChipDisp, effect, dispCalcZ(&dispPos));
}


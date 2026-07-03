#include "effect/n64/eff_stone_smoke_n64.h"

typedef struct EffStoneSmokeWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 r0;
    s32 g0;
    s32 b0;
    s32 a0;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    f32 scale;
    s32 field_38;
} EffStoneSmokeWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_StoneSmokeN64_802fc0e0[];

u8 effStoneSmokeDisp(void) {
    return 0;
}


void effStoneSmokeMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effStoneSmokeDisp(void);
    extern const Vec3 vec3_802fc0c8;
    extern f32 float_0p6_804262d8;
    extern f32 float_0p01_804262dc;
    extern u8 scale_data[];
    extern u8 scale_data2[];
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 frame;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fc0c8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    if (*(s32*)(work + 0x10) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x10) < 0x10) {
        *(s32*)(work + 0x24) = *(s32*)(work + 0x10) << 4;
    }
    frame = *(s32*)(work + 0x14);
    *(f32*)(work + 8) += float_0p6_804262d8;
    if (frame < 7) {
        *(f32*)(work + 0x38) = (f32)scale_data[frame] * float_0p01_804262dc;
    } else {
        *(f32*)(work + 0x38) = (f32)scale_data2[(frame - 7) % 7] * float_0p01_804262dc;
    }
    dispEntry(4, 2, effStoneSmokeDisp, effect, dispCalcZ(&dispPos));
}

#pragma use_lmw_stmw off
void* effStoneSmokeN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffStoneSmokeWork* work;
    s32 zero;
    s32 full;
    s32 ten;
    s32 half;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StoneSmokeN64_802fc0e0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x3C);
    *(EffStoneSmokeWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStoneSmokeMain;
    *(u32*)entry |= 2;

    zero = 0;
    work->type = type;
    work->frame = zero;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    full = 0xFF;
    ten = 10;
    work->a0 = full;
    half = 0x78;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->r0 = full;
    work->g0 = full;
    work->b0 = full;
    work->field_28 = ten;
    work->field_2C = ten;
    work->field_30 = half;
    return entry;
}
#pragma use_lmw_stmw on

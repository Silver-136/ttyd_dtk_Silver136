#include "effect/n64/eff_pokopi_thunder_n64.h"


u8 effPokopiThunderDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effPokopiThunderN64Entry(s32 type, s32 time, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effPokopiThunderMain(void*);
    extern s32 rand(void);
    extern f64 sqrt(f64);
    extern const char str_PokopiThunderN64_802fbd48[];
    extern f32 float_0_80425db0;
    extern f32 float_0p005_80425ddc;
    void* entry;
    u8* work;
    s32 camera;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;

    entry = effEntry();
    camera = 4;
    if (type >= 10) {
        camera = 8;
    }
    *(const char**)((s32)entry + 0x14) = str_PokopiThunderN64_802fbd48;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x11C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effPokopiThunderMain;
    *(u32*)entry |= 2;

    *(s32*)work = type % 10;
    *(s32*)(work + 0x2C) = 0;
    if (time <= 0) {
        *(s32*)(work + 0x28) = 0x3E8;
    } else {
        *(s32*)(work + 0x28) = time;
    }
    *(s32*)(work + 0x3C) = 0xFF;
    *(f32*)(work + 0x10) = x0;
    *(f32*)(work + 0x14) = y0;
    *(f32*)(work + 0x18) = z0;
    *(f32*)(work + 0x1C) = x1;
    *(f32*)(work + 0x20) = y1;
    *(f32*)(work + 0x24) = z1;
    *(f32*)(work + 4) = x0;
    *(f32*)(work + 8) = y0;
    *(f32*)(work + 0xC) = z0;
    *(f32*)(work + 0x4C) = scale;
    *(s32*)(work + 0x118) = camera;

    dx = x0 - x1;
    dy = y0 - y1;
    dz = z0 - z1;
    dist = (f32)sqrt(dx * dx + dy * dy + dz * dz);
    if (dist == float_0_80425db0) {
        return 0;
    }
    *(f32*)(work + 0x114) = dist * float_0p005_80425ddc;
    *(s32*)(work + 0x30) = 0xFF;
    *(s32*)(work + 0x34) = 0xDC;
    *(s32*)(work + 0x38) = 0x14;
    *(s32*)(work + 0x40) = 0xFF;
    *(s32*)(work + 0x44) = 0xFF;
    *(s32*)(work + 0x48) = 0xFF;
    *(s32*)(work + 0x110) = rand() % 0x168;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effPokopiThunderMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPokopiThunderDisp(void);
    extern const Vec3 vec3_802fbd28;
    extern f32 float_0p5_80425dd4;
    u8* work;
    Vec3 pos;
    Vec3 dispPos;
    s32 type;
    s32 frame;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbd28;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x28) = 0x10;
    }
    if (*(s32*)(work + 0x28) < 0x3E8) {
        *(s32*)(work + 0x28) -= 1;
    }
    *(s32*)(work + 0x2C) += 1;
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }
    type = *(s32*)work;
    frame = *(s32*)(work + 0x2C);
    if (type == 2) {
        if (frame >= 0x28) {
            *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p5_80425dd4);
        }
    } else if (frame >= 0xB) {
        *(s32*)(work + 0x3C) = (s32)((f32)*(s32*)(work + 0x3C) * float_0p5_80425dd4);
    }
    dispEntry(*(s32*)(work + 0x118), 2, effPokopiThunderDisp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


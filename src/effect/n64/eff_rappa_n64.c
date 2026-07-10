#include "effect/n64/eff_rappa_n64.h"


u8 effRappaDisp(void) {
    return 0;
}


void* effRappaN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 baseAngle, f32 spreadAngle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effRappaMain(void*);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_RappaN64_802fbde0[];
    extern f32 float_6p2832_80425e78;
    extern f32 float_5_80425e7c;
    extern f32 float_0p25_80425e80;
    extern f32 float_360_80425e84;
    extern f32 float_10_80425e74;
    void* entry;
    u8* work;
    u8* part;
    f32 baseRad;
    f32 angleRad;
    f32 speed;
    f32 sinBase;
    f32 cosBase;
    f32 sinAngle;
    f32 cosAngle;
    s32 i;
    s32 randValue;
    s32 angleOffset;

    entry = effEntry();
    if (count == 0) {
        return NULL;
    }

    *(char**)((s32)entry + 0x14) = str_RappaN64_802fbde0;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x38);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRappaMain;

    baseRad = (float_6p2832_80425e78 * baseAngle) / float_360_80425e84;
    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x30) = 0x1E;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x2C) = 0xFF;

    part = work + 0x38;
    for (i = 1; i < count + 1; i++, part += 0x38) {
        randValue = rand();
        *(s32*)part = type;
        *(f32*)(part + 4) = x;
        *(f32*)(part + 8) = y;
        *(f32*)(part + 0xC) = z;
        speed = (f32)(randValue % 10) * float_0p25_80425e80 + float_5_80425e7c;
        sinBase = (f32)sin(baseRad);
        angleOffset = ((i & 2) * (i / 2)) << 2;
        angleRad = (float_6p2832_80425e78 * (spreadAngle + (f32)angleOffset)) / float_360_80425e84;
        cosAngle = (f32)cos(angleRad);
        *(f32*)(part + 0x10) = cosAngle * sinBase;
        sinAngle = (f32)sin(angleRad);
        *(f32*)(part + 0x14) = sinAngle;
        cosBase = (f32)cos(baseRad);
        cosAngle = (f32)cos(angleRad);
        *(f32*)(part + 0x18) = cosAngle * cosBase;
        *(f32*)(part + 0x28) = speed;
        *(f32*)(part + 0x24) = speed / float_10_80425e74;
    }
    return entry;
}

void effRappaMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRappaDisp(void);
    extern const Vec3 vec3_802fbdc8;
    extern f32 float_0p8_80425e6c;
    extern f32 float_0p9_80425e70;
    extern f32 float_10_80425e74;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    f32 drag;
    f32 ten;
    s32 i;
    s32 timer;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbdc8;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    *(s32*)(work + 0x30) -= 1;
    *(s32*)(work + 0x34) += 1;
    timer = *(s32*)(work + 0x30);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer < 10) {
        *(s32*)(work + 0x2C) = (s32)((f32)*(s32*)(work + 0x2C) * float_0p8_80425e6c);
    }

    drag = float_0p9_80425e70;
    i = 1;
    part = work + 0x38;
    if (timer < 10) {
        ten = float_10_80425e74;
        for (; i < *(s32*)((s32)effect + 8); i++, part += 0x38) {
            *(f32*)(part + 4) += *(f32*)(part + 0x28) * *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x28) * *(f32*)(part + 0x14);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x28) * *(f32*)(part + 0x18);
            *(f32*)(part + 0x28) *= drag;
            *(f32*)(part + 0x24) += *(f32*)(part + 0x28) / ten;
        }
    } else {
        for (; i < *(s32*)((s32)effect + 8); i++, part += 0x38) {
            *(f32*)(part + 4) += *(f32*)(part + 0x28) * *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x28) * *(f32*)(part + 0x14);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x28) * *(f32*)(part + 0x18);
            *(f32*)(part + 0x28) *= drag;
        }
    }
    dispEntry(4, 2, effRappaDisp, effect, dispCalcZ(&dispPos));
}


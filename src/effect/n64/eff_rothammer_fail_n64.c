#include "effect/n64/eff_rothammer_fail_n64.h"

typedef struct EffRotHammerFailParticle {
    s32 field_00;
    f32 x;
    f32 y;
    f32 z;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
} EffRotHammerFailParticle;

typedef struct EffRotHammerFailWork {
    s16 type;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    f32 field_1C;
    f32 field_20;
    s32 lifetime;
    s32 frame;
    s32 lifetime2;
    f32 alpha;
    EffRotHammerFailParticle particle;
} EffRotHammerFailWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
double sin(double x);
double cos(double x);

extern const char str_RotHammerFailN64_802fbe88[];
extern f32 float_0_80425f5c;
extern f32 float_10_80425f74;
extern f32 float_30_80425f78;

u8 effRotHammerFailDisp(int param_1, int param_2) {
    return 0;
}


void effRotHammerFailMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effRotHammerFailDisp(void);
    extern const Vec3 vec3_802fbe70;
    extern f32 float_7_80425f64;
    extern f32 float_0p1_80425f68;
    extern f32 float_0p04_80425f6c;
    extern f32 float_0p9_80425f70;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    f32 scale;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbe70;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(f32*)(work + 0x30) = (float_7_80425f64 * (f32)*(s32*)(work + 0x28)) / (f32)*(s32*)(work + 0x2C);
    *(s32*)(work + 0x28) += 1;
    *(s32*)(work + 0x24) -= 1;
    if (*(s32*)(work + 0x24) < 0) {
        effDelete(effect);
        return;
    }
    *(f32*)(work + 0x1C) += float_0p1_80425f68 * (*(f32*)(work + 0x20) - *(f32*)(work + 0x1C));
    scale = *(f32*)(work + 0x1C);
    part = work + 0x34;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x34) {
        *(f32*)(part + 0x14) += float_0p04_80425f6c;
        *(f32*)(part + 0x14) *= float_0p9_80425f70;
        *(f32*)(part + 4) = scale * *(f32*)(part + 0x10);
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) = scale * *(f32*)(part + 0x18);
    }
    dispEntry(4, 2, effRotHammerFailDisp, effect, dispCalcZ(&dispPos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effRotHammerFailN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    void* entry;
    EffRotHammerFailWork* work;
    EffRotHammerFailParticle* particle;
    f32 zero;
    f32 ten;
    f32 thirty;
    s32 zeroInt;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_RotHammerFailN64_802fbe88;
    *(s32*)((s32)entry + 0x8) = 2;
    work = __memAlloc(3, 0x68);
    *(EffRotHammerFailWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effRotHammerFailMain;

    zero = float_0_80425f5c;
    zeroInt = 0;
    ten = float_10_80425f74;
    work->lifetime = lifetime;
    particle = &work->particle;
    thirty = float_30_80425f78;
    work->frame = zeroInt;
    work->lifetime2 = lifetime;
    work->alpha = zero;
    work->type = (s16)type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->field_1C = ten;
    work->field_20 = thirty;
    particle->x = zero;
    particle->y = zero;
    particle->z = zero;
    particle->dirX = sin(zero);
    particle->dirY = zero;
    particle->dirZ = cos(zero);
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

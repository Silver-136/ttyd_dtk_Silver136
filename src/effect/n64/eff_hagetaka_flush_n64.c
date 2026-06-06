#include "effect/n64/eff_hagetaka_flush_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffHagetakaFlushWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 colorR;
    s32 colorG;
    s32 colorB;
    s32 alpha;
    f32 scale;
    f32 scale2;
} EffHagetakaFlushWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fb070;
extern char str_HagetakaFlushN64_802fb088[];
extern f32 float_0_80425448;
extern f32 float_0p5_8042544c;
extern f32 float_0p3_80425450;
extern f32 float_0p2_80425454;


u8 effHagetakaFlushDisp(s32 param_1, void* effEntry) {
    return 0;
}


void effHagetakaFlushMain(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffHagetakaFlushWork* work;
    s32 flags;
    s32 frame;

    work = *(EffHagetakaFlushWork**)((s32)effEntry + 0xC);
    zero = &vec3_802fb070;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(s32*)effEntry;
    if (flags & 4) {
        *(s32*)effEntry = flags & ~4;
        work->timer = 10;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;
    if (work->timer < 0) {
        effDelete(effEntry);
    } else {
        frame = work->frame;
        if (work->timer < 4) {
            work->scale2 *= float_0p5_8042544c;
        } else {
            work->scale2 = float_0p3_80425450 + (float_0p2_80425454 * (f32)frame);
        }
        dispEntry(4, 2, effHagetakaFlushDisp, effEntry, dispCalcZ(&dispPos));
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effHagetakaFlushN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffHagetakaFlushWork* work;
    s32 flags;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_HagetakaFlushN64_802fb088;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x30);
    *(EffHagetakaFlushWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHagetakaFlushMain;
    flags = *(s32*)entry;
    zero = float_0_80425448;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    work->timer = 10;
    work->alpha = 0xFF;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 120;
    work->scale2 = zero;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#include "effect/n64/eff_holography_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffHolographyWork {
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
    f32 scaleStep;
} EffHolographyWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void effHolographyMain(void* effEntry);

extern const Vec3 vec3_802fb0f8;
extern char str_HolographyN64_802fb110[];
extern f32 float_0p1_804254bc;


u8 effHolographyDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effHolographyN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffHolographyWork* work;
    s32 flags;
    f32 scaleStep;

    *(char**)((s32)entry + 0x14) = str_HolographyN64_802fb110;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffHolographyWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHolographyMain;
    flags = *(s32*)entry;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    scaleStep = float_0p1_804254bc * scale;
    work->alpha = 0;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scaleStep = scaleStep;
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 120;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effHolographyMain(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffHolographyWork* work;
    s32 flags;
    s32 frame;
    s32 timer;

    work = *(EffHolographyWork**)((s32)effEntry + 0xC);
    zero = &vec3_802fb0f8;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(s32*)effEntry;
    if (flags & 4) {
        *(s32*)effEntry = flags & ~4;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;
    timer = work->timer;
    if (timer < 0) {
        effDelete(effEntry);
    } else {
        frame = work->frame;
        if (frame < 16) {
            work->alpha = (frame << 4) + 15;
        } else {
            work->alpha = 0xFF;
        }
        if (timer < 16) {
            work->alpha = timer << 4;
        }
        dispEntry(4, 2, effHolographyDisp, effEntry, dispCalcZ(&dispPos));
    }
}

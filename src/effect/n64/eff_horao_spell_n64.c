#include "effect/n64/eff_horao_spell_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffHoraoSpellWork {
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
    s32 color2R;
    s32 color2G;
    s32 color2B;
    s32 color2A;
    f32 scale;
} EffHoraoSpellWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fb120;
extern char str_HoraoSpellN64_802fb138[];


u8 effHoraoSpellDisp(void) {
    return 0;
}


void effHoraoSpellMain(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffHoraoSpellWork* work;
    s32 flags;
    s32 frame;
    s32 timer;

    work = *(EffHoraoSpellWork**)((s32)effEntry + 0xC);
    zero = &vec3_802fb120;
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
        if (timer < 16) {
            work->alpha = timer << 4;
        }
        if (frame < 16) {
            work->alpha = (frame << 4) + 15;
        }
        dispEntry(4, 2, effHoraoSpellDisp, effEntry, dispCalcZ(&dispPos));
    }
}


#pragma use_lmw_stmw off
void* effHoraoSpellN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffHoraoSpellWork* work;
    s32 flags;

    *(char**)((s32)entry + 0x14) = str_HoraoSpellN64_802fb138;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x3C);
    *(EffHoraoSpellWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHoraoSpellMain;
    flags = *(s32*)entry;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->alpha = 0;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 120;
    work->color2R = 20;
    work->color2G = 230;
    work->color2B = 50;
    work->color2A = 0xFF;

    return entry;
}
#pragma use_lmw_stmw on

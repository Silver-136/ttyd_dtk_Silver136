#include "effect/n64/eff_kemuri12_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffKemuri12Work {
    s16 type;
    u8 pad_02[2];
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s32 timer;
    s32 frame;
    s32 duration;
    f32 progress;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 colorA;
} EffKemuri12Work;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void effKemuri12Main(void* effect);

extern const Vec3 vec3_802fb350;
extern char str_Kemuri12N64_802fb368[];
extern f32 float_0_804258d4;
extern f32 float_8_804258dc;


u8 effKemuri12Disp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effKemuri12N64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffKemuri12Work* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_Kemuri12N64_802fb368;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x28);
    *(EffKemuri12Work**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri12Main;
    zero = float_0_804258d4;
    work->timer = duration;
    work->frame = 0;
    work->duration = duration;
    work->progress = zero;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    if (type == 0) {
        work->colorB = 0;
        work->colorG = 0;
        work->colorR = 0;
        work->colorA = 0x82;
    } else if (type == 1) {
        work->colorR = 0xFF;
        work->colorB = 0;
        work->colorG = 0;
        work->colorA = 0x82;
    } else {
        work->colorR = 0xE1;
        work->colorG = 0xD7;
        work->colorB = 0xFF;
        work->colorA = 0xB4;
    }

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effKemuri12Main(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffKemuri12Work* work;

    work = *(EffKemuri12Work**)((s32)effect + 0xC);
    zero = &vec3_802fb350;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->progress = (float_8_804258dc * (f32)work->frame) / (f32)work->duration;
    work->frame++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effKemuri12Disp, effect, dispCalcZ(&dispPos));
    }
}

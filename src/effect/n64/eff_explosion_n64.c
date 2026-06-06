#include "effect/n64/eff_explosion_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffExplosionWork {
    u16 type;
    u8 pad_02[2];
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s32 timer;
    s32 frame;
    s32 duration;
    f32 progress;
    f32 angle;
    f32 angleStep;
} EffExplosionWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fae38;
extern char str_ExplosionN64_802fae50[];
extern f32 float_0_804250f0;
extern f32 float_8_80425118;
extern f32 float_0p1_8042511c;
extern f32 float_40_80425120;


u8 effExplosionDisp(void) {
    return 0;
}


void effExplosionMain(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffExplosionWork* work;

    work = *(EffExplosionWork**)((s32)effect + 0xC);
    zero = &vec3_802fae38;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->progress = (float_8_80425118 * (f32)work->frame) / (f32)work->duration;
    work->frame++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(effect);
    } else {
        work->angle += work->angleStep;
        if (work->type == 0) {
            work->scale += float_0p1_8042511c;
        }
        dispEntry(4, 2, effExplosionDisp, effect, dispCalcZ(&dispPos));
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effExplosionN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffExplosionWork* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_ExplosionN64_802fae50;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffExplosionWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effExplosionMain;
    zero = float_0_804250f0;

    work->timer = duration;
    work->frame = 0;
    work->duration = duration;
    work->progress = zero;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->angle = zero;
    if (type == 0) {
        work->angleStep = zero;
    } else {
        work->angleStep = float_40_80425120;
    }

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

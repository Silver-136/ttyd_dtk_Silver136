#include "effect/n64/eff_bomhei_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffBomheiWork {
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
    f32 angle;
    f32 angleStep;
} EffBomheiWork;

void* effEntry(void);
void effDelete(void* effect);
void* __memAlloc(s32 heap, s32 size);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fabd8;
extern char str_BomheiN64_802fabf0[];
extern f32 float_0_80424db0;
extern f32 float_9_80424dc4;
extern f32 float_40_80424dc8;

u8 effBomheiDisp(int param_1, int param_2) {
    return 0;
}


void effBomheiMain(void* effect) {
    void* entry = effect;
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffBomheiWork* work;

    work = *(EffBomheiWork**)((s32)entry + 0xC);
    zero = &vec3_802fabd8;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->progress = (float_9_80424dc4 * (f32)work->frame) / (f32)work->duration;
    work->frame++;
    work->timer--;
    if (work->timer < 0) {
        effDelete(entry);
    } else {
        work->angle += work->angleStep;
        dispEntry(4, 2, effBomheiDisp, entry, dispCalcZ(&dispPos));
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBomheiN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffBomheiWork* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_BomheiN64_802fabf0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffBomheiWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBomheiMain;
    zero = float_0_80424db0;

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
        work->angleStep = float_40_80424dc8;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

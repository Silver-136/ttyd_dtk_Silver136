#include "effect/n64/eff_balloon_heiho_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffBalloonHeihoWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 rise;
    f32 riseStep;
    f32 scale;
    s32 timer;
    s32 frame;
} EffBalloonHeihoWork;

void* effEntry(void);
void effDelete(void* effect);
void* __memAlloc(s32 heap, s32 size);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fab30;
extern char str_BalloonHeihoN64_802fab3c[];
extern f32 float_0_80424cb0;
extern f32 float_2_80424cb4;

u8 effBalloonHeihoDisp(int param_1, int param_2) {
    return 0;
}


void effBalloonHeihoMain(void* effect) {
    void* entry = effect;
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffBalloonHeihoWork* work;

    work = *(EffBalloonHeihoWork**)((s32)entry + 0xC);
    zero = &vec3_802fab30;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->timer--;
    work->frame++;
    if (work->timer < 0) {
        effDelete(entry);
    } else {
        f32 maxRise = float_2_80424cb4;

        work->rise += work->riseStep;
        if (work->rise > maxRise) {
            work->rise = maxRise;
        }
        work->y += work->rise;
        dispEntry(4, 2, effBalloonHeihoDisp, entry, dispCalcZ(&dispPos));
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effBalloonHeihoN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 riseStep, f32 scale) {
    void* entry = effEntry();
    EffBalloonHeihoWork* work;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_BalloonHeihoN64_802fab3c;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x24);
    *(EffBalloonHeihoWork**)((s32)entry + 0xC) = work;
    zero = float_0_80424cb0;
    *(void**)((s32)entry + 0x10) = effBalloonHeihoMain;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->rise = zero;
    work->timer = timer;
    work->frame = 0;
    work->riseStep = riseStep;
    work->scale = scale;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

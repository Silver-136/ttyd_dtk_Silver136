#include "eff/eff_las_mon.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"

typedef struct EffLasMonWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    s32 poseId;   // 0x14
    s32 state;    // 0x18
    s32 alpha;    // 0x1C
    s32 timer;    // 0x20, counts down while fading out
} EffLasMonWork;

EffectEntry* effLasMonEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffLasMonWork* work;

    effect = effEntry();
    effect->type = "LasMon";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffLasMonWork) * effect->count);
    effect->userdata = work;
    effect->callback = effLasMonMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->state = 0;
    work->poseId = -1;
    work->alpha = 255;
    work->timer = 1000;
    return effect;
}

void effLasMonMain(EffectEntry* effect) {
    EffLasMonWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    inBattle = gp->inBattle != 0;

    // soft delete: start fading out
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 16) {
        work->alpha = work->timer * 16;
    }
    if (work->timer < 0) {
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        effDelete(effect);
        return;
    }

    if (!animGroupBaseAsync("MOBJ_EFF_las_mon", inBattle, NULL)) {
        return;
    }
    if (work->poseId == -1) {
        work->poseId = animPoseEntry("MOBJ_EFF_las_mon", inBattle);
        animPoseSetAnim(work->poseId, "A_1", TRUE);
        work->state = 0;
    }
    switch (work->state) {
        case 0:
            if (animPoseGetLoopTimes(work->poseId) >= 1.0f) {
                animPoseSetAnim(work->poseId, "S_1", TRUE);
                work->state++;
            }
            break;
        case 1:
            break;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effLasMonDisp, effect);
}

void effLasMonDisp(CameraId cameraId, EffectEntry* effect) {
    EffLasMonWork* work = effect->userdata;
    Mtx mtx;
    Mtx scale;

    if (work->poseId != -1) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXConcat(mtx, scale, mtx);
        animPoseSetMaterialFlagOn(work->poseId, 0x40);
        animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
        animPoseMain(work->poseId);
        animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 10.0f);
        animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 10.0f);
        animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 10.0f);
    }
}

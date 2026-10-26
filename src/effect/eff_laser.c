#include "eff/eff_laser.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "driver/fadedrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"
#include "driver/mapdrv.h"

typedef struct EffLaserWork {
    s32 type;       // 0x0
    Vec position;   // 0x4, starts high above the target and drops down
    Vec target;     // 0x10
    f32 scale;      // 0x1C
    s32 poseId;     // 0x20
    s32 field_0x24; // 0x24
    s32 timer;      // 0x28
    s32 state;      // 0x2C
    s32 alpha;      // 0x30
} EffLaserWork;

EffectEntry* effLaserEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffLaserWork* work;

    effect = effEntry();
    effect->type = "Laser";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffLaserWork) * effect->count);
    effect->userdata = work;
    effect->callback = effLaserMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->target.x = x;
    work->target.y = y;
    work->target.z = z;
    work->position.x = work->target.x;
    work->position.y = 1000.0f + work->target.y;
    work->position.z = work->target.z;
    work->scale = 1.0f;
    work->field_0x24 = 0;
    work->timer = 0;
    work->poseId = -1;
    work->alpha = 255;
    work->state = 0;
    return effect;
}

void effLaserMain(EffectEntry* effect) {
    EffLaserWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    inBattle = gp->inBattle != 0;

    // soft delete
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        effDelete(effect);
        return;
    }
    if (!animGroupBaseAsync("MOBJ_EFF_laser", inBattle, NULL)) {
        return;
    }
    switch (work->state) {
        case 0:
            if (work->poseId == -1) {
                work->poseId = animPoseEntry("MOBJ_EFF_laser", (u32)inBattle);
                animPoseSetAnim(work->poseId, "S_1", TRUE);
                animPoseSetLocalTimeRate(work->poseId, 0.5f);
            }
            work->state++;
            break;
        case 1:
            work->timer++;
            work->position.y = intplGetValue(EASING_QUADRATIC_IN, 500.0f + work->target.y, work->target.y, work->timer, 60);
            if (work->timer >= 60) {
                animPoseSetAnim(work->poseId, "A_1", TRUE);
                animPoseSetLocalTimeRate(work->poseId, 0.5f);
                work->state += 2;
            }
            break;
        case 2:
            if (fadeIsFinish()) {
                fadeEntry(16, 100, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
                animPoseSetAnim(work->poseId, "A_1", TRUE);
                animPoseSetLocalTimeRate(work->poseId, 0.5f);
                work->state++;
            }
            break;
        case 3:
            work->alpha--;
            if (work->alpha < 0) {
                animPoseRelease(work->poseId);
                effDelete(effect);
                return;
            }
            break;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effLaserDisp, effect);
}

void effLaserDisp(CameraId cameraId, EffectEntry* effect) {
    EffLaserWork* work = effect->userdata;
    Mtx mtx;
    Mtx scale;
    Mtx rot;

    if (work->poseId != -1) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        animPoseSetMaterialFlagOn(work->poseId, MAP_OBJ_FLAG_COLOR);
        animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
        animPoseMain(work->poseId);
        animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 10.0f);
        animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 10.0f);
        animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 10.0f);
    }
}

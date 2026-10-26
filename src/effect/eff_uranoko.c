#include "eff/eff_uranoko.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"
#include "driver/mapdrv.h"

EffectEntry* effUranokoEntry(s32 type, s32 duration, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffUranokoWork* work;

    effect = effEntry();
    effect->type = "Uranoko";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffUranokoWork) * effect->count);
    effect->userdata = work;
    effect->callback = effUranokoMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->baseScale = 1.0f;
    work->scale = 0.0f;
    work->frame = 0;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->alpha = 255;
    work->poseId = -1;
    return effect;
}

void effUranokoMain(EffectEntry* effect) {
    EffUranokoWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    inBattle = gp->inBattle != 0;

    if (!animGroupBaseAsync("MOBJ_EFF_uranoko", inBattle, NULL)) {
        return;
    }
    if (work->poseId == -1) {
        work->poseId = animPoseEntry("MOBJ_EFF_uranoko", inBattle);
        animPoseSetAnim(work->poseId, "A_1", TRUE);
    }

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
    work->frame++;
    work->scale += (1.0f - work->scale) / 8.0f;
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effUranokoDisp, effect);
}

void effUranokoDisp(CameraId cameraId, EffectEntry* effect) {
    EffUranokoWork* work = effect->userdata;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    f32 s;

    if (work->poseId != -1) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        s = work->scale * work->baseScale;
        PSMTXScale(scale, s, s, s);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        animPoseSetMaterialFlagOn(work->poseId, MAP_OBJ_FLAG_COLOR);
        animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
        animPoseMain(work->poseId);
        animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 6.0f);
        animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 6.0f);
        animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 6.0f);
    }
}

#include "eff/eff_nokotarou.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"

EffectEntry* effNokotarouEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 size) {
    EffectEntry* effect;
    EffNokotarouWork* work;

    effect = effEntry();
    effect->type = "Nokotarou";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffNokotarouWork));
    effect->userdata = work;
    effect->callback = effNokotarouMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->start.x = x;
    work->start.y = y;
    work->start.z = z;
    work->position.x = work->start.x;
    work->position.y = work->start.y;
    work->position.z = work->start.z;
    work->size = size;
    work->scale = 0.0f;
    work->alpha = 255;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->poseId = -1;
    work->poseId2 = -1;
    return effect;
}

void effNokotarouMain(EffectEntry* effect) {
    EffNokotarouWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    inBattle = gp->inBattle != 0;

    if (!animGroupBaseAsync("MOBJ_EFF_nokotarou3", inBattle, NULL)) {
        return;
    }
    if (work->poseId == -1) {
        work->poseId = animPoseEntry("MOBJ_EFF_nokotarou3", inBattle);
        animPoseSetAnim(work->poseId, "S_1", TRUE);
        work->poseId2 = animPoseEntry("MOBJ_EFF_nokotarou3", inBattle);
        animPoseSetAnim(work->poseId2, "A_1", TRUE);
    }

    // soft delete: start fading out
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->timer = 32;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 0) {
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        if (work->poseId2 != -1) {
            animPoseRelease(work->poseId2);
        }
        effDelete(effect);
        return;
    }
    if (work->timer < 32) {
        work->alpha = work->timer * 8;
        work->scale += -work->scale / 32.0f;
    } else {
        work->scale += (1.0f - work->scale) / 32.0f;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effNokotarouDisp, effect);
}

void effNokotarouDisp(CameraId cameraId, EffectEntry* effect) {
    EffNokotarouWork* work = effect->userdata;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    f32 s, sx, sy, sz;

    camGetPtr(cameraId);
    s = work->scale * work->size;
    if (work->poseId != -1) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, s, s, s);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        animPoseSetMaterialFlagOn(work->poseId, 0x40);
        animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
        animPoseMain(work->poseId);
        animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 2.0f);
        animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 2.0f);
        animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 2.0f);

        if (work->poseId2 != -1) {
            sx = (work->position.x - work->start.x) / 100.0f;
            sy = 1.0f + (work->position.y - work->start.y) / 100.0f;
            sz = 1.0f + (work->position.z - work->start.z) / 100.0f;
            sx *= work->size;
            sy *= work->size;
            sz *= work->size;
            PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
            PSMTXScale(scale, sx, sy, sz);
            PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
            PSMTXConcat(mtx, rot, mtx);
            PSMTXConcat(mtx, scale, mtx);
            animPoseSetMaterialFlagOn(work->poseId2, 0x40);
            animPoseSetMaterialEvtColor(work->poseId2, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
            animPoseMain(work->poseId2);
            animPoseDrawMtx(work->poseId2, mtx, 1, 0.0f, 2.0f);
            animPoseDrawMtx(work->poseId2, mtx, 2, 0.0f, 2.0f);
            animPoseDrawMtx(work->poseId2, mtx, 3, 0.0f, 2.0f);
        }
    }
}

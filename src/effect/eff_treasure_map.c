#include "eff/eff_treasure_map.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"
#include "driver/mapdrv.h"

EffectEntry* effTreasureMapEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffTreasureMapWork* work;

    effect = effEntry();
    effect->type = "Treasure_Map";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffTreasureMapWork));
    effect->userdata = work;
    effect->callback = effTreasureMapMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->timer = 1000;
    work->alpha = 255;
    work->scale = 1.0f;
    work->rotY = 0.0f;
    work->poseId = -1;
    work->cameraId = CAMERA_3D;
    return effect;
}

void effTreasureMapMain(EffectEntry* effect) {
    EffTreasureMapWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    inBattle = gp->inBattle != 0;

    if (work->poseId == -1) {
        if (!animGroupBaseAsync("EFF_treasure_map", inBattle, NULL)) {
            return;
        }
        // FAKE: no-op cast (animPoseEntry takes s32); the plain argument changes the object
        work->poseId = animPoseEntry("EFF_treasure_map", (u32)inBattle);
        animPoseSetAnim(work->poseId, "Z_1", TRUE);
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
    dispEntry(work->cameraId, dispCalcZ(&position), 2, (DispCallback)effTreasureMapDisp, effect);
}

void effTreasureMapDisp(CameraId cameraId, EffectEntry* effect) {
    EffTreasureMapWork* work = effect->userdata;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    f32 s;
    s32 i;

    camGetPtr(cameraId);
    if (work->poseId != -1) {
        // draw a brown, slightly larger outline first, then the map itself
        for (i = 0; i < 2; i++) {
            s = 1.1f * work->scale;
            if (i == 0) {
                PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
                PSMTXScale(scale, 1.06f * s, 1.06f * s, 1.06f * s);
                PSMTXRotRad(rot, 'y', MTXDegToRad(work->rotY));
                PSMTXConcat(trans, scale, mtx);
                PSMTXConcat(mtx, rot, mtx);
                animPoseSetMaterialFlagOn(work->poseId, MAP_OBJ_FLAG_COLOR);
                animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0x2E, 0x1B, 0x08, work->alpha });
            } else {
                PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
                PSMTXScale(scale, s, s, s);
                PSMTXRotRad(rot, 'y', MTXDegToRad(work->rotY));
                PSMTXConcat(trans, scale, mtx);
                PSMTXConcat(mtx, rot, mtx);
                animPoseSetMaterialFlagOn(work->poseId, MAP_OBJ_FLAG_COLOR);
                animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
            }
            animPoseMain(work->poseId);
            animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 10.0f);
            animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 10.0f);
            animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 10.0f);
        }
    }
}

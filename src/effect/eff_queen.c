// Shadow Queen tornado (queen: the Shadow Queen): plays the MOBJ_EFF_queen_tornade model animation and fades out.
// The duration argument sets how long it lasts (0 or less: until removed).

#include "eff/eff_queen.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"
#include "driver/mapdrv.h"
static void effQueenDisp(CameraId cameraId, EffectEntry* effect);
static void effQueenMain(EffectEntry* effect);

typedef struct EffQueenWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 scale;      // 0x10
    f32 size;       // 0x14
    s32 poseId;     // 0x18
    s32 state; // 0x1C
    s32 timer;      // 0x20, counts down while fading out
    s32 alpha;      // 0x24
    s32 maxAlpha;   // 0x28
} EffQueenWork;

/**
 * Spawns the Shadow Queen tornado at (x, y, z) scaled by `size` for `duration` frames;
 * reached through evt_eff ("queen").
 */
EffectEntry* effQueenEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 size) {
    EffectEntry* effect;
    EffQueenWork* work;

    effect = effEntry();
    effect->type = "Queen";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffQueenWork) * effect->count);
    effect->userdata = work;
    effect->callback = effQueenMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->size = size;
    work->scale = 0.0f;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->state = 0;
    work->poseId = -1;
    work->alpha = 0;
    work->maxAlpha = 255;
    work->scale = 1.0f;
    return effect;
}

/** Per frame: loads the MOBJ_EFF_queen_tornade animation, fades it in and out and deletes the effect at the end. */
static void effQueenMain(EffectEntry* effect) {
    EffQueenWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    inBattle = gp->inBattle != 0;

    // soft delete: start fading out
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->timer = 100;
    }
    if (!animGroupBaseAsync("MOBJ_EFF_queen_tornade", inBattle, NULL)) {
        return;
    }
    if (work->poseId == -1) {
        work->poseId = animPoseEntry("MOBJ_EFF_queen_tornade", inBattle);
        animPoseSetAnim(work->poseId, "S_1", TRUE);
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 0) {
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        effDelete(effect);
        return;
    }
    if (work->timer < 100) {
        work->alpha -= 3;
    } else {
        work->alpha += 3;
    }
    if (work->alpha > (s32)(0.8f * work->maxAlpha)) {
        work->alpha = 0.8f * work->maxAlpha;
    }
    if (work->alpha < 0) {
        work->alpha = 0;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, (DispCallback)effQueenDisp, effect);
}

/** Draws the camera-facing tornado pose in xlu modes 1-3 at draw scale 10. */
static void effQueenDisp(CameraId cameraId, EffectEntry* effect) {
    EffQueenWork* work = effect->userdata;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    f32 s;

    if (work->poseId != -1) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        s = work->scale * work->size;
        PSMTXScale(scale, s, work->size, s);
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

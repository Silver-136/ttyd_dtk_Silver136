// Grodus effects (syuryou, 首領: the leader): type 0 plays his staff animation EFF_batug_tue once, type 1 his barrier
// MOBJ_EFF_baria; both fade out when removed.

#include "eff/eff_syuryou.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"
#include "driver/mapdrv.h"

/**
 * Spawns Grodus's effect at (x, y, z): type 0 his staff animation, type 1 his barrier.
 * `duration` is in frames (0 or less: until soft deleted);
 * the aji (X-Naut Fortress) fights spawn the barrier with a 1000-frame duration.
 */
EffectEntry* effSyuryouEntry(s32 type, s32 duration, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffSyuryouWork* work;

    effect = effEntry();
    effect->type = "Syuryou";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffSyuryouWork) * effect->count);
    effect->userdata = work;
    effect->callback = effSyuryouMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->alpha = 255;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->alpha = 255;
    work->poseId = -1;
    return effect;
}

/**
 * Waits for the animation group to load, starts the pose, then runs the timer:
 * soft delete or timeout fades the alpha over 32 frames;
 * the staff deletes itself after one loop and the barrier switches to its S_1 animation.
 */
void effSyuryouMain(EffectEntry* effect) {
    EffSyuryouWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;
    s32 type;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    type = work->type;
    inBattle = gp->inBattle != 0;

    switch (type) {
        case 0:
            if (!animGroupBaseAsync("EFF_batug_tue", inBattle, NULL)) {
                return;
            }
            if (work->poseId == -1) {
                work->poseId = animPoseEntry("EFF_batug_tue", (u32)inBattle);
                animPoseSetAnim(work->poseId, "A_1", TRUE);
            }
            if (animPoseGetLoopTimes(work->poseId) >= 1.0f) {
                animPoseRelease(work->poseId);
                effDelete(effect);
                return;
            }
            break;
        case 1:
            if (!animGroupBaseAsync("MOBJ_EFF_baria", inBattle, NULL)) {
                return;
            }
            if (work->poseId == -1) {
                work->poseId = animPoseEntry("MOBJ_EFF_baria", (u32)inBattle);
                animPoseSetAnim(work->poseId, "A_1", TRUE);
            }
            if (animPoseGetLoopTimes(work->poseId) >= 1.0f) {
                animPoseSetAnim(work->poseId, "S_1", FALSE);
            }
            break;
    }

    // soft delete: start fading out
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->timer = 32;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 32) {
        work->alpha = work->timer * 8;
    }
    if (work->timer < 0) {
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        effDelete(effect);
        return;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effSyuryouDisp, effect);
}

/** Draws the animation pose camera-facing, with the fade alpha applied as an event colour. */
void effSyuryouDisp(CameraId cameraId, EffectEntry* effect) {
    EffSyuryouWork* work = effect->userdata;
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

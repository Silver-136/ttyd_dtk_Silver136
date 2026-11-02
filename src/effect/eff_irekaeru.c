// Swap effect (irekaeru: swap places): plays the language-specific EFF_Irekae animation at a point, fading in and out.

#include "version.h"

#if VERSION >= VERSION_US
#include "mario/mariost.h"
#endif

#include "eff/eff_irekaeru.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"
#include "driver/mapdrv.h"

#if VERSION >= VERSION_US
/**
 * Animation group of the partner-swap effect by language
 * (Japanese, English, German, French, Spanish, Italian).
 */
const char* eff_irekae_str_tbl[6] = {"EFF_Irekae", "EFF_Irekae", "EFF_Irekae_D", "EFF_Irekae_F_Sp", "EFF_Irekae_F_Sp", "EFF_Irekae_I"};
#endif

typedef struct EffIrekaeruWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 scale;      // 0x10
    s32 poseId;     // 0x14
    s32 state;      // 0x18
    s32 timer; // 0x1C
    s32 alpha;      // 0x20
} EffIrekaeruWork;

/** Spawns the swap (irekaeru) animation at (x, y, z); spawned by event scripts. */
EffectEntry* effIrekaeruEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffIrekaeruWork* work;

    effect = effEntry();
    effect->type = "Irekaeru";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffIrekaeruWork) * effect->count);
    effect->userdata = work;
    effect->callback = effIrekaeruMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->poseId = -1;
    work->state = 0;
    work->timer = 0;
    work->alpha = 0;
    return effect;
}

/** Loads the pose, fades it in, plays it, then fades out and deletes it; a soft delete deletes it immediately. */
void effIrekaeruMain(EffectEntry* effect) {
    EffIrekaeruWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
#if VERSION >= VERSION_US
    inBattle = !!gp->inBattle;
#else
    inBattle = gp->inBattle != 0;
#endif

    // soft delete: release the pose and delete at once
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        effDelete(effect);
        return;
    }
    switch (work->state) {
        case 0:
#if VERSION >= VERSION_US
            if (!animGroupBaseAsync(eff_irekae_str_tbl[gp->language], inBattle, NULL)) {
#else
            if (!animGroupBaseAsync("EFF_Irekae", inBattle, NULL)) {
#endif
                return;
            }
#if VERSION >= VERSION_US
            // FAKE: no-op (u32) cast (animPoseEntry takes s32); the plain argument changes the object
            work->poseId = animPoseEntry(eff_irekae_str_tbl[gp->language], (u32)inBattle);
#else
            // FAKE: no-op (u32) cast (animPoseEntry takes s32); the plain argument changes the object
            work->poseId = animPoseEntry("EFF_Irekae", (u32)inBattle);
#endif
            animPoseSetAnim(work->poseId, "Z_1", TRUE);
            work->state++;
            break;
        case 1:
            if (animPoseGetLoopTimes(work->poseId) > 1.0f) {
                animPoseRelease(work->poseId);
                effDelete(effect);
                return;
            }
            break;
    }

    // fade in, then fade out over the last quarter of the animation
    if (animPoseGetLoopTimes(work->poseId) < 0.75f) {
        work->alpha += 20;
        if (work->alpha > 255) {
            work->alpha = 255;
        }
    } else {
        work->alpha -= 20;
        if (work->alpha < 0) {
            work->alpha = 0;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, (DispCallback)effIrekaeruDisp, effect);
}

/** Draws the EFF_Irekae pose billboarded to the camera with the current alpha. */
void effIrekaeruDisp(CameraId cameraId, EffectEntry* effect) {
    EffIrekaeruWork* work = effect->userdata;
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
        animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 2.0f);
        animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 2.0f);
        animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 2.0f);
    }
}

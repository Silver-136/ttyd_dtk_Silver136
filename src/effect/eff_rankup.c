// Rank-up banner: plays the EFF_Rankup model animation once (EFF_TekiKyouka, "enemy strengthening", before US);
// from US on the model is chosen per language.

#include "version.h"
#include "eff/eff_rankup.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"
#include "driver/mapdrv.h"

#if VERSION >= VERSION_US
/**
 * Animation group of the rank-up effect by language
 * (Japanese, English, German, French, Spanish, Italian).
 */
const char* eff_rankup_str_tbl[6] = {"EFF_Rankup", "EFF_Rankup_us", "EFF_Rankup_ge", "EFF_Rankup_fr", "EFF_Rankup_sp", "EFF_Rankup_it"};
#endif

typedef struct EffRankupWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    f32 scale;      // 0x10
    s32 poseId;     // 0x14
    s32 state;      // 0x18
    s32 timer; // 0x1C
    s32 alpha;      // 0x20
} EffRankupWork;

/** Spawns the rank-up banner at (x, y, z); reached through evt_eff ("rankup"). */
EffectEntry* effRankupEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffRankupWork* work;

    effect = effEntry();
    effect->type = "Rankup";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffRankupWork) * effect->count);
    effect->userdata = work;
    effect->callback = effRankupMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->poseId = -1;
    work->state = 0;
    work->timer = 0;
    work->alpha = 255;
    return effect;
}

/** Per frame: loads the banner animation, fades it in, and deletes the effect after one loop. */
void effRankupMain(EffectEntry* effect) {
    EffRankupWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
#if VERSION >= VERSION_US
    inBattle = !!gp->inBattle;
#else
    inBattle = gp->inBattle != 0;
#endif

    // soft delete
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
            if (!animGroupBaseAsync(eff_rankup_str_tbl[gp->language], inBattle, NULL)) {
#else
            if (!animGroupBaseAsync("EFF_TekiKyouka", inBattle, NULL)) {
#endif
                return;
            }

            // FAKE: no-op (u32) cast (animPoseEntry takes s32); removing it changes the object
#if VERSION >= VERSION_US
            work->poseId = animPoseEntry(eff_rankup_str_tbl[gp->language], (u32)inBattle);
#else
            work->poseId = animPoseEntry("EFF_TekiKyouka", (u32)inBattle);
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

    // fade out over the last part of the animation
    if (!(animPoseGetLoopTimes(work->poseId) < 0.61538464f)) {
        work->alpha -= 20;
        if (work->alpha < 0) {
            work->alpha = 0;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, (DispCallback)effRankupDisp, effect);
}

/** Draws the banner animation pose in three segments scaled by 6. */
void effRankupDisp(CameraId cameraId, EffectEntry* effect) {
    EffRankupWork* work = effect->userdata;
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
        animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 6.0f);
        animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 6.0f);
        animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 6.0f);
    }
}

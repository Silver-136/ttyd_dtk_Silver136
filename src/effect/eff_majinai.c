// Spell effect (majinai: charm/spell): plays the MOBJ_EFF_majinai animation while the map darkens and Mario glows,
// with a stardust burst, then restores the map (states 0-3: load, darken, wait, restore). The type is stored, never read.

#include "version.h"
#include "eff/eff_majinai.h"
#include "eff/eff_stardust.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"

/**
 * Spawns the charm/spell effect at (x, y, z); called when Mario casts a spell in battle (battle_mario.c),
 * by npcdrv.c for a field spell, and by evt_eff ("majinai").
 */
EffectEntry* effMajinaiEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffMajinaiWork* work;
    static const GXColor white = { 0xFF, 0xFF, 0xFF, 0xFF };

    effect = effEntry();
    effect->type = "Majinai";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffMajinaiWork) * effect->count);
    effect->userdata = work;
    effect->callback = effMajinaiMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = 1.0f;
    work->alpha = 255;
    work->state = 0;
    work->poseId = -1;
    mapGetWork()->entries[0].color = white;
    return effect;
}

/**
 * Per frame: runs the 4-state sequence (load the MOBJ_EFF_majinai animation, darken the map and make Mario glow,
 * wait, restore) and deletes the effect when the animation has looped.
 */
#if VERSION <= VERSION_DEMO_US
static void effMajinaiMain(EffectEntry* effect) {
#else
void effMajinaiMain(EffectEntry* effect) {
#endif
    EffMajinaiWork* work = effect->userdata;
    Vec position;
    BOOL inBattle;
    BattleUnit* unit;
    MapWork* map;
    s32 i;
    s32 color;
    MarioWork* mario;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    inBattle = gp->inBattle ? TRUE : FALSE;

    // soft delete
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        if (work->poseId != -1) {
            animPoseRelease(work->poseId);
        }
        mapGetWork()->entries[0].flags &= ~8;
        effDelete(effect);
        return;
    }
    switch (work->state) {
        case 0:
            if (!animGroupBaseAsync("MOBJ_EFF_majinai", inBattle, NULL)) {
                return;
            }
            work->poseId = animPoseEntry("MOBJ_EFF_majinai", inBattle);
            animPoseSetAnim(work->poseId, "A_1", TRUE);
            work->state++;
            break;
        case 1:
            // darken the map and make Mario glow
            map = mapGetWork();
            color = map->entries[0].color.r - 5;
            if (color < 64) {
                color = 64;
            }
            map->entries[0].color = (GXColor){ color, color, color, 0xFF };
            map->entries[0].flags |= 8;
            if (!gp->inBattle) {
                mario = marioGetPtr();
#if VERSION == VERSION_EU
                for (i = 0; i < 3; i++) {
                    if (mario->paperAnimGroupId[i] != -1) {
                    animPoseSetMaterialFlagOn(mario->paperAnimGroupId[i], MAP_OBJ_FLAG_NO_MAP_BLEND);
                    }
#else
                for (i = 0; i < 5; i++) {
                    animPoseSetMaterialFlagOn(mario->paperAnimGroupId[i], MAP_OBJ_FLAG_NO_MAP_BLEND);
#endif
                }
            } else {
                unit = BattleGetMarioPtr(_battleWorkPointer);
                animPoseSetMaterialFlagOn(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit))->poseId, MAP_OBJ_FLAG_NO_MAP_BLEND);
            }
            if (animPoseGetLoopTimes(work->poseId) >= 0.93f) {
#if VERSION <= VERSION_DEMO_US
                effStardustEntry(0, 16, 60, work->position.x, 55.0f + work->position.y, work->position.z, 45.0f, 45.0f);
#else
                effStardustEntry(0, 16, 60, work->position.x, 55.0f * work->scale + work->position.y,
                                 work->position.z, 45.0f * work->scale, 45.0f * work->scale);
#endif
                work->state++;
            }
            break;
        case 2:
            if (animPoseGetLoopTimes(work->poseId) >= 1.0f) {
                work->state++;
            }
            break;
        case 3:
            // restore the map and fade out
            map = mapGetWork();
            color = map->entries[0].color.r + 5;
            if (color > 255) {
                color = 255;
            }
            map->entries[0].color = (GXColor){ color, color, color, 0xFF };
            map->entries[0].flags |= 8;
            if (!gp->inBattle) {
                mario = marioGetPtr();
#if VERSION == VERSION_EU
                for (i = 0; i < 3; i++) {
                    if (mario->paperAnimGroupId[i] != -1) {
                    animPoseSetMaterialFlagOff(mario->paperAnimGroupId[i], MAP_OBJ_FLAG_NO_MAP_BLEND);
                    }
#else
                for (i = 0; i < 5; i++) {
                    animPoseSetMaterialFlagOff(mario->paperAnimGroupId[i], MAP_OBJ_FLAG_NO_MAP_BLEND);
#endif
                }
            } else {
                unit = BattleGetMarioPtr(_battleWorkPointer);
                animPoseSetMaterialFlagOff(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit))->poseId, MAP_OBJ_FLAG_NO_MAP_BLEND);
            }
            work->alpha -= 5;
            if (work->alpha < 0) {
                if (work->poseId != -1) {
                    animPoseRelease(work->poseId);
                }
                mapGetWork()->entries[0].flags &= ~8;
                effDelete(effect);
                return;
            }
            break;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMajinaiDisp, effect);
}

/** Draws the MOBJ_EFF_majinai animation pose at the effect position. */
void effMajinaiDisp(CameraId cameraId, EffectEntry* effect) {
    EffMajinaiWork* work = effect->userdata;
    Mtx mtx;
    Mtx scale;
    Mtx rot;

    if (work->poseId != -1) {
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale, work->scale, work->scale);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        animPoseSetMaterialFlagOn(work->poseId, MAP_OBJ_FLAG_COLOR | MAP_OBJ_FLAG_NO_MAP_BLEND);
        animPoseSetMaterialEvtColor(work->poseId, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
        animPoseMain(work->poseId);
        animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 10.0f);
        animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 10.0f);
        animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 10.0f);
    }
}

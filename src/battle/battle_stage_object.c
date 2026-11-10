// Battle stage objects: breakable and falling set pieces, their motion, shaking and destruction.

#include "version.h"
#include "battle/battle_stage_object.h"
#include "battle/battle.h"
#include "battle/battle_audience.h"
#include "battle/battle_damage.h"
#include "battle/battle_database_common.h"
#include "battle/battle_stage.h"
#include "battle/battle_unit.h"
#include "battle/battle_unit_event.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/mapdrv.h"
#include "driver/msgdrv.h"
#include "driver/npcdrv.h"
#include "driver/windowdrv.h"
#include "eff/eff_nice.h"
#include "eff/n64/eff_confetti_n64.h"
#include "eff/n64/eff_kemuri1_n64.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "system.h"
#include <string.h>
#include "battle/battle_weapon_power.h"
#include "unit/unit_object_tree.h"
#include "unit/unit_object_switch.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_camera.h"
#include "battle/battle_seq.h"
#include "battle/battle_sub.h"
#include "battle/battle_ac.h"
#include "battle/battle_message.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_detect.h"
#include "eff/eff_fall.h"
#include "eff/eff_nozle.h"

// local prototypes
static void BattleObjectConfig(void);
static void _mobj_shake_main(BattleStageObject* obj);
static void _Nozzle_Change_Check(u8 index, u8* rate);
static void _disp_escape_wall_message(CameraId camera, void* param);
static void _nozzle_type_init(void);
static s32 _get_nozzle_attack_index(u8 index);
static void _combine_nozzle_weapon(BOOL copy, BattleWeapon* dst, BattleWeapon* src);
static USER_FUNC(_object_fall_attack);
static USER_FUNC(_object_fall_wait);
static USER_FUNC(_nozzle_work_get_weapon_address);
static USER_FUNC(_nozzle_work_attack_set);
static USER_FUNC(_nozzle_work_effect);
static USER_FUNC(_nozzle_work_wait);
static USER_FUNC(_fall_frame_delete);
static USER_FUNC(_fall_frame);
static USER_FUNC(_fall_frame_wait_unit_ready);
static USER_FUNC(_fall_frame_wait);

#if VERSION < VERSION_US
static USER_FUNC(_announce_wall_message);
#endif

static USER_FUNC(_nozzle_turn);
static USER_FUNC(_set_mobj_shake);
static USER_FUNC(_bgset_audience_bgset_access);
static USER_FUNC(_bgset_kemuri_effect);
static USER_FUNC(_get_bgset_attack_data);
static USER_FUNC(_mobj_destroy);
static USER_FUNC(_mobj_set_alpha);
static USER_FUNC(_set_mobj_rotate_x);

extern BattleStageNozzleData battle_stage_nozzle_data[];
extern BattleStageFallObjectData battle_stage_fall_object_data[];

/**
 * Finds the stage object with the given id (1-based, assigned in BattleStageObjectInit), or NULL.
 * The tree and switch units use it to reach the set piece they stand for through unit->stageObjectId.
 */
BattleStageObject* BattleGetObjectPtr(s32 id) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObject* obj;
    int i;

    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id == id) {
            return obj;
        }
    }
    return NULL;
}

/** Finds a live stage object by its map object name ("nozzle_l", "if_body", ...), or NULL. */
BattleStageObject* BattleSearchObjectPtr(const char* name) {
    BattleWork* wp = _battleWorkPointer;
    int i;
    BattleStageObject* obj;

    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id && !strcmp(obj->data->name, name)) {
            return obj;
        }
    }
    return NULL;
}

/**
 * Sets up the stage's set pieces at battle start (from battle_stage.c):
 * resets the stage object table, then registers the battle's stage objects (spawning a tree or switch unit for
 * those that ask for one), the three nozzles from Mario's rank 1 up, the escape wall and the spotlight.
 * Picks the nozzle and falling-object tables for the rank, starts the stage init and background layer scripts,
 * and marks layers that are missing or not destroyable as gone.
 * A stage with no iron frame hides it and counts it as already fallen.
 */
void BattleStageObjectInit(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageData* stage = wp->fbatInfo->setup->stageData;
    s32 numObjects = stage->numObjects;
    BattleStageObjectData* data = stage->objects;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    s32 countA1 = 0;
    s32 countA2 = 0;
    s32 countB = 0;
    s32 countC = 0;
    BOOL hasIronFrame = FALSE;
    s32 id;
    BattleStageObject* obj;
    BattleUnit* unit;
    int i;
    int j;

    memset(wp->stageObjects, 0, sizeof(wp->stageObjects));
    switch (pouchGetPtr()->rank) {
        case 1:
        case 2:
        case 3:
            mapPlayAnimationLv("den", MAP_ANIM_PLAY_LOOP, 0);
            break;
    }
    switch (pouchGetPtr()->rank) {
        case 1:
        case 2:
        case 3:
            sw->nozzleData = &battle_stage_nozzle_data[pouchGetPtr()->rank];
            break;
        default:
            sw->nozzleData = NULL;
            break;
    }
    sw->fallObjectData = &battle_stage_fall_object_data[pouchGetPtr()->rank];
    sw->nozzleType = -1;
    sw->fogTimer = 0;
    sw->fogEffect = NULL;
    mapGrpFlagOff(mapGetWork()->entries[0].rootMapObj->joints->name, MAP_OBJ_FLAG_CLIP);
    if (mapGetMapObj("A1")) {
        mapGrpFlagOn("A1", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOff("A1", MAP_OBJ_FLAG_CLIP);
    }
    if (mapGetMapObj("A2")) {
        mapGrpFlagOn("A2", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOff("A2", MAP_OBJ_FLAG_CLIP);
    }
    if (mapGetMapObj("B")) {
        mapGrpFlagOn("B", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOff("B", MAP_OBJ_FLAG_CLIP);
    }
    if (mapGetMapObj("C")) {
        mapGrpFlagOn("C", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOff("C", MAP_OBJ_FLAG_CLIP);
    }
    mapGrpFlagOn("escape_wall", MAP_OBJ_FLAG_HIDDEN);

    id = 1;
    if (data) {
        for (; numObjects != 0; id++, numObjects--, data++) {
            for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
                obj = &wp->stageObjects[i];
                if (obj->id <= 0) {
                    break;
                }
            }
            if (i >= 32) {
                break;
            }
            obj->data = data;
            obj->id = id;
            obj->battleUnitIdx = -1;
            switch (obj->data->layer) {
                case STAGE_LAYER_A1:
                    countA1++;
                    break;
                case STAGE_LAYER_A2:
                    countA2++;
                    break;
                case STAGE_LAYER_B:
                    countB++;
                    break;
                case STAGE_LAYER_C:
                    countC++;
                    break;
                case STAGE_LAYER_IRON_FRAME:
                    hasIronFrame = TRUE;
                    break;
            }
            switch (obj->data->layer) {
                case STAGE_LAYER_A1:
                case STAGE_LAYER_A2:
                case STAGE_LAYER_IRON_FRAME:
                case STAGE_LAYER_SPOTLIGHT:
                    obj->position = data->position;
                    break;
                case STAGE_LAYER_B:
                case STAGE_LAYER_C:
                    obj->position = data->position;
                    break;
            }
            obj->rotation.x = obj->rotation.y = obj->rotation.z = 0.0f;
            obj->prevPosition = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevPositionOffset = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevRotation = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevPositionOffset2 = (Vec){999.0f, 999.0f, 999.0f};
            mapGrpFlagOff(obj->data->name, MAP_OBJ_FLAG_HIDDEN);
            switch (data->spawnSpecialUnitType) {
                case 0:
                    unit = BtlUnit_Entry(&_entry_tree_data);
                    unit->stageObjectId = obj->id;
                    obj->battleUnitIdx = unit->unitId;
                    BtlUnit_SetPos(unit, obj->position.x, obj->position.y, obj->position.z);
                    BtlUnit_SetHomePos(unit, obj->position.x, obj->position.y, obj->position.z);
                    break;
                case 1:
                    unit = BtlUnit_Entry(&_entry_switch_data);
                    unit->stageObjectId = obj->id;
                    obj->battleUnitIdx = unit->unitId;
                    BtlUnit_SetPos(unit, obj->position.x, obj->position.y, obj->position.z);
                    BtlUnit_SetHomePos(unit, obj->position.x, obj->position.y, obj->position.z);
                    break;
                case 2:
                    break;
            }
        }
    }

    if (pouchGetPtr()->rank > 0) {
        data = _object_nozzle_data;
        for (j = 0; j < 3; j++, id++, data++) {
            for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
                obj = &wp->stageObjects[i];
                if (obj->id <= 0) {
                    break;
                }
            }
            if (i >= 32) {
                break;
            }
            obj->data = data;
            obj->id = id;
            obj->battleUnitIdx = -1;
            obj->position = data->position;
            obj->rotation.x = obj->rotation.y = obj->rotation.z = 0.0f;
            obj->prevPosition = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevPositionOffset = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevRotation = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevPositionOffset2 = (Vec){999.0f, 999.0f, 999.0f};
            mapGrpFlagOff(obj->data->name, MAP_OBJ_FLAG_HIDDEN);
        }
    }

    data = &_object_wall_data;
    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id <= 0) {
            break;
        }
    }

    // With no free slot, obj still points to the last slot; there is no full-table check here.
    obj->data = data;
    obj->id = id;
    obj->battleUnitIdx = -1;
    obj->position = data->position;
    obj->position.y = BattleStageGetPtr()->stageSizeY;
    obj->rotation.x = obj->rotation.y = obj->rotation.z = 0.0f;
    obj->prevPosition = (Vec){999.0f, 999.0f, 999.0f};
    obj->prevPositionOffset = (Vec){999.0f, 999.0f, 999.0f};
    obj->prevRotation = (Vec){999.0f, 999.0f, 999.0f};
    obj->prevPositionOffset2 = (Vec){999.0f, 999.0f, 999.0f};
    mapGrpFlagOn(obj->data->name, MAP_OBJ_FLAG_HIDDEN);
    obj->flags |= STAGE_OBJECT_FLAG_HIDDEN;
    id++;

    j = 0;
    while (TRUE) {
        data = &_object_spot_light_data[j];
        if (!data->name) {
            break;
        }
        for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
            obj = &wp->stageObjects[i];
            if (obj->id <= 0) {
                break;
            }
        }
        obj->data = data;
        obj->id = id;
        obj->battleUnitIdx = -1;
        obj->position = data->position;
        obj->rotation.x = obj->rotation.y = obj->rotation.z = 0.0f;
        obj->prevPosition = (Vec){999.0f, 999.0f, 999.0f};
        obj->prevPositionOffset = (Vec){999.0f, 999.0f, 999.0f};
        obj->prevRotation = (Vec){999.0f, 999.0f, 999.0f};
        obj->prevPositionOffset2 = (Vec){999.0f, 999.0f, 999.0f};
        mapGrpFlagOn(obj->data->name, MAP_OBJ_FLAG_HIDDEN);
        obj->flags |= STAGE_OBJECT_FLAG_HIDDEN;
        id++;
        j++;
    }

    if (stage->initEvt) {
        evtEntry(stage->initEvt, 10, 0);
    }
    if (countA1 && stage->bgA1Evt) {
        wp->stageObjectWork.bgEvtId[0] = evtEntry(stage->bgA1Evt, 10, 0)->eventId;
    }
    if (countA2 && stage->bgA2Evt) {
        wp->stageObjectWork.bgEvtId[1] = evtEntry(stage->bgA2Evt, 10, 0)->eventId;
    }
    if (countB && stage->bgBEvt) {
        wp->stageObjectWork.bgEvtId[2] = evtEntry(stage->bgBEvt, 10, 0)->eventId;
    }
    if (countC && stage->bgCEvt) {
        wp->stageObjectWork.bgEvtId[3] = evtEntry(stage->bgCEvt, 10, 0)->eventId;
    }
    if (!countA1) {
        wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_A1_DESTROY;
    }
    if (!countA2) {
        wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_A2_DESTROY;
    }
    if (!countB) {
        wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_B_DESTROY;
    }
    if (stage->disableBgA1Destroy) {
        wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_A1_DESTROY;
    }
    if (stage->disableBgA2Destroy) {
        wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_A2_DESTROY;
    }
    if (stage->disableBgBDestroy) {
        wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_B_DESTROY;
    }
    if (!hasIronFrame) {
        wp->stageObjectWork.flags |= STAGE_FLAG_NO_IRON_FRAME;
        mapGrpFlagOn("if_body", MAP_OBJ_FLAG_HIDDEN);
        mapGrpFlagOn("if_wire", MAP_OBJ_FLAG_HIDDEN);
    }
}

/**
 * Per-frame stage object update (from battle.c):
 * pushes changed positions, offsets and rotations into the map objects (layer B tilts as a whole around its own
 * height, layer C pivots on its y), and shows or hides each one by flag 1.
 */
void BattleStageObjectMain(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    BattleStageObject* obj;
    BOOL changed;
    int i;

    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id <= 0) {
            continue;
        }
        changed = FALSE;
        if (obj->position.x != obj->prevPosition.x || obj->position.y != obj->prevPosition.y || obj->position.z != obj->prevPosition.z) {
            obj->prevPosition = obj->position;
            changed = TRUE;
        }
        if (obj->positionOffset.x != obj->prevPositionOffset.x || obj->positionOffset.y != obj->prevPositionOffset.y || obj->positionOffset.z != obj->prevPositionOffset.z) {
            obj->prevPositionOffset = obj->positionOffset;
            changed = TRUE;
        }
        if (obj->positionOffset2.x != obj->prevPositionOffset2.x || obj->positionOffset2.y != obj->prevPositionOffset2.y || obj->positionOffset2.z != obj->prevPositionOffset2.z) {
            obj->prevPositionOffset2 = obj->positionOffset2;
            changed = TRUE;
        }
        if (obj->rotation.x != obj->prevRotation.x || obj->rotation.y != obj->prevRotation.y || obj->rotation.z != obj->prevRotation.z) {
            obj->prevRotation = obj->rotation;
            changed = TRUE;
        }
        if (obj->flags & STAGE_OBJECT_FLAG_UPDATE) {
            obj->flags &= ~STAGE_OBJECT_FLAG_UPDATE;
            changed = TRUE;
        }
        switch (obj->data->layer) {
            case STAGE_LAYER_B:
                if (sw->bgRotation.x != sw->bgRotationPrev.x ||
                    sw->bgRotation.y != sw->bgRotationPrev.y ||
                    sw->bgRotation.z != sw->bgRotationPrev.z) {
                    sw->bgRotationPrev = sw->bgRotation;
                    changed = TRUE;
                }
                break;
        }
        switch (obj->data->layer) {
            case STAGE_LAYER_A1:
            case STAGE_LAYER_A2:
            case STAGE_LAYER_NOZZLE:
            case STAGE_LAYER_WALL:
            case STAGE_LAYER_IRON_FRAME:
            case STAGE_LAYER_SPOTLIGHT:
                if (changed) {
                    mapObjTranslate(obj->data->name,
                        obj->positionOffset2.x + (obj->position.x + obj->positionOffset.x),
                        obj->positionOffset2.y + (obj->position.y + obj->positionOffset.y),
                        obj->positionOffset2.z + (obj->position.z + obj->positionOffset.z));
                    mapObjRotate(obj->data->name, obj->rotation.x, obj->rotation.y, obj->rotation.z);
                }
                break;
            case STAGE_LAYER_B:
                if (changed) {
                    mapObjTranslate(obj->data->name,
                        obj->positionOffset2.x + (obj->position.x + obj->positionOffset.x),
                        obj->positionOffset2.y + (obj->position.y + obj->positionOffset.y),
                        obj->positionOffset2.z + (obj->position.z + obj->positionOffset.z));
                    mapObjTranslate(obj->data->name, 0.0f, -obj->positionOffset.y, 0.0f);
                    mapObjRotate(obj->data->name, sw->bgRotation.x, sw->bgRotation.y, sw->bgRotation.z);
                    mapObjTranslate(obj->data->name, 0.0f, obj->positionOffset.y, 0.0f);
                }
                break;
            case STAGE_LAYER_C:
                if (changed) {
                    mapObjTranslate(obj->data->name,
                        obj->positionOffset2.x + (obj->position.x + obj->positionOffset.x),
                        obj->positionOffset2.y + (obj->position.y + obj->positionOffset.y),
                        obj->positionOffset2.z + (obj->position.z + obj->positionOffset.z));
                    mapObjTranslate(obj->data->name, 0.0f, -obj->position.y, 0.0f);
                    mapObjRotate(obj->data->name, obj->rotation.x, obj->rotation.y, obj->rotation.z);
                    mapObjTranslate(obj->data->name, 0.0f, obj->position.y, 0.0f);
                }
                break;
        }
        if (obj->flags & STAGE_OBJECT_FLAG_HIDDEN) {
            if (!(mapGetMapObj(obj->data->name)->flags & MAP_OBJ_FLAG_HIDDEN)) {
                mapGrpFlagOn(obj->data->name, MAP_OBJ_FLAG_HIDDEN);
            }
        } else {
            if (mapGetMapObj(obj->data->name)->flags & MAP_OBJ_FLAG_HIDDEN) {
                mapGrpFlagOff(obj->data->name, MAP_OBJ_FLAG_HIDDEN);
            }
        }
    }
    BattleObjectConfig();
}

/**
 * Empty in every version; called at the end of BattleStageObjectMain each frame.
 */
static void BattleObjectConfig(void) {
    // does nothing
}

/**
 * Knocks down background layer A1: stops its idle script and any running background destroy event,
 * then runs the stage's own A1 destroy script or the default _object_turn_event_A1.
 * Rolled by btlevtcmd_WeaponAftereffect after attacks that shake the stage.
 */
void BattleStage_DestroyA1(void) {
    BattleWork* wp = _battleWorkPointer;
    void* evt = wp->fbatInfo->setup->stageData->destroyBgA1Evt;
    EventEntry* entry;

    evtDeleteID(wp->stageObjectWork.bgEvtId[0]);
    wp->stageObjectWork.bgEvtId[0] = 0;
    if (wp->stageObjectWork.bgDestroyEvtId && evtCheckID(wp->stageObjectWork.bgDestroyEvtId)) {
        evtDeleteID(wp->stageObjectWork.bgDestroyEvtId);
    }
    if (!evt) {
        evt = _object_turn_event_A1;
    }
    entry = evtEntry(evt, 10, 0);
    if (entry) {
        wp->stageObjectWork.bgDestroyEvtId = entry->eventId;
    }
}

/**
 * Knocks down background layer A2 the same way as BattleStage_DestroyA1, but without tracking the event id,
 * so nothing waits for it.
 */
void BattleStage_DestroyA2(void) {
    BattleWork* wp = _battleWorkPointer;
    void* evt = wp->fbatInfo->setup->stageData->destroyBgA2Evt;

    evtDeleteID(wp->stageObjectWork.bgEvtId[1]);
    wp->stageObjectWork.bgEvtId[1] = 0;
    if (!evt) {
        evt = _object_turn_event_A2;
    }
    evtEntry(evt, 10, 0);
}

/**
 * Knocks down the rear background layer B, the same way as BattleStage_DestroyA1.
 * btlevtcmd_WeaponAftereffect only rolls this once A1 is gone.
 */
void BattleStage_DestroyB(void) {
    BattleWork* wp = _battleWorkPointer;
    void* evt = wp->fbatInfo->setup->stageData->destroyBgBEvt;
    EventEntry* entry;

    evtDeleteID(wp->stageObjectWork.bgEvtId[2]);
    wp->stageObjectWork.bgEvtId[2] = 0;
    if (wp->stageObjectWork.bgDestroyEvtId && evtCheckID(wp->stageObjectWork.bgDestroyEvtId)) {
        evtDeleteID(wp->stageObjectWork.bgDestroyEvtId);
    }
    if (!evt) {
        evt = _object_turn_event_B;
    }
    entry = evtEntry(evt, 10, 0);
    if (entry) {
        wp->stageObjectWork.bgDestroyEvtId = entry->eventId;
    }
}

/**
 * Base weapon of a nozzle spray aimed at Mario's side (it skips enemies):
 * _nozzle_work_attack_set starts from it and adds the nozzle type's weapon once per nozzle pointing that way.
 */
static BattleWeaponData _nozzle_at_base_mario = {{
    NULL, 0, // name..icon
    {0}, // pad_06
    0, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_ENEMY, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
}};

/** Base weapon of a nozzle spray aimed at the enemy side (it skips Mario and his partner), used the same way. */
static BattleWeaponData _nozzle_at_base_monster = {{
    NULL, 0, // name..icon
    {0}, // pad_06
    0, // itemId
    NULL, // description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_MARIO_OR_SHELL | TARGET_CLASS_NO_PARTNER | TARGET_CLASS_UNUSED_4 | TARGET_CLASS_UNUSED_8 | TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
}};

BattleStageNozzleData battle_stage_nozzle_data[] = {
    {
#if VERSION >= VERSION_US
        {{0, 0, 40, 60}, {0, 30, 0, 70}, {0, 40, 60, 0}}, {100, 0, 0, 0},
#else
        {{0, 0, 50, 50}, {0, 50, 0, 50}, {0, 50, 50, 0}}, {100, 0, 0, 0},
#endif
        {
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
#if VERSION >= VERSION_US
        {{0, 0, 30, 70}, {0, 30, 0, 70}, {0, 40, 60, 0}}, {65, 35, 0, 0},
#else
        {{0, 0, 50, 50}, {0, 50, 0, 50}, {0, 50, 50, 0}}, {60, 40, 0, 0},
#endif
        {
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_ICE, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 100, 1, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
#if VERSION >= VERSION_US
        {{0, 0, 40, 60}, {0, 30, 0, 70}, {0, 40, 60, 0}}, {35, 25, 40, 0},
#else
        {{0, 0, 50, 50}, {0, 50, 0, 50}, {0, 50, 50, 0}}, {40, 30, 30, 0},
#endif
        {
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_ICE, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 100, 1, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_EXPLOSION, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
#if VERSION >= VERSION_US
        {{0, 0, 40, 60}, {0, 30, 0, 70}, {0, 40, 60, 0}}, {20, 20, 25, 35},
#else
        {{0, 0, 50, 50}, {0, 50, 0, 50}, {0, 50, 50, 0}}, {20, 30, 30, 20},
#endif
        {
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_ICE, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 100, 1, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_EXPLOSION, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_FIRE, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                100, 3, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
};

/**
 * Falling stage objects by Mario's rank: the weapon of the falling iron frame,
 * the odds of each of the nine kinds of object that can drop on a unit (more kinds join at higher ranks),
 * and each kind's weapon. Used by the object-fall event; kind 6 picks a random side to hit.
 */
BattleStageFallObjectData battle_stage_fall_object_data[] = {
    {
        {{
            NULL, 0, // name..icon
            {0}, // pad_06
            0, // itemId
            NULL, // description
            100, // baseAccuracy
            0, 0, // baseFpCost..baseSpCost
            2, // superguardsAllowed
            0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
            weaponGetPowerDefault, // hpDamageFn
            {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
            NULL, // fpDamageFn
            {0}, // fpDamageWork
            TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
            TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
            ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
            3, // acLevel
            2, // reserved_6F
            NULL, // acHelpMsg
            0, // special
            COUNTER_RESIST_ALL, // counterResistFlags
            0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
            0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
            0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
            0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
            0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
            0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
            {0}, // pad_AE
            NULL, // attackEvtScript
            {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
        }},
        {0, 10, 0, 0, 0, 0, 0, 0, 10},
        {
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_DAMAGE_SURPRISE, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 100, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                SPECIAL_IGNORE_VULNERABILITY, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 100, 0, 100, 0, 100, 0, 100, // targetWeightFlags..poisonChance
                0, 0, 100, 0, 100, 0, 0, 0, // poisonTime..dodgyTime
                100, 0, 100, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
#if VERSION <= VERSION_DEMO_JP
                -99, 0, 0, 0, 0, 0, 0, 0,
#else
                -99, 0, 0, 0, 0, 0, 0, -99, // chargeStrength..paybackTime
#endif
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 100, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 25, 3, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
        {{
            NULL, 0, // name..icon
            {0}, // pad_06
            0, // itemId
            NULL, // description
            100, // baseAccuracy
            0, 0, // baseFpCost..baseSpCost
            2, // superguardsAllowed
            0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
            weaponGetPowerDefault, // hpDamageFn
            {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
            NULL, // fpDamageFn
            {0}, // fpDamageWork
            TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
            TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
            ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
            3, // acLevel
            2, // reserved_6F
            NULL, // acHelpMsg
            0, // special
            COUNTER_RESIST_ALL, // counterResistFlags
            0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
            0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
            0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
            0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
            0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
            0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
            {0}, // pad_AE
            NULL, // attackEvtScript
            {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
        }},
        {10, 10, 0, 0, 0, 10, 0, 0, 10},
        {
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_DAMAGE_SURPRISE, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 100, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                SPECIAL_IGNORE_VULNERABILITY, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 100, 0, 100, 0, 100, 0, 100, // targetWeightFlags..poisonChance
                0, 0, 100, 0, 100, 0, 0, 0, // poisonTime..dodgyTime
                100, 0, 100, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
#if VERSION <= VERSION_DEMO_JP
                -99, 0, 0, 0, 0, 0, 0, 0,
#else
                -99, 0, 0, 0, 0, 0, 0, -99, // chargeStrength..paybackTime
#endif
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 100, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 25, 3, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
        {{
            NULL, 0, // name..icon
            {0}, // pad_06
            0, // itemId
            NULL, // description
            100, // baseAccuracy
            0, 0, // baseFpCost..baseSpCost
            2, // superguardsAllowed
            0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
            weaponGetPowerDefault, // hpDamageFn
            {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
            NULL, // fpDamageFn
            {0}, // fpDamageWork
            TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
            TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
            ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
            3, // acLevel
            2, // reserved_6F
            NULL, // acHelpMsg
            0, // special
            COUNTER_RESIST_ALL, // counterResistFlags
            0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
            0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
            0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
            0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
            0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
            0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
            {0}, // pad_AE
            NULL, // attackEvtScript
            {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
        }},
        {10, 10, 10, 10, 10, 10, 0, 0, 10},
        {
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_DAMAGE_SURPRISE, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 100, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                SPECIAL_IGNORE_VULNERABILITY, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 100, 0, 100, 0, 100, 0, 100, // targetWeightFlags..poisonChance
                0, 0, 100, 0, 100, 0, 0, 0, // poisonTime..dodgyTime
                100, 0, 100, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
#if VERSION <= VERSION_DEMO_JP
                -99, 0, 0, 0, 0, 0, 0, 0,
#else
                -99, 0, 0, 0, 0, 0, 0, -99, // chargeStrength..paybackTime
#endif
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 100, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 25, 3, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
        {{
            NULL, 0, // name..icon
            {0}, // pad_06
            0, // itemId
            NULL, // description
            100, // baseAccuracy
            0, 0, // baseFpCost..baseSpCost
            2, // superguardsAllowed
            0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
            weaponGetPowerDefault, // hpDamageFn
            {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
            NULL, // fpDamageFn
            {0}, // fpDamageWork
            TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
            TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
            ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
            3, // acLevel
            2, // reserved_6F
            NULL, // acHelpMsg
            0, // special
            COUNTER_RESIST_ALL, // counterResistFlags
            0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
            0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
            0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
            0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
            0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
            0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
            {0}, // pad_AE
            NULL, // attackEvtScript
            {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
        }},
        {10, 10, 10, 10, 10, 10, 10, 10, 10},
        {
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_DAMAGE_SURPRISE, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 100, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                NULL, // hpDamageFn
                {0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                SPECIAL_IGNORE_VULNERABILITY, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 100, 0, 100, 0, 100, 0, 100, // targetWeightFlags..poisonChance
                0, 0, 100, 0, 100, 0, 0, 0, // poisonTime..dodgyTime
                100, 0, 100, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
#if VERSION <= VERSION_DEMO_JP
                -99, 0, 0, 0, 0, 0, 0, 0,
#else
                -99, 0, 0, 0, 0, 0, 0, -99, // chargeStrength..paybackTime
#endif
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 100, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
            {{
                NULL, 0, // name..icon
                {0}, // pad_06
                0, // itemId
                NULL, // description
                100, // baseAccuracy
                0, 0, // baseFpCost..baseSpCost
                2, // superguardsAllowed
                0.0f, 0, 0, 0, 0, // reserved_14..reserved_1B
                weaponGetPowerDefault, // hpDamageFn
                {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
                NULL, // fpDamageFn
                {0}, // fpDamageWork
                TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
                TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
                ELEMENT_NORMAL, ATTACKPATTERN_PRESS, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                COUNTER_RESIST_ALL, // counterResistFlags
                TARGET_WEIGHT_RANDOM, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 25, 3, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // attackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
};

/** The three stage nozzles (left, centre, right) at the front of the stage, placed from Mario's rank 1 up. */
BattleStageObjectData _object_nozzle_data[] = {
    {"nozzle_l", 3, STAGE_LAYER_NOZZLE, {-120.0f, 0.0f, 90.0f}, 0, 0},
    {"nozzle_c", 3, STAGE_LAYER_NOZZLE, {0.0f, 0.0f, 90.0f}, 0, 0},
    {"nozzle_r", 3, STAGE_LAYER_NOZZLE, {120.0f, 0.0f, 90.0f}, 0, 0},
};

#if VERSION <= VERSION_DEMO_JP
BattleStageObjectData _object_wall_data = {"escape_wall", 4, STAGE_LAYER_WALL, {-175.0f, 0.0f, 0.0f}, 0, 0};
BattleStageObjectData _object_spot_light_data[] = {
    {"spotlight", 6, STAGE_LAYER_SPOTLIGHT, {0.0f, 240.0f, 0.0f}, 0, 0},
    {NULL, 6, 7, {0.0f, 0.0f, 0.0f}, 0, 0},
};
BattleUnitSetup _entry_tree_data = {&unit_object_tree, 2, {0}, BATTLE_PHASE_AFTER_ENEMY, {-1000.0f, -1000.0f, -1000.0f}, 0, {0, 0, 0, 0}, NULL};
BattleUnitSetup _entry_switch_data = {&unit_object_switch, 2, {0}, BATTLE_PHASE_AFTER_ENEMY, {-1000.0f, -1000.0f, -1000.0f}, 0, {0, 0, 0, 0}, NULL};
/** Default destroy script for background layer A1: runs _object_turn_event_core with LW(9) = 0. */
EVT_BEGIN(_object_turn_event_A1)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(_object_turn_event_core)
    RETURN
EVT_END()
/** Default destroy script for background layer A2: runs _object_turn_event_core with LW(9) = 1. */
EVT_BEGIN(_object_turn_event_A2)
    SET_VALUE(LW(9), 1)
    CALL_CHILD(_object_turn_event_core)
    RETURN
EVT_END()
/** Default destroy script for background layer B: runs _object_turn_event_core with LW(9) = 2. */
EVT_BEGIN(_object_turn_event_B)
    SET_VALUE(LW(9), 2)
    CALL_CHILD(_object_turn_event_core)
    RETURN
EVT_END()
#endif

EVT_BEGIN(_object_turn_event_core)
    CALL(_set_mobj_shake_init, LW(9))
    CALL(_set_mobj_shake, LW(9))
    CALL(_set_mobj_rotate_x, LW(9))
    IF_INT_NOT_EQUAL(LW(9), 1)
        CALL(evt_btl_camera_shake_h, 0, 4, 0, 30, 13)
    END_IF
    CALL(_bgset_kemuri_effect, LW(9))
    IF_INT_NOT_EQUAL(LW(9), 1)
        CALL(_bgset_audience_bgset_access)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_PARTY_IN_AUDIENCE)
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(_get_bgset_attack_data, LW(9), LW(12))
    IF_INT_EQUAL(LW(12), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SYSTEM, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SYSTEM, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    SET_VALUE(LW(15), 0)
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(20)
    END_IF
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_GetACPossibility, LW(10), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            ADD(LW(15), 1)
            CALL(btlevtcmd_ResultACDefence, LW(10), LW(12))
        END_IF
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SYSTEM, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_INTANGIBLE)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SYSTEM, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_DEFAULT
    END_SWITCH
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(10)
    LABEL(20)
    WAIT_FRAMES(50)
    SET_VALUE(LW(0), 255)
    SET_VALUE(LW(1), 51)
    LABEL(50)
    SUBTRACT(LW(0), 5)
    CALL(_mobj_set_alpha, LW(9), LW(0))
    SUBTRACT(LW(1), 1)
    IF_INT_GREATER(LW(1), 0)
        WAIT_FRAMES(1)
        GOTO(50)
    END_IF
    CALL(_mobj_destroy, LW(9))
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_JP
/** Turns one nozzle (index in the event's unitId) to its new direction; started by _Nozzle_Change_Check. */
EVT_BEGIN(_nozzle_turn_event)
    CALL(_nozzle_turn)
    RETURN
EVT_END()
EVT_BEGIN(_close_wall_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(_announce_wall_message)
    RETURN
EVT_END()
#endif

/**
 * The iron frame over the stage falls: waits for other attacks and nozzle work,
 * runs every unit's ceiling-fall reaction script, drops the frame, damages everyone the frame weapon reaches,
 * shakes the camera and fades it out.
 */
EVT_BEGIN(_fall_frame_event)
    CALL(_fall_frame_wait)
    CALL(_fall_frame_wait_unit_ready)
    CALL(_fall_frame, LW(12))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SYSTEM, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SYSTEM, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    SET_VALUE(LW(15), 0)
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(20)
    END_IF
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_GetACPossibility, LW(10), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            ADD(LW(15), 1)
            CALL(btlevtcmd_ResultACDefence, LW(10), LW(12))
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SYSTEM, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(10)
    LABEL(20)
    CALL(evt_btl_camera_shake_h, 0, 10, 0, 20, 13)
    CALL(_fall_frame_delete)
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_JP
/**
 * Nozzle work at the first turn (BattleStage_NozzleWorkCheck(0)): _nozzle_work_event_core with LW(10) = 0.
 * The early demos place this script and _nozzle_work_event_1 here; JP has its copies after the default
 * destroy scripts below, US and later after _set_mobj_rotate_x.
 */
EVT_BEGIN(_nozzle_work_event_0)
    SET_VALUE(LW(10), 0)
    CALL_CHILD(_nozzle_work_event_core)
    RETURN
EVT_END()
/**
 * Nozzle attack during battle (BattleStage_NozzleWorkCheck(1)):
 * _nozzle_work_event_core with LW(10) = 1, which also lets a type-0 nozzle set fill the stage with fog.
 */
EVT_BEGIN(_nozzle_work_event_1)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(_nozzle_work_event_core)
    RETURN
EVT_END()
#endif

/**
 * The three nozzles fire (smoke, a silent effect or fire, by nozzleType) or, for type 0 with LW(10) = 1,
 * fog starts.
 * When something fired, it builds the Mario-side and enemy-side weapons from the nozzles' aim
 * and runs each one on its targets, with one action-command guard check on the Mario side and dodges
 * where the pre-check says so.
 */
EVT_BEGIN(_nozzle_work_event_core)
    CALL(_nozzle_work_wait)
    CALL(_nozzle_work_effect, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(_nozzle_work_attack_set)
    CALL(_nozzle_work_get_weapon_address, 0, LW(12))
    IF_INT_EQUAL(LW(12), 0)
        GOTO(19)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SYSTEM, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SYSTEM, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    SET_VALUE(LW(15), 0)
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(19)
    END_IF
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_GetACPossibility, LW(10), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            ADD(LW(15), 1)
            CALL(btlevtcmd_ResultACDefence, LW(10), LW(12))
        END_IF
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SYSTEM, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_INTANGIBLE)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SYSTEM, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_DEFAULT
    END_SWITCH
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(10)
    LABEL(19)
    CALL(_nozzle_work_get_weapon_address, 1, LW(12))
    IF_INT_EQUAL(LW(12), 0)
        GOTO(29)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SYSTEM, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SYSTEM, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    LABEL(20)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(29)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SYSTEM, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(PRECHECK_INTANGIBLE)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_MISS)
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(10), DAMAGE_CODE_DODGY)
        CASE_EQUAL(PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SYSTEM, LW(10), LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_DEFAULT
    END_SWITCH
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(20)
    LABEL(29)
    RETURN
EVT_END()

/**
 * An object falls from the ceiling: waits for other stage events, runs _object_fall_attack,
 * and if the object landed in the audience waits another second. Started by BattleStage_ObjectFallCheck.
 */
EVT_BEGIN(_object_fall_event)
    CALL(_object_fall_wait)
    CALL(_object_fall_attack, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(1000)
    END_IF
    RETURN
EVT_END()

/** The escape wall at the left edge of the stage, which closes in battles that cannot be fled. */
#if VERSION >= VERSION_JP
BattleStageObjectData _object_wall_data = {"escape_wall", 4, STAGE_LAYER_WALL, {-175.0f, 0.0f, 0.0f}, 0, 0};

/** The overhead spotlight; the entry with no name ends the list. */
BattleStageObjectData _object_spot_light_data[] = {
    {"spotlight", 6, STAGE_LAYER_SPOTLIGHT, {0.0f, 240.0f, 0.0f}, 0, 0},
    {NULL, 6, 7, {0.0f, 0.0f, 0.0f}, 0, 0},
};

/** Unit setup for a hittable stage tree, spawned for stage objects that ask for one (unit_object_tree.c). */
BattleUnitSetup _entry_tree_data = {&unit_object_tree, 2, {0}, BATTLE_PHASE_AFTER_ENEMY, {-1000.0f, -1000.0f, -1000.0f}, 0, {0, 0, 0, 0}, NULL};

/** Unit setup for a hittable stage switch, spawned for stage objects that ask for one (unit_object_switch.c). */
BattleUnitSetup _entry_switch_data = {&unit_object_switch, 2, {0}, BATTLE_PHASE_AFTER_ENEMY, {-1000.0f, -1000.0f, -1000.0f}, 0, {0, 0, 0, 0}, NULL};

/** Default destroy script for background layer A1: runs _object_turn_event_core with LW(9) = 0. */
EVT_BEGIN(_object_turn_event_A1)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(_object_turn_event_core)
    RETURN
EVT_END()

/** Default destroy script for background layer A2: runs _object_turn_event_core with LW(9) = 1. */
EVT_BEGIN(_object_turn_event_A2)
    SET_VALUE(LW(9), 1)
    CALL_CHILD(_object_turn_event_core)
    RETURN
EVT_END()

/** Default destroy script for background layer B: runs _object_turn_event_core with LW(9) = 2. */
EVT_BEGIN(_object_turn_event_B)
    SET_VALUE(LW(9), 2)
    CALL_CHILD(_object_turn_event_core)
    RETURN
EVT_END()

/** Turns one nozzle (index in the event's unitId) to its new direction; started by _Nozzle_Change_Check. */
EVT_BEGIN(_nozzle_turn_event)
    CALL(_nozzle_turn)
    RETURN
EVT_END()
#endif

#if VERSION < VERSION_US
#if VERSION >= VERSION_JP
EVT_BEGIN(_close_wall_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(_announce_wall_message)
    RETURN
EVT_END()

EVT_BEGIN(_nozzle_work_event_0)
    SET_VALUE(LW(10), 0)
    CALL_CHILD(_nozzle_work_event_core)
    RETURN
EVT_END()
#endif

/**
 * Nozzle attack during battle (BattleStage_NozzleWorkCheck(1)):
 * _nozzle_work_event_core with LW(10) = 1, which also lets a type-0 nozzle set fill the stage with fog.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(_nozzle_work_event_1)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(_nozzle_work_event_core)
    RETURN
EVT_END()
#endif
#endif

static USER_FUNC(_set_mobj_rotate_x) {
    const char* fall1;
    const char* fall3;
    BattleWork* wp;
    int j;
    int i;
    BOOL busy;
    const char* fall2;
    s32 layer;
    BattleStageObjectWork* sw;
    BattleStageObject* obj;

    EvtWord* args = event->args;
    busy = FALSE;

    sw = (&(wp = _battleWorkPointer)->stageObjectWork);
    layer = evtGetValue(event, *args++);
    if (isFirstCall) {
        switch (layer) {
            case STAGE_LAYER_A1:
            case STAGE_LAYER_A2:
                for (j = 0; j < BATTLE_STAGE_OBJECT_MAX; j++) {
                    obj = &wp->stageObjects[j];
                    if (obj->id > 0 && obj->data->layer == layer) {
                        obj->numFramesToStartFalling = obj->data->numFramesToStartFalling;
                        obj->numFramesFalling = 0;
                    }
                }
                break;
            case STAGE_LAYER_B:
                event->userWork[1] = 0;
                event->userWork[2] = 60;
                event->userWork[3] = 0;
                break;
        }
    }
    switch (layer) {
        case STAGE_LAYER_A1:
        case STAGE_LAYER_A2:
            fall1 = "SFX_BTL_STAGE_SET_FALL1";
            fall3 = "SFX_BTL_STAGE_SET_FALL3";
            for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
                obj = &wp->stageObjects[i];
                if (obj->id > 0 && obj->data->layer == layer) {
                    if (obj->numFramesToStartFalling) {
                        obj->numFramesToStartFalling--;
                        busy = TRUE;
                        _mobj_shake_main(obj);
                    } else {
                        if (obj->numFramesFalling == 0) {
                            if (layer == 0) {
                                psndSFXOn_3D(fall1, &obj->position);
                            } else {
                                psndSFXOn_3D(fall3, &obj->position);
                            }
                        }
                        obj->numFramesFalling++;
                        obj->rotation.x = intpl_sub(EASING_CUBIC_IN, obj->shakeAngle, 90.0f, obj->numFramesFalling, obj->data->numFramesToFall);
                        if (obj->numFramesFalling < obj->data->numFramesToFall) {
                            busy = TRUE;
                        }
                    }
                }
            }
            break;
        case STAGE_LAYER_B:
            if (event->userWork[1]) {
                event->userWork[1]--;
                busy = TRUE;
            } else {
                if (event->userWork[3] == 0) {
                    fall2 = "SFX_BTL_STAGE_SET_FALL2";
                    psndSFXOn_3D(fall2, &(Vec){0.0f, 0.0f, -100.0f});
                }
                event->userWork[3]++;
                sw->bgRotation.x = (s32)intpl_sub(EASING_CUBIC_IN, 0.0f, 90.0f, event->userWork[3], event->userWork[2]);
                if (event->userWork[3] < event->userWork[2]) {
                    busy = TRUE;
                }
            }
            break;
    }
    return busy ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/* US and later: these three scripts sit after _set_mobj_rotate_x so the message string pools after its Vec literal;
   these builds show the wall banner with btlevtcmd_AnnounceMessage for 180 frames */
#if VERSION >= VERSION_US
EVT_BEGIN(_close_wall_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_AnnounceMessage, 2, 0, -225, STRING("btl_msg_escape_wall_close2"), 180)
    RETURN
EVT_END()

EVT_BEGIN(_nozzle_work_event_0)
    SET_VALUE(LW(10), 0)
    CALL_CHILD(_nozzle_work_event_core)
    RETURN
EVT_END()

EVT_BEGIN(_nozzle_work_event_1)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(_nozzle_work_event_core)
    RETURN
EVT_END()
#endif

static USER_FUNC(_mobj_set_alpha) {
    EvtWord* args = event->args;
    s32 layer = evtGetValue(event, args[0]);
    u8 alpha = evtGetValue(event, args[1]);

    switch (layer) {
        case STAGE_LAYER_A1:
            if (mapGetMapObj("A1")) {
                mapGrpSetColor("A1", (GXColor){255, 255, 255, alpha});
            }
            break;
        case STAGE_LAYER_A2:
            if (mapGetMapObj("A2")) {
                mapGrpSetColor("A2", (GXColor){255, 255, 255, alpha});
            }
            break;
        case STAGE_LAYER_B:
            if (mapGetMapObj("B")) {
                mapGrpSetColor("B", (GXColor){255, 255, 255, alpha});
            }
            break;
        case STAGE_LAYER_C:
            if (mapGetMapObj("C")) {
                mapGrpSetColor("C", (GXColor){255, 255, 255, alpha});
            }
            break;
    }
    return EVT_RETURN_DONE;
}

/**
 * Hides every piece of background layer args[0], makes any tree or switch unit on it untargetable,
 * and sets the layer's GONE flag so it cannot fall again.
 */
static USER_FUNC(_mobj_destroy) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObject* obj;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 layer;
    int i;

    layer = evtGetValue(event, event->args[0]);
    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && obj->data->layer == layer) {
            obj->flags |= STAGE_OBJECT_FLAG_HIDDEN;
            unit = BattleGetUnitPtr(wp, obj->battleUnitIdx);
            if (unit) {
                for (part = unit->parts; part; part = part->nextPart) {
                    part->attributes |= kNotTargetable;
                }
            }
        }
    }
    switch (layer) {
        case STAGE_LAYER_A1:
            wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_A1_DESTROY;
            break;
        case STAGE_LAYER_A2:
            wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_A2_DESTROY;
            break;
        case STAGE_LAYER_B:
            wp->stageObjectWork.flags |= STAGE_FLAG_NO_BG_B_DESTROY;
            break;
    }
    return EVT_RETURN_DONE;
}

/**
 * Returns in args[1] the weapon a falling background layer args[0] hits with:
 * the stage's bgAWeapon for A1, bgBWeapon for B, or 0 for A2,
 * other layers and weapons without a damage function.
 */
static USER_FUNC(_get_bgset_attack_data) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 out = args[1];
    BattleWeapon* weapon;

    switch (type) {
        case 0:
            weapon = &wp->fbatInfo->setup->stageData->bgAWeapon;
            break;
        case 2:
            weapon = &wp->fbatInfo->setup->stageData->bgBWeapon;
            break;
        case 1:
        default:
            weapon = NULL;
            break;
    }
    if (weapon && !weapon->hpDamageFn) {
        weapon = NULL;
    }
    evtSetValue(event, out, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}

/** Puffs dust (kemuri) in front of each piece of the fallen layer args[0] and plays its landing sound. */
static USER_FUNC(_bgset_kemuri_effect) {
    s32 layer;
    const char* sfxA1;
    const char* sfxA2;
    const char* sfxB;
    BattleWork* wp;
    EvtWord* args = event->args;
    int i;
    BattleStageObject* obj;
    f32 x, y, z;

    wp = _battleWorkPointer;
    layer = evtGetValue(event, args[0]);
    sfxA1 = "SFX_BTL_STAGE_SET_DOWN1";
    sfxA2 = "SFX_BTL_STAGE_SET_DOWN3";
    sfxB = "SFX_BTL_STAGE_SET_DOWN2";
    for (i = 0; i < 32; i++) {
        // FAKE: args++/args-- cancel out; removing them changes the object
        args++;
        args--;
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && obj->data->layer == layer) {
            x = obj->position.x;
            y = obj->position.y;
            z = obj->position.z;
            switch (layer) {
                case STAGE_LAYER_A1:
                    z += 30.0f;
                    effKemuri1N64Entry(3, x, y, z, 0.0f);
                    psndSFXOn_3D(sfxA1, &obj->position);
                    break;
                case STAGE_LAYER_A2:
                    z += 15.0f;
                    effKemuri1N64Entry(0, x, y, z, 0.0f);
                    psndSFXOn_3D(sfxA2, &obj->position);
                    break;
                case STAGE_LAYER_B:
                    z += 30.0f;
                    effKemuri1N64Entry(3, x, y, z, 0.0f);
                    effKemuri1N64Entry(4, x, y, z, 0.0f);
                    psndSFXOn_3D(sfxB, &obj->position);
                    break;
            }
        }
    }
    return EVT_RETURN_DONE;
}

/** Gives the audience its reaction to a falling background set (BattleAudience_Case_HaikeiSet, haikei = background). */
static USER_FUNC(_bgset_audience_bgset_access) {
    BattleAudience_Case_HaikeiSet();
    return EVT_RETURN_DONE;
}

/** Gives each piece of layer args[0] a random wobble period of 60-69 frames, starting at the start or the middle of the period. */
USER_FUNC(_set_mobj_shake_init) {
    BattleWork* wp = _battleWorkPointer;
    s32 layer;
    int i;
    BattleStageObject* obj;
    u8 position;

    layer = evtGetValue(event, event->args[0]);
    for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && obj->data->layer == layer) {
            obj->reserved_72 = 0;
            obj->shakePeriodLength = irand(10) + 60;
            if (irand(100) & 1) {
                position = 0;
            } else {
                position = obj->shakePeriodLength / 2;
            }
            obj->shakePeriodPosition = position;
            obj->shakeAngle = 0.0f;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Wobbles the A1/A2 pieces of layer args[0] each frame until all other battle events,
 * nozzle work and the iron frame fall are finished; layer B just waits.
 */
static USER_FUNC(_set_mobj_shake) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObject* obj;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    s32 layer;
    int i;

    layer = evtGetValue(event, event->args[0]);
    if (BattleWaitAllActiveEvtEnd_NoBgSetEndWait(wp)) {
        if (!sw->nozzleWorkEvtId || !evtCheckID(sw->nozzleWorkEvtId)) {
            if (!sw->fallFrameEvtId || !evtCheckID(sw->fallFrameEvtId)) {
                return EVT_RETURN_DONE;
            }
        }
    }
    switch (layer) {
        case STAGE_LAYER_A1:
        case STAGE_LAYER_A2:
            for (i = 0; i < BATTLE_STAGE_OBJECT_MAX; i++) {
                obj = &wp->stageObjects[i];
                if (obj->id > 0 && obj->data->layer == layer) {
                    _mobj_shake_main(obj);
                }
            }
            break;
        case STAGE_LAYER_B:
            break;
    }
    return EVT_RETURN_BLOCK;
}

/** Advances one piece's wobble a frame: rotation.x swings +-5 degrees as a sine over its shake period. */
static void _mobj_shake_main(BattleStageObject* obj) {
    obj->shakePeriodPosition++;
    if (obj->shakePeriodPosition >= obj->shakePeriodLength) {
        obj->shakePeriodPosition = 0;
    }
    obj->shakeAngle = 5.0f * sinfd(obj->shakePeriodPosition * 360 / obj->shakePeriodLength);
    obj->rotation.x = obj->shakeAngle;
}

/**
 * Rolls a new aim for each of the three nozzles from the rank's dirRate weights (no nozzles below rank 1).
 * Rolled by btlevtcmd_WeaponAftereffect after stage-shaking attacks.
 */
void BattleStage_NozzleDirChangeCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    u8 i;

    if (sw->nozzleData) {
        for (i = 0; i < 3; i++) {
            _Nozzle_Change_Check(i, sw->nozzleData->dirRate[i]);
        }
    }
}

/**
 * Picks direction 0-3 for nozzle index from the four weights in rate (0 upright, 1-3 aimed);
 * if it differs and the nozzle is not already turning, starts _nozzle_turn_event toward it.
 */
static void _Nozzle_Change_Check(u8 index, u8* rate) {
    BattleStageObjectWork* sw = &_battleWorkPointer->stageObjectWork;
    u8* cur = &sw->nozzleDir[index];
    EventEntry* evt;
    s32 total = 0;
    u8* p;
    u8 dir;
    int i;

    for (i = 0; i < 4; i++) {
        total += rate[i];
    }
    total = irand(total);
    for (dir = 0, p = rate; dir < 4; p++, dir++) {
        total -= *p;
        if (total < 0) {
            break;
        }
    }
    if (dir != *cur) {
        if (sw->nozzleTurnEvtId[index] && !evtCheckID(sw->nozzleTurnEvtId[index])) {
            sw->nozzleTurnEvtId[index] = 0;
        }
        if (sw->nozzleTurnEvtId[index] == 0) {
            sw->nozzleNextDir[index] = dir;
            evt = evtEntry(_nozzle_turn_event, 10, 0);

            // evt->eventId is read before the NULL test below (evtEntry never returns NULL; a full pool gives the one-past entry).
            sw->nozzleTurnEvtId[index] = evt->eventId;
            if (evt) {
                evt->unitId = index;
            }
        }
    }
}

/**
 * Animates nozzle event->unitId (0 left, 1 centre, 2 right):
 * an upright nozzle first tips its head down to -80 degrees,
 * and the head swings from the old yaw to the new one (45, 0 or -45) over 30 frames. Stores the new direction.
 */
static USER_FUNC(_nozzle_turn) {
    BattleStageObjectWork* sw = &_battleWorkPointer->stageObjectWork;
    s32 index = event->unitId;
    const char* name;
    f32 rotX, rotY, from, to;

    if (isFirstCall) {
        event->userWork[2] = 0;
        event->userWork[3] = 0;
        if (sw->nozzleDir[index]) {
            event->userWork[2] = 30;
        }
    }
    switch (index) {
        case 0:
            name = "nozzle_l";
            break;
        case 1:
            name = "nozzle_c";
            break;
        case 2:
            name = "nozzle_r";
            break;
        default:
            return EVT_RETURN_DONE;
    }
    BattleSearchObjectPtr(name);
    if (event->userWork[2] < 30) {
        event->userWork[2]++;
        rotX = intpl_sub(EASING_CUBIC_IN, 0.0f, -80.0f, event->userWork[2], 30);
    } else {
        rotX = -80.0f;
    }
    if (event->userWork[3] < 30) {
        event->userWork[3]++;
        switch (sw->nozzleDir[index]) {
            case NOZZLE_DIR_PLUS_45:
                from = 45.0f;
                break;
            case NOZZLE_DIR_CENTER:
                from = 0.0f;
                break;
            case NOZZLE_DIR_MINUS_45:
                from = -45.0f;
                break;
            case NOZZLE_DIR_RAISED:
            default:
                from = 0.0f;
                break;
        }
        switch (sw->nozzleNextDir[index]) {
            case NOZZLE_DIR_PLUS_45:
                to = 45.0f;
                break;
            case NOZZLE_DIR_CENTER:
                to = 0.0f;
                break;
            case NOZZLE_DIR_MINUS_45:
                to = -45.0f;
                break;
            case NOZZLE_DIR_RAISED:
            default:
                to = 0.0f;
                break;
        }
        rotY = intpl_sub(EASING_SINE_OUT, from, to, event->userWork[3], 30);
    }
    switch (index) {
        case 0:
            mapObjRotate("n_head_l", rotX, rotY, 0.0f);
            break;
        case 1:
            mapObjRotate("n_head_c", rotX, rotY, 0.0f);
            break;
        case 2:
            mapObjRotate("n_head_r", rotX, rotY, 0.0f);
            break;
    }
    if (event->userWork[2] >= 30 && event->userWork[3] >= 30) {
        sw->nozzleDir[index] = sw->nozzleNextDir[index];
        sw->nozzleTurnEvtId[index] = 0;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Starts _close_wall_event once per battle; called from the first-act sequence of battles that cannot be fled
 * (NPC_BATTLE_NO_ESCAPE).
 */
void BattleStage_WallCloseCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;

    if (!(sw->flags & STAGE_FLAG_WALL_CLOSED)) {
        sw->flags |= STAGE_FLAG_WALL_CLOSED;
        sw->closeWallEvtId = evtEntry(_close_wall_event, 10, 0)->eventId;
    }
}

#if VERSION < VERSION_US
static USER_FUNC(_announce_wall_message) {
    if (isFirstCall) {
        event->userdata[0] = 60;
    }
    if (event->userdata[0] < 0) {
        return EVT_RETURN_DONE;
    }
    event->userdata[0]--;
    dispEntry(CAMERA_2D, 900.0f, DISP_MODE_ALPHA_TEST, _disp_escape_wall_message, NULL);
    return EVT_RETURN_BLOCK;
}

/**
 * 2D draw callback for the JP and early-demo wall banner:
 * a white window frame with btl_msg_escape_wall_close2 centred in the lower half of the screen.
 */
static void _disp_escape_wall_message(CameraId camera, void* param) {
    const char* msg = msgSearch("btl_msg_escape_wall_close2");
    s32 width = FontGetMessageWidth(msg);
    f32 x;

    // FAKE: spare temporary for 0.0f; writing 0.0f in place changes the object
    f32 zero = 0.0f;

    x = zero - width / 2;
    windowDispGX_Waku_col(0, (GXColor){255, 255, 255, 255}, x - 20.0f, -117.0f, width + 40, 30.0f, 10.0f);
    FontDrawStart();
    FontDrawString(zero - (width / 2), -120.0f, msg);
}
#endif

/**
 * Drops the iron frame over the stage (_fall_frame_event) unless it already fell or the stage has none;
 * rolled by btlevtcmd_WeaponAftereffect. Once it is gone, falling objects can be rolled.
 */
void BattleStage_IronFrameFallCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;

    if (!(sw->flags & STAGE_FLAG_KEEP_IRON_FRAME_UNUSED) && !(sw->flags & STAGE_FLAG_NO_IRON_FRAME)) {
        sw->flags |= STAGE_FLAG_NO_IRON_FRAME;
        sw->fallFrameEvtId = evtEntry(_fall_frame_event, 10, 0)->eventId;
    }
}

/** Waits until all battle events (except the background set ones) and any nozzle work are over, then yields a frame. */
static USER_FUNC(_fall_frame_wait) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;

    if (!BattleWaitAllActiveEvtEnd_NoBgSetEndWait(wp)) {
        return EVT_RETURN_BLOCK;
    }
    if (sw->nozzleWorkEvtId) {
        if (evtCheckID(sw->nozzleWorkEvtId)) {
            return EVT_RETURN_BLOCK;
        }
        sw->nozzleWorkEvtId = 0;
    }
    return EVT_RETURN_YIELD;
}

/**
 * Runs each living unit's ceiling-fall script (btlevtcmd_SetEventCeilFall) and waits for all of them to end.
 * JP and later skip units with the instant-kill status.
 */
static USER_FUNC(_fall_frame_wait_unit_ready) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    EventEntry* evt;
    int i;

    if (isFirstCall) {
        for (i = 0; i < BATTLE_UNIT_MAX; i++) {
            unit = BattleGetUnitPtr(wp, i);
#if VERSION <= VERSION_DEMO_JP
            if (unit && !(unit->flags & BATTLE_UNIT_FLAG_BROKEN) && unit->ceilFallEventCode) {
#else
            if (unit && !(unit->flags & BATTLE_UNIT_FLAG_BROKEN) && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) && unit->ceilFallEventCode) {
#endif
                evt = evtEntry(unit->ceilFallEventCode, 10, 0);
                if (evt) {
                    evt->unitId = unit->unitId;
                }

                // Read outside the if (evt) block above (evtEntry never returns NULL; a full pool gives the one-past entry).
                unit->ceilFallEventId = evt->eventId;
            }
        }
    }
    for (i = 0; i < BATTLE_UNIT_MAX; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && !(unit->flags & BATTLE_UNIT_FLAG_BROKEN)) {
            if (unit->ceilFallEventId && !evtCheckID(unit->ceilFallEventId)) {
                unit->ceilFallEventId = 0;
            }
            if (unit->ceilFallEventId) {
                return EVT_RETURN_BLOCK;
            }
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Drops the iron frame (if_body) from its height to the floor over 60 frames,
 * then returns in args[0] the rank's falling-object table, whose first member is the frame's weapon.
 */
static USER_FUNC(_fall_frame) {
    BattleWork* wp;
    s32 arg = event->args[0];
    BattleStageObject* body;
    BattleStageObjectWork* sw;

    wp = _battleWorkPointer;
    sw = &wp->stageObjectWork;
    body = BattleSearchObjectPtr("if_body");
    if (isFirstCall) {
        event->userWork[2] = 0;
        event->userWork[3] = body->position.y;
    }
    event->userWork[2]++;
    body->position.y = intpl_sub(EASING_CUBIC_IN, event->userWork[3], 0.0f, event->userWork[2] * event->userWork[2] / 60, 60);
    if (event->userWork[2] < 60) {
        return EVT_RETURN_BLOCK;
    }
    evtSetValue(event, arg, (EvtWord)sw->fallObjectData);
    return EVT_RETURN_DONE;
}

/** Fades the fallen iron frame out over 51 frames and clears fallFrameEvtId. */
static USER_FUNC(_fall_frame_delete) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;

    if (isFirstCall) {
        event->userWork[2] = 255;
    }
    event->userWork[2] -= 5;
    mapGrpSetColor("if_body", (GXColor){255, 255, 255, event->userWork[2]});
    if (event->userWork[2] > 0) {
        return EVT_RETURN_BLOCK;
    }
    sw->fallFrameEvtId = 0;
    return EVT_RETURN_DONE;
}

/**
 * From rank 1 up, picks the battle's nozzle type if not yet chosen and starts the nozzle event
 * unless one is running.
 * type 0 (first turn, battle_seq.c) only fires when the nozzle type is not 0;
 * type 1 (weapon aftereffects, the Toad audience member's trick) always runs and can start fog.
 */
void BattleStage_NozzleWorkCheck(s32 type) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    EventEntry* evt;

    if (pouchGetPtr()->rank > 0) {
        _nozzle_type_init();
        if (type || sw->nozzleType) {
            if (sw->nozzleWorkEvtId && !evtCheckID(sw->nozzleWorkEvtId)) {
                sw->nozzleWorkEvtId = 0;
            }
            if (sw->nozzleWorkEvtId == 0) {
                if (type) {
                    evt = evtEntry(_nozzle_work_event_1, 10, 0);
                } else {
                    evt = evtEntry(_nozzle_work_event_0, 10, 0);
                }
                sw->nozzleWorkEvtId = evt->eventId;
            }
        }
    }
}

/** Picks the battle's nozzle type 0-3 once from the rank's typeRate weights; nozzleType stays -1 until then. */
static void _nozzle_type_init(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    BattleStageNozzleData* data = sw->nozzleData;
    s32 total;
    s8 i;

    if (sw->nozzleType == -1) {
        total = 0;
        for (i = 0; i < 4; i++) {
            total += data->typeRate[i];
        }
        total = irand(total);
        for (i = 0; i < 4; i++) {
            total -= data->typeRate[i];
            if (total < 0) {
                break;
            }
        }
        sw->nozzleType = i;
    }
}

/** Waits until other battle events (except background set ones) are over and no nozzle is still turning. */
static USER_FUNC(_nozzle_work_wait) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    int i;

    if (!BattleWaitAllActiveEvtEnd_NoBgSetEndWait(wp)) {
        return EVT_RETURN_BLOCK;
    }
    for (i = 0; i < 3; i++) {
        if (sw->nozzleTurnEvtId[i]) {
            if (evtCheckID(sw->nozzleTurnEvtId[i])) {
                return EVT_RETURN_BLOCK;
            }
            sw->nozzleTurnEvtId[i] = 0;
        }
    }
    return EVT_RETURN_DONE;
}

/** Spray angle in degrees for a nozzle's direction: -45 for 1, -135 for 3, -90 for 2 or upright. */
static f32 _get_nozzle_dir(s32 index) {
    switch (_battleWorkPointer->stageObjectWork.nozzleDir[index]) {
        case NOZZLE_DIR_PLUS_45:
            return -45.0f;
        case NOZZLE_DIR_MINUS_45:
            return -135.0f;
        case NOZZLE_DIR_CENTER:
        default:
            return -90.0f;
    }
}

/**
 * Fires the nozzles for 90 frames with nozzleType's effect and sound (1 smoke, 2 silent, 3 fire);
 * upright ones spray straight up, with confetti when args[0] is 0.
 * Type 0 with args[0] set instead starts a dry-ice fog lasting two turns.
 * Returns 1 in args[1] once done if something fired, 0 at once if not or if the battle is already decided.
 */
static USER_FUNC(_nozzle_work_effect) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    EvtWord* args = event->args;
    const char* sfx;
    s32 effType = 0;
    BOOL started;
    BattleStageObject* obj;
    f32 z, rot, angle1, angle2, angle3;
    s32 fog;
    s32 out;

    fog = evtGetValue(event, args[0]);
    out = args[1];
    started = FALSE;
    if (isFirstCall) {
        if (BattleCheckConcluded(wp)) {
            evtSetValue(event, out, 0);
            return EVT_RETURN_DONE;
        }
        event->userWork[0] = 90;
        switch (sw->nozzleType) {
            case STAGE_NOZZLE_ICE:
                sfx = "SFX_BTL_STAGE_SMOKE1";
                effType = 0;
                break;
            case STAGE_NOZZLE_EXPLOSION:
                sfx = NULL;
                effType = 1;
                break;
            case STAGE_NOZZLE_FIRE:
                sfx = "SFX_BTL_STAGE_FIRE1";
                effType = 2;
                break;
        }
        switch (sw->nozzleType) {
            case STAGE_NOZZLE_ICE:
            case STAGE_NOZZLE_EXPLOSION:
            case STAGE_NOZZLE_FIRE:
                obj = BattleSearchObjectPtr("nozzle_l");
                angle1 = _get_nozzle_dir(0);
                if (sw->nozzleDir[0]) {
                    if (sfx) {
                        event->userWork[5] = psndSFXOn_3D(sfx, &obj->position);
                    } else {
                        event->userWork[5] = -1;
                    }

                    // FAKE: the sum goes through rot first; rot = reviseAngle(90.0f + angle1) changes the object
                    rot = 90.0f + angle1;
                    rot = reviseAngle(rot);
                    z = 10.0f * sinfd(angle1) + obj->position.z;
                    event->userWork[1] = (EvtWord)effNozleEntry(effType, obj->position.x - 10.0f * cosfd(angle1), 12.0f + obj->position.y, z, -80.0f, rot, 90);
                } else {
                    if (sfx) {
                        event->userWork[5] = psndSFXOn_3D(sfx, &obj->position);
                    } else {
                        event->userWork[5] = -1;
                    }
                    event->userWork[1] = (EvtWord)effNozleEntry(effType, obj->position.x, 20.0f + obj->position.y, 8.0f + obj->position.z, 0.0f, 0.0f, 90);
                    if (fog == 0) {
                        effConfettiN64Entry(4, 90, obj->position.x, 20.0f + obj->position.y, 8.0f + obj->position.z, 1.0f);
                    }
                }
                obj = BattleSearchObjectPtr("nozzle_c");
                angle2 = _get_nozzle_dir(1);
                if (sw->nozzleDir[1]) {
                    if (sfx) {
                        event->userWork[6] = psndSFXOn_3D(sfx, &obj->position);
                    } else {
                        event->userWork[6] = -1;
                    }
                    rot = reviseAngle(90.0f + angle2);
                    z = 10.0f * sinfd(angle2) + obj->position.z;
                    event->userWork[2] = (EvtWord)effNozleEntry(effType, obj->position.x - 10.0f * cosfd(angle2), 12.0f + obj->position.y, z, -80.0f, rot, 90);
                } else {
                    if (sfx) {
                        event->userWork[6] = psndSFXOn_3D(sfx, &obj->position);
                    } else {
                        event->userWork[6] = -1;
                    }
                    event->userWork[2] = (EvtWord)effNozleEntry(effType, obj->position.x, 20.0f + obj->position.y, 8.0f + obj->position.z, 0.0f, 0.0f, 90);
                    if (fog == 0) {
                        effConfettiN64Entry(4, 90, obj->position.x, 20.0f + obj->position.y, 8.0f + obj->position.z, 1.0f);
                    }
                }
                obj = BattleSearchObjectPtr("nozzle_r");
                angle3 = _get_nozzle_dir(2);
                if (sw->nozzleDir[2]) {
                    if (sfx) {
                        event->userWork[7] = psndSFXOn_3D(sfx, &obj->position);
                    } else {
                        event->userWork[7] = -1;
                    }

                    // FAKE: z and rot swap roles here (z holds the angle); the left/centre nozzle form changes the object
                    z = reviseAngle(90.0f + angle3);
                    rot = 10.0f * sinfd(angle3) + obj->position.z;
                    event->userWork[3] = (EvtWord)effNozleEntry(effType, obj->position.x - 10.0f * cosfd(angle3), 12.0f + obj->position.y, rot, -80.0f, z, 90);
                } else {
                    if (sfx) {
                        event->userWork[7] = psndSFXOn_3D(sfx, &obj->position);
                    } else {
                        event->userWork[7] = -1;
                    }
                    event->userWork[3] = (EvtWord)effNozleEntry(effType, obj->position.x, 20.0f + obj->position.y, 8.0f + obj->position.z, 0.0f, 0.0f, 90);
                    if (fog == 0) {
                        effConfettiN64Entry(4, 90, obj->position.x, 20.0f + obj->position.y, 8.0f + obj->position.z, 1.0f);
                    }
                }
                started = TRUE;
                break;
            default:
                if (fog && !sw->fogEffect) {
                    event->userWork[0] = 120;
                    event->userWork[1] = 0;
                    event->userWork[3] = 0;
                    event->userWork[2] = 0;
                    sw->fogEffect = effNozleEntry(3, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1000);
                    event->userWork[3] = (EvtWord)sw->fogEffect;
                    sw->fogTimer = 2;
                    event->userWork[5] = psndSFXOn_3D("SFX_BTL_STAGE_DRYICE1", &BattleSearchObjectPtr("nozzle_c")->position);
                    started = TRUE;
                    event->userWork[6] = -1;
                    event->userWork[7] = -1;
                }
                break;
        }
        if (!started) {
            evtSetValue(event, out, 0);
            return EVT_RETURN_DONE;
        }
    }
    if (event->userWork[0] > 0) {
        event->userWork[0]--;
        if (event->userWork[0] <= 0) {
            // FAKE: (u32) casts compared with 0xFFFFFFFF; the plain != -1 changes the object
            if ((u32)event->userWork[5] != 0xFFFFFFFF) {
                psndSFXOff(event->userWork[5]);
            }
            if ((u32)event->userWork[6] != 0xFFFFFFFF) {
                psndSFXOff(event->userWork[6]);
            }
            if ((u32)event->userWork[7] != 0xFFFFFFFF) {
                psndSFXOff(event->userWork[7]);
            }
        }
        return EVT_RETURN_BLOCK;
    }
    evtSetValue(event, out, 1);
    return EVT_RETURN_DONE;
}

/**
 * Counts the stage fog down at the start of each turn (btlseqTurnFirstProcess) and removes the fog effect
 * when it runs out. Returns TRUE when no fog is left.
 */
BOOL BattleFogEndCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;

    if (sw->fogTimer == 0) {
        return TRUE;
    }
    sw->fogTimer--;
    if (sw->fogTimer > 0) {
        return FALSE;
    }
    if (sw->fogEffect) {
        effSoftDelete(sw->fogEffect);
        sw->fogEffect = NULL;
    }
    return TRUE;
}

/**
 * Removes the fog effect at once (btlevtcmd_StageDispellFog, a Hold Fast counter blast, the battle end).
 * Returns TRUE if fog was active; fogTimer is left to run out on its own.
 */
BOOL BattleFogForceStop(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;

    if (sw->fogTimer == 0) {
        return FALSE;
    }
    if (sw->fogEffect) {
        effSoftDelete(sw->fogEffect);
        sw->fogEffect = NULL;
    }
    return TRUE;
}

/**
 * Builds the nozzle weapons for this burst: starts from the Mario-side and enemy-side base weapons
 * and adds the nozzle type's weapon once for every nozzle aimed at that side.
 * A side no nozzle hits keeps a NULL weapon.
 */
static USER_FUNC(_nozzle_work_attack_set) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    s32 index;
    int i;

    sw->nozzleWeapons[0] = NULL;
    sw->nozzleWeapons[1] = NULL;
    memset(&sw->nozzleWeaponWork[0], 0, sizeof(BattleWeapon));
    memset(&sw->nozzleWeaponWork[1], 0, sizeof(BattleWeapon));
    _combine_nozzle_weapon(TRUE, &sw->nozzleWeaponWork[0], &_nozzle_at_base_mario.weapon);
    _combine_nozzle_weapon(TRUE, &sw->nozzleWeaponWork[1], &_nozzle_at_base_monster.weapon);
    for (i = 0; i < 3; i++) {
        index = _get_nozzle_attack_index(i);
        if (index >= 0) {
            if (!sw->nozzleWeapons[index]) {
                sw->nozzleWeapons[index] = &sw->nozzleWeaponWork[index];
            }
            _combine_nozzle_weapon(FALSE, sw->nozzleWeapons[index], &sw->nozzleData->weapons[sw->nozzleType].weapon);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Which side nozzle index hits with its current direction:
 * 0 Mario's side, 1 the enemy side, -1 neither (upright or aimed away).
 */
static s32 _get_nozzle_attack_index(u8 index) {
    BattleStageObjectWork* sw = &_battleWorkPointer->stageObjectWork;

    // Unmatched directions fall through the remaining nozzle cases to the final -1.
    switch (index) {
        case 0:
            switch (sw->nozzleDir[index]) {
                case NOZZLE_DIR_PLUS_45:
                    return -1;
                case NOZZLE_DIR_CENTER:
                    return 0;
                case NOZZLE_DIR_MINUS_45:
                    return 1;
            }
        case 2:
            switch (sw->nozzleDir[index]) {
                case NOZZLE_DIR_PLUS_45:
                    return 0;
                case NOZZLE_DIR_CENTER:
                    return 1;
                case NOZZLE_DIR_MINUS_45:
                    return -1;
            }
        case 1:
            switch (sw->nozzleDir[index]) {
                case NOZZLE_DIR_PLUS_45:
                    return 0;
                case NOZZLE_DIR_CENTER:
                    return -1;
                case NOZZLE_DIR_MINUS_45:
                    return 1;
            }
    }
    return -1;
}

/**
 * Merges one status inflict pair into a weapon:
 * keeps the higher chance and adds the turns, when the source has one.
 */
static void _rate_mix(s8* dstChance, s8* dstTime, s8* srcChance, s8* srcTime) {
    if (*srcChance <= 0) {
        return;
    }
    if (*srcChance > *dstChance) {
        *dstChance = *srcChance;
    }
    *dstTime += *srcTime;
}

/**
 * Like _rate_mix for statuses with a strength (size, defence):
 * also adds the strengths, and drops the status if the strengths cancel to 0.
 */
static void _rate_mix_3(s8* dstChance, s8* dstTime, s8* dstStrength, s8* srcChance, s8* srcTime, s8* srcStrength) {
    if (*srcChance <= 0) {
        return;
    }
    if (*srcChance > *dstChance) {
        *dstChance = *srcChance;
    }
    *dstTime += *srcTime;
    *dstStrength += *srcStrength;
    if (*dstStrength == 0) {
        *dstChance = 0;
        *dstTime = 0;
    }
}

/**
 * copy TRUE copies src into dst; FALSE adds src onto dst:
 * FP/SP costs and damage parameters summed, element replaced, statuses merged with _rate_mix.
 * Attack change is not merged, and poison adds src's strength to dst's turns, as in the original.
 */
static void _combine_nozzle_weapon(BOOL copy, BattleWeapon* dst, BattleWeapon* src) {
    s8* d;
    s8* s;
    int i;

    if (copy) {
        *dst = *src;
        return;
    }
    dst->baseFpCost += src->baseFpCost;
    dst->baseSpCost += src->baseSpCost;
    if (!dst->hpDamageFn) {
        dst->hpDamageFn = src->hpDamageFn;
    }
    for (i = 0; i < 8; i++) {
        dst->hpDamageWork[i] += src->hpDamageWork[i];
    }
    dst->element = src->element;
    s = (s8*)&src->sleepChance; // status chance/time block from 0x80, indexed below
    d = (s8*)&dst->sleepChance;
    _rate_mix(&d[0x0], &d[0x1], &s[0x0], &s[0x1]); // sleep
    _rate_mix(&d[0x2], &d[0x3], &s[0x2], &s[0x3]); // stop
    _rate_mix(&d[0x4], &d[0x5], &s[0x4], &s[0x5]); // dizzy

    // BUG: adds the source's poisonStrength (s[0x8]) to poisonTime instead of its poisonTime (s[0x7]);
    // poisonStrength itself is not combined
    _rate_mix(&d[0x6], &d[0x7], &s[0x6], &s[0x8]); // poison
    _rate_mix(&d[0x9], &d[0xA], &s[0x9], &s[0xA]); // confuse
    _rate_mix(&d[0xB], &d[0xC], &s[0xB], &s[0xC]); // electric
    _rate_mix(&d[0xD], &d[0xE], &s[0xD], &s[0xE]); // dodgy
    _rate_mix(&d[0xF], &d[0x10], &s[0xF], &s[0x10]); // burn
    _rate_mix(&d[0x11], &d[0x12], &s[0x11], &s[0x12]); // freeze
    _rate_mix_3(&d[0x13], &d[0x14], &d[0x15], &s[0x13], &s[0x14], &s[0x15]); // size change
    _rate_mix_3(&d[0x19], &d[0x1A], &d[0x1B], &s[0x19], &s[0x1A], &s[0x1B]); // defense change (attack change 0x16..0x18 is not combined)
    _rate_mix(&d[0x1C], &d[0x1D], &s[0x1C], &s[0x1D]); // allergic
    d[0x1F] += s[0x1F]; // chargeStrength
    _rate_mix(&d[0x20], &d[0x21], &s[0x20], &s[0x21]); // fast
    _rate_mix(&d[0x22], &d[0x23], &s[0x22], &s[0x23]); // slow
    d[0x2A] += s[0x2A]; // hpRegenTime
    d[0x2B] += s[0x2B]; // hpRegenStrength
    d[0x2C] += s[0x2C]; // fpRegenTime
    d[0x2D] += s[0x2D]; // fpRegenStrength
}

/** Returns in args[1] the nozzle weapon for side args[0] (0 Mario's side, 1 enemies), or 0 if no nozzle hits it. */
static USER_FUNC(_nozzle_work_get_weapon_address) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], (EvtWord)sw->nozzleWeapons[index]);
    return EVT_RETURN_DONE;
}

/**
 * Starts _object_fall_event unless the battle disables falling objects (BATTLE_FLAG_NO_OBJECT_FALL)
 * and returns its event id (0 if not started).
 * Rolled by btlevtcmd_WeaponAftereffect after the iron frame is gone,
 * and used by the Toad audience member's trick.
 */
s32 BattleStage_ObjectFallCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;
    EventEntry* evt;

    if (wp->flags & BATTLE_FLAG_NO_OBJECT_FALL) {
        return 0;
    }
    evt = evtEntry(_object_fall_event, 10, 0);
    sw->objectFallEvtId = evt->eventId;
    return evt->eventId;
}

/** Waits until other battle events, nozzle work, the iron frame fall and any background set fall are all over. */
static USER_FUNC(_object_fall_wait) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = &wp->stageObjectWork;

    if (!BattleWaitAllActiveEvtEnd_NoBgSetEndWait(wp)) {
        return EVT_RETURN_BLOCK;
    }
    if (sw->nozzleWorkEvtId) {
        if (evtCheckID(sw->nozzleWorkEvtId)) {
            return EVT_RETURN_BLOCK;
        }
        sw->nozzleWorkEvtId = 0;
    }
    if (sw->fallFrameEvtId) {
        if (evtCheckID(sw->fallFrameEvtId)) {
            return EVT_RETURN_BLOCK;
        }
        sw->fallFrameEvtId = 0;
    }
    if (sw->bgDestroyEvtId) {
        if (evtCheckID(sw->bgDestroyEvtId)) {
            return EVT_RETURN_BLOCK;
        }
        sw->bgDestroyEvtId = 0;
    }
    return EVT_RETURN_DONE;
}

typedef struct FallObjectEntry {
    s8 index; // 0x0
    s8 type; // 0x1
    s32 kind; // 0x4, 0 = unit, 1 = audience
    s32 unitId; // 0x8
    s32 partId; // 0xC
    s16 delay; // 0x10
    s16 reserved_12; // 0x12
    u8 noAc; // 0x14
    u8 forceHit; // 0x15, set on earlier entries for the same unit once a hit is scheduled; damage then skips the evasion rolls
    u8 pad_16[0x1C - 0x16]; // 0x16, never accessed
    s32 preCheck; // 0x1C
    s32 sfxId; // 0x20
    EffectEntry* effect; // 0x24
    BattleWeaponData weapon; // 0x28
} FallObjectEntry;

typedef struct FallObjectWork {
    u8 count; // 0x0
    s8 prevTypes[3]; // 0x1
    FallObjectEntry entries[1]; // 0x4
} FallObjectWork;

/**
 * Drops one ceiling object: picks its type from the rank's rates (the branch that repeats earlier picks once
 * three are recorded never runs: the work is new each call and holds one entry), aims it at a target the type's weapon can reach or, 10% of the time once Mario has a Star Power,
 * at a random audience member, then waits for the effect to land and deals the damage (guard check, dodges,
 * Superguard and lucky reactions). Returns 1 in args[0] if it fell on the audience.
 */
static USER_FUNC(_object_fall_attack) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageFallObjectData* fd = wp->stageObjectWork.fallObjectData;
    const char* sfxNormal;
    BOOL done;
    s32 acCount;
    EvtWord* args = event->args;
    s32 outArg = *(args++);
    FallObjectEntry* e3;
    BattleUnit* unit;
    BattleUnitPart* part;
    s16 delay;
    BattleWeapon* weapon3;
    s32 j;
    BattleWeapon* weapon;
    BOOL found = FALSE;
    s32 k2;
    FallObjectWork* w;
    FallObjectEntry* e;
    FallObjectEntry* o;
    s32 k;
    const char* sfxTub2;
    FallObjectEntry* o2;
    const char* sfxRest;
    s32 i;
    s32 sum;
    s32 r;
    u32 flags;
    f32 width;
    f32 dir;
    BattleWorkWeaponTargets targets;
    BattleWorkWeaponTargets targets2;
    Vec pos;
    Vec hit;
    Vec sfxPos;
    Vec p;
    s32 unitId;
    s32 partId;
    EffectEntry* eff;
    if (isFirstCall) {
        w = BattleAlloc(sizeof(FallObjectWork));
        event->userdatap[0] = w;
        event->userdata[1] = 0;
        memset(w, 0, sizeof(FallObjectWork));
        w->entries[0].index = -1;
        for (i = 0; i < 3; i++) {
            w->prevTypes[i] = -1;
        }

        if (BattleCheckConcluded(wp)) {
            evtSetValue(event, outArg, event->userdata[1]);
            return EVT_RETURN_DONE;
        }
        w->count = 1;
        for (i = 0; i < w->count; i++) {
            e = &w->entries[i];
            e->index = i;
            for (j = 0; j < 3; j++) {
                if (w->prevTypes[j] == -1) {
                    break;
                }
            }

            if (j >= 3) {
                e->type = w->prevTypes[irand(3)];
            } else {
                sum = 0;
                for (k = 0; k < 9; k++) {
                    sum += fd->rates[k];
                }

                r = irand(sum);
                for (e->type = 0; e->type < 8; e->type++) {
                    if ((r -= fd->rates[e->type]) < 0) {
                        break;
                    }
                }

                w->prevTypes[j] = e->type;
            }
            e->weapon = fd->weapons[e->type];
            if (e->type == STAGE_FALL_BOWSER_STATUE) {
                if (irand(100) < 50) {
                    e->weapon.weapon.targetClassFlags |= TARGET_CLASS_NO_OPPOSITE_SIDE;
                    e->weapon.weapon.targetClassFlags &= ~TARGET_CLASS_NO_OWN_SIDE;
                } else {
                    e->weapon.weapon.targetClassFlags &= ~TARGET_CLASS_NO_OPPOSITE_SIDE;
                    e->weapon.weapon.targetClassFlags |= TARGET_CLASS_NO_OWN_SIDE;
                }
            }
            weapon = &e->weapon.weapon;
            e->kind = 0;
            if (pouchGetPtr()->starPowersObtained && !(BattleAudienceBaseGetPtr()->flags & AUDIENCE_FLAG_NO_AUDIENCE_EVENTS) && e->type != STAGE_FALL_BOWSER_STATUE && e->type != STAGE_FALL_METEOR) {
                if ((wp->debug.flags & BATTLE_DEBUG_FORCE_FALL_ON_AUDIENCE) || irand(100) < 10) {
                    e->kind = 1;
                    e->partId = -1;
                    e->unitId = BattleAudience_HaitiRandForFallObject();
                    found = TRUE;
                    event->userdata[1] = 1;
                }
            }
            if (e->kind == 0) {
                BattleSamplingEnemy(&targets, weapon, BattleGetSystemPtr(wp)->unitId, 1, weapon->targetClassFlags, weapon->targetPropertyFlags, -wp->alliances[1].attackDirection);
                if (targets.numTargets <= 0) {
                    evtSetValue(event, outArg, event->userdata[1]);
                    return EVT_RETURN_DONE;
                }
                found = TRUE;
                BattleChoiceSamplingEnemy(&targets, weapon->targetWeightFlags, &unitId, &partId);
                e->unitId = unitId;
                e->partId = partId;
                e->noAc = 1;
                for (k = i; k >= 0; k--) {
                    o = &w->entries[k];
                    if (e != o && o->kind == 0 && e->unitId == o->unitId) {
                        o->noAc = 0;
                    }
                }
            }
        }

        if (!found) {
            evtSetValue(event, outArg, event->userdata[1]);
            return EVT_RETURN_DONE;
        }
        for (i = 0; i < w->count; i++) {
            e = &w->entries[i];
            weapon = &e->weapon.weapon;
            if (e->kind == 0) {
                flags = 0;
                if (e->noAc) {
                    flags |= DAMAGE_RESULT_FINAL_HIT;
                }
                if (e->forceHit) {
                    flags |= DAMAGE_RESULT_NO_EVADE;
                }
                e->preCheck = BattlePreCheckDamage(BattleGetSystemPtr(wp), BattleGetUnitPtr(wp, e->unitId), BattleGetUnitPartsPtr(e->unitId, e->partId), weapon, flags);
                if (e->preCheck == PRECHECK_HIT && !e->noAc) {
                    delay = irand(0);
                    for (k2 = 0; k2 < i; k2++) {
                        o2 = &w->entries[k2];
                        if (e->unitId == o2->unitId) {
                            o2->delay = delay;
                            delay += irand(22) + 20;

                            // BUG: o2 is an earlier entry whose pre-check already ran, and forceHit is read only by
                            // that pre-check, so setting it here changes nothing.
                            if (o2->noAc) {
                                o2->forceHit = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    sfxTub2 = (0, "SFX_BTL_STAGE_DAMAGE_TUB1"); // FAKE: comma expr makes the literal an early @ temp (same pointer)
    sfxNormal = "SFX_BTL_STAGE_DAMAGE_NORMAL1";
    sfxRest = "SFX_REST_CONDITION1";
    w = event->userdatap[0];
    done = TRUE;
    for (i = 0; i < w->count; i++) {
        e3 = &w->entries[i];
        if (e3->index == -1) {
            continue;
        }
        acCount = 0;
        done = FALSE;
        if (e3->kind == 0) {
            unit = BattleGetUnitPtr(wp, e3->unitId);
            part = BattleGetUnitPartsPtr(e3->unitId, e3->partId);
        }
        if (e3->effect == NULL || e3->delay > 0) {
            e3->delay--;
            if (e3->delay > 0) {
                continue;
            }
            switch (e3->kind) {
                case 0:
                    BtlUnit_GetHitPos(unit, part, &hit.x, &hit.y, &hit.z);
                    width = BtlUnit_GetWidth(unit);
                    break;

                case 1:
                    hit.x = ((e3->unitId % 20) * 25) - 230;
                    hit.y = BattleStageGetPtr()->audienceSizeY;
                    hit.z = (0.1f * (e3->unitId % 20)) + (((e3->unitId / 20) * 30) + 115);
                    btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_AUDIENCE);
                    btl_camera_set_moveSpeedLv(BTL_CAM_PRI_NORMAL, 2);
                    width = 0.0f;
                    break;

                default:
                    hit = (Vec){0.0f, 0.0f, 0.0f};
                    width = 0.0f;
                    break;
            }

            pos = hit;
            pos.y = BattleGetFloorHeight(wp, pos.x, pos.y, pos.z);
            if (hit.x >= 0.0f) {
                dir = 90.0f;
            } else {
                dir = 270.0f;
            }
            switch (e3->preCheck) {
                case PRECHECK_MISS:
                case PRECHECK_LUCKY:
                case PRECHECK_INTANGIBLE:
                case PRECHECK_DODGY:
                    hit.y = pos.y;
                    break;
            }

            e3->sfxId = -1;
            e3->reserved_12 = 0;
            switch (e3->type) {
                case STAGE_FALL_BASIN:
                    e3->effect = effFallEntry(4, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
                    break;

                case STAGE_FALL_BUCKET:
                    e3->effect = effFallEntry(5, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
                    break;

                case STAGE_FALL_SMALL_BUGS:
                    e3->effect = effFallEntry(0, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 100, 90);
                    break;

                case STAGE_FALL_LARGE_BUG:
                    e3->effect = effFallEntry(1, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
                    break;

                case STAGE_FALL_FORK:
#if VERSION <= VERSION_DEMO_JP
                    e3->effect = effFallEntry(2, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
#else
                    e3->effect = effFallEntry(2, pos.x, pos.y, 5.0f + pos.z, width, hit.y - pos.y, dir, 1, 90);
#endif
                    break;

                case STAGE_FALL_STAGE_LIGHT:
                    e3->effect = effFallEntry(8, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
                    break;

                case STAGE_FALL_WATER:
                    e3->effect = effFallEntry(3, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
                    break;

                case STAGE_FALL_BOWSER_STATUE:
                    if (BtlUnit_GetBelong(unit) == 0) {
#if VERSION <= VERSION_DEMO_JP
                        e3->effect = effFallEntry(6, -300.0f, pos.y, pos.z, 0.0f, 0.0f, dir, 1, 180);
#else
                        e3->effect = effFallEntry(6, -300.0f, pos.y, 15.0f + pos.z, 0.0f, 0.0f, dir, 1, 180);
#endif
                    } else {
#if VERSION <= VERSION_DEMO_JP
                        e3->effect = effFallEntry(6, 300.0f, pos.y, pos.z, 0.0f, 0.0f, dir, 1, 180);
#else
                        e3->effect = effFallEntry(6, 300.0f, pos.y, 15.0f + pos.z, 0.0f, 0.0f, dir, 1, 180);
#endif
                    }
                    break;

                case STAGE_FALL_METEOR:
#if VERSION <= VERSION_DEMO_JP
                    e3->effect = effFallEntry(7, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 180);
#else
                    e3->effect = effFallEntry(7, pos.x, pos.y, 5.0f + pos.z, width, hit.y - pos.y, dir, 1, 180);
#endif
                    break;
            }

        } else {
            if (e3->effect != NULL && !effFallCheckHit(e3->effect)) {
                continue;
            }
            weapon3 = &e3->weapon.weapon;
            if (e3->kind == 1) {
                BattleAudience_Case_FallObject_Aud(e3->unitId, e3->type);
                psndSFXOff(e3->sfxId);
                BattleAudience_GetHomePosition(e3->unitId, &sfxPos.x, &sfxPos.y, &sfxPos.z);
                switch (e3->type) {
                    case STAGE_FALL_WATER:
                        break;

                    case STAGE_FALL_BASIN:
                    case STAGE_FALL_BUCKET:
                        psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_DAMAGE_TUB1"), &sfxPos); // FAKE: integer cast makes the literal a hoisted @ temp (same pointer)
                        break;

                    default:
                        psndSFXOn_3D(sfxNormal, &sfxPos);
                        break;
                }

            } else {
                BattleAudience_Case_FallObject_Stage();
                if (weapon3->targetClassFlags & TARGET_CLASS_SINGLE) {
                    BtlUnit_GetHitPos(unit, part, &sfxPos.x, &sfxPos.y, &sfxPos.z);
                    flags = 0;
                    if (e3->noAc) {
                        flags |= DAMAGE_RESULT_FINAL_HIT;
                    }
                    psndSFXOff(e3->sfxId);
                    switch (e3->preCheck) {
                        case PRECHECK_MISS:
                        case PRECHECK_LUCKY:
                        case PRECHECK_INTANGIBLE:
                        case PRECHECK_DODGY:
                            switch (e3->type) {
                                case STAGE_FALL_WATER:
                                    break;
                                case STAGE_FALL_BASIN:
                                case STAGE_FALL_BUCKET:
                                    psndSFXOn_3D(sfxTub2, &sfxPos);
                                    break;
                            }
                            break;
                        case PRECHECK_HIT:
                            switch (e3->type) {
                                case STAGE_FALL_BASIN:
                                case STAGE_FALL_BUCKET:
                                    psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_DAMAGE_TUB1"), &sfxPos); // FAKE: integer cast makes the literal a hoisted @ temp (same pointer)
                                    break;
                                case STAGE_FALL_WATER:
#if VERSION >= VERSION_JP
                                    psndSFXOn_3D(sfxRest, &sfxPos);
#endif
                                    break;
                                default:
                                    psndSFXOn_3D(sfxNormal, &sfxPos);
                                    break;
                            }
                            if (BtlUnit_GetACPossibility(unit)) {
                                BattleActionCommandCheckDefence(unit, weapon3);
                            }
                            BattleCheckDamage(BattleGetSystemPtr(wp), unit, part, weapon3, flags);
                            break;
                    }

                } else {
                    psndSFXOff(e3->sfxId);
                    BattleSamplingEnemy(&targets2, weapon3, BattleGetSystemPtr(wp)->unitId, 1, weapon3->targetClassFlags, weapon3->targetPropertyFlags, -wp->alliances[1].attackDirection);
                    if (targets2.numTargets > 0) {
                        for (;;) {
                            BattleWorkTarget* tg;
                            BattleUnit* unit2;
                            if (targets2.currentTarget >= targets2.numTargets) {
                                break;
                            }
                            tg = &targets2.targets[targets2.targetIndices[targets2.currentTarget]];
                            unit2 = BattleGetUnitPtr(wp, tg->unitIdx);
                            switch (BattlePreCheckDamage(BattleGetSystemPtr(wp), unit2, BattleGetUnitPartsPtr(tg->unitIdx, tg->partIdx), weapon3, DAMAGE_RESULT_FINAL_HIT)) {
                                case PRECHECK_MISS:
                                case PRECHECK_INTANGIBLE:
                                    break;

                                case PRECHECK_LUCKY:
                                    BtlUnit_GetPos(unit2, &p.x, &p.y, &p.z);
                                    if (p.y < 85.0f) {
                                        p.y += 50.0f;
                                    } else {
                                        p.y += -50.0f;
                                    }
                                    p.x -= 45.0f;
                                    eff = effNiceEntry(NICE_EFFECT_LUCKY, p.x, p.y, p.z);
                                    ((NiceEffectWork*)eff->userdata)->overallScale = 0.75f;
                                    unit2->damagePattern = 0;
                                    BattleRunHitEvent(unit2, DAMAGE_RESULT_FINAL_HIT | DAMAGE_CODE_LUCKY);
                                    break;

                                case PRECHECK_DODGY:
                                    unit2->damagePattern = 0;
                                    BattleRunHitEvent(unit2, DAMAGE_RESULT_FINAL_HIT | DAMAGE_CODE_DODGY);
                                    break;

                                case PRECHECK_HIT:
                                    if (BtlUnit_GetACPossibility(unit2) && acCount <= 0) {
                                        BattleActionCommandCheckDefence(unit2, weapon3);
                                        acCount++;
                                    }
                                    BattleCheckDamage(BattleGetSystemPtr(wp), unit2, BattleGetUnitPartsPtr(tg->unitIdx, tg->partIdx), weapon3, DAMAGE_RESULT_FINAL_HIT);
                                    break;
                            }

                            targets2.currentTarget++;
                        }
                    }
                }
            }
            e3->index = -1;
        }
        w->entries[0].weapon.weapon.reserved_14 = w->entries[0].weapon.weapon.reserved_14; // FAKE: no-op self-assign (no code) raises w spill cost
    }

    if (!done) {
        return EVT_RETURN_BLOCK;
    }
    BattleFree(event->userdatap[0]);
    evtSetValue(event, outArg, event->userdata[1]);
    return EVT_RETURN_DONE;
}

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

#pragma dont_inline on

#define STAGE_WORK(wp) (&(wp)->stageObjectWork)

void mapObjTranslate(const char* name, f32 x, f32 y, f32 z);
void mapObjRotate(const char* name, f32 x, f32 y, f32 z);
void mapGrpSetColor(const char* name, GXColor color);
void mapPlayAnimationLv(const char* name, s32 mode, s32 level);
f32 intpl_sub(s32 type, f32 start, f32 end, s32 cur, s32 max);
f32 sinfd(f32 degrees);
f32 cosfd(f32 degrees);
void FontDrawString(f32 x, f32 y, const char* str);
BOOL BattleCheckConcluded(BattleWork* wp);
BOOL BattleWaitAllActiveEvtEnd_NoBgSetEndWait(BattleWork* wp);
void BattleAudience_Case_HaikeiSet(void);
void BattleAudience_Case_FallObject_Aud(s32 index, u8 type);
void BattleAudience_Case_FallObject_Stage(void);
EffectEntry* effNozleEntry(s32 type, f32 x, f32 y, f32 z, f32 rx, f32 ry, s32 lifetime);
EffectEntry* effFallEntry(s32 type, f32 x, f32 y, f32 z, f32 width, f32 drop, f32 dir, s32 num, s32 duration);
BOOL effFallCheckHit(EffectEntry* effect);
void BattleSamplingEnemy(BattleWorkWeaponTargets* targets, BattleWeapon* weapon, s32 unitId, s8 belong, u32 classFlags, u32 propFlags, s8 direction);
void BattleChoiceSamplingEnemy(BattleWorkWeaponTargets* targets, u32 flags, s32* unitId, s32* partId);
s32 BattlePreCheckDamage(BattleUnit* attacker, BattleUnit* target, BattleUnitPart* part, BattleWeapon* weapon, u32 flags);
BOOL BtlUnit_GetACPossibility(BattleUnit* unit);
s32 BattleActionCommandCheckDefence(BattleUnit* unit, BattleWeapon* weapon);
f32 BattleGetFloorHeight(BattleWork* wp, f32 x, f32 y, f32 z);

//local prototypes
void BattleObjectConfig(void);
void _mobj_shake_main(BattleStageObject* obj);
void _Nozzle_Change_Check(u8 index, u8* rate);
void _disp_escape_wall_message(CameraId camera, void* param);
void _nozzle_type_init(void);
f32 _get_nozzle_dir(s32 index);
s32 _get_nozzle_attack_index(u8 index);
void _rate_mix(s8* dstChance, s8* dstTime, s8* srcChance, s8* srcTime);
void _rate_mix_3(s8* dstChance, s8* dstTime, s8* dstStrength, s8* srcChance, s8* srcTime, s8* srcStrength);
void _combine_nozzle_weapon(BOOL copy, BattleWeapon* dst, BattleWeapon* src);
USER_FUNC(_object_fall_attack);
USER_FUNC(_object_fall_wait);
USER_FUNC(_nozzle_work_get_weapon_address);
USER_FUNC(_nozzle_work_attack_set);
USER_FUNC(_nozzle_work_effect);
USER_FUNC(_nozzle_work_wait);
USER_FUNC(_fall_frame_delete);
USER_FUNC(_fall_frame);
USER_FUNC(_fall_frame_wait_unit_ready);
USER_FUNC(_fall_frame_wait);
#ifdef VERSION_G8ME01
USER_FUNC(btlevtcmd_AnnounceMessage);
#else
USER_FUNC(_announce_wall_message);
#endif
USER_FUNC(_nozzle_turn);
USER_FUNC(_set_mobj_shake);
USER_FUNC(_bgset_audience_bgset_access);
USER_FUNC(_bgset_audience_bgset_access);
USER_FUNC(_bgset_kemuri_effect);
USER_FUNC(_get_bgset_attack_data);
USER_FUNC(_mobj_destroy);
USER_FUNC(_mobj_set_alpha);
USER_FUNC(_set_mobj_rotate_x);

USER_FUNC(btlevtcmd_CheckDamage);
USER_FUNC(btlevtcmd_GetACPossibility);
USER_FUNC(btlevtcmd_GetEnemyBelong);
USER_FUNC(btlevtcmd_GetSelectEnemy);
USER_FUNC(btlevtcmd_GetSelectNextEnemy);
USER_FUNC(btlevtcmd_PreCheckDamage);
USER_FUNC(btlevtcmd_ResultACDefence);
USER_FUNC(btlevtcmd_SamplingEnemy);
USER_FUNC(btlevtcmd_StartAvoid);
USER_FUNC(btlevtcmd_WaitAttackEnd);
USER_FUNC(btlevtcmd_check_battleflag);
USER_FUNC(evt_btl_camera_shake_h);

extern BattleWeaponData _nozzle_at_base_mario;
extern BattleWeaponData _nozzle_at_base_monster;
extern BattleStageNozzleData battle_stage_nozzle_data[];
extern BattleStageFallObjectData battle_stage_fall_object_data[];

BattleStageObject* BattleGetObjectPtr(s32 id) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObject* obj;
    int i;

    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id == id) {
            return obj;
        }
    }
    return NULL;
}

BattleStageObject* BattleSearchObjectPtr(const char* name) {
    BattleWork* wp = _battleWorkPointer;
    int i;
    BattleStageObject* obj;

    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id && !strcmp(obj->data->name, name)) {
            return obj;
        }
    }
    return NULL;
}

void BattleStageObjectInit(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageData* stage = wp->fbatInfo->setup->stageData;
    s32 numObjects = stage->mNumObjects;
    BattleStageObjectData* data = stage->mObjects;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
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
            mapPlayAnimationLv("den", 1, 0);
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
    mapGrpFlagOff(mapGetWork()->entries[0].rootMapObj->joints->name, 2);
    if (mapGetMapObj("A1")) {
        mapGrpFlagOn("A1", 1);
        mapGrpFlagOff("A1", 2);
    }
    if (mapGetMapObj("A2")) {
        mapGrpFlagOn("A2", 1);
        mapGrpFlagOff("A2", 2);
    }
    if (mapGetMapObj("B")) {
        mapGrpFlagOn("B", 1);
        mapGrpFlagOff("B", 2);
    }
    if (mapGetMapObj("C")) {
        mapGrpFlagOn("C", 1);
        mapGrpFlagOff("C", 2);
    }
    mapGrpFlagOn("escape_wall", 1);

    id = 1;
    if (data) {
        for (; numObjects != 0; id++, numObjects--, data++) {
            for (i = 0; i < 32; i++) {
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
            obj->mBattleUnitIdx = -1;
            switch (obj->data->mLayer) {
                case 0:
                    countA1++;
                    break;
                case 1:
                    countA2++;
                    break;
                case 2:
                    countB++;
                    break;
                case 3:
                    countC++;
                    break;
                case 6:
                    hasIronFrame = TRUE;
                    break;
            }
            switch (obj->data->mLayer) {
                case 0:
                case 1:
                case 6:
                case 7:
                    obj->mPosition = data->mPosition;
                    break;
                case 2:
                case 3:
                    obj->mPosition = data->mPosition;
                    break;
            }
            obj->mRotation.x = obj->mRotation.y = obj->mRotation.z = 0.0f;
            obj->prevPosition = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevPositionOffset = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevRotation = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevPositionOffset2 = (Vec){999.0f, 999.0f, 999.0f};
            mapGrpFlagOff(obj->data->name, 1);
            switch (data->mSpawnSpecialUnitType) {
                case 0:
                    unit = BtlUnit_Entry(&_entry_tree_data);
                    unit->stageObjectId = obj->id;
                    obj->mBattleUnitIdx = unit->unitId;
                    BtlUnit_SetPos(unit, obj->mPosition.x, obj->mPosition.y, obj->mPosition.z);
                    BtlUnit_SetHomePos(unit, obj->mPosition.x, obj->mPosition.y, obj->mPosition.z);
                    break;
                case 1:
                    unit = BtlUnit_Entry(&_entry_switch_data);
                    unit->stageObjectId = obj->id;
                    obj->mBattleUnitIdx = unit->unitId;
                    BtlUnit_SetPos(unit, obj->mPosition.x, obj->mPosition.y, obj->mPosition.z);
                    BtlUnit_SetHomePos(unit, obj->mPosition.x, obj->mPosition.y, obj->mPosition.z);
                    break;
                case 2:
                    break;
            }
        }
    }

    if (pouchGetPtr()->rank > 0) {
        data = _object_nozzle_data;
        for (j = 0; j < 3; j++, id++, data++) {
            for (i = 0; i < 32; i++) {
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
            obj->mBattleUnitIdx = -1;
            obj->mPosition = data->mPosition;
            obj->mRotation.x = obj->mRotation.y = obj->mRotation.z = 0.0f;
            obj->prevPosition = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevPositionOffset = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevRotation = (Vec){999.0f, 999.0f, 999.0f};
            obj->prevPositionOffset2 = (Vec){999.0f, 999.0f, 999.0f};
            mapGrpFlagOff(obj->data->name, 1);
        }
    }

    data = &_object_wall_data;
    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id <= 0) {
            break;
        }
    }
    obj->data = data;
    obj->id = id;
    obj->mBattleUnitIdx = -1;
    obj->mPosition = data->mPosition;
    obj->mPosition.y = BattleStageGetPtr()->stageSizeY;
    obj->mRotation.x = obj->mRotation.y = obj->mRotation.z = 0.0f;
    obj->prevPosition = (Vec){999.0f, 999.0f, 999.0f};
    obj->prevPositionOffset = (Vec){999.0f, 999.0f, 999.0f};
    obj->prevRotation = (Vec){999.0f, 999.0f, 999.0f};
    obj->prevPositionOffset2 = (Vec){999.0f, 999.0f, 999.0f};
    mapGrpFlagOn(obj->data->name, 1);
    obj->mFlags |= 1;
    id++;

    j = 0;
    while (TRUE) {
        data = &_object_spot_light_data[j];
        if (!data->name) {
            break;
        }
        for (i = 0; i < 32; i++) {
            obj = &wp->stageObjects[i];
            if (obj->id <= 0) {
                break;
            }
        }
        obj->data = data;
        obj->id = id;
        obj->mBattleUnitIdx = -1;
        obj->mPosition = data->mPosition;
        obj->mRotation.x = obj->mRotation.y = obj->mRotation.z = 0.0f;
        obj->prevPosition = (Vec){999.0f, 999.0f, 999.0f};
        obj->prevPositionOffset = (Vec){999.0f, 999.0f, 999.0f};
        obj->prevRotation = (Vec){999.0f, 999.0f, 999.0f};
        obj->prevPositionOffset2 = (Vec){999.0f, 999.0f, 999.0f};
        mapGrpFlagOn(obj->data->name, 1);
        obj->mFlags |= 1;
        id++;
        j++;
    }

    if (stage->initEvt) {
        evtEntry(stage->initEvt, 10, 0);
    }
    if (countA1 && stage->bgA1Evt) {
        STAGE_WORK(wp)->bgEvtId[0] = evtEntry(stage->bgA1Evt, 10, 0)->eventId;
    }
    if (countA2 && stage->bgA2Evt) {
        STAGE_WORK(wp)->bgEvtId[1] = evtEntry(stage->bgA2Evt, 10, 0)->eventId;
    }
    if (countB && stage->bgBEvt) {
        STAGE_WORK(wp)->bgEvtId[2] = evtEntry(stage->bgBEvt, 10, 0)->eventId;
    }
    if (countC && stage->bgCEvt) {
        STAGE_WORK(wp)->bgEvtId[3] = evtEntry(stage->bgCEvt, 10, 0)->eventId;
    }
    if (!countA1) {
        STAGE_WORK(wp)->flags |= 2;
    }
    if (!countA2) {
        STAGE_WORK(wp)->flags |= 4;
    }
    if (!countB) {
        STAGE_WORK(wp)->flags |= 8;
    }
    if (stage->disableBgA1Destroy) {
        STAGE_WORK(wp)->flags |= 2;
    }
    if (stage->disableBgA2Destroy) {
        STAGE_WORK(wp)->flags |= 4;
    }
    if (stage->disableBgBDestroy) {
        STAGE_WORK(wp)->flags |= 8;
    }
    if (!hasIronFrame) {
        STAGE_WORK(wp)->flags |= 0x20;
        mapGrpFlagOn("if_body", 1);
        mapGrpFlagOn("if_wire", 1);
    }
}

void BattleStageObjectMain(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
    BattleStageObject* obj;
    BOOL changed;
    int i;

    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id <= 0) {
            continue;
        }
        changed = FALSE;
        if (obj->mPosition.x != obj->prevPosition.x || obj->mPosition.y != obj->prevPosition.y || obj->mPosition.z != obj->prevPosition.z) {
            obj->prevPosition = obj->mPosition;
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
        if (obj->mRotation.x != obj->prevRotation.x || obj->mRotation.y != obj->prevRotation.y || obj->mRotation.z != obj->prevRotation.z) {
            obj->prevRotation = obj->mRotation;
            changed = TRUE;
        }
        if (obj->mFlags & 2) {
            obj->mFlags &= ~2;
            changed = TRUE;
        }
        switch (obj->data->mLayer) {
            case 2:
                if (sw->bgRotation.x != sw->bgRotationPrev.x ||
                    sw->bgRotation.y != sw->bgRotationPrev.y ||
                    sw->bgRotation.z != sw->bgRotationPrev.z) {
                    sw->bgRotationPrev = sw->bgRotation;
                    changed = TRUE;
                }
                break;
        }
        switch (obj->data->mLayer) {
            case 0:
            case 1:
            case 4:
            case 5:
            case 6:
            case 7:
                if (changed) {
                    mapObjTranslate(obj->data->name,
                        obj->positionOffset2.x + (obj->mPosition.x + obj->positionOffset.x),
                        obj->positionOffset2.y + (obj->mPosition.y + obj->positionOffset.y),
                        obj->positionOffset2.z + (obj->mPosition.z + obj->positionOffset.z));
                    mapObjRotate(obj->data->name, obj->mRotation.x, obj->mRotation.y, obj->mRotation.z);
                }
                break;
            case 2:
                if (changed) {
                    mapObjTranslate(obj->data->name,
                        obj->positionOffset2.x + (obj->mPosition.x + obj->positionOffset.x),
                        obj->positionOffset2.y + (obj->mPosition.y + obj->positionOffset.y),
                        obj->positionOffset2.z + (obj->mPosition.z + obj->positionOffset.z));
                    mapObjTranslate(obj->data->name, 0.0f, -obj->positionOffset.y, 0.0f);
                    mapObjRotate(obj->data->name, sw->bgRotation.x, sw->bgRotation.y, sw->bgRotation.z);
                    mapObjTranslate(obj->data->name, 0.0f, obj->positionOffset.y, 0.0f);
                }
                break;
            case 3:
                if (changed) {
                    mapObjTranslate(obj->data->name,
                        obj->positionOffset2.x + (obj->mPosition.x + obj->positionOffset.x),
                        obj->positionOffset2.y + (obj->mPosition.y + obj->positionOffset.y),
                        obj->positionOffset2.z + (obj->mPosition.z + obj->positionOffset.z));
                    mapObjTranslate(obj->data->name, 0.0f, -obj->mPosition.y, 0.0f);
                    mapObjRotate(obj->data->name, obj->mRotation.x, obj->mRotation.y, obj->mRotation.z);
                    mapObjTranslate(obj->data->name, 0.0f, obj->mPosition.y, 0.0f);
                }
                break;
        }
        if (obj->mFlags & 1) {
            if (!(mapGetMapObj(obj->data->name)->flags & 1)) {
                mapGrpFlagOn(obj->data->name, 1);
            }
        }
        else {
            if (mapGetMapObj(obj->data->name)->flags & 1) {
                mapGrpFlagOff(obj->data->name, 1);
            }
        }
    }
    BattleObjectConfig();
}

void BattleObjectConfig(void) {
    //stubbed in retail
}

void BattleStage_DestroyA1(void) {
    BattleWork* wp = _battleWorkPointer;
    void* evt = wp->fbatInfo->setup->stageData->destroyBgA1Evt;
    EventEntry* entry;

    evtDeleteID(STAGE_WORK(wp)->bgEvtId[0]);
    STAGE_WORK(wp)->bgEvtId[0] = 0;
    if (STAGE_WORK(wp)->bgDestroyEvtId && evtCheckID(STAGE_WORK(wp)->bgDestroyEvtId)) {
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

void BattleStage_DestroyA2(void) {
    BattleWork* wp = _battleWorkPointer;
    void* evt = wp->fbatInfo->setup->stageData->destroyBgA2Evt;

    evtDeleteID(STAGE_WORK(wp)->bgEvtId[1]);
    STAGE_WORK(wp)->bgEvtId[1] = 0;
    if (!evt) {
        evt = _object_turn_event_A2;
    }
    evtEntry(evt, 10, 0);
}

void BattleStage_DestroyB(void) {
    BattleWork* wp = _battleWorkPointer;
    void* evt = wp->fbatInfo->setup->stageData->destroyBgBEvt;
    EventEntry* entry;

    evtDeleteID(STAGE_WORK(wp)->bgEvtId[2]);
    STAGE_WORK(wp)->bgEvtId[2] = 0;
    if (STAGE_WORK(wp)->bgDestroyEvtId && evtCheckID(STAGE_WORK(wp)->bgDestroyEvtId)) {
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


BattleWeaponData _nozzle_at_base_mario = {{
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
    0x2000070, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
}};

BattleWeaponData _nozzle_at_base_monster = {{
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
    0x200006F, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
}};

BattleStageNozzleData battle_stage_nozzle_data[] = {
    {
#ifdef VERSION_G8ME01
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
#ifdef VERSION_G8ME01
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                2, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 100, 1, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
#ifdef VERSION_G8ME01
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                2, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 100, 1, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                3, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
    {
#ifdef VERSION_G8ME01
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                2, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 100, 1, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                3, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                1, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                100, 3, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
};

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
            0x2100060, // targetClassFlags
            0x20000000, // targetPropertyFlags
            0, 10, // element..damagePattern
            3, // acLevel
            2, // reserved_6F
            NULL, // acHelpMsg
            0, // special
            0x7FF, // counterResistFlags
            0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
            0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
            0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
            0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
            0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
            0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
            {0}, // pad_AE
            NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 1, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 100, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0x100, // special
                0x7FF, // counterResistFlags
                0x80000000, 100, 0, 100, 0, 100, 0, 100, // targetWeightFlags..poisonChance
                0, 0, 100, 0, 100, 0, 0, 0, // poisonTime..dodgyTime
                100, 0, 100, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                -99, 0, 0, 0, 0, 0, 0, -99, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 100, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 25, 3, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
            0x2100060, // targetClassFlags
            0x20000000, // targetPropertyFlags
            0, 10, // element..damagePattern
            3, // acLevel
            2, // reserved_6F
            NULL, // acHelpMsg
            0, // special
            0x7FF, // counterResistFlags
            0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
            0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
            0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
            0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
            0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
            0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
            {0}, // pad_AE
            NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 1, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 100, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0x100, // special
                0x7FF, // counterResistFlags
                0x80000000, 100, 0, 100, 0, 100, 0, 100, // targetWeightFlags..poisonChance
                0, 0, 100, 0, 100, 0, 0, 0, // poisonTime..dodgyTime
                100, 0, 100, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                -99, 0, 0, 0, 0, 0, 0, -99, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 100, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 25, 3, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
            0x2100060, // targetClassFlags
            0x20000000, // targetPropertyFlags
            0, 10, // element..damagePattern
            3, // acLevel
            2, // reserved_6F
            NULL, // acHelpMsg
            0, // special
            0x7FF, // counterResistFlags
            0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
            0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
            0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
            0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
            0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
            0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
            {0}, // pad_AE
            NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 1, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 100, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0x100, // special
                0x7FF, // counterResistFlags
                0x80000000, 100, 0, 100, 0, 100, 0, 100, // targetWeightFlags..poisonChance
                0, 0, 100, 0, 100, 0, 0, 0, // poisonTime..dodgyTime
                100, 0, 100, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                -99, 0, 0, 0, 0, 0, 0, -99, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 100, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 25, 3, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
            0x2100060, // targetClassFlags
            0x20000000, // targetPropertyFlags
            0, 10, // element..damagePattern
            3, // acLevel
            2, // reserved_6F
            NULL, // acHelpMsg
            0, // special
            0x7FF, // counterResistFlags
            0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
            0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
            0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
            0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
            0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
            0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
            {0}, // pad_AE
            NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 1, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 50, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 100, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 0, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0x100, // special
                0x7FF, // counterResistFlags
                0x80000000, 100, 0, 100, 0, 100, 0, 100, // targetWeightFlags..poisonChance
                0, 0, 100, 0, 100, 0, 0, 0, // poisonTime..dodgyTime
                100, 0, 100, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                -99, 0, 0, 0, 0, 0, 0, -99, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 100, 3, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x2100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 50, 3, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
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
                0x1100060, // targetClassFlags
                0x20000000, // targetPropertyFlags
                0, 10, // element..damagePattern
                3, // acLevel
                2, // reserved_6F
                NULL, // acHelpMsg
                0, // special
                0x7FF, // counterResistFlags
                0x80000000, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
                0, 0, 0, 0, 25, 3, 0, 0, // poisonTime..dodgyTime
                0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
                0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
                0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
                0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
                {0}, // pad_AE
                NULL, // mAttackEvtScript
                {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
            }},
        },
    },
};

BattleStageObjectData _object_nozzle_data[] = {
    {"nozzle_l", 3, 4, {-120.0f, 0.0f, 90.0f}, 0, 0},
    {"nozzle_c", 3, 4, {0.0f, 0.0f, 90.0f}, 0, 0},
    {"nozzle_r", 3, 4, {120.0f, 0.0f, 90.0f}, 0, 0},
};

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
    CALL(btlevtcmd_check_battleflag, LW(0), 256)
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(_get_bgset_attack_data, LW(9), LW(12))
    IF_INT_EQUAL(LW(12), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -5, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -5, LW(0), LW(12))
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
    CALL(btlevtcmd_PreCheckDamage, -5, LW(10), LW(11), LW(12), 256, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(4)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(10), 40)
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(10), 38)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(10), 39)
        CASE_EQUAL(1)
        CALL(btlevtcmd_CheckDamage, -5, LW(10), LW(11), LW(12), 256, LW(5))
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

EVT_BEGIN(_fall_frame_event)
    CALL(_fall_frame_wait)
    CALL(_fall_frame_wait_unit_ready)
    CALL(_fall_frame, LW(12))
    CALL(btlevtcmd_GetEnemyBelong, -5, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -5, LW(0), LW(12))
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
    CALL(btlevtcmd_CheckDamage, -5, LW(10), LW(11), LW(12), 256, LW(5))
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(10)
    LABEL(20)
    CALL(evt_btl_camera_shake_h, 0, 10, 0, 20, 13)
    CALL(_fall_frame_delete)
    RETURN
EVT_END()

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
    CALL(btlevtcmd_GetEnemyBelong, -5, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -5, LW(0), LW(12))
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
    CALL(btlevtcmd_PreCheckDamage, -5, LW(10), LW(11), LW(12), 256, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(4)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(10), 40)
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(10), 38)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(10), 39)
        CASE_EQUAL(1)
        CALL(btlevtcmd_CheckDamage, -5, LW(10), LW(11), LW(12), 256, LW(5))
        CASE_DEFAULT
    END_SWITCH
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(10)
    LABEL(19)
    CALL(_nozzle_work_get_weapon_address, 1, LW(12))
    IF_INT_EQUAL(LW(12), 0)
        GOTO(29)
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, -5, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -5, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    LABEL(20)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(29)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, -5, LW(10), LW(11), LW(12), 256, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(4)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(10), 40)
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(10), 38)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(10), 39)
        CASE_EQUAL(1)
        CALL(btlevtcmd_CheckDamage, -5, LW(10), LW(11), LW(12), 256, LW(5))
        CASE_DEFAULT
    END_SWITCH
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(20)
    LABEL(29)
    RETURN
EVT_END()

EVT_BEGIN(_object_fall_event)
    CALL(_object_fall_wait)
    CALL(_object_fall_attack, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(1000)
    END_IF
    RETURN
EVT_END()

BattleStageObjectData _object_wall_data = {"escape_wall", 4, 5, {-175.0f, 0.0f, 0.0f}, 0, 0};

BattleStageObjectData _object_spot_light_data[] = {
    {"spotlight", 6, 7, {0.0f, 240.0f, 0.0f}, 0, 0},
    {NULL, 6, 7, {0.0f, 0.0f, 0.0f}, 0, 0},
};

BattleUnitSetup _entry_tree_data = {&unit_object_tree, 2, {0}, 0x04000005, {-1000.0f, -1000.0f, -1000.0f}, 0, {0, 0, 0, 0}, NULL};

BattleUnitSetup _entry_switch_data = {&unit_object_switch, 2, {0}, 0x04000005, {-1000.0f, -1000.0f, -1000.0f}, 0, {0, 0, 0, 0}, NULL};

EVT_BEGIN(_object_turn_event_A1)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(_object_turn_event_core))
    RETURN
EVT_END()

EVT_BEGIN(_object_turn_event_A2)
    SET_VALUE(LW(9), 1)
    CALL_CHILD(PTR(_object_turn_event_core))
    RETURN
EVT_END()

EVT_BEGIN(_object_turn_event_B)
    SET_VALUE(LW(9), 2)
    CALL_CHILD(PTR(_object_turn_event_core))
    RETURN
EVT_END()

EVT_BEGIN(_nozzle_turn_event)
    CALL(_nozzle_turn)
    RETURN
EVT_END()

#ifndef VERSION_G8ME01
EVT_BEGIN(_close_wall_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(_announce_wall_message)
    RETURN
EVT_END()
#endif

#ifndef VERSION_G8ME01
EVT_BEGIN(_nozzle_work_event_0)
    SET_VALUE(LW(10), 0)
    CALL_CHILD(PTR(_nozzle_work_event_core))
    RETURN
EVT_END()

EVT_BEGIN(_nozzle_work_event_1)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(PTR(_nozzle_work_event_core))
    RETURN
EVT_END()
#endif
USER_FUNC(_set_mobj_rotate_x) {
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
    sw = STAGE_WORK(wp = _battleWorkPointer);
    layer = evtGetValue(event, *args++);
    if (isFirstCall) {
        switch (layer) {
            case 0:
            case 1:
                for (j = 0; j < 32; j++) {
                    obj = &wp->stageObjects[j];
                    if (obj->id > 0 && obj->data->mLayer == layer) {
                        obj->mNumFramesToStartFalling = obj->data->mNumFramesToStartFalling;
                        obj->mNumFramesFalling = 0;
                    }
                }
                break;
            case 2:
                event->userWork[1] = 0;
                event->userWork[2] = 60;
                event->userWork[3] = 0;
                break;
        }
    }
    switch (layer) {
        case 0:
        case 1:
            fall1 = "SFX_BTL_STAGE_SET_FALL1";
            fall3 = "SFX_BTL_STAGE_SET_FALL3";
            for (i = 0; i < 32; i++) {
                obj = &wp->stageObjects[i];
                if (obj->id > 0 && obj->data->mLayer == layer) {
                    if (obj->mNumFramesToStartFalling) {
                        obj->mNumFramesToStartFalling--;
                        busy = TRUE;
                        _mobj_shake_main(obj);
                    }
                    else {
                        if (obj->mNumFramesFalling == 0) {
                            if (layer == 0) {
                                psndSFXOn_3D(fall1, &obj->mPosition);
                            }
                            else {
                                psndSFXOn_3D(fall3, &obj->mPosition);
                            }
                        }
                        obj->mNumFramesFalling++;
                        obj->mRotation.x = intpl_sub(2, obj->mShakeAngle, 90.0f, obj->mNumFramesFalling, obj->data->mNumFramesToFall);
                        if (obj->mNumFramesFalling < obj->data->mNumFramesToFall) {
                            busy = TRUE;
                        }
                    }
                }
            }
            break;
        case 2:
            if (event->userWork[1]) {
                event->userWork[1]--;
                busy = TRUE;
            }
            else {
                if (event->userWork[3] == 0) {
                    fall2 = "SFX_BTL_STAGE_SET_FALL2";
                    psndSFXOn_3D(fall2, &(Vec){0.0f, 0.0f, -100.0f});
                }
                event->userWork[3]++;
                sw->bgRotation.x = (s32)intpl_sub(2, 0.0f, 90.0f, event->userWork[3], event->userWork[2]);
                if (event->userWork[3] < event->userWork[2]) {
                    busy = TRUE;
                }
            }
            break;
    }
    return busy ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}
/* US: these three scripts sit after _set_mobj_rotate_x so the message string pools after its Vec literal */
#ifdef VERSION_G8ME01
EVT_BEGIN(_close_wall_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_AnnounceMessage, 2, 0, -225, STRING("btl_msg_escape_wall_close2"), 180)
    RETURN
EVT_END()

EVT_BEGIN(_nozzle_work_event_0)
    SET_VALUE(LW(10), 0)
    CALL_CHILD(PTR(_nozzle_work_event_core))
    RETURN
EVT_END()

EVT_BEGIN(_nozzle_work_event_1)
    SET_VALUE(LW(10), 1)
    CALL_CHILD(PTR(_nozzle_work_event_core))
    RETURN
EVT_END()
#endif

USER_FUNC(_mobj_set_alpha) {
    EvtWord* args = event->args;
    s32 layer = evtGetValue(event, args[0]);
    u8 alpha = evtGetValue(event, args[1]);

    switch (layer) {
        case 0:
            if (mapGetMapObj("A1")) {
                mapGrpSetColor("A1", (GXColor){255, 255, 255, alpha});
            }
            break;
        case 1:
            if (mapGetMapObj("A2")) {
                mapGrpSetColor("A2", (GXColor){255, 255, 255, alpha});
            }
            break;
        case 2:
            if (mapGetMapObj("B")) {
                mapGrpSetColor("B", (GXColor){255, 255, 255, alpha});
            }
            break;
        case 3:
            if (mapGetMapObj("C")) {
                mapGrpSetColor("C", (GXColor){255, 255, 255, alpha});
            }
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_mobj_destroy) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObject* obj;
    BattleUnit* unit;
    BattleUnitPart* part;
    s32 layer;
    int i;

    layer = evtGetValue(event, event->args[0]);
    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && obj->data->mLayer == layer) {
            obj->mFlags |= 1;
            unit = BattleGetUnitPtr(wp, obj->mBattleUnitIdx);
            if (unit) {
                for (part = unit->mParts; part; part = part->mNextPart) {
                    part->attributes |= 0x10000;
                }
            }
        }
    }
    switch (layer) {
        case 0:
            STAGE_WORK(wp)->flags |= 2;
            break;
        case 1:
            STAGE_WORK(wp)->flags |= 4;
            break;
        case 2:
            STAGE_WORK(wp)->flags |= 8;
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_get_bgset_attack_data) {
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

USER_FUNC(_bgset_kemuri_effect) {
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
        args++;
        args--;
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && obj->data->mLayer == layer) {
            x = obj->mPosition.x;
            y = obj->mPosition.y;
            z = obj->mPosition.z;
            switch (layer) {
                case 0:
                    z += 30.0f;
                    effKemuri1N64Entry(3, x, y, z, 0.0f);
                    psndSFXOn_3D(sfxA1, &obj->mPosition);
                    break;
                case 1:
                    z += 15.0f;
                    effKemuri1N64Entry(0, x, y, z, 0.0f);
                    psndSFXOn_3D(sfxA2, &obj->mPosition);
                    break;
                case 2:
                    z += 30.0f;
                    effKemuri1N64Entry(3, x, y, z, 0.0f);
                    effKemuri1N64Entry(4, x, y, z, 0.0f);
                    psndSFXOn_3D(sfxB, &obj->mPosition);
                    break;
            }
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_bgset_audience_bgset_access) {
    BattleAudience_Case_HaikeiSet();
    return EVT_RETURN_DONE;
}

USER_FUNC(_set_mobj_shake_init) {
    BattleWork* wp = _battleWorkPointer;
    s32 layer;
    int i;
    BattleStageObject* obj;
    u8 position;

    layer = evtGetValue(event, event->args[0]);
    for (i = 0; i < 32; i++) {
        obj = &wp->stageObjects[i];
        if (obj->id > 0 && obj->data->mLayer == layer) {
            obj->reserved_72 = 0;
            obj->mShakePeriodLength = irand(10) + 60;
            if (irand(100) & 1) {
                position = 0;
            }
            else {
                position = obj->mShakePeriodLength / 2;
            }
            obj->mShakePeriodPosition = position;
            obj->mShakeAngle = 0.0f;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_set_mobj_shake) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObject* obj;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
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
        case 0:
        case 1:
            for (i = 0; i < 32; i++) {
                obj = &wp->stageObjects[i];
                if (obj->id > 0 && obj->data->mLayer == layer) {
                    _mobj_shake_main(obj);
                }
            }
            break;
        case 2:
            break;
    }
    return EVT_RETURN_BLOCK;
}

void _mobj_shake_main(BattleStageObject* obj) {
    obj->mShakePeriodPosition++;
    if (obj->mShakePeriodPosition >= obj->mShakePeriodLength) {
        obj->mShakePeriodPosition = 0;
    }
    obj->mShakeAngle = 5.0f * sinfd(obj->mShakePeriodPosition * 360 / obj->mShakePeriodLength);
    obj->mRotation.x = obj->mShakeAngle;
}

void BattleStage_NozzleDirChangeCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
    u8 i;

    if (sw->nozzleData) {
        for (i = 0; i < 3; i++) {
            _Nozzle_Change_Check(i, sw->nozzleData->dirRate[i]);
        }
    }
}

void _Nozzle_Change_Check(u8 index, u8* rate) {
    BattleStageObjectWork* sw = STAGE_WORK(_battleWorkPointer);
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
            sw->nozzleTurnEvtId[index] = evt->eventId;
            if (evt) {
                evt->unitId = index;
            }
        }
    }
}

USER_FUNC(_nozzle_turn) {
    BattleStageObjectWork* sw = STAGE_WORK(_battleWorkPointer);
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
        rotX = intpl_sub(2, 0.0f, -80.0f, event->userWork[2], 30);
    }
    else {
        rotX = -80.0f;
    }
    if (event->userWork[3] < 30) {
        event->userWork[3]++;
        switch (sw->nozzleDir[index]) {
            case 1:
                from = 45.0f;
                break;
            case 2:
                from = 0.0f;
                break;
            case 3:
                from = -45.0f;
                break;
            case 0:
            default:
                from = 0.0f;
                break;
        }
        switch (sw->nozzleNextDir[index]) {
            case 1:
                to = 45.0f;
                break;
            case 2:
                to = 0.0f;
                break;
            case 3:
                to = -45.0f;
                break;
            case 0:
            default:
                to = 0.0f;
                break;
        }
        rotY = intpl_sub(12, from, to, event->userWork[3], 30);
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

void BattleStage_WallCloseCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);

    if (!(sw->flags & 0x10)) {
        sw->flags |= 0x10;
        sw->closeWallEvtId = evtEntry(_close_wall_event, 10, 0)->eventId;
    }
}

#ifndef VERSION_G8ME01
USER_FUNC(_announce_wall_message) {
    if (isFirstCall) {
        event->userdata[0] = 60;
    }
    if (event->userdata[0] < 0) {
        return EVT_RETURN_DONE;
    }
    event->userdata[0]--;
    dispEntry(CAMERA_2D, 900.0f, 1, _disp_escape_wall_message, NULL);
    return EVT_RETURN_BLOCK;
}

void _disp_escape_wall_message(CameraId camera, void* param) {
    const char* msg = msgSearch("btl_msg_escape_wall_close2");
    s32 width = FontGetMessageWidth(msg);
    f32 x;
    f32 zero = 0.0f;

    x = zero - width / 2;
    windowDispGX_Waku_col(0, (GXColor){255, 255, 255, 255}, x - 20.0f, -117.0f, width + 40, 30.0f, 10.0f);
    FontDrawStart();
    FontDrawString(zero - (width / 2), -120.0f, msg);
}

#endif

void BattleStage_IronFrameFallCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);

    if (!(sw->flags & 0x40) && !(sw->flags & 0x20)) {
        sw->flags |= 0x20;
        sw->fallFrameEvtId = evtEntry(_fall_frame_event, 10, 0)->eventId;
    }
}

USER_FUNC(_fall_frame_wait) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);

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

USER_FUNC(_fall_frame_wait_unit_ready) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    EventEntry* evt;
    int i;

    if (isFirstCall) {
        for (i = 0; i < 64; i++) {
            unit = BattleGetUnitPtr(wp, i);
            if (unit && !(unit->flags & 0x80000000) && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL) && unit->ceilFallEventCode) {
                evt = evtEntry(unit->ceilFallEventCode, 10, 0);
                if (evt) {
                    evt->unitId = unit->unitId;
                }
                unit->ceilFallEventId = evt->eventId;
            }
        }
    }
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit && !(unit->flags & 0x80000000)) {
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

USER_FUNC(_fall_frame) {
    BattleWork* wp;
    s32 arg = event->args[0];
    BattleStageObject* body;
    BattleStageObjectWork* sw;

    wp = _battleWorkPointer;
    sw = STAGE_WORK(wp);
    body = BattleSearchObjectPtr("if_body");
    if (isFirstCall) {
        event->userWork[2] = 0;
        event->userWork[3] = body->mPosition.y;
    }
    event->userWork[2]++;
    body->mPosition.y = intpl_sub(2, event->userWork[3], 0.0f, event->userWork[2] * event->userWork[2] / 60, 60);
    if (event->userWork[2] < 60) {
        return EVT_RETURN_BLOCK;
    }
    evtSetValue(event, arg, (EvtWord)sw->fallObjectData);
    return EVT_RETURN_DONE;
}

USER_FUNC(_fall_frame_delete) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);

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

void BattleStage_NozzleWorkCheck(s32 type) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
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
                }
                else {
                    evt = evtEntry(_nozzle_work_event_0, 10, 0);
                }
                sw->nozzleWorkEvtId = evt->eventId;
            }
        }
    }
}

void _nozzle_type_init(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
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

USER_FUNC(_nozzle_work_wait) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
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

f32 _get_nozzle_dir(s32 index) {
    switch (STAGE_WORK(_battleWorkPointer)->nozzleDir[index]) {
        case 1:
            return -45.0f;
        case 3:
            return -135.0f;
        case 2:
        default:
            return -90.0f;
    }
}

USER_FUNC(_nozzle_work_effect) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
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
            case 1:
                sfx = "SFX_BTL_STAGE_SMOKE1";
                effType = 0;
                break;
            case 2:
                sfx = NULL;
                effType = 1;
                break;
            case 3:
                sfx = "SFX_BTL_STAGE_FIRE1";
                effType = 2;
                break;
        }
        switch (sw->nozzleType) {
            case 1:
            case 2:
            case 3:
                obj = BattleSearchObjectPtr("nozzle_l");
                angle1 = _get_nozzle_dir(0);
                if (sw->nozzleDir[0]) {
                    if (sfx) {
                        event->userWork[5] = psndSFXOn_3D(sfx, &obj->mPosition);
                    }
                    else {
                        event->userWork[5] = -1;
                    }
                    rot = 90.0f + angle1;
                    rot = reviseAngle(rot);
                    z = 10.0f * sinfd(angle1) + obj->mPosition.z;
                    event->userWork[1] = (EvtWord)effNozleEntry(effType, obj->mPosition.x - 10.0f * cosfd(angle1), 12.0f + obj->mPosition.y, z, -80.0f, rot, 90);
                }
                else {
                    if (sfx) {
                        event->userWork[5] = psndSFXOn_3D(sfx, &obj->mPosition);
                    }
                    else {
                        event->userWork[5] = -1;
                    }
                    event->userWork[1] = (EvtWord)effNozleEntry(effType, obj->mPosition.x, 20.0f + obj->mPosition.y, 8.0f + obj->mPosition.z, 0.0f, 0.0f, 90);
                    if (fog == 0) {
                        effConfettiN64Entry(4, 90, obj->mPosition.x, 20.0f + obj->mPosition.y, 8.0f + obj->mPosition.z, 1.0f);
                    }
                }
                obj = BattleSearchObjectPtr("nozzle_c");
                angle2 = _get_nozzle_dir(1);
                if (sw->nozzleDir[1]) {
                    if (sfx) {
                        event->userWork[6] = psndSFXOn_3D(sfx, &obj->mPosition);
                    }
                    else {
                        event->userWork[6] = -1;
                    }
                    rot = reviseAngle(90.0f + angle2);
                    z = 10.0f * sinfd(angle2) + obj->mPosition.z;
                    event->userWork[2] = (EvtWord)effNozleEntry(effType, obj->mPosition.x - 10.0f * cosfd(angle2), 12.0f + obj->mPosition.y, z, -80.0f, rot, 90);
                }
                else {
                    if (sfx) {
                        event->userWork[6] = psndSFXOn_3D(sfx, &obj->mPosition);
                    }
                    else {
                        event->userWork[6] = -1;
                    }
                    event->userWork[2] = (EvtWord)effNozleEntry(effType, obj->mPosition.x, 20.0f + obj->mPosition.y, 8.0f + obj->mPosition.z, 0.0f, 0.0f, 90);
                    if (fog == 0) {
                        effConfettiN64Entry(4, 90, obj->mPosition.x, 20.0f + obj->mPosition.y, 8.0f + obj->mPosition.z, 1.0f);
                    }
                }
                obj = BattleSearchObjectPtr("nozzle_r");
                angle3 = _get_nozzle_dir(2);
                if (sw->nozzleDir[2]) {
                    if (sfx) {
                        event->userWork[7] = psndSFXOn_3D(sfx, &obj->mPosition);
                    }
                    else {
                        event->userWork[7] = -1;
                    }
                    z = reviseAngle(90.0f + angle3);
                    rot = 10.0f * sinfd(angle3) + obj->mPosition.z;
                    event->userWork[3] = (EvtWord)effNozleEntry(effType, obj->mPosition.x - 10.0f * cosfd(angle3), 12.0f + obj->mPosition.y, rot, -80.0f, z, 90);
                }
                else {
                    if (sfx) {
                        event->userWork[7] = psndSFXOn_3D(sfx, &obj->mPosition);
                    }
                    else {
                        event->userWork[7] = -1;
                    }
                    event->userWork[3] = (EvtWord)effNozleEntry(effType, obj->mPosition.x, 20.0f + obj->mPosition.y, 8.0f + obj->mPosition.z, 0.0f, 0.0f, 90);
                    if (fog == 0) {
                        effConfettiN64Entry(4, 90, obj->mPosition.x, 20.0f + obj->mPosition.y, 8.0f + obj->mPosition.z, 1.0f);
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
                    event->userWork[5] = psndSFXOn_3D("SFX_BTL_STAGE_DRYICE1", &BattleSearchObjectPtr("nozzle_c")->mPosition);
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

BOOL BattleFogEndCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);

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

BOOL BattleFogForceStop(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);

    if (sw->fogTimer == 0) {
        return FALSE;
    }
    if (sw->fogEffect) {
        effSoftDelete(sw->fogEffect);
        sw->fogEffect = NULL;
    }
    return TRUE;
}

USER_FUNC(_nozzle_work_attack_set) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
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

s32 _get_nozzle_attack_index(u8 index) {
    BattleStageObjectWork* sw = STAGE_WORK(_battleWorkPointer);

    switch (index) {
        case 0:
            switch (sw->nozzleDir[index]) {
                case 1:
                    return -1;
                case 2:
                    return 0;
                case 3:
                    return 1;
            }
        case 2:
            switch (sw->nozzleDir[index]) {
                case 1:
                    return 0;
                case 2:
                    return 1;
                case 3:
                    return -1;
            }
        case 1:
            switch (sw->nozzleDir[index]) {
                case 1:
                    return 0;
                case 2:
                    return -1;
                case 3:
                    return 1;
            }
    }
    return -1;
}

void _rate_mix(s8* dstChance, s8* dstTime, s8* srcChance, s8* srcTime) {
    if (*srcChance <= 0) {
        return;
    }
    if (*srcChance > *dstChance) {
        *dstChance = *srcChance;
    }
    *dstTime += *srcTime;
}

void _rate_mix_3(s8* dstChance, s8* dstTime, s8* dstStrength, s8* srcChance, s8* srcTime, s8* srcStrength) {
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

void _combine_nozzle_weapon(BOOL copy, BattleWeapon* dst, BattleWeapon* src) {
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
    _rate_mix(&d[0x0], &d[0x1], &s[0x0], &s[0x1]);
    _rate_mix(&d[0x2], &d[0x3], &s[0x2], &s[0x3]);
    _rate_mix(&d[0x4], &d[0x5], &s[0x4], &s[0x5]);
    _rate_mix(&d[0x6], &d[0x7], &s[0x6], &s[0x8]);
    _rate_mix(&d[0x9], &d[0xA], &s[0x9], &s[0xA]);
    _rate_mix(&d[0xB], &d[0xC], &s[0xB], &s[0xC]);
    _rate_mix(&d[0xD], &d[0xE], &s[0xD], &s[0xE]);
    _rate_mix(&d[0xF], &d[0x10], &s[0xF], &s[0x10]);
    _rate_mix(&d[0x11], &d[0x12], &s[0x11], &s[0x12]);
    _rate_mix_3(&d[0x13], &d[0x14], &d[0x15], &s[0x13], &s[0x14], &s[0x15]);
    _rate_mix_3(&d[0x19], &d[0x1A], &d[0x1B], &s[0x19], &s[0x1A], &s[0x1B]);
    _rate_mix(&d[0x1C], &d[0x1D], &s[0x1C], &s[0x1D]);
    d[0x1F] += s[0x1F];
    _rate_mix(&d[0x20], &d[0x21], &s[0x20], &s[0x21]);
    _rate_mix(&d[0x22], &d[0x23], &s[0x22], &s[0x23]);
    d[0x2A] += s[0x2A];
    d[0x2B] += s[0x2B];
    d[0x2C] += s[0x2C];
    d[0x2D] += s[0x2D];
}

USER_FUNC(_nozzle_work_get_weapon_address) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], (EvtWord)sw->nozzleWeapons[index]);
    return EVT_RETURN_DONE;
}

s32 BattleStage_ObjectFallCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);
    EventEntry* evt;

    if (wp->flags & 0x1000) {
        return 0;
    }
    evt = evtEntry(_object_fall_event, 10, 0);
    sw->objectFallEvtId = evt->eventId;
    return evt->eventId;
}

USER_FUNC(_object_fall_wait) {
    BattleWork* wp = _battleWorkPointer;
    BattleStageObjectWork* sw = STAGE_WORK(wp);

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
    s8 index; //0x0
    s8 type; //0x1
    u8 field_0x2[0x4 - 0x2]; //0x2
    s32 kind; //0x4, 0 = unit, 1 = audience
    s32 unitId; //0x8
    s32 partId; //0xC
    s16 delay; //0x10
    s16 field_0x12; //0x12
    u8 noAc; //0x14
    u8 field_0x15; //0x15
    u8 field_0x16[0x1C - 0x16]; //0x16
    s32 preCheck; //0x1C
    s32 sfxId; //0x20
    EffectEntry* effect; //0x24
    BattleWeaponData weapon; //0x28
} FallObjectEntry;

typedef struct FallObjectWork {
    u8 count; //0x0
    s8 prevTypes[3]; //0x1
    FallObjectEntry entries[1]; //0x4
} FallObjectWork;
s32 _object_fall_attack(EventEntry *event, BOOL isFirstCall);
s32 BattleStage_ObjectFallCheck(void);
void _combine_nozzle_weapon(BOOL copy, BattleWeapon* dst, BattleWeapon* src);
void _rate_mix_3(s8* dstChance, s8* dstTime, s8* dstStrength, s8* srcChance, s8* srcTime, s8* srcStrength);
void _rate_mix(s8* dstChance, s8* dstTime, s8* srcChance, s8* srcTime);
s32 _get_nozzle_attack_index(u8 index);
BOOL BattleFogEndCheck(void);
f32 _get_nozzle_dir(s32 index);
void _nozzle_type_init(void);
void BattleStage_NozzleWorkCheck(s32 type);
void BattleStage_IronFrameFallCheck(void);
void _disp_escape_wall_message(CameraId camera, void* param);
void BattleStage_WallCloseCheck(void);
void _Nozzle_Change_Check(u8 index, u8* rate);
void BattleStage_NozzleDirChangeCheck(void);
void _mobj_shake_main(BattleStageObject* obj);
void BattleStage_DestroyB(void);
void BattleStage_DestroyA2(void);
void BattleStage_DestroyA1(void);
void BattleObjectConfig(void);
USER_FUNC(_object_fall_wait);
USER_FUNC(_nozzle_work_get_weapon_address);
USER_FUNC(_nozzle_work_attack_set);
USER_FUNC(_nozzle_work_effect);
USER_FUNC(_nozzle_work_wait);
USER_FUNC(_fall_frame_delete);
USER_FUNC(_fall_frame);
USER_FUNC(_fall_frame_wait_unit_ready);
USER_FUNC(_fall_frame_wait);
#ifdef VERSION_G8ME01
USER_FUNC(btlevtcmd_AnnounceMessage);
#else
USER_FUNC(_announce_wall_message);
#endif
USER_FUNC(_nozzle_turn);
USER_FUNC(_set_mobj_shake);
USER_FUNC(_set_mobj_shake_init);
USER_FUNC(_bgset_audience_bgset_access);
USER_FUNC(_bgset_kemuri_effect);
USER_FUNC(_get_bgset_attack_data);
USER_FUNC(_mobj_destroy);
USER_FUNC(_mobj_set_alpha);
USER_FUNC(_set_mobj_rotate_x);

s32 _object_fall_attack(EventEntry *event, BOOL isFirstCall)
{
  BattleWork *wp = _battleWorkPointer;
  BattleStageFallObjectData *fd = wp->stageObjectWork.fallObjectData;
  const char *sfxNormal;
  BOOL done;
  s32 acCount;
  EvtWord *args = event->args;
  s32 outArg = *(args++);
  FallObjectEntry *e3;
  BattleUnit *unit;
  BattleUnitPart *part;
  s16 delay;
  BattleWeapon *weapon3;
  s32 j;
  BattleWeapon *weapon;
  BOOL found = 0;
  s32 k2;
  FallObjectWork *w;
  FallObjectEntry *e;
  FallObjectEntry *o;
  s32 k;
  const char *sfxTub2;
  FallObjectEntry *o2;
  const char *sfxRest;
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
  if (isFirstCall)
  {
    w = BattleAlloc(sizeof(FallObjectWork));
    event->userdatap[0] = w;
    event->userdata[1] = 0;
    memset(w, 0, sizeof(FallObjectWork));
    w->entries[0].index = -1;
    for (i = 0; i < 3; i++)
    {
      w->prevTypes[i] = -1;
    }

    if (BattleCheckConcluded(wp))
    {
      evtSetValue(event, outArg, event->userdata[1]);
      return 2;
    }
    w->count = 1;
    for (i = 0; i < w->count; i++)
    {
      e = &w->entries[i];
      e->index = i;
      for (j = 0; j < 3; j++)
      {
        if (w->prevTypes[j] == (-1))
        {
          break;
        }
      }

      if (j >= 3)
      {
        e->type = w->prevTypes[irand(3)];
      }
      else
      {
        sum = 0;
        for (k = 0; k < 9; k++)
        {
          sum += fd->rates[k];
        }

        r = irand(sum);
        for (e->type = 0; e->type < 8; e->type++)
        {
          if ((r -= fd->rates[e->type]) < 0)
          {
            break;
          }
        }

        w->prevTypes[j] = e->type;
      }
      e->weapon = fd->weapons[e->type];
      if (e->type == 6)
      {
        if (irand(100) < 50)
        {
          e->weapon.weapon.targetClassFlags |= 0x100;
          e->weapon.weapon.targetClassFlags &= ~0x200;
        }
        else
        {
          e->weapon.weapon.targetClassFlags &= ~0x100;
          e->weapon.weapon.targetClassFlags |= 0x200;
        }
      }
      weapon = &e->weapon.weapon;
      e->kind = 0;
      if (((pouchGetPtr()->mStarPowersObtained && (!(BattleAudienceBaseGetPtr()->flags & 0x4000))) && (e->type != 6)) && (e->type != 7))
      {
        if (((u32)wp->debug.flags & 2) || (irand(100) < 10))
        {
          e->kind = 1;
          e->partId = -1;
          e->unitId = BattleAudience_HaitiRandForFallObject();
          found = 1;
          event->userdata[1] = 1;
        }
      }
      if (e->kind == 0)
      {
        BattleSamplingEnemy(&targets, weapon, BattleGetSystemPtr(wp)->unitId, 1, weapon->targetClassFlags, weapon->targetPropertyFlags, -wp->alliances[1].attackDirection);
        if (targets.numTargets <= 0)
        {
          evtSetValue(event, outArg, event->userdata[1]);
          return 2;
        }
        found = 1;
        BattleChoiceSamplingEnemy(&targets, weapon->targetWeightFlags, &unitId, &partId);
        e->unitId = unitId;
        e->partId = partId;
        e->noAc = 1;
        for (k = i; k >= 0; k--)
        {
          o = &w->entries[k];
          if (((e != o) && (o->kind == 0)) && (e->unitId == o->unitId))
          {
            o->noAc = 0;
          }
        }

      }
    }

    if (!found)
    {
      evtSetValue(event, outArg, event->userdata[1]);
      return 2;
    }
    for (i = 0; i < w->count; i++)
    {
      e = &w->entries[i];
      weapon = &e->weapon.weapon;
      if (e->kind == 0)
      {
        flags = 0;
        if (e->noAc)
        {
          flags |= 0x100;
        }
        if (e->field_0x15)
        {
          flags |= 0x100000;
        }
        e->preCheck = BattlePreCheckDamage(BattleGetSystemPtr(wp), BattleGetUnitPtr(wp, e->unitId), BattleGetUnitPartsPtr(e->unitId, e->partId), weapon, flags);
        if ((e->preCheck == 1) && (!e->noAc))
        {
          delay = irand(0);
          for (k2 = 0; k2 < i; k2++)
          {
            o2 = &w->entries[k2];
            if (e->unitId == o2->unitId)
            {
              o2->delay = delay;
              delay += irand(22) + 20;
              if (o2->noAc)
              {
                o2->field_0x15 = 1;
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
  done = 1;
  for (i = 0; i < w->count; i++)
  {
    e3 = &w->entries[i];
    if (e3->index == (-1))
    {
      continue;
    }
    acCount = 0;
    done = 0;
    if (e3->kind == 0)
    {
      unit = BattleGetUnitPtr(wp, e3->unitId);
      part = BattleGetUnitPartsPtr(e3->unitId, e3->partId);
    }
    if ((e3->effect == ((void *) 0)) || (e3->delay > 0))
    {
      e3->delay--;
      if (e3->delay > 0)
      {
        continue;
      }
      switch (e3->kind)
      {
        case 0:
          BtlUnit_GetHitPos(unit, part, &hit.x, &hit.y, &hit.z);
          width = BtlUnit_GetWidth(unit);
          break;

        case 1:
          hit.x = ((e3->unitId % 20) * 25) - 230;
          hit.y = BattleStageGetPtr()->audienceSizeY;
          hit.z = (0.1f * (e3->unitId % 20)) + (((e3->unitId / 20) * 30) + 115);
          btl_camera_set_mode(0, 6);
          btl_camera_set_moveSpeedLv(0, 2);
          width = 0.0f;
          break;

        default:
          hit = (Vec){0.0f, 0.0f, 0.0f};
          width = 0.0f;
          break;

      }

      pos = hit;
      pos.y = BattleGetFloorHeight(wp, pos.x, pos.y, pos.z);
      if (hit.x >= 0.0f)
      {
        dir = 90.0f;
      }
      else
      {
        dir = 270.0f;
      }
      switch (e3->preCheck)
      {
        case 2:

        case 3:

        case 4:

        case 6:
          hit.y = pos.y;
          break;

      }

      e3->sfxId = -1;
      e3->field_0x12 = 0;
      switch (e3->type)
      {
        case 0:
          e3->effect = effFallEntry(4, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
          break;

        case 1:
          e3->effect = effFallEntry(5, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
          break;

        case 2:
          e3->effect = effFallEntry(0, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 100, 90);
          break;

        case 3:
          e3->effect = effFallEntry(1, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
          break;

        case 4:
          e3->effect = effFallEntry(2, pos.x, pos.y, 5.0f + pos.z, width, hit.y - pos.y, dir, 1, 90);
          break;

        case 8:
          e3->effect = effFallEntry(8, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
          break;

        case 5:
          e3->effect = effFallEntry(3, pos.x, pos.y, pos.z, width, hit.y - pos.y, dir, 1, 90);
          break;

        case 6:
          if (BtlUnit_GetBelong(unit) == 0)
        {
          e3->effect = effFallEntry(6, -300.0f, pos.y, 15.0f + pos.z, 0.0f, 0.0f, dir, 1, 180);
        }
        else
        {
          e3->effect = effFallEntry(6, 300.0f, pos.y, 15.0f + pos.z, 0.0f, 0.0f, dir, 1, 180);
        }
          break;

        case 7:
          e3->effect = effFallEntry(7, pos.x, pos.y, 5.0f + pos.z, width, hit.y - pos.y, dir, 1, 180);
          break;

      }

    }
    else
    {
      if ((e3->effect != ((void *) 0)) && (!effFallCheckHit(e3->effect)))
      {
        continue;
      }
      weapon3 = &e3->weapon.weapon;
      if (e3->kind == 1)
      {
        BattleAudience_Case_FallObject_Aud(e3->unitId, e3->type);
        psndSFXOff(e3->sfxId);
        BattleAudience_GetHomePosition(e3->unitId, &sfxPos.x, &sfxPos.y, &sfxPos.z);
        switch (e3->type)
        {
          case 5:
            break;

          case 0:

          case 1:
            psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_DAMAGE_TUB1"), &sfxPos); // FAKE: integer cast makes the literal a hoisted @ temp (same pointer)
            break;

          default:
            psndSFXOn_3D(sfxNormal, &sfxPos);
            break;

        }

      }
      else
      {
        BattleAudience_Case_FallObject_Stage();
        if (weapon3->targetClassFlags & 0x1000000)
        {
          BtlUnit_GetHitPos(unit, part, &sfxPos.x, &sfxPos.y, &sfxPos.z);
          flags = 0;
          if (e3->noAc)
          {
            flags |= 0x100;
          }
          psndSFXOff(e3->sfxId);
          switch (e3->preCheck)
          {
            case 2:

            case 3:

            case 4:

            case 6:
              switch (e3->type)
            {
              case 5:
                break;

              case 0:

              case 1:
                psndSFXOn_3D(sfxTub2, &sfxPos);
                break;

            }

              break;

            case 1:
              switch (e3->type)
            {
              case 0:

              case 1:
                psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_DAMAGE_TUB1"), &sfxPos); // FAKE: integer cast makes the literal a hoisted @ temp (same pointer)
                break;

              case 5:
                psndSFXOn_3D(sfxRest, &sfxPos);
                break;

              default:
                psndSFXOn_3D(sfxNormal, &sfxPos);
                break;

            }

              if (BtlUnit_GetACPossibility(unit))
            {
              BattleActionCommandCheckDefence(unit, weapon3);
            }
              BattleCheckDamage(BattleGetSystemPtr(wp), unit, part, weapon3, flags);
              break;

          }

        }
        else
        {
          psndSFXOff(e3->sfxId);
          BattleSamplingEnemy(&targets2, weapon3, BattleGetSystemPtr(wp)->unitId, 1, weapon3->targetClassFlags, weapon3->targetPropertyFlags, -wp->alliances[1].attackDirection);
          if (targets2.numTargets > 0)
          {
            for (;;)
            {
              BattleWorkTarget *tg;
              BattleUnit *unit2;
              if (targets2.currentTarget >= targets2.numTargets)
              {
                break;
              }
              tg = &targets2.targets[targets2.targetIndices[targets2.currentTarget]];
              unit2 = BattleGetUnitPtr(wp, tg->unitIdx);
              switch (BattlePreCheckDamage(BattleGetSystemPtr(wp), unit2, BattleGetUnitPartsPtr(tg->unitIdx, tg->partIdx), weapon3, 0x100))
              {
                case 2:

                case 4:
                  break;

                case 3:
                {
                  EffectEntry *eff;
                  BtlUnit_GetPos(unit2, &p.x, &p.y, &p.z);
                  if (p.y < 85.0f)
                  {
                    p.y += 50.0f;
                  }
                  else
                  {
                    p.y += -50.0f;
                  }
                  p.x -= 45.0f;
                  eff = effNiceEntry(6, p.x, p.y, p.z);
                  ((NiceEffectWork*)eff->userdata)->overallScale = 0.75f;
                  unit2->damagePattern = 0;
                  BattleRunHitEvent(unit2, 0x126);
                  break;
                }

                case 6:
                  unit2->damagePattern = 0;
                  BattleRunHitEvent(unit2, 0x127);
                  break;

                case 1:
                  if (BtlUnit_GetACPossibility(unit2) && (acCount <= 0))
                {
                  BattleActionCommandCheckDefence(unit2, weapon3);
                  acCount++;
                }
                  BattleCheckDamage(BattleGetSystemPtr(wp), unit2, BattleGetUnitPartsPtr(tg->unitIdx, tg->partIdx), weapon3, 0x100);
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

  if (!done)
  {
    return 0;
  }
  BattleFree(event->userdatap[0]);
  evtSetValue(event, outArg, event->userdata[1]);
  return 2;
}

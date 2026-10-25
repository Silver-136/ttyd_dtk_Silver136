#include "battle/battle.h"
#include "battle/battle_stage_object.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"

USER_FUNC(_rumble_tree);
USER_FUNC(_turn_tree);

static BattleUnitKindPart parts[1];
static EvtWord init_event[25];
static EvtWord damage_event[14];
static s32 _data[35];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[1];
static EvtWord attack_event[5];
static EvtWord wait_event[2];

BattleUnitKind unit_object_tree = {
    216,
    "btl_un_tree",
    999,
    0,
    1,
    1,
    2,
    0,
    0,
    0,
    0,
    100,
    9,
    24,
    24,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    0.0f,
    12.0f,
    15.6f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BTL_DAMAGE1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x12003001,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_tree", "b_mario", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x3000009, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetMaxMoveCount, -2, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(_rumble_tree, -2)
    CALL(_turn_tree, -2)
    RETURN
EVT_END()

static s32 _data[] = {
    -5, 0, 15, 0, -15, 0, 14, 0,
    -13, 0, 12, 0, -11, 0, 10, 0,
    -9, 0, 8, 0, -7, 0, 6, 0,
    -6, 0, 5, 0, -4, 0, 2, 0,
    -2, 0, 0,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "M_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "M_S_1"},
    {POSE_TYPE_DAMAGE, "M_O_1"},
    {POSE_TYPE_ACTION, "M_S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    RETURN
EVT_END()

USER_FUNC(_rumble_tree) {
    BattleUnit* unit;
    BattleStageObject* object;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    object = BattleGetObjectPtr(unit->stageObjectId);
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    object->mPosition.y += _data[event->userdata[0]];
    event->userdata[0]++;
    if ((u32)event->userdata[0] >= 34) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(_turn_tree) {
    BattleUnit* unit;
    BattleStageObject* object;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    object = BattleGetObjectPtr(unit->stageObjectId);
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    event->userdata[0] += 3;
    if (event->userdata[0] > 90) {
        object->mRotation.x = 0.0f;
        return EVT_RETURN_DONE;
    }
    object->mRotation.x = event->userdata[0];
    return EVT_RETURN_BLOCK;
}

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_cmd.h"


BattleUnitKind unit_act_atmic_teresa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[7];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[9];
static EvtWord damage_event[2];
static EvtWord phase_event[2];
static EvtWord attack_event[2];
static EvtWord wait_event[6];

BattleUnitKind unit_act_atmic_teresa = {
    UNIT_EPILOGUE_ATOMIC_BOO,
    "btl_un_act_atmic_teresa",
    10,
    0,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    100,
    9,
    28,
    24,
    5,
    25,
    0.0f,
    14.0f,
    0.0f,
    0,
    0,
    0.0f,
    30.0f,
    0.0f,
    14.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    0.0f,
    14.0f,
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
    0,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_teresa", "c_atmic_trs", 0.0f, 0.0f, 0.0f, 0.0f, 95.0f, 0.0f, 0.0f, 95.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()


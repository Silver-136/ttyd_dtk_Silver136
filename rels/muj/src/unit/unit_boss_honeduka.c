#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"


BattleUnitKind unit_boss_honeduka;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static EvtWord init_event[21];
static EvtWord damage_event[10];
EvtWord honeduka_damage_sub_event[14];
static EvtWord wait_event[6];

BattleUnitKind unit_boss_honeduka = {
    94,
    "btl_un_honeduka",
    100,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    9,
    100,
    100,
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
    50.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    32.0f,
    0.0f,
    0.0f,
    50.0f,
    65.0f,
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
    "SFX_BOSS_KOR_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x52200000,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 80, 0, 100, 0, 100, 100, 50, 0, 0, 100, 0, 100, 40, 50, 0, 0, 100, 50, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {28, "KRT_Z_1"},
    {31, "KRT_Z_1"},
    {39, "KRT_Z_1"},
    {69, "KRT_Z_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, honeduka_damage_sub_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_honeduka", "c_honezuka", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x2090009, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, -2, 16777216)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetMaxMoveCount, -2, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

EVT_BEGIN(honeduka_damage_sub_event)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
    CALL(btlevtcmd_RunDataEventChild, LW(3), UNIT_DATA_WEAPON_BREAK_EVENT)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()


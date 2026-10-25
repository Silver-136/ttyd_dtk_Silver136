#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_cmd.h"


static BattleUnitKindPart parts[1];
BattleUnitKind unit_system;
static EvtWord init_event[21];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleDataEntry data_table[1];
static StatusVulnerability regist;
static EvtWord damage_event[2];
static EvtWord attack_event[2];
static EvtWord wait_event[2];

static BattleUnitKindPart parts[] = {
    {1, "btl_un_system", NULL, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kHidden | kNoShadow, 0, NULL},
};

BattleUnitKind unit_system = {
    UNIT_SYSTEM,
    "btl_un_system",
    999,
    0,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    9,
    24,
    24,
    0,
    20,
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
    kNoGravityBob | kActsInZeroGravity | kUnitNoHpGauge | kUnitNoncombatant | kUnitHpFixed,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetMaxMoveCount, -2, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{2, 2, 2, 2, 2}};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static EVT_BEGIN(damage_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    RETURN
EVT_END()


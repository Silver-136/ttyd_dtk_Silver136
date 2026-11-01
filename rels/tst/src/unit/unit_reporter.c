// reporter: a placeholder (unit kind 213) with the Mario model that only waits.
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_sub.h"

BattleUnitKind unit_reporter;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[7];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[15];
static EvtWord wait_event[7];

/**
 * Unit definition of the placeholder unit `reporter`:
 * 10 HP and level 1, with its sizes, damage sounds, status vulnerability, parts and init script.
 */
BattleUnitKind unit_reporter = {
    213,
    "btl_un_reporter",
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

/** Defence of the placeholder unit `reporter`: none, every hit does its full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of the placeholder unit `reporter`: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Status vulnerability of the placeholder unit `reporter`: immune to every status. */
static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * Pose table of the placeholder unit `reporter`:
 * maps each battle pose (waiting, hurt, running...) to one of its EM_* animations.
 */
static PoseTableEntry pose_table[] = {
    {28, "EM_S_1"},
    {31, "KNP_S_1"},
    {39, "EM_D_1"},
    {42, "EM_R_1"},
    {40, "EM_W_1"},
    {POSE_TYPE_TALK, "EM_T_1"},
    {POSE_TYPE_ACTION, "EM_S_1"},
};

/** Data table of the placeholder unit `reporter`: it has no special reaction scripts. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Body part of the placeholder unit `reporter`: a single part using the c_mario model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_teresa", "c_mario", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/** Battle init script of the placeholder unit: installs only the wait handler and starts the wait event. */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
EVT_END()

/**
 * Wait (idle) event: refreshes the body animation (part 1) from the unit's pose table
 * so it matches the current state.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END_PAD()

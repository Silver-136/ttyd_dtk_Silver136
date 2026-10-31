// nancy: a placeholder (unit kind 211) with the Mario model that only waits.
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_sub.h"

BattleUnitKind unit_nancy;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[7];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[15];
static EvtWord wait_event[7];

/**
 * Unit definition of the `nancy` placeholder unit: 10 HP, level 1, 1 part, with its sounds, init script and
 * data table.
 */
BattleUnitKind unit_nancy = {
    211,
    "btl_un_nancy",
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

/** Defence of the `nancy` placeholder unit: none, so it takes full damage from every kind of attack. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Damage-type modifiers of the `nancy` placeholder unit: none, no special weakness or immunity. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/**
 * Status vulnerability of the `nancy` placeholder unit (100% is the normal chance): immune to Sleep, Stop,
 * Dizzy, Poison, Confusion, Electrified, Burn, Freeze, Huge, Tiny, Attack Up, Attack Down, Defense Up,
 * Defense Down, Allergic, Fright, Gale Force, Fast, Slow, Dodgy, Invisible and instant KO.
 */
static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * Pose table of the `nancy` placeholder unit: its 7 battle poses (status, hurt and action) and the animation
 * each one plays; the idle animation is `EM_S_1`.
 */
static PoseTableEntry pose_table[] = {
    {28, "EM_S_1"},
    {31, "KNP_S_1"},
    {39, "EM_D_1"},
    {42, "EM_R_1"},
    {40, "EM_W_1"},
    {BTL_POSE_TALK, "EM_T_1"},
    {BTL_POSE_STAY_DEFAULT, "EM_S_1"},
};

/** Data table of the `nancy` placeholder unit: no events. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/**
 * Body of the `nancy` placeholder unit: one targetable part, the model `c_mario`, with its hit box, defence
 * tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_teresa", "c_mario", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
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

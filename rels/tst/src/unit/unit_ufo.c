// ufo (unit kind 208): a test UFO unit carrying a dropped coin (おとしもの, part 2); it never attacks.
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_sub.h"
#include "battle/battle_ufo.h"

BattleUnitKind unit_ufo;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[5];
static PoseTableEntry pose_table_2[4];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[2];
static EvtWord init_event[20];
static EvtWord main_event[4];
static EvtWord damage_event[56];
static EvtWord attack_event[2];
static EvtWord wait_event[7];

/**
 * Unit definition of the test UFO unit: 120 HP and level 1, with its sizes, damage sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_ufo = {
    208,
    "btl_un_ufo",
    120,
    0,
    1,
    1,
    1,
    0,
    0,
    100,
    0,
    100,
    9,
    24,
    24,
    24,
    24,
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
    0,
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
    kUnitLeader | kNoTurn | kUnitNoncombatant,
    &regist,
    2,
    parts,
    init_event,
    data_table,
};

/** Defence of the test UFO unit: none, every hit does its full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of the test UFO unit: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Status vulnerability of the test UFO unit: immune to the 22 statuses covered by this table. */
static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/** Pose table of the UFO itself (part 1): maps the battle poses to its CAT_* animations. */
static PoseTableEntry pose_table[] = {
    {BTL_POSE_NORMAL, "CAT_S_1"},
    {31, "CAT_S_1"},
    {BTL_POSE_DAMAGE, "CAT_K_1"},
    {POSE_TYPE_WALK, "CAT_W_1"},
    {POSE_TYPE_ACTION, "CAT_S_1"},
};

/** Pose table of the coin the UFO carries (part 2): every pose shows BRT_O_1. */
static PoseTableEntry pose_table_2[] = {
    {BTL_POSE_NORMAL, "BRT_O_1"},
    {31, "BRT_O_1"},
    {BTL_POSE_DAMAGE, "BRT_O_1"},
    {POSE_TYPE_ACTION, "BRT_O_1"},
};

/** Data table of the test UFO unit: it has no special reaction scripts. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/**
 * Parts of the test UFO unit: the UFO itself (part 1, model ufo)
 * and the coin it carries as a dropped item (part 2, model coin).
 */
static BattleUnitKindPart parts[] = {
    {1, "bll_un_ufo", "ufo", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 65.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table},
    {2, "おとしもの", "coin", 0.0f, -15.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table_2},
};

/**
 * Init script of the test UFO: registers its wait,
 * attack (empty) and damage scripts and the default confusion script and starts main_event alongside.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    SPAWN_EVENT(main_event)
    RETURN
EVT_END()

/** Runs the UFO's whole fly-in, coin drop and fly-off sequence (btlevtcmd_UFOAction) until the UFO deletes itself. */
static EVT_BEGIN(main_event)
    CALL(btlevtcmd_UFOAction)
    RETURN
EVT_END()

/**
 * Damage reaction of the test UFO: plays the common damage reaction with the hurt pose.
 * On the last hit of a move (damage flag 0x100) it first stops the UFO in place (btlevtcmd_UFODamageOn)
 * and afterwards sends it flying away (btlevtcmd_UFOEscapeOn). The words after its RETURN are never reached.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 39)
        SET_VALUE(LW(15), LW(14))
        CALL(btlevtcmd_UFODamageOn)
        CALL_CHILD(subsetevt_damage)
        SET_VALUE(LW(14), 40)
        CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(10), LW(14))
        CALL(btlevtcmd_UFOEscapeOn)
    ELSE
        SET_VALUE(LW(14), 39)
        SET_VALUE(LW(15), LW(14))
        CALL_CHILD(subsetevt_damage)
        SET_VALUE(LW(14), 40)
    END_IF
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
EVT_END()

/** Attack script of the test UFO: does nothing; it never attacks. */
static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

/** Wait script of the test UFO: shows its idle pose (CAT_S_1) from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END_PAD()

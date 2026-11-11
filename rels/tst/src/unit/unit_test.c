// test (unit kind 187): a test enemy on the Koopa poses (NKT_*) with a flip-over state (dmg_turn_event,
// wakeup_event) and one attack.
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// Unit work slots of the test enemy.
enum {
    UW_TEST_FLIPPED_TURNS = 0, // turns left on its back after being flipped (0: upright)
};

static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static PoseTableEntry pose_table[7];
static PoseTableEntry pose_table_turn[6];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static StatusVulnerability regist;
BattleUnitKind unit_test;
static BattleWeapon weapon_test_attack;
static EvtWord init_event[26];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[54];
static EvtWord wakeup_event[67];
static EvtWord attack_event[190];
static EvtWord wait_event[6];

/** Base defence of the test unit `test`: no damage reduction. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of the test unit `test`: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Base defence of the test unit `test` while it is flipped on its back: no damage reduction.
 */
static BattleUnitDefense defence_turn = {{0, 0, 0, 0, 0}};

/**
 * Pose table of the test unit `test`: maps each battle pose (waiting, hurt, running...) to one of its NKT_*
 * animations.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "NKT_S_1"},
    {29, "NKT_Q_1A"},
    {30, "NKT_Q_1B"},
    {31, "NKT_S_1"},
    {POSE_TYPE_DAMAGE, "NKT_D_1"},
    {POSE_TYPE_WALK, "NKT_W_1"},
    {POSE_TYPE_ACTION, "NKT_W_1"},
};

/**
 * Pose table of the test unit `test` while it is flipped on its back:
 * maps each battle pose (waiting, hurt, running...) to one of its NKT_* animations.
 */
static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_NORMAL, "NKT_B_1"},
    {29, "NKT_Q_1A"},
    {30, "NKT_Q_1B"},
    {31, "NKT_S_1"},
    {POSE_TYPE_DAMAGE, "NKT_B_1"},
    {POSE_TYPE_ACTION, "NKT_B_1"},
};

/** Data table of the test unit `test`: reaction scripts for being flipped over. */
static BattleDataEntry data_table[] = {
    {13, dmg_turn_event},
    {0, NULL},
};

/** Body part of the test unit `test`: a single part using the nokonoko model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_test", "nokonoko", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kIsShelled, 0, pose_table},
};

/** Status vulnerability of the test unit `test`: every status lands at the normal rate. */
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

/**
 * Unit definition of the test unit `test`: 2 HP and level 1, with its sizes, damage sounds,
 * status vulnerability, parts and init script.
 */
BattleUnitKind unit_test = {
    187,
    "btl_un_test",
    2,
    0,
    1,
    1,
    1,
    0,
    1,
    100,
    0,
    100,
    9,
    20,
    20,
    20,
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
    10.0f,
    0.0f,
    0.0f,
    10.0f,
    13.0f,
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
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BTL_DAMAGE1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kUnitLeader,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

/** Attack data of the test unit `test`: it deals no direct damage. */
static BattleWeapon weapon_test_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0, // special
    0, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Init script of the test enemy: registers its wait, attack and damage scripts and the default confusion script,
 * clears unit work 0 (not flipped) and starts the wait script.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_TEST_FLIPPED_TURNS, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage reaction of the test enemy: the default damage script. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Flip-over reaction (data event 13): switches to the flipped defence and pose tables,
 * sets unit work 0 to 2 (turns left on its back), takes pose NKT_D_1 and bounces twice in place.
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_SetUnitWork, LW(10), UW_TEST_FLIPPED_TURNS, 2)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("NKT_D_1"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, -1)
    RETURN
EVT_END()

/**
 * Run instead of an attack while flipped: counts unit work 0 down and, when it reaches 0,
 * restores the normal defence and pose tables and hops back onto its feet (pose NK_S_1).
 * Otherwise it just stays down for another turn.
 */
static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetUnitWork, LW(10), UW_TEST_FLIPPED_TURNS, LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetUnitWork, LW(10), UW_TEST_FLIPPED_TURNS, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("NK_S_1"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
EVT_END()

/**
 * The test enemy's only attack (weapon_test_attack): while flipped it tries to get up instead.
 * Otherwise it picks a target, jumps onto it in 30 frames,
 * and on a hit applies the guard result and damage before jumping home;
 * on a miss it bounces past the target, waits half a second and jumps home.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_TEST_FLIPPED_TURNS, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_test_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_test_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_test_attack))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_test_attack), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_MISS)
        CASE_OR_EQUAL(PRECHECK_LUCKY)
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        CASE_END
        CASE_EQUAL(PRECHECK_HIT)
        CASE_DEFAULT
            WAIT_MS(1000)
    END_SWITCH
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
    IF_INT_NOT_EQUAL(LW(5), PRECHECK_HIT)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)
        CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -1)
        WAIT_MS(500)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, -1)

        // BUG: returns before LABEL(99), so after a miss btlevtcmd_StartWaitEvent is not called as on the
        // other paths.
        RETURN
    END_IF
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_test_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_test_attack), 256, LW(5))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, -1)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script of the test enemy: shows its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

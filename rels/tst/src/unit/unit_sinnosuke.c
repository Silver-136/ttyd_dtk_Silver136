// Battle unit Bald Cleft (`sinnosuke`): tst.rel's copy of rels/hei/src/unit/unit_sinnosuke.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"

BattleUnitKind unit_sinnosuke;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_sinnosuke_attack;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_turn[15];
static BattleDataEntry data_table[3];
static EvtWord init_event[42];
static EvtWord damage_event[58];
static EvtWord dmg_turn_event[91];
static EvtWord attack_event[454];
static EvtWord wait_event[6];

/**
 * Unit definition of the Bald Cleft: 2 HP and level 12, with its sizes, damage sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_sinnosuke = {
    18,
    "btl_un_sinnosuke",
    2,
    0,
    1,
    1,
    12,
    0,
    5,
    100,
    0,
    70,
    9999,
    26,
    28,
    4,
    28,
    0.0f,
    14.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    13.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    13.0f,
    0.0f,
    0.0f,
    10.0f,
    24.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    26.0f,
    28.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_EMON_DAMAGED1",
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

/**
 * Defence of the Bald Cleft: 2 against normal, ice, explosion and electric hits;
 * immune to fire (defence 99 cancels the hit).
 */
static BattleUnitDefense defence = {{2, 99, 2, 2, 2}};

/** Element modifiers of the Bald Cleft: fire hits do no damage. */
static BattleUnitDefenseAttr defence_attr = {{0, 2, 0, 0, 0}};

/** Defence of the Bald Cleft while it is flipped on its back: none, every hit does its full damage. */
static BattleUnitDefense defence_turn = {0};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of the Bald Cleft in the demo builds: immune to burn; resists most other statuses.
 */
static StatusVulnerability regist = {40, 90, 50, 100, 50, 100, 0, 70, 100, 70, 100, 70, 100, 80, 80, 50, 30, 100, 80, 100, 100, 95};
#else
/**
 * Status vulnerability of the Bald Cleft in the retail builds: immune to burn; resists most other statuses.
 */
#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {40, 90, 50, 100, 50, 100, 0, 70, 100, 70, 100, 70, 100, 80, 80, 50, 30, 100, 80, 100, 100, 95};
#else
static StatusVulnerability regist = {50, 100, 50, 100, 50, 100, 0, 70, 100, 70, 100, 70, 100, 100, 70, 50, 40, 100, 70, 100, 100, 95};
#endif
#endif

/** Body part of the Bald Cleft: a single part using the c_sinnosuke model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_sinnosuke", "c_sinnosuke", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | kIsBombFlippable, 0, pose_table},
};

/** Attack data of the Bald Cleft's charge: 1 damage. */
static BattleWeapon weapon_sinnosuke_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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
 * Pose table of the Bald Cleft: maps each battle pose (waiting, hurt, running...) to one of its SIS_*
 * animations.
 */
static PoseTableEntry pose_table[] = {
    {1, "SIS_N_1"},
    {2, "SIS_Y_1"},
    {9, "SIS_Y_1"},
    {5, "SIS_K_1"},
    {4, "SIS_I_1"},
    {3, "SIS_I_1"},
    {BTL_POSE_NORMAL, "SIS_S_1"},
    {29, "SIS_Q_1"},
    {30, "SIS_Q_1"},
    {31, "SIS_A_1a"},
    {BTL_POSE_DAMAGE, "SIS_D_1"},
    {50, "SIS_A_1a"},
    {POSE_TYPE_WALK, "SIS_W_1"},
    {POSE_TYPE_RUN, "SIS_R_1"},
    {POSE_TYPE_DANGER, "SIS_I_1"},
    {POSE_TYPE_PERIL, "SIS_I_1"},
    {POSE_TYPE_TALK, "SIS_T_1"},
    {POSE_TYPE_ACTION, "SIS_S_1"},
};

/**
 * Pose table of the Bald Cleft while it is flipped on its back:
 * maps each battle pose (waiting, hurt, running...) to one of its SIS_* animations.
 */
static PoseTableEntry pose_table_turn[] = {
    {1, "SIS_N_2"},
    {2, "SIS_Y_2"},
    {9, "SIS_Y_2"},
    {5, "SIS_K_2"},
    {4, "SIS_K_2"},
    {3, "SIS_K_2"},
    {BTL_POSE_NORMAL, "SIS_D_2"},
    {29, "SIS_D_2"},
    {30, "SIS_D_2"},
    {31, "SIS_D_2"},
    {BTL_POSE_DAMAGE, "SIS_D_2"},
    {POSE_TYPE_DANGER, "SIS_K_2"},
    {POSE_TYPE_PERIL, "SIS_K_2"},
    {POSE_TYPE_TALK, "SIS_D_2"},
    {POSE_TYPE_ACTION, "SIS_D_2"},
};

/**
 * Data table of the Bald Cleft: reaction scripts for being flipped over;
 * a First Strike on it triggers nothing special.
 */
static BattleDataEntry data_table[] = {
    {13, dmg_turn_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Registers the Bald Cleft's wait, attack and damage events (confusion reuses the attack event) and its walk,
 * run and jump sounds, then starts the idle loop.
 * `sinnosuke` is the Bald Cleft: 2 HP, defence 2 (immune to fire),
 * attack 1 and no spikes (part counter attributes 0).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), STRING("SFX_ENM_EMON_MOVE2"), 0, 10, 10)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), STRING("SFX_ENM_EMON_MOVE2"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_JUMP1"), STRING("SFX_ENM_EMON_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Hit reaction: takes the hurt pose SIS_D_1 when the hit did damage;
 * while flipped it switches to the upside-down pose SIS_D_2 and bounces twice in place.
 * Then runs btldefaultevt_Damage.
 */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(15))
    IF_INT_GREATER_OR_EQUAL(LW(15), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SIS_D_1"))
    END_IF
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SIS_D_2"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -1)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 5, -1)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * The pose and bounce part of damage_event without the btldefaultevt_Damage call.
 * Nothing in this unit references it.
 */
static EVT_BEGIN(damage_event_sub)
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(15))
    IF_INT_GREATER_OR_EQUAL(LW(15), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SIS_D_1"))
    END_IF
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SIS_D_2"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -1)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 5, -1)
        RETURN
    END_IF
    RETURN
EVT_END()

/**
 * Flip event (data-table event 13, run by subsetevt_damage for a flipping hit, e.g. a bomb blast):
 * defence drops to 0, the upside-down pose table is used and it stays on its back for 2 turns.
 * It bounces twice (SFX_ENM_INSIDE1) and rests at height 5 with a lowered hit offset.
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("SIS_D_2"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 15, -1)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 5, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 5, LW(2))
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, -10, 0)
    RETURN
EVT_END()

/**
 * Runs instead of attacking while flipped: counts the flip turns down and, when they run out,
 * restores normal defence, poses and ground height, then hops back upright (SFX_ENM_INSIDE2).
 * Either way the turn ends.
 */
static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("SIS_S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Attack (also used when confused). While flipped it runs wakeup_event instead.
 * Otherwise it uses a held item if the AI wants to, or picks the frontmost target,
 * revs up and charges into it for 1 damage (weapon_sinnosuke_attack):
 * on a hit it shakes the camera and bounces back off the target; on a miss or a dodge it runs past.
 * Then it goes home.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF
    // First-strike battle: the target is already fixed; otherwise one is picked from the opposing party.
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        // The camera frames the Bald Cleft and its target.
        CALL(evt_btl_camera_set_mode, 0, 8)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
        WAIT_MS(800)
    ELSE
        // If the Bald Cleft holds an item, it uses that instead of attacking.
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sinnosuke_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sinnosuke_attack), LW(3), LW(4))
    END_IF
    // No target to attack: a confused Bald Cleft just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sinnosuke_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sinnosuke_attack))
    // The Bald Cleft walks up to the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 100)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, -1, 1)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // It rears back and revs up, shaking, for about three quarters of a second.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(750)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_DASH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    // It charges at the target.
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, -1, 0)
    DELETE_EVENT(LW(15))
    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sinnosuke_attack), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    // A miss: it charges on past the target.
    LABEL(90)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, -1, 0)
    WAIT_MS(500)
    GOTO(98)
    // A hit: the camera shakes, the target takes the damage and the Bald Cleft bounces back off it in two
    // hops.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sinnosuke_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sinnosuke_attack), 256, LW(5))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(1.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 75)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 100)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    // Turn finished: the Bald Cleft goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle: plays the stand pose of the current pose table (normal or flipped). */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

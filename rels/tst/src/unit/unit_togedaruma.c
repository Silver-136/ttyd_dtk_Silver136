// Battle unit Bristle (`togedaruma`): tst.rel's copy of rels/hei/src/unit/unit_togedaruma.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "evt/evt_eff.h"

BattleUnitKind unit_togedaruma;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_turn[15];
static BattleDataEntry data_table[6];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_togedaruma_attack;
static EvtWord init_event[21];
static EvtWord dmg_avoid_counter_thorn_head_event[63];
static EvtWord dmg_avoid_counter_thorn_front_event[121];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[131];
static EvtWord attack_event[358];
static EvtWord wait_event[102];

/**
 * Unit definition of the Bristle: 2 HP and level 16, with its sizes, damage sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_togedaruma = {
    UNIT_BRISTLE,
    "btl_un_togedaruma",
    2,
    0,
    1,
    1,
    16,
    0,
    5,
    100,
    0,
    70,
    9999,
    46,
    40,
    5,
    40,
    0.0f,
    20.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    23.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    23.0f,
    0.0f,
    0.0f,
    20.0f,
    26.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    46.0f,
    40.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_TOGEDA_DAMAGED1",
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

/**
 * Defence of the Bristle: 4 against normal, ice, explosion and electric hits;
 * immune to fire (defence 99 cancels the hit).
 */
static BattleUnitDefense defence = {{4, 99, 4, 4, 4}};

/** Element modifiers of the Bristle: fire hits do no damage. */
static BattleUnitDefenseAttr defence_attr = {{0, 2, 0, 0, 0}};

/** Defence of the Bristle while it is flipped on its back: none, every hit does its full damage. */
static BattleUnitDefense defence_turn = {0};

#if VERSION <= VERSION_DEMO_US
/** Status vulnerability of the Bristle in the demo builds: immune to burn; resists most other statuses. */
static StatusVulnerability regist = {40, 90, 50, 100, 50, 100, 0, 70, 100, 70, 100, 70, 100, 70, 70, 50, 75, 100, 70, 100, 100, 95};
#else
/**
 * Status vulnerability of the Bristle in the retail builds: immune to burn; resists most other statuses.
 */
#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {40, 90, 50, 100, 50, 100, 0, 70, 100, 70, 100, 70, 100, 70, 70, 50, 75, 100, 70, 100, 100, 95};
#else
static StatusVulnerability regist = {40, 100, 40, 100, 40, 100, 0, 60, 100, 60, 100, 60, 100, 100, 60, 50, 40, 100, 60, 100, 100, 95};
#endif
#endif

/**
 * Pose table of the Bristle: maps each battle pose (waiting, hurt, running...) to one of its TGD_* animations.
 */
static PoseTableEntry pose_table[] = {
    {STATUS_SLEEP, "TGD_N_1"},
    {STATUS_STOP, "TGD_I_1"},
    {STATUS_FREEZE, "TGD_I_1"},
    {STATUS_CONFUSE, "TGD_K_1"},
    {STATUS_POISON, "TGD_X_1"},
    {STATUS_DIZZY, "TGD_X_1"},
    {BTL_POSE_NORMAL, "TGD_S_4"},
    {29, "TGD_Q_1"},
    {30, "TGD_Q_1"},
    {31, "TGD_S_2"},
    {BTL_POSE_DAMAGE, "TGD_D_1"},
    {50, "TGD_A_1"},
    {POSE_TYPE_WALK, "TGD_S_4"},
    {POSE_TYPE_RUN, "TGD_S_4"},
    {POSE_TYPE_DANGER, "TGD_X_1"},
    {POSE_TYPE_PERIL, "TGD_X_1"},
    {POSE_TYPE_TALK, "TGD_T_1"},
    {POSE_TYPE_ACTION, "TGD_S_4"},
};

/**
 * Pose table of the Bristle while it is flipped on its back:
 * maps each battle pose (waiting, hurt, running...) to one of its TGD_* animations.
 */
static PoseTableEntry pose_table_turn[] = {
    {STATUS_SLEEP, "TGD_N_2"},
    {STATUS_STOP, "TGD_I_2"},
    {STATUS_FREEZE, "TGD_Z_4"},
    {STATUS_CONFUSE, "TGD_K_2"},
    {STATUS_POISON, "TGD_X_2"},
    {STATUS_DIZZY, "TGD_X_2"},
    {BTL_POSE_NORMAL, "TGD_S_5"},
    {29, "TGD_Q_2"},
    {30, "TGD_Q_2"},
    {31, "TGD_S_3"},
    {BTL_POSE_DAMAGE, "TGD_D_2"},
    {POSE_TYPE_DANGER, "TGD_X_2"},
    {POSE_TYPE_PERIL, "TGD_X_2"},
    {POSE_TYPE_TALK, "TGD_S_5"},
    {POSE_TYPE_ACTION, "TGD_S_5"},
};

/**
 * Data table of the Bristle: reaction scripts for being flipped over and an attacker hurt by its spikes;
 * a First Strike on it triggers nothing special.
 */
static BattleDataEntry data_table[] = {
    {13, dmg_turn_event},
    {37, dmg_avoid_counter_thorn_head_event},
    {39, dmg_avoid_counter_thorn_front_event},
    {38, dmg_avoid_counter_thorn_front_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/** Body part of the Bristle: a single part using the c_togedaruma model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_togedaruma", "c_togedaruma", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | kIsBombFlippable, kCounterTopSpiky | kCounterPreemptiveFrontSpiky | kCounterFrontSpiky, pose_table},
};

/** Attack data of the Bristle's spiked slide: 1 damage. */
static BattleWeapon weapon_togedaruma_attack = {
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
 * Init script of the Bristle: registers its wait,
 * attack and damage scripts (confused, it runs the normal attack script) and starts the wait script.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Run when an attack on the Bristle's top is stopped by its spikes (data event 37):
 * after half a second it flashes a toge_flush effect on its spikes,
 * placed higher when it is Huge and lower when Tiny.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 58)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 32)
        GOTO(90)
    END_IF
    ADD(LW(1), 45)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

/**
 * Run when a front attack is stopped by its spikes (data events 38 and 39):
 * it thrusts its spikes (TGD_J_1/J_2, or J_3/J_4 when flipped) with SFX_ENM_TOGEDA_MOVE1
 * and flashes toge_flush in front of itself (placed by Huge/Tiny).
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_front_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FLIPPED, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_3"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_4"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_1"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_2"))
        END_IF
    END_SIBLING_EVENT
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 53)
        ADD(LW(1), 26)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 33)
        ADD(LW(1), 16)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 43)
    ADD(LW(1), 21)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

/** Damage reaction of the Bristle: the default damage script. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Flip-over reaction (data event 13): switches to the flipped defence and pose tables,
 * sets the flipped status flag and an overturn count of 2, plays SFX_ENM_INSIDE1 and hops in place,
 * spinning over the first time, then rests at y 10.
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(12))
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_1"))
    IF_INT_LESS_OR_EQUAL(LW(12), 0)
        SET_VALUE(LW(12), 0)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                IF_INT_GREATER_OR_EQUAL(LW(12), 10)
                    BREAK_LOOP
                END_IF
                ADD(LW(12), 1)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -18)
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 25, -1)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_5"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 10, LW(2))
    RETURN
EVT_END()

/**
 * Run instead of an attack while flipped: counts the overturn count down and, when it reaches 0,
 * restores the normal defence and pose tables and spins back upright with SFX_ENM_TOGEDA_MOVE6,
 * setting its home back to the ground.
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
    SET_VALUE(LW(12), 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(12), 10)
                BREAK_LOOP
            END_IF
            ADD(LW(12), 1)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -18)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_ENM_TOGEDA_MOVE6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_4"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Bristle's attack (weapon_togedaruma_attack): while flipped it tries to get up instead.
 * It may use an item first; otherwise it raises its spikes and slides at the target.
 * A hit applies the guard result and damage with a camera shake;
 * a miss or dodge has it tumble past onto the floor and pause. Then it hops home.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        // If it is flipped over it tries to get up instead of attacking.
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF
    // First-strike battle: the target is already fixed; otherwise one is picked from the opposing party.
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        // If the Bristle holds an item, it uses that instead of attacking.
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_togedaruma_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_togedaruma_attack), LW(3), LW(4))
    END_IF
    // No target to attack: a confused Bristle just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // It crouches and then rears up with its spikes out.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_1"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_A_1"))
    WAIT_MS(400)
    // The camera frames the Bristle and its target.
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
    // It slides at the target.
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togedaruma_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // A miss: it tumbles on past the target, lands on the floor and lies there for a moment.
    LABEL(90)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_A_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 0, FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), FLOAT(60.0f), -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_N_1"))
    WAIT_MS(1500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    // A hit: the camera shakes and the target takes the damage.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togedaruma_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    // It hops back to its place.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(5.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_2"))
    WAIT_FRAMES(15)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_S_4"))
    WAIT_MS(400)
    // Turn finished: the Bristle goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Wait script of the Bristle: shows its idle pose
 * and every frame turns off its pre-emptive front spikes (the counter that hurts anyone who walks up to it)
 * while it is asleep, stopped, dizzy, poisoned, confused or frozen.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    LABEL(0)
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_SLEEP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_STOP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DIZZY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_POISON, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FREEZE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterPreemptiveFrontSpiky)
    ELSE
        CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterPreemptiveFrontSpiky)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

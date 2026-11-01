// Battle unit Paragoomba (`patakuri`): tst.rel's copy of rels/gon/src/unit/unit_patakuri.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "system.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

static USER_FUNC(krb_get_dir);

BattleUnitKind unit_patakuri;
static BattleUnitDefense defence_patakuri;
static BattleUnitDefenseAttr defence_patakuri_attr;
static StatusVulnerability regist_patakuri;
static BattleWeapon weapon_patakuri_attack;
static BattleUnitKindPart parts_patakuri[1];
static PoseTableEntry pose_table_patakuri[19];
static BattleDataEntry data_table_patakuri[3];
static PoseSoundTiming pose_sound_timing_table[7];
static EvtWord init_event_patakuri[26];
static EvtWord dmg_fall_event[107];
static EvtWord patakuri_dmg_first_attack_pos[19];
static EvtWord attack_event_patakuri[391];
static EvtWord wait_event_patakuri[12];
static PoseTableEntry pose_table[20];
static BattleDataEntry data_table[2];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_kuriboo_attack;
static EvtWord init_event[42];
static EvtWord damage_event[10];
static EvtWord attack_event[492];
static EvtWord wait_event[6];

/**
 * Unit definition of the Paragoomba: 2 HP and level 7, with its sizes, damage sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_patakuri = {
    2,
    "btl_un_patakuri",
    2,
    0,
    1,
    1,
    7,
    0,
    1,
    60,
    0,
    60,
    9999,
    23,
    25,
    5,
    25,
    0.0f,
    12.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    11.5f,
    0.0f,
    0.0f,
    11.5f,
    16.25f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    23.0f,
    25.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_PATAKURI_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    UNIT_ATTR_FLOATING | kUnitLeader,
    &regist_patakuri,
    1,
    parts_patakuri,
    init_event_patakuri,
    data_table_patakuri,
};

/** Defence of the Paragoomba while it still has its wings: none, every hit does its full damage. */
static BattleUnitDefense defence_patakuri = {{0, 0, 0, 0, 0}};

/** Element modifiers of the Paragoomba while it still has its wings: none, no element is special to it. */
static BattleUnitDefenseAttr defence_patakuri_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of the Paragoomba while it still has its wings in the demo builds:
 * extra prone to stop, dizzy, fright, Gale Force and instant KO.
 */
static StatusVulnerability regist_patakuri = {100, 120, 120, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 150, 100, 100, 100, 100, 150};
#else
/**
 * Status vulnerability of the Paragoomba while it still has its wings in the retail builds:
 * extra prone to stop, dizzy, fright, Gale Force and instant KO.
 */
static StatusVulnerability regist_patakuri = {100, 110, 120, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 110, 110, 100, 100, 100, 100, 150};
#endif

/** Attack data of the Paragoomba's dive attack: 1 damage. */
static BattleWeapon weapon_patakuri_attack = {
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
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    0, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/** Body part of the Paragoomba while it still has its wings: a single part using the c_kuribo model. */
static BattleUnitKindPart parts_patakuri[] = {
    {1, "btl_un_patakuri", "c_kuribo", 0.0f, 0.0f, 0.0f, 0.0f, 22.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence_patakuri, &defence_patakuri_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | kIsWinged | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table_patakuri},
};

/**
 * Pose table of the Paragoomba while it still has its wings:
 * maps each battle pose (waiting, hurt, running...) to one of its PTK_* animations.
 */
static PoseTableEntry pose_table_patakuri[] = {
    {1, "PTK_N_1"},
    {2, "PTK_Y_1"},
    {9, "PTK_Y_1"},
    {5, "PTK_K_1"},
    {4, "PTK_I_1"},
    {3, "PTK_I_1"},
    {BTL_POSE_NORMAL, "PTK_S_1"},
    {29, "PTK_Q_1"},
    {30, "PTK_Q_1"},
    {31, "PTK_S_1"},
    {BTL_POSE_DAMAGE, "PTK_D_1"},
    {50, "PTK_A_1"},
    {POSE_TYPE_RUN, "PTK_R_1"},
    {41, "PTK_R_2"},
    {POSE_TYPE_WALK, "PTK_W_1"},
    {POSE_TYPE_DANGER, "PTK_I_1"},
    {POSE_TYPE_PERIL, "PTK_I_1"},
    {POSE_TYPE_TALK, "PTK_T_1"},
    {POSE_TYPE_ACTION, "PTK_S_1"},
};

/**
 * Data table of the Paragoomba while it still has its wings:
 * reaction scripts for being knocked out of the air and a First Strike opening the battle.
 */
static BattleDataEntry data_table_patakuri[] = {
    {14, dmg_fall_event},
    {48, patakuri_dmg_first_attack_pos},
    {0, NULL},
};

/** Sound effects the Paragoomba plays at set points of its animations, keyed by animation name. */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"PTK_S_1", 0.01f, NULL, "SFX_ENM_PATAKURI_WAIT1", 0, 1},
    {"PTK_W_1", 0.01f, NULL, "SFX_ENM_PATAKURI_MOVE1", 0, 1},
    {"PTK_R_1", 0.01f, NULL, "SFX_ENM_PATAKURI_MOVE1", 0, 1},
    {"PTK_K_1", 0.01f, NULL, "SFX_ENM_PATAKURI_WAIT1", 0, 1},
    {"PTK_N_1", 0.01f, NULL, "SFX_ENM_PATAKURI_WAIT1", 0, 1},
    {"PTK_I_1", 0.01f, NULL, "SFX_ENM_PATAKURI_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Init script of the Paragoomba.
 * Registers its wait, attack, damage and fall scripts, sets the flying movement sounds and starts waiting.
 */
static EVT_BEGIN(init_event_patakuri)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_patakuri))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_patakuri))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event_patakuri))
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Reaction when a Paragoomba loses its wings: it drops with SFX_ENM_FALL1, lands with a hop,
 * and its pose table and data switch to those of the walking Goomba.
 */
static EVT_BEGIN(dmg_fall_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OffAttribute, LW(10), UNIT_ATTR_FLOATING)
    CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), kIsWinged)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_ReplaceParts, LW(10), LW(11), PTR(parts), 1)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_KIND_CHANGED)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), BTL_POSE_DAMAGE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 10, -1)
    CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL(btlevtcmd_ChangeKind, LW(10), 1)
    CALL_CHILD(init_event)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), BTL_POSE_DAMAGE)
    RETURN
EVT_END()

/**
 * Data-table event 48, run when the battle opens with Mario's or his partner's First Strike on the Paragoomba:
 * puts it at height 10 and waits for the attack to end.
 */
static EVT_BEGIN(patakuri_dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * The Paragoomba's dive attack: it flies up, swoops down on the target with SFX_ENM_PATAKURI_MOVE2 and MOVE3,
 * and the pre-damage check decides between a miss (avoid animation)
 * and a hit (damage with the action-command result); then it returns to its place.
 */
static EVT_BEGIN(attack_event_patakuri)
    // If the Paragoomba holds an item, it uses that instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // A target is picked from the opposing party.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_patakuri_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_patakuri_attack), LW(3), LW(4))

    // No target to attack: a confused Paragoomba just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The camera frames the Paragoomba and its target.
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_patakuri_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_patakuri_attack))

    // It flaps up to a spot above and short of the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 50)
    ADD(LW(1), 40)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, 0, 0, -1)
    WAIT_MS(100)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)

    // It tilts into its dive pose and swoops down at the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 50)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAKURI_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 5, 4, 0, 0)

    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_patakuri_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // A miss: it flies on past the target and hovers there for a moment.
    LABEL(90)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAKURI_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 50)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -20, 1, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    WAIT_MS(500)
    GOTO(98)

    // A hit: it crashes into the target, the camera shakes and the damage is dealt.
    LABEL(91)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAKURI_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, 1, 0, 0)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_patakuri_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_patakuri_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)

    // The dive is over: it flies back up to its place.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_PATAKURI_MOVE1"), STRING("SFX_ENM_PATAKURI_MOVE1"), 0, 15, 15, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, 0, 0, -2)

    // Turn finished: the Paragoomba goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script of the Paragoomba: shows its flying idle pose from the pose table. */
static EVT_BEGIN(wait_event_patakuri)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    LABEL(0)
    WAIT_FRAMES(30)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Pose table of the Paragoomba once its wings are gone:
 * maps each battle pose (waiting, hurt, running...) to one of its KUR_* animations.
 */
static PoseTableEntry pose_table[] = {
    {1, "KUR_N_1"},
    {2, "KUR_Y_1"},
    {9, "KUR_Y_1"},
    {5, "KUR_K_1"},
    {4, "KUR_I_1"},
    {3, "KUR_I_1"},
    {BTL_POSE_NORMAL, "KUR_S_1"},
    {29, "KUR_Q_1"},
    {30, "KUR_Q_1"},
    {31, "KUR_S_1"},
    {BTL_POSE_DAMAGE, "KUR_D_1"},
    {50, "KUR_A_1"},
    {51, "KUR_A_2"},
    {POSE_TYPE_RUN, "KUR_R_1"},
    {41, "KUR_R_2"},
    {POSE_TYPE_WALK, "KUR_W_1"},
    {POSE_TYPE_DANGER, "KUR_I_1"},
    {POSE_TYPE_PERIL, "KUR_I_1"},
    {POSE_TYPE_TALK, "KUR_T_1"},
    {POSE_TYPE_ACTION, "KUR_S_1"},
};

/**
 * Data table of the Paragoomba once its wings are gone: it has no special reaction scripts;
 * a First Strike on it triggers nothing special.
 */
static BattleDataEntry data_table[] = {
    {48, btldefaultevt_Dummy},
    {0},
};

/** Defence of the Paragoomba once its wings are gone: none, every hit does its full damage. */
static BattleUnitDefense defence = {0};

/** Element modifiers of the Paragoomba once its wings are gone: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {0};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of the Paragoomba once its wings are gone in the demo builds:
 * extra prone to stop, fright, Gale Force and instant KO.
 */
static StatusVulnerability regist = {100, 120, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 100, 100, 100, 100, 150};
#else
/**
 * Status vulnerability of the Paragoomba once its wings are gone in the retail builds:
 * extra prone to stop, fright and instant KO.
 */
static StatusVulnerability regist = {100, 110, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 110, 100, 100, 100, 100, 100, 150};
#endif

/** Body part of the Paragoomba once its wings are gone: a single part using the c_kuribo model. */
static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_kuriboo", // partName
        "c_kuribo", // modelName
        {0}, // positionOffset
        {0.0f, 22.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, // attributes
        0, // counterAttributes
        pose_table, // poseTable
    },
};

/** Attack data of the Paragoomba's headbutt: 1 damage. */
static BattleWeapon weapon_kuriboo_attack = {
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
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    0, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
 * Init script of the Goomba form of the Paragoomba (after losing its wings).
 * Registers its wait, attack and damage scripts and the walking sounds.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 6, 6)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_JUMP1"), STRING("SFX_ENM_KURI_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage reaction of the walking Paragoomba: the default damage script. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Phase script of the walking Paragoomba; does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** The Goomba-form headbutt attack, the same as the Goomba's. */
static EVT_BEGIN(attack_event)
    // First-strike battle: the target is already fixed; otherwise one is picked from the opposing party.
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_kuriboo_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_kuriboo_attack), LW(3), LW(4))
    END_IF

    // No target to attack: a confused Goomba just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // If the Goomba holds an item, it uses that instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.5996094f))

    // The camera frames the Goomba and its target.
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_kuriboo_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_kuriboo_attack))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 100)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))

    // The Goomba runs up to the target and crouches.
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, -1, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, BTL_POSE_STAY)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(170)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)

    // It leaps, its body tilting along the arc of the jump.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        SET_VALUE(LW(8), 0)
        LOOP(32)
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(9), LW(10), LW(11))
            CALL(krb_get_dir, LW(5), LW(6), LW(9), LW(10), LW(8))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(8))
            SET_FLOAT_VALUE(LW(5), LW(9))
            SET_FLOAT_VALUE(LW(6), LW(10))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(12)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(15), 50)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))

    // It comes down on the target head first.
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 32, -1)

    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_kuriboo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(91)
        CASE_END
    END_SWITCH

    // A miss: it lands beyond the target and hops away in two small hops.
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(15), POSE_TYPE_ACTION)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -1)
    WAIT_MS(500)
    GOTO(95)

    // A hit: the camera shakes, the target takes the damage and the Goomba bounces off it in three hops.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KURI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_kuriboo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_kuriboo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(15), POSE_TYPE_ACTION)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, -1)
    GOTO(95)

    // It runs back to its place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 3, 3, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 41)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -2, 0)

    // Turn finished: the Goomba goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script of the walking Paragoomba: shows its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Script helper for the Goomba family: args[0], args[1] are the x and z of one point and args[2],
 * args[3] those of another; args[4] receives the angle between them (angleABf of the x/z difference, degrees).
 * When the two points coincide, args[4] keeps the value it already held.
 */
static USER_FUNC(krb_get_dir) {
    EvtWord* args = event->args;
    f32 x1 = evtGetFloat(event, args[0]);
    f32 z1 = evtGetFloat(event, args[1]);
    f32 x2 = evtGetFloat(event, args[2]);
    f32 z2 = evtGetFloat(event, args[3]);
    s32 dir = evtGetValue(event, args[4]);
    f32 dx = x2 - x1;
    f32 dz = z2 - z1;

    if (dx == 0.0f && dz == 0.0f) {
        evtSetValue(event, args[4], dir);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[4], angleABf(dx, dz, 0.0f, 0.0f));
    return EVT_RETURN_DONE;
}

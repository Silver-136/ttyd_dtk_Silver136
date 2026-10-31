#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "evt/evt_sub.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"


BattleUnitKind unit_gundan_zako;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[16];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_gundan_zako_attack;
static BattleWeapon weapon_gundan_zako_jump_attack;
static BattleWeapon weapon_gundan_zako_dekadeka_bottle;
static EvtWord init_event[42];
static EvtWord damage_event[10];
static EvtWord attack_event[98];
static EvtWord attack_common_event1[182];
static EvtWord attack_common_event2[33];
static EvtWord normal_attack_event[307];
static EvtWord jump_attack_event[324];
static EvtWord dekadeka_event[125];
static EvtWord wait_event[6];

/** Unit data of the X-Naut: 4 HP, level 13; the battle starts it from init_event. */
BattleUnitKind unit_gundan_zako = {
    UNIT_X_NAUT, // mUnitType
    "btl_un_gundan_zako", // name
    4, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    13, // mLevel
    0, // mBonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    60, // runRate
    9999, // pbSoftCap
    30, // width
    36, // height
    {0, 36}, // hitOffset
    {0.0f, 18.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {15.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {15.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 27.0f, 0.0f}, // kissHitOffset
    {0.0f, 18.0f, 0.0f}, // cutBaseOffset
    30.0f, // cutWidth
    36.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_3RD1_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the X-Naut: none against any element. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Elemental reactions of the X-Naut: none, every element hurts it normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    90, // sleep
    90, // stop
    90, // dizzy
    100, // poison
    90, // confuse
    100, // electric
    100, // burn
    80, // freeze
    100, // huge
    95, // tiny
    100, // attackUp
    95, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    100, // fright
    80, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};

/** Pose table of the X-Naut: the animation each battle pose (idle, hurt, walk, attack and so on) plays. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_KO, "Z_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "D_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "D_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/** Event table of the X-Naut: empty, it has no special reactions. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Body part of the X-Naut, model c_zako_n: its one target part. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_gundan_zako", "c_zako_n", 0.0f, 0.0f, 0.0f, 0.0f, 28.0f, 0.0f, 0.0f, 38.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Weapon data for the X-Naut's run-up strike: 3 damage, a hammer-type hit, one target. */
static BattleWeapon weapon_gundan_zako_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon data for the X-Naut's jump attack: 3 damage, a jump-type hit, one target. */
static BattleWeapon weapon_gundan_zako_jump_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon data for the X-Naut's size-up bottle: no damage, one target, may inflict size change (100%). */
static BattleWeapon weapon_gundan_zako_dekadeka_bottle = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, // burnChance..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE1"), STRING("SFX_ENM_3RD1_MOVE2"), 0, 5, 5)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE1"), STRING("SFX_ENM_3RD1_MOVE2"), 0, 10, 10)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, 0, STRING("SFX_ENM_3RD1_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage event for the X-Naut: runs the default damage handler (btldefaultevt_Damage) with LW(10) = -2
 * and LW(11) = 1.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Attack script of the X-Naut: if battle flag 2 is set it always uses the normal attack;
 * otherwise it lets an enemy item be used if one is ready and rolls 50% normal attack (normal_attack_event),
 * 30% jump attack and 20% to drink the size-up bottle (dekadeka_event), re-rolling if it is already enlarged.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    LABEL(10)
    SET_VALUE(LW(0), 0)
    ADD(LW(0), 50)
    ADD(LW(0), 30)
    ADD(LW(0), 20)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 50)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 30)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(jump_attack_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        // Reroll this selection if already huge instead of consuming the turn on another bottle.
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(2))
        IF_INT_EQUAL(LW(2), 0)
            CALL_CHILD(PTR(dekadeka_event))
            GOTO(99)
        END_IF
    END_IF
    GOTO(10)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Shared run-up of the X-Naut's physical attacks:
 * pays for the attack, aims the camera, and runs toward the target with dust puffs
 * and footstep sounds (pose R_2), then switches to the attack pose A_1 and moves to the target's hit position.
 */
static EVT_BEGIN(attack_common_event1)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(14))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(14))
    BEGIN_SIBLING_EVENT
        LOOP(9)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(5)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(5)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_2"))
    LOOP(9)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
        ADD(LW(2), 1)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.5f), 10, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(10)
    END_LOOP
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.5f), 10, 0, 0, 0, 0, 0, 0)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(5)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 90)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // Stop the repeated movement sound once the approach has finished.
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    ADD(LW(2), 1)
    RETURN
EVT_END()

/**
 * Shared recovery of the X-Naut's physical attacks:
 * resets the camera and runs back to its home position in pose R_1, then restores its facing.
 */
static EVT_BEGIN(attack_common_event2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The X-Naut's normal attack: picks a target (the first attack target if battle flag 2 is set),
 * runs up (attack_common_event1) and jumps at the target;
 * a miss or dodge ends in the D_2/D_3 stumble poses with SFX_ENM_3RD1_MISS1,
 * a hit applies the damage with the 256 guard flag, then it returns through attack_common_event2.
 */
static EVT_BEGIN(normal_attack_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_attack))
    // Choose the target: the First Strike victim, otherwise a sampled opposing unit.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    // No valid target: a confused X-Naut is flustered, otherwise the turn is skipped.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    // Wind-up: it runs up to the target (attack_common_event1).
    CALL_CHILD(attack_common_event1)
    // It springs at the target with a shout.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.19921875f))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE)
    END_SIBLING_EVENT
    // It flies through the air until it reaches the target's position.
    LOOP(0)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), EVT_NULLPTR, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
        IF_INT_EQUAL(LW(14), -1)
            IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
                BREAK_LOOP
            END_IF
        ELSE
            IF_INT_GREATER_OR_EQUAL(LW(0), LW(10))
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    // Hit check: a miss or dodge goes on to the stumble, a hit to the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
        LABEL(90)
        // Miss: it lands with a whiff, stumbles forward and sits down dazed (D_2, D_3).
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_2"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(0.5f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 1)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_3"))
        WAIT_FRAMES(10)
        GOTO(98)
        LABEL(91)
        // Hit: the camera shakes and the damage is dealt with the action-command result.
        CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_add_zoom, 0, -50)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        // It rebounds off the target and lands back on its feet (J_1B, S_1).
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 48)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(2.0f), FLOAT(0.099609375f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("J_1B"))
        WAIT_FRAMES(5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
        WAIT_FRAMES(20)
        GOTO(98)
        LABEL(98)
        // Retreat: it runs back to its home position (attack_common_event2).
        CALL_CHILD(attack_common_event2)
        RETURN
EVT_END()

/**
 * The X-Naut's jump attack: picks a target, runs up and leaps with a high arc (pose A_2) onto it;
 * a miss ends in the D_2/D_3 stumble poses, a hit applies the damage
 * and bounces on the target in decreasing hops, then it returns through attack_common_event2.
 */
static EVT_BEGIN(jump_attack_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_jump_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL_CHILD(attack_common_event1)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(1.75f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
        LABEL(90)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_MISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_eff64, STRING(""), STRING("kemuri1_n64"), 4, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_2"))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(0.5f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 1)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_3"))
        WAIT_FRAMES(10)
        GOTO(98)
        LABEL(91)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_eff64, STRING(""), STRING("kemuri1_n64"), 4, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(0.5f), FLOAT(0.2998047f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(0.5f), FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 3)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(0.5f), FLOAT(0.099609375f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE)
        WAIT_FRAMES(15)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
        WAIT_FRAMES(10)
        GOTO(98)
        LABEL(98)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL_CHILD(attack_common_event2)
        RETURN
EVT_END()

/**
 * The X-Naut drinks a bottle (dekadeka) on itself:
 * camera close-up with bottle sounds and poses A_4A/A_4B,
 * then applies the dekadeka bottle weapon to itself (always hits; size-change status for 3 turns, strength 2).
 */
static EVT_BEGIN(dekadeka_event)
    SET_VALUE(LW(3), UNIT_ID_SELF)
    SET_VALUE(LW(4), 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gundan_zako_dekadeka_bottle))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_gundan_zako_dekadeka_bottle))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    WAIT_FRAMES(60)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4A"))
    WAIT_FRAMES(80)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(30)
        LOOP(3)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(20)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4B"))
    WAIT_FRAMES(120)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(5)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD1_BOTTLE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(55)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    END_INLINE_EVENT
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gundan_zako_dekadeka_bottle))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gundan_zako_dekadeka_bottle), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

/** Idle script of the X-Naut: sets the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()


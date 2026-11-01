// Rogueport Sewers (tik) battle unit: R. Tentacle (gesso_right_arm), Blooper's right tentacle: its parts, defence/status tables, and
// attack events.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "unit_boss_gesso.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

static USER_FUNC(_check_zenei_kouei);
extern s32 R_defence_attr_turn[];
extern s32 R_defence_turn[];
extern s32 R_regist_turn[];
extern PoseTableEntry gesso_pose_table_turn[];

BattleUnitKind unit_boss_gesso_right_arm;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
#if VERSION > VERSION_DEMO_US
BattleDataEntry gesso_right_arm_data_table[13];
#endif
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[1];
static EvtWord init_event[25];
static EvtWord damage_event[31];
#if VERSION > VERSION_DEMO_US
EvtWord gesso_right_arm_damage_event[93];
#endif
static EvtWord phase_event[2];
BattleWeapon weapon_gesso_right_arm_front_attack;
BattleWeapon weapon_gesso_right_arm_rear_attack;
static EvtWord attack_event[273];
static EvtWord wait_event[6];
EvtWord gesso_arm_dead_event[130];
EvtWord gesso_fall_check[219];
EvtWord gesso_arm_damage_pose_event[35];

/**
 * Unit kind of the R. Tentacle (HP 3, level 0):
 * its size, offsets, damage sounds, status vulnerability, parts and init event.
 */
BattleUnitKind unit_boss_gesso_right_arm = {
    UNIT_BLOOPER_RIGHT_TENTACLE, // unitType
    "btl_un_gesso_right_arm", // name
    3, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    0, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    60, // width
    110, // height
    {-68, -100}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {-85, -140}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {30.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {30.0f, 0.0f, 0.0f}, // bintaHitOffset
    {30.0f, 71.5f, 0.0f}, // kissHitOffset
    {-60.0f, -60.0f, 0.0f}, // cutBaseOffset
    60.0f, // cutWidth
    100.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    0, // hammerKnockbackChance
    0, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_GESSO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
#if VERSION <= VERSION_DEMO_US
    data_table,
#else
    gesso_right_arm_data_table, // dataTable
#endif
};

/** Defence of the R. Tentacle: no damage reduction. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of the R. Tentacle: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the R. Tentacle (percent chance to take each status):
 * immune to sleep, stop, dizzy, poison, confusion, electric shock, freeze, growing huge, shrinking, attack up,
 * attack down, defence up, defence down, allergy, fright, gale force, speed up, slow, dodgy,
 * invisibility and instant kill.
 */
static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    0, // poison
    0, // confuse
    0, // electric
    100, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    0, // defenseDown
    0, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "GSO_S1_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "GSO_S1_1"},
    {POSE_TYPE_DAMAGE, "GSO_D1_1"},
    {POSE_TYPE_ACTION, "GSO_S1_1"},
};

/** Data table of the Blooper's R. Tentacle: its damage reaction for the hit tags 24 to 36. */
#if VERSION > VERSION_DEMO_US
BattleDataEntry gesso_right_arm_data_table[] = {
    {UNIT_DATA_COUNTER_TOP_SPIKY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, gesso_right_arm_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, gesso_right_arm_damage_event},
};
#endif

/** Data table of the R. Tentacle: event handlers for its arm hit pose and being defeated. */
static BattleDataEntry data_table[] = {
    {10, gesso_arm_damage_pose_event},
    {UNIT_DATA_DEAD_EVENT, gesso_arm_dead_event},
    {0},
};

/** Body part of the R. Tentacle (model c_gesso2): hit position, defences, attributes and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_gesso_right_arm", "c_gesso2", 0.0f, 0.0f, 0.0f, 85.0f, -110.0f, 0.0f, 85.0f, -110.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, VERSION_PICK(0x680009, 0x2680009), 0, pose_table},
};

/**
 * Init script of the Blooper's right tentacle.
 * Registers its damage, phase and attack scripts and starts it waiting.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage reaction of the right tentacle: runs the default damage,
 * then tells the body (unit work 0) to go back to its wait script if it is idle.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_GetUnitWork, LW(3), GESSO_UNIT_WORK_BUSY, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(btlevtcmd_StartWaitEvent, LW(3))
        END_IF
    END_IF
    RETURN
EVT_END()

#if VERSION > VERSION_DEMO_US
/** Damage reaction of the Blooper's right tentacle (JP and later); the same as the left tentacle's damage event. */
EVT_BEGIN(gesso_right_arm_damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
    SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
    SET_VALUE(LW(15), -1)
    CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_DAMAGE_SURPRISE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), POSE_TYPE_DAMAGE_SURPRISED)
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_DAMAGE_POSE_EVENT)
    END_SIBLING_EVENT
    CALL_CHILD(PTR(subsetevt_damage))
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        WAIT_FRAMES(30)
        CALL_CHILD(PTR(subsetevt_dead))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(1))
    IF_INT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_WaitAttackEnd)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, LW(10))
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    LABEL(99)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_GetUnitWork, LW(3), GESSO_UNIT_WORK_BUSY, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(btlevtcmd_StartWaitEvent, LW(3))
        END_IF
    END_IF
    RETURN
EVT_END()
#endif

/** Phase script of the right tentacle; does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** Weapon of the R. Tentacle's front swat: 1 damage. */
BattleWeapon weapon_gesso_right_arm_front_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT, // targetWeightFlags
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

/** Weapon of the R. Tentacle's rear swat: 1 damage. */
BattleWeapon weapon_gesso_right_arm_rear_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_BACK, // targetWeightFlags
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
 * Attack script of the right tentacle: uses an enemy item if it has one;
 * otherwise picks the front or rear weapon
 * at random (they prefer front or back targets) and samples a target. The first time it targets Mario (unit kind
 * 0xDE, tracked in the Blooper's unit work 5) the Blooper asks whether Mario thinks it would taste good, with a
 * yes/no answer (tik_02_gs_04). It then swings with GSO_A1_1 or GSO_A1_3 depending on the target's side of the
 * stage (_check_zenei_kouei) and damage-checks the targets.
 */
static EVT_BEGIN(attack_event)
    // The tentacle may use an item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF

    // Pick the front or rear swat at random and choose a target.
    CALL(evt_sub_random, 1, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&weapon_gesso_right_arm_front_attack))
    ELSE
        SET_VALUE(LW(12), PTR(&weapon_gesso_right_arm_rear_attack))
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF

    // The attack is announced and the guard timing is awaited.
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))

    // The first time the target is Mario, the Blooper stops to ask whether he thinks it would taste good (yes
    // or no).
    SET_VALUE(LW(11), 0)
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    IF_INT_EQUAL(LW(0), UNIT_MARIO)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(9))
        CALL(btlevtcmd_GetUnitWork, LW(9), GESSO_UNIT_WORK_ARM_TALK_DONE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LW(11), 1)
            CALL(btlevtcmd_SetUnitWork, LW(9), GESSO_UNIT_WORK_ARM_TALK_DONE, 1)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(LW(11), 0)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(9))
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_02_gs_04_1"), 0, LW(9))
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_02_gs_04_2"), 0, LW(9))
        CALL(evt_msg_select, 0, STRING("tik_02_gs_04_3"))
        CALL(evt_msg_continue)
        IF_INT_EQUAL(LW(0), 0)
            BEGIN_INLINE_EVENT
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_02_gs_04_3_y"), 0, LW(9))
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            END_INLINE_EVENT
        ELSE
            BEGIN_INLINE_EVENT
                CALL(evt_msg_toge, MSG_TOGE_AT_POS, 110, 120, 30)
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_02_gs_04_3_n"), 0, LW(9))
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            END_INLINE_EVENT
        END_IF
    END_IF

    // The tentacle swings across the stage, from the near or far side depending on where the target stands.
    CALL(_check_zenei_kouei, LW(3), LW(13))
    IF_INT_EQUAL(LW(13), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_A1_1"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_A1_3"))
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftof, 32, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_ARM_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftof, 55, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
#if VERSION >= VERSION_US
    CALL(btlevtcmd_ftof, 97, LW(0))
#else
    CALL(btlevtcmd_ftof, 94, LW(0))
#endif
    WAIT_FRAMES(LW(0))

    // The swat lands unless the target dodges; then the damage is dealt.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 1)
        SWITCH(LW(5))
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_SWITCH
    ELSE
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF

    // The swing finishes and the tentacle returns to waiting.
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script of the right tentacle: shows its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Death script shared by the two tentacles.
 * Plays the arm's retreat sound and pose (GSO_F1_1),
 * clears the body's slot that pointed to this arm (the left arm waits 50 frames, the right 80) and
 * then runs the fall check; finally kills the arm unit.
 */
EVT_BEGIN(gesso_arm_dead_event)
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), UNIT_BLOOPER_RIGHT_TENTACLE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_ARM_GOOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_F1_1"))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), GESSO_UNIT_WORK_RIGHT_ARM, -1)
        END_IF
        WAIT_FRAMES(50)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), GESSO_UNIT_WORK_RIGHT_ARM, -1)
        END_IF
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_ARM_GOOUT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_F1_1"))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), GESSO_UNIT_WORK_LEFT_ARM, -1)
        END_IF
        WAIT_FRAMES(80)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), GESSO_UNIT_WORK_LEFT_ARM, -1)
        END_IF
    END_IF
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(3), GESSO_UNIT_WORK_BUSY, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 2)
        GOTO(99)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        WAIT_FRAMES(1)
        GOTO(10)
    END_IF
    CALL_CHILD(gesso_fall_check)
    LABEL(99)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

// Start the body fall only after both arms are gone and no fall is already in progress.
EVT_BEGIN(gesso_fall_check)
    CALL(btlevtcmd_GetUnitWork, LW(3), GESSO_UNIT_WORK_LEFT_ARM, LW(0))
    CALL(btlevtcmd_GetUnitId, LW(0), LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(3), GESSO_UNIT_WORK_RIGHT_ARM, LW(0))
    CALL(btlevtcmd_GetUnitId, LW(0), LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(3), GESSO_UNIT_WORK_BUSY, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(3), GESSO_UNIT_WORK_STATE, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, LW(3), GESSO_UNIT_WORK_BUSY, 2)
    CALL(btlevtcmd_GetUnitWork, LW(3), GESSO_UNIT_WORK_STATE_TURNS, 0)
    CALL(btlevtcmd_StopWaitEvent, LW(3))
    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F_1"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
    CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("GSO_F_2"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftof, 16, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_snd_se, LW(3), STRING("SFX_BOSS_GESSO_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(3), 1)
    CALL(btlevtcmd_OffAttribute, LW(3), kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, (kHammerlikeCannotTarget | kShellTosslikeCannotTarget))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(3), 1, PTR(gesso_pose_table_turn))
    CALL(btlevtcmd_SetHpGaugeOffset, LW(3), 0, 0)
    CALL(btlevtcmd_SetStatusIconOffset, LW(3), 46, 118)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(3), 1, PTR(R_defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(3), 1, PTR(R_defence_attr_turn))
    CALL(btlevtcmd_SetRegistStatus, LW(3), PTR(R_regist_turn))
    CALL(btlevtcmd_SetHitOffset, LW(3), 1, -15, -90, 0)
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 55, 113)
    CALL(evt_btl_camera_shake_h, BTL_CAM_PRI_NORMAL, 6, 0, 30, 4)
    CALL(btlevtcmd_SetUnitWork, LW(3), GESSO_UNIT_WORK_STATE, 1)
    CALL(btlevtcmd_SetHitOffset, LW(3), 1, 51, 38, 30)
    CALL(btlevtcmd_SetHitCursorOffset, LW(3), 1, 51, 38, 30)
    CALL(btlevtcmd_SetUnitWork, LW(3), GESSO_UNIT_WORK_BUSY, 0)
    CALL(btlevtcmd_StartWaitEvent, LW(3))
    RETURN
EVT_END()

/** When a tentacle is hit, makes the Blooper body (unit work 0) stop waiting and take the same damage pose (LW(14)). */
EVT_BEGIN(gesso_arm_damage_pose_event)
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), UNIT_BLOOPER_RIGHT_TENTACLE)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(3))
    ELSE
        IF_INT_EQUAL(LW(0), UNIT_BLOOPER_LEFT_TENTACLE)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, GESSO_ARM_WORK_BODY, LW(3))
        ELSE
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_StopWaitEvent, LW(3))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), 1, LW(14))
    RETURN
EVT_END()

// Select the arm animation from the target position, with X = -120 on the front side.
static USER_FUNC(_check_zenei_kouei) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 out = args[1];
    f32 x, y, z;

    id = BattleTransID(event, id);
    BtlUnit_GetPos(BattleGetUnitPtr(wp, id), &x, &y, &z);
    if (x >= -120.0f) {
        evtSetValue(event, out, 0);
    } else {
        evtSetValue(event, out, 1);
    }
    return EVT_RETURN_DONE;
}

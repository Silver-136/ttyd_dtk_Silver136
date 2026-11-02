// Rogueport Sewers (tik) battle unit: L. Tentacle (gesso_left_arm), Blooper's left tentacle: its parts, defence/status tables, and
// attack events.

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
#include "battle/battle_damage.h"
#include "unit_boss_gesso.h"

extern EvtWord gesso_arm_damage_pose_event[];
extern EvtWord gesso_arm_dead_event[];

BattleUnitKind unit_boss_gesso_left_arm;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];

#if VERSION > VERSION_DEMO_US
BattleDataEntry gesso_left_arm_data_table[13];
#endif

static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[1];
static EvtWord init_event[25];
static EvtWord damage_event[31];

#if VERSION > VERSION_DEMO_US
EvtWord gesso_left_arm_damage_event[93];
#endif

static EvtWord phase_event[2];
BattleWeapon weapon_gesso_left_arm_attack;
static EvtWord attack_event[171];
static EvtWord wait_event[6];

/**
 * Unit kind of the L. Tentacle (HP 3, level 0):
 * its size, offsets, damage sounds, status vulnerability, parts and init event.
 */
BattleUnitKind unit_boss_gesso_left_arm = {
    UNIT_BLOOPER_LEFT_TENTACLE, // unitType
    "btl_un_gesso_left_arm", // name
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
    150, // width
    40, // height
    {-240, 36}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {-255, -5}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {75.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {75.0f, 0.0f, 0.0f}, // bintaHitOffset
    {75.0f, 26.0f, 0.0f}, // kissHitOffset
    {-215.0f, 20.0f, 0.0f}, // cutBaseOffset
    150.0f, // cutWidth
    40.0f, // cutHeight
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
    gesso_left_arm_data_table, // dataTable
#endif
};

/** Defence of the L. Tentacle: no damage reduction. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of the L. Tentacle: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the L. Tentacle (multipliers for each status's base chance):
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

/** Data table of the Blooper's L. Tentacle: its damage reaction for the hit tags 24 to 36. */
#if VERSION > VERSION_DEMO_US
BattleDataEntry gesso_left_arm_data_table[] = {
    {UNIT_DATA_COUNTER_TOP_SPIKY_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, gesso_left_arm_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, gesso_left_arm_damage_event},
};
#endif

/** Data table of the L. Tentacle: event handlers for its arm hit pose and being defeated. */
static BattleDataEntry data_table[] = {
    {10, gesso_arm_damage_pose_event},
    {UNIT_DATA_DEAD_EVENT, gesso_arm_dead_event},
    {0},
};

/** Body part of the L. Tentacle (model c_gesso3): hit position, defences, attributes and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_gesso_left_arm", "c_gesso3", 0.0f, 0.0f, 0.0f, 270.0f, 20.0f, 0.0f, 270.0f, 20.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, VERSION_PICK(PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | PART_ATTR_UNTATTLEABLE, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | PART_ATTR_UNTATTLEABLE | PART_ATTR_NO_SHADOW), 0, pose_table},
};

/** Init script of the Blooper's left tentacle. Registers its damage, wait and attack scripts and starts it waiting. */
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
 * Damage reaction of the left tentacle: runs the default damage, then,
 * if the body (unit work 0) is idle (work 4 not above 0), tells the body to go back to its wait script.
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
/**
 * Damage reaction of the Blooper's left tentacle (JP and later).
 * Plays the hit pose (38 or 39 depending on the damage pattern), runs the damage and,
 * if the tentacle was defeated, the death event; otherwise it returns to the wait pose.
 * Afterwards the body is told to wait again when its busy work value is nonpositive.
 */
EVT_BEGIN(gesso_left_arm_damage_event)
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

/** Phase script of the left tentacle; does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** Weapon of the L. Tentacle's swat: 1 damage. */
BattleWeapon weapon_gesso_left_arm_attack = {
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
    TARGET_WEIGHT_FRONT, // targetWeightFlags
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
 * Attack script of the left tentacle: may use an enemy item; otherwise samples a target for
 * weapon_gesso_left_arm_attack, swings (GSO_A1_1) with the arm-move and attack sounds,
 * damage-checks the target (and each further one if the weapon hits several) and waits for the pose to finish.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&weapon_gesso_left_arm_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GSO_A1_1"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftof, 16, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_ARM_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftof, 44, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GESSO_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
#if VERSION >= VERSION_US
    CALL(btlevtcmd_ftof, 67, LW(0))
#else
    CALL(btlevtcmd_ftof, 64, LW(0))
#endif
    WAIT_FRAMES(LW(0))
    LABEL(50)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_NOT_EQUAL(LW(5), PRECHECK_HIT)
        SWITCH(LW(5))
            CASE_EQUAL(PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
            CASE_EQUAL(PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
            CASE_EQUAL(PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_SWITCH
    ELSE
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    CALL(btlevtcmd_CheckWeaponPluralTarget, LW(12), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_NOT_EQUAL(LW(3), -1)
            GOTO(50)
        END_IF
    END_IF
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script of the left tentacle: shows its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()


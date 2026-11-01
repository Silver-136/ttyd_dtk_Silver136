// Ruff Puff (kurokumorn): enemy battle unit (thunder attack); Riverside Station's copy of the unit data and scripts.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LOW 0 // 1 while low (y 10, hammer and shell toss can reach it), 0 while high (y 40); unison_phase_event switches it


BattleUnitKind unit_kurokumorn;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_kurokumorn_attack;
static BattleWeapon weapon_kurokumorn_thunder_attack;
static EvtWord attack_event[69];
static EvtWord thunder_event[493];
static PoseTableEntry pose_table[17];
static BattleDataEntry data_table[3];
static EvtWord init_event[37];
static EvtWord barrier_event[59];
static EvtWord dmg_first_attack_pos[19];
static EvtWord damage_event[10];
static EvtWord counter_event[2];
static EvtWord unison_phase_event[140];
static EvtWord normal_attack_event[386];
static EvtWord charge_event[58];
static EvtWord wait_event[12];

/** Unit data of the Ruff Puff: 7 HP, level 23; the battle starts it from init_event. */
BattleUnitKind unit_kurokumorn = {
    UNIT_RUFF_PUFF, // unitType
    "btl_un_kurokumorn", // name
    7, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    23, // level
    0, // bonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    35, // width
    30, // height
    {4, 30}, // hitOffset
    {0.0f, 15.0f, 0.0f}, // centerOffset
    {0, -3}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {18.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {17.5f, 0.0f, 0.0f}, // bintaHitOffset
    {17.5f, 19.5f, 0.0f}, // kissHitOffset
    {0.0f, 15.0f, 0.0f}, // cutBaseOffset
    35.0f, // cutWidth
    30.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_KUMO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the Ruff Puff: none against any element. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Elemental reactions of the Ruff Puff: is healed by electric attacks. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_ABSORB}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {60, 80, 80, 100, 70, 0, 100, 70, 100, 70, 100, 70, 100, 70, 70, 70, 90, 100, 70, 100, 100, 95};
#else
static StatusVulnerability regist = {
    75, // sleep
    75, // stop
    100, // dizzy
    100, // poison
    70, // confuse
    0, // electric
    100, // burn
    100, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    90, // defenseDown
    60, // allergic
    70, // fright
    95, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Body part of the Ruff Puff, model c_kmoon: its one target part. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_kurokumorn", "c_kmoon", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 30, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
};

/** Weapon data for the Ruff Puff's body slam: 4 damage, one target. */
static BattleWeapon weapon_kurokumorn_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    COUNTER_RESIST_ELECTRIC, // counterResistFlags
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

/** Weapon data for the Ruff Puff's thunder blast: 4 damage, electric element, every target. */
static BattleWeapon weapon_kurokumorn_thunder_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ELECTRIC, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_FRONT, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, // poisonStrength..electricChance
    3, // electricTime
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyChance..sizeChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeStrength..allergicChance
    0, 0, 0, 0, 0, 0, 0, 0, // allergicTime..frightChance
    0, 0, 0, 0, 0, 0, 0, 0, // galeForceChance..fpRegenTime
    0, // fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Turn script of the Ruff Puff.
 * After an item-use check, if it is charged (electric counter attribute 1024 on part 1) it fires the
 * lightning attack through thunder_event.
 * Otherwise it rolls 0 to 99: below 60 it makes a normal attack,
 * otherwise it charges up through charge_event.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterElectric, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(9), PTR(&weapon_kurokumorn_thunder_attack))
        CALL_CHILD(PTR(thunder_event))
        GOTO(99)
    END_IF
    SET_VALUE(LW(0), 60)
    ADD(LW(0), 40)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 60)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_kurokumorn_attack))
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    CALL_CHILD(PTR(charge_event))
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Ruff Puff's lightning attack, used while it is charged.
 * It samples a target, declares an attack on every opponent,
 * plays the charge sound and the A_2B/A_3A/A_3B animations,
 * drops its charge (clears counter attribute 1024)
 * and rains pokopi_thunder_n64 lightning bolts on the player's side,
 * with the damage decided by the pre-damage check and CheckDamage per target.
 */
static EVT_BEGIN(thunder_event)
    // The thunder reaches every opposing unit: a target is chosen only to aim at.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))

    // No valid target: a confused Ruff Puff is flustered, otherwise the turn is skipped.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The attack is declared on all targets and the guard window opens.
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // Charge-up: the charge sound and poses play while it faces the first target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    WAIT_FRAMES(30)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3A"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3B"))

    // It lets its charge go and fires.
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterElectric)

    // Four pairs of lightning bolts rain down on the opposing side one after another.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetEnemyBelong, LW(3), LW(10))
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LW(10), 125)
        ELSE
            SET_VALUE(LW(10), -125)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_CHARGE_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(0), LW(10))
        SET_VALUE(LW(2), 0)
        ADD(LW(0), -60)
        ADD(LW(2), -60)
        CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 0, LW(0), 210, LW(2), LW(0), 0, LW(2), FLOAT(4.0f), 20, 0, 0, 0)
        SET_VALUE(LW(0), LW(10))
        SET_VALUE(LW(2), 0)
        ADD(LW(0), 60)
        ADD(LW(2), 60)
        CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 0, LW(0), 210, LW(2), LW(0), 0, LW(2), FLOAT(4.0f), 20, 0, 0, 0)
        WAIT_FRAMES(12)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_CHARGE_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(0), LW(10))
        SET_VALUE(LW(2), 0)
        ADD(LW(0), 0)
        ADD(LW(2), -75)
        CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 0, LW(0), 210, LW(2), LW(0), 0, LW(2), FLOAT(4.0f), 20, 0, 0, 0)
        SET_VALUE(LW(0), LW(10))
        SET_VALUE(LW(2), 0)
        ADD(LW(0), 0)
        ADD(LW(2), 75)
        CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 0, LW(0), 210, LW(2), LW(0), 0, LW(2), FLOAT(4.0f), 20, 0, 0, 0)
        WAIT_FRAMES(12)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_CHARGE_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(0), LW(10))
        SET_VALUE(LW(2), 0)
        ADD(LW(0), 60)
        ADD(LW(2), -60)
        CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 0, LW(0), 210, LW(2), LW(0), 0, LW(2), FLOAT(4.0f), 20, 0, 0, 0)
        SET_VALUE(LW(0), LW(10))
        SET_VALUE(LW(2), 0)
        ADD(LW(0), -60)
        ADD(LW(2), 60)
        CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 0, LW(0), 210, LW(2), LW(0), 0, LW(2), FLOAT(4.0f), 20, 0, 0, 0)
        WAIT_FRAMES(12)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_CHARGE_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SET_VALUE(LW(0), LW(10))
        SET_VALUE(LW(2), 0)
        ADD(LW(0), 75)
        ADD(LW(2), 0)
        CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 0, LW(0), 210, LW(2), LW(0), 0, LW(2), FLOAT(4.0f), 20, 0, 0, 0)
        SET_VALUE(LW(0), LW(10))
        SET_VALUE(LW(2), 0)
        ADD(LW(0), -75)
        ADD(LW(2), 0)
        CALL(evt_eff64, STRING(""), STRING("pokopi_thunder_n64"), 0, LW(0), 210, LW(2), LW(0), 0, LW(2), FLOAT(4.0f), 20, 0, 0, 0)
        WAIT_FRAMES(12)
    END_SIBLING_EVENT

    // The camera shakes as the bolts land.
    WAIT_FRAMES(10)
    CALL(evt_btl_camera_shake_h, 0, 5, 0, 30, 0)
    SET_VALUE(LW(6), 0)
    LABEL(0)

    // Each target in turn gets the hit check and then the damage; the action-command result is read once.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        SET_VALUE(LW(6), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(98)
    END_IF
    GOTO(0)
    LABEL(98)

    // It settles back into its idle pose.
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(250)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Pose table of the Ruff Puff: the animation each battle pose (idle, hurt, walk, attack and so on) plays. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "A_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/**
 * Event table of the Ruff Puff: the attacker being shocked by its charge (counter_event) and the First
 * Strike reaction (dmg_first_attack_pos).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_ELECTRIC_EVENT, counter_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0, NULL},
};

/**
 * Init script of the Ruff Puff: installs the wait, unison-phase, attack, damage and confusion handlers,
 * clears its unit work 0 and 1, and starts barrier_event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    SPAWN_EVENT(PTR(barrier_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Background loop on the Ruff Puff (barrier_event): every 31 frames, while it is charged,
 * spawns the akari_charge_n64 spark effect above it; ends when the unit is gone.
 */
static EVT_BEGIN(barrier_event)
    LABEL(0)
    WAIT_FRAMES(31)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterElectric, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(3))
        CALL(evt_eff64, STRING(""), STRING("akari_charge_n64"), 0, LW(0), LW(1), LW(2), LW(3), 30, 0, 0, 0, 0, 0, 0)
    END_IF
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Data-table event 48, run when the battle opens with Mario's or his partner's First Strike on the Ruff Puff:
 * puts it at height 10 and waits for the attack to end.
 */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/** Damage event of the Ruff Puff: runs the default damage reaction (btldefaultevt_Damage) for itself. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/** Counter handler of the Ruff Puff: empty, it does nothing. */
static EVT_BEGIN(counter_event)
    RETURN
EVT_END()

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterElectric, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(evt_sub_random, 99, LW(0))
    IF_INT_LESS(LW(0), 50)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SET_VALUE(LW(1), 40)
            CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, 0)
        ELSE
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SET_VALUE(LW(1), 10)
            CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, 1)
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * The Ruff Puff's body slam with the weapon in LW(9): picks a target (the first-attack target on a pre-emptive
 * strike), shakes and squashes itself as a wind-up, lunges back and then dives into the target, which dodges,
 * guards or takes the hit (pre-damage check and action-command defence); then it bounces off and floats home.
 */
static EVT_BEGIN(normal_attack_event)
    // Choose the target: the First Strike victim, otherwise a sampled opposing unit.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF

    // No valid target: a confused unit is flustered, otherwise the turn is skipped.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The attack is declared and the guard window opens.
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))

    // It turns to the target while the camera follows.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)

    // Wind-up: it rattles from side to side while shrinking.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        LOOP(10)
            CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 1, 0, 0)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, -1, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
        CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(40)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.009765625f), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT

    // It springs up and back, swelling to full size again.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2A"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), -40)
    SET_VALUE(LW(1), 60)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))

    // It dives at the target in a swooping arc.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(10), LW(11), LW(12), 40, -40, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_snd_sfxoff, LW(15))

    // Hit check: a miss or dodge goes on to the fly-by, a hit to the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        SET_VALUE(LW(15), 1)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(98)
    LABEL(91)

    // Hit: the camera shakes and the damage is dealt with the action-command result.
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)
    LABEL(98)

    // It rebounds up off the target and floats back to its home position.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(10), 40)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(10), LW(1), LW(12), 40, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE3"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 40, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(250)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, 1, 1, 1)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Charge-up: the camera zooms on the Puff, a charge sound plays
 * and it becomes charged (electric counter attribute 1024 on part 1) with its A_2A/A_2B charge animations.
 */
static EVT_BEGIN(charge_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterElectric)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2A"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    WAIT_FRAMES(60)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(600)
    LABEL(99)
    RETURN
EVT_END()

/** Wait script of the Ruff Puff: shows its idle pose from the pose table and resets its scale to 1. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, 1, 1, 1)
    RETURN
EVT_END()


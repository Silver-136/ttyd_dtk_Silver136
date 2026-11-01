// Battle unit Putrid Piranha (`poison_pakkun`): tst.rel's copy of rels/muj/src/unit/unit_poison_pakkun.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "tst_battle.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "stdlib.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "eff/n64/eff_vapor_n64.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "system.h"

static USER_FUNC(shadow_scale_memory);
static USER_FUNC(shadow_scale_return);
static USER_FUNC(shadow_scale_small);
static USER_FUNC(pakkun_ceil_exist);
static USER_FUNC(eff_poison_breath);

BattleUnitKind unit_poison_pakkun;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
BattleWeapon weapon_poison_pakkun_attack;
BattleWeapon weapon_poison_pakkun_breath_attack;
static EvtWord breath_event[369];
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_ceil[18];
static BattleDataEntry data_table[15];
static EvtWord init_event[31];
static EvtWord dmg_avoid_counter_thorn_head_event[71];
static EvtWord damage_event[10];
static EvtWord counter_damage_event[30];
#if VERSION <= VERSION_DEMO_US
static EvtWord return_event[244];
#else
static EvtWord return_event[256];
#endif
static EvtWord normal_attack_event[311];
static EvtWord wait_event[6];

/**
 * Unit definition of the Putrid Piranha: its HP, level, sizes, sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_poison_pakkun = {
    88,
    "btl_un_poison_pakkun",
    8,
    0,
    1,
    1,
    20,
    0,
#if VERSION <= VERSION_DEMO_US
    2,
    70,
    1,
#else
    3,
    70,
    0,
#endif
    60,
    9999,
    40,
    48,
    4,
    48,
    0.0f,
    24.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    0.0f,
    16.0f,
    32.0f,
    0.0f,
    0.0f,
    24.0f,
    0.0f,
    40.0f,
    48.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_PAKKUN_DAMAGED1",
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

/** Defence of the Putrid Piranha: none, every hit does its full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of the Putrid Piranha: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of the Putrid Piranha in the demo builds: immune to poison; resists most other statuses.
 */
static StatusVulnerability regist = {90, 90, 70, 0, 85, 100, 100, 90, 100, 80, 100, 80, 100, 80, 80, 30, 40, 100, 80, 100, 100, 95};
#else
/**
 * Status vulnerability of the Putrid Piranha in the retail builds: immune to poison; extra prone to sleep;
 * resists most other statuses.
 */
static StatusVulnerability regist = {105, 80, 50, 0, 95, 100, 100, 70, 100, 90, 100, 90, 100, 95, 80, 20, 40, 100, 80, 100, 100, 95};
#endif

/** Body part of the Putrid Piranha: a single part using the c_pakflwr_p model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_poison_pakkun", "c_pakflwr_p", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, kCounterTopSpiky, pose_table},
};

/** Attack data of the Putrid Piranha's bite: 3 damage. */
BattleWeapon weapon_poison_pakkun_attack = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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
 * Attack data of the Putrid Piranha's poison breath: 2 damage; 50% chance of poison for 10 turns;
 * pierces defence.
 */
BattleWeapon weapon_poison_pakkun_breath_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#if VERSION <= VERSION_DEMO_US
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..dizzyTime
    50, // poisonChance
    10, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Attack script of the Putrid Piranha: in a battle-flag-2 battle it bites (normal_attack_event with
 * weapon_poison_pakkun_attack, 3 damage).
 * Otherwise, after the item-use check, it bites 60 percent of the time
 * and else uses its poison breath (breath_event).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_poison_pakkun_attack))
        CALL_CHILD(normal_attack_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_poison_pakkun_attack))
        CALL_CHILD(normal_attack_event)
        GOTO(99)
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        SET_VALUE(LW(0), 60)
        ADD(LW(0), 40)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 60)
            SET_VALUE(LW(9), PTR(&weapon_poison_pakkun_attack))
            CALL_CHILD(normal_attack_event)
            GOTO(99)
        END_IF
        CALL_CHILD(breath_event)
        GOTO(99)
        LABEL(99)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
EVT_END()

/**
 * The Putrid Piranha's poison breath (weapon_poison_pakkun_breath_attack: 2 damage,
 * 50 percent poison for 10 turns): it declares the attack on every sampled target,
 * sinks and resurfaces at x = -40 facing them, winds up (PKF_S_2 with mouth sounds) and breathes (PKF_A_3A, 3B,
 * the poison sound and 20 puffs of eff_poison_breath).
 * Each target in turn is checked and, if hit, damaged (only the first applies the guard result).
 * It then closes its mouth (PKF_A_3C) and burrows back with return_event.
 */
static EVT_BEGIN(breath_event)
    // First-strike battle: the target is already fixed; otherwise one is picked from the opposing party.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_poison_pakkun_breath_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_poison_pakkun_breath_attack), LW(3), LW(4))

    // No target to attack: a confused Putrid Piranha just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_poison_pakkun_breath_attack))

    // The breath is declared against every target.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_poison_pakkun_breath_attack))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        // It sinks into the ground, its shadow shrinking.
        CALL(shadow_scale_small, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(15))
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_4"))
    END_IF
    WAIT_MS(300)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))

    // It resurfaces at x = -40, facing its targets.
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, -40, 0, LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_return, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_1"))
    WAIT_MS(500)
    BEGIN_SIBLING_EVENT
        LOOP(3)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOUTH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(20)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(15))

    // It winds up with a wheeze.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_S_2"))
    WAIT_MS(1000)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_POISON1"), EVT_NULLPTR, 0, EVT_NULLPTR)

    // It breathes out the poison breath: twenty puffs of it.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_3A"))
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_3B"))
    WAIT_MS(300)
    CALL(evt_btl_camera_set_mode, 0, 0)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetEnemyBelong, LW(3), LW(10))
        IF_INT_EQUAL(LW(10), 0)
            LOOP(20)
                CALL(eff_poison_breath, 1)
                WAIT_FRAMES(3)
            END_LOOP
        ELSE
            LOOP(20)
                CALL(eff_poison_breath, 0)
                WAIT_FRAMES(3)
            END_LOOP
        END_IF
    END_SIBLING_EVENT

    // Every target is checked in turn.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)

    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_poison_pakkun_breath_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // A miss: this target dodges the breath.
    LABEL(90)
    GOTO(97)

    // A hit: this target takes the damage and may be poisoned.
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_poison_pakkun_breath_attack))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_poison_pakkun_breath_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)

    // On to the next target in line.
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    WAIT_MS(1000)
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(15))

    // It closes its mouth and burrows back to its place.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_3C"))
    WAIT_MS(200)
    CALL_CHILD(return_event)

    // Turn finished: the Putrid Piranha goes back to waiting.
    LABEL(99)
    RETURN
EVT_END()

/**
 * Pose table of the Putrid Piranha: maps each battle pose (waiting, hurt, running...) to one of its PKF_*
 * animations.
 */
static PoseTableEntry pose_table[] = {
    {1, "PKF_N_1"},
    {2, "PKF_Y_1"},
    {9, "PKF_Y_1"},
    {5, "PKF_K_1"},
    {4, "PKF_X_1"},
    {3, "PKF_X_1"},
    {BTL_POSE_NORMAL, "PKF_S_1"},
    {29, "PKF_Q_1"},
    {30, "PKF_Q_1"},
    {31, "PKF_S_2"},
    {BTL_POSE_DAMAGE, "PKF_D_1"},
    {50, "PKF_A_1B"},
    {POSE_TYPE_WALK, "PKF_S_1"},
    {POSE_TYPE_RUN, "PKF_S_1"},
    {POSE_TYPE_DANGER, "PKF_X_1"},
    {POSE_TYPE_PERIL, "PKF_X_1"},
    {POSE_TYPE_TALK, "PKF_T_1"},
    {POSE_TYPE_ACTION, "PKF_S_1"},
};

/**
 * Pose table of the Putrid Piranha while it clings to the ceiling:
 * maps each battle pose (waiting, hurt, running...) to one of its PKF_* animations.
 */
static PoseTableEntry pose_table_ceil[] = {
    {1, "PKF_N_2"},
    {2, "PKF_Y_2"},
    {9, "PKF_Y_2"},
    {5, "PKF_K_2"},
    {4, "PKF_X_2"},
    {3, "PKF_X_2"},
    {BTL_POSE_NORMAL, "PKF_S_3"},
    {29, "PKF_Q_2"},
    {30, "PKF_Q_2"},
    {31, "PKF_S_4"},
    {BTL_POSE_DAMAGE, "PKF_D_2"},
    {50, "PKF_A_5"},
    {POSE_TYPE_WALK, "PKF_S_3"},
    {POSE_TYPE_RUN, "PKF_S_3"},
    {POSE_TYPE_DANGER, "PKF_X_2"},
    {POSE_TYPE_PERIL, "PKF_X_2"},
    {POSE_TYPE_TALK, "PKF_S_3"},
    {POSE_TYPE_ACTION, "PKF_S_3"},
};

/**
 * Data table of the Putrid Piranha: reaction scripts for an attacker hurt by its spikes
 * and taking a hit with a particular damage pattern; a First Strike on it triggers nothing special.
 */
static BattleDataEntry data_table[] = {
    {37, dmg_avoid_counter_thorn_head_event},
    {26, counter_damage_event},
    {25, counter_damage_event},
    {27, counter_damage_event},
    {28, counter_damage_event},
    {29, counter_damage_event},
    {30, counter_damage_event},
    {31, counter_damage_event},
    {32, counter_damage_event},
    {33, counter_damage_event},
    {34, counter_damage_event},
    {35, counter_damage_event},
    {36, counter_damage_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Init script of the Putrid Piranha: installs the wait, attack,
 * damage and confusion event handlers (confusion reuses the attack),
 * sets the hit cursor offset of part 1 and saves its shadow size with shadow_scale_memory.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 50, 0)
    CALL(shadow_scale_memory, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Data-table event 37, run by the default damage event when an attacker is hurt by the Putrid Piranha's spiky head
 * (damage code 31): it flinches (pose type 31, 250 ms), returns to its idle pose PKF_S_1 and flashes the
 * "toge_flush" spike effect above its head, 50 units up (63 when Huge, 37 when Tiny).
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 31)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 63)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 37)
        GOTO(90)
    END_IF
    ADD(LW(1), 50)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

/** Damage handler: runs the shared default damage script (btldefaultevt_Damage) for itself, part 1. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Data-table events 25 to 36: the Putrid Piranha is hurt by a counter (spikes, fire, ice, electricity,
 * explosions, poison, Payback, Hold Fast) while biting.
 * It shows its damage pose (PKF_D_1) for a second and,
 * unless the counter knocked it out (KO flag 0x200 without the Life Shroom flag 0x800),
 * runs return_event so it burrows back to its spot.
 */
static EVT_BEGIN(counter_damage_event)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_D_1"))
    WAIT_MS(1000)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL_CHILD(return_event)
    RETURN
EVT_END()

/**
 * Ceiling-fall script, installed by return_event while the Putrid Piranha hangs from the ceiling and run
 * when the stage's overhead iron frame falls (_fall_frame_event): it clears itself,
 * sinks into the ceiling (PKF_E_4 with the move sound, shadow shrunk, hidden),
 * moves its position and home to its home x/z on the ground (y = 0),
 * then pops back up standing (PKF_E_1, then PKF_S_1) with the ground hit, cursor, status-icon and kiss offsets,
 * the ceiling attributes cleared and the ground pose table.
 */
static EVT_BEGIN(ceil_fall_ready_event)
    CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_small, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_4"))
    WAIT_MS(300)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_return, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_1"))
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 50, 0)
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 4, 48)
#if VERSION >= VERSION_JP // the JP demo has no kiss move offsets
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, 32, 0)
#endif
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HAMMERLIKE_CANNOT_TARGET)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_S_1"))
    RETURN
EVT_END()

/**
 * Burrow back after an attack (run by normal_attack_event and counter_damage_event):
 * the Putrid Piranha sinks (PKF_E_2 with the move sound, shadow shrunk, hidden).
 * If the stage has a ceiling (pakkun_ceil_exist) it picks ground or ceiling at random (half each),
 * else the ground.
 * On the ground it reappears at its home x/z at y = 0 standing (PKF_E_1 then PKF_S_1,
 * ground offsets and pose table, ceiling attributes off, no ceiling-fall script);
 * on the ceiling it reappears at y = 130 hanging upside down (PKF_E_3 then PKF_S_3,
 * flipped hit and cursor offsets, ceiling unit attribute and the two part attributes on,
 * the ceiling pose table) with ceil_fall_ready_event as its ceiling-fall script.
 */
static EVT_BEGIN(return_event)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(pakkun_ceil_exist, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_sub_random, 1, LW(15))
    ELSE
        SET_VALUE(LW(15), 0)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_small, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_2"))
    WAIT_MS(300)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
        CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
        WAIT_MS(500)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        BEGIN_SIBLING_EVENT
            CALL(shadow_scale_return, UNIT_ID_SELF)
        END_SIBLING_EVENT
        CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_1"))
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 50, 0)
        CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 4, 48)
#if VERSION >= VERSION_JP
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, 32, 0)
#endif
        CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HAMMERLIKE_CANNOT_TARGET)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
        WAIT_MS(500)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_S_1"))
    ELSE
        CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, PTR(ceil_fall_ready_event))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 130, LW(2))
        CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 130, LW(2))
        WAIT_MS(500)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        BEGIN_SIBLING_EVENT
            CALL(shadow_scale_return, UNIT_ID_SELF)
        END_SIBLING_EVENT
        CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitHidden)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_3"))
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, -80, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, -50, 0)
        CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 4, 10)
#if VERSION >= VERSION_JP
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, -32, 0)
#endif
        CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kCeiling)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HAMMERLIKE_CANNOT_TARGET)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_ceil))
        WAIT_MS(500)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_S_3"))
    END_IF
    RETURN
EVT_END()

/**
 * The Putrid Piranha's bite with the weapon in LW(9), at the First Strike target in a battle-flag-2 battle,
 * else a sampled enemy: it sinks (from the ground or the ceiling),
 * resurfaces on the ground 45 units in front of the target,
 * faces it and bites (PKF_A_1A to 1C with the attack sound).
 * A miss does nothing more; a hit shakes the camera and applies the guard result and the damage.
 * Then it burrows back with return_event.
 */
static EVT_BEGIN(normal_attack_event)
    // First-strike battle: the target is already fixed; otherwise one is picked from the opposing party.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF

    // No target to attack: a confused Putrid Piranha just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        // It sinks out of sight, from the ground or the ceiling.
        CALL(shadow_scale_small, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(15))
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_4"))
    END_IF
    WAIT_MS(300)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 45)

    // It resurfaces on the ground in front of the target.
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_return, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_1"))
    WAIT_MS(150)

    // It rears up and bites, with the attack sound.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_1A"))
    WAIT_MS(200)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_1B"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_1C"))

    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // A miss: the target dodges the bite.
    LABEL(90)
    GOTO(98)

    // A hit: the camera shakes and the target takes the damage.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)
    LABEL(98)
    WAIT_MS(500)

    // It burrows back to its place.
    CALL_CHILD(return_event)

    // Turn finished: the Putrid Piranha goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle script: sets the standard waiting pose from the unit's pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Script helper: grows the body part's shadow scale back from 0 to the value kept by shadow_scale_memory over 15
 * frames (blocks until done).
 */
static USER_FUNC(shadow_scale_return) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);

    if (isFirstCall) {
        unit->work[1] = 0;
    }
    part->shadowScale = intplGetValue(0, 0.0f, unit->work[0], unit->work[1], 15);
    if (unit->work[1] >= 15) {
        return EVT_RETURN_DONE;
    }
    unit->work[1]++;
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: shrinks the body part's shadow from its remembered scale to 0 over 15 frames (blocks until
 * done), so the shadow vanishes while the plant is underground.
 */
static USER_FUNC(shadow_scale_small) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);

    if (isFirstCall) {
        unit->work[1] = 0;
    }
    part->shadowScale = intplGetValue(0, unit->work[0], 0.0f, unit->work[1], 15);
    if (unit->work[1] >= 15) {
        return EVT_RETURN_DONE;
    }
    unit->work[1]++;
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: remembers the body part's current shadow scale in work[0] for the two helpers that shrink
 * and restore it.
 */
static USER_FUNC(shadow_scale_memory) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);

    unit->work[0] = part->shadowScale;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: writes 0 to args[0] when the stage's iron frame is gone (STAGE_OBJ_FLAG_IRON_FRAME_GONE) and 1
 * while the ceiling still exists, so the plant knows whether it can hang from it.
 */
static USER_FUNC(pakkun_ceil_exist) {
    EvtWord* args = event->args;
#if VERSION <= VERSION_DEMO_US
    if (_battleWorkPointer->stageObjectWork.flags & STAGE_OBJ_FLAG_IRON_FRAME_GONE) {
#else
    if (TST_BATTLE_WORK(_battleWorkPointer)->stageObjectWork.flags & STAGE_OBJ_FLAG_IRON_FRAME_GONE) {
#endif
        evtSetValue(event, args[0], 0);
    } else {
        evtSetValue(event, args[0], 1);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: puffs one green vapour effect at a random spot on one side of the stage (x about -125 when
 * args[0] is 0, +125 otherwise), for the poison breath cloud.
 */
static USER_FUNC(eff_poison_breath) {
    f32 x;
    f32 y;
    f32 z;
    EffectEntry* eff;
    EffVaporN64Work* wk;

    if (evtGetValue(event, event->args[0]) == 0) {
        x = -125.0f;
    } else {
        x = 125.0f;
    }
    x += irand(150) - 75;
    y = irand(70);
    z = irand(80) - 40;
    eff = effVaporN64Entry(0, 15, x, y, z, 2.5f + rand() / 32767.0f);
    wk = eff->userdata;
    wk->r1 = 32;
    wk->g1 = 115;
    wk->b1 = 100;
    wk->alpha = 255;
    wk->r2 = 32;
    wk->g2 = 115;
    wk->b2 = 100;
    wk->a2 = 255;
    return EVT_RETURN_DONE;
}

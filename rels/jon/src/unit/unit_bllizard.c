// Pit of 100 Trials battle unit: the Ice Puff (`bllizard`): parts, stats, poses and AI scripts.

#include "version.h"
#include "battle/battle_unit.h"
#include "eff/n64/eff_kumokumo_n64.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"
#include "battle/unit/unit_bllizard.h"

static USER_FUNC(eff_ice_barrier);
static USER_FUNC(eff_ice_barrier_end);

BattleUnitKind unit_bllizard;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_bllizard_attack;
static BattleWeapon weapon_bllizard_bllizard_attack;
static EvtWord attack_event[69];
static EvtWord bllizard_event[361];
static PoseTableEntry pose_table[17];
static BattleDataEntry data_table[3];
static EvtWord init_event[37];
static EvtWord barrier_event[37];
static EvtWord dmg_first_attack_pos[19];
static EvtWord damage_event[36];
static EvtWord counter_event[2];
static EvtWord unison_phase_event[140];
static EvtWord normal_attack_event[386];
static EvtWord charge_event[58];
static EvtWord wait_event[12];

/** Unit data of the Ice Puff: stats, sounds, attributes, parts, init script and data table. */
BattleUnitKind unit_bllizard = {
    UNIT_ICE_PUFF, // unitType
    "btl_un_bllizard", // name
    9, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    25, // level
    0, // bonusExp
    4, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
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

/** Damage reduction of the Ice Puff: none, every attack type does full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element response of the Ice Puff: it takes extra damage from fire and is healed by ice. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_WEAK, DEFENSE_ATTR_ABSORB, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {70, 70, 80, 100, 60, 0, 100, 0, 100, 60, 100, 60, 100, 70, 60, 70, 80, 100, 60, 100, 100, 95};
#else
static StatusVulnerability regist = {
    65, // sleep
    65, // stop
    95, // dizzy
    100, // poison
    60, // confuse
    0, // electric
    100, // burn
    0, // freeze
    100, // huge
    75, // tiny
    100, // attackUp
    75, // attackDown
    100, // defenseUp
    90, // defenseDown
    65, // allergic
    70, // fright
    85, // galeForce
    100, // fast
    65, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Body part of the Ice Puff: hit position, size, defence tables and attributes. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_bllizard", "c_kmoon_b", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 30, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
};

/** Attack data for the Ice Puff's ramming lunge: 4 damage. */
static BattleWeapon weapon_bllizard_attack = {
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
    COUNTER_RESIST_ICY, // counterResistFlags
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

/** Attack data for the Ice Puff's icy breath on the whole party: 5 damage. */
static BattleWeapon weapon_bllizard_bllizard_attack = {
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
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ICE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_FRONT, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, // burnTime
#if VERSION >= VERSION_US
    60, // freezeChance
#else
    30, // freezeChance
#endif
    2, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Attack script of the Ice Puff.
 * After an item check, if its ice barrier is up (counter attribute 64 on part 1) it unleashes the icy breath on
 * all enemies (bllizard_event).
 * Otherwise it picks between the normal ramming attack (70 in 100)
 * and charging up a new ice barrier (charge_event).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterIcy, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(9), PTR(&weapon_bllizard_bllizard_attack))
        CALL_CHILD(bllizard_event)
        GOTO(99)
    END_IF
    SET_VALUE(LW(0), 70)
    ADD(LW(0), 30)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 70)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_bllizard_attack))
        CALL_CHILD(normal_attack_event)
        GOTO(99)
    END_IF
    CALL_CHILD(charge_event)
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Ice Puff's icy breath: it drops the barrier, floats to the left of the stage,
 * charges for 60 frames and blows a cloud of ice ("kumokumo_n64" effect, scaled to its size) over the party.
 * Every target is checked in turn and takes the damage
 * if the breath reaches it (the defence action command result is read once, at the first hit).
 * It then returns home.
 */
static EVT_BEGIN(bllizard_event)

    // Pick the party as the target; a confused Ice Puff with nobody to hit gets flustered.
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))

    // Declare the attack on every enemy; the Ice Puff drops its ice barrier.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterIcy)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))

    // It floats off to the left side of the stage.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE3"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -40, 15, LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // It charges up its icy breath.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BLIZ_KUMO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    WAIT_FRAMES(60)

    // It blows a huge cloud of ice across the stage toward the party.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BLIZ_KUMO_CHARGE_ATTACK1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(5))
    SET_VALUE(LW(6), LW(5))
    MULTIPLY_FLOAT(LW(6), FLOAT(5.0f))
    ADD(LW(1), LW(6))
    MULTIPLY_FLOAT(LW(5), FLOAT(2.0f))
    CALL(btlevtcmd_GetEnemyBelong, LW(3), LW(10))
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_eff64, STRING(""), STRING("kumokumo_n64"), 4, LW(0), LW(1), LW(2), 0, 0, LW(5), 140, 0, 0, 0, 0)
    ELSE
        CALL(evt_eff64, STRING(""), STRING("kumokumo_n64"), 4, LW(0), LW(1), LW(2), 180, 0, LW(5), 140, 0, 0, 0, 0)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3A"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3B"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(20)
    SET_VALUE(LW(6), 0)

    // Check each party member in turn; those the breath reaches take the damage.
    LABEL(0)
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

    // The breath dies down and the Ice Puff floats back home.
    LABEL(98)
    WAIT_FRAMES(60)
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE3"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(250)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Pose table of the Ice Puff: battle pose types mapped to animation names. */
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

/** Data table of the Ice Puff: event handlers keyed by tag (counter_event, dmg_first_attack_pos). */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_ICE_EVENT, counter_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0, NULL},
};

/**
 * Init script of the Ice Puff: registers its wait, unison-phase, attack, damage and confusion handlers,
 * clears unit work 0 and 1 and starts barrier_event to keep the ice barrier effect in sync with its state.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BARRIER_EFFECT, 0)
    SPAWN_EVENT(barrier_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Background loop of the Ice Puff, run for as long as it exists:
 * each frame it shows the ice barrier effect while part 1 carries counter attribute 64 and removes the effect
 * when the attribute is gone.
 */
static EVT_BEGIN(barrier_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterIcy, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(eff_ice_barrier, UNIT_ID_SELF)
    ELSE
        CALL(eff_ice_barrier_end, UNIT_ID_SELF)
    END_IF
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/** Data event 48 of the Ice Puff: after a first strike it is set to y 10 and waits for the attack to end. */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Damage script of the Ice Puff: a hit with DAMAGE_RESULT_ZERO_HP set and DAMAGE_RESULT_REVIVAL clear breaks its ice barrier (clears counter
 * attribute 64 and removes the effect) before the default damage handling runs.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterIcy)
            CALL(eff_ice_barrier_end, UNIT_ID_SELF)
        END_IF
    END_IF
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Empty counter script (just returns). */
static EVT_BEGIN(counter_event)
    RETURN
EVT_END()

/**
 * Before the player's phase, if the Ice Puff can act and its barrier is down,
 * it has a 50 percent chance to swap height: from the low position (unit work 0 set) it rises to y 40
 * and sets part attributes 0x600000, otherwise it drops to y 10 and clears them.
 * The new height becomes its home position.
 */
static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterIcy, LW(0))
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
 * The Ice Puff's ramming attack: it picks a target (the one hit first on a first strike),
 * shrinks while rattling, rises and lunges into the target.
 * A hit deals damage with a small camera shake, a miss or dodge ends with the lunge;
 * either way it floats back up and home.
 */
static EVT_BEGIN(normal_attack_event)

    // Pick the target (the First Strike target or a sampled enemy); a confused Ice Puff
    // with nobody to hit gets flustered.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))

    // Declare the attack.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)

    // It shakes from side to side and slowly shrinks, winding up.
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

    // It pulls back and lunges at the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2A"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), -40)
    SET_VALUE(LW(1), 60)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(10), LW(11), LW(12), 40, -40, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_snd_sfxoff, LW(15))

    // Find out whether the lunge connects.
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

    // Hit: the damage is dealt.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)

    // It floats back up and home, puffing back out to full size.
    LABEL(98)
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
 * Script helper: args[0] = unit.
 * Keeps the cold cloud effect (a Kumokumo particle,
 * created on the first call and stored in work[1]) hovering at the Ice Puff's position, scaled by its size.
 */
static USER_FUNC(eff_ice_barrier) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    EffectEntry* eff;
    f32 x, y, z;

    if (isFirstCall && unit->work[UNIT_WORK_BARRIER_EFFECT] == 0) {
        unit->work[UNIT_WORK_BARRIER_EFFECT] = (EvtWord)effKumokumoN64Entry(3, 1000, 0.0f, -1000.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    }
    eff = (EffectEntry*)unit->work[UNIT_WORK_BARRIER_EFFECT];
    BtlUnit_GetPos(unit, &x, &y, &z);
    ((EffKumokumoN64Particle*)eff->userdata)->x = x;
    ((EffKumokumoN64Particle*)eff->userdata)->y = 10.0f * unit->sizeMultiplier + y;
    ((EffKumokumoN64Particle*)eff->userdata)->z = z;
    ((EffKumokumoN64Particle*)eff->userdata)->scale = unit->sizeMultiplier;
    return EVT_RETURN_DONE;
}

/** Script helper: args[0] = unit. Deletes the Ice Puff's cloud effect (work[1]) if it exists. */
static USER_FUNC(eff_ice_barrier_end) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (unit->work[UNIT_WORK_BARRIER_EFFECT] != 0) {
        effDelete((EffectEntry*)unit->work[UNIT_WORK_BARRIER_EFFECT]);
        unit->work[UNIT_WORK_BARRIER_EFFECT] = 0;
    }
    return EVT_RETURN_DONE;
}

/**
 * The Ice Puff raises an ice barrier: it sets counter attribute 64 on part 1,
 * plays the charge pose and sound for 80 frames and goes back to its idle pose.
 */
static EVT_BEGIN(charge_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BLIZ_KUMO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterIcy)
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

/** Wait script: plays the Ice Puff's idle pose from its pose table and resets its scale. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, 1, 1, 1)
    RETURN
EVT_END()


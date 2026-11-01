// Pit of 100 Trials battle unit: the Poison Puff (`dokugassun`): parts, stats, poses and AI scripts.

#include "version.h"
#include "eff/n64/eff_vapor_n64.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "system.h"
#include "stdlib.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LOWERED 0 // 1 while it hangs low (y 10, hammer and shell can reach it), 0 while raised (y 40); unison_phase_event flips it at random (25 %)

static USER_FUNC(eff_poison_barrier);
static USER_FUNC(eff_poison_breath);

BattleUnitKind unit_dokugassun;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_dokugassun_attack;
static BattleWeapon weapon_dokugassun_poisongas_attack;
static EvtWord attack_event[69];
#if VERSION <= VERSION_DEMO_US
static EvtWord poisongas_event[274];
#else
static EvtWord poisongas_event[303];
#endif
static PoseTableEntry pose_table[17];
static BattleDataEntry data_table[3];
static EvtWord init_event[37];
static EvtWord barrier_event[33];
static EvtWord dmg_first_attack_pos[19];
static EvtWord damage_event[10];
static EvtWord counter_event[2];
static EvtWord unison_phase_event[140];
static EvtWord normal_attack_event[386];
static EvtWord charge_event[58];
#if VERSION <= VERSION_DEMO_US
static EvtWord wait_event[12];
#else
static EvtWord wait_event[13];
#endif

/** Unit data of the Poison Puff: stats, sounds, attributes, parts, init script and data table. */
BattleUnitKind unit_dokugassun = {
    UNIT_POISON_PUFF, // unitType
    "btl_un_dokugassun", // name
    15, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    34, // level
    2, // bonusExp
    2, // bonusCoin
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

/** Damage reduction of the Poison Puff: none, every attack type does full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element response of the Poison Puff: none, every attack type is treated normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {70, 70, 80, 0, 60, 100, 100, 60, 100, 60, 100, 60, 100, 60, 60, 60, 80, 100, 60, 100, 100, 20};
#else
static StatusVulnerability regist = {
    65, // sleep
    65, // stop
    90, // dizzy
    0, // poison
    60, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    80, // defenseDown
    50, // allergic
    60, // fright
    85, // galeForce
    100, // fast
    60, // slow
    100, // dodgy
    100, // invisible
    20, // ohko
};
#endif

/** Body part of the Poison Puff: hit position, size, defence tables and attributes. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_dokugassun", "c_kmoon_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 30, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
};

/** Attack data for the Poison Puff's charge: 8 damage. */
static BattleWeapon weapon_dokugassun_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    COUNTER_RESIST_POISON, // counterResistFlags
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

/** Attack data for the Poison Puff's poison gas breath: 10 damage. */
static BattleWeapon weapon_dokugassun_poisongas_attack = {
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
    {10, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_FRONT, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
#if VERSION <= VERSION_DEMO_US
    50,
#else
    70, // poisonChance
#endif
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
 * Poison Puff's turn: after an item-use check,
 * if it has saved up toxins (counter attribute 0x200, poison status, on part 1) it breathes poison gas;
 * otherwise it picks between a normal attack and charging up the barrier at random (a 65 to 35 split, 70 to 30
 * in the demos).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterPoisonStatus, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(9), PTR(&weapon_dokugassun_poisongas_attack))
        CALL_CHILD(PTR(poisongas_event))
        GOTO(99)
    END_IF
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(LW(0), 70)
    ADD(LW(0), 30)
#else
    SET_VALUE(LW(0), 65)
    ADD(LW(0), 35)
#endif
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(LW(0), 70)
#else
    SET_VALUE(LW(0), 65)
#endif
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_dokugassun_attack))
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
 * Poison gas attack (weapon_dokugassun_poisongas_attack, multiple targets): the Puff floats over and exhales,
 * dropping its poison counter attribute (0x200);
 * twenty green vapour clouds spawn on the targets' side of the stage
 * and each target in turn is hit or avoids it by the usual checks; then it floats home.
 */
static EVT_BEGIN(poisongas_event)
    // Pick the party as the targets; a confused Poison Puff with nobody to hit gets flustered.
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
    CALL(btlevtcmd_WeaponAftereffect, LW(9))

    // Declare the attack on every enemy.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
#if VERSION > VERSION_DEMO_US
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE3"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -40, LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
#endif
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // It swells up, charging.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_DOKU_KUMO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    WAIT_FRAMES(60)

    // It exhales its stored poison: twenty green gas clouds roll over the party's side of the stage.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_DOKU_KUMO_CHARGE_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3A"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3B"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterPoisonStatus)
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
    WAIT_FRAMES(10)
    SET_VALUE(LW(6), 0)

    // Check each party member in turn; those the gas reaches take the damage.
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

    // The gas clears and the Poison Puff floats back home.
    LABEL(98)
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

/** Pose table of the Poison Puff: battle pose types mapped to animation names. */
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

/** Data table of the Poison Puff: event handlers keyed by tag (counter_event, dmg_first_attack_pos). */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_POISON_EVENT, counter_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0, NULL},
};

/**
 * Init script of the Poison Puff: installs the wait, unison-phase, attack, damage and confusion handlers,
 * clears its unit work 0 and 1, and starts barrier_event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    SPAWN_EVENT(PTR(barrier_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Background loop on the Poison Puff: every 31 frames, while its poison barrier is up,
 * spawns the green poison cloud around it (eff_poison_barrier); ends when the unit is gone.
 */
static EVT_BEGIN(barrier_event)
    LABEL(0)
    WAIT_FRAMES(31)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterPoisonStatus, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(eff_poison_barrier, UNIT_ID_SELF)
    END_IF
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Data table tag 48, run by the default damage event when the Puff is the target of a first strike (damage code
 * 0x39 set in battle_seq.c): drops it to height 10 and waits for the attack to end.
 */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Damage handler: runs the shared default damage script (btldefaultevt_Damage) with its default parameters
 * (LW(10) = -2, LW(11) = 1).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/** Counter handler of the Poison Puff: empty, it does nothing. */
static EVT_BEGIN(counter_event)
    RETURN
EVT_END()

/**
 * Before the player's phase, if the Puff can act and has no saved-up toxins (counter attribute 0x200), it has a
 * 25 percent chance to change height: from low (unit work 0 = 1) it rises to y 40 and sets part attributes
 * 0x600000 (no hammer or shell-toss hits), otherwise it drops to y 10 and clears them, with a movement sound.
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
    CALL(btlevtcmd_CheckPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterPoisonStatus, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(evt_sub_random, 99, LW(0))
    IF_INT_LESS(LW(0), 25)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SET_VALUE(LW(1), 40)
            CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, 0)
        ELSE
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SET_VALUE(LW(1), 10)
            CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget | kShellTosslikeCannotTarget)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOWERED, 1)
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * The Poison Puff's regular charge (weapon in LW(9)):
 * picks a target (or the first-strike target; when confused with
 * no target it is flustered), floats over and dives into it, the target dodging on a miss,
 * then floats back home.
 */
static EVT_BEGIN(normal_attack_event)
    // Pick the target (after a First Strike the one already chosen, otherwise sampled from the enemies); a
    // confused Poison Puff with nobody to hit gets flustered.
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

    // It shakes and slowly shrinks, winding up.
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

    // Find out whether the charge connects.
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

    // It floats back up and home.
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
 * Puffs three green vapour clouds (staggered start times 10, 20, 30) around the Poison Puff, scaled by its size.
 */
static USER_FUNC(eff_poison_barrier) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    for (i = 0; i < 3; i++) {
        EffectEntry* eff;
        EffVaporN64Work* wk;

        BtlUnit_GetPos(unit, &x, &y, &z);
        x += irand(20) - 10;
        y += irand(30);
        eff = effVaporN64Entry(2, (i + 1) * 10, x, y, z, 0.5f + 0.5f * rand() / 32767.0f);
        wk = eff->userdata;
        wk->r1 = 32;
        wk->g1 = 115;
        wk->b1 = 100;
        wk->alpha = 255;
        wk->r2 = 32;
        wk->g2 = 115;
        wk->b2 = 100;
        wk->a2 = 255;
        wk->scale = unit->sizeMultiplier;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] = 0 for the left side of the stage, otherwise the right.
 * Spawns one large green vapour cloud at a random spot on that side, the attack's poison breath.
 * Called once per cloud.
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

static EVT_BEGIN(charge_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_DOKU_KUMO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterPoisonStatus)
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

/** Wait event of the Poison Puff: switches to its idle pose and resets its scale to 1. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, 1, 1, 1)
    RETURN
#if VERSION <= VERSION_DEMO_US
EVT_END()
#else
EVT_END_PAD()
#endif


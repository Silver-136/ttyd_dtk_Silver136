// Battle unit Poison Puff (ドクガッスン): win.rel's copy.

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

static BattleUnitKind unit_dokugassun;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_dokugassun_attack;
static BattleWeapon weapon_dokugassun_poisongas_attack;
#if VERSION <= VERSION_DEMO_US
static EvtWord attack_event[];
#else
static EvtWord attack_event[69];
#endif
#if VERSION <= VERSION_DEMO_US
#if VERSION <= VERSION_DEMO_US
static EvtWord poisongas_event[];
#else
static EvtWord poisongas_event[274];
#endif
#else
#if VERSION <= VERSION_DEMO_US
static EvtWord poisongas_event[];
#else
static EvtWord poisongas_event[303];
#endif
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
#if VERSION <= VERSION_DEMO_US
static EvtWord wait_event[];
#else
static EvtWord wait_event[12];
#endif
#else
#if VERSION <= VERSION_DEMO_US
static EvtWord wait_event[];
#else
static EvtWord wait_event[13];
#endif
#endif

/**
 * Unit data of the Poison Puff (btl_un_dokugassun): 15 HP, level 34,
 * a floating cloud that slams into a target for 8 damage or raises a poison barrier and then breathes poison
 * gas on everyone. win.rel carries this copy, but no battle group of Boggly Woods places it.
 */
static BattleUnitKind unit_dokugassun = {
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
    {0}, // talkTogeBaseOffset
    {18.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0}, // burnFlameOffset
    0.0f, 0.0f, // vivianOffsetX..vivianOffsetZ
    {17.5f, 0.0f, 0.0f}, // bintaHitOffset
    {17.5f, 19.5f, 0.0f}, // kissHitOffset
    {0.0f, 15.0f, 0.0f}, // cutBaseOffset
    35.0f, // cutWidth
    30.0f, // cutHeight
    0, 0, 0, // turnOrder..swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0}, // starPointDispOffset
    "SFX_ENM_KUMO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    (BattleDataEntry*)data_table, // dataTable
};

/** Flat damage reduction of the Poison Puff against normal, fire, ice, explosion and electric hits: none. */
static BattleUnitDefense defence = {0};

/** Element attributes of the Poison Puff: none, so every element does its normal damage. */
static BattleUnitDefenseAttr defence_attr = {0};

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

/**
 * The Poison Puff's single part: the Puff model (c_kmoon_g), the main target;
 * hammer-like and shell-toss moves cannot target it while it floats high.
 */
static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_dokugassun", // partName
        "c_kmoon_g", // modelName
        {0}, // positionOffset
        {0.0f, 25.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        30, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, // attributes
        0, // counterAttributes
        pose_table, // poseTable
    },
};

/** The Poison Puff's body slam: 8 damage to one opposing target, a jump-like attack that can be guarded. */
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

#if VERSION <= VERSION_DEMO_US
static BattleWeapon weapon_dokugassun_poisongas_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0,
0,
1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {10, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_FRONT, // targetWeightFlags
    0, 0, 0, 0, 0, 0,
50,
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
#else
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
    70, // poisonChance
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
#endif

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
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(poisongas_event)
#else
        CALL_CHILD(PTR(poisongas_event))
#endif
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
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(normal_attack_event)
#else
        CALL_CHILD(PTR(normal_attack_event))
#endif
        GOTO(99)
    END_IF
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(charge_event)
#else
    CALL_CHILD(PTR(charge_event))
#endif
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(poisongas_event)
    // The Puff picks a target from the opposing side; with none it gives up (flustered if confused).
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
    // Declare an attack on every opposing unit and pay the cost.
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_DOKU_KUMO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_DOKU_KUMO_CHARGE_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3A"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3B"))
    // The poison barrier drops and, in a side script,
    // green gas puffs spray across the targets' side of the stage.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterPoisonStatus)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetEnemyBelong, LW(3), LW(10))
        IF_INT_EQUAL(LW(10), ALLIANCE_PLAYER)
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
    LABEL(0)
    // Each target in turn: the pre-check decides whether it avoids the gas;
    // the first hit also resolves the guard result.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
#if VERSION > VERSION_DEMO_US
    // Check defense once, at the first target that reaches the damage branch.
#endif
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
    // After the last target the Puff pauses, drifts back to its home position and returns to its idle pose.
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE3"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_US
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, -1)
#else
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
#endif
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(250)
    LABEL(99)
#if VERSION <= VERSION_DEMO_US
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
#else
    CALL(btlevtcmd_ResetFaceDirection, -2)
#endif
    RETURN
EVT_END()

/**
 * Poses of the Poison Puff: idle, status, hurt, move,
 * charge and talking animations (the names have no prefix).
 */
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
 * Data table of the Poison Puff: event 43 does nothing (counter_event) and event 48 (First Strike)
 * lowers the Puff to height 10.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_POISON_EVENT, counter_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0},
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
 * Data-table event 48, which the default damage script runs
 * when the player strikes first (damage code 0x39 from battle_seq.c):
 * drops the Puff to height 10 so it can be hit and waits for the attack to end.
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
 * Before the player's phase, if the Puff can act and its barrier is down, with a 25% chance it changes height:
 * from high (unit work 0 = 0) it dives to height 10,
 * where hammers and shell tosses can reach it (unit work 0 = 1);
 * from low it rises to height 40 and turns PART_ATTR_HAMMERLIKE_CANNOT_TARGET
 * and PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET back on. The new height becomes its home.
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
 * The Poison Puff's body slam (weapon_dokugassun_attack, 8 damage to one enemy):
 * picks a target (or the first-strike target), floats over, squashes and rises,
 * then dives onto the target, handles guard and miss results and returns home.
 */
static EVT_BEGIN(normal_attack_event)
    // A First Strike aims at the first-strike target;
    // otherwise the Puff picks a target from the opposing side.
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
    // Declare the attack, wait for any guard and pay the cost.
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    // Wind-up: the Puff trembles and shrinks for 40 frames while the camera follows it.
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
    // It rises to height 60 and leans away from the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2A"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), -40)
    SET_VALUE(LW(1), 60)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KUMO_MOVE2"), EVT_NULLPTR, 0, LW(15))
    // Dive onto the target; the pre-check decides whether it is dodged or the hit lands.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(10), LW(11), LW(12), 40, -40, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_snd_sfxoff, LW(15))
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
        SET_VALUE(LW(15), 1)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(98)
    // Hit: the camera shakes, the guard result is resolved and the damage is dealt.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)
    LABEL(98)
    // The Puff floats back up, drifts to its home position and sets its size back to normal.
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
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

/**
 * Script helper: spawns three green vapor puffs around the Poison Puff (args[0]) at random offsets,
 * sized by its size multiplier, as its poison cloud.
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
 * Script helper: spawns one green vapor puff at a random point near x = -125 (args[0] = 0)
 * or x = 125 (otherwise) for the Poison Puff's breath attack.
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


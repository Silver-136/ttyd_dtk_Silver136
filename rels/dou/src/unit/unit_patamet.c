// Parabuzzy (patamet): battle unit used in Pirate's Grotto.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_ON_CEILING 0 // nonzero: starts on the ceiling (init_event2 sets the ceiling attributes, tables and events; setup value, 0 in every dou/jon setup); dropping from the ceiling clears it
#define UNIT_WORK_SPIN_DONE 1  // 1 ends the sibling loop that spins it during attack_event
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that spin (10, 20, then 10)


BattleUnitKind unit_patamet;
static BattleWeapon weapon_patamet_attack;
static BattleUnitDefense defence_pata;
static BattleUnitDefenseAttr defence_attr_pata;
static StatusVulnerability regist_pata;
static PoseTableEntry pose_table_pata[18];
static BattleDataEntry data_table_pata[3];
static BattleUnitKindPart parts_pata[1];
static PoseSoundTiming pose_sound_timing_table[8];
static EvtWord init_event_pata[26];
static EvtWord damage_event_pata[10];
static EvtWord dmg_fall_event_pata[106];
static EvtWord dmg_first_attack_pos_event_pata[19];
static EvtWord attack_event_pata[339];
static EvtWord wait_event_pata[6];
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_ceil[18];
#if VERSION <= VERSION_DEMO_US
static PoseTableEntry pose_table_turn[15];
#else
static PoseTableEntry pose_table_turn[17];
#endif
static BattleDataEntry data_table[3];
static BattleDataEntry data_table_ceil[14];
static BattleWeapon weapon_met_attack;
static BattleWeapon weapon_met_ceil_attack;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static BattleUnitDefenseAttr defence_attr_turn;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static EvtWord init_event[12];
static EvtWord init_event2[148];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[71];
static EvtWord wakeup_event[85];
static EvtWord attack_event[546];
static EvtWord wait_event[6];
static EvtWord damage_event_ceil[49];
static EvtWord counter_damage_event_ceil[76];
static EvtWord ceil_fall_ready_event[158];
static EvtWord attack_event_ceil[467];
static EvtWord wait_event_ceil[6];

/** Unit data of the Parabuzzy: 5 HP, level 20; the battle starts it from init_event_pata. */
BattleUnitKind unit_patamet = {
    UNIT_PARABUZZY, // unitType
    "btl_un_patamet", // name
    5, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    20, // level
    0, // bonusExp
    3, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    28, // width
    30, // height
    {5, 30}, // hitOffset
    {0.0f, 17.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {14.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {14.0f, 0.0f, 0.0f}, // bintaHitOffset
    {14.0f, 19.5f, 0.0f}, // kissHitOffset
    {0.0f, 17.0f, 0.0f}, // cutBaseOffset
    28.0f, // cutWidth
    30.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_MET_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts_pata, // parts
    init_event_pata, // initEventCode
    data_table_pata, // dataTable
};

/** Weapon data for the Parabuzzy's dive attack: 3 damage, a jump-type hit, one target. */
static BattleWeapon weapon_patamet_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    COUNTER_RESIST_FIERY | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_HOLD_FAST, // counterResistFlags
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

/**
 * Defence of the Parabuzzy while it still has its wings: 4 against normal, ice and electric hits;
 * immune to fire and explosions (defence 99 cancels the hit).
 */
static BattleUnitDefense defence_pata = {{4, 99, 4, 99, 4}};

/** Elemental reactions of the Parabuzzy: is immune to fire and explosion attacks. */
static BattleUnitDefenseAttr defence_attr_pata = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist_pata = {75, 90, 75, 100, 75, 100, 0, 75, 100, 75, 100, 75, 100, 75, 75, 60, 95, 100, 75, 100, 100, 95};
#else
static StatusVulnerability regist_pata = {
    75, // sleep
    90, // stop
    105, // dizzy
    100, // poison
    75, // confuse
    100, // electric
    0, // burn
    75, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    75, // allergic
    60, // fright
    90, // galeForce
    100, // fast
    75, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/**
 * Pose table of the winged Parabuzzy: the animation each battle pose (idle, hurt, fly, attack and so on)
 * plays.
 */
static PoseTableEntry pose_table_pata[] = {
    {POSE_TYPE_SLEEP, "PMT_N_1"},
    {POSE_TYPE_STOP, "PMT_Y_1"},
    {POSE_TYPE_FREEZE, "PMT_Y_1"},
    {POSE_TYPE_CONFUSE, "PMT_K_1"},
    {POSE_TYPE_POISON, "PMT_X_1"},
    {POSE_TYPE_DIZZY, "PMT_X_1"},
    {POSE_TYPE_NORMAL, "PMT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "PMT_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "PMT_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PMT_S_1"},
    {POSE_TYPE_DAMAGE, "PMT_D_1"},
    {POSE_TYPE_ATTACK, "PMT_A_1"},
    {POSE_TYPE_RUN, "PMT_R_1"},
    {POSE_TYPE_WALK, "PMT_W_1"},
    {POSE_TYPE_DANGER, "PMT_X_1"},
    {POSE_TYPE_PERIL, "PMT_X_1"},
    {POSE_TYPE_TALK, "PMT_S_1"},
    {POSE_TYPE_ACTION, "PMT_S_1"},
};

/**
 * Event table of the Parabuzzy: being knocked out of the air (dmg_fall_event_pata) and the First
 * Strike reaction (dmg_first_attack_pos_event_pata).
 */
static BattleDataEntry data_table_pata[] = {
    {UNIT_DATA_DAMAGE_FALL_EVENT, dmg_fall_event_pata},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos_event_pata},
    {0, NULL},
};

/** Body part of the Parabuzzy (winged), model c_met: its one target part. */
static BattleUnitKindPart parts_pata[] = {
    {1, "btl_un_patamet", "c_met", 0.0f, 0.0f, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f, 25.0f, 0.0f, 20, 30, 255, &defence_pata, &defence_attr_pata, kMainPart | kGuardTargetPart | kIsWinged | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table_pata},
};

/**
 * Sound table of the Parabuzzy: which sound each of its poses (idle, walk, run, attack and so on) plays, and
 * when.
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"PMT_S_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PMT_W_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PMT_R_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PMT_A_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PMT_K_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PMT_N_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PMT_X_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Init script of the winged form (the Parabuzzy).
 * Registers its wait, attack, damage and confusion scripts and the pose-sound timing table,
 * then starts the wait script.
 */
static EVT_BEGIN(init_event_pata)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_pata))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_pata))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_pata))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event_pata))
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage reaction of the winged form: the default damage script. */
static EVT_BEGIN(damage_event_pata)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Runs when the winged unit is knocked out of the air.
 * It drops the flying state, swaps in the grounded pose table, data table and part,
 * falls to the floor and turns into unit kind 72 (UNIT_BUZZY_BEETLE).
 * If it took more than 1 HP damage it also flips over through dmg_turn_event.
 */
// Replace winged parts/data with the ground form before the optional turnover path.
static EVT_BEGIN(dmg_fall_event_pata)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OffAttribute, LW(10), kFloating)
    CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), kIsWinged)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_ReplaceParts, LW(10), LW(11), PTR(parts), 1)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_KIND_CHANGED)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ChangeKind, LW(10), UNIT_BUZZY_BEETLE)
    CALL(btlevtcmd_GetHpDamageCount, LW(10), LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(init_event))
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
        RETURN
    ELSE
        CALL_CHILD(PTR(init_event))
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
        CALL_CHILD(PTR(dmg_turn_event))
    END_IF
    RETURN
EVT_END()

/**
 * Data-table event 48, run when the battle opens with Mario's or his partner's First Strike on it:
 * puts the unit at height 10 and waits for the attack to end.
 */
static EVT_BEGIN(dmg_first_attack_pos_event_pata)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * The Parabuzzy's dive attack.
 * It picks a target (the first-attack target on a pre-emptive strike), rises 40 units,
 * dives at the target in its rolled pose and, depending on the pre-damage check,
 * passes it (avoid) or hits it with SFX_ENM_PATAMET_ATTACK1 damage, then flies back home.
 */
static EVT_BEGIN(attack_event_pata)
    // Choose the target: the First Strike victim, an item if one is ready, or a sampled opposing unit.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_patamet_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_patamet_attack), LW(3), LW(4))
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

    // The camera follows it as it declares the attack and the guard window opens.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_patamet_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_patamet_attack))

    // Wind-up: it turns to the target and flaps 40 units up into the air.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAMET_MOVE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, FLOAT(1.0f), EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(250)
    CALL(evt_snd_sfxoff, LW(15))

    // It curls into its rolled pose and dives at the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAMET_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(9.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 10)
    SUBTRACT(LW(1), 30)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)

    // Hit check: a miss goes on to the fly-by, a hit to the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_patamet_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: it swoops past the target and hovers for a moment.
    LABEL(90)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, LW(3), EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 75)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(300)
    GOTO(98)

    // Hit: the damage is dealt with the action-command result and the impact sound plays.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_patamet_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_patamet_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAMET_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)

    // Retreat: the camera pulls back and it flies back to its home position.
    LABEL(98)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script of the winged form: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event_pata)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Pose table of the Buzzy Beetle: the animation each battle pose (idle, hurt, walk, attack and so on) plays.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "MET_N_1"},
    {POSE_TYPE_STOP, "MET_Y_1"},
    {POSE_TYPE_FREEZE, "MET_Y_1"},
    {POSE_TYPE_CONFUSE, "MET_K_1"},
    {POSE_TYPE_POISON, "MET_X_1"},
    {POSE_TYPE_DIZZY, "MET_X_1"},
    {POSE_TYPE_NORMAL, "MET_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "MET_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "MET_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "MET_A_1"},
    {POSE_TYPE_DAMAGE, "MET_D_1"},
    {POSE_TYPE_ATTACK, "MET_A_1"},
    {POSE_TYPE_WALK, "MET_W_1"},
    {POSE_TYPE_RUN, "MET_R_1"},
    {POSE_TYPE_DANGER, "MET_X_1"},
    {POSE_TYPE_PERIL, "MET_X_1"},
    {POSE_TYPE_TALK, "MET_T_1"},
    {POSE_TYPE_ACTION, "MET_S_1"},
};

/**
 * Pose table of the Buzzy Beetle while it clings to the ceiling: the animation each battle pose (idle, hurt,
 * walk, attack and so on) plays.
 */
static PoseTableEntry pose_table_ceil[] = {
    {POSE_TYPE_SLEEP, "SMT_N_1"},
    {POSE_TYPE_STOP, "SMT_Y_1"},
    {POSE_TYPE_FREEZE, "SMT_Y_1"},
    {POSE_TYPE_CONFUSE, "SMT_K_1"},
    {POSE_TYPE_POISON, "SMT_X_1"},
    {POSE_TYPE_DIZZY, "SMT_X_1"},
    {POSE_TYPE_NORMAL, "SMT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "SMT_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "SMT_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "SMT_A_1"},
    {POSE_TYPE_DAMAGE, "SMT_D_1"},
    {POSE_TYPE_ATTACK, "SMT_A_1"},
    {POSE_TYPE_WALK, "SMT_W_1"},
    {POSE_TYPE_RUN, "SMT_R_1"},
    {POSE_TYPE_DANGER, "SMT_X_1"},
    {POSE_TYPE_PERIL, "SMT_X_1"},
    {POSE_TYPE_TALK, "SMT_S_1"},
    {POSE_TYPE_ACTION, "SMT_S_1"},
};

/**
 * Pose table of the Buzzy Beetle while it lies flipped on its back: the animation each battle pose (idle,
 * hurt, walk, attack and so on) plays.
 */
static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "MET_N_2"},
    {POSE_TYPE_STOP, "MET_Y_2"},
    {POSE_TYPE_FREEZE, "MET_Y_2"},
    {POSE_TYPE_CONFUSE, "MET_K_2"},
    {POSE_TYPE_POISON, "MET_X_2"},
    {POSE_TYPE_DIZZY, "MET_X_2"},
    {POSE_TYPE_NORMAL, "MET_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "MET_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "MET_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "MET_A_1"},
    {POSE_TYPE_DAMAGE, "MET_D_3"},
#if VERSION > VERSION_DEMO_US
    {POSE_TYPE_ATTACK, "MET_A_1"},
    {POSE_TYPE_WALK, "MET_W_1"},
#endif
    {POSE_TYPE_DANGER, "MET_X_2"},
    {POSE_TYPE_PERIL, "MET_X_2"},
    {POSE_TYPE_TALK, "MET_S_2"},
    {POSE_TYPE_ACTION, "MET_S_2"},
};

/**
 * Event table of the Buzzy Beetle: being flipped onto its back (dmg_turn_event) and the First Strike
 * reaction (does nothing).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/**
 * Event table of the Buzzy Beetle while it clings to the ceiling: its counter-damage reactions (all running counter_damage_event_ceil) and the First Strike reaction (does nothing).
 */
static BattleDataEntry data_table_ceil[] = {
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_FIERY_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_ICY_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_POISON_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, counter_damage_event_ceil},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/** Weapon data for the Buzzy Beetle's shell attack: 3 damage, a shell-toss-type hit, one target. */
static BattleWeapon weapon_met_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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

/** Weapon data for the Buzzy Beetle's drop from the ceiling: 4 damage, a jump-type hit, one target. */
static BattleWeapon weapon_met_ceil_attack = {
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
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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

/**
 * Defence of the Parabuzzy once it has lost its wings: 4 against normal, ice and electric hits;
 * immune to fire and explosions (defence 99 cancels the hit).
 */
static BattleUnitDefense defence = {{4, 99, 4, 99, 4}};

/** Elemental reactions of the Buzzy Beetle: is immune to fire and explosion attacks. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE}};

/** Defence of the Buzzy Beetle while it lies flipped on its back: none against any element. */
static BattleUnitDefense defence_turn = {0};

/**
 * Elemental reactions of the Buzzy Beetle while it lies flipped on its back: none, every element hurts it
 * normally.
 */
static BattleUnitDefenseAttr defence_attr_turn = {0};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {75, 90, 75, 100, 75, 100, 0, 75, 100, 75, 100, 75, 100, 75, 75, 60, 65, 100, 75, 100, 100, 95};
#else
static StatusVulnerability regist = {
    75, // sleep
    90, // stop
    75, // dizzy
    100, // poison
    75, // confuse
    100, // electric
    0, // burn
    75, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    75, // allergic
    60, // fright
    30, // galeForce
    100, // fast
    75, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Body part of the Buzzy Beetle, model c_met: its one target part. */
static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_met", // partName
        "c_met", // modelName
        {0}, // positionOffset
        {0.0f, 20.0f, 0.0f}, // hitBaseOffset
        {0.0f, 25.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table, // poseTable
    },
};

/**
 * Init script of the Buzzy Beetle.
 * Calls init_event2 to set the unit up for its current state (unit work 0) and starts the wait script.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ON_CEILING, LW(15))
    CALL_CHILD(PTR(init_event2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Configures the Buzzy Beetle for the floor (unit work 0 is 0: normal scripts, hit cursor at the body)
 * or for the ceiling (nonzero: raised to height 130, ceiling pose/data tables and scripts, hit offsets lowered,
 * ceiling-fall event set). Also sets its run and walk sounds and clears the overturned counter.
 */
static EVT_BEGIN(init_event2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ON_CEILING, LW(15))
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
        CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
        CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
        CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
        CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kIsShelled)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    ELSE
        CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kCeiling)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_ceil))
        CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_ceil))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_VALUE(LW(1), 130)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_ceil))
        CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_ceil))
        CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_ceil))
        CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event_ceil))
        CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, PTR(ceil_fall_ready_event))
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, -50, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, -50, 0)
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 12, -15, 0)
    END_IF
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE1L"), STRING("SFX_ENM_MET_MOVE1R"), 0, 3, 3)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE1L"), STRING("SFX_ENM_MET_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetOverTurnCount, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * Damage event of the grounded Buzzy Beetle form (init_event2 sets it on the floor): runs the default damage
 * reaction (btldefaultevt_Damage) for itself.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Flips the Buzzy Beetle onto its back after a stomp-type hit:
 * switches to the overturned defence tables and pose table, sets status flag 1,
 * sets the overturned counter to 1 and hops to the floor.
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 12, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

/**
 * Counts the overturned counter down by one.
 * Once it reaches 0 the beetle restores its normal defence and pose tables,
 * clears status flag 1 and jumps back onto its feet; while it is still above 0 it just goes back to waiting.
 */
// A flipped attack turn runs this countdown/recovery path instead of attacking.
static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ACTION)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Buzzy Beetle's shell roll. If it is overturned it only wakes up.
 * Otherwise it tucks into its shell and spins while a smoke trail follows it, and rolls into the target;
 * a miss makes it bounce past, a hit applies damage after the action command.
 * It then walks back to its home position.
 */
static EVT_BEGIN(attack_event)
    // A unit lying on its back spends the turn getting up.
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF

    // Choose the target: the First Strike victim, an item if one is ready, or a sampled opposing unit.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        WAIT_FRAMES(30)
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_met_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_met_attack), LW(3), LW(4))
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

    // The camera follows it as it declares the attack and the guard window opens.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_met_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_met_attack))

    // Wind-up: it turns to the target, hops up and tucks into its shell.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(100)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_SHELL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_ATTACK2"), EVT_NULLPTR, 0, LW(15))

    // The shell starts to spin: a side event turns it faster as its spin speed is raised.
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, LW(15))
            IF_INT_EQUAL(LW(15), 1)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(14))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(14), 0)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_MS(100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 10)
    WAIT_MS(100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 20)
    WAIT_MS(100)

    // A trail of smoke puffs behind the shell.
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD(LW(1), 10)
            CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.2998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT

    // The shell slides along the ground into the target.
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, MOVE_SOUND_NONE, 0)
    DELETE_EVENT(LW(13))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)

    // Hit check: a miss goes on to the overshoot, a hit to the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss: the shell slides past the target, slows, then hops up onto its feet again.
    LABEL(90)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 10)
    WAIT_MS(250)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(10.0f), FLOAT(0.39941406f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    GOTO(98)

    // Hit: the damage is dealt and the shell bounces back off the target in two hops.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_met_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 10)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(3.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 75)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 1)
    GOTO(98)

    // Retreat: it walks back to its home position.
    LABEL(98)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait (idle) event of the grounded Buzzy Beetle form: sets the idle pose from the current pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_ceil)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetTotalDamage, UNIT_ID_SELF, LW(12))
    IF_INT_EQUAL(LW(12), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(ceil_fall_ready_event))
    END_IF
    RETURN
EVT_END()

/**
 * Counter-damage reaction of the ceiling Buzzy Beetle:
 * leaves the ceiling state, restores the floor pose and data tables and hit offsets,
 * drops its home position to the floor and applies the counter damage.
 */
static EVT_BEGIN(counter_damage_event_ceil)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 12, 15, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL_CHILD(PTR(init_event2))
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(PTR(subsetevt_counter_damage))
    RETURN
EVT_END()

/**
 * The ceiling Buzzy Beetle falls (SFX_ENM_FALL1),
 * lands with SFX_ENM_MET_LANDING1 and ends up overturned on the floor with its home position set there.
 */
static EVT_BEGIN(ceil_fall_ready_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetJumpSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 15, LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_turn))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 12, 15, 0)
    CALL_CHILD(init_event2)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ACTION)
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 1)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    RETURN
EVT_END()

/**
 * Attack of the ceiling Buzzy Beetle.
 * It slides over the target and drops on it (SFX_ENM_MET_FALL1),
 * with a miss or hit decided by the pre-damage check, then swings back and forth,
 * leaves the ceiling state and hops back to its home position on the floor.
 */
// A completed drop attack restores the upright ground form, not the flipped damage-fall state.
static EVT_BEGIN(attack_event_ceil)
    // Choose the target: the First Strike victim, an item if one is ready, or a sampled opposing unit.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_met_ceil_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_met_ceil_attack), LW(3), LW(4))
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

    // The camera follows it as it declares the attack and the guard window opens.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_met_ceil_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_met_ceil_attack))

    // Wind-up: it turns to the target and slides along the ceiling until it is directly above it.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(11), LW(12), 60, MOVE_SOUND_FROM_TABLE, 1)

    // It lets go of the ceiling and drops onto the target with a thud.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    WAIT_MS(250)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetJumpSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 18, LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)

    // Hit check: a miss leaves the target untouched, a hit goes on to the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_ceil_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    GOTO(98)

    // Hit: the damage is dealt with the action-command result.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_met_ceil_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_ceil_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)

    // It sways back and forth like a pendulum, the swing dying down.
    LABEL(98)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, -18, 0)
    SET_FLOAT_VALUE(LW(12), FLOAT(30.0f))
    SET_FLOAT_VALUE(LW(13), FLOAT(0.0f))
    LOOP(60)
        CALL(evt_sub_get_sincos, LW(13), LW(14), LW(15))
        MULTIPLY_FLOAT(LW(14), LW(12))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(14))
        SUBTRACT_FLOAT(LW(12), FLOAT(0.5f))
        ADD_FLOAT(LW(13), FLOAT(10.0f))
        WAIT_FRAMES(1)
    END_LOOP

    // It leaves the ceiling state: its pose table, data table and hit offsets switch back to the floor
    // versions.
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 12, 15, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)

    // It hops down onto the floor twice, walks back to its home position and runs the init event again.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(3.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(7), 0, LW(9))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(7), 0, LW(9), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL_CHILD(init_event2)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script of the ceiling Buzzy Beetle: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event_ceil)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()


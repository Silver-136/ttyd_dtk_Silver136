// R. Parabuzzy (`crimson_patamet`, unit kind 173): a variant of tst's unit_patamet.c with its own kind, name and stats.
#include "version.h"
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "evt/evt_sub.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "data_tables.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// Unit work slots of the R. Parabuzzy's Buzzy Beetle form (btlevtcmd_SetUnitWork / GetUnitWork).
enum {
    UW_CRIMSON_PATAMET_ON_CEILING = 0, // non-zero: hangs from the ceiling (battle setup); 0 once on the floor
    UW_CRIMSON_PATAMET_SPIN_STOP = 1,  // 1 stops the spin loop of the shell attack
    UW_CRIMSON_PATAMET_SPIN_SPEED = 2  // rotation per frame of that spin
};

BattleUnitKind unit_crimson_patamet;
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

#if VERSION <= VERSION_DEMO_JP
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

#if VERSION <= VERSION_DEMO_JP
static EvtWord init_event2[142];
#else
static EvtWord init_event2[148];
#endif

static EvtWord damage_event[10];
static EvtWord dmg_turn_event[71];
static EvtWord wakeup_event[85];
static EvtWord attack_event[546];
static EvtWord wait_event[6];
static EvtWord damage_event_ceil[49];

#if VERSION <= VERSION_DEMO_JP
static EvtWord counter_damage_event_ceil[70];
static EvtWord ceil_fall_ready_event[152];
static EvtWord attack_event_ceil[461];
#else
static EvtWord counter_damage_event_ceil[76];
static EvtWord ceil_fall_ready_event[158];
static EvtWord attack_event_ceil[467];
#endif

static EvtWord wait_event_ceil[6];

/**
 * Unit kind of Red Parabuzzy: 5 max HP, level 17. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_crimson_patamet = {
    173,
    "btl_un_crimson_patamet",
    5,
    0,
    1,
    1,
    17,
    0,
    1,
    60,
    1,
    50,
    9999,
    28,
    30,
    5,
    30,
    0.0f,
    17.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    0.0f,
    14.0f,
    19.5f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    28.0f,
    30.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_MET_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x4,
    &regist,
    1,
    parts_pata,
    init_event_pata,
    data_table_pata,
};

/** Attack data of Red Parabuzzy's patamet attack: base damage 3, 100% accuracy. */
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
#if VERSION <= VERSION_DEMO_JP
    COUNTER_RESIST_FIERY | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_PAYBACK | COUNTER_RESIST_HOLD_FAST,
#else
    COUNTER_RESIST_FIERY | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_HOLD_FAST, // counterResistFlags
#endif
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
 * Defence of the R. Parabuzzy while it still has its wings: 4 against normal, ice and electric hits;
 * immune to fire and explosions (defence 99 cancels the hit).
 */
static BattleUnitDefense defence_pata = {{4, 99, 4, 99, 4}};

/**
 * Element reactions of Red Parabuzzy in its winged state: fire immune and explosion immune;
 * the other elements hit normally.
 */
static BattleUnitDefenseAttr defence_attr_pata = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP

/**
 * Status vulnerability of Red Parabuzzy in its winged state (the early demos),
 * as percent multipliers on each status's chance: immune to Burn;
 * 100 (unchanged) for Poison, Electric, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 60, Stop 90, Dizzy 75, Confuse 75, Freeze 75, Tiny 75, Attack Down 75, Defense Down 75,
 * Allergic 75, Fright 60, Gale Force 95, Slow 75 and instant KO 95.
 */
static StatusVulnerability regist_pata = {60, 90, 75, 100, 75, 100, 0, 75, 100, 75, 100, 75, 100, 75, 75, 60, 95, 100, 75, 100, 100, 95};
#else

/**
 * Status vulnerability of Red Parabuzzy in its winged state (the final versions),
 * as percent multipliers on each status's chance: immune to Burn;
 * 100 (unchanged) for Poison, Electric, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 75, Stop 90, Dizzy 105, Confuse 75, Freeze 75, Tiny 90, Attack Down 90, Defense Down 95,
 * Allergic 75, Fright 60, Gale Force 90, Slow 75 and instant KO 95.
 */
static StatusVulnerability regist_pata = {75, 90, 105, 100, 75, 100, 0, 75, 100, 90, 100, 90, 100, 95, 75, 60, 90, 100, 75, 100, 100, 95};
#endif

/**
 * Pose table of Red Parabuzzy in its winged state: maps its battle pose types (stay, walk, hurt, dead,
 * talk and so on) to animation names, 18 entries such as PMT_N_1, PMT_Y_1, PMT_K_1, PMT_X_1.
 */
static PoseTableEntry pose_table_pata[] = {
    {1, "PMT_N_1"},
    {2, "PMT_Y_1"},
    {9, "PMT_Y_1"},
    {5, "PMT_K_1"},
    {4, "PMT_X_1"},
    {3, "PMT_X_1"},
    {POSE_TYPE_NORMAL, "PMT_S_1"},
    {29, "PMT_Q_1"},
    {30, "PMT_Q_1"},
    {31, "PMT_S_1"},
    {POSE_TYPE_DAMAGE, "PMT_D_1"},
    {50, "PMT_A_1"},
    {POSE_TYPE_RUN, "PMT_R_1"},
    {POSE_TYPE_WALK, "PMT_W_1"},
    {POSE_TYPE_DANGER, "PMT_X_1"},
    {POSE_TYPE_PERIL, "PMT_X_1"},
    {POSE_TYPE_TALK, "PMT_S_1"},
    {POSE_TYPE_ACTION, "PMT_S_1"},
};

/**
 * Event table of Red Parabuzzy in its winged state: maps event tags to the scripts that react to them
 * (dmg_fall_event_pata and dmg_first_attack_pos_event_pata); tag 0 ends it.
 * Tag 14 is being knocked out of the air and 48 is a First Strike.
 */
static BattleDataEntry data_table_pata[] = {
    {14, dmg_fall_event_pata},
    {48, dmg_first_attack_pos_event_pata},
    {0, NULL},
};

/**
 * Body parts of Red Parabuzzy in its winged state: a single part (model c_met_h) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts_pata[] = {
    {1, "btl_un_crimson_patamet", "c_met_h", 0.0f, 0.0f, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f, 25.0f, 0.0f, 20, 30, 255, &defence_pata, &defence_attr_pata, 0x600809, 0, pose_table_pata},
};

/**
 * Sound cues of Red Parabuzzy's animations: for each of its 7 animation names,
 * the sound effect that plays with it (SFX_ENM_PATAMET_WAIT1).
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
 * Init script of the winged form: installs its own wait,
 * attack and damage handlers (confusion reuses the attack), sets the pose sound table and starts the wait event.
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

/**
 * Damage handler: runs the shared default damage script (btldefaultevt_Damage) with its default parameters
 * (LW(10) = -2, LW(11) = 1).
 */
static EVT_BEGIN(damage_event_pata)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Called when the winged form is hit down: clears its flying state and winged part attribute,
 * switches to the ground tables and parts, changes its kind back to the plain version (kind 72) and lands,
 * then runs init_event; if it has taken more than one hit this turn (hit count) it also flips over (dmg_turn_event).
 */
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
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 10, -1)
    CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))

    // BUG: changes the fallen Red Parabuzzy to the plain Buzzy Beetle kind (72), not to the Red Buzzy kind
    // that unit_crimson_met uses (172).
    CALL(btlevtcmd_ChangeKind, LW(10), UNIT_BUZZY_BEETLE)
    CALL(btlevtcmd_GetHpDamageCount, LW(10), LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        CALL_CHILD(init_event)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
        RETURN
    ELSE
        CALL_CHILD(init_event)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
        CALL_CHILD(dmg_turn_event)
    END_IF
    RETURN
EVT_END()

/**
 * Data-table event 48, run when the battle opens with Mario's or his partner's First Strike on it:
 * puts it at height 10 and waits for the attack to end.
 */
static EVT_BEGIN(dmg_first_attack_pos_event_pata)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Attack script of the winged form: picks a target after the item-use check, flies up,
 * then dives in shell form at the target; on a miss it flies past,
 * on a hit it plays the attack sound and deals damage. It then flies back home.
 */
static EVT_BEGIN(attack_event_pata)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)

        // On a First Strike, the winged Parabuzzy uses the first-strike target without an item check.
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
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_patamet_attack))

    // The attack is declared and paid for, and the Parabuzzy flies up over its target.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_patamet_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAMET_MOVE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)

    // It swoops in toward the target.
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, FLOAT(1.0f), 0, 0, -1)
    WAIT_MS(250)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAMET_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(9.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 10)
    SUBTRACT(LW(1), 30)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, 0, 0, -1)

    // The damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_patamet_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // On a miss it flies past the target.
    LABEL(90)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, LW(3), EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 75)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, 0, 0, 0, -1)
    WAIT_MS(300)
    GOTO(98)

    // On a hit it plays the attack sound and the shell damages the target.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_patamet_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_patamet_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAMET_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)

    // It flies back home and returns to waiting.
    LABEL(98)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, 0, 0, -1)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle script of the winged form: sets the standard waiting pose from the unit's pose table. */
static EVT_BEGIN(wait_event_pata)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Pose table of Red Parabuzzy: maps its battle pose types (stay, walk, hurt, dead, talk and so on) to
 * animation names, 18 entries such as MET_N_1, MET_Y_1, MET_K_1, MET_X_1.
 */
static PoseTableEntry pose_table[] = {
    {1, "MET_N_1"},
    {2, "MET_Y_1"},
    {9, "MET_Y_1"},
    {5, "MET_K_1"},
    {4, "MET_X_1"},
    {3, "MET_X_1"},
    {POSE_TYPE_NORMAL, "MET_S_1"},
    {29, "MET_Q_1"},
    {30, "MET_Q_1"},
    {31, "MET_A_1"},
    {POSE_TYPE_DAMAGE, "MET_D_1"},
    {50, "MET_A_1"},
    {POSE_TYPE_WALK, "MET_W_1"},
    {POSE_TYPE_RUN, "MET_R_1"},
    {POSE_TYPE_DANGER, "MET_X_1"},
    {POSE_TYPE_PERIL, "MET_X_1"},
    {POSE_TYPE_TALK, "MET_T_1"},
    {POSE_TYPE_ACTION, "MET_S_1"},
};

/**
 * Pose table of Red Parabuzzy hanging from the ceiling: maps its battle pose types (stay, walk, hurt,
 * dead, talk and so on) to animation names, 18 entries such as SMT_N_1, SMT_Y_1, SMT_K_1, SMT_X_1.
 */
static PoseTableEntry pose_table_ceil[] = {
    {1, "SMT_N_1"},
    {2, "SMT_Y_1"},
    {9, "SMT_Y_1"},
    {5, "SMT_K_1"},
    {4, "SMT_X_1"},
    {3, "SMT_X_1"},
    {POSE_TYPE_NORMAL, "SMT_S_1"},
    {29, "SMT_Q_1"},
    {30, "SMT_Q_1"},
    {31, "SMT_A_1"},
    {POSE_TYPE_DAMAGE, "SMT_D_1"},
    {50, "SMT_A_1"},
    {POSE_TYPE_WALK, "SMT_W_1"},
    {POSE_TYPE_RUN, "SMT_R_1"},
    {POSE_TYPE_DANGER, "SMT_X_1"},
    {POSE_TYPE_PERIL, "SMT_X_1"},
    {POSE_TYPE_TALK, "SMT_S_1"},
    {POSE_TYPE_ACTION, "SMT_S_1"},
};

/**
 * Pose table of Red Parabuzzy flipped onto its back: maps its battle pose types (stay, walk, hurt, dead,
 * talk and so on) to animation names, 15 entries in demos and 17 in retail, such as MET_N_2, MET_Y_2, MET_K_2, MET_X_2.
 */
static PoseTableEntry pose_table_turn[] = {
    {1, "MET_N_2"},
    {2, "MET_Y_2"},
    {9, "MET_Y_2"},
    {5, "MET_K_2"},
    {4, "MET_X_2"},
    {3, "MET_X_2"},
    {POSE_TYPE_NORMAL, "MET_S_2"},
    {29, "MET_Q_2"},
    {30, "MET_Q_2"},
    {31, "MET_A_1"},
    {POSE_TYPE_DAMAGE, "MET_D_3"},
#if VERSION >= VERSION_JP
    {50, "MET_A_1"},
    {POSE_TYPE_WALK, "MET_W_1"},
#endif
    {POSE_TYPE_DANGER, "MET_X_2"},
    {POSE_TYPE_PERIL, "MET_X_2"},
    {POSE_TYPE_TALK, "MET_S_2"},
    {POSE_TYPE_ACTION, "MET_S_2"},
};

/**
 * Event table of Red Parabuzzy: maps event tags to the scripts that react to them (dmg_turn_event);
 * tag 0 ends it.
 */
static BattleDataEntry data_table[] = {
    {13, dmg_turn_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Event table of Red Parabuzzy hanging from the ceiling: maps event tags to the scripts that react to
 * them (counter_damage_event_ceil); tag 0 ends it.
 */
static BattleDataEntry data_table_ceil[] = {
    {26, counter_damage_event_ceil},
    {25, counter_damage_event_ceil},
    {27, counter_damage_event_ceil},
    {28, counter_damage_event_ceil},
    {29, counter_damage_event_ceil},
    {30, counter_damage_event_ceil},
    {31, counter_damage_event_ceil},
    {32, counter_damage_event_ceil},
    {33, counter_damage_event_ceil},
    {34, counter_damage_event_ceil},
    {35, counter_damage_event_ceil},
    {36, counter_damage_event_ceil},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/** Attack data of Red Parabuzzy's met attack: base damage 3, 100% accuracy. */
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
#if VERSION <= VERSION_DEMO_JP
    COUNTER_RESIST_ALL,
#else
    COUNTER_RESIST_ELECTRIC | COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_HOLD_FAST, // counterResistFlags
#endif
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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

/** Attack data of Red Parabuzzy's met ceil attack: base damage 4, 100% accuracy. */
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
#if VERSION <= VERSION_DEMO_JP
    COUNTER_RESIST_ALL,
#else
    COUNTER_RESIST_ELECTRIC | COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_HOLD_FAST, // counterResistFlags
#endif
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
 * Defence of the R. Parabuzzy once it has lost its wings: 4 against normal, ice and electric hits;
 * immune to fire and explosions (defence 99 cancels the hit).
 */
static BattleUnitDefense defence = {{4, 99, 4, 99, 4}};

/** Element reactions of Red Parabuzzy: fire immune and explosion immune; the other elements hit normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE}};

/**
 * Defence of Red Parabuzzy flipped onto its back per element (normal, fire, ice, explosion, electric),
 * subtracted from each hit: none.
 */
static BattleUnitDefense defence_turn = {{0, 0, 0, 0, 0}};

/** Element reactions of Red Parabuzzy flipped onto its back: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr_turn = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP

/**
 * Status vulnerability of Red Parabuzzy (the early demos), as percent multipliers on each status's chance:
 * immune to Burn; 100 (unchanged) for Poison, Electric, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 60, Stop 90, Dizzy 75, Confuse 75, Freeze 75, Tiny 75, Attack Down 75, Defense Down 75,
 * Allergic 75, Fright 60, Gale Force 65, Slow 75 and instant KO 95.
 */
static StatusVulnerability regist = {60, 90, 75, 100, 75, 100, 0, 75, 100, 75, 100, 75, 100, 75, 75, 60, 65, 100, 75, 100, 100, 95};
#else

/**
 * Status vulnerability of Red Parabuzzy (the final versions), as percent multipliers on each status's chance:
 * immune to Burn; 100 (unchanged) for Poison, Electric, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 75, Stop 90, Dizzy 75, Confuse 75, Freeze 75, Tiny 90, Attack Down 90, Defense Down 95,
 * Allergic 75, Fright 60, Gale Force 30, Slow 75 and instant KO 95.
 */
static StatusVulnerability regist = {75, 90, 75, 100, 75, 100, 0, 75, 100, 90, 100, 90, 100, 95, 75, 60, 30, 100, 75, 100, 100, 95};
#endif

/**
 * Body parts of Red Parabuzzy: a single part (model c_met_h) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_crimson_met", "c_met_h", 0.0f, 0.0f, 0.0f, 0.0f, 20.0f, 0.0f, 0.0f, 25.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

/**
 * Init script of the R. Parabuzzy:
 * reads unit work 0 (1 means it starts on the ceiling) into LW(15),
 * runs init_event2 to install the right handlers and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_ON_CEILING, LW(15))
    CALL_CHILD(init_event2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Installs the handlers for the unit's current form, chosen by unit work 0.
 * On the ground (0) it installs its normal wait, attack, damage and confusion scripts;
 * on the ceiling (1) it switches to the ceiling pose and data tables,
 * moves to height 130 and installs the ceiling scripts, including the fall script.
 * Both set the run and walk sounds and clear the flipped-over count.
 */
static EVT_BEGIN(init_event2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_ON_CEILING, LW(15))
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
#if VERSION >= VERSION_JP // the JP demo has no kiss move offsets
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 12, -15, 0)
#endif
    END_IF
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE1L"), STRING("SFX_ENM_MET_MOVE1R"), 0, 3, 3)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE1L"), STRING("SFX_ENM_MET_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetOverTurnCount, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * Damage handler: runs the shared default damage script (btldefaultevt_Damage) with its default parameters
 * (LW(10) = -2, LW(11) = 1).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Flips the unit onto its back after a hit that turns it over:
 * swaps in the flipped defence, pose and attribute tables, marks status flag 1 with a flipped-over count of 1,
 * plays the shell sound, and drops it to the ground in two bounces.
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 18, -1)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 12, -1)
    RETURN
EVT_END()

/**
 * Turn-over recovery: counts down the flipped-over count;
 * once it reaches 0 it restores the normal defence and pose tables, clears status flag 1,
 * plays the wake-up sound and hops back to the ground; otherwise it keeps lying flipped and waits.
 */
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 18, -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Attack script of the R. Parabuzzy: if it is lying flipped it gets up (wakeup_event);
 * otherwise it picks a target (after the item-use check),
 * jumps into its shell and spins up while rolling at the target, leaving dust puffs.
 * If the attack misses it rolls past the target and jumps back;
 * if it hits it deals damage with the guard-timing check and bounces off, then walks home.
 */
static EVT_BEGIN(attack_event)
    // If the Parabuzzy is lying on its back it spends its turn getting back up instead of attacking.
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF

    // On a first strike (battle flag 2) it takes the first-strike target; otherwise it uses an enemy item, or picks a target.
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
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_met_attack))

    // The attack is declared and paid for, and the Parabuzzy turns to face its target.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_met_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(100)

    // It hops into its shell with a clank.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_SHELL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)

    // It starts rolling toward the target, leaving puffs of dust behind it.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_ATTACK2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_STOP, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_SPEED, 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_STOP, LW(15))
            IF_INT_EQUAL(LW(15), 1)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_SPEED, LW(14))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(14), 0)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_MS(100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_SPEED, 10)
    WAIT_MS(100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_SPEED, 20)
    WAIT_MS(100)
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD(LW(1), 10)
            CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.2998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, 0, 0)
    DELETE_EVENT(LW(13))
    CALL(evt_btl_camera_set_mode, 0, 0)

    // The damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // On a miss it rolls past the target, uncurls and hops back.
    LABEL(90)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, 0, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_SPEED, 10)
    WAIT_MS(250)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_STOP, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(10.0f), FLOAT(0.39941406f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    GOTO(98)

    // On a hit the shell slams into the target (guard check and damage), then bounces back in two hops.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_met_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_SPEED, 10)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(3.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 75)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_SPIN_STOP, 1)
    GOTO(98)

    // It walks home and returns to waiting.
    LABEL(98)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle script: sets the standard waiting pose from the unit's pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Damage handler while it hangs on the ceiling:
 * runs the default damage script, and unless the hit knocked it out (DAMAGE_RESULT_ZERO_HP) without a Life Shroom
 * revive (DAMAGE_RESULT_REVIVAL) it runs ceil_fall_ready_event when it took damage while on the ceiling, so it falls.
 */
static EVT_BEGIN(damage_event_ceil)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
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
        CALL_CHILD(ceil_fall_ready_event)
    END_IF
    RETURN
EVT_END()

/**
 * Counter-damage handler on the ceiling: leaves the ceiling state (restores the ground pose and data tables,
 * hit offsets and home position, runs init_event2 for the ground form) and then applies the counter damage.
 */
static EVT_BEGIN(counter_damage_event_ceil)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 12, 15, 0)
#endif
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL_CHILD(init_event2)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(subsetevt_counter_damage)
    RETURN
EVT_END()

/**
 * Makes the ceiling unit fall: plays the fall sound, drops it to the ground with the landing sound,
 * swaps to the ground tables, runs init_event2 for the ground form,
 * and leaves it lying flipped over (turned defence and pose tables, flipped-over count 1), after two bounces.
 */
static EVT_BEGIN(ceil_fall_ready_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetJumpSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 15, LW(2), 15, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_turn))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 12, 15, 0)
#endif
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 18, -1)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 12, -1)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    RETURN
EVT_END()

/**
 * Attack script on the ceiling: picks a target, slides along the ceiling over it,
 * drops with the fall and landing sounds and hits it (guard check and damage),
 * then sways while standing back up, restores the ground form (init_event2) and walks home.
 */
// Hanging from the ceiling, the Parabuzzy checks, unless it is making a First Strike, whether to use a held item.
static EVT_BEGIN(attack_event_ceil)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)

        // It picks a target; with none, it ends the turn (or flusters when confused).
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
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_met_ceil_attack))

    // The attack is declared and paid for.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_met_ceil_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(100)

    // It slides along the ceiling until it is above the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(11), LW(12), 60, -1, 1)

    // It lets go and drops with a falling sound.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
    WAIT_MS(250)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetJumpSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 18, LW(2), 15, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, 0, 0)

    // The damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_ceil_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // On a miss it lands beside the target.
    LABEL(90)
    GOTO(98)

    // On a hit it lands on the target (guard check and damage).
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_met_ceil_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_ceil_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)
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
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)

    // After the sway above it goes back to its ground form.
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATAMET_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 12, 15, 0)
#endif

    // It hops home and returns to waiting.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(3.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(7), 0, LW(9))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(7), 0, LW(9), 0, -1, 0)
    CALL_CHILD(init_event2)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle script on the ceiling: sets the standard waiting pose from the unit's pose table. */
static EVT_BEGIN(wait_event_ceil)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

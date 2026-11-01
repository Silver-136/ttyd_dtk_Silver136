// R. Parabuzzy (`crimson_patatogemet`, unit kind 174): a variant of tst's unit_patatogemet.c with its own kind, name and
// stats. Its English name, R. Parabuzzy, is shared with the spikeless one in unit_crimson_patamet.c.
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
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "data_tables.h"
#include "battle/battle_sub.h"

// Unit work slots of the R. Parabuzzy (btlevtcmd_SetUnitWork / GetUnitWork)
enum {
    UW_CRIMSON_PATATOGEMET_ON_CEILING = 0, // 1 while it clings to the ceiling (set by the battle setup, cleared when it falls)
    UW_CRIMSON_PATATOGEMET_SPIN_DONE = 1,  // set to 1 to stop the shell-spin loop of the attack (it then resets its rotation)
    UW_CRIMSON_PATATOGEMET_SPIN_SPEED = 2, // degrees per frame the spinning shell turns by
};

BattleUnitKind unit_crimson_patatogemet;
static BattleWeapon weapon_patamet_attack;
static BattleUnitDefense defence_pata;
static BattleUnitDefenseAttr defence_attr_pata;
static StatusVulnerability regist_pata;
static PoseTableEntry pose_table_pata[18];
static BattleDataEntry data_table_pata[4];
static BattleUnitKindPart parts_pata[1];
static PoseSoundTiming pose_sound_timing_table[9];
static EvtWord init_event_pata[26];
EvtWord crimson_dmg_avoid_counter_thorn_head_event_pata[61];
static EvtWord damage_event_pata[10];
static EvtWord dmg_fall_event_pata[106];
static EvtWord dmg_first_attack_pos_event_pata[19];
static EvtWord attack_event_pata[339];
static EvtWord wait_event_pata[6];
static PoseTableEntry pose_table[17];
static PoseTableEntry pose_table_ceil[17];
#if VERSION <= VERSION_DEMO_US
static PoseTableEntry pose_table_turn[14];
#else
static PoseTableEntry pose_table_turn[16];
#endif
static BattleDataEntry data_table[4];
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
#if VERSION <= VERSION_DEMO_US
static EvtWord init_event2[142];
#else
static EvtWord init_event2[148];
#endif
static EvtWord dmg_avoid_counter_thorn_head_event[61];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[82];
static EvtWord wakeup_event[96];
static EvtWord attack_event[546];
static EvtWord wait_event[6];
static EvtWord damage_event_ceil[49];
#if VERSION <= VERSION_DEMO_US
static EvtWord counter_damage_event_ceil[70];
static EvtWord ceil_fall_ready_event[157];
static EvtWord attack_event_ceil[420];
#else
static EvtWord counter_damage_event_ceil[76];
static EvtWord ceil_fall_ready_event[163];
static EvtWord attack_event_ceil[426];
#endif
static EvtWord wait_event_ceil[6];

/**
 * Unit kind of the R. Parabuzzy: 5 max HP, level 21. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_crimson_patatogemet = {
    174,
    "btl_un_crimson_patatogemet",
    5,
    0,
    1,
    1,
    21,
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
#if VERSION >= VERSION_US
    &regist_pata,
#else
    &regist,
#endif
    1,
    parts_pata,
    init_event_pata,
    data_table_pata,
};

/** Attack data of the R. Parabuzzy's patamet attack: base damage 3, 100% accuracy. */
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
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
#if VERSION <= VERSION_DEMO_US
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
 * Element reactions of the R. Parabuzzy in its winged state: fire immune and explosion immune;
 * the other elements hit normally.
 */
static BattleUnitDefenseAttr defence_attr_pata = {{0, 2, 0, 2, 0}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of the R. Parabuzzy in its winged state (the demos),
 * as the percent chance a status takes hold: immune to Burn;
 * always hit by Poison, Electric, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 60, Stop 90, Dizzy 75, Confuse 75, Freeze 75, Tiny 75, Attack Down 75, Defense Down 75,
 * Allergic 75, Fright 60, Gale Force 65, Slow 75 and instant KO 95.
 */
static StatusVulnerability regist_pata = {60, 90, 75, 100, 75, 100, 0, 75, 100, 75, 100, 75, 100, 75, 75, 60, 65, 100, 75, 100, 100, 95};
#else
/**
 * Status vulnerability of the R. Parabuzzy in its winged state (the final versions),
 * as the percent chance a status takes hold: immune to Burn;
 * always hit by Poison, Electric, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 75, Stop 90, Dizzy 75, Confuse 75, Freeze 75, Tiny 90, Attack Down 90, Defense Down 95,
 * Allergic 75, Fright 60, Gale Force 30, Slow 75 and instant KO 95.
 */
static StatusVulnerability regist_pata = {75, 90, 75, 100, 75, 100, 0, 75, 100, 90, 100, 90, 100, 95, 75, 60, 30, 100, 75, 100, 100, 95};
#endif

/**
 * Pose table of the R. Parabuzzy in its winged state: maps its battle pose types (stay, walk, hurt,
 * dead, talk and so on) to animation names, 18 entries such as PTM_N_1, PTM_Y_1, PTM_K_1, PTM_X_1.
 */
static PoseTableEntry pose_table_pata[] = {
    {1, "PTM_N_1"},
    {2, "PTM_Y_1"},
    {9, "PTM_Y_1"},
    {5, "PTM_K_1"},
    {4, "PTM_X_1"},
    {3, "PTM_X_1"},
    {BTL_POSE_NORMAL, "PTM_S_1"},
    {29, "PTM_Q_1"},
    {30, "PTM_Q_1"},
    {31, "PTM_S_1"},
    {BTL_POSE_DAMAGE, "PTM_D_1"},
    {50, "PTM_A_1"},
    {POSE_TYPE_RUN, "PTM_R_1"},
    {POSE_TYPE_WALK, "PTM_W_1"},
    {POSE_TYPE_DANGER, "PTM_X_1"},
    {POSE_TYPE_PERIL, "PTM_X_1"},
    {POSE_TYPE_TALK, "PTM_T_1"},
    {POSE_TYPE_ACTION, "PTM_S_1"},
};

/**
 * Event table of the R. Parabuzzy in its winged state: maps event tags to the scripts that react to
 * them (dmg_fall_event_pata, crimson_dmg_avoid_counter_thorn_head_event_pata and
 * dmg_first_attack_pos_event_pata); tag 0 ends it. Tag 14 is being knocked out of the air, 37 is the
 * attacker hurt by spikes and 48 is a First Strike.
 */
static BattleDataEntry data_table_pata[] = {
    {14, dmg_fall_event_pata},
    {37, crimson_dmg_avoid_counter_thorn_head_event_pata},
    {48, dmg_first_attack_pos_event_pata},
    {0, NULL},
};

/**
 * Body parts of the R. Parabuzzy in its winged state: a single part (model c_met_h) holding its hit
 * box. Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts_pata[] = {
    {1, "btl_un_crimson_patatogemet", "c_met_h", 0.0f, 0.0f, 0.0f, 0.0f, 23.0f, 0.0f, 0.0f, 28.0f, 0.0f, 20, 30, 255, &defence_pata, &defence_attr_pata, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | kIsWinged | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, kCounterTopSpiky, pose_table_pata},
};

/**
 * Sound cues of the R. Parabuzzy's animations: for each of its 9 animation names,
 * the sound effect that plays with it (SFX_ENM_PATAMET_WAIT1).
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"PTM_S_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PTM_W_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PTM_R_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PTM_A_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PTM_K_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PTM_N_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PTM_X_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
    {"PTM_T_1", 0.01f, NULL, "SFX_ENM_PATAMET_WAIT1", 0, 1},
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
 * After an attacker is hurt by the R. Parabuzzy's spikes (data event 37),
 * flashes a thorn-spark effect (toge_flush) above its head 30 frames later,
 * higher or lower depending on whether it is Mega or Mini.
 */
EVT_BEGIN(crimson_dmg_avoid_counter_thorn_head_event_pata)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 50)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 30)
        GOTO(90)
    END_IF
    ADD(LW(1), 40)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

/**
 * Damage handler: runs the shared default damage script (btldefaultevt_Damage) with its default parameters
 * (LW(10) = -2, LW(11) = 1).
 */
static EVT_BEGIN(damage_event_pata)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Called when the winged form is hit down: clears its flying state and winged part attribute,
 * switches to the ground tables and parts, changes its kind to the wingless version (kind 73) and lands,
 * then runs init_event; if it took more than one point of damage it also flips over (dmg_turn_event).
 */
static EVT_BEGIN(dmg_fall_event_pata)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OffAttribute, LW(10), UNIT_ATTR_FLOATING)
    CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), kIsWinged)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), BTL_POSE_DAMAGE)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_ReplaceParts, LW(10), LW(11), PTR(parts), 1)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_KIND_CHANGED)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), BTL_POSE_DAMAGE)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 10, -1)
    CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ChangeKind, LW(10), 73)
    CALL(btlevtcmd_GetHpDamageCount, LW(10), LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        CALL_CHILD(init_event)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), BTL_POSE_DAMAGE)
        RETURN
    ELSE
        CALL_CHILD(init_event)
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), BTL_POSE_DAMAGE)
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
        // The winged R. Parabuzzy checks for a held item, then picks a target.
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
    // The attack is declared and paid for, and the R. Parabuzzy flies up over its target.
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
 * Pose table of the R. Parabuzzy: maps its battle pose types (stay, walk, hurt, dead, talk and so on)
 * to animation names, 17 entries such as TMT_N_1, TMT_Y_1, TMT_K_1, TMT_X_1.
 */
static PoseTableEntry pose_table[] = {
    {1, "TMT_N_1"},
    {2, "TMT_Y_1"},
    {9, "TMT_Y_1"},
    {5, "TMT_K_1"},
    {4, "TMT_X_1"},
    {3, "TMT_X_1"},
    {BTL_POSE_NORMAL, "TMT_S_1"},
    {29, "TMT_Q_1"},
    {30, "TMT_Q_1"},
    {31, "TMT_A_1"},
    {BTL_POSE_DAMAGE, "TMT_D_1"},
    {50, "TMT_A_1"},
    {POSE_TYPE_WALK, "TMT_W_1"},
    {POSE_TYPE_RUN, "TMT_R_1"},
    {POSE_TYPE_DANGER, "TMT_X_1"},
    {POSE_TYPE_PERIL, "TMT_X_1"},
    {POSE_TYPE_ACTION, "TMT_S_1"},
};

/**
 * Pose table of the R. Parabuzzy hanging from the ceiling:
 * maps its battle pose types (stay, walk, hurt, dead, talk and so on) to animation names,
 * 17 entries such as STM_N_1, STM_Y_1, STM_K_1, STM_X_1.
 */
static PoseTableEntry pose_table_ceil[] = {
    {1, "STM_N_1"},
    {2, "STM_Y_1"},
    {9, "STM_Y_1"},
    {5, "STM_K_1"},
    {4, "STM_X_1"},
    {3, "STM_X_1"},
    {BTL_POSE_NORMAL, "STM_S_1"},
    {29, "STM_Q_1"},
    {30, "STM_Q_1"},
    {31, "STM_A_1"},
    {BTL_POSE_DAMAGE, "STM_D_1"},
    {50, "STM_A_1"},
    {POSE_TYPE_WALK, "STM_W_1"},
    {POSE_TYPE_RUN, "STM_R_1"},
    {POSE_TYPE_DANGER, "STM_X_1"},
    {POSE_TYPE_PERIL, "STM_X_1"},
    {POSE_TYPE_ACTION, "STM_S_1"},
};

/**
 * Pose table of the R. Parabuzzy flipped onto its back: maps its battle pose types (stay, walk, hurt,
 * dead, talk and so on) to animation names, 16 entries such as TMT_Y_2, TMT_S_3, TMT_Q_2, TMT_A_1.
 */
static PoseTableEntry pose_table_turn[] = {
    {1, "TMT_Y_2"},
    {2, "TMT_Y_2"},
    {9, "TMT_Y_2"},
    {5, "TMT_S_3"},
    {4, "TMT_S_3"},
    {3, "TMT_S_3"},
    {BTL_POSE_NORMAL, "TMT_S_3"},
    {29, "TMT_Q_2"},
    {30, "TMT_Q_2"},
    {31, "TMT_A_1"},
    {BTL_POSE_DAMAGE, "TMT_S_3"},
#if VERSION >= VERSION_JP
    {POSE_TYPE_WALK, "TMT_W_1"},
    {POSE_TYPE_RUN, "TMT_R_1"},
#endif
    {POSE_TYPE_DANGER, "TMT_S_3"},
    {POSE_TYPE_PERIL, "TMT_S_3"},
    {POSE_TYPE_ACTION, "TMT_S_3"},
};

/**
 * Event table of the R. Parabuzzy: maps event tags to the scripts that react to them (dmg_turn_event
 * and dmg_avoid_counter_thorn_head_event); tag 0 ends it. Tag 37 is the attacker hurt by spikes.
 */
static BattleDataEntry data_table[] = {
    {13, dmg_turn_event},
    {37, dmg_avoid_counter_thorn_head_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Event table of the R. Parabuzzy hanging from the ceiling:
 * maps event tags to the scripts that react to them (counter_damage_event_ceil); tag 0 ends it.
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

/** Attack data of the R. Parabuzzy's met attack: base damage 3, 100% accuracy. */
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
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
#if VERSION <= VERSION_DEMO_US
    COUNTER_RESIST_ALL,
#else
    COUNTER_RESIST_ELECTRIC | COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_HOLD_FAST, // counterResistFlags
#endif
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

/** Attack data of the R. Parabuzzy's met ceil attack: base damage 5, 100% accuracy. */
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
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
#if VERSION <= VERSION_DEMO_US
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

/**
 * Element reactions of the R. Parabuzzy: fire immune and explosion immune;
 * the other elements hit normally.
 */
static BattleUnitDefenseAttr defence_attr = {{0, 2, 0, 2, 0}};

/**
 * Defence of the R. Parabuzzy flipped onto its back per element (normal, fire, ice, explosion,
 * electric), subtracted from each hit: none.
 */
static BattleUnitDefense defence_turn = {{0, 0, 0, 0, 0}};

/** Element reactions of the R. Parabuzzy flipped onto its back: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr_turn = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of the R. Parabuzzy (the demos), as the percent chance a status takes hold:
 * immune to Burn; always hit by Poison, Electric, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 60, Stop 90, Dizzy 75, Confuse 75, Freeze 75, Tiny 75, Attack Down 75, Defense Down 75,
 * Allergic 75, Fright 60, Gale Force 65, Slow 75 and instant KO 95.
 */
static StatusVulnerability regist = {60, 90, 75, 100, 75, 100, 0, 75, 100, 75, 100, 75, 100, 75, 75, 60, 65, 100, 75, 100, 100, 95};
#else
/**
 * Status vulnerability of the R. Parabuzzy (the final versions),
 * as the percent chance a status takes hold: immune to Burn;
 * always hit by Poison, Electric, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 75, Stop 90, Dizzy 75, Confuse 75, Freeze 75, Tiny 90, Attack Down 90, Defense Down 95,
 * Allergic 75, Fright 60, Gale Force 30, Slow 75 and instant KO 95.
 */
static StatusVulnerability regist = {75, 90, 75, 100, 75, 100, 0, 75, 100, 90, 100, 90, 100, 95, 75, 60, 30, 100, 75, 100, 100, 95};
#endif

/**
 * Body parts of the R. Parabuzzy: a single part (model c_met_h) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_crimson_togemet", "c_met_h", 0.0f, 0.0f, 0.0f, 0.0f, 23.0f, 0.0f, 0.0f, 28.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, kCounterTopSpiky, pose_table},
};

/**
 * Init script of the R. Parabuzzy:
 * reads unit work 0 (1 means it starts on the ceiling) into LW(15),
 * runs init_event2 to install the right handlers and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_ON_CEILING, LW(15))
    CALL_CHILD(init_event2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Installs the handlers for the spiked unit's current form, chosen by unit work 0:
 * on the ground (0) its normal wait, attack, damage and confusion scripts;
 * on the ceiling (1) the ceiling pose and data tables at height 130 with the ceiling scripts
 * and the fall script. Both set the run and walk sounds and clear the flipped-over count.
 */
static EVT_BEGIN(init_event2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_ON_CEILING, LW(15))
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
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HAMMERLIKE_CANNOT_TARGET)
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
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 14, -17, 0)
#endif
    END_IF
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE1L"), STRING("SFX_ENM_MET_MOVE1R"), 0, 3, 3)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE1L"), STRING("SFX_ENM_MET_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetOverTurnCount, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * Spike spark shown when an attacker is hurt by (or avoids) the spiked head:
 * waits 30 frames, then spawns the toge_flush effect above the unit's head (38 units up when Huge, 18 when Tiny,
 * else 28).
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 38)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 18)
        GOTO(90)
    END_IF
    ADD(LW(1), 28)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

/**
 * Damage handler: runs the shared default damage script (btldefaultevt_Damage) with its default parameters
 * (LW(10) = -2, LW(11) = 1).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Flips the spiked unit onto its back: swaps in the flipped defence, pose and attribute tables,
 * marks status flag 1 (flipped) with a flipped-over count of 1, plays the shell sound,
 * lands it in two bounces at height 5 and turns off the spike counter on part 1.
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), BTL_POSE_DAMAGE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(4)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 5, LW(2), 18, -1)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 5, LW(2), 12, -1)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 5, LW(2))
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    RETURN
EVT_END()

/**
 * Turn-over recovery of the spiked unit: counts down the flipped-over count;
 * at 0 it restores the normal defence and pose tables, clears status flag 1, plays the wake-up sound,
 * hops to the ground and turns the spike counter on part 1 back on; otherwise it keeps lying flipped.
 */
static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), -2)
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
    CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
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
    // If the R. Parabuzzy is lying on its back it spends its turn getting back up instead of attacking.
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
    // The attack is declared and paid for, and the R. Parabuzzy turns to face its target.
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
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_DONE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_SPEED, 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_DONE, LW(15))
            IF_INT_EQUAL(LW(15), 1)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_SPEED, LW(14))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(14), 0)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_MS(100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_SPEED, 10)
    WAIT_MS(100)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_SPEED, 20)
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
    // On a miss it rolls past the target, uncurls and hops back.
    LABEL(90)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 100)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, 0, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_SPEED, 10)
    WAIT_MS(250)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_DONE, 1)
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
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_SPEED, 10)
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
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_SPIN_DONE, 1)
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
 * runs the default damage script, and unless the hit knocked it out (damage flag 0x200) without a Life Shroom
 * revive (0x800) it runs ceil_fall_ready_event when it took damage while still on the ceiling, so it falls.
 */
static EVT_BEGIN(damage_event_ceil)
    SET_VALUE(LW(10), -2)
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
 * Counter-damage handler on the ceiling for the spiked unit:
 * leaves the ceiling state (ground pose and data tables, hit offsets, home position,
 * init_event2 for the ground form) and then applies the counter damage.
 */
static EVT_BEGIN(counter_damage_event_ceil)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HAMMERLIKE_CANNOT_TARGET)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 14, 17, 0)
#endif
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL_CHILD(init_event2)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(subsetevt_counter_damage)
    RETURN
EVT_END()

/**
 * Makes the ceiling spiked unit fall: plays the fall sound, drops it to the ground with the landing sound,
 * swaps to the ground tables and runs init_event2,
 * then leaves it lying flipped over (turned tables, flipped-over count 1) after two bounces,
 * with its spike counter turned off.
 */
static EVT_BEGIN(ceil_fall_ready_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetJumpSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 20, LW(2), 15, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEMET_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HAMMERLIKE_CANNOT_TARGET)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_turn))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 14, 17, 0)
#endif
    CALL_CHILD(init_event2)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ACTION)
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 1)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 5, LW(2), 18, -1)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 5, LW(2), 12, -1)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 5, LW(2))
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    RETURN
EVT_END()

/**
 * Attack script on the ceiling for the spiked unit:
 * picks a target, slides along the ceiling over it,
 * drops with the fall and landing sounds and hits it (guard check and damage), waits a second,
 * then jumps back down, restores the ground form (init_event2) and walks home.
 */
// Hanging from the ceiling, the R. Parabuzzy first checks whether to use a held item.
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 20, LW(2), 15, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEMET_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, 0, 0)
    // The damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_ceil_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    // On a miss it lands beside the target.
    LABEL(90)
    GOTO(98)
    // On a hit it lands on the target (guard check and damage).
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_met_ceil_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_ceil_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)
    LABEL(98)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HAMMERLIKE_CANNOT_TARGET)
    // It sways while getting back on its feet, then goes back to its ground form.
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CRIMSON_PATATOGEMET_ON_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 14, 17, 0)
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

/** Idle script for the ceiling form: sets the standard waiting pose from the unit's pose table. */
static EVT_BEGIN(wait_event_ceil)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

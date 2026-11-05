// Battle unit Swooper (`basabasa`): tst.rel's copy of rels/jin/src/unit/unit_basabasa.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_damage.h"
#include "battle/battle_camera.h"
#include "system.h"

// Unit work slots of the Swooper (initial values come from the battle setup).
enum {
    UNIT_WORK_START_FLYING = 0, // 1: starts the battle already in flight instead of hanging from the ceiling
};

BattleUnitKind unit_basabasa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleWeapon weapon_basabasa_drain_attack_biribiri_counterd;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_ceil[18];
static BattleDataEntry data_table[14];
static BattleUnitKindPart parts[1];
static StatusVulnerability regist;
static BattleWeapon weapon_basabasa_attack;
static PoseSoundTiming pose_sound_timing_table[9];

#if VERSION <= VERSION_DEMO_JP
static EvtWord init_event[72];
#else
static EvtWord init_event[78];
#endif

static EvtWord dmg_first_attack_pos[12];
static EvtWord damage_event[10];
static EvtWord damage_event_ceil[51];
static EvtWord counter_damage_event[16];
static EvtWord ceil_fall_ready_event[23];
static EvtWord attack_event[448];
static EvtWord wait_event[6];

/**
 * Unit kind of Swooper: 6 max HP, level 17. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_basabasa = {
    UNIT_SWOOPER,
    "btl_un_basabasa",
    6,
    0,
    1,
    1,
    17,
    0,
    2,
    70,
    0,
    50,
    9999,
    32,
    32,
    6,
    32,
    0.0f,
    16.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    -15.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    0.0f,
    16.0f,
    20.8f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    32.0f,
    32.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_BASA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kCeiling | kUnitLeader | kUnitFaceCamera,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

/**
 * Defence of Swooper per element (normal, fire, ice, explosion, electric), subtracted from each hit:
 * none.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of Swooper: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Unused Swooper drain-attack biribiri counter weapon data: no damage, base accuracy 100. */
static BattleWeapon weapon_basabasa_drain_attack_biribiri_counterd = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
#if VERSION <= VERSION_DEMO_JP
    0, 0, 0,
#else
    0, 0, 2, // baseFpCost..superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, 0, // acHelpMsg..counterResistFlags
    TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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
 * Pose table of Swooper: maps its battle pose types (stay, walk, hurt, sleep, talk and so on) to animation
 * names, 18 entries such as BSA_N_1, BSA_Y_1, BSA_K_1, BSA_X_1.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "BSA_N_1"},
    {POSE_TYPE_STOP, "BSA_Y_1"},
    {POSE_TYPE_FREEZE, "BSA_Y_1"},
    {POSE_TYPE_CONFUSE, "BSA_K_1"},
    {POSE_TYPE_POISON, "BSA_X_1"},
    {POSE_TYPE_DIZZY, "BSA_X_1"},
    {BTL_POSE_NORMAL, "BSA_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "BSA_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "BSA_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "BSA_S_1"},
    {BTL_POSE_DAMAGE, "BSA_D_1"},
    {POSE_TYPE_ATTACK, "BSA_A_1"},
    {POSE_TYPE_RUN, "BSA_R_1"},
    {POSE_TYPE_WALK, "BSA_W_1"},
    {POSE_TYPE_DANGER, "BSA_X_1"},
    {POSE_TYPE_PERIL, "BSA_X_1"},
    {POSE_TYPE_TALK, "BSA_T_1"},
    {POSE_TYPE_ACTION, "BSA_S_1"},
};

/**
 * Pose table of Swooper hanging from the ceiling: maps its battle pose types (stay, walk, hurt, sleep,
 * talk and so on) to animation names, 18 entries such as BSA_N_2, BSA_Y_2, BSA_K_2, BSA_X_2.
 */
static PoseTableEntry pose_table_ceil[] = {
    {POSE_TYPE_SLEEP, "BSA_N_2"},
    {POSE_TYPE_STOP, "BSA_Y_2"},
    {POSE_TYPE_FREEZE, "BSA_Y_2"},
    {POSE_TYPE_CONFUSE, "BSA_K_2"},
    {POSE_TYPE_POISON, "BSA_X_2"},
    {POSE_TYPE_DIZZY, "BSA_X_2"},
    {BTL_POSE_NORMAL, "BSA_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "BSA_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "BSA_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "BSA_S_2"},
    {BTL_POSE_DAMAGE, "BSA_D_2"},
    {POSE_TYPE_ATTACK, "BSA_A_1"},
    {POSE_TYPE_RUN, "BSA_S_1"},
    {POSE_TYPE_WALK, "BSA_S_1"},
    {POSE_TYPE_DANGER, "BSA_X_2"},
    {POSE_TYPE_PERIL, "BSA_X_2"},
    {POSE_TYPE_TALK, "BSA_T_2"},
    {POSE_TYPE_ACTION, "BSA_S_2"},
};

/**
 * Event table of Swooper: maps event tags to the scripts that react to them (counter_damage_event and
 * dmg_first_attack_pos); tag 0 ends it. Tag 48 is a First Strike.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, counter_damage_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0, NULL},
};

/**
 * Body parts of Swooper: a single part (model c_basabasa) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_basabasa", "c_basabasa", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table_ceil},
};

#if VERSION <= VERSION_DEMO_JP

/**
 * Status vulnerability multipliers of Swooper (early-demo branch): 100 for Dizzy, Poison, Electric, Burn,
 * Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible; Gale Force 110;
 * otherwise Sleep 70, Stop 85, Confuse 80, Freeze 80, Tiny 80, Attack Down 80,
 * Defense Down 80, Allergic 80, Fright 80, Slow 80 and instant KO 95.
 */
static StatusVulnerability regist = {70, 85, 100, 100, 80, 100, 100, 80, 100, 80, 100, 80, 100, 80, 80, 80, 110, 100, 80, 100, 100, 95};
#else

/**
 * Status vulnerability multipliers of Swooper (JP-and-later branch): 100 for Poison, Electric, Burn, Huge,
 * Attack Up, Defense Up, Fast, Dodgy and Invisible; Dizzy 105;
 * otherwise Sleep 80, Stop 85, Confuse 80, Freeze 80, Tiny 90, Attack Down 90, Defense Down 95,
 * Allergic 80, Fright 80, Gale Force 95, Slow 80 and instant KO 95.
 */
static StatusVulnerability regist = {80, 85, 105, 100, 80, 100, 100, 80, 100, 90, 100, 90, 100, 95, 80, 80, 95, 100, 80, 100, 100, 95};
#endif

/** Attack data of Swooper's attack: base damage 3, 100% accuracy. */
static BattleWeapon weapon_basabasa_attack = {
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
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    0, // counterResistFlags
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
 * Sound cues of Swooper's animations: for each of its 8 animation names,
 * the sound effect that plays with it (SFX_ENM_BASA_WAIT1 and SFX_ENM_BASA_MOVE1).
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"BSA_S_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_W_1", 0.01f, NULL, "SFX_ENM_BASA_MOVE1", 0, 1},
    {"BSA_R_1", 0.01f, NULL, "SFX_ENM_BASA_MOVE1", 0, 1},
    {"BSA_T_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_A_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_N_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_K_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_X_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Brings the bat down from the ceiling into the open air the first time it is needed:
 * if it is still hanging (unit attribute 2 set) it clears that, sets attribute 4,
 * switches to the flying pose table and selects walk pose (type 40),
 * shifts the unit and its home position down 30 (then another 40 for its home), adjusts the hit,
 * cursor and status icon offsets and hands the damage event over to damage_event.
 * Leaves the home position in LW(0) to LW(2).
 */
static EVT_BEGIN(ceil_sky_event)
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, UNIT_ATTR_FLOATING)
    CALL(btlevtcmd_SetPossessionItemOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, -30, 0)
    CALL(btlevtcmd_AddHomePos, UNIT_ID_SELF, 0, -30, 0)
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 30, 0)
#if VERSION >= VERSION_JP // the demos have no kiss move offsets
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, 21, 0)
#endif
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 6, 32)
    CALL(btlevtcmd_AddHomePos, UNIT_ID_SELF, 0, -40, 0)
    LABEL(99)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

/**
 * Init script of the bat: registers its wait, attack,
 * damage (damage_event_ceil, for while it hangs) and confusion handlers and the ceiling-fall event,
 * sets the pose sound table and the offsets of its hit, cursor and status icon for the upside-down pose,
 * and if it starts in the sky (unit work 0 = 1) runs ceil_sky_event right away; then starts the idle event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_ceil))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, PTR(ceil_fall_ready_event))
    CALL(btlevtcmd_SetPossessionItemOffset, UNIT_ID_SELF, 0, -20, 0)
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, -50, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, -30, 0)
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 6, 10)
#if VERSION >= VERSION_JP // the demos have no kiss move offsets
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, -18, 0)
#endif
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_START_FLYING, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(ceil_sky_event)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Position event for a first strike on the bat:
 * brings it down from the ceiling (ceil_sky_event), sets its height to 10 and waits for the attack to finish.
 */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL_CHILD(ceil_sky_event)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Damage reaction: runs the default damage handling (btldefaultevt_Damage) with LW(10) = -2 (the unit itself)
 * and LW(11) = 1.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Damage reaction of the bat while it still hangs from the ceiling:
 * after the default damage handling, unless the damage code marks zero HP without revival
 * (bits 512 without 2048) or no damage was dealt, it drops from the ceiling (ceil_sky_event)
 * and glides down to its home position over 60 frames.
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
    CALL(btlevtcmd_GetTotalDamage, LW(10), LW(12))
    IF_INT_EQUAL(LW(12), 0)
        RETURN
    END_IF
    CALL_CHILD(ceil_sky_event)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_LINEAR, 0, -1)
    RETURN
EVT_END()

/**
 * Damage reaction when the bat is hurt by a counter effect on its victim:
 * it comes down from the ceiling and then runs the standard counter-damage handling (subsetevt_counter_damage)
 * with the damage code.
 */
static EVT_BEGIN(counter_damage_event)
    CALL_CHILD(ceil_sky_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(subsetevt_counter_damage)
    RETURN
EVT_END()

/**
 * Runs when the ceiling falls on the bat (an attack that drops the ceiling):
 * default damage handling, then it comes down from the ceiling (ceil_sky_event)
 * and glides to its home position over 60 frames.
 */
static EVT_BEGIN(ceil_fall_ready_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    CALL_CHILD(ceil_sky_event)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_LINEAR, 0, -1)
    RETURN
EVT_END()

/**
 * The Swooper's attack: it uses an item or picks a target, drops from the ceiling if it is still hanging,
 * swoops in (pose types 40 and 42) and bites it;
 * a miss lets it pass by, a hit applies the damage with the action-command defence;
 * afterwards it returns to its idle pose BSA_S_1 (BSA_S_2 if it is still on the ceiling).
 */
static EVT_BEGIN(attack_event)

    // The Swooper first checks whether to use a held item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // It picks a target; with none, it ends the turn (or flusters when confused).
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_basabasa_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The attack is declared, the camera follows the Swooper toward its target, and the cost is paid.
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)

    // If it is hanging from the ceiling it drops down; it has already turned toward the target.
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, -30, 0)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.5f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        SUBTRACT(LW(1), 40)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, EASING_LINEAR, 0, -1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), -6)

    // It swoops in, dipping up and down twice before the final dive at the target's position.
    LOOP(2)
        ADD(LW(1), 13)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, 10, EASING_LINEAR, 0, -1)
        ADD(LW(1), -13)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -10, EASING_LINEAR, 0, -1)
    END_LOOP
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, EASING_LINEAR, 0, -1)

    // The damage check decides between a miss (it sails past) and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // On a miss, it flies past the target and out the other side.
    LABEL(90)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_LINEAR, 0, -1)
    WAIT_MS(200)
    GOTO(98)

    // On a hit, the camera shakes, the action-command defence is resolved, the bite damages the target and the Swooper swoops past.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_QUADRATIC_OUT, 0, -1)
    WAIT_MS(200)
    GOTO(98)

    // The Swooper flies back to its home position (up to the ceiling if it came from there).
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(7))
    IF_INT_EQUAL(LW(7), 1)
        SUBTRACT(LW(1), 30)
    END_IF
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_LINEAR, 0, -1)
    LABEL(99)

    // It goes back to its idle pose, hanging (BSA_S_2) if it started on the ceiling, and returns to waiting.
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BSA_S_2"))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, 30, 0)
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BSA_S_1"))
        WAIT_FRAMES(1)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle event: sets the unit's body part 1 to the pose given by its pose table for its current state. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

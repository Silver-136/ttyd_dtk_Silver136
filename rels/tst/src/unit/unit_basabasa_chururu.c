// Battle unit Swoopula (`basabasa_chururu`): tst.rel's copy of rels/jon/src/unit/unit_basabasa_chururu.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_sub.h"

// Unit work slots of the Swoopula.
enum {
    UNIT_WORK_START_FLYING = 0, // 1 (from the battle setup): starts off the ceiling, already flying
};

BattleUnitKind unit_basabasa_chururu;
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
#if VERSION <= VERSION_DEMO_US
static EvtWord init_event[72];
#else
static EvtWord init_event[78];
#endif
static EvtWord dmg_first_attack_pos[12];
static EvtWord damage_event[10];
static EvtWord damage_event_ceil[51];
static EvtWord counter_damage_event[16];
static EvtWord ceil_fall_ready_event[23];
static EvtWord attack_event[683];
static EvtWord wait_event[6];

/**
 * Unit kind of Swoopula: 9 max HP, level 30. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_basabasa_chururu = {
    123,
    "btl_un_basabasa_chururu",
    9,
    0,
    1,
    1,
    30,
    0,
#if VERSION <= VERSION_DEMO_US
    2,
    70,
    2,
#else
    4,
    70,
    0,
#endif
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
 * Defence of Swoopula per element (normal, fire, ice, explosion, electric), subtracted from each hit:
 * none.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of Swoopula: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Attack data of Swoopula's basabasa drain attack biribiri counterd attack: no damage, 100% accuracy. */
static BattleWeapon weapon_basabasa_drain_attack_biribiri_counterd = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
#if VERSION <= VERSION_DEMO_US
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
    ELEMENT_NORMAL, 0, // element..damagePattern
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
 * Pose table of Swoopula: maps its battle pose types (stay, walk, hurt, dead, talk and so on) to
 * animation names, 18 entries such as CYU_N_1, CYU_Y_1, CYU_K_1, CYU_X_1.
 */
static PoseTableEntry pose_table[] = {
    {1, "CYU_N_1"},
    {2, "CYU_Y_1"},
    {9, "CYU_Y_1"},
    {5, "CYU_K_1"},
    {4, "CYU_X_1"},
    {3, "CYU_X_1"},
    {BTL_POSE_NORMAL, "CYU_S_1"},
    {29, "CYU_Q_1"},
    {30, "CYU_Q_1"},
    {31, "CYU_S_1"},
    {BTL_POSE_DAMAGE, "CYU_D_1"},
    {50, "CYU_A_1"},
    {POSE_TYPE_RUN, "CYU_R_1"},
    {POSE_TYPE_WALK, "CYU_W_1"},
    {POSE_TYPE_DANGER, "CYU_X_1"},
    {POSE_TYPE_PERIL, "CYU_X_1"},
    {POSE_TYPE_TALK, "CYU_T_1"},
    {POSE_TYPE_ACTION, "CYU_S_1"},
};

/**
 * Pose table of Swoopula hanging from the ceiling: maps its battle pose types (stay, walk, hurt, dead,
 * talk and so on) to animation names, 18 entries such as CYU_N_2, CYU_Y_2, CYU_K_2, CYU_X_2.
 */
static PoseTableEntry pose_table_ceil[] = {
    {1, "CYU_N_2"},
    {2, "CYU_Y_2"},
    {9, "CYU_Y_2"},
    {5, "CYU_K_2"},
    {4, "CYU_X_2"},
    {3, "CYU_X_2"},
    {BTL_POSE_NORMAL, "CYU_S_2"},
    {29, "CYU_Q_2"},
    {30, "CYU_Q_2"},
    {31, "CYU_S_2"},
    {BTL_POSE_DAMAGE, "CYU_D_2"},
    {50, "CYU_A_1"},
    {POSE_TYPE_RUN, "CYU_S_1"},
    {POSE_TYPE_WALK, "CYU_S_1"},
    {POSE_TYPE_DANGER, "CYU_X_2"},
    {POSE_TYPE_PERIL, "CYU_X_2"},
    {POSE_TYPE_TALK, "CYU_T_2"},
    {POSE_TYPE_ACTION, "CYU_S_2"},
};

/**
 * Event table of Swoopula: maps event tags to the scripts that react to them (counter_damage_event and
 * dmg_first_attack_pos); tag 0 ends it. Tag 48 is a First Strike.
 */
static BattleDataEntry data_table[] = {
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
    {48, dmg_first_attack_pos},
    {0, NULL},
};

/**
 * Body parts of Swoopula: a single part (model c_cyuru) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_basabasa_chururu", "c_cyuru", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table_ceil},
};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of Swoopula (the demos), as the percent chance a status takes hold:
 * always hit by Poison, Electric, Burn, Huge, Attack Up, Defense Up, Gale Force, Fast,
 * Dodgy and Invisible; otherwise Sleep 60, Stop 75, Dizzy 90, Confuse 70, Freeze 70, Tiny 70,
 * Attack Down 70, Defense Down 70, Allergic 70, Fright 70, Slow 70 and instant KO 95.
 */
static StatusVulnerability regist = {60, 75, 90, 100, 70, 100, 100, 70, 100, 70, 100, 70, 100, 70, 70, 70, 100, 100, 70, 100, 100, 95};
#else
/**
 * Status vulnerability of Swoopula (the final versions), as the percent chance a status takes hold:
 * always hit by Dizzy, Poison, Electric, Burn, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 70, Stop 75, Confuse 70, Freeze 70, Tiny 80, Attack Down 80, Defense Down 90,
 * Allergic 70, Fright 70, Gale Force 85, Slow 70 and instant KO 95.
 */
static StatusVulnerability regist = {70, 75, 100, 100, 70, 100, 100, 70, 100, 80, 100, 80, 100, 90, 70, 70, 85, 100, 70, 100, 100, 95};
#endif

/** Attack data of Swoopula's basabasa attack: base damage 4, 100% accuracy. */
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE, // special
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
 * Sound cues of Swoopula's animations: for each of its 9 animation names,
 * the sound effect that plays with it (SFX_ENM_BASA_WAIT1 and SFX_ENM_BASA_MOVE1).
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"CYU_S_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"CYU_W_1", 0.01f, NULL, "SFX_ENM_BASA_MOVE1", 0, 1},
    {"CYU_R_1", 0.01f, NULL, "SFX_ENM_BASA_MOVE1", 0, 1},
    {"CYU_T_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"CYU_A_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"CYU_N_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"CYU_K_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"CYU_X_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Brings the bat down from the ceiling into the open air the first time it is needed:
 * if it is still hanging (unit attribute 2 set) it clears that, sets attribute 4,
 * restores the flying pose table (pose type 40),
 * shifts the unit and its home position down 30 (then another 40 for its home), adjusts the hit,
 * cursor and status icon offsets and hands the damage event over to damage_event.
 * Leaves the home position in LW(0) to LW(2).
 */
/**
 * Brings the bat down from the ceiling into the open air the first time it is needed:
 * if it is still hanging (unit attribute 2 set) it clears that, sets attribute 4,
 * restores the flying pose table (pose type 40),
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
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Damage reaction of the bat while it still hangs from the ceiling:
 * after the default damage handling, if the hit did damage (and was not of the kind that spares it,
 * damage code bits 512 without 2048), it drops from the ceiling (ceil_sky_event)
 * and glides down to its home position over 60 frames.
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
    CALL(btlevtcmd_GetTotalDamage, LW(10), LW(12))
    IF_INT_EQUAL(LW(12), 0)
        RETURN
    END_IF
    CALL_CHILD(ceil_sky_event)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, 0, 0, -1)
    RETURN
EVT_END()

/**
 * Damage reaction when the bat is hurt by a counter effect on its victim:
 * it comes down from the ceiling and then runs the standard counter-damage handling (subsetevt_counter_damage)
 * with the damage code.
 */
static EVT_BEGIN(counter_damage_event)
    CALL_CHILD(ceil_sky_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(subsetevt_counter_damage)
    RETURN
EVT_END()

/**
 * Ceiling-fall event, run when the stage's overhead iron frame falls (_fall_frame_event):
 * default damage handling, then it comes down from the ceiling (ceil_sky_event)
 * and glides to its home position over 60 frames.
 */
static EVT_BEGIN(ceil_fall_ready_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    CALL_CHILD(ceil_sky_event)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, 0, 0, -1)
    RETURN
EVT_END()

/**
 * The Swoopula's draining bite: like the Swooper's swoop,
 * but when the hit lands it tilts into the target (SFX_ENM_BASA_SUCK1, pose CYU_A_2)
 * and heals itself with the recovery effect for the damage it dealt.
 * When the target would counter the bite (btlevtcmd_PreCheckCounter result 14, Electrified, or 17, Hold Fast)
 * it applies weapon_basabasa_drain_attack_biribiri_counterd instead and drains nothing.
 */
static EVT_BEGIN(attack_event)
    // The Swoopula first checks whether to use a held item instead of attacking.
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
    // The attack is declared, the camera follows the Swoopula toward its target, and the cost is paid.
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    // If it is hanging from the ceiling it drops down first, then turns toward the target.
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, -30, 0)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.5f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        SUBTRACT(LW(1), 40)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, 0, 0, -1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), -6)
    // It swoops in, dipping up and down twice before the final dive at the target's position.
    LOOP(2)
        ADD(LW(1), 13)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, 10, 0, 0, -1)
        ADD(LW(1), -13)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -10, 0, 0, -1)
    END_LOOP
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    SUBTRACT(LW(1), 10)
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, 0, 0, -1)
    // The damage check decides between a miss (it sails past) and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
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
    // On a miss, it flies past the target and out the other side.
    LABEL(90)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, 0, 0, -1)
    WAIT_MS(200)
    GOTO(98)
    LABEL(91)
    // On a hit, it checks whether the target would counter the bite (Electrified or Hold Fast); if so, the counter damage is dealt instead and nothing is drained.
    SET_VALUE(LF(0), 0)
    CALL(btlevtcmd_PreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
    IF_INT_EQUAL(LW(5), 17)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(5), 14)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
        SET_VALUE(LW(9), PTR(&weapon_basabasa_drain_attack_biribiri_counterd))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 256, LW(5))
        RETURN
    END_IF
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, -45)
    // The Swoopula latches on with a sucking sound and its CYU_A_2 pose, pulsing in size as it drinks, while the camera shakes and the bite damage lands.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BASA_SUCK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CYU_A_2"))
    LOOP(2)
        LOOP(10)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, 0, FLOAT(0.049804688f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(10)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, 0, FLOAT(-0.049804688f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(2), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0)
    // If the bite did damage, it recovers that much HP and squashes and stretches happily; the recovery number pops up.
    CALL(btlevtcmd_GetDamage, LW(3), LW(10))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CYU_N_1"))
        WAIT_MS(300)
        CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, 1, LW(10))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        ADD(LW(6), 20)
        ADD(LW(7), 10)
        CALL(evt_eff, STRING(""), STRING("recovery"), 7, LW(5), LW(6), LW(7), LW(10), 0, 0, 0, 0, 0, 0, 0)
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 5)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.099609375f), FLOAT(0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 10)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.099609375f), FLOAT(-0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 5)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.099609375f), FLOAT(0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, 1, 1, 1)
        WAIT_MS(1000)
    END_IF
    WAIT_MS(500)
    GOTO(98)
    // The Swoopula flies back to its home position (up to the ceiling if it came from there).
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(7))
    IF_INT_EQUAL(LW(7), 1)
        SUBTRACT(LW(1), 30)
    END_IF
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, 0, 0, -1)
    LABEL(99)
    // It goes back to its idle pose, hanging (CYU_S_2) if it started on the ceiling, and returns to waiting.
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CYU_S_2"))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, 30, 0)
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CYU_S_1"))
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

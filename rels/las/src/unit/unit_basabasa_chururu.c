// Palace of Shadow battle unit: the Swoopula (`basabasa_chururu`): parts, stats, poses and AI scripts.

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
#include "battle/battle_damage.h"
#include "battle/unit/unit_basabasa_chururu.h"

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
static EvtWord ceil_sky_event[98];
static EvtWord init_event[78];
static EvtWord dmg_first_attack_pos[12];
static EvtWord damage_event[10];
static EvtWord damage_event_ceil[51];
static EvtWord counter_damage_event[16];
static EvtWord ceil_fall_ready_event[23];
static EvtWord attack_event[683];
static EvtWord wait_event[6];

/**
 * Unit data of the Swoopula in the Palace of Shadow: 9 HP, level 30;
 * names its parts, status table and init script.
 */
BattleUnitKind unit_basabasa_chururu = {
    UNIT_SWOOPULA, // unitType
    "btl_un_basabasa_chururu", // name
    9, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    30, // level
    0, // bonusExp
    4, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    32, // width
    32, // height
    {6, 32}, // hitOffset
    {0.0f, 16.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {16.0f, 0.0f, -15.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {16.0f, 0.0f, 0.0f}, // bintaHitOffset
    {16.0f, 20.8f, 0.0f}, // kissHitOffset
    {0.0f, 16.0f, 0.0f}, // cutBaseOffset
    32.0f, // cutWidth
    32.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_BASA_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kCeiling | kUnitLeader | kUnitFaceCamera, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * Defence of the Swoopula against each element (normal, fire, ice, explosion, electric):
 * none, so every hit does its full damage.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/**
 * Damage modifiers of the Swoopula per element (normal, fire, ice, explosion, electric):
 * none, so no element is a weakness, an immunity or a heal.
 */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Weapon of the Swoopula's bite when the target is electrified or held fast, which drains nothing. */
static BattleWeapon weapon_basabasa_drain_attack_biribiri_counterd = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
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

/** Pose table of the Swoopula: animation names for each pose type (idle, walk, hurt, attack and so on). */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "CYU_N_1"},
    {POSE_TYPE_STOP, "CYU_Y_1"},
    {POSE_TYPE_FREEZE, "CYU_Y_1"},
    {POSE_TYPE_CONFUSE, "CYU_K_1"},
    {POSE_TYPE_POISON, "CYU_X_1"},
    {POSE_TYPE_DIZZY, "CYU_X_1"},
    {POSE_TYPE_NORMAL, "CYU_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "CYU_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "CYU_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "CYU_S_1"},
    {POSE_TYPE_DAMAGE, "CYU_D_1"},
    {POSE_TYPE_ATTACK, "CYU_A_1"},
    {POSE_TYPE_RUN, "CYU_R_1"},
    {POSE_TYPE_WALK, "CYU_W_1"},
    {POSE_TYPE_DANGER, "CYU_X_1"},
    {POSE_TYPE_PERIL, "CYU_X_1"},
    {POSE_TYPE_TALK, "CYU_T_1"},
    {POSE_TYPE_ACTION, "CYU_S_1"},
};

/**
 * Pose table of the Swoopula while hanging from the ceiling:
 * animation names for each pose type (idle, walk, hurt, attack and so on).
 */
static PoseTableEntry pose_table_ceil[] = {
    {POSE_TYPE_SLEEP, "CYU_N_2"},
    {POSE_TYPE_STOP, "CYU_Y_2"},
    {POSE_TYPE_FREEZE, "CYU_Y_2"},
    {POSE_TYPE_CONFUSE, "CYU_K_2"},
    {POSE_TYPE_POISON, "CYU_X_2"},
    {POSE_TYPE_DIZZY, "CYU_X_2"},
    {POSE_TYPE_NORMAL, "CYU_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "CYU_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "CYU_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "CYU_S_2"},
    {POSE_TYPE_DAMAGE, "CYU_D_2"},
    {POSE_TYPE_ATTACK, "CYU_A_1"},
    {POSE_TYPE_RUN, "CYU_S_1"},
    {POSE_TYPE_WALK, "CYU_S_1"},
    {POSE_TYPE_DANGER, "CYU_X_2"},
    {POSE_TYPE_PERIL, "CYU_X_2"},
    {POSE_TYPE_TALK, "CYU_T_2"},
    {POSE_TYPE_ACTION, "CYU_S_2"},
};

/**
 * Data table of the Swoopula: the counter-damage handlers and
 * first-strike-positioning handler the battle looks up by tag.
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

/** Part of the Swoopula: its one body part with hit box, defence tables and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_basabasa_chururu", "c_cyuru", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table_ceil},
};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {70, 75, 90, 100, 70, 100, 100, 70, 100, 70, 100, 70, 100, 70, 70, 70, 100, 100, 70, 100, 100, 95};
#else
static StatusVulnerability regist = {
    70, // sleep
    75, // stop
    100, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    90, // defenseDown
    70, // allergic
    70, // fright
    85, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Weapon of the Swoopula's draining bite: damage 4. */
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
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

/** Sound table of the Swoopula: for each pose name, the sound that plays with its animation. */
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
 * Brings a Swoopula down from the ceiling when it is hit, countered, the ceiling falls, or init starts it flying:
 * if its ceiling attribute is still set (attribute 2) it clears it, sets the flying attribute (4),
 * switches to the flying pose table and moves it 30 units down with new hit, cursor,
 * kiss and status icon offsets. Leaves its home position in LW(0) to LW(2).
 */
static EVT_BEGIN(ceil_sky_event)
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kFloating)
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
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, 21, 0)
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 6, 32)
    CALL(btlevtcmd_AddHomePos, UNIT_ID_SELF, 0, -40, 0)
    LABEL(99)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

/**
 * Init event of the Swoopula: installs wait, attack, ceiling damage (damage_event_ceil),
 * confusion and ceiling-fall (ceil_fall_ready_event, run when the stage's overhead iron frame falls) events
 * and the hang-from-ceiling hit offsets;
 * if UNIT_WORK_START_FLYING is 1 it starts already in flight (ceil_sky_event).
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
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, -18, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_START_FLYING, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(ceil_sky_event)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Data table entry 48, run when the battle opens with a First Strike on the Swoopula (damage code 0x39):
 * drops from the ceiling (ceil_sky_event), sets its height to 10 and waits for the attack to end.
 */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL_CHILD(ceil_sky_event)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/** Damage event of the Swoopula: runs the default damage handling. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Damage event of the Swoopula while it hangs from the ceiling:
 * runs the default damage handling and then, unless the hit leaves it at zero HP without revival (512 without 2048)
 * or did no damage, makes it drop to its flying position (ceil_sky_event) and dive down.
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
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

/**
 * Damage event used for the Swoopula's counter-damage case:
 * drops it from the ceiling (ceil_sky_event) and runs the shared counter damage handling
 * (subsetevt_counter_damage).
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
 * Ceiling-fall event of the Swoopula, run when the stage's overhead iron frame falls:
 * runs the default damage handling, then drops it to its flying position (ceil_sky_event) and dives down.
 */
static EVT_BEGIN(ceil_fall_ready_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    CALL_CHILD(ceil_sky_event)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

/**
 * The Swoopula's draining bite: like the Swoopula's swoop,
 * but when the hit lands it tilts into the target (SFX_ENM_BASA_SUCK1, pose CYU_A_2)
 * and heals itself with the recovery effect for the damage it dealt.
 * When the counter check (btlevtcmd_PreCheckCounter) finds the target electrified (14) or under Hold Fast (17),
 * it skips the drain and applies weapon_basabasa_drain_attack_biribiri_counterd instead.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Pick a target for the attack; a confused unit that has none flusters.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_basabasa_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF

        // BUG: this skips the -30 drop of a ceiling Swoopula, but LABEL(99) still adds 30 when kCeiling is on,
        // so it ends 30 above its spot.
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_basabasa_attack))

    // Announce the attack; the camera swings to follow it.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 1)

        // Take off and fly toward the target in a dive.
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, -30, 0)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.5f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        SUBTRACT(LW(1), 40)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), -6)
    LOOP(2)
        ADD(LW(1), 13)

        // Swoop in on the target.
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, 10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        ADD(LW(1), -13)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    END_LOOP
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    SUBTRACT(LW(1), 10)
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)

    // Find out whether the attack will connect or miss.
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

    // The attack misses or is dodged.
    LABEL(90)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    ADD(LW(1), 10)

    // The Swoopula swoops on past the target.
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(200)
    GOTO(98)
    LABEL(91)
    SET_VALUE(LF(0), 0)

    // A target that is electrified or under Hold Fast gets a no-damage variant of the bite, with no drain.
    CALL(btlevtcmd_PreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
    IF_INT_EQUAL(LW(5), CHECK_COUNTER_HOLD_FAST)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(5), CHECK_COUNTER_ELECTRIFIED)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)

        // Record the action command defence and run the zero-damage bite that triggers the counter.
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
        SET_VALUE(LW(9), PTR(&weapon_basabasa_drain_attack_biribiri_counterd))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF

    // On contact it tilts into the target and sinks its fangs in.
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, -45)
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
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(2), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_LINEAR, 0, MOVE_SOUND_NONE)
    CALL(btlevtcmd_GetDamage, LW(3), LW(10))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CYU_N_1"))
        WAIT_MS(300)

        // It heals itself for the damage it dealt, with the recovery effect.
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

    // Return to its place.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(7))
    IF_INT_EQUAL(LW(7), 1)
        SUBTRACT(LW(1), 30)
    END_IF
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)

    // The attack is over; back to waiting.
    LABEL(99)
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

/** Wait (idle) event of the Swoopula: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

// Battle unit Frost Piranha (`ice_pakkun`): tst.rel's copy of rels/bom/src/unit/unit_ice_pakkun.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "tst_battle.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_eff.h"
#include "system.h"
#include "battle/battle_damage.h"

static USER_FUNC(shadow_scale_memory);
static USER_FUNC(shadow_scale_return);
static USER_FUNC(shadow_scale_small);
static USER_FUNC(pakkun_ceil_exist);

BattleUnitKind unit_ice_pakkun;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_ice_pakkun_attack;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_ceil[18];
static BattleDataEntry data_table[15];
static EvtWord init_event[31];
static EvtWord dmg_avoid_counter_thorn_head_event[71];
static EvtWord damage_event[10];
static EvtWord counter_damage_event[30];

#if VERSION <= VERSION_DEMO_JP
static EvtWord return_event[244];
#else
static EvtWord return_event[256];
#endif

static EvtWord normal_attack_event[311];
static EvtWord wait_event[6];

/**
 * Unit definition of the Frost Piranha: 10 HP, level 26, 1 part, with its sounds, init script and data table.
 */
BattleUnitKind unit_ice_pakkun = {
    113,
    "btl_un_ice_pakkun",
    10,
    0,
    1,
    1,
    26,
    0,
#if VERSION <= VERSION_DEMO_JP
    2,
    70,
    2,
#else
    3,
    70,
    1,
#endif
    50,
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

/** Defence of the Frost Piranha: no flat defence; elemental reactions are separate. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/**
 * Damage-type modifiers of the Frost Piranha: it takes 1 extra damage from fire attacks; is healed by ice
 * attacks.
 */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_WEAK, DEFENSE_ATTR_ABSORB, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP

/**
 * Status vulnerability of the Frost Piranha (100% is the normal chance): immune to Freeze; resists Sleep 80%,
 * Stop 80%, Dizzy 60%, Confusion 75%, Tiny 70%, Attack Down 70%, Defense Down 70%, Allergic 70%, Fright 20%,
 * Gale Force 30%, Slow 70% and instant KO 95%.
 */
static StatusVulnerability regist = {80, 80, 60, 100, 75, 100, 100, 0, 100, 70, 100, 70, 100, 70, 70, 20, 30, 100, 70, 100, 100, 95};
#else
static StatusVulnerability regist = {100, 70, 40, 100, 80, 100, 100, 0, 100, 85, 100, 85, 100, 85, 70, 10, 40, 100, 70, 100, 100, 95};
#endif

/**
 * Body of the Frost Piranha: one targetable part, the model `c_pakflwr_a`, with its hit box, defence tables
 * and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_ice_pakkun", "c_pakflwr_a", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0x1, pose_table},
};

/**
 * Attack data of the Frost Piranha's bite: normal_attack_event uses it against one selected opposing unit
 * for 5 damage; may inflict Freeze (30%, 2
 * turns).
 */
static BattleWeapon weapon_ice_pakkun_attack = {
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, // burnTime
    30, // freezeChance
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
 * Attack script of the Frost Piranha: when battle flag 2 is set it runs normal_attack_event;
 * otherwise it uses a selected item event, or its normal attack (weapon_ice_pakkun_attack) if none is returned.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_ice_pakkun_attack))
        CALL_CHILD(normal_attack_event)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_ice_pakkun_attack))
    CALL_CHILD(normal_attack_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Pose table of the Frost Piranha: its 18 battle poses (status, hurt and action) and the animation each one
 * plays; the idle animation is `PKF_S_1`.
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
 * Pose table of the Frost Piranha while it hangs from the ceiling: its 18 battle poses (status, hurt and
 * action) and the animation each one plays; the idle animation is `PKF_S_3`.
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
 * Data table of the Frost Piranha: event 37 runs dmg_avoid_counter_thorn_head_event when an attacker is hurt
 * by its spikes; events 25 to 36 run counter_damage_event; event 48 does nothing (btldefaultevt_Dummy).
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
 * Init script of the Frost Piranha: installs the wait, attack,
 * damage and confusion handlers (confusion reuses the attack), sets the hit cursor offset to 50,
 * records the shadow scale (shadow_scale_memory) and starts the wait event.
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
 * Data-table event 37, run by the default damage event when an attacker is hurt by the Frost Piranha's spiky head
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
 * Damage handler for when the Frost Piranha was hurt by a counter:
 * plays PKF_D_1 for a second, and unless the hit knocked it out (damage flag 0x200) without a Life Shroom revive
 * (0x800) it burrows back to its place with return_event.
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
 * Called when the Frost Piranha is knocked off the ceiling:
 * it clears the ceiling fall handler, burrows (PKF_E_4, shrinking its shadow),
 * reappears on the ground at height 0 (PKF_E_1), restores the ground hit offsets and pose table,
 * and returns to the PKF_S_1 idle pose.
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
 * Returns the Frost Piranha to its home after an attack:
 * it burrows (PKF_E_2, shrinking its shadow) and,
 * depending on whether a ceiling spot exists (pakkun_ceil_exist) and a random pick,
 * reappears either on the ground (PKF_E_1, ground tables) or hanging from the ceiling at height 130 (PKF_E_3,
 * ceiling hit offsets and pose table, with the ceiling fall handler).
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
 * The Frost Piranha's normal attack (weapon_ice_pakkun_attack):
 * it burrows (PKF_E_2, or PKF_E_4 from the ceiling) and pops up in front of the target,
 * then bites in three poses (PKF_A_1A to A_1C) with the attack sound;
 * the usual avoid, guard and hit check follows, with a camera shake on a hit,
 * and it returns home with return_event.
 */
static EVT_BEGIN(normal_attack_event)

    // Picks the target: the preset first-attack target (after a short pause) when battle flag 2 is set, otherwise a
    // random opposing unit; with none it does nothing (or is flustered when confused).
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
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

    // Declares the attack and pays its cost.
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))

    // Burrow: the plant ducks out of sight (hanging from the ceiling, it pulls up out of view instead) and its shadow
    // shrinks away.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
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

    // Unseen, it moves to a spot beside the target and turns to face it, then the camera zooms in.
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 45)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)

    // It pops back up out of the ground with a rustle as its shadow returns.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_return, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_E_1"))
    WAIT_MS(150)

    // Wind-up and bite: it rears back, then snaps its jaws shut on the target.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_1A"))
    WAIT_MS(200)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PAKKUN_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_1B"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PKF_A_1C"))

    // Checks the outcome of the hit: a miss or dodge goes to the no-damage branch; a connecting hit applies
    // the Action Command defence result and deals the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // Miss branch: the bite finds nothing.
    LABEL(90)
    GOTO(98)

    // Hit branch: the camera shakes and pulls in, the defence result is read and the damage is applied.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)

    // Pauses, then returns to its home spot with the shared return event.
    LABEL(98)
    WAIT_MS(500)
    CALL_CHILD(return_event)
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
#if VERSION <= VERSION_DEMO_JP
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

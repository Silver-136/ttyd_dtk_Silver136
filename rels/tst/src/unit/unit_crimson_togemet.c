// Battle unit Red Spike Top (`crimson_togemet`, "R. S. Buzzy"): tst.rel's copy of rels/tou2/src/unit/unit_crimson_togemet.c.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_CEILING 0    // setup value: nonzero = starts on the ceiling (init_event2 sets the ceiling tables and events); the fall/turn events clear it
#define UNIT_WORK_SPIN_DONE 1  // 1 ends the sibling loop that spins it during the attack (and resets the rotation)
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that spin (0, 10, 20)


BattleUnitKind unit_crimson_togemet;
static PoseTableEntry pose_table[17];
static PoseTableEntry pose_table_ceil[17];

#if VERSION == VERSION_DEMO_JP
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
 * Unit data of the Red Spike Top: 5 HP and level 17, with its parts, status vulnerability, init event and
 * event data table.
 */
BattleUnitKind unit_crimson_togemet = {
    UNIT_RED_SPIKY_BUZZY, // unitType
    "btl_un_crimson_togemet", // name
    5, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    17, // level
    0, // bonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    28, // width
    26, // height
    {5, 20}, // hitOffset
    {0.0f, 13.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {14.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {14.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 16.0f, 0.0f}, // kissHitOffset
    {0.0f, 13.0f, 0.0f}, // cutBaseOffset
    28.0f, // cutWidth
    26.0f, // cutHeight
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
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "TMT_N_1"},
    {POSE_TYPE_STOP, "TMT_Y_1"},
    {POSE_TYPE_FREEZE, "TMT_Y_1"},
    {POSE_TYPE_CONFUSE, "TMT_K_1"},
    {POSE_TYPE_POISON, "TMT_X_1"},
    {POSE_TYPE_DIZZY, "TMT_X_1"},
    {POSE_TYPE_NORMAL, "TMT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "TMT_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TMT_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "TMT_A_1"},
    {POSE_TYPE_DAMAGE, "TMT_D_1"},
    {POSE_TYPE_ATTACK, "TMT_A_1"},
    {POSE_TYPE_WALK, "TMT_W_1"},
    {POSE_TYPE_RUN, "TMT_R_1"},
    {POSE_TYPE_DANGER, "TMT_X_1"},
    {POSE_TYPE_PERIL, "TMT_X_1"},
    {POSE_TYPE_ACTION, "TMT_S_1"},
};

static PoseTableEntry pose_table_ceil[] = {
    {POSE_TYPE_SLEEP, "STM_N_1"},
    {POSE_TYPE_STOP, "STM_Y_1"},
    {POSE_TYPE_FREEZE, "STM_Y_1"},
    {POSE_TYPE_CONFUSE, "STM_K_1"},
    {POSE_TYPE_POISON, "STM_X_1"},
    {POSE_TYPE_DIZZY, "STM_X_1"},
    {POSE_TYPE_NORMAL, "STM_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "STM_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "STM_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "STM_A_1"},
    {POSE_TYPE_DAMAGE, "STM_D_1"},
    {POSE_TYPE_ATTACK, "STM_A_1"},
    {POSE_TYPE_WALK, "STM_W_1"},
    {POSE_TYPE_RUN, "STM_R_1"},
    {POSE_TYPE_DANGER, "STM_X_1"},
    {POSE_TYPE_PERIL, "STM_X_1"},
    {POSE_TYPE_ACTION, "STM_S_1"},
};

static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "TMT_Y_2"},
    {POSE_TYPE_STOP, "TMT_Y_2"},
    {POSE_TYPE_FREEZE, "TMT_Y_2"},
    {POSE_TYPE_CONFUSE, "TMT_S_3"},
    {POSE_TYPE_POISON, "TMT_S_3"},
    {POSE_TYPE_DIZZY, "TMT_S_3"},
    {POSE_TYPE_NORMAL, "TMT_S_3"},
    {POSE_TYPE_DAMAGE_FIRE, "TMT_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TMT_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "TMT_A_1"},
    {POSE_TYPE_DAMAGE, "TMT_S_3"},
#if VERSION > VERSION_DEMO_US
    {POSE_TYPE_WALK, "TMT_W_1"},
    {POSE_TYPE_RUN, "TMT_R_1"},
#endif
    {POSE_TYPE_DANGER, "TMT_S_3"},
    {POSE_TYPE_PERIL, "TMT_S_3"},
    {POSE_TYPE_ACTION, "TMT_S_3"},
};

/**
 * Event data table of the Red Spike Top: scripts that replace the standard handling for being flipped over
 * by a stomp and an attacker being hurt by its spiked head.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_head_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Event data table of the Red Spike Top while it hangs from the ceiling: scripts that replace the standard
 * handling for the counter-damage reactions (from the ceiling).
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

/** Weapon record of the Red Spike Top's spinning shell attack: 3 damage. */
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
#if VERSION <= VERSION_DEMO_US
    COUNTER_RESIST_ALL,
#else
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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

/** Weapon record of the Red Spike Top's drop from the ceiling: 5 damage and counts as a jump-type hit. */
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
#if VERSION <= VERSION_DEMO_US
    COUNTER_RESIST_ALL,
#else
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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
 * Defence of the Red Spike Top: 4 against normal, ice and electric hits;
 * immune to fire and explosions (defence 99 cancels the hit).
 */
static BattleUnitDefense defence = {{4, 99, 4, 99, 4}};

/** Damage-type reactions of the Red Spike Top: immune to fire and explosion. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE}};

/**
 * Defense of the Red Spike Top against normal, fire, ice, explosion and electric attacks while it is
 * flipped onto its back: none, so it takes full damage from every element.
 */
static BattleUnitDefense defence_turn = {0};

/**
 * Damage-type reactions of the Red Spike Top while it is flipped onto its back: none, so every element
 * hurts it normally.
 */
static BattleUnitDefenseAttr defence_attr_turn = {0};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {60, 90, 75, 100, 75, 100, 0, 75, 100, 75, 100, 75, 100, 75, 75, 60, 65, 100, 75, 100, 100, 95};
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

/**
 * Parts of the Red Spike Top: a single body part (part 1, model c_met_h), each with its defense tables,
 * attributes and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_crimson_togemet", "c_met_h", 0.0f, 0.0f, 0.0f, 0.0f, 23.0f, 0.0f, 0.0f, 28.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, kCounterTopSpiky, pose_table},
};

/**
 * Init event of the Red Spike Top: reads unit work 0 (0 = on the floor,
 * anything else = hanging from the ceiling),
 * applies the matching event set with init_event2, and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CEILING, LW(15))
    CALL_CHILD(PTR(init_event2))
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
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CEILING, LW(15))
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
#if VERSION > VERSION_DEMO_US
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 14, -17, 0)
#endif
    END_IF
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE1L"), STRING("SFX_ENM_MET_MOVE1R"), 0, 3, 3)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_MET_MOVE1L"), STRING("SFX_ENM_MET_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetOverTurnCount, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * Data event 37 (the thorn-head counter, run when a player attack is bounced off its spiked top):
 * waits 30 frames and flashes the "toge_flush" spike effect above the unit,
 * 38 units up when it is Huge (STATUS_HUGE), 18 when Tiny (status
 * 11) and 28 otherwise.
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

/** Damage event: runs the default battle damage reaction on the unit itself (part 1). */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Flips the spiked unit onto its back: swaps in the flipped defence, pose and attribute tables,
 * marks status flag 1 (flipped) with a flipped-over count of 1, plays the shell sound,
 * lands it in two bounces at height 5 and turns off the spike counter on part 1.
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 5, LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 5, LW(2), 12, MOVE_SOUND_FROM_TABLE)
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
    CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Attack event of the floor Red Spike Top: a flipped unit runs wakeup_event.
 * Otherwise it uses an enemy item if one fits, or picks a target (the first-strike target in a first strike)
 * and curls into its shell, spins up in steps to a rolling charge (leaving smoke)
 * and rolls into the target with weapon_met_attack.
 * A miss rolls on past and turns back; a hit bounces it off in two hops. Then it returns home.
 */
static EVT_BEGIN(attack_event)

    // If the Red Spike Top is flipped over, it first rights itself with wakeup_event.
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF

    // Use the target of the opening first attack if there was one; otherwise use an enemy item, or choose a target.
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_met_attack))

    // The attack is announced; the Red Spike Top turns to face its target.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_met_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(100)

    // The Red Spike Top hops up and tucks into its shell.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_SHELL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_ATTACK2"), EVT_NULLPTR, 0, LW(15))

    // It begins spinning in its shell, faster and faster.
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
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD(LW(1), 10)
            CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.2998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT

    // The spinning shell rolls across the stage into the target.
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, MOVE_SOUND_NONE, 0)
    DELETE_EVENT(LW(13))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)

    // Find out how the target reacts to the spinning shell.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // The shell missed: it rolls on past the target, slows down, then hops out of the shell and lands.
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

    // The shell connects: the damage is applied and the Red Spike Top bounces back in two hops.
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

    // The Red Spike Top walks back to its home position.
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

/** Wait event: sets the idle animation from the unit's pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Damage event while hanging from the ceiling: default damage reaction, then,
 * unless the hit knocked it out for good or did no damage, it falls with ceil_fall_ready_event.
 */
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
 * Counter-damage handler on the ceiling for the spiked unit:
 * leaves the ceiling state (ground pose and data tables, hit offsets, home position,
 * init_event2 for the ground form) and then applies the counter damage.
 */
static EVT_BEGIN(counter_damage_event_ceil)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION > VERSION_DEMO_US
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 14, 17, 0)
#endif
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
 * Ceiling-fall event: the Red Spike Top drops from the ceiling with the fall sound, lands (landing sound),
 * switches to the floor tables and offsets (init_event2) and ends up flipped on its back like
 * after a stomp (flipped defence and pose tables, over-turn counter 1, small bounces).
 */
static EVT_BEGIN(ceil_fall_ready_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetJumpSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 20, LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEMET_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_turn))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION > VERSION_DEMO_US
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 14, 17, 0)
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 5, LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 5, LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 5, LW(2))
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    RETURN
EVT_END()

/**
 * Attack event while hanging from the ceiling: picks a target, moves over it along the ceiling,
 * lets go and drops onto it (weapon_met_ceil_attack), then waits 60 frames and returns to the floor tables.
 * It hops twice near the target, then walks home and sets up the floor events again with init_event2.
 */
static EVT_BEGIN(attack_event_ceil)

    // Use the target of the opening first attack if there was one; otherwise use an enemy item, or choose a target.
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
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_met_ceil_attack))

    // The attack is announced; the Red Spike Top, hanging from the ceiling, turns to face its target.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_met_ceil_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)

    // It crawls along the ceiling until it is above the target.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(11), LW(12), 60, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    WAIT_MS(250)

    // The Red Spike Top lets go and drops spikes-first onto the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MET_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetJumpSpeed, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 20, LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEMET_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)

    // Find out how the target reacts to the drop.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_ceil_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    GOTO(98)

    // The drop connects: the damage is applied.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_met_ceil_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_met_ceil_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)

    // Now on the ground, the Red Spike Top switches back to its floor data (poses, hit points, events), then hops back toward its home.
    LABEL(98)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CEILING, 0)
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 0, 0)
#if VERSION > VERSION_DEMO_US
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 14, 17, 0)
#endif
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
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

/** Wait event while hanging: sets the idle pose from the ceiling pose table. */
static EVT_BEGIN(wait_event_ceil)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()


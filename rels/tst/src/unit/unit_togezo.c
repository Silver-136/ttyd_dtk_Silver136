// Battle unit Spiny (`togezo`): tst.rel's copy of rels/jon/src/unit/unit_togezo.c.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"
#include "battle/unit/unit_togezo.h"

BattleUnitKind unit_togezo;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitDefense defence_turn;
static BattleUnitDefenseAttr defence_attr_turn;
static StatusVulnerability regist_turn;
static BattleUnitDefense defence_guard;
static BattleUnitDefenseAttr defence_attr_guard;
static StatusVulnerability regist_guard;
static PoseTableEntry pose_table[15];

#if VERSION <= VERSION_DEMO_JP
static PoseTableEntry pose_table_turn[12];
#else
static PoseTableEntry pose_table_turn[14];
#endif

static PoseTableEntry pose_table_guard[12];
static BattleDataEntry data_table[5];
static BattleDataEntry data_table_turn[3];
static BattleDataEntry data_table_guard[4];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_togezo_attack;
static EvtWord init_event[52];
static EvtWord dmg_avoid_counter_thorn_event[61];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[84];
static EvtWord wakeup_event[90];
static EvtWord attack_event[71];
static EvtWord normal_attack_event[813];
static EvtWord guard_event[95];
static EvtWord guard_cancel_event[89];
static EvtWord wait_event[6];

/**
 * Unit data of the Spiny: 3 HP, level 9;
 * names its parts, status table and init script.
 */
BattleUnitKind unit_togezo = {
    UNIT_SPINY, // unitType
    "btl_un_togezo", // name
    3, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    9, // level
    0, // bonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    32, // width
    30, // height
    {4, 30}, // hitOffset
    {0.0f, 15.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {16.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {16.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 16.0f, 0.0f}, // kissHitOffset
    {0.0f, 15.0f, 0.0f}, // cutBaseOffset
    32.0f, // cutWidth
    30.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_TOGEZO_DAMAGED1", // damageSfxName
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

/**
 * Defence of the Spiny against each element (normal, fire, ice, explosion, electric):
 * 3 against every element, so each hit is cut by 3.
 */
static BattleUnitDefense defence = {{3, 3, 3, 3, 3}};

/**
 * Damage modifiers of the Spiny per element (normal, fire, ice, explosion, electric):
 * none, so no element is a weakness, an immunity or a heal.
 */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {90, 90, 90, 100, 90, 100, 100, 80, 100, 90, 100, 90, 100, 90, 90, 100, 80, 100, 90, 100, 100, 100};
#else
static StatusVulnerability regist = {
    80, // sleep
    90, // stop
    80, // dizzy
    100, // poison
    80, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    90, // allergic
    100, // fright
    30, // galeForce
    100, // fast
    90, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};
#endif

/**
 * Defence of the Spiny while flipped over against each element (normal, fire, ice, explosion, electric):
 * none, so every hit does its full damage.
 */
static BattleUnitDefense defence_turn = {0};

/**
 * Damage modifiers of the Spiny while flipped over per element (normal, fire, ice, explosion, electric):
 * none, so no element is a weakness, an immunity or a heal.
 */
static BattleUnitDefenseAttr defence_attr_turn = {0};

/**
 * Status vulnerability of the Spiny while flipped over:
 * percentage multiplier on each move's status chance (100 is normal).
 */
static StatusVulnerability regist_turn = {
    100, // sleep
    100, // stop
    100, // dizzy
    100, // poison
    100, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    100, // defenseDown
    100, // allergic
    100, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};

/**
 * Defence of the Spiny while it is curled up guarding:
 * defence 99 against every kind of hit, so no attack does damage.
 */
static BattleUnitDefense defence_guard = {{99, 99, 99, 99, 99}};

/**
 * Damage modifiers of the Spiny while guarding per element (normal, fire, ice, explosion, electric): normal:
 * immune, fire: immune, ice: immune, explosion: immune, electric: immune.
 */
static BattleUnitDefenseAttr defence_attr_guard = {{DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE}};

/**
 * Status vulnerability of the Spiny while guarding:
 * percentage multiplier on each move's status chance (100 is normal).
 */
static StatusVulnerability regist_guard = {
    100, // sleep
    100, // stop
    100, // dizzy
    100, // poison
    100, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    100, // defenseDown
    100, // allergic
    100, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};

/** Pose table of the Spiny: animation names for each pose type (idle, walk, hurt, attack and so on). */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "TGZ_N_1"},
    {POSE_TYPE_STOP, "TGZ_Y_1"},
    {POSE_TYPE_FREEZE, "TGZ_Y_1"},
    {POSE_TYPE_CONFUSE, "TGZ_K_1"},
    {POSE_TYPE_POISON, "TGZ_X_1"},
    {POSE_TYPE_DIZZY, "TGZ_X_1"},
    {POSE_TYPE_NORMAL, "TGZ_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "TGZ_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TGZ_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGZ_A_1"},
    {POSE_TYPE_DAMAGE, "TGZ_D_1"},
    {POSE_TYPE_WALK, "TGZ_W_1"},
    {POSE_TYPE_RUN, "TGZ_R_1"},
    {POSE_TYPE_TALK, "TGZ_T_1"},
    {POSE_TYPE_ACTION, "TGZ_S_1"},
};

/**
 * Pose table of the Spiny while flipped over: animation names for each pose type (idle, walk, hurt, attack
 * and so on).
 */
static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "TGZ_N_2"},
    {POSE_TYPE_STOP, "TGZ_Y_2"},
    {POSE_TYPE_FREEZE, "TGZ_Y_2"},
    {POSE_TYPE_CONFUSE, "TGZ_K_2"},
    {POSE_TYPE_POISON, "TGZ_X_2"},
    {POSE_TYPE_DIZZY, "TGZ_X_2"},
    {POSE_TYPE_NORMAL, "TGZ_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "TGZ_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TGZ_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGZ_S_1"},
    {POSE_TYPE_DAMAGE, "TGZ_D_3"},
#if VERSION >= VERSION_JP
    {POSE_TYPE_WALK, "TGZ_W_1"},
    {POSE_TYPE_RUN, "TGZ_R_1"},
#endif
    {POSE_TYPE_ACTION, "TGZ_S_2"},
};

/**
 * Pose table of the Spiny while guarding: animation names for each pose type (idle, walk, hurt, attack and
 * so on).
 */
static PoseTableEntry pose_table_guard[] = {
    {POSE_TYPE_SLEEP, "TGZ_P_1"},
    {POSE_TYPE_STOP, "TGZ_P_1"},
    {POSE_TYPE_FREEZE, "TGZ_P_1"},
    {POSE_TYPE_CONFUSE, "TGZ_P_1"},
    {POSE_TYPE_POISON, "TGZ_P_1"},
    {POSE_TYPE_DIZZY, "TGZ_P_1"},
    {POSE_TYPE_NORMAL, "TGZ_P_1"},
    {POSE_TYPE_DAMAGE_FIRE, "TGZ_P_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TGZ_P_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGZ_P_1"},
    {POSE_TYPE_DAMAGE, "TGZ_P_1"},
    {POSE_TYPE_ACTION, "TGZ_P_1"},
};

/**
 * Data table of the Spiny: event scripts and values the battle looks up by tag (damage reactions, dodge,
 * first strike and so on).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_event},
    {UNIT_DATA_AVOID_THORN_EVENT, dmg_avoid_counter_thorn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Data table of the Spiny while flipped over: event scripts
 * and values the battle looks up by tag (damage reactions, dodge, first strike and so on).
 */
static BattleDataEntry data_table_turn[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/**
 * Data table of the Spiny while guarding: event scripts
 * and values the battle looks up by tag (damage reactions, dodge, first strike and so on).
 */
static BattleDataEntry data_table_guard[] = {
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_event},
    {UNIT_DATA_AVOID_THORN_EVENT, dmg_avoid_counter_thorn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/** Part of the Spiny: its one body part with hit box, defence tables and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_togezo", "c_togezo", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kIsShelled, kCounterTopSpiky | kCounterFrontSpiky, pose_table},
};

/** Weapon of the Spiny's roll: damage 3. */
static BattleWeapon weapon_togezo_attack = {
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
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
#if VERSION <= VERSION_DEMO_JP
    COUNTER_RESIST_ALL,
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
#else
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x2004, // targetWeightFlags
#endif
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
 * Init event of the Spiny: installs its wait, attack, damage and confusion events,
 * clears unit work 0 (guarding) and 3, and sets its run, walk and jump sounds.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_MOVE1L"), STRING("SFX_ENM_TOGEZO_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_MOVE1L"), STRING("SFX_ENM_TOGEZO_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_JUMP1"), STRING("SFX_ENM_KURI_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Waits half a second, then shows the toge_flush spike flash above the Spiny;
 * it is higher for a big (STATUS_HUGE) than a normal Spiny and lower for a small (STATUS_TINY) one.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 45)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 25)
        GOTO(90)
    END_IF
    ADD(LW(1), 35)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

/** Damage event of the Spiny: runs the default damage handling with LW(10) = -2 and LW(11) = 1. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Flips the Spiny onto its back: swaps in the turned defence, defence-attribute, status-vulnerability,
 * pose and data tables, sets the flipped status flag,
 * turns off the part's counter attributes (the spikes no longer counter),
 * sets the over-turn count to 1 and hops twice with the inside sound.
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_turn))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_OffPartsCounterAttribute, LW(10), LW(11), kCounterTopSpiky)
    CALL(btlevtcmd_OffPartsCounterAttribute, LW(10), LW(11), kCounterFrontSpiky)
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table_turn))
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(evt_snd_sfxon, STRING("SFX_CONDITION_INSIDE"), 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

/**
 * Counts the over-turn down by one; while it is still nonzero, returns to waiting.
 * At zero it flips the Spiny back onto its feet, restores the normal tables and counter attributes
 * (the reverse of dmg_turn_event) and hops back up.
 */
static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, LW(10))
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_OnPartsCounterAttribute, LW(10), LW(11), kCounterTopSpiky)
    CALL(btlevtcmd_OnPartsCounterAttribute, LW(10), LW(11), kCounterFrontSpiky)
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ACTION)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

/**
 * Attack event of the Spiny: if flipped over it only wakes up.
 * Otherwise it uses an enemy item if the item-use check selects one;
 * a guarding Spiny (unit work 0 = 1) leaves its guard, and an unguarded one rolls 0 to 99:
 * below 50 the roll attack (normal_attack_event), else it curls up to guard (guard_event).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(wakeup_event))
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(guard_cancel_event))
        GOTO(99)
    ELSE
        SET_VALUE(LW(0), 50)
        ADD(LW(0), 50)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 50)
            CALL_CHILD(PTR(normal_attack_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(guard_event))
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Spiny's roll attack (weapon_togezo_attack): it curls up (TGZ_P_1, TURN1 sound),
 * rolls at the target spinning in z (ROLL sounds),
 * and either misses and rolls past or hits with the defence check and rebounds with two hops (JUMP1),
 * then returns home, either uncurled (TURN2, TGZ_S_1) or in guard after 30% of hits.
 */
static EVT_BEGIN(normal_attack_event)

    // Pick a target for the attack; a confused unit that has none flusters.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_togezo_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_togezo_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togezo_attack))

    // Announce the attack; the camera swings to follow it.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_togezo_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(14))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(14))

    // The Spiny curls up into a ball and spins up on the spot.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_TURN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
    LOOP(5)
        SET_VALUE(LW(0), -36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_P_1"))
    LOOP(5)
        SET_VALUE(LW(0), -36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLLING, 1)
    SET_VALUE(LW(0), -36)
    MULTIPLY(LW(0), LW(14))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, LW(0))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLLING, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, LW(0))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(10)

    // It rolls at the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 30, 0, FLOAT(0.14941406f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(9.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)

    // Find out whether the attack will connect or miss.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togezo_attack), 0, LW(5))
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

    // It rolls on past the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), -18)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, LW(0))
        SET_VALUE(LW(0), 1)
        MULTIPLY(LW(0), LW(14))
        LOOP(18)
            CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, LW(0))
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 26)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLLING, 0)
    WAIT_FRAMES(10)
    GOTO(97)

    // The attack connects.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL4"), EVT_NULLPTR, 0, EVT_NULLPTR)

    // Deal the damage, reduced by the action command defence.
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togezo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togezo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SET_VALUE(LW(0), 36)
    MULTIPLY(LW(0), LW(14))
    // BUG: stores the literal -36 instead of LW(0) (36 times the face direction), so the rebound spin matches
    // LW(0) only when LW(14) is -1; facing right it keeps the roll's direction.
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, -36)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    ADD(LW(2), 10)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 30, 0, FLOAT(0.5f))

    // It rebounds off the target with two hops.
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 15, 0, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), 18)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, LW(0))
        SET_VALUE(LW(0), -1)
        MULTIPLY(LW(0), LW(14))
        LOOP(18)
            CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, LW(0))
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 18)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 9)
    ADD(LW(2), 5)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLLING, 0)
    WAIT_FRAMES(3)
    CALL(evt_sub_random, 99, LW(0))
    IF_INT_LESS(LW(0), 30)
        GOTO(98)
    ELSE
        GOTO(97)
    END_IF
    LABEL(97)

    // It uncurls and turns back to its normal pose.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_TURN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
    LOOP(5)
        SET_VALUE(LW(0), 36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_S_1"))
    LOOP(5)
        SET_VALUE(LW(0), 36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_S_1"))
    WAIT_MS(250)
    GOTO(99)
    LABEL(98)

    // It rolls back toward its place.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLLING, 1)
    SET_VALUE(LW(0), 36)
    MULTIPLY(LW(0), LW(14))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, LW(0))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLLING, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLL_SPEED, LW(0))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ROLLING, 0)
    CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_guard))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr_guard))

    // Back home, it settles into its guarded form (guard defence tables, poses and data table).
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_guard))
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_guard))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_guard))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kIsShelled)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 1)
    WAIT_MS(250)
    GOTO(99)

    // The attack is over; back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Spiny curls into a spiky guard: it spins 360 degrees (TURN1 sound) as it switches to the TGZ_P_1 pose,
 * then swaps in the guard defence, defence-attribute, status, pose and data tables,
 * turns off the part's attribute 0x1000 and sets unit work 0 to 1.
 */
static EVT_BEGIN(guard_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_TURN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
    LOOP(5)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_P_1"))
    LOOP(5)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_guard))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_guard))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist_guard))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_guard))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table_guard))
    CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), kIsShelled)
    CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_GUARD, 1)
    RETURN
EVT_END()

/**
 * Undoes guard_event: the Spiny spins back the other way (TURN2 sound) into TGZ_S_1, restores the normal tables,
 * turns the part's attribute 0x1000 back on and clears unit work 0.
 */
static EVT_BEGIN(guard_cancel_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_TURN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
    LOOP(5)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_S_1"))
    LOOP(5)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), kIsShelled)
    CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_GUARD, 0)
    RETURN
EVT_END()

/** Idle event: sets the unit's body part 1 to the pose given by its pose table for its current state. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()


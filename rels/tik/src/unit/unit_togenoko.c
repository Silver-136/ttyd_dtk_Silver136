// Rogueport Sewers (tik) battle unit: Koopatrol (togenoko): its parts, defence/status tables, and attack events.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"

#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"
#include "battle/unit/unit_togenoko.h"

static USER_FUNC(togenoko_getnum);

BattleUnitKind unit_togenoko;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitDefense defence_turn;
static BattleUnitDefenseAttr defence_attr_turn;
static StatusVulnerability regist_turn;
static BattleUnitDefense defence_guard;
static BattleUnitDefenseAttr defence_attr_guard;
static StatusVulnerability regist_guard;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_turn[15];
static PoseTableEntry pose_table_guard[15];
static PoseTableEntry pose_table_guard_turn[15];
static BattleDataEntry data_table[4];
static BattleDataEntry data_table_turn[3];
static BattleDataEntry data_table_guard[4];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_togenoko_normal_attack;
static BattleWeapon weapon_togenoko_shell_attack;
static BattleWeapon weapon_togenoko_charge;
static EvtWord init_event[37];
static EvtWord damage_event[10];
static EvtWord dmg_avoid_counter_thorn_head_event[61];
static EvtWord dmg_turn_event[116];
static EvtWord wakeup_event[104];
static BattleUnitSetup entry_togenoko;
static EvtWord attack_event[333];
static EvtWord normal_attack_event[406];
static EvtWord shell_attack_event[489];
static EvtWord charge_event[99];
static EvtWord guard_event[127];
static EvtWord guard_cancel_event[90];
static EvtWord friend_event[88];
static EvtWord wait_event[6];

/**
 * Unit kind of the Koopatrol (HP 6, level 26): its size, offsets, damage sounds, status vulnerability,
 * parts and init event.
 */
BattleUnitKind unit_togenoko = {
    UNIT_KOOPATROL, // unitType
    "btl_un_togenoko", // name
    6, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    26, // level
    0, // bonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    36, // width
    50, // height
    {-4, 50}, // hitOffset
    {0.0f, 25.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {18.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {18.0f, 0.0f, 0.0f}, // bintaHitOffset
    {13.0f, 32.0f, 0.0f}, // kissHitOffset
    {0.0f, 25.0f, 0.0f}, // cutBaseOffset
    36.0f, // cutWidth
    50.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_TOGENOKO_DAMAGED1", // damageSfxName
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

/** Defence of the Koopatrol: 2 damage points off every hit. */
static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

/** Element reactions of the Koopatrol: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Status vulnerability of the Koopatrol (percentage multipliers on status-infliction chances): resists most statuses. */
#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {70, 70, 60, 100, 60, 100, 100, 60, 100, 70, 100, 70, 100, 70, 70, 60, 60, 100, 60, 100, 100, 95};
#else
static StatusVulnerability regist = {
    70, // sleep
    75, // stop
    75, // dizzy
    100, // poison
    75, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    70, // fright
    70, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Defence of the Koopatrol (flipped over): no damage reduction. */
static BattleUnitDefense defence_turn = {0};

/** Element reactions of the Koopatrol (flipped over): no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr_turn = {0};

/**
 * Status vulnerability of the Koopatrol (flipped over) (percentage multipliers on status-infliction chances):
 * average vulnerability to every status.
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
 * Defence of the Koopatrol while it is withdrawn into its shell:
 * defence 99 against every kind of hit, so no attack does damage.
 */
static BattleUnitDefense defence_guard = {{99, 99, 99, 99, 99}};

/**
 * Element reactions of the Koopatrol (guarding): normal: no damage, fire: no damage, ice: no damage, explosion:
 * no damage, electric: no damage.
 */
static BattleUnitDefenseAttr defence_attr_guard = {{DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_IMMUNE}};

/**
 * Status vulnerability of the Koopatrol (guarding) (percentage multipliers on status-infliction chances):
 * average vulnerability to every status.
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
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "N_2"},
    {POSE_TYPE_STOP, "Z_2"},
    {POSE_TYPE_FREEZE, "Z_2"},
    {POSE_TYPE_CONFUSE, "K_2"},
    {POSE_TYPE_POISON, "S_2"},
    {POSE_TYPE_DIZZY, "S_2"},
    {POSE_TYPE_NORMAL, "S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_DANGER, "S_2"},
    {POSE_TYPE_PERIL, "S_2"},
    {POSE_TYPE_TALK, "S_2"},
    {POSE_TYPE_ACTION, "S_2"},
};

static PoseTableEntry pose_table_guard[] = {
    {POSE_TYPE_SLEEP, "A_1"},
    {POSE_TYPE_STOP, "A_1"},
    {POSE_TYPE_FREEZE, "A_1"},
    {POSE_TYPE_CONFUSE, "A_1"},
    {POSE_TYPE_POISON, "A_1"},
    {POSE_TYPE_DIZZY, "A_1"},
    {POSE_TYPE_NORMAL, "A_1"},
    {POSE_TYPE_DAMAGE_FIRE, "A_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "A_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "A_1"},
    {POSE_TYPE_DAMAGE, "A_1"},
    {POSE_TYPE_DANGER, "A_1"},
    {POSE_TYPE_PERIL, "A_1"},
    {POSE_TYPE_TALK, "A_1"},
    {POSE_TYPE_ACTION, "A_1"},
};

static PoseTableEntry pose_table_guard_turn[] = {
    {POSE_TYPE_SLEEP, "G_1"},
    {POSE_TYPE_STOP, "G_1"},
    {POSE_TYPE_FREEZE, "G_1"},
    {POSE_TYPE_CONFUSE, "G_1"},
    {POSE_TYPE_POISON, "G_1"},
    {POSE_TYPE_DIZZY, "G_1"},
    {POSE_TYPE_NORMAL, "G_1"},
    {POSE_TYPE_DAMAGE_FIRE, "G_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "G_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "G_1"},
    {POSE_TYPE_DAMAGE, "G_1"},
    {POSE_TYPE_DANGER, "G_1"},
    {POSE_TYPE_PERIL, "G_1"},
    {POSE_TYPE_TALK, "G_1"},
    {POSE_TYPE_ACTION, "G_1"},
};

/**
 * Data table of the Koopatrol: event handlers for being flipped over,
 * the attacker hurt by its spikes and a First Strike on it (no reaction).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_head_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Data table of the Koopatrol while flipped on its back: the flip-over reaction;
 * a First Strike on it is ignored.
 */
static BattleDataEntry data_table_turn[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/**
 * Data table of the Koopatrol while guarding in its shell:
 * the spike counter, the flip-over reaction and an ignored First Strike.
 */
static BattleDataEntry data_table_guard[] = {
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_head_event},
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/** Body part of the Koopatrol (model c_togenoko): hit position, defences, attributes and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_togenoko", "c_togenoko", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kIsShelled, kCounterTopSpiky, pose_table},
};

/** Weapon of the Koopatrol's normal attack: 4 damage, hammer-type contact attack. */
static BattleWeapon weapon_togenoko_normal_attack = {
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
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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

/** Weapon of the Koopatrol's shell attack: 4 damage, shell-toss-type attack. */
static BattleWeapon weapon_togenoko_shell_attack = {
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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

/** Weapon of the Koopatrol's charge: no HP damage. */
static BattleWeapon weapon_togenoko_charge = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    2, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Init script of the Koopatrol. Registers its scripts and sets its run and walk sounds. */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE3"), STRING("SFX_ENM_TOGENOKO_MOVE4"), 0, 3, 3)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE3"), STRING("SFX_ENM_TOGENOKO_MOVE4"), 0, 6, 6)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage reaction of the Koopatrol: the default damage script. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Data event 37, run by the default damage event for damage code 0x1F (the name: an attack avoided by the
 * thorn-head counter, i.e. bounced off its spiked helmet): waits 30 frames
 * and flashes the "toge_flush" spike effect for 60 frames above the unit,
 * 70 units up when it is Huge (STATUS_HUGE),
 * 40 when Tiny (STATUS_TINY) and 55 otherwise.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 70)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 40)
        GOTO(90)
    END_IF
    ADD(LW(1), 55)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, -20, 0)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_turn))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist_turn))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    ELSE
        CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_guard_turn))
        CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ACTION)
    END_IF
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_OffPartsCounterAttribute, LW(10), LW(11), kCounterTopSpiky)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ACTION)
    RETURN
EVT_END()

// Recovery consumes this attack opportunity, including the turn that restores the standing stance.
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
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_OnPartsCounterAttribute, LW(10), LW(11), kCounterTopSpiky)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("S_1"))
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, 0, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 0)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

/** Spawn setup for a Koopatrol: starts off-screen on the enemy side until it is moved into place. */
static BattleUnitSetup entry_togenoko = {
    &unit_togenoko, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Attack script of the Koopatrol.
 * Chooses between the normal attack, the shell attack, the charge, guarding and calling a friend,
 * based on its state, the number of occupied Koopatrol slots (togenoko_getnum) and random chances.
 */
static EVT_BEGIN(attack_event)
    // In a First Strike it just attacks normally.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_togenoko_normal_attack))
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF

    // A Koopatrol flipped onto its back spends its turn getting back up.
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(wakeup_event))
        RETURN
    END_IF

    // A guarding Koopatrol usually lowers its guard on its turn.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(0), 80)
        ADD(LW(0), 20)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 80)
            CALL_CHILD(PTR(guard_cancel_event))
            GOTO(99)
        END_IF
        GOTO(99)
    ELSE
        // A Koopatrol that is not guarding checks its state: a charged one chooses equally between its normal
        // and shell attacks, otherwise it may use an item.
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CHARGE, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(0), 0)
            ADD(LW(0), 25)
            ADD(LW(0), 25)
            SUBTRACT(LW(0), 1)
            CALL(evt_sub_random, LW(0), LW(1))
            SET_VALUE(LW(0), 25)
            IF_INT_LESS(LW(1), LW(0))
                SET_VALUE(LW(9), PTR(&weapon_togenoko_normal_attack))
                CALL_CHILD(PTR(normal_attack_event))
                GOTO(99)
            END_IF
            SET_VALUE(LW(9), PTR(&weapon_togenoko_shell_attack))
            CALL_CHILD(PTR(shell_attack_event))
            GOTO(99)
        END_IF
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF

        // It looks for free room on the stage near itself, since a friend needs room to join the fight.
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        LOOP(0)
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), 0)
                SUBTRACT(LW(2), 10)
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                IF_INT_EQUAL(LW(3), -1)
                    GOTO(10)
                END_IF
            ELSE
                GOTO(9)
            END_IF
        END_LOOP
        LABEL(9)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        LOOP(0)
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
            IF_INT_LESS_OR_EQUAL(LW(0), 130)
                ADD(LW(2), 10)
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                IF_INT_EQUAL(LW(3), -1)
                    GOTO(10)
                END_IF
            ELSE
                GOTO(11)
            END_IF
        END_LOOP

        // With room to spare, it may call a friend to join (the fewer occupied slots hold Koopatrols, the likelier).
        LABEL(10)
        CALL(togenoko_getnum, LW(10))
        CALL(evt_sub_random, 99, LW(11))
        SWITCH(LW(10))
            CASE_EQUAL(1)
            IF_INT_LESS(LW(11), 40)
                CALL_CHILD(PTR(friend_event))
                GOTO(99)
            END_IF
            CASE_END
            CASE_EQUAL(2)
            IF_INT_LESS(LW(11), 20)
                CALL_CHILD(PTR(friend_event))
                GOTO(99)
            END_IF
            CASE_END
            CASE_EQUAL(3)
            IF_INT_LESS(LW(11), 10)
                CALL_CHILD(PTR(friend_event))
                GOTO(99)
            END_IF
            CASE_END
        END_SWITCH

        // Otherwise it picks its move at random: the normal attack, the shell attack, the charge or guarding.
        LABEL(11)
        SET_VALUE(LW(0), 0)
        ADD(LW(0), 25)
        ADD(LW(0), 25)
        ADD(LW(0), 25)
        ADD(LW(0), 25)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        SET_VALUE(LW(0), 25)
        IF_INT_LESS(LW(1), LW(0))
            SET_VALUE(LW(9), PTR(&weapon_togenoko_normal_attack))
            CALL_CHILD(PTR(normal_attack_event))
            GOTO(99)
        END_IF
        ADD(LW(0), 25)
        IF_INT_LESS(LW(1), LW(0))
            SET_VALUE(LW(9), PTR(&weapon_togenoko_shell_attack))
            CALL_CHILD(PTR(shell_attack_event))
            GOTO(99)
        END_IF
        ADD(LW(0), 25)
        IF_INT_LESS(LW(1), LW(0))
            SET_VALUE(LW(9), PTR(&weapon_togenoko_charge))
            CALL_CHILD(PTR(charge_event))
            GOTO(99)
        END_IF
        CALL_CHILD(PTR(guard_event))
        GOTO(99)
    END_IF

    // The move is done and the Koopatrol returns to waiting.
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Koopatrol's normal attack: it winds up (R_3), then lunges at the target (A_2) with SFX_ENM_TOGENOKO_MOVE5,
 * and the pre-damage check decides between a miss and a hit; on a hit it then hops back.
 */
static EVT_BEGIN(normal_attack_event)
    // Choose the target: the First Strike target if the battle opened with one, otherwise a normal pick.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF

    // No target left: a confused Koopatrol is flustered; either way it ends its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The attack is announced and protecting-unit movement is awaited, and the camera frames the Koopatrol
    // and its target.
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)

    // It takes its stamping R_3 pose, then lunges at the target with its spear.
    BEGIN_SIBLING_EVENT
        LOOP(10)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(3)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_3"))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)

    // Check how the target defends against the thrust.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
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

    // The thrust missed or was dodged: the Koopatrol runs on past, reappears from the far side and walks home.
    LABEL(90)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 500)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, -1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 300, LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    GOTO(98)

    // The thrust hits: the camera shakes and the damage is dealt, then it hops back toward home in two hops.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
    SUBTRACT_FLOAT(LW(10), LW(0))
    SUBTRACT_FLOAT(LW(11), LW(1))
    SUBTRACT_FLOAT(LW(12), LW(2))
    MULTIPLY_FLOAT(LW(10), FLOAT(0.75f))
    MULTIPLY_FLOAT(LW(11), FLOAT(0.75f))
    MULTIPLY_FLOAT(LW(12), FLOAT(0.75f))
    ADD_FLOAT(LW(0), LW(10))
    ADD_FLOAT(LW(1), LW(11))
    ADD_FLOAT(LW(2), LW(12))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, 0, FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, 0, FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)

    // It settles at home.
    LABEL(98)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    WAIT_MS(500)
    RETURN
EVT_END()

/**
 * The Koopatrol's shell attack (weapon in LW(9)):
 * it hops into its shell (A_1, SFX_ENM_TOGENOKO_SHELL1), spins up
 * (10, 20, then 40 degrees per frame) and slides through every selected target in turn,
 * each one avoiding or taking
 * the damage (the action-command result only once), trailing dust; it slides 500 units on, reappears at x 250,
 * slides home and pops out of the shell (SHELL2).
 */
static EVT_BEGIN(shell_attack_event)
    // Choose the targets for the shell attack; a confused Koopatrol with none is flustered.
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

    // The camera frames the Koopatrol and its target, and the attack is announced.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(100)

    // The Koopatrol hops into its shell.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_SHELL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))

    // It spins up in three steps, faster each time.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 10)
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
    WAIT_MS(200)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 20)
    WAIT_MS(200)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 40)
    WAIT_MS(200)

    // It starts its slide, kicking up dust.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD(LW(1), 10)
            CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.2998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(12.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, MOVE_SOUND_NONE, 0)
    SET_VALUE(LW(6), 0)

    // It slides through each target in turn: missed or dodging targets are slipped past, while connecting hits deal damage.
    LABEL(10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(12.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 100)
    // BUG: LW(5) held this unit's Y, but PreCheckDamage overwrote it with the result code (2..6 here), so the
    // move goes to that height.
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, MOVE_SOUND_NONE, 0)
    GOTO(97)
    LABEL(91)

    // Resolve the defense command once, at the first selected target that passes the damage precheck.
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        SET_VALUE(LW(6), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(10)
    END_IF

    // After the last target it slides on off the stage, reappears on the far side and slides home.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 500)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(12.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    WAIT_MS(500)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 250, LW(1), LW(2))

    // It pops out of its shell at home.
    LABEL(98)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(12.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_SHELL2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(10.0f), FLOAT(0.39941406f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 1)
    WAIT_FRAMES(3)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Koopatrol's Charge move: it targets only itself with weapon_togenoko_charge (charge strength 2),
 * playing SFX_ENM_TOGENOKO_CHARGE1 with the charge effect, so its next attack hits 2 harder.
 */
static EVT_BEGIN(charge_event)
    SET_VALUE(LW(3), UNIT_ID_SELF)
    SET_VALUE(LW(4), 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togenoko_charge))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_togenoko_charge))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        LOOP(8)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(3)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_3"))
    WAIT_FRAMES(48)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(60)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    END_INLINE_EVENT
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togenoko_charge))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togenoko_charge), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

/**
 * The Koopatrol pulls into its shell to guard (SHELL1, A_1): it hops and wobbles, then takes the guarding
 * defence, defence-attribute, status (regist_guard), pose and data tables,
 * sets unit work 0 to 1 and lowers its hit
 * offset.
 */
static EVT_BEGIN(guard_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_SHELL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(5)
        CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("A_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, LW(10), 20, 0, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    SET_FLOAT_VALUE(LW(12), FLOAT(30.0f))
    SET_FLOAT_VALUE(LW(13), FLOAT(0.0f))
    LOOP(15)
        CALL(evt_sub_get_sincos, LW(13), LW(14), LW(15))
        MULTIPLY_FLOAT(LW(14), LW(12))
        CALL(btlevtcmd_SetRotate, LW(10), 0, 0, LW(14))
        SUBTRACT_FLOAT(LW(12), FLOAT(2.0f))
        ADD_FLOAT(LW(13), FLOAT(30.0f))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_guard))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_guard))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist_guard))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_guard))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table_guard))
    CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_GUARD, 1)
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, -20, 0)
    WAIT_MS(300)
    RETURN
EVT_END()

/**
 * The Koopatrol comes out of guard (SHELL2, S_1): it hops and restores the normal defence, defence-attribute,
 * status, pose and data tables, clears unit work 0 and resets its hit offset.
 */
static EVT_BEGIN(guard_cancel_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_SHELL2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(5)
        CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("S_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, LW(10), 20, 0, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_MOVE6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_GUARD, 0)
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, 0, 0)
    WAIT_MS(300)
    RETURN
EVT_END()

/**
 * The Koopatrol calls a friend (SFX_ENM_TOGENOKO_CALL1, pose B_1):
 * another Koopatrol is spawned (entry_togenoko), runs in and takes its place.
 */
static EVT_BEGIN(friend_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGENOKO_CALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("B_1"))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_togenoko), 0)
    CALL(btlevtcmd_SetPos, LW(3), 300, LW(1), LW(2))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("R_1"))
    CALL(btlevtcmd_SetMoveSpeed, LW(3), 8)
    CALL(btlevtcmd_MovePosition, LW(3), LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("S_1"))
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(300)
    RETURN
EVT_END()

/** Wait script of the Koopatrol: shows its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

// Count occupied slots whose current kind is Koopatrol, without filtering alliance or HP.
static USER_FUNC(togenoko_getnum) {
    EvtWord* args = event->args;
    s32 num = 0;
    s32 i;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (unit != NULL && unit->currentKind == UNIT_KOOPATROL) {
            num++;
        }
    }
    evtSetValue(event, args[0], num);
    return EVT_RETURN_DONE;
}

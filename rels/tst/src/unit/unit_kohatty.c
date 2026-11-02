// kohatty (unit kind 207): the helper hatty spawns, on the yellow Goomba model; it only waits and takes damage.
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"

BattleUnitKind unit_kohatty;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[11];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[7];
BattleWeapon weapon_kohatty_attack;
static EvtWord init_event[45];
static EvtWord damage_event[12];
static EvtWord attack_event[7];
static EvtWord wait_event[6];

/**
 * Unit definition of the `kohatty` helper unit: 4 HP, level 1, 7 parts, with its sounds, init script and data
 * table.
 */
BattleUnitKind unit_kohatty = {
    207,
    "btl_un_kohatty",
    4,
    0,
    1,
    1,
    1,
    0,
    1,
    0,
    0,
    100,
    9,
    24,
    24,
    24,
    24,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    0.0f,
    12.0f,
    15.6f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BTL_DAMAGE1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    UNIT_ATTR_FLOATING,
    &regist,
    7,
    parts,
    init_event,
    data_table,
};

/** Defence of the `kohatty` helper unit: none, so it takes full damage from every kind of attack. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Damage-type modifiers of the `kohatty` helper unit: none, no special weakness or immunity. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the `kohatty` helper unit (100% is the normal chance): takes every status at the
 * normal rate.
 */
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

/**
 * Pose table of the `kohatty` helper unit: its 11 battle poses (status, hurt and action) and the animation
 * each one plays; the idle animation is `KIL_S_1`.
 */
static PoseTableEntry pose_table[] = {
    {2, "KIL_Z_1"},
    {9, "KIL_Z_1"},
    {BTL_POSE_NORMAL, "KIL_S_1"},
    {29, "KIL_Q_1"},
    {30, "KIL_Q_1"},
    {31, "KIL_A_1"},
    {BTL_POSE_DAMAGE, "KIL_D_1"},
    {50, "KIL_A_1"},
    {POSE_TYPE_WALK, "KIL_W_1"},
    {POSE_TYPE_RUN, "KIL_R_1"},
    {POSE_TYPE_ACTION, "KIL_S_1"},
};

/** Data table of the `kohatty` helper unit: event 48 does nothing (btldefaultevt_Dummy). */
static BattleDataEntry data_table[] = {
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Parts of the `kohatty` helper unit: 7 parts sharing the model `c_kuribo_y`, each with its hit box, defence
 * tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_kohatty", "c_kuribo_y", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET | PART_ATTR_HIDDEN, 0, pose_table},
    {2, "btl_un_kohatty", "c_kuribo_y", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table},
    {3, "btl_un_kohatty", "c_kuribo_y", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table},
    {4, "btl_un_kohatty", "c_kuribo_y", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table},
    {5, "btl_un_kohatty", "c_kuribo_y", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table},
    {6, "btl_un_kohatty", "c_kuribo_y", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table},
    {7, "btl_un_kohatty", "c_kuribo_y", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table},
};

/** Attack data of the `kohatty` helper unit's attack: hits one opposing unit for 1 damage. */
BattleWeapon weapon_kohatty_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    0, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
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
 * Battle init script for kohatty: installs the wait,
 * attack and damage handlers (confusion uses the default one), shrinks the model with a base scale of about 0.3,
 * starts the wait event and lifts the unit and its home position to y = 10.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetBaseScale, UNIT_ID_SELF, FLOAT(0.2998047f), FLOAT(0.2998047f), FLOAT(1.0f))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 10, LW(2))
    RETURN
EVT_END()

/**
 * Damage handler: selects this unit (-2) and its part 1
 * and runs the shared default damage reaction (btldefaultevt_Damage).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
EVT_END()

/** Empty attack: kohatty has no attack, so its turn only restarts the wait event. */
static EVT_BEGIN(attack_event)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Wait (idle) event: refreshes the body animation (part 1) from the unit's pose table
 * so it matches the current state, then returns; the battle system runs it whenever the unit is idle.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

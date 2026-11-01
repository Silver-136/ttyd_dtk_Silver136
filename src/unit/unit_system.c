// Battle unit data for the invisible system unit (unit_system) that runs battle-wide scripts.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_sub.h"

static BattleUnitKindPart parts[1];
BattleUnitKind unit_system;
static EvtWord init_event[21];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleDataEntry data_table[1];
static StatusVulnerability regist;
static EvtWord damage_event[2];
static EvtWord attack_event[2];
static EvtWord wait_event[2];

/** The system unit's single part: hidden, untargetable and without a shadow, so it never appears on stage. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_system", NULL, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kHidden | kNoShadow, 0, NULL},
};

/**
 * The invisible system unit that the battle enters before Mario (entryunit_system in battle.c).
 * It has no HP gauge, locked HP and is ignored when checking for a cleared battle;
 * it exists to run battle-wide scripts.
 */
#include "version.h"
#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
BattleUnitKind unit_system = {
    UNIT_SYSTEM, // unitType
    "btl_un_system", // name
    999, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    0, // level
    0, // bonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    0, // runRate
    9, // pbSoftCap
    24, // width
    24, // height
    {0, 20}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 0.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {12.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 15.6f, 0.0f}, // kissHitOffset
    {0.0f, 0.0f, 0.0f}, // cutBaseOffset
    0.0f, // cutWidth
    0.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BTL_DAMAGE1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kNoGravityBob | kActsInZeroGravity | kUnitNoHpGauge | kUnitNoncombatant | kUnitHpFixed, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * Init for the invisible system unit that Btl_UnitSetup enters before Mario (entryunit_system):
 * registers the empty wait, attack and damage scripts, never moves, and idles.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Defence 0 against every element; it never takes hits anyway. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element response of the system unit: immune to every kind of hit. */
static BattleUnitDefenseAttr defence_attr = {{2, 2, 2, 2, 2}};

/** Data events for the system unit: none. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Status chances for the system unit: all 0, so no status can ever land on it. */
#if VERSION > VERSION_DEMO_US
static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    0, // poison
    0, // confuse
    0, // electric
    0, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    0, // defenseDown
    0, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};
#endif

/** Hit reaction: nothing; the system unit cannot be hurt (its HP is locked). */
static EVT_BEGIN(damage_event)
    RETURN
EVT_END()

/** The system unit never acts on its own turn; empty. */
static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

/** Idle: nothing. */
static EVT_BEGIN(wait_event)
    RETURN
EVT_END()


// Keelhaul Key (muj) battle unit: the Bone Pile (honeduka) of the Cortez fight: its parts, defence/status tables, and
// events.
// No formation or script of this REL uses it.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_sub.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_OWNER 0 // unit id whose UNIT_DATA_WEAPON_BREAK_EVENT the damage sub event runs (no code spawns this unit or sets the slot)


BattleUnitKind unit_boss_honeduka;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static EvtWord init_event[21];
static EvtWord damage_event[10];
EvtWord honeduka_damage_sub_event[14];
static EvtWord wait_event[6];

/**
 * Unit data of the Bone Pile: 100 HP, 100x100 units in size; ties together its sounds, parts,
 * status vulnerability, init event and event table.
 */
BattleUnitKind unit_boss_honeduka = {
    UNIT_CORTEZ_BONE_PILE, // unitType
    "btl_un_honeduka", // name
    100, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    0, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    100, // width
    100, // height
    {0, 0}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {50.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {32.0f, 0.0f, 0.0f}, // bintaHitOffset
    {50.0f, 65.0f, 0.0f}, // kissHitOffset
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
    "SFX_BOSS_KOR_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin | kUnitNoHpGauge | kUnitNoncombatant | kUnitHpFixed, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the Bone Pile: 0 against every kind of hit. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence attributes of the Bone Pile: none, every element hits it normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the Bone Pile (percent chance each status takes hold): immune to sleep, dizzy,
 * confusion, Huge, Tiny, Attack Down, fright, Gale Force, Dodgy, Invisible, instant KO;
 * resists freeze (50%), Defense Down (40%), allergic (50%), Slow (50%); the rest take hold at 80%
 * or better.
 */
static StatusVulnerability regist = {
    0, // sleep
    80, // stop
    0, // dizzy
    100, // poison
    0, // confuse
    100, // electric
    100, // burn
    50, // freeze
    0, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    40, // defenseDown
    50, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    50, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE, "KRT_Z_1"},
    {POSE_TYPE_ACTION, "KRT_Z_1"},
};

/** Event table of the Bone Pile: honeduka_damage_sub_event. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, honeduka_damage_sub_event},
    {0, NULL},
};

/** Parts of the Bone Pile: 1 part (model c_honezuka). */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_honeduka", "c_honezuka", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kUntattleable | kNoShadow, 0, pose_table},
};

/**
 * Init event of the standalone Bone Pile unit: sets unit flag 0x1000000 (no mirror flip),
 * installs its wait and damage events and gives it no actions per turn.
 * Nothing in the tree spawns this unit (Cortez's bone pile is his own part 4);
 * its owner would be in unit work 0.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage event of the Bone Pile: runs the default damage reaction,
 * which calls its data tag 9 (honeduka_damage_sub_event) to pass the hit on to Cortez.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Damage sub-event of the Bone Pile (honeduka):
 * after it is hit it runs its owner's (unit work 0, Cortez) bone-pile damage event,
 * mode2_honeduka_damage_event (data tag 70 on the US demo and earlier, 71 later;
 * the tag number differs by version), then returns to waiting.
 */
EVT_BEGIN(honeduka_damage_sub_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_OWNER, LW(3))
#if VERSION <= VERSION_DEMO_US
    CALL(btlevtcmd_RunDataEventChild, LW(3), 70)
#else
    CALL(btlevtcmd_RunDataEventChild, LW(3), UNIT_DATA_WEAPON_BREAK_EVENT)
#endif
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait (idle) event of the Bone Pile: sets the idle pose (KRT_Z_1) from its pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()


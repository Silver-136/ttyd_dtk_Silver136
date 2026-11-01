// Battle unit data for the tree stage object (unit_object_tree).

#include "battle/battle.h"
#include "battle/battle_stage_object.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"

USER_FUNC(_rumble_tree);
USER_FUNC(_turn_tree);

static BattleUnitKindPart parts[1];
static EvtWord init_event[25];
static EvtWord damage_event[14];
#if VERSION <= VERSION_DEMO_US
static s32 _data[34];
#else
static s32 _data[35];
#endif
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[1];
static EvtWord attack_event[5];
static EvtWord wait_event[2];

/**
 * Unit data of the stage tree: an invisible prop (hidden part, no HP gauge, 999 HP) that stands in for a
 * tree on the battle stage which the player can hit. It never takes a turn.
 */
BattleUnitKind unit_object_tree = {
    UNIT_UNUSED_TREE, // unitType
    "btl_un_tree", // name
    999, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    2, // level
    0, // bonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    24, // width
    24, // height
    {0, 0}, // hitOffset
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
    kUnitMapObject | kNoGravityBob | kActsInZeroGravity | kUnitNoHpGauge | kUnitNoncombatant, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * The tree's single part: a hidden, shadowless Mario-model body (guard cover,
 * no HP gauge) used only as a hit target.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_tree", "b_mario", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHidden | kNoShadow, 0, pose_table},
};

/**
 * Init for a stage tree the player can hit (spawned by battle_stage_object.c as _entry_tree_data):
 * registers the wait, attack and damage scripts, never moves, and idles. Its part is hidden and has no shadow.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetMaxMoveCount, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Hit reaction: wobbles the tree object up and down (_rumble_tree),
 * then tips it over to 90 degrees and snaps it back upright (_turn_tree).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(_rumble_tree, UNIT_ID_SELF)
    CALL(_turn_tree, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Vertical offsets applied on successive frames by _rumble_tree: a decaying wobble that settles at zero.
 */
static s32 _data[] = {
    -5, 0, 15, 0, -15, 0, 14, 0,
    -13, 0, 12, 0, -11, 0, 10, 0,
    -9, 0, 8, 0, -7, 0, 6, 0,
    -6, 0, 5, 0, -4, 0, 2, 0,
    -2, 0, 0,
};

/** The tree takes no damage reduction; every hit just triggers its shake and tip-over. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** The tree has no elemental defence. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** The tree has no status vulnerabilities (all zero). */
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

/**
 * Poses of the tree: every pose uses the same still frame of the Mario model (M_S_1),
 * apart from one slot that uses M_O_1.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "M_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "M_S_1"},
    {POSE_TYPE_DAMAGE, "M_O_1"},
    {POSE_TYPE_ACTION, "M_S_1"},
};

/** The tree has no data table events. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** The tree never acts on its own turn; it just returns to its (empty) idle script. */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    RETURN
EVT_END()

USER_FUNC(_rumble_tree) {
    BattleUnit* unit;
    BattleStageObject* object;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    object = BattleGetObjectPtr(unit->stageObjectId);
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    object->position.y += _data[event->userdata[0]];
    event->userdata[0]++;
    if ((u32)event->userdata[0] >= 34) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Tilts the tree's stage object (the running unit) about X by 3 degrees a frame up to 90 over 30 frames,
 * then snaps it back upright. Run from damage_event after _rumble_tree.
 */
USER_FUNC(_turn_tree) {
    BattleUnit* unit;
    BattleStageObject* object;
    s32 id;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    object = BattleGetObjectPtr(unit->stageObjectId);
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    event->userdata[0] += 3;
    if (event->userdata[0] > 90) {
        object->rotation.x = 0.0f;
        return EVT_RETURN_DONE;
    }
    object->rotation.x = event->userdata[0];
    return EVT_RETURN_BLOCK;
}

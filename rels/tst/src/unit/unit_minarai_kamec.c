// minarai_kamec (apprentice Magikoopa): a placeholder (unit kind 200) with the Goomba model and poses: it waits, takes damage and at most uses an item.
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_sub.h"

BattleUnitKind unit_minarai_kamec;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[21];
static EvtWord damage_event[12];
static EvtWord attack_event[19];
static EvtWord wait_event[6];

/**
 * Unit definition of the apprentice Magikoopa placeholder (`minarai_kamec`): 2 HP, level 1, 1 part, with its
 * sounds, init script and data table.
 */
BattleUnitKind unit_minarai_kamec = {
    200,
    "btl_un_minarai_kamec",
    2,
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
    0,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

/**
 * Defence of the apprentice Magikoopa placeholder (`minarai_kamec`): none, so it takes full damage from every
 * kind of attack.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/**
 * Damage-type modifiers of the apprentice Magikoopa placeholder (`minarai_kamec`): none, no special weakness
 * or immunity.
 */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/**
 * Status vulnerability of the apprentice Magikoopa placeholder (`minarai_kamec`) (100% is the normal chance):
 * takes every status at the normal rate.
 */
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

/**
 * Pose table of the apprentice Magikoopa placeholder (`minarai_kamec`): its 4 battle poses (status, hurt and
 * action) and the animation each one plays; the idle animation is `KUR_S_1`.
 */
static PoseTableEntry pose_table[] = {
    {BTL_POSE_NORMAL, "KUR_S_1"},
    {31, "KUR_S_1"},
    {BTL_POSE_DAMAGE, "KUR_D_1"},
    {POSE_TYPE_ACTION, "KUR_S_1"},
};

/** Data table of the apprentice Magikoopa placeholder (`minarai_kamec`): no events. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/**
 * Body of the apprentice Magikoopa placeholder (`minarai_kamec`): one targetable part, the model `c_kuribo`,
 * with its hit box, defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_minarai_kamec", "c_kuribo", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/**
 * Battle init script of the placeholder unit: installs the wait,
 * attack and damage handlers (confusion uses the default one) and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage handler: selects this unit (-2) and its part 1
 * and runs the shared default damage reaction (btldefaultevt_Damage).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
EVT_END()

/**
 * Attack of the placeholder unit: uses an enemy item if the item-use check selects one (and then ends its turn),
 * otherwise does nothing and just restarts the wait event.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
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

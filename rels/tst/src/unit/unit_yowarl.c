// yowarl: a placeholder (unit kind 197) with the Goomba model and poses: it waits, takes damage and at most uses an item.
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_sub.h"

BattleUnitKind unit_yowarl;
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
 * Unit data of Yowarl, an unused test-area placeholder enemy: a Goomba model with 2 HP,
 * no attacks of its own and no special behaviour. It waits,
 * reacts to damage the default way and at most uses a held item.
 */
BattleUnitKind unit_yowarl = {
    197,
    "btl_un_yowarl",
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

/** Damage reduction of Yowarl: none. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Yowarl has no elemental defence. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Yowarl is fully vulnerable to every status effect (100 for each). */
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

/** Poses of Yowarl: the Goomba's standing frame (KUR_S_1) and hurt frame (KUR_D_1). */
static PoseTableEntry pose_table[] = {
    {28, "KUR_S_1"},
    {31, "KUR_S_1"},
    {39, "KUR_D_1"},
    {POSE_TYPE_ACTION, "KUR_S_1"},
};

/** Yowarl has no data table events. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Yowarl's single part: the Goomba model (c_kuribo). */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_yowarl", "c_kuribo", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/**
 * Init event of the placeholder unit: registers its wait,
 * attack and damage events (default confusion) and starts waiting.
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
 * Damage event of the placeholder unit: runs the default damage reaction (btldefaultevt_Damage);
 * the END_SCRIPT and second RETURN after it are never reached.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
EVT_END()

/** Attack event of the placeholder unit: it has no attack; it only uses an item if it decides to, then waits again. */
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

/** Wait event of the placeholder unit: plays the idle pose from its pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

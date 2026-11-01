// tuyonarl: a placeholder (unit kind 198) with the Goomba model and poses: it waits, takes damage and at most uses an item.
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_sub.h"

BattleUnitKind unit_tuyonarl;
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
 * Unit definition of the placeholder unit `tuyonarl`:
 * 2 HP and level 1, with its sizes, damage sounds, status vulnerability, parts and init script.
 */
BattleUnitKind unit_tuyonarl = {
    UNIT_UNUSED_TUYONARL,
    "btl_un_tuyonarl",
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

/** Defence of the placeholder unit `tuyonarl`: none, every hit does its full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of the placeholder unit `tuyonarl`: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Status vulnerability of the placeholder unit `tuyonarl`: every status lands at the normal rate. */
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

/**
 * Pose table of the placeholder unit `tuyonarl`:
 * maps each battle pose (waiting, hurt, running...) to one of its KUR_* animations.
 */
static PoseTableEntry pose_table[] = {
    {BTL_POSE_NORMAL, "KUR_S_1"},
    {31, "KUR_S_1"},
    {BTL_POSE_DAMAGE, "KUR_D_1"},
    {POSE_TYPE_ACTION, "KUR_S_1"},
};

/** Data table of the placeholder unit `tuyonarl`: it has no special reaction scripts. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Body part of the placeholder unit `tuyonarl`: a single part using the c_kuribo model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_tuyonarl", "c_kuribo", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/**
 * Init script of tuyonarl: registers its wait, attack and damage scripts and the default confusion script
 * and starts the wait script.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage reaction of tuyonarl: the default damage script (the words after its RETURN are never reached). */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
    OPCODE_END_SCRIPT,
    RETURN
EVT_END()

/** tuyonarl's turn: it uses an item if EnemyItemUseCheck picks one and otherwise does nothing; it has no attack. */
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

/** Wait script of tuyonarl: shows its idle pose (KUR_S_1) from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

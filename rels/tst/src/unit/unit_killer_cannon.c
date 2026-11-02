// Battle unit Bill Blaster (`killer_cannon`): tst.rel's copy of rels/dou/src/unit/unit_killer_cannon.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"

static USER_FUNC(killer_get_angle);
static USER_FUNC(killer_copy_status);
extern BattleUnitKind unit_killer;

BattleUnitKind unit_killer_cannon;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[3];
static BattleUnitSetup entry_killer;
static PoseTableEntry pose_table[16];
static BattleDataEntry data_table[1];
static EvtWord init_event[63];
static EvtWord damage_event[10];
static EvtWord attack_event[738];
static EvtWord friend_event[244];
static EvtWord friend_event_sky[318];
static EvtWord wait_event[6];

/**
 * Script helper: args[0] and args[1] are unit ids (BattleTransID).
 * Copies the size-change (Mini/Mega) turns and strength from the first unit to the second,
 * so a unit spawned or replaced mid-battle keeps the size it had. Called with the new unit (LW(3)).
 */
static USER_FUNC(killer_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    unit2->sizeChangeTurns = unit1->sizeChangeTurns;
    unit2->sizeChangeStrength = unit1->sizeChangeStrength;
    return EVT_RETURN_DONE;
}

/**
 * Unit definition of the Bill Blaster: 5 HP, level 22, 3 parts, with its sounds, init script and data table.
 */
BattleUnitKind unit_killer_cannon = {
    90,
    "btl_un_killer_cannon",
    5,
    0,
    1,
    1,
    22,
    0,
    3,
    70,
    3,
    60,
    9999,
    52,
    38,
    5,
    38,
    0.0f,
    19.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    26.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
#if VERSION <= VERSION_DEMO_US
    0.0f,
    0.0f,
#else
    20.0f,
    10.0f,
#endif
    26.0f,
    0.0f,
    0.0f,
    26.0f,
    24.7f,
    0.0f,
    -10.0f,
    19.0f,
    0.0f,
    52.0f,
    38.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_KILLER_CANON_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    3,
    parts,
    init_event,
    data_table,
};

/** Defence of the Bill Blaster: 2 against every kind of attack. */
static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

/** Damage-type modifiers of the Bill Blaster: none, no special weakness or immunity. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US

/**
 * Status vulnerability of the Bill Blaster (100% is the normal chance): immune to Sleep, Stop, Dizzy, Poison,
 * Confusion and Fright; resists Freeze 50%, Tiny 50%, Attack Down 50%, Defense Down 50%, Allergic 50%, Gale
 * Force 15%, Slow 50% and instant KO 95%; weak to Electrified 150%.
 */
static StatusVulnerability regist = {0, 0, 0, 0, 0, 150, 100, 50, 100, 50, 100, 50, 100, 50, 50, 0, 15, 100, 50, 100, 100, 95};
#else
static StatusVulnerability regist = {0, 0, 0, 0, 0, 150, 100, 100, 100, 90, 100, 90, 100, 95, 50, 0, 15, 100, 50, 100, 100, 95};
#endif

/**
 * Parts of the Bill Blaster: 3 parts sharing the model `c_killtai`, each with its hit box, defence tables and
 * pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_killer_cannon", "c_killtai", 0.0f, 0.0f, 0.0f, 0.0f, 30.0f, 0.0f, 0.0f, 35.0f, 0.0f, 35, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
    {2, "btl_un_killer_cannon", "c_killtai", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 255, &defence, &defence_attr, 0x10000, 0, pose_table},
    {3, "btl_un_killer_cannon", "c_killtai", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 255, &defence, &defence_attr, 0x10000, 0, pose_table},
};

/**
 * Spawn record for a new Bullet Bill fired by the unit's scripts
 * (unit_killer, enemy side, parked off-screen until the script places it).
 */
static BattleUnitSetup entry_killer = {
    &unit_killer, // unitKindParams
    1, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Pose table of the Bill Blaster: its 16 battle poses (status, hurt and action) and the animation each one
 * plays; the idle animation is `KTA_S_1`.
 */
static PoseTableEntry pose_table[] = {
    {1, "KTA_N_1"},
    {2, "KTA_Y_1"},
    {9, "KTA_Y_1"},
    {5, "KTA_K_1"},
    {4, "KTA_X_1"},
    {3, "KTA_X_1"},
    {28, "KTA_S_1"},
    {29, "KTA_Q_1"},
    {30, "KTA_Q_1"},
    {31, "KTA_A_1"},
    {39, "KTA_D_1"},
    {56, "KTA_X_1"},
    {57, "KTA_X_1"},
    {50, "KTA_A_1"},
    {POSE_TYPE_TALK, "KTA_T_1"},
    {POSE_TYPE_ACTION, "KTA_S_1"},
};

/** Data table of the Bill Blaster: no events. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/**
 * Battle init script for the Bill Blaster: installs the wait, attack, damage and confusion handlers,
 * starts the wait event, shifts the unit and its home position 20 units in x,
 * sets the poses of its parts 2 and 3 and sets attribute flags on them.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 20, 0, 0)
    CALL(btlevtcmd_AddHomePos, UNIT_ID_SELF, 20, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_Z_3"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KTA_Z_2"))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_NO_SHADOW)
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
EVT_END()

/**
 * Attack AI of the Bill Blaster: runs a selected enemy-item script if one is returned,
 * otherwise draws a weighted random choice (15, 0, 10, 20, 0, 10) of firing a Bullet Bill from the front cannon
 * (friend_event) or the "sky" cannon (part 2, friend_event_sky), in a normal or a charged form (LW(12) = 1).
 * It tries three firing positions (changing the fall-off angle and distance).
 * Some choices stop after the first free spot, while others scan all three; if none is free it fires nothing.
 */
static EVT_BEGIN(attack_event)

    // Uses a held item if it has one to use.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF

    // Draws lots among six ways to load the cannon: three ground shots and three sky shots (with the barrel aimed
    // up); each one needs a free spot in front of the cannon to land.
    SET_VALUE(LW(0), 15)
    ADD(LW(0), 0)
    ADD(LW(0), 10)
    ADD(LW(0), 20)
    ADD(LW(0), 0)
    ADD(LW(0), 10)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 15)

    // Ground shot: scans up to three positions in front of the cannon for a free spot and fires a plain Bullet Bill
    // there.
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(friend_event)
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF

    // Ground shot with a longer charge-up before firing; the Bill it fires is charged.
    ADD(LW(0), 0)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 1)
                    CALL_CHILD(friend_event)
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF

    // Ground shot again, with a plain fire.
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(friend_event)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF

    // Sky shot: the barrel part is raised and the cannon fires a Bullet Bill down onto a free spot, then the barrel
    // swings back down.
    ADD(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(friend_event_sky)
                    CALL(btlevtcmd_GetPartsRotate, UNIT_ID_SELF, 2, LW(5), LW(6), LW(11))
                    LOOP(0)
                        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                            BREAK_LOOP
                        END_IF
                        ADD(LW(11), 2)
                        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(11))
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HIDDEN)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF

    // Sky shot with a longer charge-up; the Bill it fires is charged.
    ADD(LW(0), 0)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 1)
                    CALL_CHILD(friend_event_sky)
                    CALL(btlevtcmd_GetPartsRotate, UNIT_ID_SELF, 2, LW(5), LW(6), LW(11))
                    LOOP(0)
                        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                            BREAK_LOOP
                        END_IF
                        ADD(LW(11), 2)
                        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(11))
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HIDDEN)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
                    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
                    GOTO(99)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        GOTO(99)
    END_IF

    // Sky shot again; this one lowers the barrel after the scan finishes.
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(friend_event_sky)
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(btlevtcmd_GetPartsRotate, UNIT_ID_SELF, 2, LW(5), LW(6), LW(11))
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                BREAK_LOOP
            END_IF
            ADD(LW(11), 2)
            CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(11))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
        GOTO(99)
    END_IF

    // Back to waiting.
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Fires a Bullet Bill from the Bill Blaster's main cannon: LW(12) = 1 plays a longer charge-up first.
 * After a pause the cannon flashes (KTA_A_1) and the shot sound plays;
 * the smoke size depends on whether the Blaster is Huge (STATUS_HUGE) or Tiny (STATUS_TINY).
 * The camera shakes and a new Bullet Bill (entry_killer) is spawned with the Blaster's status copied
 * (killer_copy_status), flies to the free position LW(0), LW(2) at y = 10 and becomes its home position.
 * A charged shot gives it status 16.
 */
static EVT_BEGIN(friend_event)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_A_2A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_Z_1"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_A_2A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_Z_1"))
        WAIT_FRAMES(10)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KTA_A_1"))
    WAIT_FRAMES(40)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 70)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 50, LW(9), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 28)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 15, LW(9), FLOAT(0.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 40)
    ADD(LW(9), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 30, LW(9), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    GOTO(10)
    LABEL(10)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 20, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_killer), 0)
    CALL(killer_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_SetPos, LW(3), LW(7), 10, LW(2))
    CALL(btlevtcmd_SetFallAccel, LW(3), 0)
    CALL(btlevtcmd_SetJumpSpeed, LW(3), 0)
    CALL(btlevtcmd_JumpPosition, LW(3), LW(0), 10, LW(2), FLOAT(7.0f), -1)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), 10, LW(2))
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_OnOffStatus, LW(3), 16, 1, 1, 1)
    END_IF
    RETURN
EVT_END()

/**
 * Same as friend_event for the "sky" cannon (part 2):
 * first lowers the cannon's angle (part 2 rotation) to aim at the free spot, then charges (if LW(12) = 1),
 * fires, shakes the camera and spawns a Bullet Bill that flies down to the position at y = 40,
 * rotated to match the barrel angle during the launch.
 */
static EVT_BEGIN(friend_event_sky)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(evt_sub_get_dir, LW(7), LW(8), LW(0), 40, LW(10))
    CALL(killer_get_angle, LW(7), LW(8), LW(0), 40, LW(10))
    CALL(btlevtcmd_GetPartsRotate, UNIT_ID_SELF, 2, LW(5), LW(6), LW(11))
    LOOP(0)
        IF_INT_GREATER(LW(11), 0)
            BREAK_LOOP
        END_IF
        IF_INT_LESS_OR_EQUAL(LW(11), LW(10))
            BREAK_LOOP
        END_IF
        SUBTRACT(LW(11), 2)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, LW(11))
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_A_3A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_Z_3"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_A_3A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_Z_3"))
        WAIT_FRAMES(10)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KTA_A_3"))
    WAIT_FRAMES(40)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CANNON2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 60)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 75, LW(9), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 28)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 20, LW(9), FLOAT(0.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(7), 40)
    ADD(LW(9), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 38, LW(9), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    GOTO(10)
    LABEL(10)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 20, 0)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_killer), 0)
    CALL(killer_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_SetPos, LW(3), LW(7), 10, LW(2))
    CALL(btlevtcmd_SetPartsRotate, LW(3), LW(4), 0, 0, LW(10))
    CALL(btlevtcmd_SetFallAccel, LW(3), 0)
    CALL(btlevtcmd_SetJumpSpeed, LW(3), 0)
    CALL(btlevtcmd_JumpPosition, LW(3), LW(0), 40, LW(2), FLOAT(7.0f), -1)
    CALL(btlevtcmd_SetPartsRotate, LW(3), LW(4), 0, 0, 0)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), 40, LW(2))
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_OnOffStatus, LW(3), 16, 1, 1, 1)
    END_IF
    CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET)
    CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), PART_ATTR_HAMMERLIKE_CANNOT_TARGET)
    RETURN
EVT_END()

double atan(double x);

/**
 * Script helper: args[0]-args[1] and args[2]-args[3] are two points;
 * args[4] receives the angle of the line between them in degrees (atan of the slope),
 * used to aim the Bullet Bill.
 */
static USER_FUNC(killer_get_angle) {
    EvtWord* args = event->args;
    f32 x1 = evtGetFloat(event, args[0]);
    f32 y1 = evtGetFloat(event, args[1]);
    f32 x2 = evtGetFloat(event, args[2]);
    f32 y2 = evtGetFloat(event, args[3]);
    f32 angle = atan((y2 - y1) / (x2 - x1));

    evtSetFloat(event, args[4], 360.0f * angle / 6.283184f);
    return EVT_RETURN_DONE;
}

/**
 * Wait (idle) event: refreshes the body animation (part 1) from the unit's pose table
 * so it matches the current state, then returns; the battle system runs it whenever the unit is idle.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

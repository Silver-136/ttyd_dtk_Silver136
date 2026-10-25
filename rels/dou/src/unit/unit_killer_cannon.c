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

USER_FUNC(btlevtcmd_AddHomePos);
USER_FUNC(btlevtcmd_AddPos);
USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_CheckSpace);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_FaceDirectionAdd);
USER_FUNC(btlevtcmd_GetFaceDirection);
USER_FUNC(btlevtcmd_GetPartsRotate);
USER_FUNC(btlevtcmd_GetPos);
USER_FUNC(btlevtcmd_OffPartsAttribute);
USER_FUNC(btlevtcmd_OnPartsAttribute);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetPartsRotate);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(evt_sub_random);
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

static USER_FUNC(killer_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    unit2->mSizeChangeTurns = unit1->mSizeChangeTurns;
    unit2->mSizeChangeStrength = unit1->mSizeChangeStrength;
    return EVT_RETURN_DONE;
}

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
    20.0f,
    10.0f,
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

static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 150, 100, 100, 100, 90, 100, 90, 100, 95, 50, 0, 15, 100, 50, 100, 100, 95};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_killer_cannon", "c_killtai", 0.0f, 0.0f, 0.0f, 0.0f, 30.0f, 0.0f, 0.0f, 35.0f, 0.0f, 35, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
    {2, "btl_un_killer_cannon", "c_killtai", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 255, &defence, &defence_attr, 0x10000, 0, pose_table},
    {3, "btl_un_killer_cannon", "c_killtai", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 255, &defence, &defence_attr, 0x10000, 0, pose_table},
};

static BattleUnitSetup entry_killer = {
    &unit_killer, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

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
    {65, "KTA_T_1"},
    {69, "KTA_S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    CALL(btlevtcmd_AddPos, -2, 20, 0, 0)
    CALL(btlevtcmd_AddHomePos, -2, 20, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("KTA_Z_3"))
    CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("KTA_Z_2"))
    CALL(btlevtcmd_OnPartsAttribute, -2, 2, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, -2, 2, 33554432)
    CALL(btlevtcmd_OnPartsAttribute, -2, 3, 33554432)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    SET_VALUE(LW(0), 15)
    ADD(LW(0), 0)
    ADD(LW(0), 10)
    ADD(LW(0), 20)
    ADD(LW(0), 0)
    ADD(LW(0), 10)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 15)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(PTR(friend_event))
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
    ADD(LW(0), 0)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 1)
                    CALL_CHILD(PTR(friend_event))
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
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        LOOP(0)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(PTR(friend_event))
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
    ADD(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, -2, 1, 16777216)
        CALL(btlevtcmd_OffPartsAttribute, -2, 2, 16777216)
        CALL(btlevtcmd_OffPartsAttribute, -2, 3, 16777216)
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(PTR(friend_event_sky))
                    CALL(btlevtcmd_GetPartsRotate, -2, 2, LW(5), LW(6), LW(11))
                    LOOP(0)
                        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                            BREAK_LOOP
                        END_IF
                        ADD(LW(11), 2)
                        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(11))
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(btlevtcmd_OffPartsAttribute, -2, 1, 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, -2, 2, 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
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
    ADD(LW(0), 0)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, -2, 1, 16777216)
        CALL(btlevtcmd_OffPartsAttribute, -2, 2, 16777216)
        CALL(btlevtcmd_OffPartsAttribute, -2, 3, 16777216)
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 1)
                    CALL_CHILD(PTR(friend_event_sky))
                    CALL(btlevtcmd_GetPartsRotate, -2, 2, LW(5), LW(6), LW(11))
                    LOOP(0)
                        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                            BREAK_LOOP
                        END_IF
                        ADD(LW(11), 2)
                        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(11))
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(btlevtcmd_OffPartsAttribute, -2, 1, 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, -2, 2, 16777216)
                    CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
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
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(15), 0)
        SET_VALUE(LW(14), 160)
        SET_VALUE(LW(13), 30)
        CALL(btlevtcmd_OnPartsAttribute, -2, 1, 16777216)
        CALL(btlevtcmd_OffPartsAttribute, -2, 2, 16777216)
        CALL(btlevtcmd_OffPartsAttribute, -2, 3, 16777216)
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, 0)
        LOOP(0)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), LW(14))
            SET_VALUE(LW(4), LW(0))
            CALL(btlevtcmd_GetFaceDirection, -2, LW(5))
            MULTIPLY(LW(4), LW(5))
            IF_INT_LESS(LW(4), LW(5))
                CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
                ADD(LW(2), LW(13))
                IF_INT_EQUAL(LW(3), -1)
                    SET_VALUE(LW(12), 0)
                    CALL_CHILD(PTR(friend_event_sky))
                END_IF
            END_IF
            ADD(LW(15), 1)
            SUBTRACT(LW(14), 40)
            SUBTRACT(LW(13), 10)
            IF_INT_GREATER_OR_EQUAL(LW(15), 3)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(btlevtcmd_GetPartsRotate, -2, 2, LW(5), LW(6), LW(11))
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(11), 0)
                BREAK_LOOP
            END_IF
            ADD(LW(11), 2)
            CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(11))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, 16777216)
        CALL(btlevtcmd_OnPartsAttribute, -2, 2, 16777216)
        CALL(btlevtcmd_OnPartsAttribute, -2, 3, 16777216)
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(friend_event)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_KILLER_CANNON_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KTA_A_2A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KTA_Z_1"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KTA_A_2A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KTA_Z_1"))
        WAIT_FRAMES(10)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KTA_A_1"))
    WAIT_FRAMES(40)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_KILLER_CANNON3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_KILLER_CANNON1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, 10, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, -2, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(7), 70)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 50, LW(9), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, 11, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, -2, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(7), 28)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 15, LW(9), FLOAT(0.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetPos, -2, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(7), 40)
    ADD(LW(9), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 30, LW(9), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    GOTO(10)
    LABEL(10)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 20, 0)
    CALL(btlevtcmd_GetPos, -2, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_killer), 0)
    CALL(killer_copy_status, -2, LW(3))
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

static EVT_BEGIN(friend_event_sky)
    CALL(btlevtcmd_GetPos, -2, LW(7), LW(8), LW(9))
    CALL(evt_sub_get_dir, LW(7), LW(8), LW(0), 40, LW(10))
    CALL(killer_get_angle, LW(7), LW(8), LW(0), 40, LW(10))
    CALL(btlevtcmd_GetPartsRotate, -2, 2, LW(5), LW(6), LW(11))
    LOOP(0)
        IF_INT_GREATER(LW(11), 0)
            BREAK_LOOP
        END_IF
        IF_INT_LESS_OR_EQUAL(LW(11), LW(10))
            BREAK_LOOP
        END_IF
        SUBTRACT(LW(11), 2)
        CALL(btlevtcmd_SetPartsRotate, -2, 2, 0, 0, LW(11))
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_KILLER_CANNON_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("KTA_A_3A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("KTA_Z_3"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("KTA_A_3A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("KTA_Z_3"))
        WAIT_FRAMES(10)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("KTA_A_3"))
    WAIT_FRAMES(40)
    IF_INT_EQUAL(LW(12), 1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_KILLER_CANNON4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_KILLER_CANNON2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, 10, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, -2, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(7), 60)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 75, LW(9), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, 11, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, -2, LW(7), LW(8), LW(9))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(7), 28)
        ADD(LW(9), 10)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 20, LW(9), FLOAT(0.5f), 0, 0, 0, 0, 0, 0, 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetPos, -2, LW(7), LW(8), LW(9))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(7), 40)
    ADD(LW(9), 10)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(7), 38, LW(9), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    GOTO(10)
    LABEL(10)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 20, 0)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_killer), 0)
    CALL(killer_copy_status, -2, LW(3))
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
    CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), 4194304)
    CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), 2097152)
    RETURN
EVT_END()

double atan(double x);

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

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()


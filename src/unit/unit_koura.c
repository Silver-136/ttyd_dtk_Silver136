#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "evt/evt_eff.h"

USER_FUNC(btlevtcmd_SetEventPhase);

BattleUnitKind unit_koura;
static BattleDataEntry data_table[7];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[5];
PoseTableEntry pose_table_crack_lv1[5];
PoseTableEntry pose_table_crack_lv2[5];
static BattleUnitKindPart parts[2];
static EvtWord init_event[46];
static EvtWord phase_event[38];
EvtWord koura_open[72];
EvtWord koura_close[79];
EvtWord damage_core[92];
EvtWord pose_tbl_reset[58];
static EvtWord dead_event[243];
EvtWord destroy_event[47];
static EvtWord damage_event[10];
static EvtWord wait_event[6];
static EvtWord attack_event[2];

BattleUnitKind unit_koura = {
    223,
    "btl_un_koura",
    8,
    0,
    5,
    1,
    1,
    0,
    0,
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
    25.0f,
    0.0f,
    0.0f,
    25.0f,
    15.6f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    -1,
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
    0x10000000,
    &regist,
    2,
    parts,
    init_event,
    data_table,
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_KOURA_OPEN_EVENT, koura_open},
    {UNIT_DATA_KOURA_CLOSE_EVENT, koura_close},
    {UNIT_DATA_DAMAGE_SUB_EVENT, damage_core},
    {61, pose_tbl_reset},
    {62, destroy_event},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {28, "PKO_S_1"},
    {31, "PKO_S_1"},
    {39, "PKO_D_1"},
    {68, "PKO_Z_1"},
    {69, "PKO_S_1"},
};

PoseTableEntry pose_table_crack_lv1[] = {
    {28, "PKO_S_2"},
    {31, "PKO_S_2"},
    {39, "PKO_D_2"},
    {68, "PKO_Z_2"},
    {69, "PKO_S_2"},
};

PoseTableEntry pose_table_crack_lv2[] = {
    {28, "PKO_S_3"},
    {31, "PKO_S_3"},
    {39, "PKO_D_3"},
    {68, "PKO_Z_3"},
    {69, "PKO_S_3"},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_koura", "c_koura", 0.0f, 0.0f, 0.0f, 20.0f, 40.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x80009, 0, pose_table},
    {2, "btl_un_koura", "c_koura", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x3010000, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetBaseScale, -2, FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.5f))
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetMaxMoveCount, -2, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, -2, LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL_CHILD(PTR(koura_open))
        END_IF
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, -2, LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL_CHILD(PTR(koura_close))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(koura_open)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_PhaseEventStartDeclare, -2)
    CALL(btlevtcmd_SetRotateOffset, -2, -23, 0, 0)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, 16777216)
    LOOP(30)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, 2)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
    CALL(btlevtcmd_OffAttribute, LW(3), 64)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kNotTargetable)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 68)
    RETURN
EVT_END()

EVT_BEGIN(koura_close)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_PhaseEventStartDeclare, -2)
    CALL(btlevtcmd_SetRotateOffset, -2, -23, 0, 0)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 60)
    LOOP(30)
        CALL(btlevtcmd_AddRotate, -2, 0, 0, -2)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
    CALL(btlevtcmd_OnAttribute, LW(3), 64)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kNotTargetable)
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, 16777216)
    CALL(btlevtcmd_StartWaitEvent, -2)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(damage_core)
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(1))
    CALL(btlevtcmd_GetHp, -2, LW(0))
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(3)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_crack_lv2))
        IF_INT_NOT_EQUAL(LW(1), 2)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        CALL(btlevtcmd_SetUnitWork, -2, 3, 2)
        CASE_LESS_OR_EQUAL(6)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_crack_lv1))
        IF_INT_NOT_EQUAL(LW(1), 1)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        CALL(btlevtcmd_SetUnitWork, -2, 3, 1)
        CASE_DEFAULT
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 39)
    WAIT_MS(500)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(pose_tbl_reset)
    CALL(btlevtcmd_GetHp, -2, LW(0))
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(3)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_crack_lv2))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetUnitWork, -2, 3, 2)
        CASE_LESS_OR_EQUAL(6)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_crack_lv1))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_NOKO_GUARD3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetUnitWork, -2, 3, 1)
        CASE_DEFAULT
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
        CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
    CALL(btlevtcmd_OffAttribute, LW(3), 96)
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, 16777216)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_NOKO_GUARD4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKO_H_1"))
    CALL(btlevtcmd_AnimeChangePose, -2, 2, STRING("PKO_H_2"))
    CALL(btlevtcmd_OffPartsAttribute, -2, 2, kHidden)
    CALL(btlevtcmd_SetPartsRotateOffset, -2, 1, 23, 0, 0)
    CALL(btlevtcmd_SetPartsRotateOffset, -2, 2, -23, 0, 0)
    BEGIN_SIBLING_EVENT
        LOOP(5)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 1, 0, 1, 0)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 2, 0, 1, 0)
            WAIT_FRAMES(2)
        END_LOOP
        LOOP(6)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 1, 0, 1, 0)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 2, 0, 1, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(2)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 1, 0, 1, 0)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 2, 0, 1, 0)
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(10)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 1, -1, 0, 0)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 2, 1, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(5)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 1, -1, 0, 0)
            CALL(btlevtcmd_AddPartsDispOffset, -2, 2, 1, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(10)
            CALL(btlevtcmd_AddPartsRotateOffset, -2, 1, 0, 1, 0)
            CALL(btlevtcmd_AddPartsRotateOffset, -2, 2, 0, 1, 0)
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(14)
            CALL(btlevtcmd_AddPartsRotateOffset, -2, 1, -1, 0, 0)
            CALL(btlevtcmd_AddPartsRotateOffset, -2, 2, 1, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    LOOP(20)
        CALL(btlevtcmd_AddPartsRotate, -2, 1, 0, 0, 2)
        CALL(btlevtcmd_AddPartsRotate, -2, 2, 0, 0, -2)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(8)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, kHidden)
        CALL(btlevtcmd_OffPartsAttribute, -2, 2, kHidden)
        WAIT_FRAMES(2)
        CALL(btlevtcmd_OnPartsAttribute, -2, 1, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, -2, 2, kHidden)
        WAIT_FRAMES(2)
    END_LOOP
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()

EVT_BEGIN(destroy_event)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
    CALL(btlevtcmd_OffAttribute, LW(3), 96)
    CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, 16777216)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kHidden)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_KillUnit, -2, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()


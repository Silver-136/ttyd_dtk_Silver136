#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "manager/evtmgr_cmd.h"

static USER_FUNC(_battle_set_lecture_unit_id);
USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_ChangeFaceDirection);
USER_FUNC(btlevtcmd_CheckPhase);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_MovePosition);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetMaxMoveCount);
USER_FUNC(btlevtcmd_SetPos);
USER_FUNC(btlevtcmd_SetStayPose);
USER_FUNC(btlevtcmd_SetTalkPose);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_StatusWindowOnOff);
USER_FUNC(btlevtcmd_StopWaitEvent);
USER_FUNC(btlevtcmd_get_turn);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_btl_camera_set_moveto);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_toge);
static USER_FUNC(lecture_init);

f32 fd_lecture_frankli_kuriboo_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_lecture_frankli_kuriboo_neg_one_382;
f32 fd_lecture_frankli_kuriboo___local_PI_383;
f32 fd_lecture_frankli_kuriboo___local_3PIO2_385;
f32 fd_lecture_frankli_kuriboo_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_lecture_frankli_kuriboo___local_PIO2_384;
f32 fd_lecture_frankli_kuriboo_one_381;
f32 fd_lecture_frankli_kuriboo_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_lecture_frankli_kuriboo_neg_one_365;
f32 fd_lecture_frankli_kuriboo___local_PI_366;
f32 fd_lecture_frankli_kuriboo___local_3PIO2_368;
f32 fd_lecture_frankli_kuriboo_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_lecture_frankli_kuriboo___local_PIO2_367;
f32 fd_lecture_frankli_kuriboo_one_364;
BattleUnitKind unit_lecture_frankli_kuriboo;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[5];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[53];
static EvtWord entry_event[29];
static EvtWord phase_event[160];
static EvtWord damage_event[10];
static EvtWord wait_event[6];

static USER_FUNC(lecture_init) {
    return EVT_RETURN_DONE;
}

static USER_FUNC(_battle_set_lecture_unit_id) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 value;

    value = evtGetValue(event, *args++);
    wp->lectureUnitId = value;
    return EVT_RETURN_DONE;
}

f32 fd_lecture_frankli_kuriboo_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_lecture_frankli_kuriboo_neg_one_382 = -1.0f;

f32 fd_lecture_frankli_kuriboo___local_PI_383 = 3.1415927f;

f32 fd_lecture_frankli_kuriboo___local_3PIO2_385 = 4.712389f;

f32 fd_lecture_frankli_kuriboo_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_lecture_frankli_kuriboo___local_PIO2_384 = 1.5707964f;

f32 fd_lecture_frankli_kuriboo_one_381 = 1.0f;

f32 fd_lecture_frankli_kuriboo_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_lecture_frankli_kuriboo_neg_one_365 = -1.0f;

f32 fd_lecture_frankli_kuriboo___local_PI_366 = 3.1415927f;

f32 fd_lecture_frankli_kuriboo___local_3PIO2_368 = 4.712389f;

f32 fd_lecture_frankli_kuriboo_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_lecture_frankli_kuriboo___local_PIO2_367 = 1.5707964f;

f32 fd_lecture_frankli_kuriboo_one_364 = 1.0f;

BattleUnitKind unit_lecture_frankli_kuriboo = {
    180,
    "btl_un_frankli",
    10,
    0,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    100,
    9,
    28,
    24,
    5,
    25,
    0.0f,
    14.0f,
    0.0f,
    0,
    0,
    0.0f,
    30.0f,
    0.0f,
    14.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    0.0f,
    14.0f,
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
    "SFX_ENM_KURI_DAMAGED11",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x12000000,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {27, "D_1"},
    {28, "S_1"},
    {31, "D_2"},
    {39, "D_2"},
    {69, "S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_frankli", "c_kuri_h", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x890009, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(lecture_init)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(entry_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_GetUnitId, -2, LW(0))
    CALL(_battle_set_lecture_unit_id, LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetPos, -2, 0, -1000, 0)
    CALL(btlevtcmd_ChangeFaceDirection, -2, 1)
    CALL(btlevtcmd_SetMaxMoveCount, -2, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    WAIT_MS(900)
    CALL(btlevtcmd_SetPos, -2, -250, 0, -30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("R_1"))
    CALL(btlevtcmd_MovePosition, -2, -60, 0, -30, 45, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_StopWaitEvent, -3)
            CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_S_1"))
            CALL(btlevtcmd_SetTalkPose, -2, STRING("T_1"))
            CALL(btlevtcmd_SetStayPose, -2, STRING("S_1"))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(evt_msg_print, 2, STRING("mac_0_092_01"), 0, -2)
            CALL(evt_btl_camera_set_mode, 0, 3)
            CALL(evt_btl_camera_set_moveto, 0, 71, 49, 213, 71, 35, 0, 30, 11)
            WAIT_FRAMES(30)
            CALL(evt_msg_toge, 1, 0, 0, 0)
            CALL(evt_msg_print, 2, STRING("mac_0_092_02"), 0, -2)
            CALL(evt_btl_camera_set_moveto, 0, 111, 85, 213, 111, 71, 0, 30, 11)
            WAIT_FRAMES(30)
            CALL(evt_msg_toge, 1, 0, 0, 0)
            CALL(evt_msg_print, 2, STRING("mac_0_092_03"), 0, -2)
            CALL(btlevtcmd_ChangeFaceDirection, -2, -1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            WAIT_FRAMES(60)
            CALL(evt_msg_print, 2, STRING("mac_0_092_04"), 0, -2)
            CALL(btlevtcmd_ChangeFaceDirection, -2, 1)
            WAIT_MS(1500)
            CALL(btlevtcmd_ChangeFaceDirection, -2, -1)
            CALL(evt_btl_camera_set_mode, 0, 3)
            CALL(evt_btl_camera_set_moveto, 0, -60, 82, 340, -60, 60, 13, 30, 11)
            WAIT_FRAMES(30)
            CALL(evt_msg_print, 2, STRING("mac_0_092_05"), 0, -2)
            CALL(btlevtcmd_ChangeFaceDirection, -2, 1)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            GOTO(99)
        END_IF
    END_IF
    LABEL(99)
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


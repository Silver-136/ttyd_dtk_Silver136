#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"


USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_overwrite);
USER_FUNC(evt_cam_ctrl_onoff);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mobj_signboard);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);

f32 fd_tik_08_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_08_neg_one_382;
f32 fd_tik_08___local_PI_383;
f32 fd_tik_08___local_3PIO2_385;
f32 fd_tik_08_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik_08___local_PIO2_384;
f32 fd_tik_08_one_381;
f32 fd_tik_08_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_08_neg_one_365;
f32 fd_tik_08___local_PI_366;
f32 fd_tik_08___local_3PIO2_368;
f32 fd_tik_08_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik_08___local_PIO2_367;
f32 fd_tik_08_one_364;
EvtWord oshimodo_event_sub[33];
static EvtWord oshimodo_event[10];
static BeroEntry bero_oshimodo[1];
static BeroEntry bero_entry_data[3];
static BeroEntry bero_entry_data2[2];
static EvtWord evt_kanban[16];
EvtWord tik_08_init_evt[68];

f32 fd_tik_08_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_08_neg_one_382 = -1.0f;

f32 fd_tik_08___local_PI_383 = 3.1415927f;

f32 fd_tik_08___local_3PIO2_385 = 4.712389f;

f32 fd_tik_08_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_08___local_PIO2_384 = 1.5707964f;

f32 fd_tik_08_one_381 = 1.0f;

f32 fd_tik_08_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_08_neg_one_365 = -1.0f;

f32 fd_tik_08___local_PI_366 = 3.1415927f;

f32 fd_tik_08___local_3PIO2_368 = 4.712389f;

f32 fd_tik_08_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_08___local_PIO2_367 = 1.5707964f;

f32 fd_tik_08_one_364 = 1.0f;

EVT_BEGIN(oshimodo_event_sub)
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(750)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("tik_08_01"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_ctrl_onoff, 4, 1)
    RETURN
EVT_END()

static EVT_BEGIN(oshimodo_event)
    SPAWN_EVENT(PTR(oshimodo_event_sub))
    SET_VALUE(GSWF(1341), 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_oshimodo[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        (s32*)oshimodo_event, // exitEvent
        0, // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

static BeroEntry bero_entry_data[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "usu_00", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        1, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_07", // nextMapName
        "n_bero", // nextBeroName
        2, // enterAnimeType
        2, // exitAnimeType
        "door", // enterAnimeArg
        0, // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_entry_data2[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "sys_01", // nextMapName
        "stg_4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_kanban)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_msg_print, 0, STRING("tik_08_02"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tik_08_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS_OR_EQUAL(GSW(0), 175)
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan"), PTR(bero_oshimodo))
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW(0), 176)
        IF_INT_LESS(GSW(0), 178)
            SET_VALUE(LW(0), STRING("dokan"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_bero_overwrite, STRING("dokan"), PTR(bero_entry_data2))
            SET_VALUE(LW(0), STRING("dokan"))
            CALL_CHILD(PTR(bero_case_switch_on))
        END_IF
    END_IF
    CALL(evt_mobj_signboard, STRING("board"), 85, 0, 0, PTR(evt_kanban), 0)
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "stdio.h"
#include "evt/evt_bero.h"
#include "manager/evtmgr_cmd.h"

static char name[8];

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_eff);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mobj_recovery_blk);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
static USER_FUNC(make_name);

f32 fd_las_27_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_las_27_neg_one_382;
f32 fd_las_27___local_PI_383;
f32 fd_las_27___local_3PIO2_385;
f32 fd_las_27_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_las_27___local_PIO2_384;
f32 fd_las_27_one_381;
f32 fd_las_27_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_las_27_neg_one_365;
f32 fd_las_27___local_PI_366;
f32 fd_las_27___local_3PIO2_368;
f32 fd_las_27_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_las_27___local_PIO2_367;
f32 fd_las_27_one_364;
static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
static EvtWord shan_init[46];
EvtWord las_27_init_evt[38];

f32 fd_las_27_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_las_27_neg_one_382 = -1.0f;

f32 fd_las_27___local_PI_383 = 3.1415927f;

f32 fd_las_27___local_3PIO2_385 = 4.712389f;

f32 fd_las_27_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_las_27___local_PIO2_384 = 1.5707964f;

f32 fd_las_27_one_381 = 1.0f;

f32 fd_las_27_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_las_27_neg_one_365 = -1.0f;

f32 fd_las_27___local_PI_366 = 3.1415927f;

f32 fd_las_27___local_3PIO2_368 = 4.712389f;

f32 fd_las_27_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_las_27___local_PIO2_367 = 1.5707964f;

f32 fd_las_27_one_364 = 1.0f;

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_25", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_28", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(shan_init)
    SET_VALUE(LW(0), 1)
    LOOP(28)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
        CALL(evt_mapobj_get_position, LW(1), LW(2), LW(3), LW(4))
        SUBTRACT_FLOAT(LW(3), FLOAT(9.0f))
        CALL(evt_eff, 0, STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(0.69921875f), 1000, 0, 0, 0, 0, 0, 0)
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(las_27_init_evt)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS9"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 520, 60, -150, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("r_blk"), 10, 450, 60, -150, 0, 0)
    CALL_CHILD(PTR(shan_init))
    RETURN
EVT_END()

static USER_FUNC(make_name) {
    EvtWord* args = event->args;

    sprintf(name, "hi%d\0\0\0", evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}


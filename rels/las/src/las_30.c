#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "stdio.h"
#include "evt/evt_bero.h"
#include "evt/evt_mobj.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "manager/evtmgr_cmd.h"

static char name[8];

USER_FUNC(evt_eff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_wait_animation_end);
static USER_FUNC(make_name);

f32 fd_las_30_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_las_30_neg_one_382;
f32 fd_las_30___local_PI_383;
f32 fd_las_30___local_3PIO2_385;
f32 fd_las_30_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_las_30___local_PIO2_384;
f32 fd_las_30_one_381;
f32 fd_las_30_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_las_30_neg_one_365;
f32 fd_las_30___local_PI_366;
f32 fd_las_30___local_3PIO2_368;
f32 fd_las_30_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_las_30___local_PIO2_367;
f32 fd_las_30_one_364;
static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
static EvtWord rousoku_init[46];
static EvtWord mobj_box01[34];
static EvtWord mobj_box02[34];
EvtWord las_30_init_evt[71];

f32 fd_las_30_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_las_30_neg_one_382 = -1.0f;

f32 fd_las_30___local_PI_383 = 3.1415927f;

f32 fd_las_30___local_3PIO2_385 = 4.712389f;

f32 fd_las_30_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_las_30___local_PIO2_384 = 1.5707964f;

f32 fd_las_30_one_381 = 1.0f;

f32 fd_las_30_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_las_30_neg_one_365 = -1.0f;

f32 fd_las_30___local_PI_366 = 3.1415927f;

f32 fd_las_30___local_3PIO2_368 = 4.712389f;

f32 fd_las_30_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_las_30___local_PIO2_367 = 1.5707964f;

f32 fd_las_30_one_364 = 1.0f;

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_28", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
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
        "las_29", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(rousoku_init)
    SET_VALUE(LW(0), 1)
    LOOP(12)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
        CALL(evt_mapobj_get_position, LW(1), LW(2), LW(3), LW(4))
        SUBTRACT_FLOAT(LW(3), FLOAT(7.0f))
        CALL(evt_eff, 0, STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(0.5f), 1000, 0, 0, 0, 0, 0, 0)
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box01)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box01"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item01"), 150, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item01"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box01"))
    CALL(evt_item_get_item, STRING("item01"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box02)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box02"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item02"), 156, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item02"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box02"))
    CALL(evt_item_get_item, STRING("item02"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(las_30_init_evt)
    IF_INT_EQUAL(GSW(0), 400)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER2"))
        CALL(evt_snd_envoff, 512)
    ELSE
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS1"))
    END_IF
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 1150, 60, -125, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("r_blk"), 10, -1050, 860, -75, 0, 0)
    CALL(evt_mobj_itembox, STRING("mobj_box01"), -1150, 800, -75, 0, 0, PTR(mobj_box01), GSWF(4393))
    CALL(evt_mobj_itembox, STRING("mobj_box02"), 1050, 0, -125, 0, 0, PTR(mobj_box02), GSWF(4394))
    CALL_CHILD(rousoku_init)
    RETURN
EVT_END_PAD()

static USER_FUNC(make_name) {
    EvtWord* args = event->args;

    sprintf(name, "hi%d", evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}


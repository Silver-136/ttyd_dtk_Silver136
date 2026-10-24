#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"

#pragma force_active on

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mobj_recovery_blk);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_npc_change_fbat_mode);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_envon);

f32 fd_gon_10_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_gon_10_neg_one_382;
f32 fd_gon_10___local_PI_383;
f32 fd_gon_10___local_3PIO2_385;
f32 fd_gon_10_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_gon_10___local_PIO2_384;
f32 fd_gon_10_one_381;
f32 fd_gon_10_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_gon_10_neg_one_365;
f32 fd_gon_10___local_PI_366;
f32 fd_gon_10___local_3PIO2_368;
f32 fd_gon_10_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_gon_10___local_PIO2_367;
f32 fd_gon_10_one_364;
static EvtWord door02_close[22];
static EvtWord door02_open[22];
static EvtWord door01_close[22];
static EvtWord door01_open[22];
static BeroEntry bero_entry_data[3];
EvtWord gon_10_init_evt[46];

f32 fd_gon_10_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gon_10_neg_one_382 = -1.0f;

f32 fd_gon_10___local_PI_383 = 3.1415927f;

f32 fd_gon_10___local_3PIO2_385 = 4.712389f;

f32 fd_gon_10_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gon_10___local_PIO2_384 = 1.5707964f;

f32 fd_gon_10_one_381 = 1.0f;

f32 fd_gon_10_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gon_10_neg_one_365 = -1.0f;

f32 fd_gon_10___local_PI_366 = 3.1415927f;

f32 fd_gon_10___local_3PIO2_368 = 4.712389f;

f32 fd_gon_10_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gon_10___local_PIO2_367 = 1.5707964f;

f32 fd_gon_10_one_364 = 1.0f;

static EVT_BEGIN(door02_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door02_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        13, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_11", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door02_close, // enterAnimeArg
        door02_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_05", // nextMapName
        "w_bero_5", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(gon_10_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_snd_envon, 272, STRING("ENV_STG1_GON3"))
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -460, 1010, -145, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 6, -400, 1010, -146, 0, 0)
    IF_INT_GREATER_OR_EQUAL(GSW(0), 55)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("en"), 1)
    END_IF
    RETURN
EVT_END()


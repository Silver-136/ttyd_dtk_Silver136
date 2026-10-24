#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"

#pragma force_active on

USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxon);

f32 fd_muj_11_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_muj_11_neg_one_382;
f32 fd_muj_11___local_PI_383;
f32 fd_muj_11___local_3PIO2_385;
f32 fd_muj_11_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_muj_11___local_PIO2_384;
f32 fd_muj_11_one_381;
f32 fd_muj_11_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_muj_11_neg_one_365;
f32 fd_muj_11___local_PI_366;
f32 fd_muj_11___local_3PIO2_368;
f32 fd_muj_11_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_muj_11___local_PIO2_367;
f32 fd_muj_11_one_364;
static EvtWord w_close[12];
static EvtWord w_open[12];
static EvtWord e_close[12];
static EvtWord e_open[12];
static BeroEntry bero_entry_data[3];
EvtWord warning_event[38];
EvtWord muj_11_init_evt[35];

f32 fd_muj_11_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_muj_11_neg_one_382 = -1.0f;

f32 fd_muj_11___local_PI_383 = 3.1415927f;

f32 fd_muj_11___local_3PIO2_385 = 4.712389f;

f32 fd_muj_11_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_muj_11___local_PIO2_384 = 1.5707964f;

f32 fd_muj_11_one_381 = 1.0f;

f32 fd_muj_11_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_muj_11_neg_one_365 = -1.0f;

f32 fd_muj_11___local_PI_366 = 3.1415927f;

f32 fd_muj_11___local_3PIO2_368 = 4.712389f;

f32 fd_muj_11_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_muj_11___local_PIO2_367 = 1.5707964f;

f32 fd_muj_11_one_364 = 1.0f;

static EVT_BEGIN(w_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_10", // nextMapName
        "A_b_bero03", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_12", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_close, // enterAnimeArg
        e_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(warning_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
        WAIT_MS(800)
        CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg5_dou_20"), 0, 0)
    SET_VALUE(GSW(0), 252)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(muj_11_init_evt)
    IF_INT_LESS(GSW(0), 253)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG5_GHOST1"))
        CALL(evt_snd_envoff, 512)
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG5_MUJ9"))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_EQUAL(GSW(0), 251)
        SPAWN_EVENT(PTR(warning_event))
    END_IF
    RETURN
EVT_END()


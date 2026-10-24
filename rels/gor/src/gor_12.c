#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_sub.h"

#pragma force_active on

USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam_ctrl_onoff);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_party_dispflg_onoff);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_snd_envon);

f32 fd_gor_12_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_gor_12_neg_one_382;
f32 fd_gor_12___local_PI_383;
f32 fd_gor_12___local_3PIO2_385;
f32 fd_gor_12_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_gor_12___local_PIO2_384;
f32 fd_gor_12_one_381;
f32 fd_gor_12_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_gor_12_neg_one_365;
f32 fd_gor_12___local_PI_366;
f32 fd_gor_12___local_3PIO2_368;
f32 fd_gor_12_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_gor_12___local_PIO2_367;
f32 fd_gor_12_one_364;
static EvtWord epilogue_evt[49];
EvtWord gor_12_init_evt[35];

f32 fd_gor_12_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gor_12_neg_one_382 = -1.0f;

f32 fd_gor_12___local_PI_383 = 3.1415927f;

f32 fd_gor_12___local_3PIO2_385 = 4.712389f;

f32 fd_gor_12_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gor_12___local_PIO2_384 = 1.5707964f;

f32 fd_gor_12_one_381 = 1.0f;

f32 fd_gor_12_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gor_12_neg_one_365 = -1.0f;

f32 fd_gor_12___local_PI_366 = 3.1415927f;

f32 fd_gor_12___local_3PIO2_368 = 4.712389f;

f32 fd_gor_12_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gor_12___local_PIO2_367 = 1.5707964f;

f32 fd_gor_12_one_364 = 1.0f;

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_envon, 272, STRING("ENV_END_MUJ1"))
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_party_dispflg_onoff, 0, 1, 1)
    CALL(evt_seq_wait, 2)
    WAIT_MS(7000)
    CALL(evt_snd_bgmoff_f, 512, 8000)
    CALL(evt_snd_envoff_f, 512, 8000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 4000, 9, 300)
    CALL(evt_bero_mapchange, STRING("aaa_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(gor_12_init_evt)
    CALL(evt_map_playanim, STRING("anm_umi"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_nami"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_kirakira"), 1, 0)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    IF_INT_EQUAL(GSW(0), 402)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(epilogue_evt)
    END_IF
    RETURN
EVT_END_PAD()


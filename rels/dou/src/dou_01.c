#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"


USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mobj_jumpstand_blue);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);

f32 fd_dou_01_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_dou_01_neg_one_382;
f32 fd_dou_01___local_PI_383;
f32 fd_dou_01___local_3PIO2_385;
f32 fd_dou_01_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_dou_01___local_PIO2_384;
f32 fd_dou_01_one_381;
f32 fd_dou_01_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_dou_01_neg_one_365;
f32 fd_dou_01___local_PI_366;
f32 fd_dou_01___local_3PIO2_368;
f32 fd_dou_01_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_dou_01___local_PIO2_367;
f32 fd_dou_01_one_364;
static BeroEntry bero_entry_data[3];
static EvtWord korutesu_event[74];
EvtWord dou_01_init_evt[48];

f32 fd_dou_01_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_01_neg_one_382 = -1.0f;

f32 fd_dou_01___local_PI_383 = 3.1415927f;

f32 fd_dou_01___local_3PIO2_385 = 4.712389f;

f32 fd_dou_01_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou_01___local_PIO2_384 = 1.5707964f;

f32 fd_dou_01_one_381 = 1.0f;

f32 fd_dou_01_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_01_neg_one_365 = -1.0f;

f32 fd_dou_01___local_PI_366 = 3.1415927f;

f32 fd_dou_01___local_3PIO2_368 = 4.712389f;

f32 fd_dou_01_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou_01___local_PIO2_367 = 1.5707964f;

f32 fd_dou_01_one_364 = 1.0f;

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
        "dou_00", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_02", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(korutesu_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_normalize)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_01"), 0, 0)
    SET_VALUE(GSW(0), 244)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(dou_01_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_DOU2"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump00"), 400, 92, 375, -21, 0, 0)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump01"), 350, -45, 50, -15, 0, 0)
    IF_INT_EQUAL(GSW(0), 243)
        SPAWN_EVENT(PTR(korutesu_event))
    END_IF
    RETURN
EVT_END()


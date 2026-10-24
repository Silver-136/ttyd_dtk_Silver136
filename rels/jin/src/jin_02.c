#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "jin.h"

#pragma force_active on

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mobj_switch_red);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_get_value);
USER_FUNC(evt_sub_intpl_init);
USER_FUNC(evt_sub_random);

f32 fd_jin_02_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_jin_02_neg_one_382;
f32 fd_jin_02___local_PI_383;
f32 fd_jin_02___local_3PIO2_385;
f32 fd_jin_02_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_jin_02___local_PIO2_384;
f32 fd_jin_02_one_381;
f32 fd_jin_02_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_jin_02_neg_one_365;
f32 fd_jin_02___local_PI_366;
f32 fd_jin_02___local_3PIO2_368;
f32 fd_jin_02_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_jin_02___local_PIO2_367;
f32 fd_jin_02_one_364;
static EvtWord n2_close[12];
static EvtWord n2_open[12];
EvtWord n1_close[12];
EvtWord n1_open[12];
static BeroEntry bero_entry_data[3];
static EvtWord cam_shake[62];
EvtWord evt_kaidan_temae[144];
EvtWord jin_02_init_evt[33];

f32 fd_jin_02_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_jin_02_neg_one_382 = -1.0f;

f32 fd_jin_02___local_PI_383 = 3.1415927f;

f32 fd_jin_02___local_3PIO2_385 = 4.712389f;

f32 fd_jin_02_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_jin_02___local_PIO2_384 = 1.5707964f;

f32 fd_jin_02_one_381 = 1.0f;

f32 fd_jin_02_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_jin_02_neg_one_365 = -1.0f;

f32 fd_jin_02___local_PI_366 = 3.1415927f;

f32 fd_jin_02___local_3PIO2_368 = 4.712389f;

f32 fd_jin_02_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_jin_02___local_PIO2_367 = 1.5707964f;

f32 fd_jin_02_one_364 = 1.0f;

static EVT_BEGIN(n2_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n2_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n1_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n1_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "n_bero_2", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_03", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n2_close, // enterAnimeArg
        n2_open, // exitAnimeArg
    },
    {
        "n_bero_1", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_00", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n1_close, // enterAnimeArg
        n1_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(cam_shake)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_KAIDAN_MOVE1"), 300, 100, -200, LW(9))
    CALL(evt_sub_intpl_init, 11, 0, 10, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_sub_intpl_init, 11, 10, 0, 30)
    LABEL(1)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    RETURN
EVT_END()

EVT_BEGIN(evt_kaidan_temae)
    IF_INT_NOT_EQUAL(LSW(1), 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_normalize)
        CALL(evt_party_stop, 0)
        IF_INT_EQUAL(GSWF(2234), 0)
            BEGIN_INLINE_EVENT_GET_ID(LW(3))
                WAIT_MS(500)
                CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(1000)
                LOOP(0)
                    CALL(evt_sub_random, 200, LW(0))
                    ADD(LW(0), 300)
                    CALL(evt_mario_set_dir, 90, LW(0), 0)
                    WAIT_MS(LW(0))
                    CALL(evt_sub_random, 300, LW(0))
                    ADD(LW(0), 400)
                    CALL(evt_mario_set_dir, 270, LW(0), 0)
                    WAIT_MS(LW(0))
                END_LOOP
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT_GET_ID(LW(4))
                WAIT_MS(500)
                CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(1000)
                LOOP(0)
                    CALL(evt_sub_random, 200, LW(0))
                    ADD(LW(0), 400)
                    CALL(evt_party_set_dir, 0, 90, LW(0))
                    WAIT_MS(LW(0))
                    CALL(evt_sub_random, 200, LW(0))
                    ADD(LW(0), 200)
                    CALL(evt_party_set_dir, 0, 270, LW(0))
                    WAIT_MS(LW(0))
                END_LOOP
            END_INLINE_EVENT
        END_IF
        CALL_CHILD(PTR(cam_shake))
        IF_INT_EQUAL(GSWF(2234), 0)
            WAIT_MS(500)
            DELETE_EVENT(LW(3))
            DELETE_EVENT(LW(4))
            SET_VALUE(GSWF(2234), 1)
        END_IF
        ADD(LSW(1), 1)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_party_run, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(jin_02_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG4_JIN2"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_mobj_switch_red, 0, STRING("mobj_switch1"), 485, 30, -10, PTR(evt_kaidan_temae), 0)
    RETURN
EVT_END()


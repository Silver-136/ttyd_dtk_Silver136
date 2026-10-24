#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);

f32 fd_rsh_08_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_rsh_08_neg_one_382;
f32 fd_rsh_08___local_PI_383;
f32 fd_rsh_08___local_3PIO2_385;
f32 fd_rsh_08_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_rsh_08___local_PIO2_384;
f32 fd_rsh_08_one_381;
f32 fd_rsh_08_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_rsh_08_neg_one_365;
f32 fd_rsh_08___local_PI_366;
f32 fd_rsh_08___local_3PIO2_368;
f32 fd_rsh_08_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_rsh_08___local_PIO2_367;
f32 fd_rsh_08_one_364;
BeroEntry bero_data_a_nidome[2];
BeroEntry bero_data_b_nidome[2];
BeroEntry bero_data_c_nidome[2];
EvtWord rsh_08_a_init_evt[72];

f32 fd_rsh_08_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_rsh_08_neg_one_382 = -1.0f;

f32 fd_rsh_08___local_PI_383 = 3.1415927f;

f32 fd_rsh_08___local_3PIO2_385 = 4.712389f;

f32 fd_rsh_08_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_rsh_08___local_PIO2_384 = 1.5707964f;

f32 fd_rsh_08_one_381 = 1.0f;

f32 fd_rsh_08_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_rsh_08_neg_one_365 = -1.0f;

f32 fd_rsh_08___local_PI_366 = 3.1415927f;

f32 fd_rsh_08___local_3PIO2_368 = 4.712389f;

f32 fd_rsh_08_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_rsh_08___local_PIO2_367 = 1.5707964f;

f32 fd_rsh_08_one_364 = 1.0f;

BeroEntry bero_data_a_nidome[] = {
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_01_a", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

BeroEntry bero_data_b_nidome[] = {
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_01_b", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

BeroEntry bero_data_c_nidome[] = {
    {
        "e_bero", // hitName
        1, // type
        8, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "rsh_01_c", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        "e_door_close", // enterAnimeArg
        "e_door_open", // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(rsh_08_a_init_evt)
    IF_INT_EQUAL(GSWF(3460), 1)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_bgmoff, 26624)
        ELSE
        IF_INT_EQUAL(GSWF(3461), 1)
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
            CALL(evt_snd_envoff, 512)
            CALL(evt_snd_bgmoff, 26624)
            ELSE
            CALL(evt_snd_bgmoff, 512)
            CALL(evt_snd_envon, 272, STRING("ENV_STG6_RSH1_3"))
        END_IF
    END_IF
    SET_VALUE(LW(0), PTR(bero_data_a_nidome))
    IF_INT_EQUAL(GSWF(3453), 1)
        SET_VALUE(LW(0), PTR(bero_data_a_nidome))
    END_IF
    IF_INT_EQUAL(GSWF(3454), 1)
        SET_VALUE(LW(0), PTR(bero_data_b_nidome))
    END_IF
    IF_INT_EQUAL(GSWF(3455), 1)
        SET_VALUE(LW(0), PTR(bero_data_c_nidome))
    END_IF
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_map_playanim, STRING("yure"), 1, 0)
    RETURN
EVT_END()


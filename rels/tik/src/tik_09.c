#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_sub_intpl_get_value);
USER_FUNC(evt_sub_intpl_init);

f32 fd_tik_09_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_09_neg_one_382;
f32 fd_tik_09___local_PI_383;
f32 fd_tik_09___local_3PIO2_385;
f32 fd_tik_09_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik_09___local_PIO2_384;
f32 fd_tik_09_one_381;
f32 fd_tik_09_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_09_neg_one_365;
f32 fd_tik_09___local_PI_366;
f32 fd_tik_09___local_3PIO2_368;
f32 fd_tik_09_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik_09___local_PIO2_367;
f32 fd_tik_09_one_364;
static BeroEntry bero_entry_data[3];
EvtWord evt_tik09_dokan[51];
EvtWord tik_09_init_evt[36];

f32 fd_tik_09_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_09_neg_one_382 = -1.0f;

f32 fd_tik_09___local_PI_383 = 3.1415927f;

f32 fd_tik_09___local_3PIO2_385 = 4.712389f;

f32 fd_tik_09_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_09___local_PIO2_384 = 1.5707964f;

f32 fd_tik_09_one_381 = 1.0f;

f32 fd_tik_09_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_09_neg_one_365 = -1.0f;

f32 fd_tik_09___local_PI_366 = 3.1415927f;

f32 fd_tik_09___local_3PIO2_368 = 4.712389f;

f32 fd_tik_09_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_09___local_PIO2_367 = 1.5707964f;

f32 fd_tik_09_one_364 = 1.0f;

static BeroEntry bero_entry_data[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_10", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        8, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_06", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_tik09_dokan)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
    CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, 0, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    WAIT_MS(1500)
    CALL(evt_sub_intpl_init, 13, 0, 30, 100)
    LOOP(100)
        CALL(evt_sub_intpl_get_value)
        CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_1"))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, 30, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    RETURN
EVT_END()

EVT_BEGIN(tik_09_init_evt)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
    CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, 30, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(evt_tik09_dokan))
    RETURN
EVT_END()


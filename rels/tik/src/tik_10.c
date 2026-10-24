#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_itembox);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);

f32 fd_tik_10_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_10_neg_one_382;
f32 fd_tik_10___local_PI_383;
f32 fd_tik_10___local_3PIO2_385;
f32 fd_tik_10_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik_10___local_PIO2_384;
f32 fd_tik_10_one_381;
f32 fd_tik_10_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_10_neg_one_365;
f32 fd_tik_10___local_PI_366;
f32 fd_tik_10___local_3PIO2_368;
f32 fd_tik_10_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik_10___local_PIO2_367;
f32 fd_tik_10_one_364;
static BeroEntry bero_entry_data[4];
static EvtWord item_get00[34];
EvtWord tik_10_init_evt[44];

f32 fd_tik_10_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_10_neg_one_382 = -1.0f;

f32 fd_tik_10___local_PI_383 = 3.1415927f;

f32 fd_tik_10___local_3PIO2_385 = 4.712389f;

f32 fd_tik_10_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_10___local_PIO2_384 = 1.5707964f;

f32 fd_tik_10_one_381 = 1.0f;

f32 fd_tik_10_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_10_neg_one_365 = -1.0f;

f32 fd_tik_10___local_PI_366 = 3.1415927f;

f32 fd_tik_10___local_3PIO2_368 = 4.712389f;

f32 fd_tik_10_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_10___local_PIO2_367 = 1.5707964f;

f32 fd_tik_10_one_364 = 1.0f;

static BeroEntry bero_entry_data[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "tik_06", // nextMapName
        "dokan_3", // nextBeroName
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
        "tik_09", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_14", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(item_get00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), 148, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tik_10_init_evt)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_3"), STRING("dokan_1_s"))
    CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, 30, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_3"))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 0, 26, -110, 0, 0, PTR(item_get00), GSWF(1360))
    RETURN
EVT_END()


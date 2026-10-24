#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_npc_check);
USER_FUNC(evt_npc_set_link);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);

f32 fd_dou_13_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_dou_13_neg_one_382;
f32 fd_dou_13___local_PI_383;
f32 fd_dou_13___local_3PIO2_385;
f32 fd_dou_13_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_dou_13___local_PIO2_384;
f32 fd_dou_13_one_381;
f32 fd_dou_13_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_dou_13_neg_one_365;
f32 fd_dou_13___local_PI_366;
f32 fd_dou_13___local_3PIO2_368;
f32 fd_dou_13_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_dou_13___local_PIO2_367;
f32 fd_dou_13_one_364;
static BeroEntry bero_entry_data[3];
EvtWord dou_13_init_evt[44];

f32 fd_dou_13_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_13_neg_one_382 = -1.0f;

f32 fd_dou_13___local_PI_383 = 3.1415927f;

f32 fd_dou_13___local_3PIO2_385 = 4.712389f;

f32 fd_dou_13_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou_13___local_PIO2_384 = 1.5707964f;

f32 fd_dou_13_one_381 = 1.0f;

f32 fd_dou_13_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_13_neg_one_365 = -1.0f;

f32 fd_dou_13___local_PI_366 = 3.1415927f;

f32 fd_dou_13___local_3PIO2_368 = 4.712389f;

f32 fd_dou_13_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou_13___local_PIO2_367 = 1.5707964f;

f32 fd_dou_13_one_364 = 1.0f;

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
        "dou_08", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_09", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(dou_13_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_DOU9"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_check, STRING("キラー大砲１"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_link, STRING("キラー大砲１"), STRING("キラー大砲２"))
    END_IF
    CALL(evt_npc_check, STRING("キラー大砲３"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_link, STRING("キラー大砲３"), STRING("キラー大砲４"))
    END_IF
    RETURN
EVT_END()


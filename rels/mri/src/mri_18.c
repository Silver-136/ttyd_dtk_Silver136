#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_item_delete_check);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_npc_check_delete);
USER_FUNC(evt_puni_mem_init);
USER_FUNC(evt_puni_set_hamari);
USER_FUNC(evt_puni_set_yuka);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);

f32 fd_mri_18_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_mri_18_neg_one_382;
f32 fd_mri_18___local_PI_383;
f32 fd_mri_18___local_3PIO2_385;
f32 fd_mri_18_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_mri_18___local_PIO2_384;
f32 fd_mri_18_one_381;
f32 fd_mri_18_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_mri_18_neg_one_365;
f32 fd_mri_18___local_PI_366;
f32 fd_mri_18___local_3PIO2_368;
f32 fd_mri_18_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_mri_18___local_PIO2_367;
f32 fd_mri_18_one_364;
static BeroEntry bero_entry_data[5];
static EvtWord evt_ueki_04[20];
static KusaAccess ueki_access_data[7];
EvtWord mri_18_init_evt[105];

f32 fd_mri_18_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_mri_18_neg_one_382 = -1.0f;

f32 fd_mri_18___local_PI_383 = 3.1415927f;

f32 fd_mri_18___local_3PIO2_385 = 4.712389f;

f32 fd_mri_18_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_mri_18___local_PIO2_384 = 1.5707964f;

f32 fd_mri_18_one_381 = 1.0f;

f32 fd_mri_18_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_mri_18_neg_one_365 = -1.0f;

f32 fd_mri_18___local_PI_366 = 3.1415927f;

f32 fd_mri_18___local_3PIO2_368 = 4.712389f;

f32 fd_mri_18_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_mri_18___local_PIO2_367 = 1.5707964f;

f32 fd_mri_18_one_364 = 1.0f;

static BeroEntry bero_entry_data[] = {
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_12", // nextMapName
        "dokan3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan2", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_16", // nextMapName
        "dokan2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan4", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_ueki_04)
    CALL(evt_item_delete_check, STRING("item_ueki_04"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_04"), 121, 50, -320, -35, 4, GSWF(2873), 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki02", "S_ueki02", NULL},
    {1, "A_ueki03", "S_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", evt_ueki_04},
    {1, "A_ueki05", "S_ueki05", NULL},
    {1, "A_ueki08", "S_ueki08", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(mri_18_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 18, -89, -319, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 9, -139, 106, -87, -91, -317, -321)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    SWITCH(GSW(0))
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI4"))
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_GREATER(GSW(0), 113)
        CALL(evt_npc_check_delete, STRING("軍団ザコ"))
        CALL(evt_npc_check_delete, STRING("バリアーン２"))
    END_IF
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    RETURN
EVT_END()


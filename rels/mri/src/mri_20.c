#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_overwrite);
USER_FUNC(evt_item_delete_check);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_npc_check_delete);
USER_FUNC(evt_puni_mem_init);
USER_FUNC(evt_puni_set_hamari);
USER_FUNC(evt_puni_set_yuka);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);

f32 fd_mri_20_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_mri_20_neg_one_382;
f32 fd_mri_20___local_PI_383;
f32 fd_mri_20___local_3PIO2_385;
f32 fd_mri_20_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_mri_20___local_PIO2_384;
f32 fd_mri_20_one_381;
f32 fd_mri_20_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_mri_20_neg_one_365;
f32 fd_mri_20___local_PI_366;
f32 fd_mri_20___local_3PIO2_368;
f32 fd_mri_20_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_mri_20___local_PIO2_367;
f32 fd_mri_20_one_364;
BeroEntry bero_black_shadow[1];
static BeroEntry bero_entry_data[5];
static EvtWord evt_ueki_08[20];
static KusaAccess ueki_access_data[7];
EvtWord mri_20_init_evt[123];

f32 fd_mri_20_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_mri_20_neg_one_382 = -1.0f;

f32 fd_mri_20___local_PI_383 = 3.1415927f;

f32 fd_mri_20___local_3PIO2_385 = 4.712389f;

f32 fd_mri_20_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_mri_20___local_PIO2_384 = 1.5707964f;

f32 fd_mri_20_one_381 = 1.0f;

f32 fd_mri_20_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_mri_20_neg_one_365 = -1.0f;

f32 fd_mri_20___local_PI_366 = 3.1415927f;

f32 fd_mri_20___local_3PIO2_368 = 4.712389f;

f32 fd_mri_20_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_mri_20___local_PIO2_367 = 1.5707964f;

f32 fd_mri_20_one_364 = 1.0f;

BeroEntry bero_black_shadow[] = {
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_04", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

static BeroEntry bero_entry_data[] = {
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_03", // nextMapName
        "dokan", // nextBeroName
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
        "mri_02", // nextMapName
        "dokan3", // nextBeroName
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
        6, // directionId
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

static EVT_BEGIN(evt_ueki_08)
    CALL(evt_item_delete_check, STRING("item_ueki_08"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_08"), 142, 250, -320, 130, 4, GSWF(2875), 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki02", "S_ueki02", NULL},
    {1, "A_ueki03", "S_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", NULL},
    {1, "A_ueki05", "S_ueki05", NULL},
    {1, "A_ueki08", "S_ueki08", evt_ueki_08},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(mri_20_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 20, -89, -319, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 45, -319, 92, -87, -91, -317, -321)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
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
    IF_INT_GREATER(GSW(0), 113)
        CALL(evt_npc_check_delete, STRING("軍団ザコ２"))
        CALL(evt_npc_check_delete, STRING("バリアーン"))
    END_IF
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    IF_INT_EQUAL(GSW(0), 90)
        SET_VALUE(LW(0), STRING("dokan1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan1"), PTR(bero_black_shadow))
        SET_VALUE(LW(0), STRING("dokan1"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    RETURN
EVT_END()


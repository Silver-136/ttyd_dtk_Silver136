#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "data_tables.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_mobj_brick);
USER_FUNC(evt_mobj_flag_onoff);
USER_FUNC(evt_npc_change_fbat_mode);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_sub_get_system_flag);

f32 fd_hei_13_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_hei_13_neg_one_382;
f32 fd_hei_13___local_PI_383;
f32 fd_hei_13___local_3PIO2_385;
f32 fd_hei_13_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_hei_13___local_PIO2_384;
f32 fd_hei_13_one_381;
f32 fd_hei_13_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_hei_13_neg_one_365;
f32 fd_hei_13___local_PI_366;
f32 fd_hei_13___local_3PIO2_368;
f32 fd_hei_13_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_hei_13___local_PIO2_367;
f32 fd_hei_13_one_364;
static BeroEntry bero_entry_data[5];
static TreeAccess ki_data[2];
EvtWord evt_anm_kan[59];
EvtWord hei_13_init_evt[110];

f32 fd_hei_13_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hei_13_neg_one_382 = -1.0f;

f32 fd_hei_13___local_PI_383 = 3.1415927f;

f32 fd_hei_13___local_3PIO2_385 = 4.712389f;

f32 fd_hei_13_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hei_13___local_PIO2_384 = 1.5707964f;

f32 fd_hei_13_one_381 = 1.0f;

f32 fd_hei_13_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hei_13_neg_one_365 = -1.0f;

f32 fd_hei_13___local_PI_366 = 3.1415927f;

f32 fd_hei_13___local_3PIO2_368 = 4.712389f;

f32 fd_hei_13_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hei_13___local_PIO2_367 = 1.5707964f;

f32 fd_hei_13_one_364 = 1.0f;

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
        "hei_00", // nextMapName
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
        "hei_01", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "a_dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "a_dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "a_dokan_2", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "a_dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static TreeAccess ki_data[] = {
    {0, "a_ki_1", "miki_", "ha_temae", "ha_usiro", NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(evt_anm_kan)
    IF_INT_EQUAL(GF(0), 1)
        GOTO(99)
    END_IF
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(GSW(301), 10)
            SET_VALUE(GF(0), 1)
            WAIT_MS(200)
            CALL(evt_item_entry, STRING("item_tukushi"), 175, 565, 15, -65, 5, -1, 0)
            ELSE
            ADD(GSW(301), 1)
            IF_INT_EQUAL(GSW(301), 10)
                SET_VALUE(GF(0), 1)
                WAIT_MS(200)
                CALL(evt_item_entry, STRING("item_tukushi"), 175, 565, 15, -65, 5, -1, 0)
            END_IF
        END_IF
    END_INLINE_EVENT
    LABEL(99)
    CALL(evt_map_playanim, STRING("anm_kan_1"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(hei_13_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SET_VALUE(GF(0), 0)
    SET_VALUE(GSW(301), 0)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_mobj_badgeblk, STRING("m"), -320, 60, -100, 299, 0, GSWF(1797), 1)
    SET_VALUE(LSWF(3), 0)
    CALL(evt_mobj_brick, STRING("mm"), 325, 175, -100, 0, 0, 0, LSWF(3))
    IF_INT_EQUAL(LSWF(3), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("mm"), 2)
    END_IF
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_CLEAR(LW(0), 4096)
        CALL(evt_item_entry, STRING("i"), 292, 325, 200, -100, 16, GSWF(1798), 0)
    END_IF
    CALL(evt_mobj_brick, STRING("10m"), 325, 60, -100, 0, 3, 0, GSWF(1799))
    SET_VALUE(LW(0), PTR(ki_data))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    CALL(evt_map_playanim, STRING("anm_mugi"), 1, 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG1_HEI1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG1_HEI1"))
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_CLEAR(LW(0), 4096)
        CALL(evt_run_case_evt, 13, 1, STRING("a_kan_5"), 0, PTR(evt_anm_kan), 0)
    END_IF
    RETURN
EVT_END()


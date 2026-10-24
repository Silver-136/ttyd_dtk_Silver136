#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"

#pragma force_active on

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_overwrite);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mobj_jumpstand_blue);
USER_FUNC(evt_mobj_recovery_blk);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_sub_get_system_flag);

f32 fd_gon_00_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_gon_00_neg_one_382;
f32 fd_gon_00___local_PI_383;
f32 fd_gon_00___local_3PIO2_385;
f32 fd_gon_00_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_gon_00___local_PIO2_384;
f32 fd_gon_00_one_381;
f32 fd_gon_00_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_gon_00_neg_one_365;
f32 fd_gon_00___local_PI_366;
f32 fd_gon_00___local_3PIO2_368;
f32 fd_gon_00_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_gon_00___local_PIO2_367;
f32 fd_gon_00_one_364;
static BeroEntry bero_entry_data[4];
static BeroEntry e3_bero[2];
EvtWord gon_00_init_evt[77];

f32 fd_gon_00_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gon_00_neg_one_382 = -1.0f;

f32 fd_gon_00___local_PI_383 = 3.1415927f;

f32 fd_gon_00___local_3PIO2_385 = 4.712389f;

f32 fd_gon_00_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gon_00___local_PIO2_384 = 1.5707964f;

f32 fd_gon_00_one_381 = 1.0f;

f32 fd_gon_00_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gon_00_neg_one_365 = -1.0f;

f32 fd_gon_00___local_PI_366 = 3.1415927f;

f32 fd_gon_00___local_3PIO2_368 = 4.712389f;

f32 fd_gon_00_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gon_00___local_PIO2_367 = 1.5707964f;

f32 fd_gon_00_one_364 = 1.0f;

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
        "a_usiro_bero", // nextBeroName
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
        "gon_01", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_3", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_01", // nextMapName
        "w_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static BeroEntry e3_bero[] = {
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
        "gon_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(gon_00_init_evt)
    SET_VALUE(GSWF(255), 1)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_snd_envon, 272, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 160, 60, -90, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 6, -150, 260, 0, 0, 0)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump"), 230, 0, 0, -90, 0, 0)
    CALL(evt_map_playanim, STRING("hokori"), 1, 0)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 4096)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("w_bero"), PTR(e3_bero))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "jin.h"

#pragma force_active on

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mobj_breaking_floor);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);

f32 fd_jin_11_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_jin_11_neg_one_382;
f32 fd_jin_11___local_PI_383;
f32 fd_jin_11___local_3PIO2_385;
f32 fd_jin_11_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_jin_11___local_PIO2_384;
f32 fd_jin_11_one_381;
f32 fd_jin_11_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_jin_11_neg_one_365;
f32 fd_jin_11___local_PI_366;
f32 fd_jin_11___local_3PIO2_368;
f32 fd_jin_11_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_jin_11___local_PIO2_367;
f32 fd_jin_11_one_364;
EvtWord jin_11_ana[12];
static EvtWord s_close[12];
static EvtWord s_open[12];
static BeroEntry bero_entry_data[3];
EvtWord evt_yuka[5];
EvtWord jin_11_init_evt[54];

f32 fd_jin_11_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_jin_11_neg_one_382 = -1.0f;

f32 fd_jin_11___local_PI_383 = 3.1415927f;

f32 fd_jin_11___local_3PIO2_385 = 4.712389f;

f32 fd_jin_11_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_jin_11___local_PIO2_384 = 1.5707964f;

f32 fd_jin_11_one_381 = 1.0f;

f32 fd_jin_11_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_jin_11_neg_one_365 = -1.0f;

f32 fd_jin_11___local_PI_366 = 3.1415927f;

f32 fd_jin_11___local_3PIO2_368 = 4.712389f;

f32 fd_jin_11_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_jin_11___local_PIO2_367 = 1.5707964f;

f32 fd_jin_11_one_364 = 1.0f;

EVT_BEGIN(jin_11_ana)
    SET_VALUE(GSWF(2079), 1)
    CALL(evt_bero_mapchange, STRING("jin_07"), 0)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static EVT_BEGIN(s_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door10"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door10"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_10", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_close, // enterAnimeArg
        s_open, // exitAnimeArg
    },
    {
        "ana", // hitName
        0, 0, // type..sfxId
        4, // directionId
        845, // centerPosition
        115, // centerPositionY
        77, // centerPositionZ
        -1, // length
        NULL, 0, // enterEvent..caseType
        (s32*)jin_11_ana, // exitEvent
        "jin_07", // nextMapName
        NULL, // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_yuka)
    SET_VALUE(GSW(0), 208)
    RETURN
EVT_END()

EVT_BEGIN(jin_11_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG4_JIN1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -50, 60, -40, 0, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), 40, 60, -40, 0, GSWF(5551), 0)
    IF_INT_LESS(GSW(0), 208)
        CALL(evt_mobj_breaking_floor, STRING("mobj_bf00"), 845, 115, 5, 0, PTR(evt_yuka), GSWF(2233))
    END_IF
    RETURN
EVT_END()


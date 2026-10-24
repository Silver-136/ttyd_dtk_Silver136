#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon_3d);

f32 fd_tik_13_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_13_neg_one_382;
f32 fd_tik_13___local_PI_383;
f32 fd_tik_13___local_3PIO2_385;
f32 fd_tik_13_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik_13___local_PIO2_384;
f32 fd_tik_13_one_381;
f32 fd_tik_13_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_13_neg_one_365;
f32 fd_tik_13___local_PI_366;
f32 fd_tik_13___local_3PIO2_368;
f32 fd_tik_13_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik_13___local_PIO2_367;
f32 fd_tik_13_one_364;
static BeroEntry bero_entry_data[2];
EvtWord tik_13_init_evt[60];

f32 fd_tik_13_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_13_neg_one_382 = -1.0f;

f32 fd_tik_13___local_PI_383 = 3.1415927f;

f32 fd_tik_13___local_3PIO2_385 = 4.712389f;

f32 fd_tik_13_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_13___local_PIO2_384 = 1.5707964f;

f32 fd_tik_13_one_381 = 1.0f;

f32 fd_tik_13_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_13_neg_one_365 = -1.0f;

f32 fd_tik_13___local_PI_366 = 3.1415927f;

f32 fd_tik_13___local_3PIO2_368 = 4.712389f;

f32 fd_tik_13_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_13___local_PIO2_367 = 1.5707964f;

f32 fd_tik_13_one_364 = 1.0f;

static BeroEntry bero_entry_data[] = {
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
        "tik_02", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(tik_13_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK6"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_map_playanim, STRING("anm_mizu"), 0, 0)
            CALL(evt_map_checkanim, STRING("anm_mizu"), LW(0), LW(1))
            WAIT_MS(LW(1))
            CALL(evt_mapobj_get_position, STRING("Tshatu"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_AMB_WATERDROP2"), LW(0), LW(1), LW(2), 0)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_item_entry, STRING("item_00"), 125, -30, 52, -70, 16, GSWF(5596), 0)
    RETURN
EVT_END()


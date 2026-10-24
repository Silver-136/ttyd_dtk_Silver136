#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "driver/mapdrv.h"
#include "driver/envdrv.h"
#include "evt/evt_bero.h"

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_npc_change_fbat_mode);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
void hei_test_water_tev(MapTevCallbackInfo* info);

f32 fd_hei_09_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_hei_09_neg_one_382;
f32 fd_hei_09___local_PI_383;
f32 fd_hei_09___local_3PIO2_385;
f32 fd_hei_09_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_hei_09___local_PIO2_384;
f32 fd_hei_09_one_381;
f32 fd_hei_09_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_hei_09_neg_one_365;
f32 fd_hei_09___local_PI_366;
f32 fd_hei_09___local_3PIO2_368;
f32 fd_hei_09_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_hei_09___local_PIO2_367;
f32 fd_hei_09_one_364;
static BeroEntry bero_entry_data[3];
EvtWord hei_09_init_evt[44];

f32 fd_hei_09_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hei_09_neg_one_382 = -1.0f;

f32 fd_hei_09___local_PI_383 = 3.1415927f;

f32 fd_hei_09___local_3PIO2_385 = 4.712389f;

f32 fd_hei_09_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hei_09___local_PIO2_384 = 1.5707964f;

f32 fd_hei_09_one_381 = 1.0f;

f32 fd_hei_09_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hei_09_neg_one_365 = -1.0f;

f32 fd_hei_09___local_PI_366 = 3.1415927f;

f32 fd_hei_09___local_3PIO2_368 = 4.712389f;

f32 fd_hei_09_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hei_09___local_PIO2_367 = 1.5707964f;

f32 fd_hei_09_one_364 = 1.0f;

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
        "hei_08", // nextMapName
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
        "hei_10", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(hei_09_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG1_TORIDE1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG1_HEI3"))
    CALL(evt_snd_set_rev_mode, 1)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    CALL(evt_map_set_tevcallback, 1, PTR(hei_test_water_tev))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("mizu"), 16777216)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("mizu"), 256)
    RETURN
EVT_END()

void hei_test_water_tev(MapTevCallbackInfo* info) {
    envSetWater(info);
}

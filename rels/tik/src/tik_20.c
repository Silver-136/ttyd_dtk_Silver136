#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_movefloor.h"
#include "driver/hitdrv.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_hit_damage_return_set);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);

f32 fd_tik_20_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_20_neg_one_382;
f32 fd_tik_20___local_PI_383;
f32 fd_tik_20___local_3PIO2_385;
f32 fd_tik_20_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik_20___local_PIO2_384;
f32 fd_tik_20_one_381;
f32 fd_tik_20_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_20_neg_one_365;
f32 fd_tik_20___local_PI_366;
f32 fd_tik_20___local_3PIO2_368;
f32 fd_tik_20_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik_20___local_PIO2_367;
f32 fd_tik_20_one_364;
static BeroEntry bero_entry_data[2];
static HitDamageReturn hitDmgRetPoints[7];
EvtWord tik_20_init_evt[104];

f32 fd_tik_20_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_20_neg_one_382 = -1.0f;

f32 fd_tik_20___local_PI_383 = 3.1415927f;

f32 fd_tik_20___local_3PIO2_385 = 4.712389f;

f32 fd_tik_20_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_20___local_PIO2_384 = 1.5707964f;

f32 fd_tik_20_one_381 = 1.0f;

f32 fd_tik_20_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_20_neg_one_365 = -1.0f;

f32 fd_tik_20___local_PI_366 = 3.1415927f;

f32 fd_tik_20___local_3PIO2_368 = 4.712389f;

f32 fd_tik_20_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_20___local_PIO2_367 = 1.5707964f;

f32 fd_tik_20_one_364 = 1.0f;

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
        "tik_15", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_1", {0.0f, -1000.0f, 0.0f}},
    {"mod_2", {0.0f, -1000.0f, 0.0f}},
    {"mod_3", {0.0f, -1000.0f, 0.0f}},
    {"mod_4", {0.0f, -1000.0f, 0.0f}},
    {"mod_5", {0.0f, -1000.0f, 0.0f}},
    {"ue", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(tik_20_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK7"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_playanim, STRING("anm_mizu"), 1, 0)
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change_01"), STRING("ue"))
    SET_VALUE(LW(0), STRING("dai_3"))
    SET_VALUE(LW(1), STRING("a_dai_3"))
    SET_VALUE(LW(2), -10)
    SET_VALUE(LW(3), 30)
    SET_VALUE(LW(4), 900)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    CALL(evt_item_entry, STRING("item_01"), 274, 180, 100, -125, 16, GSWF(1367), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk01"), -50, 60, -75, 0, GSWF(5535), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk02"), 0, 100, -25, 0, GSWF(5536), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk03"), 50, 60, -75, 0, GSWF(5537), 0)
    RETURN
EVT_END()


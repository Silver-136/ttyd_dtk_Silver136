#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "driver/hitdrv.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_dou_normal_puka);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_hit_damage_return_set);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_sub_random);

f32 fd_dou_08_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_dou_08_neg_one_382;
f32 fd_dou_08___local_PI_383;
f32 fd_dou_08___local_3PIO2_385;
f32 fd_dou_08_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_dou_08___local_PIO2_384;
f32 fd_dou_08_one_381;
f32 fd_dou_08_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_dou_08_neg_one_365;
f32 fd_dou_08___local_PI_366;
f32 fd_dou_08___local_3PIO2_368;
f32 fd_dou_08_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_dou_08___local_PIO2_367;
f32 fd_dou_08_one_364;
static BeroEntry bero_entry_data[3];
static EvtWord dou_puka_event_main0[60];
static EvtWord dou_puka_event_main1[60];
static HitDamageReturn hitDmgRetPoints[10];
EvtWord dou_08_init_evt[82];

f32 fd_dou_08_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_08_neg_one_382 = -1.0f;

f32 fd_dou_08___local_PI_383 = 3.1415927f;

f32 fd_dou_08___local_3PIO2_385 = 4.712389f;

f32 fd_dou_08_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou_08___local_PIO2_384 = 1.5707964f;

f32 fd_dou_08_one_381 = 1.0f;

f32 fd_dou_08_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_08_neg_one_365 = -1.0f;

f32 fd_dou_08___local_PI_366 = 3.1415927f;

f32 fd_dou_08___local_3PIO2_368 = 4.712389f;

f32 fd_dou_08_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou_08___local_PIO2_367 = 1.5707964f;

f32 fd_dou_08_one_364 = 1.0f;

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
        "dou_13", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_07", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(dou_puka_event_main0)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(dou_puka_event_main1)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {"mod_05", {0.0f, -1000.0f, 0.0f}},
    {"mod_06", {0.0f, -1000.0f, 0.0f}},
    {"mod_07", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_01", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_02", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(dou_08_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_DOU8"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_map_playanim, STRING("nami"), 1, 0)
    CALL(evt_map_playanim, STRING("moku"), 1, 0)
    CALL(evt_map_playanim, STRING("tubo"), 1, 0)
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    SET_VALUE(LW(0), STRING("s08_bin"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main0), LW(10))
    SET_VALUE(LW(0), STRING("s08_taru"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main1), LW(10))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_hibi"), 1)
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change_01"), STRING("Ship_Change_01"))
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change_02"), STRING("Ship_Change_02"))
    RETURN
EVT_END()


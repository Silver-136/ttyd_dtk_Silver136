#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_movefloor.h"
#include "driver/hitdrv.h"

#pragma force_active on

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_hit_damage_return_set);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mobj_flag_onoff);
USER_FUNC(evt_mobj_lv_blk);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_tornadoswitch_blue);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);

f32 fd_tik_16_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_16_neg_one_382;
f32 fd_tik_16___local_PI_383;
f32 fd_tik_16___local_3PIO2_385;
f32 fd_tik_16_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik_16___local_PIO2_384;
f32 fd_tik_16_one_381;
f32 fd_tik_16_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_16_neg_one_365;
f32 fd_tik_16___local_PI_366;
f32 fd_tik_16___local_3PIO2_368;
f32 fd_tik_16_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik_16___local_PIO2_367;
f32 fd_tik_16_one_364;
static BeroEntry bero_entry_data[7];
static HitDamageReturn hitDmgRetPoints[9];
static EvtWord evt_switch_00_init[26];
static EvtWord evt_switch_00_after[11];
static EvtWord evt_switch_00[56];
static EvtWord evt_switch_01_init[26];
static EvtWord evt_switch_01_after[11];
static EvtWord evt_switch_01[56];
EvtWord tik_16_init_evt[115];

f32 fd_tik_16_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_16_neg_one_382 = -1.0f;

f32 fd_tik_16___local_PI_383 = 3.1415927f;

f32 fd_tik_16___local_3PIO2_385 = 4.712389f;

f32 fd_tik_16_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_16___local_PIO2_384 = 1.5707964f;

f32 fd_tik_16_one_381 = 1.0f;

f32 fd_tik_16_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_16_neg_one_365 = -1.0f;

f32 fd_tik_16___local_PI_366 = 3.1415927f;

f32 fd_tik_16___local_3PIO2_368 = 4.712389f;

f32 fd_tik_16_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_16___local_PIO2_367 = 1.5707964f;

f32 fd_tik_16_one_364 = 1.0f;

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_17", // nextMapName
        "e_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_17", // nextMapName
        "e_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_15", // nextMapName
        "w_bero_2", // nextBeroName
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
        "tik_15", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_00", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "nok_01", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_1", {0.0f, -1000.0f, 0.0f}},
    {"mod_2", {-250.0f, 0.0f, 0.0f}},
    {"mod_3", {-250.0f, 0.0f, 0.0f}},
    {"mod_4", {250.0f, 0.0f, 0.0f}},
    {"mod_5", {250.0f, 0.0f, 0.0f}},
    {"mod_6", {0.0f, -1000.0f, 0.0f}},
    {"a_dai_1", {0.0f, -1000.0f, 0.0f}},
    {"a_dai_2", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EVT_BEGIN(evt_switch_00_init)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("S_dakan_1"))
    IF_INT_EQUAL(GSWF(1345), 1)
        CALL(evt_mapobj_trans, 1, STRING("S_dakan_1"), 0, 30, 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_1"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_1_k"), 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_00_after)
    IF_INT_EQUAL(GSWF(1345), 0)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_00)
    CALL(evt_mapobj_get_position, STRING("S_dakan_1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 30, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_dakan_1"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_1"))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_1_k"), 1)
    SET_VALUE(LW(0), STRING("dokan_1"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_01_init)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_2"), STRING("S_dakan_2"))
    IF_INT_EQUAL(GSWF(1346), 1)
        CALL(evt_mapobj_trans, 1, STRING("S_dakan_2"), 0, 30, 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_2"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_2_k"), 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_01_after)
    IF_INT_EQUAL(GSWF(1346), 0)
        SET_VALUE(LW(0), STRING("dokan_2"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_01)
    CALL(evt_mapobj_get_position, STRING("S_dakan_2"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 30, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_dakan_2"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_2"))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_2_k"), 1)
    SET_VALUE(LW(0), STRING("dokan_2"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(tik_16_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK7"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL_CHILD(PTR(evt_switch_00_init))
    CALL_CHILD(PTR(evt_switch_01_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_switch_00_after))
    CALL_CHILD(PTR(evt_switch_01_after))
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_playanim, STRING("anm_mizu"), 1, 0)
    CALL(evt_mobj_lv_blk, STRING("block"), 0, 0, 0, 0, GSWF(1347), STRING("MOBJ_Lv1BigBigBlock"))
    IF_INT_EQUAL(GSWF(1347), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("block"), 128)
    END_IF
    SET_VALUE(LW(0), STRING("dai_3"))
    SET_VALUE(LW(1), STRING("a_dai_3"))
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), -75)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 1)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    CALL(evt_mobj_tornadoswitch_blue, 0, STRING("switch00"), -125, 0, 0, PTR(evt_switch_00), GSWF(1345))
    CALL(evt_mobj_tornadoswitch_blue, 0, STRING("switch01"), 125, 0, 0, PTR(evt_switch_01), GSWF(1346))
    CALL(evt_mobj_powerupblk, STRING("pblk1"), 125, 160, -100, 0, GSWF(5534), 0)
    RETURN
EVT_END()


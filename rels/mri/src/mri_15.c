#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"

#pragma force_active on

USER_FUNC(evt_bero_exec_get);
USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_item_delete_check);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_pauseanim);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_npc_battle_start);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_set_battle_info);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_wait_battle_end);
USER_FUNC(evt_puni_mem_init);
USER_FUNC(evt_puni_set_hamari);
USER_FUNC(evt_puni_set_yuka);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxon_3d);

f32 fd_mri_15_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_mri_15_neg_one_382;
f32 fd_mri_15___local_PI_383;
f32 fd_mri_15___local_3PIO2_385;
f32 fd_mri_15_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_mri_15___local_PIO2_384;
f32 fd_mri_15_one_381;
f32 fd_mri_15_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_mri_15_neg_one_365;
f32 fd_mri_15___local_PI_366;
f32 fd_mri_15___local_3PIO2_368;
f32 fd_mri_15_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_mri_15___local_PIO2_367;
f32 fd_mri_15_one_364;
static EvtWord bero_s_doa_close[10];
static EvtWord bero_s_doa_open[10];
EvtWord evt_dokan[12];
static BeroEntry bero_entry_data[3];
static EvtWord door_switch_init[17];
static EvtWord door_switch[59];
EvtWord dai3_escape[32];
EvtWord dai3_battle_init[66];
EvtWord dai3_battle[140];
EvtWord ueki_evt_01[20];
static KusaAccess ueki_access_data[7];
EvtWord mri_15_init_evt[156];

f32 fd_mri_15_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_mri_15_neg_one_382 = -1.0f;

f32 fd_mri_15___local_PI_383 = 3.1415927f;

f32 fd_mri_15___local_3PIO2_385 = 4.712389f;

f32 fd_mri_15_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_mri_15___local_PIO2_384 = 1.5707964f;

f32 fd_mri_15_one_381 = 1.0f;

f32 fd_mri_15_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_mri_15_neg_one_365 = -1.0f;

f32 fd_mri_15___local_PI_366 = 3.1415927f;

f32 fd_mri_15___local_3PIO2_368 = 4.712389f;

f32 fd_mri_15_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_mri_15___local_PIO2_367 = 1.5707964f;

f32 fd_mri_15_one_364 = 1.0f;

static EVT_BEGIN(bero_s_doa_close)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), STRING("S_doa"))
    SPAWN_EVENT(PTR(evt_mri_door_close))
    RETURN
EVT_END()

static EVT_BEGIN(bero_s_doa_open)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), STRING("S_doa"))
    SPAWN_EVENT(PTR(evt_mri_door_open))
    RETURN
EVT_END()

EVT_BEGIN(evt_dokan)
    IF_INT_EQUAL(GSWF(2866), 0)
        SET_VALUE(GSWF(2866), 1)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_06", // nextMapName
        "e_bero", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        bero_s_doa_close, // enterAnimeArg
        bero_s_doa_open, // exitAnimeArg
    },
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        (s32*)evt_dokan, // exitEvent
        "mri_14", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(door_switch_init)
    IF_INT_EQUAL(GSWF(2865), 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        ELSE
        CALL(evt_map_playanim, STRING("light"), 2, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(door_switch)
    CALL(evt_mapobj_get_position, STRING("S_lights"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_UNLOCK"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_DOOR_LAMP"), LW(0), LW(1), LW(2), 0)
    IF_INT_EQUAL(GSWF(2865), 0)
        SET_VALUE(GSWF(2865), 1)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_map_playanim, STRING("light"), 0, 0)
        ELSE
        SET_VALUE(GSWF(2865), 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_map_playanim, STRING("light"), 0, 0)
        CALL(evt_map_pauseanim, 0, STRING("light"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(dai3_escape)
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), STRING("S_doa"))
        SPAWN_EVENT(PTR(evt_mri_door_close))
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("第三勢力幹部"), -193, 29, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_position, STRING("第三勢力幹部"), 0, -1000, 0)
    WAIT_WHILE(1)
    RETURN
EVT_END()

EVT_BEGIN(dai3_battle_init)
    CALL(evt_npc_set_position, STRING("第三勢力部下Ａ"), -119, 0, 29)
    CALL(evt_npc_set_ry, STRING("第三勢力部下Ａ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("第三勢力部下Ａ"), 1536)
    CALL(evt_npc_flag_onoff, 1, STRING("第三勢力部下Ａ"), 1073741856)
    CALL(evt_npc_set_position, STRING("第三勢力部下Ｂ"), -129, 0, 66)
    CALL(evt_npc_set_ry, STRING("第三勢力部下Ｂ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("第三勢力部下Ｂ"), 1536)
    CALL(evt_npc_flag_onoff, 1, STRING("第三勢力部下Ｂ"), 1073741856)
    IF_INT_EQUAL(GSWF(2866), 0)
        CALL(evt_npc_set_position, STRING("第三勢力幹部"), -5, 0, 29)
        CALL(evt_npc_set_ry, STRING("第三勢力幹部"), 270)
        CALL(evt_npc_flag_onoff, 1, STRING("第三勢力幹部"), 1536)
        CALL(evt_npc_flag_onoff, 1, STRING("第三勢力幹部"), 1073741856)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(dai3_battle)
    CALL(evt_seq_wait, 2)
    IF_INT_EQUAL(GSWF(2866), 0)
        SPAWN_EVENT_GET_ID(PTR(dai3_escape), LW(10))
    END_IF
    CALL(evt_bero_exec_wait, 65536)
    LOOP(0)
        CALL(evt_mario_get_name_hitobj_ride, LW(0))
        CALL(evt_bero_exec_get, LW(1))
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        IF_INT_EQUAL(LW(1), 65536)
            IF_STR_EQUAL(LW(0), STRING("A_yuka"))
                IF_INT_LESS_OR_EQUAL(LW(10), 20)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        ADD(LW(0), -10)
        ADD(LW(2), -20)
        CALL(evt_npc_move_position, STRING("第三勢力部下Ａ"), LW(0), LW(2), 400, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        ADD(LW(0), -20)
        ADD(LW(2), 20)
        CALL(evt_npc_move_position, STRING("第三勢力部下Ｂ"), LW(0), LW(2), 400, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_npc_set_battle_info, STRING("第三勢力部下Ａ"), 11)
    CALL(evt_npc_battle_start, STRING("第三勢力部下Ａ"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    IF_INT_EQUAL(GSWF(2866), 0)
        DELETE_EVENT(LW(10))
        CALL(evt_npc_set_position, STRING("第三勢力幹部"), 0, -1000, 0)
        SET_VALUE(GSWF(2866), 1)
    END_IF
    SET_VALUE(GSW(0), 110)
    CALL(evt_npc_set_position, STRING("第三勢力部下Ａ"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("第三勢力部下Ｂ"), 0, -1000, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(ueki_evt_01)
    CALL(evt_item_delete_check, STRING("item_ueki01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki01"), 125, -37, 0, 96, 0, GSWF(5616), 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", ueki_evt_01},
    {1, "A_ueki02", "S_ueki02", NULL},
    {1, "A_ueki03", "S_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", NULL},
    {1, "A_ueki05", "S_ueki05", NULL},
    {1, "A_ueki06", "S_ueki06", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(mri_15_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 15, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 134, 0, 63, 2, -2, -1000, -1000)
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
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    SWITCH(GSW(0))
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("kawa_sibu_01"), 1, 0)
    SET_VALUE(LW(0), -100)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), -75)
    SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
    SPAWN_EVENT(PTR(mri_waterfall_effect))
    SET_VALUE(LW(0), 60)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), -40)
    SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
    SPAWN_EVENT(PTR(mri_waterfall_effect))
    IF_INT_EQUAL(GSW(0), 109)
        CALL_CHILD(PTR(dai3_battle_init))
        SPAWN_EVENT(PTR(dai3_battle))
    END_IF
    SPAWN_EVENT(PTR(door_switch_init))
    CALL(evt_run_case_evt, 9, 1, STRING("A_sui"), 0, PTR(door_switch), 0)
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "party.h"
#include "mario/mario.h"
#include "driver/animdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "aji.h"

USER_FUNC(check_kou_evt);
USER_FUNC(dot_mario);
USER_FUNC(evt_bero_exec_wait);
USER_FUNC(evt_bero_get_entername);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_get_start_position);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_bero_overwrite);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_del_case_evt);
USER_FUNC(evt_exit_case_evt);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_fade_entry);
USER_FUNC(evt_fade_set_mapchange_type);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mapobj_scale);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_flag_onoff);
USER_FUNC(evt_mario_get_mode);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_mode);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_wait_landon);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_npc_set_autotalkpose);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_peach_transform_gundan_off);
USER_FUNC(evt_peach_transform_gundan_on);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_set_case_wrk);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfx_dist);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_get_value);
USER_FUNC(evt_sub_intpl_init);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
static USER_FUNC(peach_set_pose);

static EvtWord s_door_open[22];
static EvtWord s_door_close[22];
static BeroEntry bero_entry_data[2];
EvtWord peach_evt_bero_out1[25];
BeroEntry bero_data_peach1[2];
EvtWord peach_evt_bero_out2[25];
BeroEntry bero_data_peach2[2];
EvtWord evt_kou1[115];
EvtWord evt_kou2[155];
const char* rokka_data[21];
EvtWord evt_rokka_open[127];
EvtWord evt_rokka_close[165];
EvtWord evt_rokka_init[34];
EvtWord peach_evt_fuku_g[36];
EvtWord peach_evt_ka_open[46];
EvtWord peach_evt_ka_close[46];
EvtWord peach_evt_kou_in[172];
EvtWord peach_evt_kou_out[89];
EvtWord peach_evt_kou1_in[72];
EvtWord peach_evt_kou1_out[43];
EvtWord peach_evt_kou2_in[72];
EvtWord peach_evt_kou2_out[43];
EvtWord s_bero_out[90];
static EvtWord peach_evt_stg3[262];
static EvtWord evt_memo1[21];
static EvtWord evt_memo2[14];
EvtWord aji_09_init_evt[226];

static EVT_BEGIN(s_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_1"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_1"), LW(0), 0, 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        "n_bero_4", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(peach_evt_bero_out1)
    IF_INT_EQUAL(GSWF(4204), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_133"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        SET_VALUE(LW(0), 1)
        ELSE
        SET_VALUE(LW(0), 0)
    END_IF
    RETURN
EVT_END()

BeroEntry bero_data_peach1[] = {
    {
        "s_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)peach_evt_bero_out1, // exitEvent
        "aji_01", // nextMapName
        "n_bero_4", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(peach_evt_bero_out2)
    IF_INT_EQUAL(GSWF(4204), 0)
        SET_VALUE(LW(0), 0)
        ELSE
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_155_01"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        SET_VALUE(LW(0), 1)
    END_IF
    RETURN
EVT_END()

BeroEntry bero_data_peach2[] = {
    {
        "s_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        (s32*)peach_evt_bero_out2, // exitEvent
        "aji_17", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_kou1)
    CALL(evt_hit_bind_mapobj, STRING("A_kou1_ka_omo"), STRING("S_kou1_ka"))
    CALL(evt_hit_bind_mapobj, STRING("A_kou1_ka_ura"), STRING("S_kou1_ka"))
    IF_INT_EQUAL(GF(0), 0)
        CALL(evt_mapobj_get_position, STRING("S_kou1_ka"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CURTAIN_OPEN1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 11, 1000, 100, 500)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_mapobj_scale, 1, STRING("S_kou1_ka"), LW(0), FLOAT(1.0f), FLOAT(1.0f))
        CALL(evt_hit_bind_update, STRING("A_kou1_ka_omo"))
        CALL(evt_hit_bind_update, STRING("A_kou1_ka_ura"))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        SET_VALUE(GF(0), 1)
        ELSE
        CALL(evt_mapobj_get_position, STRING("S_kou1_ka"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CURTAIN_CLOSE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 11, 100, 1000, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_mapobj_scale, 1, STRING("S_kou1_ka"), LW(0), FLOAT(1.0f), FLOAT(1.0f))
        CALL(evt_hit_bind_update, STRING("A_kou1_ka_omo"))
        CALL(evt_hit_bind_update, STRING("A_kou1_ka_ura"))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        SET_VALUE(GF(0), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_kou2)
    CALL(evt_hit_bind_mapobj, STRING("A_kou2_ka_omo"), STRING("S_kou2_ka"))
    CALL(evt_hit_bind_mapobj, STRING("A_kou2_ka_ura"), STRING("S_kou2_ka"))
    IF_INT_EQUAL(GSWF(4217), 0)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("tenjo"))
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mario_normalize)
            CALL(dot_mario)
            WAIT_MS(1000)
            SET_VALUE(GSWF(4217), 1)
            CALL(evt_snd_bgmoff, 512)
            CALL(evt_snd_envoff, 512)
            CALL(evt_mario_key_onoff, 1)
        END_IF
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        CALL(evt_mapobj_get_position, STRING("S_kou2_ka"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CURTAIN_OPEN1"), LW(0), LW(1), LW(2), 0)
        IF_INT_EQUAL(GSWF(4217), 1)
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DOT_MARIO1"))
        END_IF
        CALL(evt_sub_intpl_msec_init, 11, 1000, 200, 500)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_mapobj_scale, 1, STRING("S_kou2_ka"), LW(0), FLOAT(1.0f), FLOAT(1.0f))
        CALL(evt_hit_bind_update, STRING("A_kou2_ka_omo"))
        CALL(evt_hit_bind_update, STRING("A_kou2_ka_ura"))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        SET_VALUE(GF(1), 1)
        ELSE
        CALL(evt_mapobj_get_position, STRING("S_kou2_ka"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CURTAIN_CLOSE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, 11, 200, 1000, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_mapobj_scale, 1, STRING("S_kou2_ka"), LW(0), FLOAT(1.0f), FLOAT(1.0f))
        CALL(evt_hit_bind_update, STRING("A_kou2_ka_omo"))
        CALL(evt_hit_bind_update, STRING("A_kou2_ka_ura"))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        SET_VALUE(GF(1), 0)
    END_IF
    RETURN
EVT_END()

const char* rokka_data[] = {
    "R_door_1",
    "a_door_1",
    "a_door_1_omote",
    "a_door_1_ura",
    "a_R_1_asi",
    "R_door_2",
    "a_door_2",
    "a_door_2_omote",
    "a_door_2_ura",
    "a_R_2_asi",
    "R_door_3",
    "a_door_3",
    "a_door_3_omote",
    "a_door_3_ura",
    "a_R_3_asi",
    "R_door_4",
    "a_door_4",
    "a_door_4_omote",
    "a_door_4_ura",
    "a_R_4_asi",
    NULL,
};

EVT_BEGIN(evt_rokka_open)
    CALL(evt_mario_get_pos, 0, LW(13), LW(14), LW(15))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_LOCKER_OPEN1"), LW(13), LW(14), LW(15), 0)
    SET_VALUE(LW(5), LW(0))
    SET_VALUE(LW(6), LW(1))
    SET_VALUE(LW(7), LW(2))
    SET_VALUE(LW(8), LW(3))
    SET_VALUE(LW(9), LW(4))
    CALL(evt_sub_intpl_init, 12, 0, 120, 40)
    LOOP(41)
        CALL(evt_sub_intpl_get_value)
        CALL(evt_mapobj_rotate, 1, LW(5), 0, LW(0), 0)
        CALL(evt_hit_bind_update, LW(6))
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(LW(0), LW(5))
    SET_VALUE(LW(1), LW(6))
    SET_VALUE(LW(2), LW(7))
    SET_VALUE(LW(3), LW(8))
    SET_VALUE(LW(4), LW(9))
    SET_VALUE(LW(10), 0)
    CALL(evt_run_case_evt, 10, 1, LW(2), 0, PTR(evt_rokka_close), LW(11))
    SET_VALUE(LW(0), LW(5))
    SET_VALUE(LW(1), LW(6))
    SET_VALUE(LW(2), LW(7))
    SET_VALUE(LW(3), LW(8))
    SET_VALUE(LW(4), LW(9))
    SET_VALUE(LW(10), 1)
    CALL(evt_run_case_evt, 0, 1, LW(4), 0, PTR(evt_rokka_close), LW(12))
    CALL(evt_set_case_wrk, LW(11), 11, LW(11))
    CALL(evt_set_case_wrk, LW(11), 12, LW(12))
    CALL(evt_set_case_wrk, LW(12), 11, LW(11))
    CALL(evt_set_case_wrk, LW(12), 12, LW(12))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_rokka_close)
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_del_case_evt, 0, LW(12))
        CALL(evt_mario_get_pos, 0, LW(13), LW(14), LW(15))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_LOCKER_CLOSE1"), LW(13), LW(14), LW(15), 0)
        SET_VALUE(LW(5), LW(0))
        SET_VALUE(LW(6), LW(1))
        SET_VALUE(LW(7), LW(2))
        SET_VALUE(LW(8), LW(3))
        SET_VALUE(LW(9), LW(4))
        CALL(evt_sub_intpl_init, 12, 120, 0, 40)
        LOOP(41)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_rotate, 1, LW(5), 0, LW(0), 0)
            CALL(evt_hit_bind_update, LW(6))
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), LW(5))
        SET_VALUE(LW(1), LW(6))
        SET_VALUE(LW(2), LW(7))
        SET_VALUE(LW(3), LW(8))
        SET_VALUE(LW(4), LW(9))
        CALL(evt_run_case_evt, 9, 1, LW(2), 0, PTR(evt_rokka_open), 0)
        ELSE
        CALL(evt_del_case_evt, 0, LW(11))
        CALL(evt_mario_get_pos, 0, LW(13), LW(14), LW(15))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_LOCKER_CLOSE1"), LW(13), LW(14), LW(15), 0)
        SET_VALUE(LW(5), LW(0))
        SET_VALUE(LW(6), LW(1))
        SET_VALUE(LW(7), LW(2))
        SET_VALUE(LW(8), LW(3))
        SET_VALUE(LW(9), LW(4))
        CALL(evt_sub_intpl_init, 12, 120, 0, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL(evt_mapobj_rotate, 1, LW(5), 0, LW(0), 0)
            CALL(evt_hit_bind_update, LW(6))
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), LW(5))
        SET_VALUE(LW(1), LW(6))
        SET_VALUE(LW(2), LW(7))
        SET_VALUE(LW(3), LW(8))
        SET_VALUE(LW(4), LW(9))
        CALL(evt_run_case_evt, 10, 1, LW(3), 0, PTR(evt_rokka_open), 0)
    END_IF
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_rokka_init)
    SET_INT_READ_ADDR(PTR(&rokka_data))
    LOOP(0)
        READ_INT3(LW(0), LW(1), LW(2))
        READ_INT2(LW(3), LW(4))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
        SET_VALUE(LW(10), 0)
        CALL(evt_run_case_evt, 9, 1, LW(2), 0, PTR(evt_rokka_open), 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_fuku_g)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("peach_aji_134"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("peach_aji_135"), 0, 0)
    SET_VALUE(GSWF(4203), 1)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("R_fuku_g"), 1)
    CALL(evt_hitobj_onoff, STRING("A_R_fuku_g"), 1, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_ka_open)
    SET_VALUE(LW(2), LW(0))
    CALL(evt_mapobj_get_position, LW(0), LW(1), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_CURTAIN_OPEN1"), LW(1), LW(3), LW(4), 0)
    CALL(evt_sub_intpl_msec_init, 11, 1000, 100, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_mapobj_scale, 1, LW(2), LW(0), FLOAT(1.0f), FLOAT(1.0f))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_ka_close)
    SET_VALUE(LW(2), LW(0))
    CALL(evt_mapobj_get_position, LW(0), LW(1), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_CURTAIN_CLOSE1"), LW(1), LW(3), LW(4), 0)
    CALL(evt_sub_intpl_msec_init, 11, 100, 1000, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_mapobj_scale, 1, LW(2), LW(0), FLOAT(1.0f), FLOAT(1.0f))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_kou_in)
    SET_VALUE(LW(10), LW(0))
    SET_VALUE(LW(9), LW(1))
    SET_VALUE(LW(0), LW(9))
    CALL_CHILD(PTR(peach_evt_ka_open))
    CALL(evt_mario_mov_pos2, LW(10), -125, FLOAT(60.0f))
    SET_VALUE(LW(0), LW(9))
    CALL_CHILD(PTR(peach_evt_ka_close))
    IF_INT_EQUAL(GSWF(4203), 1)
        CALL(evt_snd_bgmoff, 1024)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 167)
            IF_STR_EQUAL(LW(9), STRING("S_kou1_ka"))
                IF_INT_EQUAL(GSWF(4205), 0)
                    CALL(evt_msg_print, 0, STRING("peach_aji_137_02"), 0, STRING("mario"))
                    SET_VALUE(LW(15), 1)
                    RETURN
                END_IF
                ELSE
                IF_INT_EQUAL(GSWF(4205), 1)
                    CALL(evt_msg_print, 0, STRING("peach_aji_137_02"), 0, STRING("mario"))
                    SET_VALUE(LW(15), 1)
                    RETURN
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 168)
            IF_INT_EQUAL(GSWF(4206), 0)
                CALL(evt_msg_print, 0, STRING("peach_aji_154"), 0, STRING("mario"))
            END_IF
        END_IF
        WAIT_MS(1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_CHANGE_CLOTH1"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_snd_sfx_dist, LW(3), 0)
        WAIT_MS(1000)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 166)
            WAIT_MS(1000)
            CALL(evt_msg_print, 0, STRING("peach_aji_136"), 0, STRING("mario"))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_CHANGE_CLOTH1"), LW(0), LW(1), LW(2), LW(3))
            CALL(evt_snd_sfx_dist, LW(3), 0)
            WAIT_MS(2500)
        END_IF
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 166)
            CALL(evt_msg_print, 0, STRING("peach_aji_137"), 0, STRING("mario"))
            IF_STR_EQUAL(LW(9), STRING("S_kou1_ka"))
                SET_VALUE(GSWF(4205), 1)
                ELSE
                SET_VALUE(GSWF(4205), 0)
            END_IF
        END_IF
        IF_INT_EQUAL(GSWF(4204), 0)
            SET_VALUE(GSWF(4204), 1)
            CALL(evt_peach_transform_gundan_on)
            ELSE
            SET_VALUE(GSWF(4204), 0)
            CALL(evt_peach_transform_gundan_off)
        END_IF
    END_IF
    SET_VALUE(LW(15), 0)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_kou_out)
    SET_VALUE(LW(10), LW(0))
    SET_VALUE(LW(9), LW(1))
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    SET_VALUE(LW(0), LW(9))
    CALL_CHILD(PTR(peach_evt_ka_open))
    CALL(evt_mario_mov_pos2, LW(10), -55, FLOAT(60.0f))
    SET_VALUE(LW(0), LW(9))
    CALL_CHILD(PTR(peach_evt_ka_close))
    IF_INT_EQUAL(GSWF(4203), 1)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 166)
            CALL(evt_msg_print, 0, STRING("peach_aji_137_01"), 0, STRING("mario"))
        END_IF
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 168)
            IF_STR_EQUAL(LW(9), STRING("S_kou1_ka"))
                IF_INT_EQUAL(GSWF(4205), 0)
                    RETURN
                END_IF
                ELSE
                IF_INT_EQUAL(GSWF(4205), 1)
                    RETURN
                END_IF
            END_IF
            IF_INT_EQUAL(GSWF(4206), 0)
                CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_7"), STRING("P_T_13"))
                CALL(evt_msg_print, 0, STRING("peach_aji_155"), 0, STRING("mario"))
                CALL(peach_set_pose, STRING("P_S_1"))
                CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
                SET_VALUE(GSWF(4206), 1)
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_kou1_in)
    CALL(check_kou_evt, 0, 1, LF(0))
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam_letter_box_disable, 1)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(0), 90)
    SET_VALUE(LW(1), STRING("S_kou1_ka"))
    CALL_CHILD(PTR(peach_evt_kou_in))
    IF_INT_EQUAL(LW(15), 0)
        IF_INT_EQUAL(GSWF(4203), 1)
            IF_INT_EQUAL(GSWF(4204), 0)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fuku1_g"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1_p"), 1)
                ELSE
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1_g"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fuku1_p"), 1)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam_letter_box_disable, 0)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_kou1_out)
    CALL(check_kou_evt, 1, 1, LF(0))
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam_letter_box_disable, 1)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(0), 90)
    SET_VALUE(LW(1), STRING("S_kou1_ka"))
    CALL_CHILD(PTR(peach_evt_kou_out))
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam_letter_box_disable, 0)
        ELSE
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 166)
            SET_VALUE(GSW_STORY_PROGRESS, 167)
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_kou2_in)
    CALL(check_kou_evt, 0, 0, LF(0))
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam_letter_box_disable, 1)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(0), 160)
    SET_VALUE(LW(1), STRING("S_kou2_ka"))
    CALL_CHILD(PTR(peach_evt_kou_in))
    IF_INT_EQUAL(LW(15), 0)
        IF_INT_EQUAL(GSWF(4203), 1)
            IF_INT_EQUAL(GSWF(4204), 0)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fuku2_g"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2_p"), 1)
                ELSE
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2_g"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fuku2_p"), 1)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam_letter_box_disable, 0)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_kou2_out)
    CALL(check_kou_evt, 1, 0, LF(0))
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam_letter_box_disable, 1)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(0), 160)
    SET_VALUE(LW(1), STRING("S_kou2_ka"))
    CALL_CHILD(PTR(peach_evt_kou_out))
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam_letter_box_disable, 0)
        ELSE
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 166)
            SET_VALUE(GSW_STORY_PROGRESS, 167)
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(s_bero_out)
#ifdef VERSION_G8ME01
    CALL(evt_mario_key_onoff, 0)
#endif
    IF_INT_EQUAL(GSWF(4204), 0)
        SET_VALUE(LW(1), 0)
        SPAWN_EVENT(PTR(bero_open_door_play_se))
        CALL(evt_sub_intpl_msec_init, 11, 0, 180, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
            IF_INT_EQUAL(LW(10), 0)
                SET_FLOAT_VALUE(LW(0), FLOAT(180.0f))
                CALL_CHILD(PTR(s_door_open))
                BREAK_LOOP
                ELSE
                CALL_CHILD(PTR(s_door_open))
            END_IF
        END_LOOP
        BEGIN_INLINE_EVENT
            CALL(evt_bero_get_start_position, LW(0), LW(1), LW(2))
            CALL(evt_mario_flag_onoff, 1, 256)
            CALL(evt_mario_mov_pos2, LW(0), 450, FLOAT(150.0f))
        END_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_fade_entry, 10, 300, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(1000)
        CALL(evt_fade_set_mapchange_type, 0, 0, 0, 9, 300)
        CALL(evt_bero_mapchange, STRING("aji_17"), STRING("w_bero"))
        ELSE
#ifndef VERSION_G8ME01
        CALL(evt_mario_key_onoff, 0)
#endif
        CALL(evt_msg_print, 0, STRING("peach_aji_155_01"), 0, 0)
#ifdef VERSION_G8ME01
    END_IF
#endif
        CALL(evt_mario_key_onoff, 1)
#ifndef VERSION_G8ME01
    END_IF
#endif
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg3)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 167)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2"), 1)
        CALL(evt_mapobj_flag_onoff, 0, 0, STRING("R_fuku_g"), 1)
        CALL(evt_hitobj_onoff, STRING("A_R_fuku_g"), 1, 1)
        CALL(evt_run_case_evt, 9, 1, STRING("A_R_fuku_g"), 0, PTR(peach_evt_fuku_g), 0)
        ELSE
        IF_INT_EQUAL(GSWF(4204), 0)
            IF_INT_EQUAL(GSWF(4205), 0)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fuku2_g"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2_p"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1"), 1)
                ELSE
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fuku1_g"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1_p"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2"), 1)
            END_IF
            ELSE
            CALL(evt_peach_transform_gundan_on)
            IF_INT_EQUAL(GSWF(4205), 0)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2_g"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fuku2_p"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1"), 1)
                ELSE
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1_g"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_fuku1_p"), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2"), 1)
            END_IF
        END_IF
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("R_fuku_g"), 1)
    END_IF
    CALL(evt_run_case_evt, 9, 1, STRING("A_kou1_ka_omo"), 0, PTR(peach_evt_kou1_in), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("A_kou1_ka_ura"), 0, PTR(peach_evt_kou1_out), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("A_kou2_ka_omo"), 0, PTR(peach_evt_kou2_in), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("A_kou2_ka_ura"), 0, PTR(peach_evt_kou2_out), 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    IF_INT_LESS(GSW_STORY_PROGRESS, 168)
        SET_VALUE(LW(0), STRING("s_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("s_bero"), PTR(bero_data_peach1))
        SET_VALUE(LW(0), STRING("s_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        ELSE
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, 9, 1, STRING("s_bero"), 0, PTR(s_bero_out), 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 166)
        WAIT_MS(600)
        CALL(evt_msg_print, 0, STRING("peach_aji_132"), 0, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 168)
        WAIT_MS(500)
        CALL(evt_mario_set_dir, 90, 300, 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_152"), 0, STRING("mario"))
        CALL(evt_cam3d_evt_set, 0, 60, 442, 0, 41, 42, 800, 11)
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("peach_aji_153"), 0, STRING("mario"))
        CALL(evt_cam3d_evt_off, 800, 11)
        WAIT_MS(800)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_memo1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_33"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    IF_INT_LESS(GSW_STORY_PROGRESS, 364)
        SET_VALUE(GSW_STORY_PROGRESS, 364)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_memo2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_33_01"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(aji_09_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_hitobj_onoff, STRING("A_R_fuku_g"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("R_fuku_g"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2"), 1)
    SET_VALUE(LF(0), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 166)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 168)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mario_set_mode, 1)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(PTR(peach_evt_stg3))
        ELSE
        SET_VALUE(GF(0), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_kou1_ka_omo"), 0, PTR(evt_kou1), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_kou1_ka_ura"), 0, PTR(evt_kou1), 0)
        SET_VALUE(GF(1), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_kou2_ka_omo"), 0, PTR(evt_kou2), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_kou2_ka_ura"), 0, PTR(evt_kou2), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku1_p"), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_fuku2_g"), 1)
    END_IF
    CALL_CHILD(PTR(evt_rokka_init))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("tenjo"))
        CALL(evt_cam_letter_box_disable, 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_party_cont_onoff, 1, 0)
        CALL(evt_mario_set_pos, 170, 107, -115)
        CALL(evt_party_set_pos, 0, 160, 107, -105)
        CALL(evt_party_set_pos, 1, 150, 107, -95)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, 2)
            CALL(evt_mario_cont_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_party_cont_onoff, 1, 1)
            CALL(evt_mario_wait_landon)
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_cam_letter_box_disable, 0)
        END_INLINE_EVENT
    END_IF
    SPAWN_EVENT(PTR(evt_emergency))
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_memo1"), 0, PTR(evt_memo1), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_memo2"), 0, PTR(evt_memo2), 0)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN2"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG7_AJI2"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_memo"), 1)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
        CALL(evt_snd_envoff, 512)
    END_IF
    CALL(evt_snd_set_rev_mode, 1)
    RETURN
EVT_END()

void partySetFamicomMode(PartyEntry* party, s32 mode);

USER_FUNC(dot_mario) {
    PartyEntry* party = partyGetPtr(0);

    if (!animGroupBaseAsync("d_mario", 0, NULL)) {
        return EVT_RETURN_BLOCK;
    }
    marioSetFamicomMode(1);
    partySetFamicomMode(party, 1);
    return EVT_RETURN_DONE;
}

static USER_FUNC(peach_set_pose) {
    const char* pose = (const char*)evtGetValue(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    marioChgPose(pose);
    return EVT_RETURN_DONE;
}
USER_FUNC(check_kou_evt) {
    EvtWord* args = event->args;
    s32 value;
    s32 result;

    evtGetValue(event, args[0]);
    value = evtGetValue(event, args[1]);
    result = 0;
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) == 0xA6 && evtGetValue(NULL, GSWF(4203)) == 1) {
        result = 1;
    }
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) == 0xA8 && evtGetValue(NULL, GSWF(4206)) == 0 &&
        value == evtGetValue(NULL, GSWF(4205))) {
        result = 1;
    }
    evtSetValue(event, args[2], result);
    return EVT_RETURN_DONE;
}

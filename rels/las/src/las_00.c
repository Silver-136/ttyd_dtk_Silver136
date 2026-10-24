#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "stdio.h"
#include "evt/evt_bero.h"
#include "manager/evtmgr_cmd.h"

USER_FUNC(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_eff);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_get_party);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_party);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgm_scope);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_telop_entry);
static USER_FUNC(make_name);

f32 fd_las_00_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_las_00_neg_one_382;
f32 fd_las_00___local_PI_383;
f32 fd_las_00___local_3PIO2_385;
f32 fd_las_00_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_las_00___local_PIO2_384;
f32 fd_las_00_one_381;
f32 fd_las_00_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_las_00_neg_one_365;
f32 fd_las_00___local_PI_366;
f32 fd_las_00___local_3PIO2_368;
f32 fd_las_00_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_las_00___local_PIO2_367;
f32 fd_las_00_one_364;
static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
static EvtWord first_evt[184];
static EvtWord shan_init[46];
EvtWord las_00_init_evt[32];

f32 fd_las_00_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_las_00_neg_one_382 = -1.0f;

f32 fd_las_00___local_PI_383 = 3.1415927f;

f32 fd_las_00___local_3PIO2_385 = 4.712389f;

f32 fd_las_00_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_las_00___local_PIO2_384 = 1.5707964f;

f32 fd_las_00_one_381 = 1.0f;

f32 fd_las_00_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_las_00_neg_one_365 = -1.0f;

f32 fd_las_00___local_PI_366 = 3.1415927f;

f32 fd_las_00___local_3PIO2_368 = 4.712389f;

f32 fd_las_00_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_las_00___local_PIO2_367 = 1.5707964f;

f32 fd_las_00_one_364 = 1.0f;

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_05", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_01", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(first_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_set_party, 0, 1)
    END_IF
    CALL(evt_mario_set_pos, -420, 10, 0)
    CALL(evt_party_set_pos, 0, -470, 10, 0)
    CALL(evt_cam3d_evt_set, -385, 1441, 763, -37, 642, 100, 0, 11)
    WAIT_FRAMES(1)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 577, 35, 520, -155, 135, 99, 7000, 0)
        WAIT_MS(7000)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_seq_wait, 2)
    CALL(evt_mario_normalize)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_hitobj_onoff, STRING("w_bero"), 1, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -350, 0, FLOAT(150.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -300, 0, FLOAT(150.0f))
    CALL(evt_hitobj_onoff, STRING("w_bero"), 1, 1)
    LOOP(0)
        IF_INT_NOT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -18, 20, 205, -453, 80, -131, 2400, 11)
    WAIT_MS(2400)
    CALL(evt_party_stop, 0)
    CALL(evt_party_set_dir, 0, 270, 300)
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, 300)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_01"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    SET_VALUE(GSW(0), 382)
    CALL_CHILD(PTR(bero_case_entry))
    CALL(evt_cam3d_evt_off, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_telop_entry, 17)
    RETURN
EVT_END()

static EVT_BEGIN(shan_init)
    SET_VALUE(LW(0), 1)
    LOOP(12)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), 1)
        CALL(evt_mapobj_get_position, LW(1), LW(2), LW(3), LW(4))
        SUBTRACT_FLOAT(LW(3), FLOAT(9.0f))
        CALL(evt_eff, 0, STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(1.0f), 1000, 0, 0, 0, 0, 0, 0)
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(las_00_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    IF_INT_GREATER_OR_EQUAL(GSW(0), 382)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    IF_INT_LESS(GSW(0), 382)
        SPAWN_EVENT(PTR(first_evt))
    END_IF
    CALL_CHILD(PTR(shan_init))
    RETURN
EVT_END()

static USER_FUNC(make_name) {
    EvtWord* args = event->args;
    static char name[16];

    sprintf(name, "S_shan_rou_0%d\0", evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}


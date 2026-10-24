#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "aji.h"

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_eff);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_fog_onoff);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_fog);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_scale);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_get_mode);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_set_mode);
USER_FUNC(evt_mario_wait_move_end);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_npc_check_delete);
USER_FUNC(evt_npc_set_autotalkpose);
USER_FUNC(evt_peach_set_condition);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfx_dist);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
USER_FUNC(evt_sub_random);
USER_FUNC(peach_chk_aftershower);
USER_FUNC(peach_set_pos);

f32 fd_aji_15_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_aji_15_neg_one_382;
f32 fd_aji_15___local_PI_383;
f32 fd_aji_15___local_3PIO2_385;
f32 fd_aji_15_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_aji_15___local_PIO2_384;
f32 fd_aji_15_one_381;
f32 fd_aji_15_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_aji_15_neg_one_365;
f32 fd_aji_15___local_PI_366;
f32 fd_aji_15___local_3PIO2_368;
f32 fd_aji_15_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_aji_15___local_PIO2_367;
f32 fd_aji_15_one_364;
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[2];
EvtWord peach_evt_yuge[43];
EvtWord peach_evt_syawa_main[798];
EvtWord peach_evt_syawa_init[63];
EvtWord aji_15_init_evt[58];

f32 fd_aji_15_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_aji_15_neg_one_382 = -1.0f;

f32 fd_aji_15___local_PI_383 = 3.1415927f;

f32 fd_aji_15___local_3PIO2_385 = 4.712389f;

f32 fd_aji_15_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_aji_15___local_PIO2_384 = 1.5707964f;

f32 fd_aji_15_one_381 = 1.0f;

f32 fd_aji_15_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_aji_15_neg_one_365 = -1.0f;

f32 fd_aji_15___local_PI_366 = 3.1415927f;

f32 fd_aji_15___local_3PIO2_368 = 4.712389f;

f32 fd_aji_15_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_aji_15___local_PIO2_367 = 1.5707964f;

f32 fd_aji_15_one_364 = 1.0f;

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_16", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(peach_evt_yuge)
    LOOP(0)
        CALL(evt_sub_random, 38, LW(0))
        SUBTRACT(LW(0), 19)
        ADD(LW(0), -30)
        CALL(evt_sub_random, 20, LW(1))
        SUBTRACT(LW(1), 10)
        ADD(LW(1), 60)
        CALL(evt_eff, 0, STRING("kemuri_test"), 1, LW(0), LW(1), -15, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(700)
    END_LOOP
    RETURN
EVT_END()

USER_FUNC(peach_set_pos) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    MarioWork* mario = marioGetPtr();

    mario->position = (Vec){x, y, z};
    return EVT_RETURN_DONE;
}

EVT_BEGIN(peach_evt_syawa_main)
    CALL(evt_snd_bgmoff, 10240)
    CALL(evt_snd_envoff, 1024)
    CALL(evt_mario_key_onoff, 0)
    CALL(peach_chk_aftershower, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
        ELSE
        CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_2"), STRING("P_T_2"))
    END_IF
    IF_INT_EQUAL(GSWF(4220), 0)
        CALL(evt_msg_print, 0, STRING("peach_aji_35"), 0, STRING("mario"))
        ELSE
        CALL(evt_msg_print, 0, STRING("peach_aji_35_1"), 0, STRING("mario"))
    END_IF
    CALL(evt_cam3d_evt_set, -30, 130, 336, -30, 68, -58, 1400, 11)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(0), -60)
        CALL(evt_mario_mov_pos2, -85, -60, FLOAT(48.0f))
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(0), 0)
        IF_INT_GREATER(LW(2), -10)
            SET_VALUE(LW(0), 30)
            SET_VALUE(LW(2), -15)
            CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(48.0f))
        END_IF
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(0), 25)
        CALL(evt_mario_mov_pos2, 25, -41, FLOAT(48.0f))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -28, -45, FLOAT(48.0f))
    END_INLINE_EVENT
    WAIT_MS(1400)
    CALL(evt_mario_wait_move_end)
    CALL(evt_mario_cont_onoff, 0)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 13, 0, 1000, 300)
        LABEL(20)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(20)
        END_IF
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 13, 1000, 800, 200)
        LABEL(21)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(21)
        END_IF
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, -45, -15, 600)
    LABEL(10)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(peach_set_pos, FLOAT(-28.0f), GW(0), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(10)
    END_IF
    CALL(evt_mario_cont_onoff, 1)
    WAIT_MS(800)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_CHANGE_CLOTH1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfx_dist, LW(3), 0)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_HANG_CLOTH1"), 0, 0, 0, 0)
    CALL(evt_mapobj_trans, 1, STRING("p_fuku"), 0, -10, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("p_fuku"), 1)
    CALL(evt_sub_intpl_msec_init, 0, -10, 0, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("p_fuku"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_map_playanim, STRING("fuku"), 0, 0)
    CALL(evt_map_checkanim, STRING("fuku"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_SHOWER1"), -40, 0, -30, 0)
    WAIT_MS(300)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_SHOWER2"), -40, 0, -30, LW(10))
    CALL(evt_sub_intpl_msec_init, 12, FLOAT(0.0f), FLOAT(1.0f), 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_scale, 1, STRING("S_syawa"), LW(0), FLOAT(1.0f), FLOAT(1.0f))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(500)
    SPAWN_EVENT_GET_ID(PTR(peach_evt_yuge), LW(9))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_sub_random, 2, LW(3))
    SWITCH(LW(3))
        CASE_EQUAL(0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_HAMMING1"), LW(0), LW(1), LW(2), 0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_HAMMING2"), LW(0), LW(1), LW(2), 0)
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_HAMMING3"), LW(0), LW(1), LW(2), 0)
    END_SWITCH
    WAIT_MS(1500)
    CALL(evt_map_set_fog, 2, 900, 1500, 255, 255, 255)
    CALL(evt_map_fog_onoff, 1)
    CALL(evt_sub_intpl_msec_init, 12, 900, 0, 5000)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_map_set_fog, 2, LW(0), 1200, 255, 255, 255)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("peach_aji_36"), 0, STRING("mario"))
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_SHOWER1"), -40, 0, -30, 0)
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_sub_intpl_msec_init, 12, FLOAT(1.0f), FLOAT(0.0f), 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
    CALL(evt_mapobj_scale, 1, STRING("S_syawa"), LW(0), FLOAT(1.0f), FLOAT(1.0f))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    WAIT_MS(500)
    DELETE_EVENT(LW(9))
    WAIT_MS(1800)
    CALL(evt_sub_intpl_msec_init, 13, 0, 900, 5000)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_map_set_fog, 2, LW(0), 1200, 255, 255, 255)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_map_fog_onoff, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_HANG_CLOTH1"), 0, 0, 0, 0)
    CALL(evt_map_playanim, STRING("taoru2"), 0, 0)
    CALL(evt_map_checkanim, STRING("taoru2"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_sub_intpl_msec_init, 0, 0, -10, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("taoru"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("taoru"), 1)
    WAIT_MS(1500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_HANG_CLOTH1"), 0, 0, 0, 0)
    CALL(evt_mapobj_trans, 1, STRING("taoru"), 0, -10, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("taoru"), 1)
    CALL(evt_sub_intpl_msec_init, 0, -10, 0, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("taoru"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_map_playanim, STRING("taoru"), 0, 0)
    CALL(evt_map_checkanim, STRING("taoru"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(800)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_HANG_CLOTH1"), 0, 0, 0, 0)
    CALL(evt_map_playanim, STRING("fuku2"), 0, 0)
    CALL(evt_map_checkanim, STRING("fuku2"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_sub_intpl_msec_init, 0, 0, -10, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("p_fuku"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_fuku"), 1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_PEACH_CHANGE_CLOTH1"), 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_peach_set_condition, 1)
    CALL(evt_mario_cont_onoff, 0)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 13, 800, 1000, 200)
        LABEL(22)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(22)
        END_IF
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 13, 1000, 0, 300)
        LABEL(23)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(23)
        END_IF
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 11, -15, -45, 600)
    LABEL(11)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(peach_set_pos, FLOAT(-28.0f), GW(0), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(11)
    END_IF
    CALL(evt_mario_cont_onoff, 1)
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 30, 130, 336, 30, 60, -58, 1250, 11)
    CALL(evt_mario_mov_pos2, 30, -45, FLOAT(48.0f))
    CALL(evt_mario_wait_move_end)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_2"), STRING("P_T_2"))
    CALL(evt_msg_print, 0, STRING("peach_aji_37"), 0, STRING("mario"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    CALL(evt_snd_bgmon, 160, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    SET_VALUE(GSWF(4220), 1)
    CALL(evt_cam3d_evt_off, 800, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_syawa_init)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_fuku"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("taoru"), 1)
    CALL(evt_mapobj_scale, 1, STRING("S_syawa"), 0, 1, 1)
    CALL(evt_map_playanim, STRING("syawa"), 1, 0)
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("taoru"), 1)
    CALL(evt_map_playanim, STRING("taoru"), 0, 0)
    CALL(evt_map_checkanim, STRING("taoru"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_run_case_evt, 9, 1, STRING("A_furo"), 0, PTR(peach_evt_syawa_main), 0)
    RETURN
EVT_END()

EVT_BEGIN(aji_15_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_GREATER_OR_EQUAL(GSW(0), 373)
        CALL(evt_npc_check_delete, STRING("軍団ザコエリート"))
    END_IF
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_set_mode, 1)
        CALL(evt_npc_check_delete, STRING("軍団ザコエリート"))
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
        CALL(evt_snd_envoff, 512)
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
        CALL(evt_snd_envoff, 512)
    END_IF
    CALL(evt_snd_set_rev_mode, 1)
    CALL_CHILD(PTR(peach_evt_syawa_init))
    IF_INT_EQUAL(GSWF(4199), 0)
        SET_VALUE(GSWF(4199), 1)
    END_IF
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()

USER_FUNC(peach_chk_aftershower) {
    EvtWord* args = event->args;

    if (marioGetPtr()->peachFlags & 0x10) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

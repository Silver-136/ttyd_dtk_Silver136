#include "driver/msgdrv.h"
#include "memory.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "system.h"

#include "stdio.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "evt/evt_bero.h"
#include "evt/evt_mario.h"
#include "evt/evt_npc.h"
#include "evt/evt_case.h"
#include "evt/evt_sub.h"

USER_FUNC(evt_npc_set_stay_anim);
USER_FUNC(evt_npc_set_stop_anim);
USER_FUNC(evt_npc_set_walk_anim);
USER_FUNC(get_togenoko_pos);
USER_FUNC(kpa_chg_pose);
static USER_FUNC(make_name);
void kpa_stg1_008_cb(s32 type, smartEntry* smart);
void kpa_stg1_014_2_cb(s32 type, smartEntry* smart);
void kpa_stg1_036_cb(s32 type, smartEntry* smart);
void kpa_stg1_034_cb(s32 type, smartEntry* smart);

EvtWord kpa_01_door_open[219];
EvtWord kpa_01_door_close[237];
EvtWord kpa_01_touzyo_evt[217];
EvtWord kpa_01_syuppatu_evt[1729];
EvtWord kpa_01_pos_check[23];
EvtWord douzou_1_touch_evt[19];
EvtWord douzou_2_touch_evt[19];
EvtWord douzou_3_touch_evt[43];
EvtWord togenoko_A_talk_evt[8];
EvtWord togenoko_B_talk_evt[8];
EvtWord togenoko_C_init_evt[14];
EvtWord hammer_AB_init_evt[13];
EvtWord hammer_AB_talk_evt[111];
EvtWord hammer_C_talk_evt[19];
EvtWord hammer_D_talk_evt[8];
EvtWord hammer_E_init_evt[9];
EvtWord hammer_E_talk_evt[8];
EvtWord baba_init_evt[13];
EvtWord kamek_init_evt[9];
EvtWord hkoopa_init_evt[9];
static NpcSetupInfo npcEnt[13];
static EvtWord shan_init[46];
EvtWord kpa_01_init_evt[88];

EVT_BEGIN(kpa_01_door_open)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("トゲノコＡ"), 270)
        CALL(evt_npc_move_position, STRING("トゲノコＡ"), -729, -5, 1000, 0, 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("トゲノコＢ"), 270)
    CALL(evt_npc_move_position, STRING("トゲノコＢ"), -729, 5, 1000, 0, 0)
    WAIT_MS(1000)
    SET_FLOAT_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 1)
    CALL(evt_mapobj_get_position, STRING("door"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_CASTLE1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 2000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
        SET_FLOAT_VALUE(GW(0), LW(0))
        SET_VALUE(GW(1), LW(1))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GW(1), 1)
                BREAK_LOOP
            END_IF
            SET_FLOAT_VALUE(LW(2), GW(0))
            CALL(evt_mapobj_rotate, 0, STRING("door_1"), 0, LW(2), 0)
            MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
            CALL(evt_mapobj_rotate, 0, STRING("door_2"), 0, LW(2), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("トゲノコＡ"), STRING("W_2"))
        LOOP(0)
            IF_INT_NOT_EQUAL(GW(1), 1)
                BREAK_LOOP
            END_IF
            SET_FLOAT_VALUE(LW(2), GW(0))
            ADD_FLOAT(LW(2), FLOAT(25.0f))
            CALL(get_togenoko_pos, FLOAT(-750.0f), FLOAT(-50.0f), FLOAT(-750.0f), FLOAT(-7.5f), LW(2), LW(3), LW(4))
            CALL(evt_npc_set_position, STRING("トゲノコＡ"), LW(3), 0, LW(4))
            IF_INT_GREATER_OR_EQUAL(LW(2), 90)
                CALL(evt_npc_set_ry, STRING("トゲノコＡ"), 90)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_stop_anim, STRING("トゲノコＡ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("トゲノコＢ"), STRING("W_2"))
        LOOP(0)
            IF_INT_NOT_EQUAL(GW(1), 1)
                BREAK_LOOP
            END_IF
            SET_FLOAT_VALUE(LW(5), GW(0))
            ADD_FLOAT(LW(5), FLOAT(25.0f))
            MULTIPLY_FLOAT(LW(5), FLOAT(-1.0f))
            CALL(get_togenoko_pos, FLOAT(-750.0f), FLOAT(50.0f), FLOAT(-750.0f), FLOAT(7.5f), LW(5), LW(6), LW(7))
            CALL(evt_npc_set_position, STRING("トゲノコＢ"), LW(6), 0, LW(7))
            IF_INT_LESS_OR_EQUAL(LW(5), -90)
                CALL(evt_npc_set_ry, STRING("トゲノコＢ"), 90)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_stop_anim, STRING("トゲノコＢ"))
    END_INLINE_EVENT
    LOOP(0)
        IF_INT_NOT_EQUAL(GW(1), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(kpa_01_door_close)
    SET_FLOAT_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 180, 0, 2000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
        SET_FLOAT_VALUE(GW(0), LW(0))
        SET_VALUE(GW(1), LW(1))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GW(1), 1)
                BREAK_LOOP
            END_IF
            SET_FLOAT_VALUE(LW(2), GW(0))
            CALL(evt_mapobj_rotate, 0, STRING("door_1"), 0, LW(2), 0)
            MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
            CALL(evt_mapobj_rotate, 0, STRING("door_2"), 0, LW(2), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_rotate, 0, STRING("door_1"), 0, 0, 0)
        CALL(evt_mapobj_rotate, 0, STRING("door_2"), 0, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("トゲノコＡ"), STRING("W_3"))
        LOOP(0)
            IF_INT_NOT_EQUAL(GW(1), 1)
                BREAK_LOOP
            END_IF
            SET_FLOAT_VALUE(LW(2), GW(0))
            ADD_FLOAT(LW(2), FLOAT(25.0f))
            CALL(get_togenoko_pos, FLOAT(-750.0f), FLOAT(-50.0f), FLOAT(-750.0f), FLOAT(-7.5f), LW(2), LW(3), LW(4))
            CALL(evt_npc_set_position, STRING("トゲノコＡ"), LW(3), 0, LW(4))
            IF_INT_LESS_OR_EQUAL(LW(2), 90)
                CALL(evt_npc_set_ry, STRING("トゲノコＡ"), 270)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_stop_anim, STRING("トゲノコＡ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("トゲノコＢ"), STRING("W_3"))
        LOOP(0)
            IF_INT_NOT_EQUAL(GW(1), 1)
                BREAK_LOOP
            END_IF
            SET_FLOAT_VALUE(LW(5), GW(0))
            ADD_FLOAT(LW(5), FLOAT(25.0f))
            MULTIPLY_FLOAT(LW(5), FLOAT(-1.0f))
            CALL(get_togenoko_pos, FLOAT(-750.0f), FLOAT(50.0f), FLOAT(-750.0f), FLOAT(7.5f), LW(5), LW(6), LW(7))
            CALL(evt_npc_set_position, STRING("トゲノコＢ"), LW(6), 0, LW(7))
            IF_INT_GREATER_OR_EQUAL(LW(5), -90)
                CALL(evt_npc_set_ry, STRING("トゲノコＢ"), 270)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_walk_anim, STRING("トゲノコＢ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_mapobj_get_position, STRING("door"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_CASTLE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    LOOP(0)
        IF_INT_NOT_EQUAL(GW(1), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("トゲノコＡ"), 90)
        CALL(evt_npc_move_position, STRING("トゲノコＡ"), -730, -60, 1000, 0, 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("トゲノコＢ"), 90)
    CALL(evt_npc_move_position, STRING("トゲノコＢ"), -730, 60, 1000, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(kpa_01_touzyo_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STGK_CASTLE1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_set_pos, -810, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_dispdir, 180)
    CALL(evt_cam3d_evt_set, 622, 172, 546, 622, 77, 5, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, -630, 172, 546, -630, 77, 5, 9000, 11)
    WAIT_MS(9000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), -1600)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_MOVE1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(700)
    ADD(LW(0), 100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_MOVE2"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(700)
    ADD(LW(0), 100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_MOVE1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(700)
    ADD(LW(0), 100)
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_MOVE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("トゲノコＡ"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("トゲノコＢ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL_CHILD(PTR(kpa_01_door_open))
    CALL(evt_cam3d_evt_set, -470, 60, 269, -683, 65, 24, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, -430, 60, 234, -644, 65, -11, 2800, 11)
    CALL(evt_mario_mov_pos, -650, 0, 2800)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_LAUGH2_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_000"), 0, STRING("mario"))
    CALL_CHILD(PTR(kpa_01_door_close))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    WAIT_MS(100)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_001"), 0, STRING("トゲノコＡ"))
    SET_VALUE(GSW_STORY_PROGRESS, 60)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(kpa_01_syuppatu_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_get_position, STRING("door"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_CASTLE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_rotate, 0, STRING("door_1"), 0, 100, 0)
    CALL(evt_mapobj_rotate, 0, STRING("door_2"), 0, -100, 0)
    CALL(get_togenoko_pos, FLOAT(-750.0f), FLOAT(-50.0f), FLOAT(-750.0f), FLOAT(-7.5f), 125, LW(0), LW(1))
    CALL(evt_npc_set_position, STRING("トゲノコＡ"), LW(0), 0, LW(1))
    CALL(get_togenoko_pos, FLOAT(-750.0f), FLOAT(50.0f), FLOAT(-750.0f), FLOAT(7.5f), -125, LW(0), LW(1))
    CALL(evt_npc_set_position, STRING("トゲノコＢ"), LW(0), 0, LW(1))
    WAIT_MS(1500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("ババ"), -760, 0, 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    CALL(evt_cam3d_evt_set, -630, 134, 428, -630, 57, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_move_position, STRING("ババ"), -650, 0, 0, FLOAT(60.0f), 0)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_021"), 0, STRING("ババ"))
    SPAWN_EVENT(PTR(kpa_01_door_close))
    CALL(evt_npc_move_position, STRING("ババ"), -480, 0, 0, FLOAT(90.0f), 0)
    CALL(evt_mario_set_pos, 625, 30, 0)
    CALL(evt_mario_set_dir, 270, 0, 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("ハンマーＥ"), 90)
    CALL(evt_npc_set_ry, STRING("ハンマーＤ"), 90)
    CALL(evt_cam3d_evt_set, 555, 180, 517, 555, 90, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ（息切れ）"))
    CALL(evt_npc_set_position, STRING("ババ"), 350, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_npc_move_position, STRING("ババ"), 460, 0, 0, FLOAT(30.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_021_1"), 0, STRING("ババ"))
    CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ（地上）"))
    WAIT_FRAMES(1)
    CALL(evt_npc_set_stay_anim, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 470, 85, 311, 470, 44, -14, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_022"), 0, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 600, 158, 338, 600, 99, 20, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_023"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 555, 180, 517, 555, 90, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_025"), 0, STRING("ババ"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_026"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 470, 85, 311, 470, 44, -14, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_027"), 0, STRING("ババ"))
    CALL(evt_cam3d_evt_set, 600, 158, 338, 600, 99, 20, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_028"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 555, 180, 517, 555, 90, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_029"), 0, STRING("ババ"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_030"), 0, STRING("mario"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_031"), 0, STRING("ババ"))
    CALL(evt_msg_select, 0, STRING("kpa_stg1_032"))
    CALL(evt_msg_continue)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
        CALL(evt_msg_print, 0, STRING("kpa_stg1_033"), 0, STRING("mario"))
        CASE_EQUAL(1)
        CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
        CALL(evt_msg_print, 0, STRING("kpa_stg1_033_1"), 0, STRING("mario"))
    END_SWITCH
    SET_VALUE(LSWF(1), 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        LOOP(0)
            IF_INT_NOT_EQUAL(LSWF(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_anim, STRING("ババ"), STRING("A_2A"))
    END_INLINE_EVENT
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("U_1"), STRING("T_5"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_034"), PTR(kpa_stg1_034_cb), STRING("ババ"))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_1"), STRING("T_1"))
    DELETE_EVENT(LW(0))
    SET_VALUE(LSWF(1), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0048828125f), 800)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(450)
        BEGIN_SIBLING_EVENT
            CALL(evt_npc_set_anim, STRING("ババ"), STRING("D_1"))
            WAIT_MS(500)
            CALL(evt_npc_set_anim, STRING("ババ"), STRING("S_1"))
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(evt_npc_set_anim, STRING("トゲノコＣ"), STRING("O_1"))
            WAIT_MS(500)
            CALL(evt_npc_set_anim, STRING("トゲノコＣ"), STRING("S_1"))
        END_SIBLING_EVENT
        CALL(evt_npc_set_anim, STRING("トゲノコＤ"), STRING("O_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("トゲノコＤ"), STRING("S_1"))
        WAIT_MS(500)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_ANGRY2_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_035"), 0, STRING("mario"))
    SET_VALUE(LSWF(1), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(LSWF(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(150)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0048828125f), 1000)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("kpa_stg1_036"), PTR(kpa_stg1_036_cb), STRING("mario"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_VEXATIOUS2_2"), LW(0), LW(1), LW(2), 0)
    CALL(kpa_chg_pose, STRING("KPA_I_2"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(0), LW(1))
            IF_INT_GREATER_OR_EQUAL(LW(1), 30000)
                BREAK_LOOP
            END_IF
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 150)
            WAIT_MS(150)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ANGRY1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_ANGRY2"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("P_1"), STRING("T_6"))
    CALL(evt_npc_set_anim, STRING("ババ"), STRING("P_1"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 470, 85, 311, 470, 44, -14, 500, 11)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_037"), 0, STRING("ババ"))
    CALL(evt_npc_set_autotalkpose, STRING("ババ"), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_set_position, STRING("カメック"), 300, 0, -5)
    CALL(evt_npc_move_position, STRING("カメック"), 425, -5, 0, FLOAT(90.0f), 0)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_038"), 0, STRING("ババ"))
    CALL(evt_npc_set_ry, STRING("ババ"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_039"), 0, STRING("カメック"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_040"), 0, STRING("ババ"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 555, 180, 501, 555, 90, 0, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("カメック"), 300, 0, 0, FLOAT(90.0f), 0)
        CALL(evt_npc_set_position, STRING("カメック"), 0, -1000, 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_041"), 0, STRING("ババ"))
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    SET_VALUE(GF(0), 1)
    CALL(evt_cam3d_evt_set, 600, 158, 338, 600, 99, 20, 600, 11)
    WAIT_MS(600)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_042"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 555, 180, 517, 555, 90, 0, 800, 11)
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_043"), 0, STRING("ババ"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_4"), STRING("KPA_T_3"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_044"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 430, 180, 517, 430, 90, 0, 1500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_HIT)
        WAIT_MS(900)
        CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(2), 30)
        CALL(evt_npc_jump_position_nohit, STRING("ババ"), LW(0), LW(1), LW(2), 200, 20)
        WAIT_MS(1100)
        CALL(evt_npc_flag_onoff, 0, STRING("ババ"), NPC_FLAG_NO_HIT)
        CALL(evt_npc_move_position, STRING("ババ"), 325, 0, 0, 40, 0)
        WAIT_MS(100)
        CALL(evt_npc_set_anim, STRING("ババ"), STRING("P_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_ry, STRING("ババ"), 90)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 550, 0, 120)
    CALL(evt_mario_mov_pos2, 515, 0, 120)
    CALL(evt_mario_mov_pos2, 460, 0, 120)
    CALL(evt_mario_mov_pos2, 130, 0, 120)
    CALL(evt_mario_wait_move_end)
    LABEL(100)
    CALL(evt_npc_set_position, STRING("飛クッパ"), 200, 50, 0)
    CALL(evt_npc_set_ry, STRING("飛クッパ"), 90)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_cam3d_evt_set, 553, 177, 501, 553, 90, 0, 1500, 11)
    END_INLINE_EVENT
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 360, 2000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_sub_get_sincos, LW(0), LW(2), LW(3))
        SET_FLOAT_VALUE(GW(0), LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_AIRSHIP_MOVE1"), 200, 50, 0, LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 200, 625, 2000)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    SET_FLOAT_VALUE(LW(2), GW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(20.0f))
    ADD_FLOAT(LW(2), FLOAT(50.0f))
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(0), LW(2), 0)
    CALL(evt_snd_sfx_pos, LW(9), LW(0), LW(2), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    WAIT_MS(250)
    CALL(evt_npc_set_ry, STRING("飛クッパ"), 270)
    WAIT_MS(250)
    SET_VALUE(GW(0), 50)
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_AIRSHIP_MOVE1"), LW(2), LW(3), LW(4), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, 1000, 1500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    SET_FLOAT_VALUE(LW(5), FLOAT(552.0f))
    SUBTRACT_FLOAT(LW(5), LW(2))
    MULTIPLY_FLOAT(LW(5), LW(0))
    ADD_FLOAT(LW(5), LW(2))
    SET_FLOAT_VALUE(LW(6), FLOAT(115.0f))
    SUBTRACT_FLOAT(LW(6), LW(3))
    MULTIPLY_FLOAT(LW(6), LW(0))
    ADD_FLOAT(LW(6), LW(3))
    SET_FLOAT_VALUE(LW(7), FLOAT(355.0f))
    SUBTRACT_FLOAT(LW(7), LW(4))
    MULTIPLY_FLOAT(LW(7), LW(0))
    ADD_FLOAT(LW(7), LW(4))
    CALL(evt_npc_set_position, STRING("飛クッパ"), LW(5), LW(6), LW(7))
    CALL(evt_snd_sfx_pos, LW(9), LW(5), LW(6), LW(7))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_paper_entry, STRING("OFF_d_yabu_hantai"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_yabu_hantai"))
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S"), STRING("ofs"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("ofs"), 1, 122, 36, 364, 288)
    WAIT_FRAMES(1)
    CALL(evt_img_set_position, STRING("img"), 0, -252)
    CALL(evt_img_set_virtual_point, STRING("img"), 0, 0, -70)
    BEGIN_INLINE_EVENT
        WAIT_MS(1700)
        CALL(evt_img_onoff, STRING("img"), 1)
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
        CALL(evt_snd_sfxon, STRING("SFX_OFF_KUPPA_WALL_BREAK1"), 0)
        BEGIN_SIBLING_EVENT
            CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.049804688f), 500)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(evt_cam_shake, 6, FLOAT(0.0f), FLOAT(0.049804688f), 500)
        END_SIBLING_EVENT
        WAIT_MS(200)
        BEGIN_SIBLING_EVENT
            CALL(evt_npc_set_anim, STRING("トゲノコＣ"), STRING("O_1"))
            WAIT_MS(500)
            CALL(evt_npc_set_anim, STRING("トゲノコＣ"), STRING("S_1"))
        END_SIBLING_EVENT
        CALL(evt_npc_set_anim, STRING("トゲノコＤ"), STRING("O_1"))
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("トゲノコＤ"), STRING("S_1"))
        WAIT_MS(500)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(2), FLOAT(0.0f))
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
    BEGIN_INLINE_EVENT
        WAIT_MS(1300)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 1000, 100, 1000)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        SET_FLOAT_VALUE(LW(2), FLOAT(37.5f))
        SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
        SUBTRACT_FLOAT(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(2), LW(3))
        SET_FLOAT_VALUE(GW(1), LW(2))
        CALL(evt_npc_set_scale, STRING("飛クッパ"), LW(0), LW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 430, 3300)
        LABEL(4)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_sub_get_sincos, LW(0), LW(2), LW(3))
        SET_FLOAT_VALUE(GW(0), LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(4)
        END_IF
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, 1000, 2000)
        LABEL(5)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        SET_FLOAT_VALUE(GW(2), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(5)
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("飛クッパ"), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_AIRSHIP_MOVE1"), LW(2), LW(3), LW(4), LW(15))
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        SET_FLOAT_VALUE(LW(0), GW(2))
        SET_FLOAT_VALUE(LW(5), FLOAT(550.0f))
        SUBTRACT_FLOAT(LW(5), LW(2))
        MULTIPLY_FLOAT(LW(5), LW(0))
        ADD_FLOAT(LW(5), LW(2))
        SET_FLOAT_VALUE(LW(6), FLOAT(-90.0f))
        SUBTRACT_FLOAT(LW(6), LW(4))
        MULTIPLY_FLOAT(LW(6), LW(0))
        ADD_FLOAT(LW(6), LW(4))
        SET_FLOAT_VALUE(LW(3), GW(0))
        MULTIPLY_FLOAT(LW(3), FLOAT(90.0f))
        ADD_FLOAT(LW(3), FLOAT(115.0f))
        ADD_FLOAT(LW(3), GW(1))
        CALL(evt_npc_set_position, STRING("飛クッパ"), LW(5), LW(3), LW(6))
        CALL(evt_snd_sfx_pos, LW(15), LW(5), LW(3), LW(6))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_npc_flag_onoff, 1, STRING("飛クッパ"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ"))
    CALL(evt_sub_get_sincos, 132, LW(2), LW(3))
    MULTIPLY_FLOAT(LW(3), FLOAT(60.0f))
    ADD_FLOAT(LW(3), FLOAT(90.0f))
    CALL(evt_npc_set_position, STRING("ババ"), 300, LW(3), 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 90)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 180, 540, 2500)
        LABEL(6)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_sub_get_sincos, LW(0), LW(2), LW(3))
        SET_FLOAT_VALUE(GW(0), LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(6)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_kamek_kemuri2, STRING("ババ"), 2500, 1)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 300, 500, 2500)
    LABEL(7)
    CALL(evt_sub_intpl_msec_get_value)
    SET_FLOAT_VALUE(LW(2), GW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(20.0f))
    ADD_FLOAT(LW(2), LW(3))
    CALL(evt_npc_set_position, STRING("ババ"), LW(0), LW(2), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(7)
    END_IF
    SET_VALUE(GF(0), 1)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_045"), 0, STRING("ババ"))
    CALL(evt_sub_get_sincos, 132, LW(0), LW(1))
    SET_FLOAT_VALUE(GW(0), LW(1))
    SET_FLOAT_VALUE(GW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(2), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        WAIT_MS(700)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 1000, 300, 800)
        LABEL(8)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        SET_FLOAT_VALUE(LW(2), FLOAT(20.0f))
        SET_FLOAT_VALUE(LW(3), FLOAT(1.0f))
        SUBTRACT_FLOAT(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(2), LW(3))
        SET_FLOAT_VALUE(GW(2), LW(2))
        CALL(evt_npc_set_scale, STRING("ババ"), LW(0), LW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(8)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 132, 360, 2000)
        LABEL(9)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_sub_get_sincos, LW(0), LW(2), LW(3))
        SET_FLOAT_VALUE(GW(0), LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(9)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -90, 1700)
        LABEL(10)
        CALL(evt_sub_intpl_msec_get_value)
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(10)
        END_IF
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_kamek_kemuri2, STRING("ババ"), 1500, 1)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 500, 560, 2000)
    LABEL(11)
    CALL(evt_sub_intpl_msec_get_value)
    SET_FLOAT_VALUE(LW(2), GW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(60.0f))
    ADD_FLOAT(LW(2), FLOAT(90.0f))
    ADD_FLOAT(LW(2), GW(2))
    SET_FLOAT_VALUE(LW(3), GW(1))
    CALL(evt_npc_set_position, STRING("ババ"), LW(0), LW(2), LW(3))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(11)
    END_IF
    SET_VALUE(GF(0), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_NO_DRAW)
    WAIT_MS(500)
    CALL(evt_bero_mapchange, STRING("kpa_00"), STRING(""))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(kpa_01_pos_check)
    CALL(evt_seq_wait, 2)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 625)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL_CHILD(PTR(kpa_01_syuppatu_evt))
    RETURN
EVT_END()

EVT_BEGIN(douzou_1_touch_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_014"), 0, STRING("mario"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(douzou_2_touch_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_014_1"), 0, STRING("mario"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(douzou_3_touch_evt)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LSWF(1), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(LSWF(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_bgmon, 513, STRING("BGM_FF_GET_ITEM1"))
    END_INLINE_EVENT
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_014_2"), PTR(kpa_stg1_014_2_cb), STRING("mario"))
    WAIT_MS(300)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(togenoko_A_talk_evt)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_002"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(togenoko_B_talk_evt)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_004"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(togenoko_C_init_evt)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(hammer_AB_init_evt)
    CALL_CHILD(PTR(npc_init_evt))
    IF_INT_EQUAL(GSWF(4775), 0)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_TALK_TURN)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(hammer_AB_talk_evt)
    IF_INT_EQUAL(GSWF(4775), 0)
        CALL(evt_msg_print, 0, STRING("kpa_stg1_006"), 0, STRING("ハンマーＡ"))
        CALL(evt_msg_print, 0, STRING("kpa_stg1_007"), 0, STRING("ハンマーＢ"))
        SET_VALUE(LSWF(1), 0)
        BEGIN_INLINE_EVENT
            LOOP(0)
                IF_INT_NOT_EQUAL(LSWF(1), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_set_dir_to_target, STRING("ハンマーＡ"), STRING("mario"))
            WAIT_FRAMES(1)
            CALL(evt_npc_wait_pera, STRING("ハンマーＡ"))
            CALL(evt_npc_set_anim, STRING("ハンマーＡ"), STRING("BRO_O_1"))
            WAIT_MS(500)
            CALL(evt_npc_set_anim, STRING("ハンマーＡ"), STRING("BRO_S_1"))
        END_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("kpa_stg1_008"), PTR(kpa_stg1_008_cb), STRING("ハンマーＡ"))
        CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
        CALL(evt_msg_print, 0, STRING("kpa_stg1_009"), 0, STRING("mario"))
        SET_VALUE(GSWF(4775), 1)
        CALL(evt_npc_flag_onoff, 0, STRING("ハンマーＡ"), NPC_FLAG_NO_TALK_TURN)
        CALL(evt_npc_flag_onoff, 0, STRING("ハンマーＢ"), NPC_FLAG_NO_TALK_TURN)
        ELSE
        CALL(evt_set_dir_to_target, STRING("ハンマーＡ"), STRING("mario"))
        CALL(evt_set_dir_to_target, STRING("ハンマーＢ"), STRING("mario"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("ハンマーＡ"))
        CALL(evt_npc_wait_pera, STRING("ハンマーＢ"))
        CALL(evt_msg_print, 0, STRING("kpa_stg1_010"), 0, STRING("ハンマーＡ"))
        CALL(evt_msg_print, 0, STRING("kpa_stg1_011"), 0, STRING("ハンマーＢ"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(hammer_C_talk_evt)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_012"), 0, STRING("me"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_7"), STRING("KPA_T_7"))
    CALL(evt_msg_print, 0, STRING("kpa_stg1_013"), 0, STRING("mario"))
    RETURN
EVT_END()

EVT_BEGIN(hammer_D_talk_evt)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_015"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(hammer_E_init_evt)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(hammer_E_talk_evt)
    CALL(evt_msg_print, 0, STRING("kpa_stg1_016"), 0, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(baba_init_evt)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_tribe, STRING("me"), STRING("カメックババ（地上）"))
    RETURN
EVT_END()

EVT_BEGIN(kamek_init_evt)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(hkoopa_init_evt)
    CALL_CHILD(PTR(npc_init_evt))
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"トゲノコＡ"*/"\x83\x67\x83\x51\x83\x6D\x83\x52\x82\x60", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        togenoko_A_talk_evt, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲノコＢ"*/"\x83\x67\x83\x51\x83\x6D\x83\x52\x82\x61", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        togenoko_B_talk_evt, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲノコＣ"*/"\x83\x67\x83\x51\x83\x6D\x83\x52\x82\x62", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"トゲノコＤ"*/"\x83\x67\x83\x51\x83\x6D\x83\x52\x82\x63", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ハンマーＡ"*/"\x83\x6E\x83\x93\x83\x7D\x81\x5B\x82\x60", // name
        0, 0, // flags..reactionFlags
        hammer_AB_init_evt, // initEvent
        NULL, // regularEvent
        hammer_AB_talk_evt, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ハンマーＢ"*/"\x83\x6E\x83\x93\x83\x7D\x81\x5B\x82\x61", // name
        0, 0, // flags..reactionFlags
        hammer_AB_init_evt, // initEvent
        NULL, // regularEvent
        hammer_AB_talk_evt, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ハンマーＣ"*/"\x83\x6E\x83\x93\x83\x7D\x81\x5B\x82\x62", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        hammer_C_talk_evt, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ハンマーＤ"*/"\x83\x6E\x83\x93\x83\x7D\x81\x5B\x82\x63", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        hammer_D_talk_evt, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ハンマーＥ"*/"\x83\x6E\x83\x93\x83\x7D\x81\x5B\x82\x64", // name
        0, 0, // flags..reactionFlags
        hammer_E_init_evt, // initEvent
        NULL, // regularEvent
        hammer_E_talk_evt, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ババ"*/"\x83\x6F\x83\x6F", // name
        0, 0, // flags..reactionFlags
        baba_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カメック"*/"\x83\x4A\x83\x81\x83\x62\x83\x4E", // name
        0, 0, // flags..reactionFlags
        kamek_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"飛クッパ"*/"\x94\xF2\x83\x4E\x83\x62\x83\x70", // name
        NPC_FLAG_NO_HIT, // flags
        0, // reactionFlags
        hkoopa_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(shan_init)
    SET_VALUE(LW(0), 1)
    LOOP(12)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, LW(1), LW(2), LW(3), LW(4))
        SUBTRACT_FLOAT(LW(3), FLOAT(2.5f))
        CALL(evt_eff, STRING(""), STRING("fire"), 6, LW(2), LW(3), LW(4), FLOAT(0.2998047f), 1000, 0, 0, 0, 0, 0, 0)
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(kpa_01_init_evt)
    CALL(evt_mario_set_mode, 2)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_run_case_evt, 9, 1, STRING("a_zou_1"), 0, PTR(douzou_1_touch_evt), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("a_zou_2"), 0, PTR(douzou_2_touch_evt), 0)
    CALL(evt_run_case_evt, 9, 1, STRING("a_zou_3"), 0, PTR(douzou_3_touch_evt), 0)
    CALL_CHILD(shan_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(59)
        SPAWN_EVENT(kpa_01_touzyo_evt)
        SPAWN_EVENT(kpa_01_pos_check)
        CASE_EQUAL(60)
        CALL(evt_snd_bgmoff, 30720)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("player"))
            CALL(evt_mario_set_pos, -700, 0, 0)
            CALL(evt_mario_set_dir, 90, 0, 0)
            SPAWN_EVENT(kpa_01_pos_check)
        ELSE
            CALL(evt_mario_set_pos, 625, 30, 0)
            CALL(evt_mario_set_dir, 90, 0, 0)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                WAIT_MS(300)
                SPAWN_EVENT(kpa_01_syuppatu_evt)
            END_INLINE_EVENT
        END_IF
    END_SWITCH
    RETURN
EVT_END_PAD()

static USER_FUNC(make_name) {
    EvtWord* args = event->args;
    static char name[8];

    sprintf(name, "hi_%d", evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}

void kpa_stg1_008_cb(s32 type, smartEntry* smart) {
    MsgWinData* info = smart->address;

    switch (type) {
        case 3:
            if (info->curPage == 1) {
                evtSetValue(NULL, LSWF(1), 1);
            }
            break;
    }
}

void kpa_stg1_014_2_cb(s32 type, smartEntry* smart) {
    MsgWinData* info = smart->address;

    switch (type) {
        case 3:
            if (info->curPage == 2) {
                evtSetValue(NULL, LSWF(1), 1);
            }
            break;
    }
}

void kpa_stg1_036_cb(s32 type, smartEntry* smart) {
    MsgWinData* info = smart->address;

    switch (type) {
        case 3:
            if (info->curPage == 2) {
                evtSetValue(NULL, LSWF(1), 1);
            }
            break;
    }
}

void kpa_stg1_034_cb(s32 type, smartEntry* smart) {
    MsgWinData* info = smart->address;

    switch (type) {
        case 5:
            if (info->nextEntry > 117) {
                evtSetValue(NULL, LSWF(1), 1);
            }
            break;
    }
}

USER_FUNC(get_togenoko_pos) {
    EvtWord* args = event->args;
    f32 cx = evtGetFloat(event, args[0]);
    f32 cz = evtGetFloat(event, args[1]);
    f32 x = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 angle = evtGetFloat(event, args[4]);
    Mtx mtx;
    Vec pos;

    PSMTXRotRad(mtx, 'Y', 0.017453292f * angle);
    pos.x = x - cx;
    pos.y = 0.0f;
    pos.z = z - cz;
    PSMTXMultVec(mtx, &pos, &pos);
    pos.x += cx;
    pos.z += cz;
    evtSetFloat(event, args[5], pos.x);
    evtSetFloat(event, args[6], pos.z);
    return EVT_RETURN_DONE;
}


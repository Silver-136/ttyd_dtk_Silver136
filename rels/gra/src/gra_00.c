#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cam.h"
#include "evt/evt_img.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "driver/camdrv.h"
#include "driver/msgdrv.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "dolphin/mtx.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "gra.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#ifdef VERSION_G8ME01
#endif


static USER_FUNC(cam_r);
USER_FUNC(camera_yure);
USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_win_nameent_name);
USER_FUNC(evt_win_nameent_off);
USER_FUNC(evt_win_nameent_on);
USER_FUNC(evt_win_nameent_wait);
static USER_FUNC(zoom);
void msg_callback(s32 type, void* param);

EvtWord evt_yabure_on[56];
EvtWord evt_yabure_off[5];
EvtWord evt_nameent[109];
EvtWord name_seikai[682];
EvtWord evt_machibuse[VERSION_PICK(666, 694)];
static BeroEntry bero_entry_data[3];
static NpcSetupInfo npcEnt[2];
static TreeAccess tree_tbl[2];
EvtWord evt_majin_key[5];
EvtWord gra_00_init_evt[74];

EVT_BEGIN(evt_yabure_on)
    CALL(evt_paper_entry, STRING("OFF_d_yabu_small"))
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF4_LANPEL_APPEAR1"), -145, 270, -25, 0)
    BEGIN_INLINE_EVENT
        CALL(camera_yure, 20)
    END_INLINE_EVENT
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_yabu_small"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 100, 100, 202, 160)
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_set_position, STRING("img"), -460, -261)
    CALL(evt_img_set_virtual_point, STRING("img"), 0, 0, -78)
    CALL(evt_img_onoff, STRING("img"), 1)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    RETURN
EVT_END()

EVT_BEGIN(evt_yabure_off)
    CALL(evt_img_release, STRING("img"))
    RETURN
EVT_END()

EVT_BEGIN(evt_nameent)
    CALL(evt_paper_entry, STRING("OFF_d_roll"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_roll"))
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("Z_1"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 1)
    WAIT_FRAMES(1)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_win_nameent_on, LW(0))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF4_NAME_ENTRY1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_win_nameent_wait)
    CALL(evt_win_nameent_name, LW(0))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF4_NAME_ENTRY2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_win_nameent_off)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_roll"))
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

EVT_BEGIN(name_seikai)
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_NM_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_O_2"))
    CALL(evt_npc_set_autotalkpose, STRING("にせマリオ"), STRING("EM_Z_3"), STRING("EM_T_4"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(10), LW(11), LW(12))
    ADD(LW(2), LW(12))
    DIVIDE(LW(2), 2)
    ADD(LW(2), 542)
    SET_VALUE(LW(15), LW(2))
    SET_VALUE(LW(14), LW(12))
    SUBTRACT(LW(14), LW(15))
    MULTIPLY_FLOAT(LW(14), FLOAT(0.5996094f))
    SET_VALUE(LW(13), LW(2))
    ADD(LW(13), LW(14))
    CALL(evt_cam3d_evt_set, LW(10), 73, LW(13), LW(10), 30, LW(12), 500, 11)
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_gra_12"), 0, STRING("にせマリオ"))
    SET_VALUE(LW(15), LW(2))
    SET_VALUE(LW(14), LW(12))
    SUBTRACT(LW(14), LW(15))
    MULTIPLY_FLOAT(LW(14), FLOAT(0.7890625f))
    SET_VALUE(LW(13), LW(2))
    ADD(LW(13), LW(14))
    CALL(evt_cam3d_evt_set, LW(10), 53, LW(13), LW(10), 30, LW(12), 500, 11)
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_gra_13"), 0, STRING("にせマリオ"))
    SET_VALUE(LW(15), LW(2))
    SET_VALUE(LW(14), LW(12))
    SUBTRACT(LW(14), LW(15))
    MULTIPLY_FLOAT(LW(14), FLOAT(0.8798828f))
    SET_VALUE(LW(13), LW(2))
    ADD(LW(13), LW(14))
    CALL(evt_cam3d_evt_set, LW(10), 43, LW(13), LW(10), 30, LW(12), 500, 11)
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_gra_14"), 0, STRING("にせマリオ"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg4_gra_15"), 0, STRING("にせマリオ"))
        CALL(evt_msg_continue)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_LANPEL_SHOCK1"), LW(0), LW(1), LW(2), LW(10))
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 8)
    IF_INT_LESS(LW(0), LW(10))
        MULTIPLY(LW(6), -1)
    END_IF
    LOOP(0)
        ADD(LW(5), LW(6))
        IF_INT_LESS(LW(5), 0)
            ADD(LW(5), 360)
        END_IF
        IF_INT_GREATER(LW(5), 360)
            SUBTRACT(LW(5), 360)
        END_IF
        CALL(cam_r, LW(5))
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(cam_r, 0)
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(3), LW(4), LW(5))
    ADD(LW(0), LW(3))
    DIVIDE(LW(0), 2)
    ADD(LW(2), LW(5))
    DIVIDE(LW(2), 2)
    SET_VALUE(LW(3), LW(2))
    ADD(LW(3), 542)
    CALL(evt_cam3d_evt_set, LW(0), 82, LW(3), LW(0), 30, LW(2), 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_J_2"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_J_2"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        LOOP(0)
            LOOP(0)
                CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
                CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
                CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(10))
                IF_INT_LESS(LW(10), 20)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MARIO_ROUND1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_set_pose, STRING("M_D_1"))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 3600, 2000)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_mario_set_dispdir, LW(0))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            IF_INT_EQUAL(GF(0), 1)
                SET_VALUE(GF(1), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        LOOP(0)
            LOOP(0)
                CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
                CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
                CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(10))
                IF_INT_LESS(LW(10), 20)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_gra_vivian_pose_set, 1, STRING("PTR_S_4"))
            CALL(evt_gra_vivian_pose_set, 7, STRING("PTR_T_5"))
            CALL(evt_party_set_pose, 0, STRING("PTR_D_1"))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 3600, 2000)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_party_set_dispdir, 0, LW(0))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            CALL(evt_party_set_pose, 0, STRING("PTR_S_1"))
            IF_INT_EQUAL(GF(0), 1)
                SET_VALUE(GF(2), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_party_run, 0)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    SET_VALUE(GF(2), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 0, STRING("にせマリオ"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("にせマリオ"), 2)
    CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_R_3"))
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_NM_SURPRISED1"), LW(0), LW(1), LW(2), 0)
        ELSE
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_NM_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
    END_IF
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("にせマリオ"), -300, LW(2), 0, FLOAT(300.0f), 0)
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_NM_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
        ELSE
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_NM_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    END_IF
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("にせマリオ"), 200, LW(2), 0, FLOAT(300.0f), 0)
    CALL(evt_npc_set_position, STRING("にせマリオ"), 0, -1000, 0)
    SET_VALUE(GF(0), 1)
    CALL(zoom, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_party_get_pos, 0, LW(13), LW(14), LW(15))
    ADD(LW(10), LW(13))
    DIVIDE(LW(10), 2)
    CALL(evt_cam3d_evt_set, LW(10), LW(1), LW(2), LW(10), LW(4), LW(5), 800, 11)
    LOOP(0)
        IF_INT_EQUAL(GF(1), 1)
            IF_INT_EQUAL(GF(2), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_set_dir, 0, 90, 300)
    WAIT_MS(500)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_gra_vivian_pose_set, 1, STRING("PTR_S_1"))
    CALL(evt_gra_vivian_pose_set, 7, STRING("PTR_T_1"))
    CALL(evt_msg_print_party, STRING("stg4_gra_16"))
    CALL(evt_party_run, 0)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 500, 11)
    RETURN
EVT_END()

EVT_BEGIN(evt_machibuse)
    SET_VALUE(LF(0), 0)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(0), 30)
            IF_INT_GREATER_OR_EQUAL(LW(0), -126)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 120)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_normalize)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(0), -48)
            CALL(evt_mario_set_dir, 90, 200, 0)
            CALL(evt_party_set_dir, 0, 90, 200)
            ELSE
            CALL(evt_mario_set_dir, 270, 200, 0)
            CALL(evt_party_set_dir, 0, 270, 200)
        END_IF
        CALL(evt_mario_set_pose, STRING("M_N_3"))
    END_INLINE_EVENT
    CALL(evt_npc_set_position, STRING("にせマリオ"), -125, 240, -25)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 206)
        CALL(evt_msg_print, 0, STRING("stg4_gra_05"), 0, STRING("にせマリオ"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg4_gra_00"), 0, STRING("にせマリオ"))
    END_IF
    CALL(evt_npc_set_position, STRING("にせマリオ"), 0, -1000, 0)
    CALL(evt_cam3d_evt_set, -88, 352, 517, -88, 257, -25, 800, 11)
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_snd_bgmon, 512, STRING("BGM_FF_ENV_RPL_APPEAR1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL_CHILD(PTR(evt_yabure_on))
    IF_INT_LESS(LW(0), -48)
        SET_VALUE(LF(0), 1)
        ELSE
        SET_VALUE(LF(0), 0)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mario_set_pos, -100, 0, 0)
        ELSE
        CALL(evt_mario_set_pos, 0, 0, 0)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_EQUAL(LF(0), 1)
        ADD(LW(0), -40)
        ELSE
        ADD(LW(0), 40)
    END_IF
    CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_EQUAL(LF(0), 1)
        ADD(LW(0), 75)
        ELSE
        ADD(LW(0), -75)
    END_IF
    WAIT_MS(200)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_position, STRING("にせマリオ"), -145, 270, -25)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_LANPEL_M_FALL1"), -145, 270, -25, LW(10))
        CALL(evt_npc_jump_position_nohit, STRING("にせマリオ"), LW(0), LW(1), LW(2), 700, FLOAT(130.0f))
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_set_dir_to_target, STRING("にせマリオ"), STRING("mario"))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_NM_HAPPY1"), -145, 270, -25, 0)
    CALL_CHILD(PTR(evt_yabure_off))
    CALL(evt_gra_vivian_pose_set, 1, STRING("PTR_S_4"))
    CALL(evt_gra_vivian_pose_set, 2, STRING("PTR_S_4"))
    CALL(evt_gra_vivian_pose_set, 3, STRING("PTR_S_4"))
    CALL(evt_gra_vivian_pose_set, 7, STRING("PTR_T_5"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(3), LW(4), LW(5))
    ADD(LW(0), LW(3))
    DIVIDE(LW(0), 2)
    ADD(LW(2), LW(5))
    DIVIDE(LW(2), 2)
    SET_VALUE(LW(3), LW(2))
    ADD(LW(3), 542)
    CALL(evt_cam3d_evt_set, LW(0), 137, LW(3), LW(0), 30, LW(2), 0, 11)
    WAIT_FRAMES(1)
    IF_INT_LESS(GSW_STORY_PROGRESS, 203)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg4_gra_01"), 0, STRING("にせマリオ"))
        CALL(zoom, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 900, 11)
        WAIT_MS(900)
        CALL(evt_msg_print, 0, STRING("stg4_gra_02"), 0, STRING("にせマリオ"))
        ELSE
        CALL(zoom, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 1200, 11)
        WAIT_MS(200)
    END_IF
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    WAIT_MS(1000)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(203)
        CALL(evt_msg_print, 0, STRING("stg4_gra_03"), 0, STRING("にせマリオ"))
        CASE_LESS(206)
        CALL(evt_msg_print, 0, STRING("stg4_gra_09"), 0, STRING("にせマリオ"))
        CASE_LESS(207)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 15)
        ADD(LW(2), 30)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(70.0f))
        WAIT_MS(500)
        CALL(evt_party_stop, 0)
        WAIT_MS(300)
        CALL(evt_msg_print_party, STRING("stg4_gra_06"))
        CALL(evt_msg_print, 0, STRING("stg4_gra_07"), 0, STRING("にせマリオ"))
        CALL(evt_party_run, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_gra_09"), 0, STRING("にせマリオ"))
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(209)
        SET_VALUE(LW(0), 2)
        CALL_CHILD(PTR(evt_nameent))
        WAIT_MS(300)
        CASE_DEFAULT
        SET_VALUE(LW(0), 3)
        CALL_CHILD(PTR(evt_nameent))
        WAIT_MS(300)
#ifdef VERSION_G8ME01
        CALL(evt_sub_get_language, LW(1))
        SWITCH(LW(1))
            CASE_EQUAL(0)
            SET_VALUE(LW(2), STRING("ランペル"))
            CASE_OR_EQUAL(1)
            CASE_OR_EQUAL(6)
            SET_VALUE(LW(2), STRING("Doopliss"))
            CASE_END
            CASE_EQUAL(5)
            SET_VALUE(LW(2), STRING("Rampel"))
            CASE_DEFAULT
            SET_VALUE(LW(2), STRING("Rumpel"))
        END_SWITCH
        IF_STR_EQUAL(LW(0), LW(2))
#else
        IF_STR_EQUAL(LW(0), STRING("ランペル"))
#endif
            CALL_CHILD(PTR(name_seikai))
            SET_VALUE(GSW_STORY_PROGRESS, 210)
            CALL(evt_pouch_remove_item, ITEM_THE_LETTER_P, LW(0))
            CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_GRA1"))
            CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
            CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
            CALL(evt_gra_vivian_pose_set, 1, STRING("PTR_S_1"))
            CALL(evt_gra_vivian_pose_set, 2, STRING("PTR_W_1"))
            CALL(evt_gra_vivian_pose_set, 3, STRING("PTR_R_1"))
            CALL(evt_gra_vivian_pose_set, 7, STRING("PTR_T_1"))
            CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_party_run, 0)
            RETURN
        END_IF
    END_SWITCH
    CALL(evt_msg_print, 0, STRING("stg4_gra_04"), 0, STRING("にせマリオ"))
    CALL(evt_npc_set_battle_info, STRING("にせマリオ"), 2)
    CALL(evt_npc_battle_start, STRING("にせマリオ"))
    CALL(evt_npc_wait_battle_end)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(203)
        SET_VALUE(GSW_STORY_PROGRESS, 203)
        CASE_LESS(206)
        CASE_LESS(207)
        SET_VALUE(GSW_STORY_PROGRESS, 207)
    END_SWITCH
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_gra_vivian_pose_set, 1, STRING("PTR_S_4"))
    CALL(evt_gra_vivian_pose_set, 7, STRING("PTR_T_5"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("にせマリオ"), LW(10), LW(11), LW(12))
    CALL(evt_mario_set_pos, LW(10), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("にせマリオ"), LW(0), LW(11), LW(12))
    CALL(evt_set_dir_to_target, STRING("にせマリオ"), STRING("mario"))
    IF_INT_LESS(LW(10), LW(0))
        CALL(evt_cam3d_evt_set, -225, 134, 534, -225, 39, -8, 0, 11)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_NO_HIT)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_mov_pos2, -385, 0, FLOAT(200.0f))
        END_INLINE_EVENT
        WAIT_MS(1100)
        CALL(evt_cam3d_evt_set, -88, 137, 517, -88, 42, -25, 800, 11)
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("stg4_gra_11"), 0, STRING("にせマリオ"))
        CALL(evt_bero_mapchange, STRING("usu_01"), STRING("e_bero"))
        ELSE
        CALL(evt_cam3d_evt_set, 186, 149, 534, 186, 54, -8, 0, 11)
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_NO_HIT)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_mov_pos2, 385, 0, FLOAT(200.0f))
        END_INLINE_EVENT
        WAIT_MS(1250)
        CALL(evt_cam3d_evt_set, -20, 137, 517, -20, 42, -25, 800, 11)
        WAIT_MS(850)
        CALL(evt_msg_print, 0, STRING("stg4_gra_11"), 0, STRING("にせマリオ"))
        CALL(evt_bero_mapchange, STRING("gra_01"), STRING("w_bero1"))
    END_IF
    RETURN
EVT_END()

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
        "gra_01", // nextMapName
        "w_bero1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
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
        "usu_01", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static NpcSetupInfo npcEnt[] = {
    {
        /*"にせマリオ"*/"\x82\xC9\x82\xB9\x83\x7D\x83\x8A\x83\x49", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static TreeAccess tree_tbl[] = {
    {2, "A_ki_1", "S_gra00_ki", NULL, 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(evt_majin_key)
    SET_VALUE(GSWF(2091), 1)
    RETURN
EVT_END()

EVT_BEGIN(gra_00_init_evt)
    SET_VALUE(GSWF(256), 1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_GRA1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_npc_setup, PTR(npcEnt))
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    CALL(evt_mobj_brick, STRING("blk01"), 250, 15, 100, 0, 0, 0, LSWF(1))
    CALL(evt_mobj_brick, STRING("blk02"), -265, 60, -85, 0, 3, 0, GSWF(2089))
    IF_INT_EQUAL(GSWF(2091), 0)
        CALL(evt_item_entry, STRING("majin_key"), ITEM_BLACK_KEY_0x23, -258, 0, -85, ITEM_TYPE_STILL, -1, PTR(evt_majin_key))
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(202, 209)
        SPAWN_EVENT(PTR(evt_machibuse))
    END_SWITCH
    RETURN
EVT_END()

static USER_FUNC(cam_r) {
    f32 roll = evtGetFloat(event, event->args[0]);
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 s;

    if (!roll) {
        cam->rollAngle = 0.0f;
        cam->viewOffset.z = 0.0f;
    } else {
        cam->rollAngle = roll;
        s = sin(gp->retraceCount / 10.0f);
        cam->viewOffset.z = -__fabsf(15.0f * s);
        cam->cameraPos.y = 97.0f;
        cam->cameraAt.y = 45.0f;
    }
    return EVT_RETURN_DONE;
}

void msg_callback(s32 type, void* param) {
    EventEntry* ev = (*(MsgWinData**)param)->event;

    switch (type) {
        case 0:
            evtSetValue(ev, GW(0), 0);
            evtSetValue(ev, GW(1), 0);
            ev->lwData[15] = 0;
            break;
        case 5:
            if (ev->lwData[15] == 3) {
                marioKeyOff();
            }
            ev->lwData[15]++;
            break;
        case 1:
            evtSetValue(ev, GW(1), 1);
            break;
        case 3:
            evtSetValue(ev, GW(0), 1);
            break;
    }
}

static USER_FUNC(zoom) {
    EvtWord* args = event->args;
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    Vec v;

    PSVECSubtract(&cam->cameraPos, &cam->cameraAt, &v);
    PSVECScale(&v, &v, 0.6f);
    PSVECAdd(&cam->cameraAt, &v, &v);
    evtSetFloat(event, args[0], v.x);
    evtSetFloat(event, args[1], v.y);
    evtSetFloat(event, args[2], v.z);
    evtSetFloat(event, args[3], cam->cameraAt.x);
    evtSetFloat(event, args[4], cam->cameraAt.y);
    evtSetFloat(event, args[5], cam->cameraAt.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(camera_yure) {
    s32 frames = evtGetValue(event, event->args[0]);
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    Vec v;
    f32 amp;
    s32 left;

    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[1] = frames;
    }
    // shake three times, fading out
    amp = sin(18.849556f * event->userdata[0] / event->userdata[1]);
    left = event->userdata[1] - event->userdata[0];
    amp = left * (0.1f * amp) / event->userdata[1];
    PSVECSubtract(&cam->cameraAt, &cam->cameraPos, &v);
    PSVECScale(&v, &v, amp);
    cam->viewOffset.x = v.x;
    cam->viewOffset.y = v.y;
    cam->viewOffset.z = v.z;
    cam = camGetPtr(CAMERA_3D_IMAGE);
    cam->viewOffset.x = v.x;
    cam->viewOffset.y = v.y;
    cam->viewOffset.z = v.z;
    event->userdata[0]++;
    if (event->userdata[0] > event->userdata[1]) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

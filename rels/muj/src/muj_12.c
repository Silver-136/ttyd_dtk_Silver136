#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "evt/evt_fade.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "mario/mario.h"

#include "driver/itemdrv.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "dolphin/os.h"
#include "driver/fadedrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "muj.h"
#include "driver/seqdrv.h"

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_snd_bgm_freq);
static USER_FUNC(evtflash);
USER_FUNC(muj_item_move_3d_position);
static f32 accdec_type(s32 step, s32 total, f32 start, f32 end);

static EvtWord back_dark[27];
static EvtWord door_custom[53];
static EvtWord bero_custom[135];
static EvtWord w_close[12];
static EvtWord w_open[12];
static BeroEntry bero_entry_data[2];
EvtWord korutesu_yure[86];
EvtWord korutesu_flush[24];
EvtWord korutesu_flush2[36];
EvtWord korutesu_rotate[28];
EvtWord korutesu_small[56];
EvtWord korutesu_small2[56];
EvtWord korutesu_rotate2[28];
EvtWord korutesu_large[56];
EvtWord korutesu_blur[43];
static EvtWord starstone_fuwafuwa[53];
static EvtWord korutesu_event[627];
EvtWord korutesu_event_after[140];
EvtWord kosho_event[517];
EvtWord coin_effect[151];
EvtWord muj_12_init_evt[60];

static const GXColor evtflash_col1 = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor evtflash_col2 = {0xFF, 0xFF, 0xFF, 0xFF};

static EVT_BEGIN(back_dark)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 255, 0, LW(10))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(door_custom)
    CALL(evt_bero_get_info_anime, LW(1), LW(10), LW(11), LW(12), LW(13))
    IF_INT_NOT_EQUAL(LW(12), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(180.0f))
        CALL_CHILD(LW(12))
        CALL(evt_bero_mario_go)
        WAIT_MS(1500)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        SET_VALUE(LW(9), LW(12))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
            IF_INT_EQUAL(LW(10), 0)
                SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
                CALL_CHILD(LW(9))
                BREAK_LOOP
                ELSE
                CALL_CHILD(LW(9))
            END_IF
        END_LOOP
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(bero_custom)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(252)
        SPAWN_EVENT(PTR(door_custom))
        CALL(evt_mario_normalize)
        CALL(evt_party_stop, 0)
        CALL(evt_party_stop, 1)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_set_pos, -218, 0, 0)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_mario_mov_pos2, -100, 0, FLOAT(120.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_pos, 0, -230, 0, 20)
            CALL(evt_party_set_dir, 0, 90, 0)
            CALL(evt_party_move_pos2, 0, -130, 20, FLOAT(120.0f))
        END_INLINE_EVENT
        WAIT_MS(2000)
        SET_VALUE(LW(0), 1)
        RETURN
        CASE_EQUAL(257)
        SPAWN_EVENT(PTR(door_custom))
        CALL(evt_mario_normalize)
        CALL(evt_party_stop, 0)
        CALL(evt_party_stop, 1)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_set_pos, -218, 0, 0)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_mario_mov_pos2, -90, 0, FLOAT(120.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_pos, 0, -240, 0, -25)
            CALL(evt_party_set_dir, 0, 90, 0)
            CALL(evt_party_move_pos2, 0, -110, -25, FLOAT(120.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_pos, 1, -220, 0, 25)
            CALL(evt_party_set_dir, 1, 90, 0)
            CALL(evt_party_move_pos2, 1, -120, 25, FLOAT(120.0f))
        END_INLINE_EVENT
        WAIT_MS(2000)
        SET_VALUE(LW(0), 1)
        RETURN
    END_SWITCH
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.61035156f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.61035156f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_custom, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_11", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(korutesu_yure)
    CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SHIP_SHAKE2"), LW(0), LW(1), LW(2), LW(10))
    LOOP(0)
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 300)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam_shake, 4, FLOAT(0.0087890625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.0078125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.0068359375f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.005859375f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.0029296875f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.001953125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, 4, FLOAT(0.0009765625f), FLOAT(0.0f), 300)
    CALL(evt_snd_sfxoff, LW(10))
    RETURN
EVT_END()

EVT_BEGIN(korutesu_flush)
    LOOP(0)
        CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
        CALL(evtflash)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_sub_random, 500, LW(10))
        WAIT_MS(800)
        WAIT_MS(LW(10))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(korutesu_flush2)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    CALL(evt_sub_random, 500, LW(10))
    WAIT_MS(800)
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    CALL(evt_sub_random, 1000, LW(10))
    WAIT_MS(1600)
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    RETURN
EVT_END()

EVT_BEGIN(korutesu_rotate)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 1800, 2700)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_rotate, STRING("コルテス"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(korutesu_small)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_npc_glide_position, STRING("コルテス"), LW(0), LW(1), LW(2), 2700, 0, 0, 11, 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 1000, 100, 2700)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(LW(2), LW(0))
        DIVIDE_FLOAT(LW(2), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("コルテス"), LW(2), LW(2), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(korutesu_small2)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_npc_glide_position, STRING("コルテス"), LW(0), LW(1), LW(2), 2700, 0, 0, 11, 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 1000, 0, 2700)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(LW(2), LW(0))
        DIVIDE_FLOAT(LW(2), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("コルテス"), LW(2), LW(2), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(korutesu_rotate2)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 1800, 0, 1300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_rotate, STRING("コルテス"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(korutesu_large)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
        ADD(LW(1), -30)
        CALL(evt_npc_glide_position, STRING("コルテス"), LW(0), LW(1), LW(2), 1300, 0, 0, 11, 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 100, 1000, 1300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(LW(2), LW(0))
        DIVIDE_FLOAT(LW(2), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("コルテス"), LW(2), LW(2), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(korutesu_blur)
    LOOP(0)
        BEGIN_INLINE_EVENT
            CALL(evt_env_blur_on, 1, 200)
        END_INLINE_EVENT
        CALL(evt_cam3d_evt_set, -203, 72, 359, 0, 94, 75, 100, 11)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, -175, 75, 320, 0, 94, 75, 100, 11)
        WAIT_MS(100)
        IF_INT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GF(1), 0)
    RETURN
EVT_END()

static EVT_BEGIN(starstone_fuwafuwa)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 500, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 500, 0, 500)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(korutesu_event)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("コルテス"), 46, 35, 25)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_NO_VIEW_CORRECTION | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_ry, STRING("コルテス"), 0)
    CALL(evt_npc_pera_onoff, STRING("コルテス"), 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_21"), 0, 0)
    CALL(evt_cam3d_evt_set, -300, 60, 444, -25, 90, 57, 750, 11)
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    SET_VALUE(GF(0), 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG5_KORTESS1"))
    SPAWN_EVENT(PTR(korutesu_flush))
    CALL_CHILD(PTR(korutesu_apper))
    SET_VALUE(GF(0), 1)
    CALL(evt_msg_print, 0, STRING("stg5_dou_22"), 0, STRING("コルテス"))
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 1, 500)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -175, 75, 320, 0, 94, 75, 500, 11)
    WAIT_MS(1000)
    CALL(evt_npc_set_autotalkpose, STRING("コルテス"), STRING("KRT_S_1"), STRING("KRT_T_3"))
    WAIT_FRAMES(1)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    SPAWN_EVENT(PTR(korutesu_blur))
    CALL(evt_snd_bgmon, 513, STRING("BGM_FF_ENV_SURPRISED3"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_23"), 0, STRING("コルテス"))
    CALL(evt_npc_set_autotalkpose, STRING("コルテス"), STRING("KRT_S_1"), STRING("KRT_T_2"))
    SET_VALUE(GF(0), 0)
    WAIT_WHILE(GF(1))
    CALL(evt_cam3d_evt_set, -40, 90, 542, -40, 80, -8, 500, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_24"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -40, 94, 791, -40, 80, -8, 500, 11)
    WAIT_MS(250)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_npc_set_battle_info, STRING("コルテス"), 4)
    CALL(evt_npc_battle_start, STRING("コルテス"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_set, -80, 30, 368, 44, 90, -8, 0, 11)
    CALL(evt_mario_set_pos, -65, 0, 0)
    CALL(evt_party_set_pos, 0, -100, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    WAIT_MS(1000)
    SET_VALUE(GF(0), 0)
    SPAWN_EVENT(PTR(korutesu_yure))
    SPAWN_EVENT(PTR(korutesu_flush2))
    CALL(evt_npc_set_autotalkpose, STRING("コルテス"), STRING("KRT_S_1"), STRING("KRT_T_3"))
    WAIT_FRAMES(1)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_25"), 0, STRING("コルテス"))
    CALL(evt_npc_set_autotalkpose, STRING("コルテス"), STRING("KRT_S_1"), STRING("KRT_T_2"))
    SET_VALUE(GF(0), 1)
    WAIT_MS(2000)
    CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_KOR_HEAD_MOVE1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 32000)
        LOOP(60)
            SUBTRACT(LW(0), 516)
            CALL(evt_snd_bgm_freq, 0, LW(0))
            WAIT_MS(50)
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(korutesu_rotate))
    SPAWN_EVENT(PTR(korutesu_small))
    WAIT_MS(5000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_26"), 0, STRING("コルテス"))
    CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_KOR_HEAD_MOVE2"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(30)
            ADD(LW(0), 1066)
            CALL(evt_snd_bgm_freq, 0, LW(0))
            WAIT_MS(50)
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(korutesu_rotate2))
    SPAWN_EVENT(PTR(korutesu_large))
    WAIT_MS(1500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_27"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -20, 125, 505, -20, 80, 7, 500, 11)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_28"), 0, STRING("コルテス"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_28a"), 0, STRING("コルテス"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 40, 125, 505, 40, 80, 7, 700, 11)
    CALL(evt_npc_glide_position, STRING("コルテス"), 86, 51, 112, 0, FLOAT(120.0f), 0, 11, 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_28b"), 0, STRING("コルテス"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_28c"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -20, 125, 505, -20, 80, 7, 700, 11)
    CALL(evt_npc_glide_position, STRING("コルテス"), 46, 35, 35, 0, FLOAT(120.0f), 0, 11, 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_29"), 0, STRING("コルテス"))
    WAIT_MS(1000)
    SET_VALUE(LW(10), STRING("stone"))
    CALL(evt_item_flag_on, LW(10), 2)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 50)
    CALL(evt_item_get_position, LW(10), LW(11), LW(12), LW(13))
    ADD(LW(12), 20)
    CALL(evt_item_move_3d_position, LW(10), LW(11), LW(12), LW(13), 500, 0)
    CALL(evt_cam3d_evt_set, LW(0), 125, 505, LW(0), 80, 7, 1500, 11)
    SPAWN_EVENT_GET_ID(PTR(starstone_fuwafuwa), LW(9))
    CALL(muj_item_move_3d_position, LW(10), LW(0), LW(1), LW(2), 3000, 0, GW(1))
    DELETE_EVENT(LW(9))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
    SET_VALUE(LW(0), 5)
    SET_VALUE(LW(1), LW(10))
    SET_VALUE(GSW_STORY_PROGRESS, 253)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL_CHILD(PTR(evt_sub_starstone))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ9"))
    RETURN
EVT_END()

EVT_BEGIN(korutesu_event_after)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -65, 0, 0)
    CALL(evt_party_set_pos, 0, -100, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_position, STRING("コルテス"), 46, 35, 25)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_NO_VIEW_CORRECTION | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, STRING("コルテス"), 0)
    CALL(evt_npc_pera_onoff, STRING("コルテス"), 0)
    CALL(evt_cam3d_evt_set, -20, 125, 505, -20, 80, 7, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabe_r"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_end_wait)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 70, 800)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_rotate, 1, STRING("S_t_doa02"), 0, LW(2), 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_t_doa03"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_30"), 0, STRING("コルテス"))
    WAIT_MS(1000)
    DELETE_EVENT(GW(0))
    SET_VALUE(LW(10), 600)
    CALL_CHILD(PTR(back_dark))
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_39"), 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 253)
    CALL(evt_bero_mapchange, STRING("muj_10"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(kosho_event)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("コルテス"), 46, 35, 25)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_NO_VIEW_CORRECTION | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_DRAW)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_cam3d_evt_set, 20, 157, 337, 20, 90, -6, 500, 11)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_STG5_KORTESS1"))
    CALL_CHILD(PTR(korutesu_apper))
    CALL(evt_msg_print, 0, STRING("stg5_dou_31"), 0, STRING("コルテス"))
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_MARCO_SHAKE1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_party_set_pose, 1, STRING("O_1"))
    CALL(evt_cam3d_evt_set, -25, 125, 505, -25, 80, 7, 0, 11)
    WAIT_MS(500)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    ADD(LW(2), 10)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg5_dou_32"), 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_33"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, 11)
    WAIT_MS(500)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg5_dou_34"), 0, 0)
    CALL(evt_cam3d_evt_set, -25, 125, 505, -25, 80, 7, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_35"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, 11)
    CALL(evt_party_set_pose, 1, STRING("S_1"))
    CALL(evt_snd_sfxoff, LW(9))
    WAIT_MS(500)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_eff_fukidashi, 2, STRING(""), 0, 0, 0, LW(0), LW(1), LW(2), 50, 0, 60)
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_NPC_BIKKURI_MARK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36"), 0, STRING("extparty"))
    CALL(evt_item_entry, STRING("jewel"), ITEM_SKULL_GEM, -115, 0, 30, ITEM_TYPE_STILL, -1, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 20, 157, 337, 20, 90, -6, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_01"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -25, 125, 505, -25, 80, 7, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_02"), 0, STRING("extparty"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_03"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_04"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_set, 20, 157, 337, 20, 90, -6, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_05"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_06"), 0, STRING("extparty"))
    SET_VALUE(GF(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_item_get_position, STRING("jewel"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_JUWEL_MOVE1"), LW(0), LW(1), LW(2), LW(3))
        LOOP(0)
            CALL(evt_item_get_position, STRING("jewel"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(3), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(GF(2), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_snd_sfxoff, LW(3))
    END_INLINE_EVENT
    CALL(evt_item_move_3d_position, STRING("jewel"), -115, 70, 30, 1000, PTR(accdec_type))
    CALL(evt_cam3d_evt_set, 20, 157, 337, 20, 90, -6, 3000, 11)
    BEGIN_INLINE_EVENT
        WAIT_MS(2500)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 255, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_item_set_alpha, STRING("jewel"), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT_GET_ID(PTR(starstone_fuwafuwa), LW(10))
    CALL(muj_item_move_3d_position, STRING("jewel"), 40, 70, 30, 3000, 0, GW(1))
    SET_VALUE(GF(2), 1)
    DELETE_EVENT(LW(10))
    WAIT_MS(500)
    CALL(evt_item_delete, STRING("jewel"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_37"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_38"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_set, -45, 160, 534, -45, 55, -6, 0, 11)
    WAIT_MS(500)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    WAIT_MS(300)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
    CALL(evt_snd_bgmon_f, PSND_CONTINUE | PSND_FADEIN, 0, 2000)
    CALL_CHILD(PTR(korutesu_vanish))
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("コルテス"), 0, -1000, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, 258)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(coin_effect)
    LOOP(0)
        CALL(evt_sub_random, 10, LW(10))
        SWITCH(LW(10))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), -44)
            SET_VALUE(LW(1), 20)
            SET_VALUE(LW(2), 138)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), -8)
            SET_VALUE(LW(1), 30)
            SET_VALUE(LW(2), 146)
            CASE_EQUAL(2)
            SET_VALUE(LW(0), 26)
            SET_VALUE(LW(1), 20)
            SET_VALUE(LW(2), 143)
            CASE_EQUAL(3)
            SET_VALUE(LW(0), 78)
            SET_VALUE(LW(1), 28)
            SET_VALUE(LW(2), 147)
            CASE_EQUAL(4)
            SET_VALUE(LW(0), 91)
            SET_VALUE(LW(1), 30)
            SET_VALUE(LW(2), -62)
            CASE_EQUAL(5)
            SET_VALUE(LW(0), 49)
            SET_VALUE(LW(1), 30)
            SET_VALUE(LW(2), -82)
            CASE_EQUAL(6)
            SET_VALUE(LW(0), -28)
            SET_VALUE(LW(1), 40)
            SET_VALUE(LW(2), -82)
            CASE_EQUAL(7)
            SET_VALUE(LW(0), -67)
            SET_VALUE(LW(1), 15)
            SET_VALUE(LW(2), -50)
            CASE_EQUAL(8)
            SET_VALUE(LW(0), -11)
            SET_VALUE(LW(1), 35)
            SET_VALUE(LW(2), 98)
            CASE_EQUAL(9)
            SET_VALUE(LW(0), 25)
            SET_VALUE(LW(1), 30)
            SET_VALUE(LW(2), 133)
            CASE_EQUAL(10)
            SET_VALUE(LW(0), 82)
            SET_VALUE(LW(1), 10)
            SET_VALUE(LW(2), 148)
        END_SWITCH
        CALL(evt_eff64, STRING(""), STRING("hagetaka_flush_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(muj_12_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS(GSW_STORY_PROGRESS, 253)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG5_GHOST1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ9"))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 252)
        SPAWN_EVENT(PTR(korutesu_event))
        CALL(evt_item_entry, STRING("stone"), ITEM_SAPPHIRE_STAR, 63, 38, 138, ITEM_TYPE_STILL, -1, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 253)
        SPAWN_EVENT(PTR(korutesu_event_after))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 257)
        SPAWN_EVENT(PTR(kosho_event))
    END_IF
    SPAWN_EVENT_GET_ID(PTR(coin_effect), GW(0))
    RETURN
EVT_END()

static f32 accdec_type(s32 step, s32 total, f32 start, f32 end) {
    f32 cur;
    f32 speed;
    f32 sum;
    s32 accel = total * 20 / 100;
    s32 decel = total * 80 / 100;
    s32 i;

    speed = 0.0f;
    sum = 0.0f;

    for (i = 0; i < total; i++) {
        if (i <= accel) {
            speed += 1.0f;
        } else if (i >= decel) {
            speed -= 1.0f;
        }
        sum += speed;
        if (i == step) {
            cur = sum;
        }
    }
    return start + cur * (end - start) / sum;
}

USER_FUNC(muj_item_move_3d_position) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    f32 x = evtGetValue(event, args[1]);
    f32 y = evtGetValue(event, args[2]);
    f32 z = evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    s32 mode = evtGetValue(event, args[5]);
    f32 yOffset = evtGetFloat(event, args[6]);
    ItemEntry* item = itemNameToPtr(name);
    s32 elapsed;

    if (isFirstCall) {
        if (item == NULL) {
            return EVT_RETURN_DONE;
        }
        event->savedTime = gp->renderTime;
        event->userdataf[1] = item->position.x;
        event->userdataf[2] = item->position.y;
        event->userdata3f = item->position.z;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    if (elapsed < frames) {
        // the elapsed time is kept in the (dead) isFirstCall parameter
        isFirstCall = elapsed;
        item->position.x = intplGetValue(mode, event->userdataf[1], x, isFirstCall, frames);
        item->position.y = intplGetValue(mode, event->userdataf[2], y, isFirstCall, frames);
        item->position.z = intplGetValue(mode, event->userdata3f, z, isFirstCall, frames);
        item->position.y += yOffset;
        return EVT_RETURN_BLOCK;
    }
    item->position.x = x;
    item->position.y = y;
    item->position.z = z;
    return EVT_RETURN_DONE;
}

static USER_FUNC(evtflash) {
    if (isFirstCall) {
        event->userdata[2] = 0;
    }
    switch (event->userdata[2]) {
        case 0:
            fadeEntry(FADE_OUT_BLACK, 20, evtflash_col1);
            event->userdata[2]++;
        case 1:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[0] = 50;
            event->savedTime = (s32)gp->renderTime;
            event->userdata[2]++;
            if (event->userdata[0] == 0) {
                event->userdata[2]++;
            }
            return EVT_RETURN_BLOCK;
        case 2:
            if (OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) < (u32)event->userdata[0]) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[2]++;
            return EVT_RETURN_BLOCK;
        case 3:
            fadeEntry(FADE_IN_BLACK, 80, evtflash_col2);
            event->userdata[2]++;
        case 4:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

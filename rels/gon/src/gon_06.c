#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "pmario_sound.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_lecture.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"

#include "driver/mobjdrv.h"
#include "driver/animdrv.h"
#include "evt/evt_bero.h"
#include "gon.h"
#include "mario/mario_pouch.h"
#include "driver/seqdrv.h"

static USER_FUNC(bgm_start_wait);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_win_item_select);
static void majin_callback(s32 type);

static EvtWord door01_close[12];
static EvtWord door01_open[12];
static EvtWord door02_close[12];
static EvtWord door02_open[12];
static BeroEntry bero_entry_data[3];
EvtWord evt_ori_open[130];
EvtWord tsuritenjo_event[143];
static s32 key_tbl[2];
static EvtWord evt_majin[409];
static EvtWord evt_majin2[241];
EvtWord gon_06_init_evt[340];

static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door02_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door02_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_05", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_07", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door02_close, // enterAnimeArg
        door02_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_ori_open)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 2)
    CALL(evt_hit_bind_mapobj, STRING("a_ori_03"), STRING("ori02"))
    CALL(evt_hit_bind_mapobj, STRING("a_ori_03_atari"), STRING("ori02"))
    CALL(evt_mapobj_get_position, STRING("ori02"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_DOOR_GRID_OPEN1"), LW(10), LW(11), LW(12), 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("ori02"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_ori_03"))
        CALL(evt_hit_bind_update, STRING("a_ori_03_atari"))
        IF_INT_GREATER_OR_EQUAL(LW(0), 150)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(LW(1), -10)
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, STRING("ori02"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_DOOR_GRID_SHUT1"), LW(10), LW(11), LW(12), 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("ori02"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_ori_03"))
        CALL(evt_hit_bind_update, STRING("a_ori_03_atari"))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_get_position, STRING("ori02"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_DOOR_GRID_DOWN1"), LW(10), LW(11), LW(12), 0)
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_rumble_onoff, 0, 0)
        WAIT_MS(200)
        CALL(evt_sub_rumble_onoff, 1, 0)
    END_SIBLING_EVENT
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 500)
    RETURN
EVT_END()

EVT_BEGIN(tsuritenjo_event)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_set_pos, 620, 0, 0)
    CALL(evt_party_set_pos, 0, 670, 0, 0)
    CALL(evt_cam_road_reset)
    CALL(evt_bero_mario_go_init)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 1)
    SPAWN_EVENT(PTR(bero_play_enter_anime))
    CALL(evt_bero_mario_go_wait)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 570, 0, FLOAT(150.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 520, 0, FLOAT(150.0f))
    WAIT_MS(300)
    CALL(evt_npc_change_fbat_mode, 7)
    CALL(evt_npc_stop_for_event)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg1_gon_18_00"))
    WAIT_MS(300)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG1_GON1"), 3000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_13_01_50_SECONDS_TO_ESCAPE)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_npc_start_for_event)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static void majin_callback(s32 type) {
    MapObjectEntry* mobj = mobjNameToPtr("box");

    if (type == 0) {
        animPoseSetAnim(mobj->poseId, "A_2", TRUE);
    } else if (type == 5) {
        if (animPoseGetLoopTimes(mobj->poseId) >= 1.0f) {
            animPoseSetAnim(mobj->poseId, "A_2", TRUE);
        }
    } else if (type == 2) {
        animPoseSetAnim(mobj->poseId, "S_1", TRUE);
    }
}


static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

static s32 key_tbl[] = {
    ITEM_BLACK_KEY_0x22, -1,
};

static EVT_BEGIN(evt_majin)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_adjust_dir)
    IF_INT_EQUAL(GSWF_GON_06_BLACK_CHEST_FIRST_TALK_DONE, 0)
        CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            IF_INT_LESS_OR_EQUAL(LW(3), LW(0))
                CALL(evt_mario_set_dir, 90, 200, 0)
                ELSE
                CALL(evt_mario_set_dir, 270, 200, 0)
            END_IF
        END_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_12_01"), 0, 0)
        CALL(evt_cam3d_evt_set, 380, 125, 418, 380, 55, 24, 650, EASING_SINE_IN_OUT)
        CALL(evt_mobj_get_position, STRING("box"), LW(3), LW(4), LW(5))
        ADD(LW(4), 30)
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLACKBOX_SWING1"), LW(3), LW(4), LW(5), LW(15))
        CALL(evt_mobj_set_anim, STRING("box"), STRING("A_3"))
        CALL(evt_mobj_wait_animation_end, STRING("box"))
        CALL(evt_snd_sfxoff, LW(15))
        WAIT_MS(300)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_12_02"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_12_03"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_12_04"), PTR(majin_callback), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
        END_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_12_05"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_12_06"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_Z"))
        WAIT_MS(1600)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg1_gon_12_07"), PTR(majin_callback), 0)
        CALL(evt_mobj_exec_cancel, STRING("box"))
        SET_VALUE(GSWF_GON_06_BLACK_CHEST_FIRST_TALK_DONE, 1)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        ELSE
        CALL(evt_pouch_check_item, ITEM_BLACK_KEY_0x22, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
            ADD(LW(1), 30)
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg1_gon_12_08"), PTR(majin_callback), 0)
            CALL(evt_mobj_exec_cancel, STRING("box"))
            ELSE
            CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
            ADD(LW(1), 30)
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg1_gon_12_09"), PTR(majin_callback), 0)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
                IF_INT_LESS_OR_EQUAL(LW(3), LW(0))
                    CALL(evt_mario_set_dir, 90, 200, 0)
                    ELSE
                    CALL(evt_mario_set_dir, 270, 200, 0)
                END_IF
            END_INLINE_EVENT
            CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
            IF_INT_EQUAL(LW(0), -1)
                CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
                CALL(evt_msg_print, 0, STRING("stg1_gon_12_10"), PTR(majin_callback), 0)
                CALL(evt_mobj_exec_cancel, STRING("box"))
                ELSE
                BEGIN_INLINE_EVENT
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    ADD(LW(0), 35)
                    ADD(LW(2), 35)
                    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(80.0f))
                END_INLINE_EVENT
                CALL(evt_pouch_remove_item, ITEM_BLACK_KEY_0x22, LW(0))
                CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                ADD(LW(1), 50)
                CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
                CALL(evt_msg_print, 0, STRING("stg1_gon_12_11"), PTR(majin_callback), 0)
                CALL(evt_mario_set_pose, STRING("M_I_N"))
                CALL(evt_cam3d_evt_set, 380, 110, 330, 380, 57, 23, 500, EASING_SINE_IN_OUT)
            END_IF
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_majin2)
    CALL(evt_npc_change_fbat_mode, 7)
    CALL(evt_npc_stop_for_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_pouch_add_item, ITEM_PAPER_CURSE, LW(0))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_PERAMAJIN1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(bgm_start_wait)
    WAIT_MS(1000)
    CALL(evt_paper_entry, STRING("OFF_d_kusya"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_kusya"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_set_virtual_point, STRING("img"), 380, 0, -50)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_img_clear_virtual_point, STRING("img"))
    END_INLINE_EVENT
    CALL(evt_img_onoff, STRING("img"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_APPEAR1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_COME1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_mario_set_camid, 7)
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(lect_cam_save)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    WAIT_FRAMES(1)
    CALL_CHILD(PTR(slit_majin_lecture_init))
    CALL_CHILD(PTR(slit_majin_lecture))
    LOOP(6)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_PERAMAJIN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mario_init_camid)
    CALL(lect_cam_load, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_GOOUT1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
    CALL(evt_img_wait_animend, STRING("img"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_GOOUT2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_kusya"))
    SET_VALUE(GSW_STORY_PROGRESS, STG1_13_01_01_PAPER_MAGE_2)
    SET_VALUE(GSWF_GON_06_BLACK_CHEST_OPENED, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_npc_start_for_event)
    RETURN
EVT_END()

EVT_BEGIN(gon_06_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_13_A_SUSPICIOUS_ROOM)
        SPAWN_EVENT(PTR(bero_case_entry))
        SPAWN_EVENT(PTR(tsuritenjo_event))
        ELSE
        CALL_CHILD(PTR(evt_bero_info_run))
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
        CALL(evt_snd_set_rev_mode, 2)
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    END_IF
    CALL(evt_npc_entry, STRING("\"カロン00\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン00\""), -467, 0, 76)
    CALL(evt_npc_set_rotate, STRING("\"カロン00\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン00\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン00\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン00\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン00\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン00\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン00\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン01\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン01\""), -361, 0, -83)
    CALL(evt_npc_set_rotate, STRING("\"カロン01\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン01\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン01\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン01\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン01\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン01\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン01\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン02\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン02\""), -222, 0, 52)
    CALL(evt_npc_set_rotate, STRING("\"カロン02\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン02\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン02\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン02\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン02\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン02\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン02\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン03\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン03\""), 8, 0, -115)
    CALL(evt_npc_set_rotate, STRING("\"カロン03\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン03\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン03\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン03\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン03\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン03\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン03\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン04\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン04\""), 343, 0, -49)
    CALL(evt_npc_set_rotate, STRING("\"カロン04\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン04\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン04\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン04\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン04\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン04\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン04\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン05\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン05\""), 541, 0, 96)
    CALL(evt_npc_set_rotate, STRING("\"カロン05\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン05\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン05\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン05\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン05\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン05\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン05\""), 7)
    CALL(evt_mobj_switch_red, 0, STRING("mobj_switch1"), 92, 0, 120, PTR(evt_ori_open), GSWF_GON_06_SWITCH_PRESSED)
    CALL(evt_item_entry, STRING("item01"), ITEM_ATTACK_FX_R, -100, 0, -100, ITEM_TYPE_STILL, GSWF_GON_06_ATTACK_FX_R, 0)
    CALL(evt_sub_rumble_onoff, 1, 0)
    WAIT_FRAMES(1)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_map_playanim, STRING("hokori"), MAP_ANIM_PLAY_LOOP, 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_13_01_01_PAPER_MAGE_2)
        SET_VALUE(GSWF_GON_06_BLACK_CHEST_OPENED, 1)
    END_IF
    CALL(evt_mobj_itembox, STRING("box"), 380, 0, 0, MOBJ_ITEMBOX_BLACK, PTR(evt_majin), PTR(evt_majin2), GSWF_GON_06_BLACK_CHEST_OPENED)
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_GRAVITY)
    RETURN
EVT_END()


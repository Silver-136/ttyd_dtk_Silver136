#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "pmario_sound.h"
#include "evt/evt_cam.h"
#include "evt/evt_img.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_lecture.h"

#include "driver/mobjdrv.h"
#include "driver/animdrv.h"
#include "evt/evt_bero.h"
#include "tik.h"
#include "mario/mario_pouch.h"

static USER_FUNC(bgm_start_wait);
USER_FUNC(evt_img_set_virtual_point);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_mario_adjust_dir);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_pouch_add_item);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_pouch_remove_item);
USER_FUNC(evt_win_item_select);
static void majin_callback(s32 type);

static BeroEntry bero_entry_data[2];
static s32 key_tbl[2];
static EvtWord evt_majin[487];
static EvtWord evt_majin2[237];
EvtWord tik_19_init_evt[52];

static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_04", // nextMapName
        "n_bero_5", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static s32 key_tbl[] = {
    ITEM_BLACK_KEY_0x21, -1,
};

static EVT_BEGIN(evt_majin)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_adjust_dir)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    IF_INT_EQUAL(GSWF(1353), 0)
        CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_majin_01"), 0, 0)
        CALL(evt_cam3d_evt_set, 0, 115, 332, 0, 55, -13, 500, 11)
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            IF_INT_LESS_OR_EQUAL(LW(3), LW(0))
                CALL(evt_mario_set_dir, 90, 200, 0)
                ELSE
                CALL(evt_mario_set_dir, 270, 200, 0)
            END_IF
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_party_move_pos2, 0, -30, 25, FLOAT(120.0f))
            CALL(evt_party_set_dir, 0, 90, -1)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_party_move_pos2, 1, 30, 25, FLOAT(120.0f))
            CALL(evt_party_set_dir, 1, 270, -1)
        END_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLACKBOX_SWING1"), LW(0), LW(1), LW(2), LW(15))
        CALL(evt_mobj_set_anim, STRING("box"), STRING("A_3"))
        CALL(evt_mobj_wait_animation_end, STRING("box"))
        CALL(evt_snd_sfxoff, LW(15))
        WAIT_MS(300)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_majin_02"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_G"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_majin_03"), PTR(majin_callback), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
        END_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_majin_04"), PTR(majin_callback), 0)
        CALL(evt_cam3d_evt_set, 30, 91, 283, 30, 40, -13, 500, 11)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("mac_majin_05"), 0, STRING("extparty"))
        CALL(evt_cam3d_evt_set, 0, 115, 332, 0, 55, -13, 500, 11)
        WAIT_MS(1000)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_majin_06"), PTR(majin_callback), 0)
        WAIT_MS(500)
        CALL(evt_msg_print_party, STRING("mac_majin_07"))
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_THINK1"), LW(0), LW(1), LW(2), 0)
        END_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_I_Z"))
        WAIT_MS(1600)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_majin_08"), PTR(majin_callback), 0)
        CALL(evt_mobj_exec_cancel, STRING("box"))
        SET_VALUE(GSWF(1353), 1)
        CALL(evt_cam3d_evt_off, 500, 11)
        ELSE
        CALL(evt_pouch_check_item, 33, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
            ADD(LW(1), 30)
            CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("mac_majin_09"), PTR(majin_callback), 0)
            CALL(evt_mobj_exec_cancel, STRING("box"))
            ELSE
            CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
            ADD(LW(1), 30)
            CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("mac_majin_10"), PTR(majin_callback), 0)
            CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
            IF_INT_EQUAL(LW(0), -1)
                CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
                CALL(evt_msg_print, 0, STRING("mac_majin_11"), PTR(majin_callback), 0)
                CALL(evt_mobj_exec_cancel, STRING("box"))
                ELSE
                BEGIN_INLINE_EVENT
                    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
                    IF_INT_LESS_OR_EQUAL(LW(3), LW(0))
                        CALL(evt_mario_set_dir, 90, 200, 0)
                        ELSE
                        CALL(evt_mario_set_dir, 270, 200, 0)
                    END_IF
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    SUBTRACT(LW(0), 30)
                    CALL(evt_party_move_pos2, 0, LW(0), 25, FLOAT(80.0f))
                    CALL(evt_party_set_dir, 0, 90, -1)
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                    ADD(LW(0), 30)
                    CALL(evt_party_move_pos2, 1, LW(0), 25, FLOAT(80.0f))
                    CALL(evt_party_set_dir, 1, 270, -1)
                END_INLINE_EVENT
                CALL(evt_pouch_remove_item, 33, LW(0))
                CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
                CALL(evt_msg_print, 0, STRING("mac_majin_12"), PTR(majin_callback), 0)
                CALL(evt_mario_set_pose, STRING("M_I_N"))
                CALL(evt_cam3d_evt_set, 0, 118, 304, 0, 62, -13, 500, 11)
            END_IF
        END_IF
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_majin2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_pouch_add_item, 4, LW(0))
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_PERAMAJIN1"))
    CALL(evt_snd_envoff, 1024)
    CALL(bgm_start_wait)
    WAIT_MS(1000)
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_kusya"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_set_virtual_point, STRING("img"), 0, 10, -100)
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
    CALL_CHILD(PTR(plane_majin_lecture_init))
    CALL_CHILD(PTR(plane_majin_lecture))
    LOOP(6)
        CALL(evt_mario_dispflag_onoff, 1, 1)
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_PERAMAJIN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, 1)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
    CALL(evt_mario_dispflag_onoff, 0, 1)
    CALL(evt_mario_init_camid)
    CALL(lect_cam_load, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_GOOUT1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff_f, 513, 2000)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
    CALL(evt_img_wait_animend, STRING("img"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_GOOUT2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_kusya"))
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print_party, STRING("mac_majin_20"))
    SET_VALUE(GSW_STORY_PROGRESS, 15)
    SET_VALUE(GSWF(1352), 1)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    RETURN
EVT_END()

EVT_BEGIN(tik_19_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hikou_ue"))
    CALL(evt_paper_entry, STRING("OFF_d_kusya"))
    CALL(evt_mobj_itembox, STRING("box"), 0, 10, -50, 3, PTR(evt_majin), PTR(evt_majin2), GSWF(1352))
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), 64)
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 0, 0, 50, STRING("kakoi"), 125, GSWF(5597))
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

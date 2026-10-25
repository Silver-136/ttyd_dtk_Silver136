#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "evt/evt_movefloor.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"

USER_FUNC(evt_bero_overwrite);
USER_FUNC(evt_cam_shift_reset);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_item_set_scale);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_balloon_bikkuri);
USER_FUNC(evt_mario_balloon_off);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mobj_lv_blk);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_pouch_open_mail);
USER_FUNC(evt_pouch_receive_mail);

EvtWord bero_00_open_door[12];
EvtWord bero_00_close_door[12];
static BeroEntry bero_entry_data[8];
static BeroEntry bero_entry_data2[2];
EvtWord tik_07_down_evt[82];
static EvtWord peach_mail[188];
static EvtWord evt_block[8];
static EvtWord mobj_box00[34];
EvtWord tik_07_init_evt[323];

EVT_BEGIN(bero_00_open_door)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(bero_00_close_door)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "n_bero", // hitName
        1, // type
        1, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_08", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        bero_00_close_door, // enterAnimeArg
        bero_00_open_door, // exitAnimeArg
    },
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
        "tik_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan__1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "bom_00", // nextMapName
        "dokan", // nextBeroName
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
        "tik_18", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_5", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_4", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "gor_03", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_5", // hitName
        4098, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static BeroEntry bero_entry_data2[] = {
    {
        "dokan__1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "sys_01", // nextMapName
        "stg_7", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(tik_07_down_evt)
    IF_INT_EQUAL(GSWF(1223), 0)
        RETURN
    END_IF
    SET_VALUE(GSWF(1223), 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 100, 300, 0)
    CALL(evt_party_set_pos, 0, 70, 300, 0)
    CALL(evt_party_set_pos, 1, 85, 300, -30)
    CALL(evt_cam3d_evt_set, FLOAT(100.0f), FLOAT(252.59961f), FLOAT(528.6992f), FLOAT(100.0f), FLOAT(157.09961f), FLOAT(-13.0f), 0, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, 2)
        CALL(evt_mario_cont_onoff, 1)
        CALL(evt_party_cont_onoff, 0, 1)
        CALL(evt_party_cont_onoff, 1, 1)
        CALL(evt_mario_wait_landon)
        WAIT_MS(100)
        CALL(evt_cam_road_reset)
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_MS(400)
        CALL(evt_mario_key_onoff, 1)
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(peach_mail)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_MAIL_RECEPTION1"))
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 40)
    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(60.0f))
    CALL(evt_party_set_dir, 0, 270, -1)
    CALL(evt_msg_print_party, STRING("tik_07_mail4_00"))
    CALL(evt_mario_balloon_bikkuri)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_INT_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION1"), LW(0), LW(1), LW(2), 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_cam3d_evt_set, -218, 209, 233, -218, 157, -63, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_N_9"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    WAIT_MS(500)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_msg_print, 0, STRING("tik_07_mail4_01"), 0, 0)
    CALL(evt_msg_print_party, STRING("tik_07_mail4_02"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 220)
    CALL(evt_pouch_receive_mail, 3)
    CALL(evt_pouch_open_mail, 3)
    RETURN
EVT_END()

static EVT_BEGIN(evt_block)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_hikari"), 1)
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), 282, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tik_07_init_evt)
    CALL_CHILD(PTR(tik_07_down_evt))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 475, 60, 125, 0, 0)
    CALL(evt_mobj_lv_blk, STRING("mobj_blk01"), -300, 100, -50, PTR(evt_block), GSWF(1340), STRING("MOBJ_Lv2BigBlock"))
    IF_INT_EQUAL(GSWF(1340), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_hikari"), 1)
    END_IF
    CALL(evt_mobj_itembox, STRING("mobj_box00"), -550, 0, 100, 0, 0, PTR(mobj_box00), GSWF(1359))
    CALL(evt_mobj_powerupblk, STRING("pblk2"), -180, 160, 0, 0, GSWF(5533), 0)
    CALL(evt_item_entry, STRING("item_00"), 125, 460, 160, -450, 16, GSWF(5594), 0)
    CALL(evt_item_set_scale, STRING("item_00"), FLOAT(0.5996094f))
    CALL(evt_item_entry, STRING("item_01"), 125, 125, 0, -60, 16, GSWF(5595), 0)
    SET_VALUE(LW(0), STRING("ugo_1"))
    SET_VALUE(LW(1), STRING("a_ugo_1"))
    SET_VALUE(LW(2), -20)
    SET_VALUE(LW(3), 20)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 1)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    SET_VALUE(LW(0), STRING("ugo_2"))
    SET_VALUE(LW(1), STRING("a_ugo_2"))
    SET_VALUE(LW(2), 20)
    SET_VALUE(LW(3), -20)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 1)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    SET_VALUE(LW(0), STRING("ugo_3"))
    SET_VALUE(LW(1), STRING("a_ugi_3"))
    SET_VALUE(LW(2), -70)
    SET_VALUE(LW(3), 30)
    SET_VALUE(LW(4), 3000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    SET_VALUE(LW(0), STRING("ugo_4"))
    SET_VALUE(LW(1), STRING("a_ugo_4"))
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), -60)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 1)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    SET_VALUE(GW(0), 5000)
    SET_VALUE(GW(1), 0)
    SET_VALUE(LW(0), STRING("kur_1"))
    SET_VALUE(LW(1), STRING("a_kur_1"))
    SET_VALUE(LW(2), 60)
    SET_VALUE(LW(3), -20)
    SET_VALUE(LW(4), 50)
    SET_VALUE(LW(5), 1)
    SET_RAW_VALUE(LW(6), GW(0))
    SET_RAW_VALUE(LW(7), GW(1))
    SPAWN_EVENT(PTR(ridedown_floor))
    SET_VALUE(LW(0), STRING("kur_2"))
    SET_VALUE(LW(1), STRING("a_kur_2"))
    SET_VALUE(LW(2), 60)
    SET_VALUE(LW(3), -20)
    SET_VALUE(LW(4), 50)
    SET_VALUE(LW(5), 0)
    SET_RAW_VALUE(LW(6), GW(0))
    SET_RAW_VALUE(LW(7), GW(1))
    SPAWN_EVENT(PTR(ridedown_floor))
    IF_INT_LESS(GSW_STORY_PROGRESS, 261)
        CALL(evt_npc_check_delete, STRING("トゲノコ"))
        CALL(evt_npc_check_delete, STRING("ハンマーブロス"))
        ELSE
        CALL(evt_npc_check_delete, STRING("ハンニャー１"))
        CALL(evt_npc_check_delete, STRING("ハンニャー２"))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 349)
        IF_INT_LESS(GSW_STORY_PROGRESS, 352)
            SET_VALUE(LW(0), STRING("dokan__1"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_bero_overwrite, STRING("dokan__1"), PTR(bero_entry_data2))
            SET_VALUE(LW(0), STRING("dokan__1"))
            CALL_CHILD(PTR(bero_case_switch_on))
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 219)
        SPAWN_EVENT(PTR(peach_mail))
    END_IF
    RETURN
EVT_END()


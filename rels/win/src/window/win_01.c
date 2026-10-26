#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "win.h"
#include "data_tables.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "party.h"

USER_FUNC(evt_pouch_open_mail);
USER_FUNC(evt_pouch_receive_mail);

EvtWord evt_dokan1_message[52];
EvtWord evt_dokan1[11];
static BeroEntry bero_entry_data[6];
EvtWord guide_bigtree[336];
static EvtWord peach_mail[257];
EvtWord ki_data_evt_03[20];
static TreeAccess tree_access_data[5];
EvtWord win_01_init_evt[46];

EVT_BEGIN(evt_dokan1_message)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_party_stop, 0)
    WAIT_MS(700)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_party_set_dir, 0, 90, 200)
    CALL(evt_party_move_pos2, 1, 50, 20, FLOAT(60.0f))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg2_win_e04_01"), 0, STRING("extparty"))
    SET_VALUE(GSW_STORY_PROGRESS, 79)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_dokan1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 78)
        SPAWN_EVENT(PTR(evt_dokan1_message))
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
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
        "win_00", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
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
        "win_02", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        evt_dokan1, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        4098, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        4096, // type
        0, // sfxId
        2, // directionId
        248, // centerPosition
        44, // centerPositionY
        -450, // centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(guide_bigtree)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -50)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 1)
    WAIT_MS(300)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_N_3"))
    END_INLINE_EVENT
    CALL(evt_party_move_pos, 1, -15, 60, 2000)
    CALL(evt_msg_print, 0, STRING("stg2_win_e02_00"), 0, STRING("extparty"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, 225, 250, FLOAT(385.8994f), 225, FLOAT(180.59961f), -8, 1000, 11)
    WAIT_MS(3000)
    CALL(evt_party_set_pos, 1, -15, 0, 60)
    CALL(evt_cam3d_evt_set, -50, 100, 378, -50, 45, 58, 0, 11)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg2_win_e02_01"), 0, STRING("extparty"))
    CALL(evt_party_move_pos2, 1, 0, 25, FLOAT(120.0f))
    CALL(evt_party_flg_onoff, 1, 58720256, 1)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    WAIT_MS(1500)
    CALL(evt_party_dispflg_onoff, 1, PARTY_DISP_FLAG_HIDE, 0)
    CALL(evt_party_cont_onoff, 1, 1)
    CALL(evt_mario_clear_party, 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_cam3d_evt_off, 300, 11)
        SET_VALUE(GSW_STORY_PROGRESS, 73)
        CALL(evt_mario_key_onoff, 1)
    END_INLINE_EVENT
    CALL(evt_mapobj_trans, 0, STRING("S_hai_dokan_2"), 0, 0, -4)
    CALL(evt_npc_set_position, STRING("ガイド"), FLOAT(-158.7998f), FLOAT(-30.0f), FLOAT(-500.0f))
    WAIT_MS(200)
    CALL(evt_npc_set_position, STRING("ガイド"), FLOAT(-158.7998f), FLOAT(-0.5595703f), FLOAT(-448.0f))
    CALL(evt_npc_flag_onoff, 1, STRING("ガイド"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_NO_WALL_COLLISION)
    WAIT_MS(300)
    SPAWN_EVENT_GET_ID(PTR(win_puni_walk_se), LW(10))
    CALL(evt_npc_move_position, STRING("ガイド"), -83, -450, 0, 100, 0)
    DELETE_EVENT(LW(10))
    CALL(evt_mapobj_trans, 0, STRING("S_hai_dokan_2"), 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position, STRING("ガイド"), FLOAT(-15.5f), FLOAT(-19.789063f), -450, 400, 100, 40, 1)
    SPAWN_EVENT_GET_ID(PTR(win_puni_walk_se), LW(10))
    CALL(evt_npc_move_position, STRING("ガイド"), -6, -450, 0, 100, 0)
    DELETE_EVENT(LW(10))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position, STRING("ガイド"), FLOAT(40.299805f), FLOAT(-3.7695313f), -450, 400, 110, 25, 1)
    SPAWN_EVENT_GET_ID(PTR(win_puni_walk_se), LW(10))
    CALL(evt_npc_move_position, STRING("ガイド"), 100, -450, 0, 100, 0)
    DELETE_EVENT(LW(10))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ガイド"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_PUNI_JUMP1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position, STRING("ガイド"), FLOAT(170.0f), FLOAT(-3.7597656f), -450, 400, 110, 25, 1)
    SPAWN_EVENT_GET_ID(PTR(win_puni_walk_se), LW(10))
    CALL(evt_npc_move_position, STRING("ガイド"), 244, -450, 0, 100, 0)
    DELETE_EVENT(LW(10))
    CALL(evt_npc_set_position, STRING("ガイド"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ガイド"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY)
    RETURN
EVT_END()

static EVT_BEGIN(peach_mail)
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(2), -200)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    IF_INT_LESS(LW(0), -200)
        SET_VALUE(LF(0), 0)
        GOTO(1)
    END_IF
    IF_INT_GREATER(LW(0), 200)
        SET_VALUE(LF(0), 1)
        GOTO(1)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_MAIL_RECEPTION1"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, 0, STRING("eff0"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg2_mri_00"))
    CALL(evt_mario_balloon_bikkuri)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_INT_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION1"), LW(0), LW(1), LW(2), 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(2))
    ADD(LW(3), 267)
    SET_VALUE(LW(4), LW(2))
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam3d_evt_set, -184, 86, LW(3), -184, 38, LW(4), 1000, 11)
        ELSE
        CALL(evt_cam3d_evt_set, 184, 86, LW(3), 184, 38, LW(4), 1000, 11)
    END_IF
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_N_9"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    WAIT_MS(500)
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mario_set_dir, 90, 300, 0)
        ELSE
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg2_mri_01"), 0, 0)
    CALL(evt_msg_print_party, STRING("stg2_mri_02"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 119)
    CALL(evt_pouch_receive_mail, 1)
    CALL(evt_pouch_open_mail, 1)
    RETURN
EVT_END()

EVT_BEGIN(ki_data_evt_03)
    CALL(evt_item_delete_check, STRING("item_ki03"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ki03"), ITEM_COIN, 312, 106, -122, ITEM_TYPE_FALL_NO_PICKUP, GSWF(2686), 0)
    END_IF
    RETURN
EVT_END()

static TreeAccess tree_access_data[] = {
    {4, "A_tree_01", "S_tree_mki_01", "S_tree_ha01", 0, NULL},
    {4, "A_tree_02", "S_tree_mki_02", "S_tree_ha02", 0, NULL},
    {4, "A_tree_03", "S_tree_mki_03", "S_tree_ha03", 0, ki_data_evt_03},
    {4, "A_tree_04", "S_tree_mki_04", "S_tree_ha04", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(win_01_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_WIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_WIN1"))
    SET_VALUE(LW(0), PTR(tree_access_data))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 72)
        SPAWN_EVENT(PTR(guide_bigtree))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 118)
        SPAWN_EVENT(PTR(peach_mail))
    END_IF
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    CALL(evt_item_entry, STRING("item_00"), ITEM_INN_COUPON, 165, 0, -58, ITEM_TYPE_STILL, GSWF(2687), 0)
    RETURN
EVT_END()


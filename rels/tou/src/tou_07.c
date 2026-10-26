#include "battle/battle_unit.h"
#include "eff/eff_starstone.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"

#include "pmario_sound.h"
#include "mario/mario_pouch.h"
#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "driver/itemdrv.h"
#include "driver/icondrv.h"
#include "driver/effdrv.h"
#include "driver/npcdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "tou.h"
#include "evt/evt_npc.h"
#include "driver/seqdrv.h"

USER_FUNC(evt_mario_flag_onoff);
USER_FUNC(evt_pouch_open_mail);
USER_FUNC(evt_pouch_receive_mail);
USER_FUNC(evt_tou_get_ranking);
USER_FUNC(mario_recovery);
static USER_FUNC(paper_off);
static USER_FUNC(paper_on);
USER_FUNC(tou_07_snd_on);
USER_FUNC(tou_07_snd_off);
USER_FUNC(tou_07_icon_off);
USER_FUNC(tou_07_belt_camid);

static EvtWord n_door_open[12];
static EvtWord n_door_close[12];
static BeroEntry bero_data[3];
static EvtWord init_gans[2];
static EvtWord talk_gans[2];
static NpcSetupInfo npcEnt[4];
static EvtWord evt_sensyu[169];
static EvtWord evt_champion[578];
EvtWord evt_champion_re[308];
static EvtWord evt_dakuto[39];
EvtWord evt_dakuto2[44];
#ifdef VERSION_G8ME01
EvtWord evt_tou_bed[440];
#else
EvtWord evt_tou_bed[432];
#endif
static EvtWord bed_entry[10];
static EvtWord mobj_box00[34];
EvtWord tou_07_init_evt[286];

#ifdef VERSION_G8ME01

USER_FUNC(tou_07_mario_keyoff_check) {
    EvtWord* args = event->args;

    if (marioKeyOffChk() || marioCtrlOffChk()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}
#endif

USER_FUNC(tou_07_snd_on) {
    psndClearFlag(PSND_FLAG_MUTE_SFX);
    return EVT_RETURN_DONE;
}

USER_FUNC(tou_07_snd_off) {
    psndSetFlag(PSND_FLAG_MUTE_SFX);
    return EVT_RETURN_DONE;
}

USER_FUNC(mario_recovery) {
    pouchSetHP(pouchGetMaxHP());
    pouchSetFP(pouchGetMaxFP());
    return EVT_RETURN_DONE;
}

static USER_FUNC(paper_off) {
    marioPaperOff();
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(n_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "n_bero", // hitName
        1, // type
        4, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_04", // nextMapName
        "s_bero_3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_door_close, // enterAnimeArg
        n_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, // type
        1, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_13", // nextMapName
        "e_bero_", // nextBeroName
        0, 0, 0, 0, // enterAnimeType..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(init_gans)
    RETURN
EVT_END()

static EVT_BEGIN(talk_gans)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ガンス"*/"\x83\x4B\x83\x93\x83\x58", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_gans, // initEvent
        NULL, // regularEvent
        talk_gans, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノシコワ"*/"\x83\x4C\x83\x6D\x83\x56\x83\x52\x83\x8F", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_gans, // initEvent
        NULL, // regularEvent
        talk_gans, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_gans, // initEvent
        NULL, // regularEvent
        talk_gans, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(evt_sensyu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, 0, 0, -160)
    CALL(evt_party_set_pos, 0, 0, 0, -190)
    CALL(evt_npc_set_position, STRING("ガンス"), 0, 0, -155)
    CALL(evt_cam3d_evt_set, 0, 145, 534, 0, 50, -8, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_status_onoff, 1, STRING("ガンス"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("ガンス"), 1, -50, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_move_position, STRING("ガンス"), -30, -50, 0, FLOAT(80.0f), 1)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        WAIT_MS(500)
        CALL(evt_party_move_pos2, 0, 0, -70, FLOAT(60.0f))
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 50, -70, FLOAT(60.0f))
        CALL(evt_party_set_dir_npc, 0, STRING("ガンス"))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 0, -50, FLOAT(60.0f))
    CALL(evt_mario_mov_pos2, 35, -50, FLOAT(60.0f))
    CALL(evt_mario_set_dir_npc, STRING("ガンス"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg3_tou_49"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_fade_set_mapchange_type, 0, 49, 0, 50, 0)
    CALL(evt_bero_mapchange, STRING("tou_08"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_champion)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, 0, 0, -160)
    CALL(evt_party_set_pos, 0, 0, 0, -165)
    CALL(evt_npc_set_position, STRING("キノシコワ"), 0, 0, -155)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_ry, STRING("キノシコワ"), 90)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 3, -100, 0, FLOAT(80.0f), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        WAIT_MS(500)
        CALL(evt_party_move_pos2, 0, 3, -110, FLOAT(60.0f))
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 70, -110, FLOAT(60.0f))
        CALL(evt_party_set_dir_npc, 0, STRING("キノシコワ"))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 3, -110, FLOAT(60.0f))
    CALL(evt_mario_mov_pos2, 40, -100, FLOAT(60.0f))
    CALL(evt_mario_set_dir_npc, STRING("キノシコワ"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam3d_evt_set, 20, 90, 189, 20, 60, -9, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_449"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -140, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam3d_evt_set, 63, 78, 139, 63, 55, -9, 1000, 11)
    CALL(evt_party_move_pos2, 0, 85, -100, FLOAT(60.0f))
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_450"))
    WAIT_MS(300)
    WAIT_MS(1000)
    CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2000)
    CALL(evt_msg_print_party, STRING("stg3_tou_450_01"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -75, 68, 270, 59, 78, 2, 1000, 11)
    WAIT_MS(1000)
    LOOP(2)
        CALL(evt_msg_toge, 2, 150, 100, -90)
        CALL(evt_msg_print, 0, STRING("stg3_tou_450_02"), 0, 0)
        WAIT_MS(300)
        CALL(evt_msg_toge, 2, 140, 90, -90)
        CALL(evt_msg_print, 0, STRING("stg3_tou_450_03"), 0, 0)
        WAIT_MS(300)
    END_LOOP
    CALL(evt_cam3d_evt_set, 63, 78, 139, 63, 55, -9, 0, 11)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_450_04"))
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1500)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_MAIL_RECEPTION2"))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg3_tou_450_05"))
    LOOP(0)
        IS_EVENT_RUNNING(LW(11), LW(12))
        IF_INT_EQUAL(LW(12), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
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
    CALL(evt_mario_set_pose, STRING("M_N_9"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MAIL_RECEPTION2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_10"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_450_06"), 0, 0)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("stg3_tou_450_07"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_pouch_receive_mail, 43)
    CALL(evt_pouch_open_mail, 43)
    SET_VALUE(GSW_STORY_PROGRESS, 158)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_champion_re)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, 65, 0, -95)
    CALL(evt_party_set_pos, 0, 95, 0, -105)
    CALL(evt_npc_set_position, STRING("キノシコワ"), 30, 0, -95)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_cam3d_evt_set, 45, 91, 266, 45, 57, -11, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_re_140"), 0, STRING("キノシコワ"))
    CALL(evt_npc_flag_onoff, 1, STRING("キノシコワ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 40, -95, 0, FLOAT(80.0f), 1)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 30, -95, 0, FLOAT(80.0f), 1)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_npc_entry, STRING("ベルト"), STRING("c_bred_h"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 56)
    CALL(evt_npc_set_position, STRING("ベルト"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_anim, STRING("ベルト"), STRING("C_1"))
    ADD(LW(1), 4)
    ADD(LW(2), 15)
    CALL(evt_eff, STRING("belt"), STRING("starstone"), 9, LW(0), LW(1), LW(2), FLOAT(0.75f), 0, 0, 0, 0, 0, 0, 0)
    CALL(tou_07_belt_camid)
    CALL(evt_item_entry, STRING("belt"), ITEM_CHAMPS_BELT, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(tou_07_icon_off)
    CALL(evt_item_get_item, STRING("belt"))
    CALL(evt_item_get_item_end_wait, STRING("belt"))
    CALL(evt_npc_delete, STRING("ベルト"))
    CALL(evt_eff_delete, STRING("belt"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_re_141"), 0, STRING("キノシコワ"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_re_142"), 0, STRING("キノシコワ"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_re_143"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("S_1"))
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -130, 0, FLOAT(80.0f), 1)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), 0, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF(2503), 1)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_dakuto)
    CALL(evt_cam_shake, 4, FLOAT(0.049804688f), FLOAT(0.0f), 500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_DUCT_KOUSHI_HIT1"), 135, 80, -95, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kankikuti"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("win_kabe"), 1, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 159)
        SET_VALUE(GSW_STORY_PROGRESS, 159)
        ELSE
        SET_VALUE(GSWF(2502), 1)
    END_IF
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_dakuto2)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIRDUCT_SHAKE1"), 135, 80, -95, 0)
        WAIT_MS(400)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AIRDUCT_SHAKE1"), 135, 80, -95, 0)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_futa"), 0, 0)
    CALL(evt_map_checkanim, STRING("anm_futa"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_tou_bed)
#ifdef VERSION_G8ME01
    CALL(tou_07_mario_keyoff_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
#endif
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LF(0), 0)
    SET_VALUE(LF(1), 0)
    SET_VALUE(LF(2), 0)
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("tou_07"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("tou_08"))
        SET_VALUE(LF(1), 1)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("tou_10"))
        SET_VALUE(LF(2), 1)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg3_yado_00"), 0, 0)
    CALL(evt_msg_select, 0, STRING("stg3_yado_01"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_get_party, LW(12))
        CALL(evt_mario_get_exparty, LW(13))
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_goodbye_party, 1)
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_cam3d_evt_set, -135, 193, 295, 120, 45, 82, 0, 11)
        END_IF
        IF_INT_EQUAL(LF(1), 1)
            CALL(evt_cam3d_evt_set, -49, 117, 190, 152, 45, -10, 0, 11)
        END_IF
        IF_INT_EQUAL(LF(2), 1)
            CALL(evt_cam3d_evt_set, -75, 160, 266, 155, 65, -10, 0, 11)
        END_IF
        WAIT_FRAMES(1)
        CALL(evt_mario_flag_onoff, 1, 768)
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP5"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_jump_pos, 115, 32, 90, FLOAT(400.0f), 0, -1000)
            CALL(evt_mario_set_pos, 115, 32, 90)
        END_IF
        IF_INT_EQUAL(LF(1), 1)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP3"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_jump_pos, 205, 16, -115, FLOAT(400.0f), 0, -1000)
            CALL(evt_mario_set_pos, 205, 16, -115)
        END_IF
        IF_INT_EQUAL(LF(2), 1)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_jump_pos, 228, 17, -90, FLOAT(400.0f), 0, -1000)
            CALL(evt_mario_set_pos, 228, 17, -90)
        END_IF
        CALL(evt_mario_adjust_dir)
        WAIT_MS(200)
        BEGIN_INLINE_EVENT_GET_ID(LW(10))
            CALL(paper_on, FLOAT(-45.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_MARIO_SLEEP1"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(200)
            CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_MARIO_SLEEP2"), LW(0), LW(1), LW(2), 0)
        END_INLINE_EVENT
        CALL(tou_07_snd_off)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_GO_SLEEPING1"))
        CALL(evt_fade_entry, 4, 1000, 0, 0, 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
        CALL(evt_fade_end_wait)
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_pouch_mario_recovery)
            ELSE
            CALL(mario_recovery)
        END_IF
        CALL(evt_pouch_all_party_recovery)
        WAIT_MS(3000)
        DELETE_EVENT(LW(10))
        CALL(paper_off)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_mario_flag_onoff, 0, 768)
        IF_INT_EQUAL(LF(0), 1)
            IF_INT_NOT_EQUAL(LW(12), 0)
                CALL(evt_mario_set_party_pos, 0, LW(12), 75, 37, 100)
            END_IF
            IF_INT_NOT_EQUAL(LW(13), 0)
                CALL(evt_mario_set_party_pos, 1, LW(13), 75, 37, 105)
            END_IF
            CALL(evt_mario_set_pos, 115, 32, 90)
        END_IF
        IF_INT_EQUAL(LF(1), 1)
            IF_INT_NOT_EQUAL(LW(12), 0)
                CALL(evt_mario_set_party_pos, 0, LW(12), 160, 22, -115)
            END_IF
            IF_INT_NOT_EQUAL(LW(13), 0)
                CALL(evt_mario_set_party_pos, 1, LW(13), 160, 22, -110)
            END_IF
            CALL(evt_mario_set_pos, 205, 16, -115)
        END_IF
        IF_INT_EQUAL(LF(2), 1)
            IF_INT_NOT_EQUAL(LW(12), 0)
                CALL(evt_mario_set_party_pos, 0, LW(12), 183, 22, -90)
            END_IF
            IF_INT_NOT_EQUAL(LW(13), 0)
                CALL(evt_mario_set_party_pos, 1, LW(13), 205, 22, -85)
            END_IF
            CALL(evt_mario_set_pos, 228, 17, -90)
        END_IF
        CALL(evt_mario_set_dir, 270, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_party_set_dir, 1, 270, 0)
        CALL(evt_mario_adjust_dir)
        CALL(evt_cam3d_evt_off, 0, 11)
        WAIT_FRAMES(1)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        CALL(tou_07_snd_on)
        CALL(evt_fade_entry, 9, 1000, 0, 0, 0)
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(bed_entry)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(0), 0, PTR(evt_tou_bed), 0)
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_POWER_PLUS, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tou_07_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_hitobj_name_callback, STRING("a_bed"), PTR(bed_entry))
    CALL(evt_sub_animgroup_async, STRING("p_slit"))
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSWF(2389), 0)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            IF_INT_LESS(GSW_STORY_PROGRESS, 158)
                SET_VALUE(GSW_STORY_PROGRESS, 158)
            END_IF
        END_IF
    END_IF
    CALL(evt_tou_get_ranking, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("e_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("e_2"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("e_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("e_2"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(159)
        SET_VALUE(LF(0), 0)
        CASE_LESS(172)
        SET_VALUE(LF(0), 1)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2502), 0)
            SET_VALUE(LF(0), 0)
            ELSE
            SET_VALUE(LF(0), 1)
        END_IF
    END_SWITCH
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_run_case_evt, CASE_HAMMER_2, 1, STRING("win_kabe"), 0, PTR(evt_dakuto), 0)
        CALL(evt_run_case_evt, CASE_HAMMER, 1, STRING("win_kabe"), 0, PTR(evt_dakuto2), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("win_kabe"), 0, PTR(evt_dakuto2), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kankikuti"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("win_kabe"), 1, 0)
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("M_make"), 0, PTR(evt_tou_match_make_default), 0)
    CALL_CHILD(PTR(evt_tou_monitor_init))
    IF_INT_EQUAL(GSWF(2389), 1)
        SET_VALUE(GSWF(2389), 0)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(127, 137)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU2"))
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            CASE_GREATER_OR_EQUAL(165)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU2"))
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            CASE_DEFAULT
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        END_SWITCH
        IF_INT_LESS(GSW_STORY_PROGRESS, 158)
            SPAWN_EVENT(PTR(evt_champion))
            RETURN
        END_IF
        IF_INT_LESS(GSW_STORY_PROGRESS, 172)
            SPAWN_EVENT(PTR(evt_tou_match_after_default))
            RETURN
        END_IF
        IF_INT_EQUAL(GSWF(2503), 0)
            SPAWN_EVENT(PTR(evt_champion_re))
            ELSE
            SPAWN_EVENT(PTR(evt_tou_match_after_default))
        END_IF
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2375), 1)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_GANCE1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        SPAWN_EVENT(PTR(evt_sensyu))
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 163)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envoff, PSND_FADEOUT)
        ELSE
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(127, 138)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            CASE_DEFAULT
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        END_SWITCH
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("e_bero"))
        IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 1)
            SET_VALUE(LW(0), STRING("n_bero"))
            CALL_CHILD(PTR(bero_case_switch_off))
            SPAWN_EVENT(PTR(evt_gardman_spawn))
        END_IF
    END_IF
    RETURN
EVT_END()

static USER_FUNC(paper_on) {
    f32 value = evtGetFloat(event, event->args[0]);
    MarioWork* mario = marioGetPtr();

    if (isFirstCall) {
        marioPaperOn("p_slit");
        marioChgPose("M_S_1");
        marioChgPaper("PM_S_3A");
    }
    mario->displayDirection = mario->displayDirectionTarget = value;
    return EVT_RETURN_BLOCK;
}

USER_FUNC(tou_07_icon_off) {
    iconFlagOn(itemNameToPtr("belt")->name, 2);
    return EVT_RETURN_DONE;
}

USER_FUNC(tou_07_belt_camid) {
    EffectEntry* eff = effNameToPtr("belt");
    // ベルト
    NpcEntry* npc = npcNameToPtr("\x83\x78\x83\x8B\x83\x67");

    ((EffStarStoneWork*)eff->userdata)->cam = 7;
    npc->cameraId = 7;
    return EVT_RETURN_DONE;
}


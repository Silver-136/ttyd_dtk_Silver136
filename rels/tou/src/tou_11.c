#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "tou.h"
#include "evt/evt_npc.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"

USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);

static EvtWord n_door_open[12];
static EvtWord n_door_close[12];
static EvtWord e_door_open[12];
static EvtWord e_door_close[12];
static BeroEntry bero_data[3];
static NpcSetupInfo npcEnt[2];
EvtWord evt_tojikome[296];
EvtWord evt_tojikome_nidome[104];
EvtWord evt_tojikome_sandome[37];
static EvtWord evt_tojikome_msg[30];
EvtWord evt_postar[23];
EvtWord tou_11_init_evt[178];

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

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
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
        "s_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_door_close, // enterAnimeArg
        n_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        16, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_09", // nextMapName
        "a_door_9_omote", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static NpcSetupInfo npcEnt[] = {
    {
        /*"ガードマン"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_tojikome)
    IF_INT_EQUAL(GSWF(2386), 1)
        RETURN
    END_IF
    SET_VALUE(GSWF(2386), 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, 0, 0, -170)
    CALL(evt_party_set_pos, 0, 0, 0, -200)
    CALL(evt_npc_set_position, STRING("ガードマン"), 0, 0, -160)
    CALL(evt_npc_status_onoff, 1, STRING("ガードマン"), NPC_STATUS_FACE_MOVE_DIR)
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
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
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
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ガードマン"), -1, -90, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_move_position, STRING("ガードマン"), -30, -80, 0, FLOAT(70.0f), 1)
        CALL(evt_set_dir_to_target, STRING("ガードマン"), STRING("mario"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 0, -90, FLOAT(60.0f))
        CALL(evt_party_stop, 0)
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 60, -90, FLOAT(60.0f))
        CALL(evt_party_set_dir_npc, 0, STRING("ガードマン"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_mario_mov_pos2, 0, -80, FLOAT(60.0f))
    CALL(evt_mario_mov_pos2, 30, -80, FLOAT(60.0f))
    CALL(evt_mario_set_dir_npc, STRING("ガードマン"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_409"), 0, STRING("ガードマン"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING("m"), 0, 1, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, 3, STRING("p"), 0, 1, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("ガードマン"), 0, -100, 0, FLOAT(70.0f), 1)
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
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("ガードマン"), 0, -200, 0, FLOAT(70.0f), 1)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print_party, STRING("stg3_tou_409_01"))
    WAIT_MS(1000)
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
    CALL(evt_bero_mapchange, STRING("tou_03"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_tojikome_nidome)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg3_tou_409_02"))
    CALL(evt_party_move_pos2, 0, 10, -140, FLOAT(60.0f))
    WAIT_MS(300)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_DOOR_NOTOPEN1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 107, 351, 0, 45, -1, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg3_tou_410"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
    CALL(evt_bero_mapchange, STRING("tou_03"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_tojikome_sandome)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg3_tou_421_01"))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_tojikome_msg)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_DOOR_NOTOPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_msg_print_party, STRING("stg3_tou_421_02"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_postar)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    SET_VALUE(GSWF(2387), 1)
    RETURN
EVT_END()

EVT_BEGIN(tou_11_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_tou_locker_init))
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_EQUAL(GSWF(2387), 0)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 0, STRING("pos_1"), STRING("a_pos_1"), PTR(evt_postar))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("pos_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_pos_1"), 1, 0)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kuro_1"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LW(0), 0)
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("n_bero"), 0, PTR(evt_tojikome_msg), 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nidome"))
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        SPAWN_EVENT(PTR(evt_tojikome_nidome))
        CALL(evt_mario_set_pos, 30, 0, -100)
        CALL(evt_party_set_pos, 0, 60, 0, -100)
        CALL(evt_cam_road_reset)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("sandome"))
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        SPAWN_EVENT(PTR(evt_tojikome_sandome))
        CALL(evt_mario_set_pos, 30, 0, -100)
        CALL(evt_party_set_pos, 0, 60, 0, -100)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_cam_road_reset)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2383), 1)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envoff, PSND_FADEOUT)
        SPAWN_EVENT(PTR(evt_tojikome))
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(127, 137)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_GREATER_OR_EQUAL(165)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
    END_SWITCH
    RETURN
EVT_END()


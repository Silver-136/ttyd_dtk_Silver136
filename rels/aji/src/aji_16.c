#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_fade.h"
#include "system.h"

#include "mario/mario.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "aji.h"

USER_FUNC(evt_mario_dispflag_onoff);
static USER_FUNC(peach_set_pose);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
EvtWord w_bero_in_evt[15];
static BeroEntry bero_entry_data[3];
static BeroEntry bero_peach_data[2];
EvtWord evt_last_mail_init[62];
EvtWord evt_last_mail[56];
EvtWord peach_evt_door_open[41];
static EvtWord peach_evt_stg1[97];
static EvtWord peach_evt_stg1_1[86];
EvtWord peach_evt_stg2_1[103];
static EvtWord peach_evt_stg2[107];
static EvtWord peach_evt_stg3[158];
static EvtWord peach_evt_stg6[138];
static EvtWord peach_evt_door_stop[18];
EvtWord aji_16_init_evt[167];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_2"), 0, 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(w_bero_in_evt)
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_peach_set_condition, 0)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        w_bero_in_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_15", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
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
        "aji_01", // nextMapName
        "w_bero_5", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_peach_data[] = {
    {
        "e_bero", // hitName
        0, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        8, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        "w_bero_5", // nextBeroName
        0, 0, 0, 0, // enterAnimeType..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_last_mail_init)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, -215, 0, 0)
    CALL(evt_party_set_pos, 0, -215, 0, 0)
    CALL(evt_npc_entry, STRING("peach"), STRING("c_peach"))
    CALL(evt_paper_entry, STRING("p_b_st"))
    CALL(evt_npc_set_paper, STRING("peach"), STRING("p_b_st"))
    CALL(evt_npc_set_paper_anim, STRING("peach"), STRING("P_Z_7"))
    CALL(evt_npc_set_position, STRING("peach"), 0, 0, 0)
    CALL(evt_cam3d_evt_set, 100, 80, 285, 100, 40, -13, 0, 11)
    CALL(evt_fade_softfocus_onoff, 1)
    SPAWN_EVENT(PTR(evt_last_mail))
    RETURN
EVT_END()

EVT_BEGIN(evt_last_mail)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, -100, 80, 285, -100, 40, -13, 5000, 0)
    WAIT_MS(2500)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 255, 0, 2000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_color, STRING("peach"), 255, 255, 255, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg7_aji_58"), 0, 0)
    WAIT_MS(300)
    CALL(evt_bero_mapchange, STRING("aji_17"), 0)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_door_open)
    CALL(evt_cam3d_evt_set, 35, 114, 334, 157, 54, 17, 1000, 11)
    WAIT_MS(1000)
    WAIT_MS(1000)
    SET_VALUE(LW(1), 1)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(PTR(e_door_open))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(500)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG7_AJI2"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 0, 1)
    CALL(evt_peach_set_condition, 4)
    CALL(evt_mario_set_pos, 0, 0, 0)
    CALL(evt_cam3d_evt_set, 0, 125, 381, 0, 55, -13, 0, 11)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1000)
    CALL(evt_fade_tec_onoff, 0, 1)
    WAIT_MS(1000)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_4"), STRING("P_T_4"))
    CALL(evt_msg_print, 0, STRING("peach_aji_34"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 0, 80, 209, 0, 40, -13, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("peach_aji_34_01"), 0, STRING("mario"))
    CALL(evt_peach_set_condition, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 57)
    WAIT_MS(300)
    CALL(evt_cam3d_evt_off, 600, 11)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg1_1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_SURPRISED1_3"), LW(3), LW(4), LW(5), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(300)
    CALL_CHILD(PTR(peach_evt_door_open))
    CALL(evt_msg_print, 0, STRING("peach_aji_38"), 0, STRING("mario"))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_del_case_evt, 0, GW(15))
    SET_VALUE(GSW_STORY_PROGRESS, 58)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_stg2_1)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
    LOOP(0)
        CALL(evt_sub_get_stopwatch, LW(0), LW(1))
        IF_INT_GREATER_OR_EQUAL(LW(1), 5000)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_SURPRISED1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(500)
    CALL_CHILD(PTR(peach_evt_door_open))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_75"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_off, 1000, 11)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, 113)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_del_case_evt, 0, GW(15))
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg2)
    CALL(evt_mario_set_mode, 1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -75, 0, -50)
    CALL(evt_mario_set_dir, 90, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, -138, 146, 432, 122, 53, -60, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(800)
    CALL(peach_set_pose, STRING("P_S_4"))
    CALL(evt_mario_dispflag_onoff, 1, 4)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_4"), STRING("P_T_4"))
    CALL(evt_msg_print, 0, STRING("peach_aji_73"), 0, 0)
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_74"), 0, STRING("mario"))
    CALL(evt_mario_dispflag_onoff, 0, 4)
    CALL(peach_set_pose, STRING("P_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    CALL(evt_cam3d_evt_off, 1000, 11)
    WAIT_MS(1000)
    SPAWN_EVENT(PTR(peach_evt_stg2_1))
    SET_VALUE(GSWF(4202), 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_mario_set_pos, -75, 0, -50)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_cam3d_evt_set, 115, 90, 444, 115, 60, -55, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_cam3d_evt_set, -75, 90, 444, -75, 60, -55, 2500, 0)
    WAIT_MS(3000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_SAD1_1"), LW(0), LW(1), LW(2), 0)
    CALL(peach_set_pose, STRING("P_U_1"))
    WAIT_MS(1400)
    CALL(evt_msg_print, 0, STRING("peach_aji_112"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 800, 11)
    WAIT_MS(800)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_P_1"), STRING("P_T_9"))
    CALL(evt_msg_print, 0, STRING("peach_aji_113"), 0, STRING("mario"))
    CALL_CHILD(PTR(peach_evt_door_open))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 0, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_114"), 0, STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, 165)
    CALL(evt_cam3d_evt_off, 1000, 11)
    WAIT_MS(1000)
    CALL(peach_set_pose, STRING("P_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg6)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, 512)
    CALL(evt_mario_set_pos, -75, 0, -50)
    CALL(evt_mario_set_dir, 270, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, 250, 140, 392, 35, 75, 2, 0, 11)
    CALL(evt_mario_adjust_dir)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 0, 11)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING("P_S_7"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_7"), STRING("P_T_13"))
    CALL(evt_msg_print, 0, STRING("peach_aji_280"), 0, STRING("mario"))
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(300)
    WAIT_MS(500)
    CALL_CHILD(PTR(peach_evt_door_open))
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_B_2"), STRING("P_T_7"))
    CALL(evt_msg_print, 0, STRING("peach_aji_281"), 0, STRING("mario"))
    CALL(peach_set_pose, STRING("P_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, 340)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_door_stop)
    CALL(evt_mario_key_onoff, 0)
    SUSPEND_TYPES_OTHER(239)
    CALL(evt_msg_print, 0, STRING("peach_aji_38_01"), 0, STRING("mario"))
    RESUME_TYPES_OTHER(239)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(aji_16_init_evt)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("peach"))
        CALL_CHILD(PTR(evt_last_mail_init))
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("peach_evt_stg1"))
        CALL(evt_cam3d_evt_set, 78, 152, 528, 78, 57, -13, 0, 11)
        SPAWN_EVENT(PTR(peach_evt_stg1))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 57)
        IF_INT_EQUAL(GSWF(4199), 1)
            SPAWN_EVENT(PTR(peach_evt_stg1_1))
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 112)
        IF_INT_EQUAL(GSWF(4202), 0)
            SPAWN_EVENT(PTR(peach_evt_stg2))
            ELSE
            SPAWN_EVENT(PTR(peach_evt_stg2_1))
        END_IF
    END_IF
    IF_STR_EQUAL(LW(0), STRING("peach_evt_stg3"))
        SPAWN_EVENT(PTR(peach_evt_stg3))
    END_IF
    IF_STR_EQUAL(LW(0), STRING("peach_evt_stg6"))
        SPAWN_EVENT(PTR(peach_evt_stg6))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 57)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING(""))
            CALL(evt_mario_set_pos, 100, 0, 0)
            WAIT_FRAMES(1)
            CALL(evt_cam_road_reset)
        END_IF
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 58)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, 9, 1, STRING("e_bero"), 0, PTR(peach_evt_door_stop), GW(15))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 112)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, 9, 1, STRING("e_bero"), 0, PTR(peach_evt_door_stop), GW(15))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 352)
        CALL(evt_mario_kill_party, 0)
        CALL(evt_mario_set_mode, 1)
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_PEACH1"))
        CALL(evt_snd_envoff, 512)
    END_IF
    CALL(evt_snd_set_rev_mode, 1)
    CALL(evt_hitobj_onoff, STRING("A_door1_1"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_door1_2"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_door2_1"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_door2_2"), 1, 0)
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()

static USER_FUNC(peach_set_pose) {
    const char* pose = (const char*)evtGetValue(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    marioChgPose(pose);
    return EVT_RETURN_DONE;
}

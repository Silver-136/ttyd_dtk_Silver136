// aji_16 (敵のアジト, X-Naut Fortress): Peach's room: her captivity scenes (peach_evt_stg*) and the last e-mail
// (evt_last_mail).

#include "version.h"
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
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "mario/mario.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "aji.h"
#include "driver/seqdrv.h"

static USER_FUNC(peach_set_pose);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static EvtWord w_bero_in_evt[15];
static BeroEntry bero_entry_data[3];
static BeroEntry bero_peach_data[2];
static EvtWord evt_last_mail_init[62];
static EvtWord evt_last_mail[56];
static EvtWord peach_evt_door_open[41];
static EvtWord peach_evt_stg1[97];
static EvtWord peach_evt_stg1_1[86];
EvtWord peach_evt_stg2_1[103];
static EvtWord peach_evt_stg2[107];
static EvtWord peach_evt_stg3[158];
static EvtWord peach_evt_stg6[138];
static EvtWord peach_evt_door_stop[18];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord aji_16_init_evt[];
#else
EvtWord aji_16_init_evt[167];
#endif

/**
 * Door close animation step: moves the two door leaves S_door1_1
 * and S_door1_2 in opposite directions along Z by the animation value in LW(0) times 0.22167969;
 * the open and close scripts apply the same transform.
 */
static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/**
 * Door open animation step: moves the two door leaves S_door1_1
 * and S_door1_2 in opposite directions along Z by the animation value in LW(0) times 0.22167969;
 * the open and close scripts apply the same transform.
 */
static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/**
 * Door close animation step: moves the two door leaves S_door2_1
 * and S_door2_2 in opposite directions along Z by the animation value in LW(0) times -0.22167969;
 * the open and close scripts apply the same transform.
 */
static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/**
 * Door open animation step: moves the two door leaves S_door2_1
 * and S_door2_2 in opposite directions along Z by the animation value in LW(0) times -0.22167969;
 * the open and close scripts apply the same transform.
 */
static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door2_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/**
 * Enter event of the west door w_bero: if the player is Peach,
 * resets her to the normal condition (evt_peach_set_condition 0), and sets LW(0) = 0 so the entry proceeds.
 */
static EVT_BEGIN(w_bero_in_evt)
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_peach_set_condition, 0)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Exits of this room: the west door to aji_15 and the east door to aji_01. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Replacement for the east door exit, swapped in with evt_bero_overwrite during Peach's scene so it leads on
 * to aji_01 (w_bero_5).
 */
static BeroEntry bero_peach_data[] = {
    {
        "e_bero", // hitName
        0, // type
        10, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Sets up the Last Mail scene when aji_16 is entered as "peach":
 * Mario and the partner are placed on the left,
 * Peach uses paper animation P_Z_7 in the middle, soft focus is enabled and evt_last_mail starts.
 */
static EVT_BEGIN(evt_last_mail_init)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, -215, 0, 0)
    CALL(evt_party_set_pos, 0, -215, 0, 0)
    CALL(evt_npc_entry, STRING("peach"), STRING("c_peach")) // Princess Peach (Last Mail scene)
    CALL(evt_paper_entry, STRING("p_b_st"))
    CALL(evt_npc_set_paper, STRING("peach"), STRING("p_b_st"))
    CALL(evt_npc_set_paper_anim, STRING("peach"), STRING("P_Z_7"))
    CALL(evt_npc_set_position, STRING("peach"), 0, 0, 0)
    CALL(evt_cam3d_evt_set, 100, 80, 285, 100, 40, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_fade_softfocus_onoff, 1)
    SPAWN_EVENT(evt_last_mail)
    RETURN
EVT_END()

/**
 * Plays the Last Mail scene: the camera pans across the room for five seconds,
 * Peach fades away over two seconds, the text stg7_aji_58 appears and the map changes to aji_17.
 */
static EVT_BEGIN(evt_last_mail)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, -100, 80, 285, -100, 40, -13, 5000, EASING_LINEAR)
    WAIT_MS(2500)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 2000)
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

/**
 * Opens the east door e_bero for Peach with bero_open_door_play_se,
 * driving e_door_open's sliding leaves with values 0 to 180 over half a second.
 */
static EVT_BEGIN(peach_evt_door_open)
    CALL(evt_cam3d_evt_set, 35, 114, 334, 157, 54, 17, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    WAIT_MS(1000)
    SET_VALUE(LW(1), 1)
    SPAWN_EVENT(bero_open_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL_CHILD(e_door_open)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(500)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg1)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI2"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
    CALL(evt_peach_set_condition, 4)
    CALL(evt_mario_set_pos, 0, 0, 0)
    CALL(evt_cam3d_evt_set, 0, 125, 381, 0, 55, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_fade_tec_onoff, 0, 1)
    WAIT_MS(1000)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_4"), STRING("P_T_4"))
    CALL(evt_msg_print, 0, STRING("peach_aji_34"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, 0, 80, 209, 0, 40, -13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("peach_aji_34_01"), 0, STRING("mario"))
    CALL(evt_peach_set_condition, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_18_PEACH_CAUGHT)
    WAIT_MS(300)
    CALL(evt_cam3d_evt_off, 600, EASING_SINE_IN_OUT)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg1_1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_SURPRISED1_3"), LW(3), LW(4), LW(5), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(300)
    CALL_CHILD(peach_evt_door_open)
    CALL(evt_msg_print, 0, STRING("peach_aji_38"), 0, STRING("mario"))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_off)
    CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_on)

    // Remove the blocked-door interaction after installing the replacement exit.
    CALL(evt_del_case_evt, 0, GW(15))
    SET_VALUE(GSW_STORY_PROGRESS, STG1_18_01_PEACH_AN_OPEN_DOOR)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(peach_evt_stg2_1)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)

    // Keep control enabled during this initial stopwatch delay.
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(500)
    CALL_CHILD(peach_evt_door_open)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_75"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_25_01_PEACH_CRUMP_X_NAUTS)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_off)
    CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_on)
    CALL(evt_del_case_evt, 0, GW(15))
    RETURN
EVT_END()

/**
 * Peach's chapter 2 scene in her room: Peach's BGM, peach_aji_73 and 74,
 * then peach_evt_stg2_1 is spawned and GSWF_AJI_16_PEACH_STG2_SCENE_SEEN is set.
 */
static EVT_BEGIN(peach_evt_stg2)
    CALL(evt_mario_set_mode, 1)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -75, 0, -50)
    CALL(evt_mario_set_dir, 90, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, -138, 146, 432, 122, 53, -60, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    WAIT_MS(800)
    CALL(peach_set_pose, STRING("P_S_4"))
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_4"), STRING("P_T_4"))
    CALL(evt_msg_print, 0, STRING("peach_aji_73"), 0, 0)
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("peach_aji_74"), 0, STRING("mario"))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(peach_set_pose, STRING("P_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    SPAWN_EVENT(peach_evt_stg2_1)
    SET_VALUE(GSWF_AJI_16_PEACH_STG2_SCENE_SEEN, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_mario_set_pos, -75, 0, -50)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_cam3d_evt_set, 115, 90, 444, 115, 60, -55, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    CALL(evt_cam3d_evt_set, -75, 90, 444, -75, 60, -55, 2500, EASING_LINEAR)
    WAIT_MS(3000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_PEACH_SAD1_1"), LW(0), LW(1), LW(2), 0)
    CALL(peach_set_pose, STRING("P_U_1"))
    WAIT_MS(1400)
    CALL(evt_msg_print, 0, STRING("peach_aji_112"), 0, STRING("mario"))
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_P_1"), STRING("P_T_9"))
    CALL(evt_msg_print, 0, STRING("peach_aji_113"), 0, STRING("mario"))
    CALL_CHILD(peach_evt_door_open)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_off)
    CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_on)
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("peach_aji_114"), 0, STRING("mario"))
    SET_VALUE(GSW_STORY_PROGRESS, STG3_33_PEACH_THE_SECRET_WEAPON)
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(peach_set_pose, STRING("P_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(peach_evt_stg6)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_PEACH1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_mario_set_pos, -75, 0, -50)
    CALL(evt_mario_set_dir, 270, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, 250, 140, 392, 35, 75, 2, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_adjust_dir)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(peach_set_pose, STRING("P_S_7"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_7"), STRING("P_T_13"))
    CALL(evt_msg_print, 0, STRING("peach_aji_280"), 0, STRING("mario"))
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(300)
    WAIT_MS(500)
    CALL_CHILD(peach_evt_door_open)
    CALL(evt_cam3d_evt_set, -75, 80, 184, -75, 40, -63, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_B_2"), STRING("P_T_7"))
    CALL(evt_msg_print, 0, STRING("peach_aji_281"), 0, STRING("mario"))
    CALL(peach_set_pose, STRING("P_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("P_S_1"), STRING("P_T_1"))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_off)
    CALL(evt_bero_overwrite, STRING("e_bero"), PTR(bero_peach_data))
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_on)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_55_PEACH_GRODUS_X_NAUT)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Case script on the doors Peach may not use here:
 * freezes controls, suspends other event types and shows peach_aji_38_01.
 */
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

    // This entrance uses the mail scene and bypasses the ordinary room setup.
    IF_STR_EQUAL(LW(0), STRING("peach"))
        CALL_CHILD(evt_last_mail_init)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("peach_evt_stg1"))
        CALL(evt_cam3d_evt_set, 78, 152, 528, 78, 57, -13, 0, EASING_SINE_IN_OUT)
        SPAWN_EVENT(peach_evt_stg1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_18_PEACH_CAUGHT)
        IF_INT_EQUAL(GSWF_AJI_15_VISITED, 1)
            SPAWN_EVENT(peach_evt_stg1_1)
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_25_AFTER_THE_BOSS_BATTLE)
        IF_INT_EQUAL(GSWF_AJI_16_PEACH_STG2_SCENE_SEEN, 0)
            SPAWN_EVENT(peach_evt_stg2)
        ELSE
            SPAWN_EVENT(peach_evt_stg2_1)
        END_IF
    END_IF
    IF_STR_EQUAL(LW(0), STRING("peach_evt_stg3"))
        SPAWN_EVENT(peach_evt_stg3)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("peach_evt_stg6"))
        SPAWN_EVENT(peach_evt_stg6)
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_18_PEACH_CAUGHT)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING(""))
            CALL(evt_mario_set_pos, 100, 0, 0)
            WAIT_FRAMES(1)
            CALL(evt_cam_road_reset)
        END_IF
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_18_01_PEACH_AN_OPEN_DOOR)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(peach_evt_door_stop), GW(15))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_25_AFTER_THE_BOSS_BATTLE)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(peach_evt_door_stop), GW(15))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG7_00_START)
        CALL(evt_mario_kill_party, 0)
        CALL(evt_mario_set_mode, 1)
    ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_PEACH1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
    END_IF
#if VERSION >= VERSION_JP
    CALL(evt_snd_set_rev_mode, 1)
#endif
    CALL(evt_hitobj_onoff, STRING("A_door1_1"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_door1_2"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_door2_1"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_door2_2"), 1, 0)
    SPAWN_EVENT(evt_emergency)
    RETURN
EVT_END()

/** Script function: sets Mario's (Peach's) pose to the animation named in args[0]. */
static USER_FUNC(peach_set_pose) {
    const char* pose = (const char*)evtGetValue(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    marioChgPose(pose);
    return EVT_RETURN_DONE;
}

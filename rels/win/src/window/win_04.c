// win_04 (ウィンディん家周辺, Flurrie's house in Boggly Woods): the red-walled main room of Flurrie's house, with its
// chandelier and mirror ball; exits west to win_03 and east through Flurrie's door to her bedroom win_05.

#include "battle/battle_unit.h"
#include "driver/animdrv.h"
#include "evt/evt_window.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_fade.h"
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
#include "evt/evt_sub.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"
#include "party.h"
#include "driver/envdrv.h"

#include "driver/dispdrv.h"
#include "stdlib.h"
#include "memory.h"
#include "driver/camdrv.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "driver/fadedrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_bero.h"
#include "win.h"
#include "mario/mario_pouch.h"
#include "driver/seqdrv.h"

typedef struct Flower {
    s16 state;
    s16 timer;
    f32 x;
    f32 y;
    f32 scale;
    f32 targetScale;
    s32 alpha;
} Flower;
typedef struct FlowerWork {
    s32 count;
    Flower* flowers;
} FlowerWork;
static s32 ssf[3];

USER_FUNC(evt_flower);
USER_FUNC(evt_flower_end);
USER_FUNC(evt_flower_end_main);
USER_FUNC(evt_flower_init);
USER_FUNC(fade_alpha);
USER_FUNC(flower_pos_report);
USER_FUNC(ss_ch_check);
USER_FUNC(ss_ch_set);
static f32 randf(f32 min, f32 max);
void mirror_ball(void* param);

EvtWord win_04w_door_open[10];
EvtWord win_04w_door_close[10];
EvtWord win_04e_door_open[10];
EvtWord win_04e_door_close[10];
static EvtWord out_w_bero[12];
BeroEntry bero_witchtrio[1];
static BeroEntry bero_entry_data[3];
EvtWord meet_clouda[76];
#include "version.h"
#if VERSION <= VERSION_DEMO_US
extern EvtWord meet_clouda2[];
#else
EvtWord meet_clouda2[139];
#endif
EvtWord clouda_inai[17];
#if VERSION <= VERSION_DEMO_US
extern EvtWord clouda_irai[];
#else
EvtWord clouda_irai[444];
#endif
EvtWord clouda_irai_ato[20];
EvtWord mirror_trans[35];
EvtWord mirror_rotate[41];
EvtWord mario_move_wait[63];
EvtWord dummy1_jump[26];
EvtWord dummy2_jump[32];
static s32 item_list[2];
#if VERSION <= VERSION_DEMO_US
extern EvtWord clouda_nakama[];
#else
EvtWord clouda_nakama[1556];
#endif
EvtWord touch_door[39];
EvtWord mirror_ball_event[18];
static u32 texture_0[1248];
Vec rose_table_direct[55];
f32 rose_table[120];
EvtWord flower_event[28];
EvtWord flower_disp_pos[21];
static EvtWord cloud_toujou[95];
#if VERSION <= VERSION_DEMO_US
extern EvtWord win_04_init_evt[];
#else
EvtWord win_04_init_evt[94];
#endif


/**
 * Door-open callback for the west bero exit: names the door map objects "S_doa_w_l" and "S_doa_w_r"
 * and runs the shared evt_win_door_open.
 */
EVT_BEGIN(win_04w_door_open)
    SET_VALUE(LW(8), STRING("S_doa_w_l"))
    SET_VALUE(LW(9), STRING("S_doa_w_r"))
    SPAWN_EVENT(PTR(evt_win_door_open))
    RETURN
EVT_END()

/**
 * Door-close callback for the west bero exit: names the door map objects "S_doa_w_l" and "S_doa_w_r"
 * and runs the shared evt_win_door_close.
 */
EVT_BEGIN(win_04w_door_close)
    SET_VALUE(LW(8), STRING("S_doa_w_l"))
    SET_VALUE(LW(9), STRING("S_doa_w_r"))
    SPAWN_EVENT(PTR(evt_win_door_close))
    RETURN
EVT_END()

/**
 * Door-open callback for the east bero exit: names the door map objects "S_doa_e_l" and "S_doa_e_r"
 * and runs the shared evt_win_door_open.
 */
EVT_BEGIN(win_04e_door_open)
    SET_VALUE(LW(8), STRING("S_doa_e_l"))
    SET_VALUE(LW(9), STRING("S_doa_e_r"))
    SPAWN_EVENT(PTR(evt_win_door_open))
    RETURN
EVT_END()

/**
 * Door-close callback for the east bero exit: names the door map objects "S_doa_e_l" and "S_doa_e_r"
 * and runs the shared evt_win_door_close.
 */
EVT_BEGIN(win_04e_door_close)
    SET_VALUE(LW(8), STRING("S_doa_e_l"))
    SET_VALUE(LW(9), STRING("S_doa_e_r"))
    SPAWN_EVENT(PTR(evt_win_door_close))
    RETURN
EVT_END()

/**
 * Exit script of the west door in bero_witchtrio,
 * the exit the init script swaps in at STG2_05_02 (it leads to win_00, where witchtrio_picture plays):
 * sets the next map change to an iris wipe (500 ms out, 500 ms in); returns LW(0) = 0.
 */
static EVT_BEGIN(out_w_bero)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 1, 500)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/**
 * Replacement for the west door, swapped in once Flurrie has asked for her necklace: it leads to win_00
 * with an iris wipe, where the Sirens' sketch scene (witchtrio_picture) plays.
 */
BeroEntry bero_witchtrio[] = {
    {
        "w_bero", // hitName
        1, // type
        0, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        out_w_bero, // exitEvent
        "win_00", // nextMapName
        "", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        win_04w_door_close, // enterAnimeArg
        win_04w_door_open, // exitAnimeArg
    },
};

/** Doors of this room: west back to win_03 and east into win_05, both with their opening animations. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        0, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "win_03", // nextMapName
        "n_bero", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        win_04w_door_close, // enterAnimeArg
        win_04w_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        0, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "win_05", // nextMapName
        "w_bero", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        win_04e_door_close, // enterAnimeArg
        win_04e_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * Spawned by the init script at STG2_04 when Mario first enters Flurrie's house:
 * once he is in, Punio (the second partner, "extparty") calls out "Excuse me!",
 * the camera looks over the room and Punio guesses she went for a walk (stg2_win_e05_00_00, 01).
 * The story becomes STG2_05_A_SHY_FLURRIE and meet_clouda2 starts.
 */
EVT_BEGIN(meet_clouda)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_party_move_pos2, 1, -110, 0, FLOAT(120.0f))
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_00"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_set, 186, 202, 539, 186, 106, -3, 500, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -140, 100, 341, -140, 40, -3, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_01"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_off, 300, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_05_A_SHY_FLURRIE)
    SPAWN_EVENT(PTR(meet_clouda2))
    RETURN
EVT_END()

EVT_BEGIN(meet_clouda2)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 150)
            IF_INT_GREATER_OR_EQUAL(LW(1), 50)
                IF_INT_LESS(LW(1), 51)
#if VERSION <= VERSION_DEMO_US
CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
                    IF_INT_GREATER_OR_EQUAL(LW(1), 50)
                        IF_INT_LESS(LW(1), 51)
                            CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
                            IF_INT_GREATER_OR_EQUAL(LW(1), 50)
                                IF_INT_LESS(LW(1), 51)
#endif
                    BREAK_LOOP
#if VERSION <= VERSION_DEMO_US
                                END_IF
                            END_IF
                        END_IF
                    END_IF
#endif
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
#if VERSION > VERSION_DEMO_US
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_party_force_reset_outofscreen, 0)
        CALL(evt_party_stop, 0)
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 50)
        IF_INT_LESS(LW(2), 0)
            ADD(LW(2), 50)
            ELSE
            SUBTRACT(LW(2), 50)
        END_IF
        CALL(evt_party_move_pos2, 0, 130, LW(2), FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_party_force_reset_outofscreen, 1)
#endif
    CALL(evt_party_stop, 1)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    ADD(LW(0), 50)
    IF_INT_LESS(LW(2), 0)
        ADD(LW(2), 50)
        ELSE
        SUBTRACT(LW(2), 50)
    END_IF
#if VERSION <= VERSION_DEMO_US
CALL(evt_party_move_pos2, 1, LW(0), LW(2), FLOAT(120.0f))
#else
    CALL(evt_party_move_pos2, 1, 160, LW(2), FLOAT(120.0f))
    CALL(evt_party_set_dir, 1, 90, 200)
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#endif
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_02"), 0, STRING("extparty"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_party_run, 0)
#endif
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSW_STORY_PROGRESS, STG2_05_01_FLURRIE_LIVING_ROOM)
#else
    SET_VALUE(GSW_STORY_PROGRESS, STG2_05_01_FLURRIE_LIVING_ROOM)
#endif
    RETURN
EVT_END()

/**
 * Inner-door script (touch_door) before STG2_05_01:
 * a system message says the door won't budge (stg2_win_e04_03)
 * and the partner thinks the place is deserted (stg2_win_e04_04).
 */
EVT_BEGIN(clouda_inai)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e04_03"), 0, 0)
    CALL(evt_msg_print_party, STRING("stg2_win_e04_04"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(clouda_irai)
#if VERSION <= VERSION_DEMO_US
    // Mario's controls are locked; outside the demos both partners first gather by Mario.
#endif
    CALL(evt_mario_key_onoff, 0)
#if VERSION > VERSION_DEMO_US
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_party_force_reset_outofscreen, 0)
#endif
        CALL(evt_party_stop, 0)
#if VERSION > VERSION_DEMO_US
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_party_move_pos2, 0, 255, LW(2), FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        CALL(evt_party_force_reset_outofscreen, 1)
#endif
        CALL(evt_party_stop, 1)
#if VERSION > VERSION_DEMO_US
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_party_move_pos2, 1, 225, LW(2), FLOAT(120.0f))
        CALL(evt_party_set_dir, 1, 90, 200)
    END_INLINE_EVENT
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(0))
        IS_EVENT_RUNNING(LW(11), LW(1))
        IF_INT_EQUAL(LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#endif
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_03"), 0, 0)
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_party_set_dir, 1, 90, -1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_FOLLOWER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    // Mario and the partners step up to the door and Punio asks if Flurrie is there.
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_cam3d_evt_set, 50, 90, 308, 248, 105, 20, 0, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_party_move_pos2, 0, 200, 0, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, -1)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_party_move_pos2, 1, 230, 15, FLOAT(120.0f))
        CALL(evt_party_set_dir, 1, 90, -1)
        SET_VALUE(GF(1), 0)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 240, 0, FLOAT(120.0f))
    WAIT_WHILE(GF(0))
    WAIT_WHILE(GF(1))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_04"), 0, STRING("extparty"))
    WAIT_MS(1000)
    // Flurrie recognizes Punio, who explains that the Great Tree's door is blocked; Flurrie offers to help.
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_05"), 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_06"), 0, STRING("extparty"))
    WAIT_MS(500)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_07"), 0, 0)
    WAIT_MS(500)
    // The partner asks what is wrong: Flurrie has lost her favorite necklace and won't go out without it.
    CALL(evt_msg_print_party, STRING("stg2_win_e05_00_08"))
    WAIT_MS(500)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_09"), 0, 0)
    CALL(evt_cam3d_evt_set, 200, 120, 194, 200, 85, -3, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    // The partner remembers hearing about a necklace, and Flurrie begs whoever spoke to find it.
    CALL(evt_msg_print_party, STRING("stg2_win_e05_00_10"))
    CALL(evt_cam3d_evt_set, 110, 199, 257, 393, 72, -135, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_11"), 0, 0)
    CALL(evt_cam3d_evt_set, 220, 130, 243, 220, 87, -3, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    // Punio volunteers Mario: answering Nah earns a retort, Sure a nod.
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_12"), 0, STRING("extparty"))
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_msg_select, 0, STRING("stg2_win_e05_00_12_yn"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_14"), 0, STRING("extparty"))
        ELSE
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1"), LW(0), LW(1), LW(2), 0)
        END_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_13"), 0, STRING("extparty"))
    END_IF
    // Flurrie apologizes for the trouble, the story advances and the west door now leads to win_00.
    CALL(evt_cam3d_evt_off, 300, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_00_15"), 0, 0)
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSW_STORY_PROGRESS, STG2_05_02_FLURRIE_REQUEST)
    SET_VALUE(GSWF_WIN_FLURRIE_REQUEST_ACCEPTED, 1)
#else
    SET_VALUE(GSW_STORY_PROGRESS, STG2_05_02_FLURRIE_REQUEST)
    SET_VALUE(GSWF_WIN_FLURRIE_REQUEST_ACCEPTED, 1)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), STRING("w_bero"))
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(bero_case_switch_off)
#else
    CALL_CHILD(PTR(bero_case_switch_off))
#endif
    CALL(evt_bero_overwrite, STRING("w_bero"), PTR(bero_witchtrio))
    SET_VALUE(LW(0), STRING("w_bero"))
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(bero_case_switch_on)
#else
    CALL_CHILD(PTR(bero_case_switch_on))
#endif
    RETURN
EVT_END()

/**
 * Door script after Flurrie's request: speaks "stg2_win_e05_19" from the door
 * while Mario still has not found her necklace.
 */
EVT_BEGIN(clouda_irai_ato)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e05_19"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Lowers the mirror ball: interpolates GW(1), the y offset that mirror_rotate applies to "S_mirre",
 * from 0 to -100 over 1.5 seconds,
 * with the SFX_STG2_CRAUD_LIGHT2 sound playing at (170, 50, 10).
 */
EVT_BEGIN(mirror_trans)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CRAUD_LIGHT2"), 170, 50, 10, LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, -100, 1500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(GW(1), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    RETURN
EVT_END()

/**
 * Endless loop that spins the mirror ball ("S_mirre") 360 degrees every 6 seconds
 * while keeping its height from GW(1).
 */
EVT_BEGIN(mirror_rotate)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 360, 6000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_trans, 1, STRING("S_mirre"), 0, GW(1), 0)
            CALL(evt_mapobj_rotate, 1, STRING("S_mirre"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Waits until Mario's position has stayed the same for two frames in a row;
 * clouda_nakama uses it so the scene continues once his walk has ended.
 */
EVT_BEGIN(mario_move_wait)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SET_VALUE(LW(6), 0)
    LOOP(0)
        WAIT_FRAMES(1)
        SET_VALUE(LW(3), LW(0))
        SET_VALUE(LW(4), LW(1))
        SET_VALUE(LW(5), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LW(0), LW(3))
            IF_INT_EQUAL(LW(1), LW(4))
                IF_INT_EQUAL(LW(2), LW(5))
                    ADD(LW(6), 1)
                    IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                        BREAK_LOOP
                    END_IF
                    ELSE
                    SET_VALUE(LW(6), 0)
                END_IF
                ELSE
                SET_VALUE(LW(6), 0)
            END_IF
            ELSE
            SET_VALUE(LW(6), 0)
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * In clouda_nakama, when Flurrie rushes at Mario and knocks his partner aside:
 * hops the hidden helper NPC "dummy1" (whose position the partner follows) from LW(0..2)
 * to 90 units left and 30 back, then sets ssf[0] to 1 so the scene knows the hop is over.
 */
EVT_BEGIN(dummy1_jump)
    CALL(evt_npc_set_position, STRING("dummy1"), LW(0), LW(1), LW(2))
    ADD(LW(0), -90)
    ADD(LW(2), -30)
    CALL(evt_npc_jump_position_nohit, STRING("dummy1"), LW(0), LW(1), LW(2), 800, 100)
    CALL(ss_ch_set, 0, 1)
    RETURN
EVT_END()

/**
 * The same for Punio: hops the hidden helper NPC "dummy2" from Punio's position to 90 units left and 30 forward,
 * then sets ssf[1] to 1.
 */
EVT_BEGIN(dummy2_jump)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("dummy2"), LW(0), LW(1), LW(2))
    ADD(LW(0), -90)
    ADD(LW(2), 30)
    CALL(evt_npc_jump_position_nohit, STRING("dummy2"), LW(0), LW(1), LW(2), 800, 100)
    CALL(ss_ch_set, 1, 1)
    RETURN
EVT_END()

USER_FUNC(ss_ch_check) {
    u64 out; // FAKE: reorders the args[1] load
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    out = args[1];
    evtSetFloat(event, out, ssf[index]);
    return EVT_RETURN_DONE;
}

/** Script helper: sets ssf[args[0]] to args[1]; used by clouda_nakama and the helper hop scripts. */
USER_FUNC(ss_ch_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    ssf[index] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: covers the screen with a 2D black fade of color (args[0], args[1], args[2]) and alpha args[3],
 * without animating.
 */
USER_FUNC(fade_alpha) {
    EvtWord* args = event->args;
    s32 r = evtGetValue(event, args[0]);
    s32 g = evtGetValue(event, args[1]);
    s32 b = evtGetValue(event, args[2]);
    s32 a = evtGetValue(event, args[3]);

    fadeEntry(13, 0, (GXColor){r, g, b, a});
    return EVT_RETURN_DONE;
}

/** Items Mario may hand Flurrie through her door in clouda_nakama: only the Necklace (-1 ends the list). */
static s32 item_list[] = {
    ITEM_NECKLACE, -1,
};

EVT_BEGIN(clouda_nakama)
    // Mario picks an item to give; cancelling ends the scene with Flurrie lamenting her lost necklace.
    CALL(evt_npc_flag_onoff, 1, STRING("クラウダ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, PTR(item_list), LW(0))
#else
    CALL(evt_win_item_select, 0, PTR(item_list), LW(0), 0)
#endif
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
        CALL(evt_msg_print, 0, STRING("stg2_win_e09_00_01"), 0, 0)
        CALL(evt_party_run, 0)
        CALL(evt_party_run, 1)
        CALL(evt_mario_key_onoff, 1)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    // Flurrie is overjoyed but still won't be seen; Mario nods and sets the necklace down by the door.
    CALL(evt_cam3d_evt_set, 186, 201, 538, 186, 106, -3, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_00"), 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_mov_pos2, 255, LW(2), FLOAT(120.0f))
    CALL(evt_item_entry, STRING("necklace"), ITEM_NECKLACE, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("necklace"), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_item_set_position, STRING("necklace"), 280, 50, 0)
    CALL(evt_mario_dump_item, ITEM_NECKLACE)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 185, -20, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, -1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 1, 210, -25, FLOAT(120.0f))
        CALL(evt_party_set_dir, 1, 90, -1)
    END_INLINE_EVENT
    // Mario steps back, the door opens a crack and the necklace slides inside.
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_mov_pos2, 150, 0, FLOAT(120.0f))
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(mario_move_wait)
#else
    CALL_CHILD(PTR(mario_move_wait))
#endif
    CALL(evt_mario_set_dir, 90, 200, 0)
    SET_VALUE(LW(8), STRING("S_doa_e_l"))
    SET_VALUE(LW(9), STRING("S_doa_e_r"))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 500)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(evt_win_door_open_pure)
#else
    SPAWN_EVENT(PTR(evt_win_door_open_pure))
#endif
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_item_get_position, STRING("necklace"), LW(0), LW(1), LW(2))
        ADD(LW(0), 50)
        CALL(evt_item_move_3d_position, STRING("necklace"), LW(0), LW(1), LW(2), 500, EASING_LINEAR)
        CALL(evt_item_set_position, STRING("necklace"), 0, -1000, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    SET_VALUE(LW(8), STRING("S_doa_e_l"))
    SET_VALUE(LW(9), STRING("S_doa_e_r"))
    SET_VALUE(LW(0), 1)
    SET_VALUE(LW(1), 500)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(evt_win_door_open_pure)
#else
    SPAWN_EVENT(PTR(evt_win_door_open_pure))
#endif
    WAIT_MS(1500)
    // Flurrie rejoices; the chandelier lifts and the room goes dark, puzzling Mario.
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 350, 100, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_01"), 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CRAUD_LIGHT1"), 170, 50, 10, LW(9))
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 100, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_trans, 1, STRING("S_syan"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(9))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 120, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(fade_alpha, 0, 0, 0, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_party_set_dir, 1, 270, -1)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_FOLLOWER, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_mario_set_pose, STRING("M_N_3"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 175, 150, 297, 175, 155, -3, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    // Flurrie's theme starts: the room glitters, the mirror ball lowers and spins, and the light turns pink.
    SET_VALUE(GW(1), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S"), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("hika_"), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_map_blend_set_flag, 1, STRING("mario"), 2048)
        CALL(evt_map_blend_set_flag, 1, STRING("party"), 2048)
        CALL(evt_map_blend_set_flag, 1, STRING("extparty"), 2048)
        CALL(evt_map_blend_set_flag, 1, STRING("クラウダ"), 2048)
        CALL(evt_map_set_blend, 0, 255, 255, 255, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("hika_"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_map_playanim, STRING("kirakira"), MAP_ANIM_PLAY_LOOP, 0)
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 255, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_map_set_blend, 0, 255, 255, 255, LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(mirror_ball_event)
#else
        SPAWN_EVENT(PTR(mirror_ball_event))
#endif
        SPAWN_EVENT_GET_ID(PTR(mirror_rotate), GW(10))
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(mirror_trans)
#else
        CALL_CHILD(PTR(mirror_trans))
#endif
        WAIT_MS(500)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_CLOUDA1"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 1000, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), 1000)
            SET_VALUE(LW(2), 200)
            SET_VALUE(LW(3), 40)
            SET_VALUE(LW(4), 120)
            MULTIPLY_FLOAT(LW(2), LW(0))
            MULTIPLY_FLOAT(LW(3), LW(0))
            MULTIPLY_FLOAT(LW(4), LW(0))
            CALL(fade_alpha, LW(2), LW(3), LW(4), 120)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)
    WAIT_WHILE(GF(0))
    WAIT_MS(1000)
    // The door opens and Flurrie floats out with a wink as rose petals bloom.
    CALL(evt_cam3d_evt_set, 110, 128, 307, 250, 110, 5, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_party_set_dir, 1, 90, -1)
    CALL(evt_npc_set_ry, STRING("クラウダ"), 270)
    CALL(evt_npc_set_position, STRING("クラウダ"), 350, 50, 0)
    WAIT_MS(300)
    SET_VALUE(LW(8), STRING("S_doa_e_l"))
    SET_VALUE(LW(9), STRING("S_doa_e_r"))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 500)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(evt_win_door_open_pure)
#else
    SPAWN_EVENT(PTR(evt_win_door_open_pure))
#endif
    WAIT_MS(1500)
    CALL(evt_cam3d_evt_set, 272, 75, 175, 272, 75, -1, 0, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("クラウダ"), NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_flag_onoff, 0, STRING("クラウダ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_move_position, STRING("クラウダ"), 270, 0, 0, FLOAT(30.0f), 0)
        SET_VALUE(LW(8), STRING("S_doa_e_l"))
        SET_VALUE(LW(9), STRING("S_doa_e_r"))
        SET_VALUE(LW(0), 1)
        SET_VALUE(LW(1), 500)
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_win_door_open_pure)
#else
        SPAWN_EVENT(PTR(evt_win_door_open_pure))
#endif
        WAIT_MS(500)
    END_INLINE_EVENT
    WAIT_MS(2000)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(flower_event)
#else
    SPAWN_EVENT(PTR(flower_event))
#endif
    WAIT_MS(2500)
    CALL(evt_npc_set_anim, STRING("クラウダ"), STRING("PWD_A_7"))
    CALL(evt_npc_get_position, STRING("クラウダ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CRAUD_WINK1"), LW(0), LW(1), LW(2), 0)
    ADD(LW(0), -15)
    ADD(LW(1), 25)
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kiss"), 2, LW(0), LW(1), LW(2), -45, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 100, 0, 1500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_trans, 1, STRING("S_syan"), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)
    // The petals fade, the lights return and the mirror ball goes back up.
    SET_VALUE(GW(0), 1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("hika_"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_pauseanim, 0, STRING("kirakira"))
    CALL(evt_mapobj_trans, 1, STRING("S_mirre"), 0, 100, 0)
    DELETE_EVENT(GW(10))
    CALL(fade_alpha, 0, 0, 0, 0)
    CALL(evt_cam3d_evt_set, 225, 170, 391, 225, 100, -3, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 1)
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), -15)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 90, -1)
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 0)
    END_INLINE_EVENT
    CALL(evt_party_dispflg_onoff, 1, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 1)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    ADD(LW(0), -25)
    CALL(evt_party_move_pos2, 1, LW(0), LW(2), FLOAT(80.0f))
    CALL(evt_party_set_dir, 1, 90, -1)
    CALL(evt_party_dispflg_onoff, 1, PARTY_DISP_FLAG_NO_MAKE_DISP_DIR, 0)
    WAIT_MS(700)
    // Flurrie notices Mario and is smitten with her necklace hunter.
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("クラウダ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 1, STRING("クラウダ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_02"), 0, STRING("クラウダ"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1_2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_G"))
    WAIT_MS(1000)
    // She wants to thank him with a kiss; Mario shakes his head.
    CALL(evt_cam3d_evt_set, 270, 134, 245, 270, 90, -1, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_03"), 0, STRING("クラウダ"))
    CALL(evt_cam3d_evt_set, 150, 134, 245, 150, 90, -1, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NO2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_C"))
    WAIT_MS(1000)
    // Flurrie charges, knocking the partner and Punio aside, and kisses Mario, who keels over.
    CALL(evt_cam3d_evt_set, 270, 134, 245, 270, 90, -1, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_get_position, STRING("クラウダ"), LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_05"), 0, STRING("クラウダ"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_entry, STRING("dummy1"), STRING("c_pguide"))
        CALL(evt_npc_flag_onoff, 1, STRING("dummy1"), NPC_FLAG_NO_HIT)
        CALL(evt_npc_flag_onoff, 1, STRING("dummy1"), NPC_FLAG_NO_DRAW)
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING("dummy1"), LW(0), LW(1), LW(2))
        CALL(evt_party_cont_onoff, 0, 0)
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 30)
        LOOP(0)
            CALL(evt_npc_get_position, STRING("クラウダ"), LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(3), LW(0))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CRAUD_ATTACK1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_party_set_dir, 0, 270, -1)
        CALL(evt_mario_get_party, LW(10))
        SWITCH(LW(10))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_R_1"))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_D_1"))
        END_SWITCH
        CALL(ss_ch_set, 0, 0)
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(dummy1_jump)
#else
        SPAWN_EVENT(PTR(dummy1_jump))
#endif
        LOOP(0)
            CALL(evt_npc_get_position, STRING("dummy1"), LW(0), LW(1), LW(2))
            CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
            CALL(ss_ch_check, 0, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SWITCH(LW(10))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_D_4"))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_D_4"))
        END_SWITCH
        CALL(evt_npc_delete, STRING("dummy1"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_entry, STRING("dummy2"), STRING("c_pguide"))
        CALL(evt_npc_flag_onoff, 1, STRING("dummy2"), NPC_FLAG_NO_HIT)
        CALL(evt_npc_flag_onoff, 1, STRING("dummy2"), NPC_FLAG_NO_DRAW)
        CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
        CALL(evt_npc_set_position, STRING("dummy2"), LW(0), LW(1), LW(2))
        CALL(evt_party_cont_onoff, 1, 0)
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 30)
        LOOP(0)
            CALL(evt_npc_get_position, STRING("クラウダ"), LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(3), LW(0))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_set_dir, 1, 270, -1)
        CALL(evt_party_set_pose, 1, STRING("R_1"))
        CALL(ss_ch_set, 1, 0)
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(dummy2_jump)
#else
        SPAWN_EVENT(PTR(dummy2_jump))
#endif
        LOOP(0)
            CALL(evt_npc_get_position, STRING("dummy2"), LW(0), LW(1), LW(2))
            CALL(evt_party_set_pos, 1, LW(0), LW(1), LW(2))
            CALL(ss_ch_check, 1, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_party_set_pose, 1, STRING("D_2"))
        CALL(evt_npc_delete, STRING("dummy2"))
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 150, 143, 292, 150, 90, -3, 300, EASING_LINEAR)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 5)
    ADD(LW(2), -3)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_jump_position_nohit, STRING("クラウダ"), LW(0), LW(1), LW(2), 500, 1)
    END_INLINE_EVENT
    WAIT_MS(100)
    CALL(evt_npc_set_anim, STRING("クラウダ"), STRING("PWD_A3_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("クラウダ"), STRING("PWD_A3_2"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CRAUD_KISS1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    ADD(LW(0), 5)
    ADD(LW(1), 30)
    CALL(evt_eff, 0, STRING("kiss"), 2, LW(0), LW(1), LW(2), -45, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1700)
    CALL(evt_npc_set_anim, STRING("クラウダ"), STRING("PWD_A3_3"))
    CALL(ss_ch_set, 2, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_DOWN1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_I_S"))
        WAIT_MS(2000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_FALL_CRUSH1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_D_2"))
        WAIT_MS(1000)
        CALL(ss_ch_set, 2, 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, FLOAT(165.19922f), FLOAT(143.0f), FLOAT(292.0f), FLOAT(165.19922f), FLOAT(90.0f), FLOAT(-3.0f), 500, EASING_SINE_IN_OUT)
    CALL(evt_npc_flag_onoff, 0, STRING("クラウダ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_jump_position_nohit, STRING("クラウダ"), 205, 50, 0, 500, 1)
    CALL(evt_npc_set_anim, STRING("クラウダ"), STRING("PWD_S_1"))
    LOOP(0)
        CALL(ss_ch_check, 2, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    // Flurrie gushes over Mario, asks about the Crystal Stars and decides to join to keep him safe.
    CALL(evt_npc_flag_onoff, 1, STRING("クラウダ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_06"), 0, STRING("クラウダ"))
    CALL(evt_mario_set_pose, STRING("M_U_3"))
    WAIT_MS(650)
    CALL(evt_mario_set_pose, STRING("M_N_C"))
    WAIT_MS(400)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_mario_get_party, LW(10))
    SWITCH(LW(10))
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_S_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_S_1"))
    END_SWITCH
    CALL(evt_party_set_dir, 1, 90, -1)
    CALL(evt_party_set_pose, 1, STRING("S_1"))
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_cont_onoff, 1, 1)
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_07"), 0, STRING("クラウダ"))
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 205, 143, 292, 205, 90, -3, 500, EASING_LINEAR)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_07_01"), 0, STRING("クラウダ"))
    CALL(evt_npc_set_anim, STRING("クラウダ"), STRING("PWD_A_7"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(1000)
    WAIT_MS(500)
    // Flurrie joins with the partner lecture, Punio cheers, the story advances and the woods music returns.
    SET_VALUE(LW(8), STRING("クラウダ"))
    SET_VALUE(LW(9), 5)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(all_party_lecture)
#else
    CALL_CHILD(PTR(all_party_lecture))
#endif
    CALL(evt_party_move_pos2, 1, 148, 30, FLOAT(120.0f))
    CALL(evt_msg_print, 0, STRING("stg2_win_e09_09"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_off, 300, EASING_SINE_IN_OUT)
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSWF_WIN_FLURRIE_JOINED, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_09_FLURRIE_JOINS)
#else
    SET_VALUE(GSWF_WIN_FLURRIE_JOINED, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_09_FLURRIE_JOINS)
#endif
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_WIN1"))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(5))
#else
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(5))
#endif
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_WIN2"))
    CALL(evt_snd_env_lpf, 0, 800)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

/**
 * Front-door case script: depending on story progress it runs clouda_inai, clouda_irai, clouda_irai_ato,
 * or (after the Sirens' defeat) either asks for the necklace or starts the joining scene clouda_nakama.
 */
EVT_BEGIN(touch_door)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG2_05_01_FLURRIE_LIVING_ROOM)
        CALL_CHILD(PTR(clouda_inai))
        CASE_EQUAL(STG2_05_01_FLURRIE_LIVING_ROOM)
        CALL_CHILD(PTR(clouda_irai))
        CASE_BETWEEN(STG2_05_02_FLURRIE_REQUEST, STG2_07_MARIO_VS_SHADOW_SIRENS)
        CALL_CHILD(PTR(clouda_irai_ato))
        CASE_EQUAL(STG2_08_SHADOW_SIRENS_LOSE)
        CALL(evt_mario_check_key_item, ITEM_NECKLACE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(clouda_irai_ato))
            ELSE
            CALL_CHILD(PTR(clouda_nakama))
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_exit_case_evt)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Sets map TEV callback 1 to mirror_ball, assigns it to the map object "S_mirre"
 * and turns off its prebuilt display lists so the callback's extra shine stage is used.
 */
EVT_BEGIN(mirror_ball_event)
    CALL(evt_map_set_tevcallback, 1, PTR(mirror_ball))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_mirre"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_mirre"), MAP_OBJ_FLAG_DISPLAY_LIST)
    RETURN
EVT_END()

/* GX_TF_CMPR texel data, 101x92 (GXInitTexObj in the draw function) */
static u32 texture_0[] = {
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x7841AB2C, 0xFFFFDF8B, 0x00000001, 0xFFFFFFFF, 0xF4578103, 0x55C37050, 0x7861CB91, 0xBF2F0289,
    0x00000001, 0xFFFFFFFF, 0x91E79A29, 0xFFFFFFFE, 0x8104EB74, 0xFF800055, 0xEB7480E3, 0xD55CC000,
    0x81038905, 0xFFFFFFAA, 0x70009A08, 0xFFFFFFA9, 0x8104EB75, 0x00555555, 0x8104EB75, 0x00555555,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x70A1E373, 0x0B825856, 0x78009208, 0xFFFF7F2A,
    0x00000001, 0xFFFFFFFF, 0x78009208, 0xFFFDD849, 0x7861CB91, 0xFEF88209, 0x6040EB54, 0x25955555,
    0x8904EB55, 0xFF000055, 0x80E3E333, 0xFF020056, 0xEB34EA54, 0x00022BAD, 0xEAF4E9B3, 0x002AAFF5,
    0x78009208, 0xFFFFA7A1, 0x00000001, 0xFFFFFFFF, 0xFA569907, 0x0D00AAAA, 0x7000B1EB, 0x9F4A6061,
    0x91E79A29, 0xFFFFFFFE, 0x8104AAEB, 0xFFFFFF2F, 0xF45778C3, 0xD55CC000, 0x78A2DC14, 0x0B6F8F2F,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x8945FC57, 0x05C1C1C1, 0xEBB57903, 0x3030D050, 0x80A2EC15, 0xC1E0F0F0, 0xEB7580E3, 0x507040C0,
    0xEB95EB54, 0xAAAAAAAB, 0xEB75EB34, 0x00022AA5, 0xEB75EAF4, 0x000A2BAD, 0xEB14EA13, 0x00022BAD,
    0xEB55EAF4, 0x000AABF5, 0xEB54EAB4, 0x00AABF55, 0xEAB4E932, 0x000AAFF5, 0xEA53E8D2, 0x00AAFF55,
    0xEB135800, 0x0002090F, 0xE3746000, 0xDF608000, 0xFB3788C4, 0xB794D4D4, 0xEB75EB14, 0x0A2BABAD,
    0xEB546941, 0xC0000000, 0xEB34EA13, 0x020A2BAD, 0xEB34EA54, 0x0A2BADB5, 0xEA74E8F2, 0x0A2BADB5,
    0xEA74E912, 0x022BADB5, 0xE993E851, 0x0AAFFDD5, 0xE932E031, 0x0B2DB5D5, 0xE031C80F, 0x0002BD55,
    0xF0729967, 0xAA010101, 0xF4577081, 0xDC7040C0, 0xD00F79C3, 0x070F0F0D, 0xFC57F458, 0xAAAAAAAA,
    0xF45778C2, 0x03010D05, 0x70009A08, 0xBFBF7FFF, 0x7081EC36, 0x62635260, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x80E3E374, 0xE2C1C1C1, 0xEB75EB14, 0x0A2B2BAD, 0x8104EB54, 0xC1C1C1C1, 0xEB54EA94, 0x0B2B2DAD,
    0xEB34EA33, 0x0A2BADB5, 0xEA53E0B2, 0x0A2BADB5, 0xEA74E0F1, 0x0B2B2DB5, 0xF133C82F, 0x2DB5D555,
    0xE933D830, 0x0AAFF555, 0x4BA0E051, 0x55555556, 0xC00D81A3, 0x0000071A, 0x980692E8, 0x38C30C30,
    0xE2B38002, 0xFCF7F7DB, 0xA82B9B07, 0x7AAAC000, 0xC02E89E5, 0xD0000007, 0xC02E81C4, 0x00077FD0,
    0xB80D79A2, 0xFDFE0100, 0xD89189E6, 0x8040D034, 0xC02D79A3, 0x0040FF1E, 0x90058AE7, 0x330AA002,
    0xD850C00F, 0x2DD55555, 0xC82EC00F, 0xFFFFFFFF, 0xC00D89E5, 0x0001DE47, 0x8A059006, 0x2A5A6591,
    0xB00BA38A, 0x0D0B0A0A, 0xFC57F458, 0xAAAAAAAA, 0x90058AE7, 0xCA22E00E, 0xFC576860, 0x0000C070,
    0x8965FC57, 0x50504343, 0x00000001, 0xFFFFFFFF, 0x8945FC57, 0x53434343, 0x78009208, 0xFFFFFF6A,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x81038905, 0xFFFFFFAA, 0x8104AAEB, 0xFFFFFF2F,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x91E79A29, 0xFFFFFFFE,
    0x8103D331, 0xE1F2F0F8, 0xEAF4D0D0, 0x020B090D, 0x78A2DB53, 0xFCF88025, 0xEAF35800, 0xFF7F8000,
    0xE172C00F, 0x2DB55555, 0xC00E89E5, 0x0001060D, 0xE2925840, 0xFEFD0200, 0xEB9488A4, 0x745C5070,
    0xDB526800, 0x95778F0F, 0xC02F82E4, 0xC0000000, 0xE3736000, 0x0F0F0D27, 0x8205A008, 0x5451A696,
    0xB80C81A3, 0x0D1C6CEC, 0xC02F82E4, 0x00000003, 0xEB9578C2, 0x57CD0300, 0xCB127801, 0xF9E6DC9C,
    0xCA538002, 0xFFED9555, 0x91067800, 0x52545501, 0xEB7580E3, 0x550D0103, 0xE3536000, 0x03038976,
    0xB82D81C4, 0x004090A4, 0xB82D79A2, 0x190F0F0F, 0xDB537000, 0x5F5EE303, 0xCB117800, 0xE7F5DD63,
    0xC850C02E, 0xFFFFFFFF, 0xF45790C6, 0x50D0705C, 0xC02F81C3, 0x0040C0C0, 0xDB326800, 0xF79DF3F3,
    0xFC5988A4, 0x15171FDF, 0xEA948904, 0x55000080, 0xE0D16860, 0x70707070, 0xEA53E8F2, 0xE0F87E5E,
    0x8104EB34, 0x00555555, 0xE3536000, 0xDE2D0200, 0xEB14EA33, 0x80E06878, 0xEB75EB34, 0xA0E86868,
    0x7800B3EF, 0xFFBF2F8B, 0x00000001, 0xFFFFFFFF, 0x6020EB94, 0x62585655, 0x7800B3EF, 0xFFBF2F8B,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x91E79A29, 0xFFFFFFFE, 0x6820DB73, 0xF8E28925, 0x7000A20A, 0xF4FAD1E5, 0xEAB4B1CB, 0x4000022A,
    0xEAF4A1EA, 0x40000002, 0xEAB4E952, 0x020AAFB5, 0xEA94E932, 0x0A2BBDF5, 0xE973E871, 0x0B2DB5D5,
    0xE9938124, 0x03010101, 0xEB7480E3, 0x40434D45, 0xE83159E0, 0x03030300, 0xDB527000, 0x765C58F0,
    0x6020EB94, 0x89259555, 0x7060EB74, 0x60585655, 0xEB94EB75, 0xFFFFFFFF, 0xEB94EB75, 0xFFFFFFFF,
    0xEB9578C2, 0x00C070D0, 0xEB9578C2, 0x14370D03, 0xEB748103, 0x1C340705, 0xEB94EB75, 0xFFFFFFFF,
    0xEB757903, 0x0000C040, 0xDB526800, 0xD8F0DFD6, 0xDB526800, 0xE09F3536, 0x7061EB74, 0x09259555,
    0xEB9578C2, 0x35D77000, 0xEB756941, 0xC3000000, 0xEB94EB75, 0xFFFFFFFF, 0xEB757923, 0x0000000D,
    0xA32AA008, 0x65F5B62D, 0xEB747923, 0x73C04040, 0x7040E394, 0x50628925, 0xEB756941, 0xC0000000,
    0xE0308124, 0xD0505074, 0xE973E871, 0x78785E5E, 0xA0A8EB75, 0x40404040, 0xE8B2E031, 0x785C5E5E,
    0xEAB4E972, 0xE0E07878, 0xEB54EAF4, 0xE0E07078, 0xEA33E8D2, 0xE0787878, 0xEB34EA94, 0xE0707878,
    0xEB95EB54, 0xEAEAEAEA, 0x7081E374, 0x43625856, 0xEB95EB54, 0xFA7A7A7A, 0xEB94EB75, 0xFFFFFFFF,
    0x7800B3EF, 0xFFFFBF2F, 0x00000001, 0xFFFFFFFF, 0x6860E394, 0x8B625856, 0x00000001, 0xFFFFFFFF,
    0x78009208, 0xFFFFF6D1, 0x8104EAF4, 0xFF800055, 0x80E4E353, 0xC1C2F0FF, 0x68C2E2F3, 0x556982F8,
    0xE2325860, 0xB0608000, 0xE9F3E8B2, 0x022BBDD5, 0xF1B490C6, 0x020A0B55, 0xD830A209, 0x80000040,
    0xE952E851, 0x0BAFF555, 0xE811E071, 0x7AE8A8A0, 0xE831D810, 0x02AAEA5F, 0xE811E011, 0xAAAAAAFF,
    0xE811D810, 0x0002082D, 0xEB748103, 0x507040C0, 0x53E0D810, 0x56565555, 0xEB148104, 0x40407050,
    0xEB95EB54, 0xAAAAAAFE, 0xEB94EB75, 0xFFFFFF5F, 0xEB54EAD4, 0x0080EA7F, 0xEB75EB34, 0x00A0FA5E,
    0xEB7578C2, 0x0D010300, 0xE3536000, 0x80C062DD, 0xEB95EB54, 0xAAAAEA7F, 0xEB758104, 0xD5000000,
    0xE3735800, 0x98F060C0, 0xEB94EB75, 0xFFFFFFFF, 0x6084E394, 0x09825856, 0x4084EB75, 0x55559526,
    0x5840EB94, 0x55565862, 0xEB5478E3, 0xD570C000, 0xEAF47945, 0x1CD07040, 0xEAF4E9D3, 0x002AAFD5,
    0xEB346962, 0xC0000000, 0xEB75EB34, 0x00A8FA57, 0xEAD4E9B3, 0x00AAFF55, 0xEB14E9F3, 0x00A0FA5E,
    0x98C8EB75, 0x40404040, 0xE831C80F, 0xE0606070, 0xBB2F980A, 0x3D3D39B5, 0xE031B80D, 0xE0E0E0E0,
    0xE9D3E891, 0x78787878, 0xEB34EA33, 0x68787878, 0xE9B3E871, 0x78787878, 0xEB14EA33, 0x78787878,
    0xEB75EB34, 0xA0E0E8E0, 0xEB94EB75, 0xFFFFFFFF, 0xEB75EB34, 0x686868E0, 0xEB94EB75, 0xFFFFFFFF,
    0x78E3E374, 0x5850525B, 0x00000001, 0xFFFFFFFF, 0x78C3DB72, 0x63438B0F, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x78009208, 0x40D2F6FD, 0xF45778E3, 0x57350370, 0x00000001, 0xFFFFFFFF, 0x78A2E415, 0x85C9C280,
    0xB80CA36A, 0x00C0AC73, 0xE011C80F, 0x827D5555, 0xFC576860, 0x0D030000, 0xB00B9B68, 0xC080EA7B,
    0xD00FA82D, 0x2AAAAAAA, 0xC00DA1EA, 0xF57B361C, 0xEC75B04B, 0x5555F503, 0xB80C7180, 0x0D0FFAFF,
    0xEAB4D8B0, 0x0080EA7A, 0xEB14EA33, 0x00A0FA5F, 0xC00EA1E8, 0x0B0240B4, 0xE9F3C82F, 0x00E05E55,
    0xEB54EAD4, 0x00A8FE57, 0xEB75EB14, 0x88AAFE57, 0xEA74E0F1, 0x00A8EA5F, 0xEAD4E9D3, 0x00A8FF55,
    0xEB75EB34, 0x00A2EA55, 0xE3545022, 0x9D2F0000, 0xEB14EA13, 0x00AAFF55, 0xEB14EA13, 0x00AAFF55,
    0xF3158105, 0x4A5E3503, 0xD02FC9EF, 0x7FEA4010, 0xEB14EA13, 0x00A8FE57, 0xC2CE9808, 0xB5393D26,
    0xF9B3D030, 0xAAFF5555, 0xD9F1C82F, 0x80E87C57, 0xC82EC00F, 0xFFFFFFFF, 0xA80982E6, 0x0C0A33CC,
    0xC00E69C4, 0xF0D0C0C0, 0xE872C80F, 0x7A787878, 0x9208C00E, 0x15555555, 0xF133D02F, 0x5E5E7878,
    0xE9F3E892, 0x787878E0, 0xEB34EA54, 0x787878E8, 0xEA94E972, 0x5E7878E0, 0xEB54EAB4, 0x7878E8A0,
    0xEB75EB54, 0x78686868, 0xEB7578E2, 0x00030D05, 0xEB95EB54, 0x7AFAFAEA, 0x80E3E374, 0x50506243,
    0x8104AAEB, 0x2F3FBFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x78009208, 0xFFFDFEFE, 0xEB748104, 0x71414340, 0x78009208, 0xF4FAFAFA, 0xEB756941, 0xC0000000,
    0xF45778C2, 0xC0405CD5, 0xFC5760C0, 0x000000C0, 0xEAF47923, 0x0D000000, 0xFBD99085, 0x5CD7BDBD,
    0xFC57F458, 0xAAAAAAAA, 0xFC57F458, 0xAAAAAAAA, 0xF45778C2, 0x000070D7, 0xFC57F458, 0xAAAAAAAA,
    0xF4578103, 0xD5030000, 0xF4569888, 0xD55D3503, 0xFC57F458, 0xAAAAAAAA, 0xFC57F458, 0xAAAAAAAA,
    0xC00E91E7, 0x02004090, 0xE953C80F, 0x00FA5755, 0xF4578083, 0xDD370D00, 0xB80E89C4, 0x0000F47D,
    0xE993D830, 0x00AAFF55, 0xE973E051, 0x00AAFF55, 0xC00E81C4, 0x000000F4, 0xD030C00E, 0xA0FF5755,
    0xE993E051, 0x00AAFF55, 0xE9936080, 0x0F0F0FAD, 0x63E0C80F, 0x55555556, 0xC00E78E2, 0x0705055F,
    0xB80C89E6, 0x010619F4, 0xC00E91E6, 0x90400000, 0xC00E79C2, 0x400000FF, 0xC82F6160, 0x000000FF,
    0xE0D1C80F, 0x5557565C, 0xEA33E091, 0x5E7AE8E0, 0xE1F2D850, 0x5E7AE000, 0xEB14E9F3, 0x5E7AE8A0,
    0xEB14EA13, 0x78E8E080, 0xEB75EB14, 0x78E8A8A0, 0xEB347923, 0x0000000D, 0xEB7480E3, 0x00003557,
    0x6020EB74, 0x55555658, 0x78A2D371, 0x4B8F2FBF, 0x7060D392, 0x628B2FFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x70009A08, 0xFAF6FEFE, 0xEB757903, 0x0000C040, 0x91E79A29, 0xFEFFFFFF, 0x7080E374, 0x152589E2,
    0xEB54EAD4, 0x2D2D0B0B, 0xEA94D04F, 0x2D0B0A02, 0xEB75EB34, 0xAD2B0A02, 0xEB34EA54, 0xAD2B0A02,
    0xC82FA1E8, 0x1E0180E8, 0xF4569887, 0x70557555, 0xEAB4E132, 0xB5AD2B0A, 0xE973D00F, 0xD5BD2B0A,
    0xF45788E5, 0x00C05575, 0x8104FC57, 0x55550000, 0xE872C80F, 0x55D5AF2A, 0xE011B80D, 0xFFFFAA00,
    0xF45798C7, 0x003555D5, 0xF45778C2, 0x00C057D5, 0xE011B80D, 0xFFFFAA00, 0xE011C00E, 0x5455FD2B,
    0xF45780E3, 0xD50D00D5, 0x8B068804, 0x957FD55F, 0xB80D79A2, 0x1E03060F, 0xFC5778A1, 0x7040C000,
    0xF45780C3, 0xD55C4055, 0xFC578104, 0x550000C0, 0xDC939066, 0xD515DD75, 0xC00E71A1, 0xFF1F0000,
    0x8104FC57, 0x00555555, 0x8104FC57, 0x00555555, 0xF456A0A8, 0xC0557555, 0x98A8FC57, 0x55000000,
    0xFC5780E3, 0x55D50000, 0xF4577020, 0xEFF50D35, 0xFBFAA0A7, 0x35557F5F, 0xEAB48904, 0x55C08080,
    0x78009208, 0xA002072F, 0x7800B3EF, 0x2FBFFFFF, 0x8104E334, 0x0F4B4343, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x7800B3F0, 0xF8FEFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x78C3DB73, 0x2509E2FC, 0xEB546164, 0x000000C0, 0x8A099209, 0xFEFFFFFF, 0xEB757925, 0x40C07050,
    0xEB34EA94, 0xB52D0B02, 0xEA94E952, 0xB52D2B0A, 0xEB75EB34, 0xBD2B0A02, 0xEB34EA94, 0xB52D0A00,
    0xE993E871, 0xD5B52F0A, 0xE8F2E831, 0x55D5BF0A, 0xEAD4E9B3, 0xF5AF0A00, 0xEA94E952, 0x55FFAA00,
    0xE831E0F2, 0x0000AAFF, 0xE932E031, 0x555FFAA0, 0xE9B38144, 0x00000035, 0xD9F17020, 0x022D9557,
    0xF45690A6, 0x7754DCD0, 0xF4777842, 0x030D37DD, 0x78009208, 0x0A022A7E, 0xB0CBE293, 0x206A5595,
    0xC82F89E5, 0xC0400000, 0xE811C80F, 0x7FEA8000, 0xEA13E0B1, 0x55BD2F0A, 0xE952E831, 0x55FFAA00,
    0xE831D810, 0x55FFAAA0, 0xE8F2E031, 0x55577EF8, 0xE993E871, 0x555FFAA0, 0xEA33E8D2, 0x577EE880,
    0xEA33E8B2, 0x5E7AF8E0, 0xEB34EA54, 0x7878E8E0, 0xF2948905, 0xA08000D5, 0xEB335800, 0x000203BD,
    0x8103E333, 0x43434B8F, 0x00000001, 0xFFFFFFFF, 0x78009208, 0xAF1FBFBF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x70A1E374, 0x85C9E1F9, 0x00000001, 0xFFFFFFFF, 0x91E79A29, 0xFAFEFFFF,
    0xEB756941, 0x00000003, 0xEB545800, 0x00002FDF, 0x78C2DB72, 0x5802AFFF, 0x8104AAEB, 0x2FFFFFFF,
    0x78C2EB14, 0x5555020B, 0x78E2E2F3, 0x566080FF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x78009208, 0x800A9FFF, 0x78009208, 0x277FFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x91E79A29, 0xFEFFFFFF, 0x78E3D2F1, 0x09E0F8FF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0xE2725840, 0x00807FBF, 0x8104EA13, 0x55550000, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x8104EA13, 0x55550000, 0x7000B20C, 0x56602A7F, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x78009208, 0x0200AAFF, 0x78A2E373, 0x609400E0, 0x00000001, 0xFFFFFFFF, 0x91E79A29, 0xFEFFFFFF,
    0x78009208, 0xBFBF1F2F, 0x00000001, 0xFFFFFFFF, 0x78009209, 0x2F5FFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0xFFFFFFFF, 0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

/**
 * Rose petals of Flurrie's entrance: the screen position (x, y) and final size (z) of each of the 55 flowers
 * that evt_flower grows in flower_event.
 */
Vec rose_table_direct[] = {
    {-196.61f, 223.94f, 0.78f},
    {-141.56f, 219.43f, 0.76f},
    {-73.39f, 236.15f, 0.7f},
    {-21.96f, 242.71f, 0.7f},
    {87.9f, 241.39f, 0.98f},
    {133.93f, 223.66f, 0.84f},
    {173.11f, 215.57f, 0.78f},
    {-119.22f, -213.95f, 0.85f},
    {-71.22f, -227.94f, 0.82f},
    {-23.4f, -216.1f, 0.75f},
    {29.64f, -222.83f, 0.76f},
    {127.21f, -233.98f, 0.81f},
    {192.32f, -207.1f, 0.72f},
    {-195.15f, -179.61f, 0.96f},
    {-84.81f, -169.72f, 0.94f},
    {-25.53f, -155.86f, 0.79f},
    {32.81f, -174.25f, 0.74f},
    {86.01f, -155.31f, 0.82f},
    {188.71f, -157.01f, 0.84f},
    {-225.99f, 232.6f, 0.71f},
    {-254.44f, 95.68f, 0.75f},
    {-246.33f, 35.15f, 0.92f},
    {-245.52f, -40.32f, 0.87f},
    {-229.69f, -92.09f, 0.84f},
    {-243.1f, -214.08f, 0.95f},
    {241.17f, 215.65f, 0.95f},
    {253.63f, 153.26f, 0.93f},
    {242.99f, 105.34f, 0.73f},
    {252.56f, 23.3f, 0.86f},
    {242.58f, -86.44f, 0.84f},
    {239.01f, -156.48f, 0.84f},
    {247.68f, -207.16f, 0.73f},
    {-280.63f, 164.94f, 0.74f},
    {-284.82f, 93.31f, 0.98f},
    {-283.27f, -18.97f, 0.87f},
    {-268.18f, -95.94f, 0.73f},
    {-270.89f, -145.05f, 0.94f},
    {-292.4f, -226.47f, 0.84f},
    {275.47f, 237.61f, 0.93f},
    {288.59f, 156.75f, 0.9f},
    {288.65f, 109.74f, 0.99f},
    {282.48f, -12.99f, 0.7f},
    {268.08f, -77.21f, 0.73f},
    {268.48f, -141.41f, 0.94f},
    {-187.86f, 269.48f, 0.78f},
    {-180.2f, -245.88f, 0.72f},
    {192.61f, -266.0f, 0.92f},
    {-175.99f, 186.6f, 0.8f},
    {-202.86f, 146.75f, 1.0f},
    {189.17f, 178.65f, 0.95f},
    {210.63f, 120.26f, 0.93f},
    {235.63f, 80.26f, 0.8f},
    {-170.63f, -120.26f, 0.93f},
    {189.63f, -100.26f, 0.9f},
    {173.63f, -125.26f, 0.72f},
};

/**
 * Another layout of rose petals: 60 (x, y) screen positions in rows along the edges of the screen.
 * Nothing reads it; the petals use rose_table_direct.
 */
f32 rose_table[] = {
    -186.66667f, 230.0f, -133.33334f, 230.0f, -80.0f, 230.0f, -26.666672f, 230.0f,
    26.666656f, 230.0f, 80.0f, 230.0f, 133.33334f, 230.0f, 186.66666f, 230.0f,
    -186.66667f, -220.0f, -133.33334f, -220.0f, -80.0f, -220.0f, -26.666672f, -220.0f,
    26.666656f, -220.0f, 80.0f, -220.0f, 133.33334f, -220.0f, 186.66666f, -220.0f,
    -186.66667f, -170.0f, -133.33334f, -170.0f, -80.0f, -170.0f, -26.666672f, -170.0f,
    26.666656f, -170.0f, 80.0f, -170.0f, 133.33334f, -170.0f, 186.66666f, -170.0f,
    -240.0f, 230.0f, -240.0f, 165.7143f, -240.0f, 101.42857f, -240.0f, 37.142853f,
    -240.0f, -27.142853f, -240.0f, -91.42856f, -240.0f, -155.7143f, -240.0f, -220.0f,
    240.0f, 230.0f, 240.0f, 165.7143f, 240.0f, 101.42857f, 240.0f, 37.142853f,
    240.0f, -27.142853f, 240.0f, -91.42856f, 240.0f, -155.7143f, 240.0f, -220.0f,
    -280.0f, 230.0f, -280.0f, 165.7143f, -280.0f, 101.42857f, -280.0f, 37.142853f,
    -280.0f, -27.142853f, -280.0f, -91.42856f, -280.0f, -155.7143f, -280.0f, -220.0f,
    280.0f, 230.0f, 280.0f, 165.7143f, 280.0f, 101.42857f, 280.0f, 37.142853f,
    280.0f, -27.142853f, 280.0f, -91.42856f, 280.0f, -155.7143f, 280.0f, -220.0f,
    -190.0f, 270.0f, -190.0f, -260.0f, 190.0f, 270.0f, 190.0f, -260.0f,
};

/**
 * Rose-petal effect: allocates 55 flowers, ticks them until GW(0) is set to 1, then fades them out;
 * spawned during Flurrie's joining scene.
 */
EVT_BEGIN(flower_event)
    SET_VALUE(GW(0), 0)
    CALL(evt_flower_init)
    LOOP(0)
        CALL(evt_flower)
        IF_INT_EQUAL(GW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_flower_end)
    LOOP(0)
        CALL(evt_flower_end_main)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(flower_disp_pos)
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(evt_key_get_button, 0, LW(0))
        AND_RAW(LW(0), 256)
        IF_INT_NOT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(flower_pos_report)
    RETURN
EVT_END()

void mirror_ball(void* param) {
    envAddTev(10, param);
}

/**
 * 2D draw callback for the falling rose petals:
 * blends each of the 55 flowers' texture at its position, scale and alpha.
 */
void flower_disp(CameraId camera, void* param) {
    // rose_table_direct stores screen x/y and target scale in Vec.z, not depth.
    FlowerWork* wp = param;
    CameraEntry* cam = camGetPtr(camera);
    GXTexObj tex;
    Mtx trans;
    Mtx scale;
    Mtx mtx;
    f32 w;
    f32 h;
    Flower* fl;
    s32 i;

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXInitTexObj(&tex, texture_0, 101, 92, GX_TF_CMPR, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    w = 101.0f;
    h = 92.0f;
    for (i = 0, fl = wp->flowers; i < wp->count; i++, fl++) {
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, fl->alpha});
        PSMTXTrans(trans, fl->x, fl->y, 0.0f);
        PSMTXScale(scale, fl->scale, fl->scale, fl->scale);
        PSMTXConcat(cam->view, trans, mtx);
        PSMTXConcat(mtx, scale, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-w / 2, h / 2, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(-w / 2, -h / 2, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXPosition3f32(w / 2, -h / 2, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(w / 2, h / 2, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXEnd();
    }
}

/**
 * Second part of Flurrie's intro (after win_03's cloud_toujou), at STG2_03_02:
 * with Mario off the map, the camera looks over the room while a voice without a speaker tail
 * enjoys the peace of the woods (stg2_win_e04_00_02), then, pointed at (300, 80, 40),
 * admits she will soon crave drama (stg2_win_e04_00_03); the map changes to win_05 with an iris wipe.
 */
static EVT_BEGIN(cloud_toujou)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 75, 205, 539, 75, 110, -3, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg2_win_e04_00_02"), 0, 0)
    CALL(evt_cam3d_evt_set, 60, 120, 352, 276, 120, 15, 0, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 300, 80, 40)
    CALL(evt_msg_print, 0, STRING("stg2_win_e04_00_03"), 0, 0)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 1, 500)
    CALL(evt_bero_mapchange, STRING("win_05"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(win_04_init_evt)
#if VERSION > VERSION_DEMO_US
    SET_VALUE(GSWF_WIN_04_ENTERED, 1)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_bero_info_run)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
#endif
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_WIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_WIN2"))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(5))
#else
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(5))
#endif
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -125, 60, -95, 0, 0)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_04_EXTENDING_PLANE_PANEL)
        SPAWN_EVENT(meet_clouda)
#else
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_04_EXTENDING_PLANE_PANEL)
        SPAWN_EVENT(PTR(meet_clouda))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_05_A_SHY_FLURRIE)
        SPAWN_EVENT(meet_clouda2)
#else
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_05_A_SHY_FLURRIE)
        SPAWN_EVENT(PTR(meet_clouda2))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_05_02_FLURRIE_REQUEST)
#else
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_05_02_FLURRIE_REQUEST)
#endif
        SET_VALUE(LW(0), STRING("w_bero"))
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(bero_case_switch_off)
#else
        CALL_CHILD(PTR(bero_case_switch_off))
#endif
        CALL(evt_bero_overwrite, STRING("w_bero"), PTR(bero_witchtrio))
        SET_VALUE(LW(0), STRING("w_bero"))
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(bero_case_switch_on)
#else
        CALL_CHILD(PTR(bero_case_switch_on))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG2_08_SHADOW_SIRENS_LOSE)
#else
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG2_08_SHADOW_SIRENS_LOSE)
#endif
        SET_VALUE(LW(0), STRING("e_bero"))
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(bero_case_switch_off)
#else
        CALL_CHILD(PTR(bero_case_switch_off))
#endif
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(touch_door), 0)
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_03_02_SEALED_ENTRANCE_2)
        SPAWN_EVENT(cloud_toujou)
#else
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_03_02_SEALED_ENTRANCE_2)
        SPAWN_EVENT(PTR(cloud_toujou))
#endif
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hika_"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return (max - min) * r + min;
}

/** Script helper: allocates the rose-petal work area (55 flowers) and stores it in LW(10). */
USER_FUNC(evt_flower_init) {
    FlowerWork* wp = _mapAlloc(sizeof(FlowerWork));

    wp->count = 55;
    wp->flowers = _mapAlloc(wp->count * sizeof(Flower));
    event->lwData[10] = (EvtWord)wp;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: ticks every flower: each starts
 * after a random delay (0 to 99 frames) at its rose_table_direct position,
 * grows by 0.08 a frame to its target scale while fading in by 20 alpha a frame, and all are drawn in 2D.
 */
USER_FUNC(evt_flower) {
    FlowerWork* wp = (FlowerWork*)event->lwData[10];
    Flower* fl = wp->flowers;
    s32 i;

    for (i = 0; i < wp->count; i++, fl++) {
        switch (fl->state) {
            case 0:
                fl->timer = rand() % 100;
                fl->state++;
                break;
            case 1:
                if (--fl->timer < 0) {
                    fl->x = rose_table_direct[i].x;
                    fl->y = rose_table_direct[i].y;
                    fl->targetScale = rose_table_direct[i].z;
                    fl->scale = 0.0f;
                    fl->state++;
                }
                break;
            case 2:
                if (fl->scale < fl->targetScale) {
                    fl->scale += 0.08f;
                } else {
                    fl->timer = 0;
                    fl->state++;
                }
                fl->alpha += 20;
                if (fl->alpha > 255) {
                    fl->alpha = 255;
                }
                break;
        }
    }
    dispEntry(CAMERA_2D, 1000.0f, 0, flower_disp, wp);
    return EVT_RETURN_DONE;
}

/** Script helper: resets all flowers' states so they start fading. */
USER_FUNC(evt_flower_end) {
    FlowerWork* wp = (FlowerWork*)event->lwData[10];
    Flower* fl = wp->flowers;
    s32 i;

    for (i = 0; i < wp->count; i++, fl++) {
        fl->state = 0;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: fades the flowers' alpha by 10 per frame until they are gone, drawing them each frame. */
USER_FUNC(evt_flower_end_main) {
    FlowerWork* wp = (FlowerWork*)event->lwData[10];
    Flower* fl = wp->flowers;
    s32 i;

    for (i = 0; i < wp->count; i++, fl++) {
        switch (fl->state) {
            case 0:
                fl->alpha -= 10;
                if (fl->alpha < 0) {
                    fl->alpha = 0;
                    fl->state++;
                }
                break;
            case 1:
                break;
        }
    }
    dispEntry(CAMERA_2D, 1000.0f, 0, flower_disp, wp);
    return EVT_RETURN_DONE;
}

/** Loops over the flowers doing nothing; only flower_disp_pos calls it. Guess: its debug output was compiled out. */
USER_FUNC(flower_pos_report) {
    FlowerWork* wp = (FlowerWork*)event->lwData[10];
    s32 i;

    for (i = 0; i < wp->count; i++) {
    }
    return EVT_RETURN_DONE;
}

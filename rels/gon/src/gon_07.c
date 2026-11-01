// gon_07 (ゴンババ, Hooktail Castle): room beyond gon_06 with a treasure chest holding the Black Key (0x22) (story 43).
// Has a danger-music sequence and a door to gon_13.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static EvtWord door01_close[12];
static EvtWord door01_open[12];
EvtWord out_func[14];
static BeroEntry bero_entry_data[3];
static EvtWord shake_evt[150];
static EvtWord item_evt[268];
static EvtWord bomb_wall_evt[179];
EvtWord gon_07_init_evt[86];

/**
 * Door animation callback, closing for the w_bero exit to gon_06:
 * rotates map object "door01" about Y by 0.444 times the value in LW(0).
 */
static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the w_bero exit to gon_06:
 * rotates map object "door01" about Y by -0.444 times the value in LW(0).
 */
static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Run when Mario leaves the room during the escape countdown:
 * ends the countdown timer (evt_sub_countdown_end) and, at STG1_13_A_SUSPICIOUS_ROOM,
 * stops the ceiling's rumble sound GW(10).
 */
EVT_BEGIN(out_func)
    CALL(evt_sub_countdown_end)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_13_A_SUSPICIOUS_ROOM)
        CALL(evt_snd_sfxoff, GW(10))
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Exits (beros) of this room: w_bero to gon_06 and e_bero to gon_13. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        12, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        out_func, // exitEvent
        "gon_06", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_13", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Rumble sequence after the Black Key is taken:
 * a chain of camera shakes (strength 0.0049, 1000 ms each) whose gaps shrink from 2000 ms to 600 ms.
 */
static EVT_BEGIN(shake_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(2000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(2000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(2000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1200)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1200)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1200)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(1200)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
    WAIT_MS(600)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 0)
    RETURN
EVT_END()

static EVT_BEGIN(item_evt)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_13_A_SUSPICIOUS_ROOM)
        RETURN
    END_IF
    // Story advances and the chest is marked opened; the room music stops.
    SET_VALUE(GSW_STORY_PROGRESS, STG1_13_A_SUSPICIOUS_ROOM)
    SET_VALUE(GSWF_GON_07_BLACK_KEY_CHEST, 1)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_key_onoff, 0)
    // The Black Key pops out of the chest and Mario takes it.
    CALL(evt_mobj_get_position, STRING("gon07treasure"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("k_slit"), ITEM_BLACK_KEY_0x22, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("k_slit"), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_mobj_wait_animation_end, STRING("gon07treasure"))
    CALL(evt_item_get_item, STRING("k_slit"))
    // A rumbling earthquake starts shaking the room.
    CALL(evt_snd_sfxon, STRING("SFX_STG1_QUAKE4"), GW(10))
    SPAWN_EVENT_GET_ID(PTR(shake_evt), LW(10))
    WAIT_MS(500)
    // Mario and his partner step to the middle of the room.
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 20, 35, FLOAT(60.0f))
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -20, 35, FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, 300)
    END_INLINE_EVENT
    // The camera frames the room and the floor spikes rise from below.
    CALL(evt_cam3d_evt_set, 0, 124, 408, 0, 57, 21, 500, EASING_SINE_IN_OUT)
    WAIT_MS(400)
    CALL(evt_snd_sfxon, STRING("SFX_STG1_TOGE_UP1"), 0)
    WAIT_MS(100)
    CALL(evt_hitobj_onoff, STRING("a_togeshita"), 1, 1)
    CALL(evt_map_playanim, STRING("a_to_shita"), MAP_ANIM_PLAY_ONCE, 0)
    LABEL(0)
    CALL(evt_map_checkanim, STRING("a_to_shita"), LW(0), LW(1))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(2)
    CALL(evt_sub_rumble_onoff, 1, 0)
    WAIT_MS(1000)
    // The partner reacts to the trap.
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg1_gon_15"))
    // The danger music starts and the ceiling spikes begin to move.
    CALL(evt_map_playanim, STRING("a_to_ue"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER1"))
    CALL(evt_cam3d_evt_set_at, 0, 300, 0, 0, 900, EASING_SINE_IN_OUT)
    WAIT_MS(2600)
    CALL(evt_cam3d_evt_set, 0, 124, 408, 0, 57, 21, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_map_playanim, STRING("a_to_ue"), MAP_ANIM_PLAY_ONCE, 1)
    CALL(evt_map_pauseanim, 0, STRING("a_to_ue"))
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    // The partner warns that the ceiling is coming down.
    CALL(evt_msg_print_party, STRING("stg1_gon_16"))
    CALL(evt_map_replayanim, 0, STRING("a_to_ue"))
    // The ceiling closes in and a countdown starts; if Mario does not escape through a door in time, the game
    // is over.
    SPAWN_EVENT_GET_ID(PTR(shake_evt), LW(10))
    CALL(evt_map_checkanim, STRING("a_to_ue"), LW(0), LW(1))
    // Use the ceiling animation's remaining time for both countdown arguments.
    CALL(evt_sub_countdown_start, LW(1), LW(1))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_bero_exec_get, LW(0))
            IF_BITS_SET(LW(0), 1)
                DEBUG_PUT_MSG(STRING("脱出成功"))
                BREAK_LOOP
            END_IF
            CALL(evt_sub_countdown_get_status, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                SET_VALUE(LW(0), STRING("w_bero"))
                CALL_CHILD(PTR(bero_case_switch_off))
                CALL(evt_seq_set_seq, SEQ_GAME_OVER, 0, 0)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    // Mario escaped (or the loop ended): control returns and the partner follows.
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    BEGIN_INLINE_EVENT
        LABEL(1)
        CALL(evt_map_checkanim, STRING("a_to_ue"), LW(0), LW(1))
        IF_INT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(1)
        END_IF
        CALL(evt_snd_sfxoff, GW(10))
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Bobbery's bomb wall break (S_ana): plays SFX_OFF_WALL_BREAK1,
 * captures the wall hole into a paper image (OFF_d_bom_ver2) and plays its crumbling animation A_1
 * so the wall breaks apart, and swaps the S_ana hide flags to show the opened hole (S_ana_after).
 */
static EVT_BEGIN(bomb_wall_evt)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ana"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_ana"), STRING("ofs"))
    CALL(evt_offscreen_get_boundingbox2, STRING("ofs"), LW(0), LW(1), LW(2), LW(3))
#if VERSION > VERSION_DEMO_US
    // Skip the capture animation only when both bounding-box dimensions are zero.
#endif
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(2), 0)
        IF_INT_EQUAL(LW(3), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("ofs"), 1, LW(0), LW(1), LW(2), LW(3))
        WAIT_FRAMES(1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_img_clear_virtual_point, STRING("img"))
        CALL(evt_img_onoff, STRING("img"), 1)
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
        CALL(evt_img_wait_animend, STRING("img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
#if VERSION <= VERSION_DEMO_US
CALL(evt_hitobj_onoff, STRING("san"), 1, 0)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_on)
#endif
    CALL(evt_img_release, STRING("img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_ana"))
    CALL(evt_paper_delete, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_delete, STRING("ofs"))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
#if VERSION <= VERSION_DEMO_US
SET_VALUE(GSWF_GON_07_WALL_BOMBED, 1)
#else
    CALL(evt_hitobj_onoff, STRING("san"), 1, 0)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSWF_GON_07_WALL_BOMBED, 1)
#endif
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Map init for gon_07: sets up the door, the treasure chest holding the Black Key (already open once the story
 * reaches STG1_13_A_SUSPICIOUS_ROOM; opening it drops the spiked ceiling and starts the escape countdown,
 * and the story changes to that step), and the cracked wall that Bobbery's bomb can blow up (bomb_wall_evt),
 * which stays open afterwards.
 */
EVT_BEGIN(gon_07_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_13_A_SUSPICIOUS_ROOM)
        SET_VALUE(GSWF_GON_07_BLACK_KEY_CHEST, 1)
    END_IF
    CALL(evt_mobj_itembox, STRING("gon07treasure"), 0, 0, 0, MOBJ_ITEMBOX_NORMAL, 0, PTR(item_evt), GSWF_GON_07_BLACK_KEY_CHEST)
    CALL(evt_hitobj_onoff, STRING("a_togeshita"), 1, 0)
    IF_INT_EQUAL(GSWF_GON_07_WALL_BOMBED, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("san"), 0, PTR(bomb_wall_evt), 0)
        ELSE
        CALL(evt_hitobj_onoff, STRING("san"), 1, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()


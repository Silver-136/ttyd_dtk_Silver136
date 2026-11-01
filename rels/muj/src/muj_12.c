// muj_12 (無人島, Keelhaul Key): Cortez's lair: Cortez appears, the scenes after his battle and
// the bargain with him (korutesu_event_after, kosho_event), and the Crystal Star.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "evt/evt_fade.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "mario/mario.h"

#include "driver/itemdrv.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "dolphin/os.h"
#include "driver/fadedrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "muj.h"
#include "driver/seqdrv.h"

static USER_FUNC(evtflash);
USER_FUNC(muj_item_move_3d_position);
static f32 accdec_type(s32 step, s32 total, f32 start, f32 end);

static EvtWord back_dark[27];
static EvtWord door_custom[53];
static EvtWord bero_custom[135];
static EvtWord w_close[12];
static EvtWord w_open[12];
static BeroEntry bero_entry_data[2];
EvtWord korutesu_yure[86];
EvtWord korutesu_flush[24];
EvtWord korutesu_flush2[36];
EvtWord korutesu_rotate[28];
EvtWord korutesu_small[56];
EvtWord korutesu_small2[56];
EvtWord korutesu_rotate2[28];
EvtWord korutesu_large[56];
EvtWord korutesu_blur[43];
static EvtWord starstone_fuwafuwa[53];
#if VERSION <= VERSION_DEMO_US
static EvtWord korutesu_event[616];
#else
static EvtWord korutesu_event[627];
#endif
EvtWord korutesu_event_after[140];
#if VERSION <= VERSION_DEMO_US
extern EvtWord kosho_event[];
#else
EvtWord kosho_event[517];
#endif
EvtWord coin_effect[151];
EvtWord muj_12_init_evt[60];

/** White fade-out colour of the screen flash in the Cortez scenes. */
static const GXColor evtflash_col1 = {0xFF, 0xFF, 0xFF, 0xFF};
/** White fade-in colour of the screen flash in the Cortez scenes. */
static const GXColor evtflash_col2 = {0xFF, 0xFF, 0xFF, 0xFF};

/**
 * Darkens the room: fades map blend colour 0 from white to black over LW(10) ms;
 * korutesu_event uses it (600 ms) before the closing message.
 */
static EVT_BEGIN(back_dark)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, LW(10))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Copy of the engine's door enter animation type 5 (bero_play_enter_anime) with a longer wait:
 * if the entry has an enter hook (LW(12)), runs it at angle 180 (door open), lets Mario walk in,
 * waits 1.5 s instead of 0.5 s so the partners can follow,
 * plays the door-closing sound and swings the door shut (angle 180 to 0 over 500 ms).
 */
static EVT_BEGIN(door_custom)
    CALL(evt_bero_get_info_anime, LW(1), LW(10), LW(11), LW(12), LW(13))
    IF_INT_NOT_EQUAL(LW(12), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(180.0f))
        CALL_CHILD(LW(12))
        CALL(evt_bero_mario_go)
        WAIT_MS(1500)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        SET_VALUE(LW(9), LW(12))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
            IF_INT_EQUAL(LW(10), 0)
                SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
                CALL_CHILD(LW(9))
                BREAK_LOOP
                ELSE
                CALL_CHILD(LW(9))
            END_IF
        END_LOOP
    END_IF
    RETURN
EVT_END()

/**
 * Enter event of the lair door: at STG5_32_CORTEZS_WARNING (Mario and partner)
 * and STG5_40_TRUE_IDENTITY (Mario and both partners) it runs door_custom, walks them in from the door,
 * waits 2 s and returns LW(0) = 1 so the normal walk-in is skipped (evt_bero_mario_enter);
 * otherwise it returns LW(0) = 0.
 */
static EVT_BEGIN(bero_custom)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_32_CORTEZS_WARNING)
        SPAWN_EVENT(PTR(door_custom))
        CALL(evt_mario_normalize)
        CALL(evt_party_stop, 0)
        CALL(evt_party_stop, 1)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_set_pos, -218, 0, 0)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_mario_mov_pos2, -100, 0, FLOAT(120.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_pos, 0, -230, 0, 20)
            CALL(evt_party_set_dir, 0, 90, 0)
            CALL(evt_party_move_pos2, 0, -130, 20, FLOAT(120.0f))
        END_INLINE_EVENT
        WAIT_MS(2000)
        SET_VALUE(LW(0), 1)
        RETURN
        CASE_EQUAL(STG5_40_TRUE_IDENTITY)
        SPAWN_EVENT(PTR(door_custom))
        CALL(evt_mario_normalize)
        CALL(evt_party_stop, 0)
        CALL(evt_party_stop, 1)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_set_pos, -218, 0, 0)
            CALL(evt_mario_set_dir, 90, 0, 0)
            CALL(evt_mario_mov_pos2, -90, 0, FLOAT(120.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_pos, 0, -240, 0, -25)
            CALL(evt_party_set_dir, 0, 90, 0)
            CALL(evt_party_move_pos2, 0, -110, -25, FLOAT(120.0f))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_party_set_pos, 1, -220, 0, 25)
            CALL(evt_party_set_dir, 1, 90, 0)
            CALL(evt_party_move_pos2, 1, -120, 25, FLOAT(120.0f))
        END_INLINE_EVENT
        WAIT_MS(2000)
        SET_VALUE(LW(0), 1)
        RETURN
    END_SWITCH
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/**
 * Enter hook of the lair door (w_bero, enter anime type 5): turns S_doa to LW(0) x 0.610 degrees;
 * the door code feeds 180 down to 0, so the door stands 110 degrees open and swings shut.
 */
static EVT_BEGIN(w_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.61035156f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Exit hook of the lair door: turns S_doa to LW(0) x -0.610 degrees;
 * fed 0 up to 180, it swings 110 degrees open the other way as Mario leaves.
 */
static EVT_BEGIN(w_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.61035156f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Doors and exits of muj_12: w_bero to muj_11. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        2, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_custom, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_11", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
    {0},
};

/** Shakes the camera and plays a ship-shaking sound at Cortez until GF(0) is cleared, to show him thrashing. */
EVT_BEGIN(korutesu_yure)
    CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SHIP_SHAKE2"), LW(0), LW(1), LW(2), LW(10))
    LOOP(0)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 300)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0087890625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0078125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0068359375f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.005859375f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.00390625f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0029296875f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.001953125f), FLOAT(0.0f), 300)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0009765625f), FLOAT(0.0f), 300)
    CALL(evt_snd_sfxoff, LW(10))
    RETURN
EVT_END()

/** Flashes the screen (evtflash) at random intervals while GF(0) is set, as Cortez shows his power. */
EVT_BEGIN(korutesu_flush)
    LOOP(0)
        CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
        CALL(evtflash)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_sub_random, 500, LW(10))
        WAIT_MS(800)
        WAIT_MS(LW(10))
    END_LOOP
    RETURN
EVT_END()

/** A shorter burst of screen flashes with random pauses (one second at most), used in the fight with Cortez. */
EVT_BEGIN(korutesu_flush2)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    CALL(evt_sub_random, 500, LW(10))
    WAIT_MS(800)
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    CALL(evt_sub_random, 1000, LW(10))
    WAIT_MS(1600)
    WAIT_MS(LW(10))
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    RETURN
EVT_END()

/** Spins Cortez (NPC "コルテス") around his Y axis while he shrinks. */
EVT_BEGIN(korutesu_rotate)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 1800, 2700)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_rotate, STRING("コルテス"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Shrinks Cortez while he glides to a new position over 2.7 seconds, scaling him by LW(2). */
EVT_BEGIN(korutesu_small)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_npc_glide_position, STRING("コルテス"), LW(0), LW(1), LW(2), 2700, 0, 0, 11, 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 1000, 100, 2700)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(LW(2), LW(0))
        DIVIDE_FLOAT(LW(2), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("コルテス"), LW(2), LW(2), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Second shrinking glide for Cortez, also 2.7 seconds. */
EVT_BEGIN(korutesu_small2)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_npc_glide_position, STRING("コルテス"), LW(0), LW(1), LW(2), 2700, 0, 0, 11, 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 1000, 0, 2700)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(LW(2), LW(0))
        DIVIDE_FLOAT(LW(2), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("コルテス"), LW(2), LW(2), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Spins Cortez around his Y axis while he grows again (second rotation). */
EVT_BEGIN(korutesu_rotate2)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 1800, 0, 1300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_rotate, STRING("コルテス"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Makes Cortez glide to a new position and grow (scale LW(2)) over 1.3 seconds. */
EVT_BEGIN(korutesu_large)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
        ADD(LW(1), -30)
        CALL(evt_npc_glide_position, STRING("コルテス"), LW(0), LW(1), LW(2), 1300, 0, 0, 11, 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 100, 1000, 1300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(LW(2), LW(0))
        DIVIDE_FLOAT(LW(2), FLOAT(1000.0f))
        CALL(evt_npc_set_scale, STRING("コルテス"), LW(2), LW(2), LW(2))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Camera blur loop: switches the environment blur on and alternates between two camera positions every 100 ms
 * until GF(0) is cleared.
 */
EVT_BEGIN(korutesu_blur)
    LOOP(0)
        BEGIN_INLINE_EVENT
            CALL(evt_env_blur_on, 1, 200)
        END_INLINE_EVENT
        CALL(evt_cam3d_evt_set, -203, 72, 359, 0, 94, 75, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, -175, 75, 320, 0, 94, 75, 100, EASING_SINE_IN_OUT)
        WAIT_MS(100)
        IF_INT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GF(1), 0)
    RETURN
EVT_END()

/**
 * Bobbing for a gliding item: forever ramps GW(1) from 0 to 5 and back, 500 ms each way;
 * korutesu_event and kosho_event pass GW(1) as the height offset of muj_item_move_3d_position so the item bobs
 * while it moves, then delete this event.
 */
static EVT_BEGIN(starstone_fuwafuwa)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 500, 500)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 500, 0, 500)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(korutesu_event)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S"), MAP_OBJ_FLAG_CLIP)
    // Mario enters Cortez's lair and control is taken.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("コルテス"), 46, 35, 25)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_NO_VIEW_CORRECTION | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_ry, STRING("コルテス"), 0)
    CALL(evt_npc_pera_onoff, STRING("コルテス"), 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        WAIT_MS(500)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    // Ghostly moaning fills the lair.
    CALL(evt_msg_print, 0, STRING("stg5_dou_21"), 0, 0)
    CALL(evt_cam3d_evt_set, -300, 60, 444, -25, 90, 57, 750, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KOR_APPEAR_LIGHT1"), 0)
    CALL(evtflash)
    SET_VALUE(GF(0), 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG5_KORTESS1"))
    SPAWN_EVENT(PTR(korutesu_flush))
    CALL_CHILD(PTR(korutesu_apper))
    SET_VALUE(GF(0), 1)
    // Cortez appears and says he has had enough.
    CALL(evt_msg_print, 0, STRING("stg5_dou_22"), 0, STRING("コルテス"))
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 1, 500)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -175, 75, 320, 0, 94, 75, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_npc_set_autotalkpose, STRING("コルテス"), STRING("KRT_S_1"), STRING("KRT_T_3"))
    WAIT_FRAMES(1)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    SPAWN_EVENT(PTR(korutesu_blur))
    CALL(evt_snd_bgmon, PSND_FADEOUT | PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED3"))
    // Cortez rages at the intruders.
    CALL(evt_msg_print, 0, STRING("stg5_dou_23"), 0, STRING("コルテス"))
    CALL(evt_npc_set_autotalkpose, STRING("コルテス"), STRING("KRT_S_1"), STRING("KRT_T_2"))
    SET_VALUE(GF(0), 0)
    WAIT_WHILE(GF(1))
    CALL(evt_cam3d_evt_set, -40, 90, 542, -40, 80, -8, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_24"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -40, 94, 791, -40, 80, -8, 500, EASING_SINE_IN_OUT)
    WAIT_MS(250)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_npc_set_battle_info, STRING("コルテス"), 4)
    // The battle with Cortez starts.
    CALL(evt_npc_battle_start, STRING("コルテス"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_cam3d_evt_set, -80, 30, 368, 44, 90, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pos, -65, 0, 0)
    CALL(evt_party_set_pos, 0, -100, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    WAIT_MS(1000)
    SET_VALUE(GF(0), 0)
    SPAWN_EVENT(PTR(korutesu_yure))
    SPAWN_EVENT(PTR(korutesu_flush2))
    CALL(evt_npc_set_autotalkpose, STRING("コルテス"), STRING("KRT_S_1"), STRING("KRT_T_3"))
    WAIT_FRAMES(1)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    // After the battle Cortez cries out in defeat.
    CALL(evt_msg_print, 0, STRING("stg5_dou_25"), 0, STRING("コルテス"))
    CALL(evt_npc_set_autotalkpose, STRING("コルテス"), STRING("KRT_S_1"), STRING("KRT_T_2"))
    SET_VALUE(GF(0), 1)
    WAIT_MS(2000)
    CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_KOR_HEAD_MOVE1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 32000)
        LOOP(60)
            SUBTRACT(LW(0), 516)
            CALL(evt_snd_bgm_freq, 0, LW(0))
            WAIT_MS(50)
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(korutesu_rotate))
    SPAWN_EVENT(PTR(korutesu_small))
    WAIT_MS(5000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_26"), 0, STRING("コルテス"))
    CALL(evt_npc_get_position, STRING("コルテス"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_KOR_HEAD_MOVE2"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(30)
            ADD(LW(0), 1066)
            CALL(evt_snd_bgm_freq, 0, LW(0))
            WAIT_MS(50)
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(korutesu_rotate2))
    SPAWN_EVENT(PTR(korutesu_large))
    WAIT_MS(1500)
    // Cortez laughs, then notices Mario is not after his treasure.
    CALL(evt_msg_print, 0, STRING("stg5_dou_27"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -20, 125, 505, -20, 80, 7, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_28"), 0, STRING("コルテス"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_28a"), 0, STRING("コルテス"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 40, 125, 505, 40, 80, 7, 700, EASING_SINE_IN_OUT)
    CALL(evt_npc_glide_position, STRING("コルテス"), 86, 51, 112, 0, FLOAT(120.0f), 0, 11, 0)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_28b"), 0, STRING("コルテス"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_28c"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -20, 125, 505, -20, 80, 7, 700, EASING_SINE_IN_OUT)
    CALL(evt_npc_glide_position, STRING("コルテス"), 46, 35, 35, 0, FLOAT(120.0f), 0, 11, 0)
    WAIT_MS(1000)
    // Cortez gives up the stone and it floats over to Mario.
    CALL(evt_msg_print, 0, STRING("stg5_dou_29"), 0, STRING("コルテス"))
    WAIT_MS(1000)
    SET_VALUE(LW(10), STRING("stone"))
    CALL(evt_item_flag_on, LW(10), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 50)
    CALL(evt_item_get_position, LW(10), LW(11), LW(12), LW(13))
    ADD(LW(12), 20)
    CALL(evt_item_move_3d_position, LW(10), LW(11), LW(12), LW(13), 500, EASING_LINEAR)
    CALL(evt_cam3d_evt_set, LW(0), 125, 505, LW(0), 80, 7, 1500, EASING_SINE_IN_OUT)
    SPAWN_EVENT_GET_ID(PTR(starstone_fuwafuwa), LW(9))
    CALL(muj_item_move_3d_position, LW(10), LW(0), LW(1), LW(2), 3000, EASING_LINEAR, GW(1))
    DELETE_EVENT(LW(9))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
    SET_VALUE(LW(0), 5)
    SET_VALUE(LW(1), LW(10))
    // The story advances and the Crystal Star is awarded.
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
CALL_CHILD(evt_sub_starstone)
#else
    SET_VALUE(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL_CHILD(PTR(evt_sub_starstone))
#endif
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ9"))
#endif
    RETURN
EVT_END()

EVT_BEGIN(korutesu_event_after)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -65, 0, 0)
    CALL(evt_party_set_pos, 0, -100, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_position, STRING("コルテス"), 46, 35, 25)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_NO_VIEW_CORRECTION | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, STRING("コルテス"), 0)
    CALL(evt_npc_pera_onoff, STRING("コルテス"), 0)
    CALL(evt_cam3d_evt_set, -20, 125, 505, -20, 80, 7, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_kabe_r"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_fade_end_wait)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 70, 800)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(2), 0)
        SUBTRACT(LW(2), LW(0))
        CALL(evt_mapobj_rotate, 1, STRING("S_t_doa02"), 0, LW(2), 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_t_doa03"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_30"), 0, STRING("コルテス"))
    WAIT_MS(1000)
    DELETE_EVENT(GW(0))
    SET_VALUE(LW(10), 600)
    CALL_CHILD(PTR(back_dark))
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_39"), 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
    CALL(evt_bero_mapchange, STRING("muj_10"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(kosho_event)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("コルテス"), 46, 35, 25)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_NO_VIEW_CORRECTION | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("コルテス"), NPC_FLAG_NO_DRAW)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_cam3d_evt_set, 20, 157, 337, 20, 90, -6, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_STG5_KORTESS1"))
    // Cortez appears, moaning, and Flavio shakes with fear.
    CALL_CHILD(PTR(korutesu_apper))
    CALL(evt_msg_print, 0, STRING("stg5_dou_31"), 0, STRING("コルテス"))
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_MARCO_SHAKE1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_party_set_pose, 1, STRING("O_1"))
    CALL(evt_cam3d_evt_set, -25, 125, 505, -25, 80, 7, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    ADD(LW(2), 10)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("stg5_dou_32"), 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_33"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
    // Flavio nervously introduces himself.
    CALL(evt_msg_print, 0, STRING("stg5_dou_34"), 0, 0)
    CALL(evt_cam3d_evt_set, -25, 125, 505, -25, 80, 7, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_35"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, EASING_SINE_IN_OUT)
    CALL(evt_party_set_pose, 1, STRING("S_1"))
    CALL(evt_snd_sfxoff, LW(9))
    WAIT_MS(500)
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_POS, STRING(""), 0, 0, 0, LW(0), LW(1), LW(2), 50, 0, 60)
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_NPC_BIKKURI_MARK1"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1000)
    // Flavio brings out the Skull Gem.
    CALL(evt_msg_print, 0, STRING("stg5_dou_36"), 0, STRING("extparty"))
    CALL(evt_item_entry, STRING("jewel"), ITEM_SKULL_GEM, -115, 0, 30, ITEM_TYPE_STILL, -1, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 20, 157, 337, 20, 90, -6, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_01"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -25, 125, 505, -25, 80, 7, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_02"), 0, STRING("extparty"))
    WAIT_MS(500)
    // Cortez and Flavio argue over who stole what.
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_03"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_04"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_set, 20, 157, 337, 20, 90, -6, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_05"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_36_06"), 0, STRING("extparty"))
    SET_VALUE(GF(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_item_get_position, STRING("jewel"), LW(0), LW(1), LW(2))
        // The Skull Gem floats toward Cortez.
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_JUWEL_MOVE1"), LW(0), LW(1), LW(2), LW(3))
        LOOP(0)
            CALL(evt_item_get_position, STRING("jewel"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(3), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(GF(2), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_snd_sfxoff, LW(3))
    END_INLINE_EVENT
    CALL(evt_item_move_3d_position, STRING("jewel"), -115, 70, 30, 1000, PTR(accdec_type))
    CALL(evt_cam3d_evt_set, 20, 157, 337, 20, 90, -6, 3000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        WAIT_MS(2500)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_item_set_alpha, STRING("jewel"), LW(0))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT_GET_ID(PTR(starstone_fuwafuwa), LW(10))
    CALL(muj_item_move_3d_position, STRING("jewel"), 40, 70, 30, 3000, EASING_LINEAR, GW(1))
    SET_VALUE(GF(2), 1)
    DELETE_EVENT(LW(10))
    WAIT_MS(500)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_item_set_position, STRING("jewel"), 0, -1000, 0)
#else
    CALL(evt_item_delete, STRING("jewel"))
#endif
    WAIT_MS(500)
    // Cortez has his Black Skull back and Flavio asks for thanks.
    CALL(evt_msg_print, 0, STRING("stg5_dou_37"), 0, STRING("コルテス"))
    CALL(evt_cam3d_evt_set, -120, 107, 337, -120, 40, -6, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_38"), 0, STRING("extparty"))
    CALL(evt_cam3d_evt_set, -45, 160, 534, -45, 55, -6, 0, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    WAIT_MS(300)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
    CALL(evt_snd_bgmon_f, PSND_CONTINUE | PSND_FADEIN, 0, 2000)
    // Cortez vanishes.
    CALL_CHILD(PTR(korutesu_vanish))
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("コルテス"), 0, -1000, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    // The story advances and control returns.
    SET_VALUE(GSW_STORY_PROGRESS, STG5_41_RETURN_TO_THE_GROTTO)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Loop that makes Cortez's treasure pile glitter:
 * picks one of ten spots at random and spawns a hagetaka flush effect there.
 */
EVT_BEGIN(coin_effect)
    LOOP(0)
        CALL(evt_sub_random, 10, LW(10))
        SWITCH(LW(10))
            CASE_EQUAL(0)
            SET_VALUE(LW(0), -44)
            SET_VALUE(LW(1), 20)
            SET_VALUE(LW(2), 138)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), -8)
            SET_VALUE(LW(1), 30)
            SET_VALUE(LW(2), 146)
            CASE_EQUAL(2)
            SET_VALUE(LW(0), 26)
            SET_VALUE(LW(1), 20)
            SET_VALUE(LW(2), 143)
            CASE_EQUAL(3)
            SET_VALUE(LW(0), 78)
            SET_VALUE(LW(1), 28)
            SET_VALUE(LW(2), 147)
            CASE_EQUAL(4)
            SET_VALUE(LW(0), 91)
            SET_VALUE(LW(1), 30)
            SET_VALUE(LW(2), -62)
            CASE_EQUAL(5)
            SET_VALUE(LW(0), 49)
            SET_VALUE(LW(1), 30)
            SET_VALUE(LW(2), -82)
            CASE_EQUAL(6)
            SET_VALUE(LW(0), -28)
            SET_VALUE(LW(1), 40)
            SET_VALUE(LW(2), -82)
            CASE_EQUAL(7)
            SET_VALUE(LW(0), -67)
            SET_VALUE(LW(1), 15)
            SET_VALUE(LW(2), -50)
            CASE_EQUAL(8)
            SET_VALUE(LW(0), -11)
            SET_VALUE(LW(1), 35)
            SET_VALUE(LW(2), 98)
            CASE_EQUAL(9)
            SET_VALUE(LW(0), 25)
            SET_VALUE(LW(1), 30)
            SET_VALUE(LW(2), 133)
            CASE_EQUAL(10)
            SET_VALUE(LW(0), 82)
            SET_VALUE(LW(1), 10)
            SET_VALUE(LW(2), 148)
        END_SWITCH
        CALL(evt_eff64, STRING(""), STRING("hagetaka_flush_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
    END_LOOP
    RETURN
EVT_END()

/**
 * Map init for muj_12 (Cortez's lair): registers the exits, picks music,
 * spawns the right scene for the story stage (korutesu_event, korutesu_event_after or kosho_event)
 * and starts the treasure glitter.
 */
EVT_BEGIN(muj_12_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG5_GHOST1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ9"))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_32_CORTEZS_WARNING)
        SPAWN_EVENT(PTR(korutesu_event))
        CALL(evt_item_entry, STRING("stone"), ITEM_SAPPHIRE_STAR, 63, 38, 138, ITEM_TYPE_STILL, -1, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
        SPAWN_EVENT(PTR(korutesu_event_after))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_40_TRUE_IDENTITY)
        SPAWN_EVENT(PTR(kosho_event))
    END_IF
    SPAWN_EVENT_GET_ID(PTR(coin_effect), GW(0))
    RETURN
EVT_END()

// Requires 0 <= step < total and a nonzero accumulated speed.
static f32 accdec_type(s32 step, s32 total, f32 start, f32 end) {
    f32 cur;
    f32 speed;
    f32 sum;
    s32 accel = total * 20 / 100;
    s32 decel = total * 80 / 100;
    s32 i;

    speed = 0.0f;
    sum = 0.0f;

    for (i = 0; i < total; i++) {
        if (i <= accel) {
            speed += 1.0f;
        } else if (i >= decel) {
            speed -= 1.0f;
        }
        sum += speed;
        if (i == step) {
            cur = sum;
        }
    }
    return start + cur * (end - start) / sum;
}

/**
 * Script helper: moves the named item to the position (args[1], args[2], args[3]) over args[4] ms using
 * interpolation mode args[5], adding args[6] to its height; blocks until it arrives.
 */
USER_FUNC(muj_item_move_3d_position) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    f32 x = evtGetValue(event, args[1]);
    f32 y = evtGetValue(event, args[2]);
    f32 z = evtGetValue(event, args[3]);
    s32 frames = evtGetValue(event, args[4]);
    s32 mode = evtGetValue(event, args[5]);
    f32 yOffset = evtGetFloat(event, args[6]);
    ItemEntry* item = itemNameToPtr(name);
    s32 elapsed;

    if (isFirstCall) {
        if (item == NULL) {
            return EVT_RETURN_DONE;
        }
        event->savedTime = gp->renderTime;
        event->userdataf[1] = item->position.x;
        event->userdataf[2] = item->position.y;
        event->userdata3f = item->position.z;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime);
    if (elapsed < frames) {
        // FAKE: reuse the expired first-call parameter; direct elapsed arguments change the object.
        isFirstCall = elapsed;
        item->position.x = intplGetValue(mode, event->userdataf[1], x, isFirstCall, frames);
        item->position.y = intplGetValue(mode, event->userdataf[2], y, isFirstCall, frames);
        item->position.z = intplGetValue(mode, event->userdata3f, z, isFirstCall, frames);
        item->position.y += yOffset;
        return EVT_RETURN_BLOCK;
    }
    item->position.x = x;
    item->position.y = y;
    item->position.z = z;
    return EVT_RETURN_DONE;
}

/**
 * Script helper that flashes the screen white: fades out in 20 frames with colour evtflash_col1 (white),
 * holds 50 ms, then fades back in over 80 frames with evtflash_col2. Blocks until finished.
 */
static USER_FUNC(evtflash) {
    if (isFirstCall) {
        event->userdata[2] = 0;
    }
    switch (event->userdata[2]) {
        case 0:
            fadeEntry(FADE_OUT_BLACK, 20, evtflash_col1);
            event->userdata[2]++;
        case 1:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[0] = 50;
            event->savedTime = (s32)gp->renderTime;
            event->userdata[2]++;
            if (event->userdata[0] == 0) {
                event->userdata[2]++;
            }
            return EVT_RETURN_BLOCK;
        case 2:
            if (OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) < (u32)event->userdata[0]) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[2]++;
            return EVT_RETURN_BLOCK;
        case 3:
            fadeEntry(FADE_IN_BLACK, 80, evtflash_col2);
            event->userdata[2]++;
        case 4:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

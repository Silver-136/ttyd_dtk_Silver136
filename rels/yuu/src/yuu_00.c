// yuu_00 (遊技場, Pianta Parlor): minigame 1, Plane Mode (hikoki); entered from gor_03.

#include "version.h"
#pragma defer_codegen on
#include "battle/battle_unit.h"
#include "evt/evt_yuuranking.h"
#include "evt/evt_cmd.h"
#include "party.h"
#include "cam_shift.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_sub.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_yuugijou.h"
#include "rels/yuu/include/yuu.h"
#include "driver/mapdrv.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "string.h"
#include "mario/mario_motion.h"
#include "memory.h"
#include "driver/dispdrv.h"
#include "pmario_sound.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "driver/icondrv.h"
#include "evt/evt_movefloor.h"
#include "yuu.h"
#include "driver/seqdrv.h"
#include "icon_ids.h"
#include <dolphin/pad.h>

typedef struct HikokiWork {
    u32 flags;
    s32 phase;
    s32 coins;
    s32 result;
    s32 monteCount;
    EventEntry* viewEvent;
} HikokiWork;
#if VERSION >= VERSION_US
USER_FUNC(yuuminigame1_floor_chk);
USER_FUNC(yuuminigame1_floor_on_off);
#endif
typedef struct MonteTable {
    f32 x0;
    f32 x1;
    f32 y0;
    f32 y1;
    s32 num;
} MonteTable;
static void* yuunpc_wp;
static Monte* monte_p;
static void monte_disp(CameraId camera, void* param);

USER_FUNC(evt_change_plane);
static USER_FUNC(fall_rtn);
static USER_FUNC(re_fall_rtn);
USER_FUNC(yuu_00_cam_shift_reset);
USER_FUNC(yuuminigame1_opening_chk);
USER_FUNC(yuuminigame1_opening_on_off);
USER_FUNC(yuuminigame1_set_phase);
BOOL yuuminigame1_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, HikokiWork* data);

static MonteTable montetable[4];
static EvtWord minigame_view_monte[4];
EvtWord yuu_00_kaijo_init[12];
EvtWord yuu_00_kaijo_init2[12];
EvtWord light_sub_event[497];
EvtWord minigame01_main[35];
EvtWord minigame01_start_cam[40];
EvtWord minigame01_start_cam_cancel[20];
#if VERSION >= VERSION_US
EvtWord minigame01_start[1504];
EvtWord minigame01_end[224];
EvtWord yuu_00_init_evt[437];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord minigame01_start[348];
EvtWord minigame01_end[227];
#else
EvtWord minigame01_start[355];
EvtWord minigame01_end[236];
#endif
EvtWord yuu_00_init_evt[11];
#endif

static MonteTable montetable[] = {
#if VERSION <= VERSION_DEMO_US
    {-1200.0f, -800.0f, 450.0f, 400.0f, 2},
    {-800.0f, -300.0f, 400.0f, 300.0f, 2},
    {-300.0f, 200.0f, 300.0f, 150.0f, 1},
#else
    {-1200.0f, -800.0f, 450.0f, 200.0f, 2},
    {-800.0f, -300.0f, 300.0f, 100.0f, 2},
    {-300.0f, 200.0f, 200.0f, 100.0f, 1},
#endif
    {0},
};

/** The Plane Mode game's coin event (data->viewEvent): runs fall_rtn, which scatters and collects the coins. */
static EVT_BEGIN(minigame_view_monte)
    CALL(fall_rtn)
    RETURN
EVT_END()

#if VERSION >= VERSION_US
/**
 * Deletes the five moving floors (platforms) of the Plane Mode course once, guarded by the floor-exists flag;
 * runs when Mario touches a platform or the game ends. US and later only.
 */
EVT_BEGIN(minigame01_floor_delete)
    CALL(yuuminigame1_floor_chk, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(yuuminigame1_floor_on_off, LW(0), 0)
        CALL(yuuminigame_get_dataadr, 1, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
        CALL(yuuminigame_get_dataadr, 2, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
        CALL(yuuminigame_get_dataadr, 3, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
        CALL(yuuminigame_get_dataadr, 4, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
        CALL(yuuminigame_get_dataadr, 5, LW(15))
        CALL(evt_moving_floor_delete, LW(15))
    END_IF
    RETURN
EVT_END()
#endif

/** Puts Mario on the starting spot of the Plane Mode stage (x -1550, y 400) facing right. */
EVT_BEGIN(yuu_00_kaijo_init)
    CALL(evt_mario_set_pos, -1550, 400, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

/** Puts Mario back at the stage spot used after the opening camera or between rounds (x -1350, y 400) facing right. */
EVT_BEGIN(yuu_00_kaijo_init2)
    CALL(evt_mario_set_pos, -1350, 400, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    RETURN
EVT_END()

/**
 * Spins up the stage spotlights ("light02" and "light_c") in a background inline event;
 * the rotation speeds up in steps and stops when GF(0) is cleared.
 */
EVT_BEGIN(light_sub_event)
    SET_VALUE(GF(0), 1)

    // The first pair of spotlights (light01, light_b) sweeps back and forth until GF(0) is cleared.
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 96)
        LOOP(0)
            WAIT_FRAMES(1)
            IF_INT_NOT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(0), 30)
                SET_VALUE(LW(1), 1)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 35)
            END_IF
            IF_INT_EQUAL(LW(0), 60)
                SET_VALUE(LW(1), 2)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 35)
            END_IF
            IF_INT_EQUAL(LW(0), 78)
                SET_VALUE(LW(1), 3)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            END_IF
            IF_INT_EQUAL(LW(0), 96)
                SET_VALUE(LW(0), 0)
                SET_VALUE(LW(1), 0)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            END_IF
            SWITCH(LW(1))
                CASE_EQUAL(0)
                SET_VALUE(LW(3), 35)
                SUBTRACT(LW(3), LW(5))
                MULTIPLY(LW(3), LW(2))
                DIVIDE(LW(3), 30)
                ADD(LW(3), LW(5))
                CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
                CASE_EQUAL(1)
                SET_VALUE(LW(3), 35)
                CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
                CASE_EQUAL(2)
                SET_VALUE(LW(3), 0)
                SUBTRACT(LW(3), LW(5))
                MULTIPLY(LW(3), LW(2))
                DIVIDE(LW(3), 18)
                ADD(LW(3), LW(5))
                CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
                CASE_EQUAL(3)
                SET_VALUE(LW(3), 0)
                CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
            END_SWITCH
            ADD(LW(0), 1)
            ADD(LW(2), 1)
        END_LOOP

        // Once the show stops, the lights swing back to rest.
        WAIT_MS(300)
        SET_VALUE(LW(2), 0)
        SET_VALUE(LW(5), LW(3))
        LOOP(0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(2), 18)
                BREAK_LOOP
            END_IF
            SET_VALUE(LW(3), 0)
            SUBTRACT(LW(3), LW(5))
            MULTIPLY(LW(3), LW(2))
            DIVIDE(LW(3), 18)
            ADD(LW(3), LW(5))
            CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
            CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
            ADD(LW(2), 1)
        END_LOOP
        SET_VALUE(LW(3), 0)
        CALL(evt_mapobj_rotate, 1, STRING("light01"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("light_b"), 0, 0, LW(3))
    END_INLINE_EVENT

    // The second pair (light02, light_c) does the same, 30 frames out of step with the first.
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 30)
        LOOP(0)
            WAIT_FRAMES(1)
            IF_INT_NOT_EQUAL(GF(0), 1)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(0), 30)
                SET_VALUE(LW(1), 1)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 35)
            END_IF
            IF_INT_EQUAL(LW(0), 60)
                SET_VALUE(LW(1), 2)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 35)
            END_IF
            IF_INT_EQUAL(LW(0), 78)
                SET_VALUE(LW(1), 3)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            END_IF
            IF_INT_EQUAL(LW(0), 96)
                SET_VALUE(LW(0), 0)
                SET_VALUE(LW(1), 0)
                SET_VALUE(LW(2), 0)
                SET_VALUE(LW(5), 0)
            END_IF
            SWITCH(LW(1))
                CASE_EQUAL(0)
                SET_VALUE(LW(3), 35)
                SUBTRACT(LW(3), LW(5))
                MULTIPLY(LW(3), LW(2))
                DIVIDE(LW(3), 30)
                ADD(LW(3), LW(5))
                CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
                CASE_EQUAL(1)
                SET_VALUE(LW(3), 35)
                CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
                CASE_EQUAL(2)
                SET_VALUE(LW(3), 0)
                SUBTRACT(LW(3), LW(5))
                MULTIPLY(LW(3), LW(2))
                DIVIDE(LW(3), 18)
                ADD(LW(3), LW(5))
                CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
                CASE_EQUAL(3)
                SET_VALUE(LW(3), 0)
                CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
                CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
            END_SWITCH
            ADD(LW(0), 1)
            ADD(LW(2), 1)
        END_LOOP
        WAIT_MS(300)
        SET_VALUE(LW(2), 0)
        SET_VALUE(LW(5), LW(3))
        LOOP(0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(2), 18)
                BREAK_LOOP
            END_IF
            SET_VALUE(LW(3), 0)
            SUBTRACT(LW(3), LW(5))
            MULTIPLY(LW(3), LW(2))
            DIVIDE(LW(3), 18)
            ADD(LW(3), LW(5))
            CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
            CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
            ADD(LW(2), 1)
        END_LOOP
        SET_VALUE(LW(3), 0)
        CALL(evt_mapobj_rotate, 1, STRING("light02"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("light_c"), 0, 0, LW(3))
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Set-up for minigame 1 (Plane Mode): freezes Mario's controls, allocates the minigame work area,
 * dismisses the partners, starts the spotlights and the crowd sound, and sets phase 1.
 */
EVT_BEGIN(minigame01_main)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(yuu_00_kaijo_init))
    CALL(evt_yuuminigame_alloc, LW(0), 0)
    CALL(yuuminigame_set_dataadr, 0, LW(0))
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_kill_party, 1)
    CALL_CHILD(PTR(light_sub_event))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(yuuminigame_get_dataadr, 0, LW(0))
    CALL(yuuminigame1_set_phase, LW(0), 1)
    RETURN
EVT_END()

/**
 * Opening cutscene of Plane Mode: pans the 3D camera across the course,
 * then shows the "menu_title_minigame1" title card for a few seconds.
 */
EVT_BEGIN(minigame01_start_cam)
    CALL(evt_cam3d_evt_set, FLOAT(-1230.0f), FLOAT(360.0f), FLOAT(1140.0f), FLOAT(-1230.0f), FLOAT(160.0f), FLOAT(-10.0f), 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, SEQ_GAME)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_cam3d_evt_set, FLOAT(700.0f), FLOAT(360.0f), FLOAT(1140.0f), FLOAT(700.0f), FLOAT(160.0f), FLOAT(-10.0f), 4000, EASING_LINEAR)
#else
    CALL(evt_cam3d_evt_set, FLOAT(700.0f), FLOAT(360.0f), FLOAT(1140.0f), FLOAT(700.0f), FLOAT(160.0f), FLOAT(-10.0f), 6000, EASING_LINEAR)
#endif
    WAIT_MS(1000)
    CALL(evt_yuutitle_alloc, 0, 0, STRING("menu_title_minigame1"))
#if VERSION > VERSION_DEMO_US
    WAIT_MS(3000)
    CALL(evt_yuutitle_free)
#endif
    WAIT_MS(2000)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_yuutitle_free)
    WAIT_MS(1000)
#endif
    RETURN
EVT_END()

/**
 * Cleans up when the player skips the opening camera:
 * frees the title card, turns off the event camera and puts Mario back at the start spot.
 */
EVT_BEGIN(minigame01_start_cam_cancel)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_yuutitle_free)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_wait_move_end)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL_CHILD(PTR(yuu_00_kaijo_init2))
    RETURN
EVT_END()

/**
 * Phase 1 of Plane Mode: builds the five floating platforms (S_dai_01 to S_dai_05;
 * US and later pick a random height and direction for each), plays the opening camera (skippable),
 * shows the countdown effect, switches Mario into plane mode and starts the coin counter.
 */
EVT_BEGIN(minigame01_start)
    CALL(yuu_atai_init)
    CALL(yuu_atai_push, LW(0))
    CALL(evt_map_playanim, STRING("neon_01"), MAP_ANIM_PLAY_LOOP, 0)
#if VERSION >= VERSION_US
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 250)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 350)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_01"))
    SET_VALUE(LW(1), STRING("A_dai_01"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), STRING("S_dai_01_k"))
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))

        // Motions 3 to 5: the same three speeds, starting from the other end.
#if VERSION >= VERSION_US
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 250)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 350)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_01"))
        SET_VALUE(LW(1), STRING("A_dai_01"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 500)
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(7), STRING("S_dai_01_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 1, LW(15))

// Platform 2 (S_dai_02) slides along Z, with the same choice of motions.
#if VERSION >= VERSION_US
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 250)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 350)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_02"))
    SET_VALUE(LW(1), STRING("A_dai_02"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 2)
    SET_VALUE(LW(7), STRING("S_dai_02_k"))
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))

        // Motions 3 to 5: the same three speeds, starting from the other end.
#if VERSION >= VERSION_US
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 250)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 350)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_02"))
        SET_VALUE(LW(1), STRING("A_dai_02"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 500)
        SET_VALUE(LW(6), 2)
        SET_VALUE(LW(7), STRING("S_dai_02_k"))
        SET_VALUE(LW(8), 0)
        SET_VALUE(LW(9), 0)
        SET_VALUE(LW(10), 0)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 2, LW(15))

// Platform 3 (S_dai_03) moves on X and Z at once, with the same choice of motions.
#if VERSION >= VERSION_US
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_03"))
    SET_VALUE(LW(1), STRING("A_dai_03"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 200)
#if VERSION >= VERSION_US
        SET_VALUE(LW(6), 4)
#else
    SET_VALUE(LW(6), 3)
#endif
    SET_VALUE(LW(7), STRING("S_dai_03_k"))
    SET_VALUE(LW(8), -150)
    SET_VALUE(LW(9), 150)
    SET_VALUE(LW(10), 2000)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))

        // Motions 3 to 5: the same three speeds, starting from the other end.
#if VERSION >= VERSION_US
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_03"))
        SET_VALUE(LW(1), STRING("A_dai_03"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 200)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_03_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 2000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 3, LW(15))

// Platform 4 (S_dai_04) also moves on X and Z at once.
#if VERSION >= VERSION_US
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_04"))
    SET_VALUE(LW(1), STRING("A_dai_04"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 200)
    SET_VALUE(LW(6), 4)
    SET_VALUE(LW(7), STRING("S_dai_04_k"))
    SET_VALUE(LW(8), -150)
    SET_VALUE(LW(9), 150)
    SET_VALUE(LW(10), 2000)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))

        // Motions 3 to 5: the same three speeds, starting from the other end.
#if VERSION >= VERSION_US
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_04"))
        SET_VALUE(LW(1), STRING("A_dai_04"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 200)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_04_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 2000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 4, LW(15))

// Platform 5 (S_dai_05), the last and highest-scoring one, also moves on X and Z at once.
#if VERSION >= VERSION_US
    CALL(evt_sub_random, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(1)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), -150)
        SET_VALUE(LW(3), 150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), -150)
        SET_VALUE(LW(9), 150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(2)
#endif
    SET_VALUE(LW(0), STRING("S_dai_05"))
    SET_VALUE(LW(1), STRING("A_dai_05"))
    SET_VALUE(LW(2), -150)
    SET_VALUE(LW(3), 150)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 200)
    SET_VALUE(LW(6), 4)
    SET_VALUE(LW(7), STRING("S_dai_05_k"))
    SET_VALUE(LW(8), -150)
    SET_VALUE(LW(9), 150)
    SET_VALUE(LW(10), 2000)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))

        // Motions 3 to 5: the same three speeds, starting from the other end.
#if VERSION >= VERSION_US
        CASE_EQUAL(3)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1000)
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(4)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 1500)
        SET_VALUE(LW(5), 150)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 1500)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
        CASE_EQUAL(5)
        SET_VALUE(LW(0), STRING("S_dai_05"))
        SET_VALUE(LW(1), STRING("A_dai_05"))
        SET_VALUE(LW(2), 150)
        SET_VALUE(LW(3), -150)
        SET_VALUE(LW(4), 2000)
        SET_VALUE(LW(5), 200)
        SET_VALUE(LW(6), 4)
        SET_VALUE(LW(7), STRING("S_dai_05_k"))
        SET_VALUE(LW(8), 150)
        SET_VALUE(LW(9), -150)
        SET_VALUE(LW(10), 2000)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        SPAWN_EVENT_GET_ID(PTR(moving_floor), LW(15))
    END_SWITCH
#endif
    CALL(yuuminigame_set_dataadr, 5, LW(15))

    // The platforms are ready. On the first round the camera flies over the course; A or B skips it.
    CALL(yuu_atai_pop, LW(0))
#if VERSION >= VERSION_US
    CALL(yuuminigame1_floor_on_off, LW(0), 1)
#endif
    CALL(yuuminigame1_opening_chk, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SPAWN_EVENT_GET_ID(PTR(minigame01_start_cam), LW(8))
        LOOP(0)
            IS_EVENT_RUNNING(LW(8), LW(9))
            IF_INT_EQUAL(LW(9), 0)
                CALL(evt_seq_wait, SEQ_GAME)
                BREAK_LOOP
            ELSE
                CALL(evt_key_get_buttontrg, 0, LW(9))
                IF_BITS_SET(LW(9), PAD_BUTTON_A | PAD_BUTTON_B)
                    DELETE_EVENT(LW(8))
                    CALL(evt_seq_wait, SEQ_GAME)
                    CALL_CHILD(PTR(minigame01_start_cam_cancel))
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(yuuminigame1_opening_on_off, LW(0), 0)
        CALL_CHILD(PTR(yuu_00_kaijo_init2))
        CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
        WAIT_FRAMES(1)
    END_IF

    // The music starts, Mario poses, the spotlights stop and the start signal plays.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_MINIGAME1"))
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_I_1"))
    WAIT_MS(400)
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(600)
    CALL(evt_map_playanim, STRING("neon_01"), MAP_ANIM_PLAY_AT_END, 0)
    SET_VALUE(GF(0), 0)
    CALL(eff_call_minigame, 0, 0, 0, 500)
    WAIT_MS(3000)

    // Mario becomes a paper airplane, the controls come back and the coin counter appears.
    CALL(evt_change_plane)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mario_key_onoff, 1)
    WAIT_MS(1000)
    CALL(evt_yuucounter_alloc, 0, 70, 0, 0)
#else
    CALL(evt_cam_letter_box_onoff, 0, 0)
    CALL(evt_cam_letter_box_disable, 1)
    WAIT_MS(1000)
    CALL(evt_yuucounter_alloc, 0, 70, 0, 0)
    CALL(evt_mario_key_onoff, 1)
#endif
    RETURN
EVT_END()

EVT_BEGIN(minigame01_end)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE2"), 0)
    CALL(yuuminigame_get_dataadr, 1, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 2, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 3, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 4, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 5, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
#else
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE2"), 0)
#if VERSION >= VERSION_US
    CALL(yuu_atai_push, LW(0))
    CALL(yuu_atai_push, LW(1))
    CALL(yuu_atai_push, LW(2))
    SET_VALUE(LW(0), LW(3))
    CALL_CHILD(PTR(minigame01_floor_delete))
    CALL(yuu_atai_pop, LW(2))
    CALL(yuu_atai_pop, LW(1))
    CALL(yuu_atai_pop, LW(0))
#else
    CALL(yuuminigame_get_dataadr, 1, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 2, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 3, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 4, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
    CALL(yuuminigame_get_dataadr, 5, LW(15))
    CALL(evt_moving_floor_delete, LW(15))
#endif
#endif

    // Crashing on the ground (a negative score) plays the failure effect;
    // landing on a platform plays the success effect and Mario poses by how well he did.
    CALL(evt_mario_adjust_dir)
    IF_INT_LESS(LW(0), 0)
        CALL(eff_call_minigame, 2, 0, 0, 500)
        CALL(evt_mario_set_pose, STRING("M_I_S"))
        WAIT_MS(3800)
        CALL(evt_mario_set_pose, STRING("M_D_2"))
        WAIT_MS(200)
    ELSE
        CALL(eff_call_minigame, 1, 0, 0, 500)
        WAIT_MS(500)
        IF_INT_LESS(LW(1), 1)
            CALL(evt_mario_set_pose, STRING("M_X_1"))
        ELSE
            IF_INT_GREATER(LW(1), 1)
                CALL(evt_mario_set_pose, STRING("M_V_2"))
            ELSE
                CALL(evt_mario_set_pose, STRING("M_I_1"))
            END_IF
        END_IF
        WAIT_MS(3500)
    END_IF

    // The tally screen shows the score and Mario receives his piantas, then the ranking screen records it.
    CALL(evt_yuucounter_free)
    CALL(yuugijou_montemedal_disp_onoff, 1)
    CALL(evt_yuuranking_alloc, 0, 0, 0, 0, LW(0), LW(2), LW(1), 0)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(yuuranking_get_monte, LW(3))
    CALL(yuugijou_add_montemedal, 0, LW(3))
    CALL(yuugijou_montemedal_wait)
    CALL(evt_yuuranking_free)
    WAIT_MS(200)
    CALL(evt_yuuranking_alloc, 0, 0, 0, 1, LW(0), LW(2), LW(1), 0)
    WAIT_FRAMES(1)
    CALL(yuuranking_wait_button)
    CALL(evt_yuuranking_free)

    // With fewer than 10 piantas the game ends; otherwise the player picks Continue (10 piantas) or Quit.
    SET_VALUE(LW(1), 10)
    CALL(yuugijou_get_montemedal, LW(0))
    IF_INT_LESS(LW(0), LW(1))
        SET_VALUE(LW(0), 1)
    ELSE
        CALL(evt_msg_select, 0, STRING("sysyuu_00_00"))
    END_IF

    // Quitting sends Mario back through the pipe to gor_03.
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GSWF_GOR_RETURNING_FROM_MINIGAME, 1)
        CALL(evt_mario_set_prev_party_pos, 0, 0, -1000, 0)
        CALL(evt_mario_set_prev_party_pos, 1, 0, -1000, 0)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_bero_mapchange, STRING("gor_03"), STRING("A_dokan_02"))
        RETURN
    END_IF

    // Continuing pays 10 piantas; the stage fades out, Mario returns to the start and the lights come back on.
    MULTIPLY(LW(1), -1)
    CALL(yuugijou_add_montemedal, 0, LW(1))
    CALL(yuugijou_montemedal_wait)
    WAIT_MS(200)
    CALL(yuugijou_montemedal_disp_onoff, 0)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_sfxoff, GW(10))
    CALL(evt_fade_out, 1000)
    CALL(re_fall_rtn)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL_CHILD(PTR(yuu_00_kaijo_init2))
#if VERSION > VERSION_DEMO_US
    CALL(yuu_00_cam_shift_reset)
#endif
    WAIT_MS(500)
    CALL_CHILD(PTR(light_sub_event))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_GAME_AUDIENCE1"), GW(10))
    CALL(evt_fade_in, 1000)
    RETURN
EVT_END()

/**
 * Map init for yuu_00 (Plane Mode room): starts the minigame set-up, runs the Pianta NPC set-up,
 * and (US and later) hides all but the current game-language sign graphics on the stage.
 */
EVT_BEGIN(yuu_00_init_evt)
    // The neon sign lights up, the game is set up and the audience appears.
    CALL(evt_map_playanim, STRING("neon_02"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(PTR(minigame01_main))
    CALL_CHILD(PTR(evt_yuunpc_minigame1))
#if VERSION >= VERSION_US
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("saka_k_1"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("saka_k_2"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("saka_k_3"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("saka_k_4"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("saka_k_5"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("saka_k_6"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("saka_k_7"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("saka_k_8"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("J_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("E_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("D_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("F_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("Sp_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("I_mon"), MAP_OBJ_FLAG_DISABLED)
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_saka_k_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_saka_k_2"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_saka_k_3"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_saka_k_4"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_saka_k_5"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_saka_k_6"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_saka_k_7"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_saka_k_8"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("J_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_saka_k_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_saka_k_2"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_saka_k_3"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_saka_k_4"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_saka_k_5"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_saka_k_6"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_saka_k_7"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_saka_k_8"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("E_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(2)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_saka_k_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_saka_k_2"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_saka_k_3"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_saka_k_4"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_saka_k_5"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_saka_k_6"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_saka_k_7"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_saka_k_8"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("D_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(3)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_saka_k_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_saka_k_2"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_saka_k_3"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_saka_k_4"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_saka_k_5"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_saka_k_6"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_saka_k_7"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_saka_k_8"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("F_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(4)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_saka_k_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_saka_k_2"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_saka_k_3"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_saka_k_4"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_saka_k_5"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_saka_k_6"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_saka_k_7"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_saka_k_8"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("Sp_mon"), MAP_OBJ_FLAG_DISABLED)
        CASE_EQUAL(5)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_saka_k_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_saka_k_2"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_saka_k_3"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_saka_k_4"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_saka_k_5"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_saka_k_6"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_saka_k_7"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("i_saka_k_8"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("I_mon"), MAP_OBJ_FLAG_DISABLED)
    END_SWITCH
#endif
    RETURN
EVT_END()

// Descriptive name (not in the symbol maps); placeholder unk_yuu_00004250, JP yuu .text+0x4184. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
USER_FUNC(yuu_00_cam_shift_reset) {
    camShiftReset();
    return EVT_RETURN_DONE;
}
#endif

/**
 * Names of the floor areas below the course for yuuminigame1_gamerule;
 * landing on one scores -5 (02, 05), -10 (03), -15 (06) or -20 (04).
 */
static const char yuu00_gnd_02[] = "A_ground_02";
static const char yuu00_gnd_05[] = "A_ground_05";
static const char yuu00_gnd_03[] = "A_ground_03";
static const char yuu00_gnd_06[] = "A_ground_06";
static const char yuu00_gnd_04[] = "A_ground_04";

/**
 * Returns TRUE when two points are within 20 units in 3D and within 20 in height,
 * i.e. Mario is touching a floating coin.
 */
static inline BOOL get_monte_check(Vec* a, Vec* b) {
    if (PSVECDistance(a, b) < 20.0f && __fabs(a->y - b->y) < 20.0) {
        return TRUE;
    }
    return FALSE;
}

/** Draw callback that renders every coin icon of a Monte array that has not been collected yet. */
static inline void monte_disp(CameraId camera, void* param) {
    Monte* monte = param;
    s32 i;

    if (monte != NULL) {
        for (i = 0; i < monte->num; i++) {
            if (monte[i].hidden == 0) {
                iconDispGx(&monte[i].sub.pos, 0, ICON_ITEM_PIANTA_TOKEN, 1.0f);
            }
        }
    }
}

#define randRange(d) (irand(100.0f * (d)) / 100.0f)

/**
 * Frame-by-frame coin spawner and collector for Plane Mode.
 * On first call it allocates the Monte array and scatters coins at random spots within the rectangles of
 * montetable (at least 20 units apart), then each frame it checks Mario against every coin,
 * counting and hiding collected ones with a 3D sound. Blocks forever as a view event.
 */
static USER_FUNC(fall_rtn) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    BOOL exists = FALSE;
    HikokiWork* data = wp->data;
    s32 total;
    s32 i;
    s32 t;
    s32 k;
    SfxNameWord sfx;
    s32 j;
    MonteSub* sub;
    f32 lo;
    f32 hi;
    Vec v;
    Monte* m;

    (void)&get_monte_check;
    if (isFirstCall) {
        if (monte_p != NULL) {
            exists = TRUE;
        }
        total = 0;
        for (i = 0; montetable[i].num; i++) {
            total += montetable[i].num;
        }
        monte_p = _mapAlloc(total * sizeof(Monte));
        memset(monte_p, 0, total * sizeof(Monte));
        monte_p->num = total;
        monte_p->wp = wp;
        data->monteCount = 0;
        t = 0;
        k = 0;
        for (i = 0; i < monte_p->num; i++) {
            sub = &monte_p[i].sub;
            sub->pos.z = 0.0f;
            do {
                if (montetable[t].x0 < montetable[t].x1) {
                    lo = montetable[t].x0;
                    hi = montetable[t].x1;
                } else {
                    lo = montetable[t].x1;
                    hi = montetable[t].x0;
                }
                hi -= lo;
                sub->pos.x = lo + randRange(hi);
                if (montetable[t].y0 < montetable[t].y1) {
                    lo = montetable[t].y0;
                    hi = montetable[t].y1;
                } else {
                    lo = montetable[t].y1;
                    hi = montetable[t].y0;
                }
                hi -= lo;
                sub->pos.y = lo + randRange(hi);
                for (j = 0; j < k; j++) {
                    PSVECSubtract(&sub->pos, &(sub - (j + 1))->pos, &v);
                    if (PSVECMag(&v) < 20.0f) {
                        break;
                    }
                }
            } while (j != k);
            if (++k >= montetable[t].num) {
                k = 0;
                t++;
            }
            sub->scale = 1.0f;
        }
    }
    for (i = 0; i < monte_p->num; i++) {
        sfx = (SfxNameWord)"SFX_EVT_GAME_MONTE_COUNT1";
        m = &monte_p[i];
        switch (m->state) {
            case 0:
                if (get_monte_check(&m->sub.pos, &marioGetPtr()->position) == 1) {
                    data->monteCount++;
                    psndSFXOn_3D((const char*)sfx, &marioGetPtr()->position);
                    m->hidden = 1;
                    m->state = 1;
                }
                break;
            case 1:
                break;
        }
    }
    if (!exists) {
        dispEntry(CAMERA_3D, 0.0f, 1, monte_disp, monte_p);
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Resets the coins for the next round: stops the view event,
 * frees the Monte array and starts a fresh minigame_view_monte event.
 */
static USER_FUNC(re_fall_rtn) {
    HikokiWork* data;

    if (monte_p != NULL) {
        data = monte_p->wp->data;
        if (data->viewEvent != NULL) {
            evtDeleteID(data->viewEvent->eventId);
            data->viewEvent = NULL;
            _mapFree(monte_p);
            data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
            data->viewEvent->lwData[0] = (EvtWord)monte_p->wp;
        }
    }
    return EVT_RETURN_DONE;
}

/** Script call that switches Mario's motion to 0x18, the airplane glide motion used in Plane Mode. */
USER_FUNC(evt_change_plane) {
    marioChgMot(MARIO_MOTION_PLANE);
    return EVT_RETURN_DONE;
}

/**
 * args[0] = minigame work pointer, args[1] = new phase for the Plane Mode state machine (0 idle, 1 start, 2
 * playing, 3 finish).
 */
USER_FUNC(yuuminigame1_set_phase) {
    EvtWord* args = event->args;
    HikokiWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    data->phase = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * args[0] = minigame work pointer, args[1] = on/off.
 * Sets or clears the "opening cutscene pending" bit (flags bit 0).
 */
USER_FUNC(yuuminigame1_opening_on_off) {
    EvtWord* args = event->args;
    HikokiWork* data = ((MiniGameWork*)evtGetValue(event, args[0]))->data;

    if (evtGetValue(event, args[1])) {
        data->flags |= 1;
    } else {
        data->flags &= ~1;
    }
    return EVT_RETURN_DONE;
}

/**
 * args[0] = minigame work pointer, args[1] = output.
 * Writes 1 if the opening cutscene is still pending (flags bit 0), else 0.
 */
USER_FUNC(yuuminigame1_opening_chk) {
    EvtWord* args = event->args;
    MiniGameWork* wp = (MiniGameWork*)evtGetValue(event, args[0]);

    if (((HikokiWork*)wp->data)->flags & 1) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

#if VERSION >= VERSION_US
/** args[0] = minigame work pointer, args[1] = on/off. Sets or clears the "platforms exist" bit (flags bit 1). */
USER_FUNC(yuuminigame1_floor_on_off) {
    EvtWord* args = event->args;
    u32* flags = &((HikokiWork*)((MiniGameWork*)evtGetValue(event, args[0]))->data)->flags;

    if (evtGetValue(event, args[1])) {
        *flags |= 2;
    } else {
        *flags &= ~2;
    }
    return EVT_RETURN_DONE;
}

/**
 * args[0] = minigame work pointer, args[1] = output.
 * Writes 1 if the platforms currently exist (flags bit 1), else 0.
 */
USER_FUNC(yuuminigame1_floor_chk) {
    EvtWord* args = event->args;
    u32* flags = &((HikokiWork*)((MiniGameWork*)evtGetValue(event, args[0]))->data)->flags;

    if (*flags & 2) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}
#endif

/** Init callback for minigame 1: starts the coin view event and marks the opening cutscene as pending. */
void yuuminigame_init_1hikoki(EventEntry* event, BOOL isFirstCall) {
    HikokiWork* data = ((MiniGameWork*)event->lwData[0])->data;

    data->viewEvent = evtEntryType(minigame_view_monte, 0, 0, 0);
    data->viewEvent->lwData[0] = event->lwData[0];
    data->flags |= 1;
}

/**
 * Per-frame state machine for Plane Mode: phase 1 starts minigame01_start,
 * phase 2 waits for the rules check to finish, phase 3 launches minigame01_end with the coin count,
 * result and monte count.
 */
void yuuminigame_main_1hikoki(EventEntry* event, BOOL isFirstCall) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];
    HikokiWork* data = wp->data;

    switch (data->phase) {
        case 0:
            break;
        case 4:
            data->phase = 1;
        case 1:
            wp->subEvent = evtEntry(minigame01_start, 0, 0);
            wp->subEvent->lwData[0] = event->lwData[0];
            data->phase++;
            break;
        case 2:
            if (yuuminigame1_gamerule(event, isFirstCall, wp, data)) {
                data->phase++;
            }
            break;
        case 3:
            wp->subEvent = evtEntry(minigame01_end, 0, 0);
            wp->subEvent->lwData[0] = data->coins;
            wp->subEvent->lwData[1] = data->result;
            wp->subEvent->lwData[2] = data->monteCount;
#if VERSION >= VERSION_US
            wp->subEvent->lwData[3] = event->lwData[0];
#endif
            data->phase++;
            break;
    }
}

/** Draw callback for minigame 1: draws the collected-coin counter (monteCountDisp). */
void yuuminigame_disp_1hikoki(CameraId camera, void* param) {
    monteCountDisp();
}

BOOL yuuminigame1_gamerule(EventEntry* event, BOOL isFirstCall, MiniGameWork* wp, HikokiWork* data) {
    MarioWork* mario = marioGetPtr();

#if VERSION >= VERSION_US
    EventEntry* sub;

    if (mario->hitobjStandOn != NULL) {
        sub = evtEntry(minigame01_floor_delete, 0, 0);
        sub->lwData[0] = event->lwData[0];
        if ((u16)((mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) ? mario->prevMotionId : mario->currMotionId) != MARIO_MOTION_PLANE) {
#else
    if (mario->hitobjStandOn != NULL && (u16)((mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) ? mario->prevMotionId : mario->currMotionId) != MARIO_MOTION_PLANE) {
#endif
        data->result = 1;
        data->coins = yuucounter_get_data();
        if (!strcmp("A_dai_01", hitGetName(mario->hitobjStandOn))) {
            data->result = 2;
        }
        if (!strcmp("A_dai_02", hitGetName(mario->hitobjStandOn))) {
            data->result = 2;
        }
        if (!strcmp("A_dai_03", hitGetName(mario->hitobjStandOn))) {
            data->result = 2;
        }
        if (!strcmp("A_dai_04", hitGetName(mario->hitobjStandOn))) {
#if VERSION >= VERSION_US
            data->result = 2;
#else
            data->result = 3;
#endif
        }
        if (!strcmp("A_dai_05", hitGetName(mario->hitobjStandOn))) {
#if VERSION >= VERSION_US
            data->result = 3;
#else
            data->result = 5;
#endif
        }
        if (!strcmp(yuu00_gnd_02, hitGetName(mario->hitobjStandOn))) {
            data->result = -5;
        }
        if (!strcmp(yuu00_gnd_05, hitGetName(mario->hitobjStandOn))) {
            data->result = -5;
        }
        if (!strcmp(yuu00_gnd_03, hitGetName(mario->hitobjStandOn))) {
            data->result = -10;
        }
        if (!strcmp(yuu00_gnd_06, hitGetName(mario->hitobjStandOn))) {
            data->result = -15;
        }
        if (!strcmp(yuu00_gnd_04, hitGetName(mario->hitobjStandOn))) {
            data->result = -20;
        }
        marioKeyOff();
        partyKeyOff();
        yuucounter_flag_on(2);
        return TRUE;
#if VERSION >= VERSION_US
    }
#endif
    }
    return FALSE;
}

// aji_03 (敵のアジト, X-Naut Fortress): the crane game room: the UFO-catcher crane (ufo_main) and its prizes
// (keihin) and badges, the clockwork (zenmai) switches.

#include "version.h"
#include "evt/evt_window.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_pouch.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "dolphin/mtx.h"
#include "driver/itemdrv.h"
#include "string.h"
#include "system.h"
#include "stdio.h"
#include "icon_ids.h"
#include "evt/evt_bero.h"
#include "aji.h"

void icon_disp(CameraId camera, void* param);

USER_FUNC(evt_sousa_icon);
USER_FUNC(item_chk);
USER_FUNC(evt_itemNameToItemID);

static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[2];
EvtWord evt_kaitentou[44];
EvtWord evt_kaitentou2[44];
static EvtWord evt_hari[125];
EvtWord ufo_hand[129];
EvtWord move_left[5];
EvtWord move_left2[5];
EvtWord move_up[5];
EvtWord move_up2[5];
EvtWord hand_success[65];
EvtWord hand_fail[128];
EvtWord hand_release[65];
EvtWord ufo_main[525];
EvtWord evt_ufo_init[115];
EvtWord evt_zenmai[141];
EvtWord blink_sw[33];
EvtWord evt_ufo_sousaban[VERSION_PICK(267, 279)];
static EvtWord evt_sw[141];
EvtWord evt_sw_reinit[88];
EvtWord evt_sw0[72];
EvtWord evt_sw1[72];
EvtWord evt_sw2[72];
s32 haguruma_tbl[2];
#if VERSION <= VERSION_DEMO_US
static EvtWord evt_haguruma[130];
#else
static EvtWord evt_haguruma[132];
#endif
EvtWord badge_evt[5];
EvtWord badge_evt2[5];
EvtWord badge_evt3[5];
#if VERSION <= VERSION_DEMO_US
extern EvtWord aji_03_init_evt[];
#else
EvtWord aji_03_init_evt[525];
#endif

/**
 * Door close animation step: moves the two door leaves S_door1_1
 * and S_door1_2 in opposite directions along Z by the animation value in LW(0) times -0.25;
 * the open and close scripts apply the same transform. The demo builds use -0.2217.
 */
static EVT_BEGIN(e_door_close)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/**
 * Door open animation step: moves the two door leaves S_door1_1
 * and S_door1_2 in opposite directions along Z by the animation value in LW(0) times -0.25;
 * the open and close scripts apply the same transform. The demo builds use -0.2217.
 */
static EVT_BEGIN(e_door_open)
#if VERSION <= VERSION_DEMO_US
MULTIPLY_FLOAT(LW(0), FLOAT(-0.22167969f))
#else
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.25f))
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

/** Exit of this room: the east door to aji_17. */
static BeroEntry bero_entry_data[] = {
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
        "w_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * Spins the map object named in LW(10) around its Y axis, 5 degrees per frame, until GF(15) is set;
 * used for the rotating warning beacon S_pato4 once the crane has been powered.
 * The object is unhidden while it spins and hidden again at the end.
 */
EVT_BEGIN(evt_kaitentou)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(10), MAP_OBJ_FLAG_HIDDEN)
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    LOOP(0)
        CALL(evt_mapobj_rotate, 1, LW(10), 0, LW(0), 0)
        ADD_FLOAT(LW(0), FLOAT(5.0f))
        IF_FLOAT_GREATER(LW(0), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
        END_IF
        WAIT_FRAMES(1)
        IF_INT_EQUAL(GF(15), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/**
 * Spins the map object named in LW(10) around its Z axis, 5 degrees per frame,
 * until GF(15) is set (evt_sw_reinit sets it to stop all spinners);
 * the lamp spinners (S_pato1 to 3) of the crane power switches use it.
 */
EVT_BEGIN(evt_kaitentou2)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(10), MAP_OBJ_FLAG_HIDDEN)
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    LOOP(0)
        CALL(evt_mapobj_rotate, 1, LW(10), 0, 0, LW(0))
        ADD_FLOAT(LW(0), FLOAT(5.0f))
        IF_FLOAT_GREATER(LW(0), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
        END_IF
        WAIT_FRAMES(1)
        IF_INT_EQUAL(GF(15), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/**
 * Swings the needle (hari) map object named in LW(10) (S_hari01 to S_hari06, started when the crane game is
 * switched on) around its Z axis: picks random target angles up to 90 degrees (alternating sides)
 * over 1.5 to 3 seconds each, until GF(15) is raised by a switch reset, then eases it back to 0 over one second.
 * Guess: the needles are dial pointers on the machinery.
 */
static EVT_BEGIN(evt_hari)
    SET_VALUE(LW(6), LW(8))
    SET_VALUE(LW(7), LW(9))
    SET_VALUE(LW(8), LW(10))
    IF_INT_NOT_EQUAL(LW(6), -1)
    END_IF
    SET_VALUE(LW(2), 0)
    SET_VALUE(LF(0), 0)
    LOOP(0)
        CALL(evt_sub_random, 900, LW(3))
        IF_INT_EQUAL(LF(0), 0)
            MULTIPLY(LW(3), -1)
            SET_VALUE(LF(0), 1)
            ELSE
            SET_VALUE(LF(0), 0)
        END_IF
        CALL(evt_sub_random, 1500, LW(9))
        ADD(LW(9), 1500)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(2), LW(3), LW(9))
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            CALL(evt_mapobj_rotate, 1, LW(8), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(GF(15), 1)
                SET_VALUE(LW(3), LW(0))
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        SET_VALUE(LW(2), LW(3))
        IF_INT_EQUAL(GF(15), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    IF_INT_NOT_EQUAL(LW(6), -1)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, LW(2), 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_rotate, 1, LW(8), 0, 0, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Opens and closes the crane's claw forever: the four claw arms (S_te_1_a/b, S_te_2_a/b) swing by 30 degrees
 * over one second and back.
 * GF(9) is 1 while the claw is open, so ufo_main can wait for it to be shut before lowering.
 */
EVT_BEGIN(ufo_hand)
    LOOP(0)
        SET_VALUE(GF(9), 1)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 3000, 1000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(LW(2), LW(0))
        SET_FLOAT_VALUE(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(3), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 1, STRING("S_te_1_a"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_te_2_a"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_te_1_b"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("S_te_2_b"), 0, 0, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 3000, 0, 1000)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(LW(2), LW(0))
        SET_FLOAT_VALUE(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(3), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 1, STRING("S_te_1_a"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_te_2_a"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_te_1_b"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("S_te_2_b"), 0, 0, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        SET_VALUE(GF(9), 0)
        WAIT_FRAMES(2)
    END_LOOP
    RETURN
EVT_END()

/**
 * Sets GF(10) to 1, the flag evt_ufo_sousaban raises while the crane is driven along X.
 * Nothing runs this script; evt_ufo_sousaban sets GF(10) itself.
 */
EVT_BEGIN(move_left)
    SET_VALUE(GF(10), 1)
    RETURN
EVT_END()

/** Sets GF(10) back to 0; unused, see move_left. */
EVT_BEGIN(move_left2)
    SET_VALUE(GF(10), 0)
    RETURN
EVT_END()

/**
 * Sets GF(11) to 1, the flag evt_ufo_sousaban raises while the crane is driven along Z.
 * Nothing runs this script; evt_ufo_sousaban sets GF(11) itself.
 */
EVT_BEGIN(move_up)
    SET_VALUE(GF(11), 1)
    RETURN
EVT_END()

/** Sets GF(11) back to 0; unused, see move_up. */
EVT_BEGIN(move_up2)
    SET_VALUE(GF(11), 0)
    RETURN
EVT_END()

/** Closes the crane's claw (all three arm pairs rotate 30 degrees over one second) to grab a prize. */
EVT_BEGIN(hand_success)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 3000, 1000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_a"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_a"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_b"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_b"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_c"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_c"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

/**
 * Closes the claw to 50 degrees over one second and opens it again: the missed-grab motion its name describes,
 * but nothing runs it (ufo_main calls hand_success whether item_chk finds a prize or not).
 */
EVT_BEGIN(hand_fail)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 5000, 1000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_a"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_a"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_b"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_b"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_c"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_c"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 5000, 0, 1000)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_a"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_a"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_b"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_b"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_c"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_c"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    RETURN
EVT_END()

/** Opens the claw back up over one second after the crane has carried a prize to the chute or come up empty. */
EVT_BEGIN(hand_release)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 3000, 0, 1000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_a"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_a"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_b"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_b"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_1_c"), 0, 0, LW(0))
    CALL(evt_mapobj_rotate, 1, STRING("S_te_2_c"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ufo_main)
    SET_VALUE(GF(10), 0)
    SET_VALUE(GF(11), 0)
    SET_VALUE(GF(12), 0)
    SET_VALUE(GF(14), 0)
    SET_VALUE(GW(10), 230)
    SET_VALUE(GW(11), 100)
    CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), 0, GW(11))
    // The claw slides sideways while the player holds A and stops when A is released.
    LOOP(0)
        SPAWN_EVENT_GET_ID(PTR(ufo_hand), LW(9))
        LOOP(0)
            IF_INT_EQUAL(GF(10), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(GF(12), 1)
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_MOVE1"), LW(0), LW(1), LW(2), LW(3))
        LOOP(0)
            ADD(GW(10), -1)
            CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), 0, GW(11))
            CALL(evt_mapobj_get_position, STRING("S_kure"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(3), LW(0), LW(1), LW(2))
            IF_INT_LESS(GW(10), -150)
                BREAK_LOOP
            END_IF
            IF_INT_NOT_EQUAL(GF(10), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(3))
        // Then it slides forward while B is held.
        LOOP(0)
            IF_INT_EQUAL(GF(11), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_MOVE1"), LW(0), LW(1), LW(2), LW(3))
        LOOP(0)
            ADD(GW(11), -1)
            CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), 0, GW(11))
            CALL(evt_mapobj_get_position, STRING("S_kure"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(3), LW(0), LW(1), LW(2))
            IF_INT_LESS(GW(11), -100)
                BREAK_LOOP
            END_IF
            IF_INT_NOT_EQUAL(GF(11), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(3))
        LABEL(100)
        // The claw lowers, closes and looks for a prize underneath.
        IF_INT_EQUAL(GF(9), 1)
            WAIT_FRAMES(1)
            GOTO(100)
        END_IF
        DELETE_EVENT(LW(9))
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_MOVE1"), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -600, 2000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), LW(0), GW(11))
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfx_pos, LW(5), LW(2), LW(3), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        // Both outcomes use the same closing animation; the result only controls item transport.
        CALL(item_chk, LW(0), LW(8))
        IF_INT_EQUAL(LW(0), 1)
            CALL_CHILD(PTR(hand_success))
            SET_VALUE(LF(0), 1)
            ELSE
            CALL_CHILD(PTR(hand_success))
            SET_VALUE(LF(0), 0)
        END_IF
        // A prize is lifted and carried to the drop point, then dropped with a random bounce.
        IF_INT_EQUAL(LF(0), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_item_move_3d_position, LW(8), GW(10), 105, GW(11), 2000, EASING_SINE_IN_OUT)
            END_INLINE_EVENT
        END_IF
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -600, 0, 2000)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), LW(0), GW(11))
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfx_pos, LW(5), LW(2), LW(3), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_snd_sfxoff, LW(5))
        IF_INT_EQUAL(LF(0), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_item_move_3d_position, LW(8), GW(10), 105, 110, 3000, EASING_SINE_IN_OUT)
            END_INLINE_EVENT
        END_IF
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_MOVE1"), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, GW(11), 110, 3000)
        LABEL(2)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), 0, LW(0))
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfx_pos, LW(5), LW(2), LW(3), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(2)
        END_IF
        SET_VALUE(GW(11), 110)
        CALL(evt_snd_sfxoff, LW(5))
        CALL(evt_cam3d_evt_set, 50, 145, 529, 50, 50, -13, 2000, EASING_SINE_IN_OUT)
        IF_INT_EQUAL(LF(0), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_item_move_3d_position, LW(8), 40, 105, GW(11), 3000, EASING_SINE_IN_OUT)
            END_INLINE_EVENT
        END_IF
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_MOVE1"), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, GW(10), 40, 3000)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_kure"), LW(0), 0, GW(11))
        CALL(evt_mapobj_get_position, STRING("S_kure"), LW(2), LW(3), LW(4))
        CALL(evt_snd_sfx_pos, LW(5), LW(2), LW(3), LW(4))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
        SET_VALUE(GW(10), 40)
        CALL(evt_snd_sfxoff, LW(5))
        IF_INT_EQUAL(LF(0), 1)
            BEGIN_INLINE_EVENT
                WAIT_MS(300)
                CALL(evt_item_set_move_dir_speed, LW(8), 0, FLOAT(0.0f))
                CALL(evt_item_set_gravity, LW(8), FLOAT(1.0f))
                CALL(evt_item_set_bound_rate, LW(8), FLOAT(1.0f))
                CALL(evt_item_set_jump_power, LW(8), FLOAT(0.0f))
                CALL(evt_item_set_bound_limit, LW(8), 0)
                CALL(evt_sub_random, 30, LW(0))
                ADD(LW(0), 20)
                CALL(evt_item_set_bound_next_speed, LW(8), LW(0))
                CALL(evt_sub_random, 90, LW(0))
                ADD(LW(0), 45)
                CALL(evt_item_set_bound_next_dir, LW(8), LW(0))
                CALL(evt_item_flag_on, LW(8), ITEM_FLAG_BOUND_NEXT)
                CALL(evt_item_status_on, LW(8), ITEM_FLAG2_SKIP_BOUND_INIT)
                CALL(evt_item_change_mode, LW(8), 5)
            END_INLINE_EVENT
            // The claw opens and is ready for the next try.
            CALL_CHILD(PTR(hand_release))
            ELSE
            CALL_CHILD(PTR(hand_release))
        END_IF
        SET_VALUE(GF(12), 0)
        RETURN
    END_LOOP
    RETURN
EVT_END()

/**
 * Powers up the crane: the machine's light S_hikari flickers on
 * and the prize-light balls S_tama_a/S_tama_b blink alternately and S_hikari rotates
 * until GF(14) (crane shut down), then ufo_main starts.
 */
EVT_BEGIN(evt_ufo_init)
    LOOP(3)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
    END_LOOP
    WAIT_MS(300)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(300)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_tama_a"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_tama_b"), MAP_OBJ_FLAG_HIDDEN)
            WAIT_MS(500)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_tama_a"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_tama_b"), MAP_OBJ_FLAG_HIDDEN)
            WAIT_MS(500)
            IF_INT_EQUAL(GF(14), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 36000, 3000)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_hikari"), 0, LW(0), 0)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(GF(14), 1)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(ufo_main))
    RETURN
EVT_END()

/**
 * Spins the clock gears (zenmai, clockwork) of the room forever;
 * one revolution takes 2 seconds, 1 second once STG7_19_THE_DOOR_TO_LORD_CRUMP is reached.
 * Plays SFX_STG7_AMB_GEAR1.
 */
EVT_BEGIN(evt_zenmai)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_AMB_GEAR1"), 0)
    LOOP(0)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG7_19_THE_DOOR_TO_LORD_CRUMP)
            CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 36000, 2000)
            ELSE
            CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 36000, 1000)
        END_IF
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
        SET_FLOAT_VALUE(LW(2), LW(0))
        SET_FLOAT_VALUE(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(3), FLOAT(-1.0f))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmai1"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmai2"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmai3"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmai4"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmai5"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmai6"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmais1"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmais2"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmais3"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmais4"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmais5"), 0, 0, LW(3))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmais6"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 1, STRING("S_zenmais_ari"), 0, 0, LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Makes the switch lamps pair LW(0) and LW(1) blink forever (the three crane power switches use it while they
 * are not yet pressed).
 */
EVT_BEGIN(blink_sw)
    LOOP(0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(0), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(600)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(0), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(600)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_ufo_sousaban)
    // Mario is locked in place while the crane powers up.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 230, 200, 380, 230, 130, -13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1200)
    CALL(evt_mapobj_get_position, STRING("S_kure"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_MOVE1"), LW(0), LW(1), LW(2), LW(3))
    SPAWN_EVENT(PTR(evt_ufo_init))
    WAIT_MS(2200)
    CALL(evt_snd_sfxoff, LW(3))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 2300, 400, 3000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            SET_FLOAT_VALUE(GW(10), LW(0))
            CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), 0, GW(11))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -65, 175, 529, -65, 80, -13, 3000, EASING_SINE_IN_OUT)
    WAIT_MS(3400)
    IF_INT_EQUAL(GSWF_AJI_03_CRANE_HELP_SEEN, 0)
        SET_VALUE(GSWF_AJI_03_CRANE_HELP_SEEN, 1)
        CALL(evt_msg_print, 0, STRING("stg7_aji_00_01"), 0, 0)
    END_IF
    WAIT_MS(300)
    // The A button icon is shown until the player holds A to move the claw sideways.
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_sousa_icon, 0, 200, 130)
        END_LOOP
    END_INLINE_EVENT
#if VERSION >= VERSION_US
    WAIT_FRAMES(1)
#endif
    LOOP(0)
        CALL(evt_key_get_button, 0, LW(0))
        IF_BITS_SET(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#if VERSION >= VERSION_US
    WAIT_FRAMES(1)
#endif
    SET_VALUE(GF(10), 1)
    LOOP(0)
        CALL(evt_key_get_button, 0, LW(0))
        IF_BITS_SET(LW(0), 256)
            ELSE
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#if VERSION >= VERSION_US
    WAIT_FRAMES(1)
#endif
    SET_VALUE(GF(10), 0)
    DELETE_EVENT(LW(10))
    // The B button icon is shown until the player holds B to move the claw forward.
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        LOOP(0)
            CALL(evt_sousa_icon, 1, 200, 70)
        END_LOOP
    END_INLINE_EVENT
#if VERSION >= VERSION_US
    WAIT_FRAMES(1)
#endif
    LOOP(0)
        CALL(evt_key_get_button, 0, LW(0))
        IF_BITS_SET(LW(0), 512)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#if VERSION >= VERSION_US
    WAIT_FRAMES(1)
#endif
    SET_VALUE(GF(11), 1)
    LOOP(0)
        CALL(evt_key_get_button, 0, LW(0))
        IF_BITS_SET(LW(0), 512)
            ELSE
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#if VERSION >= VERSION_US
    WAIT_FRAMES(1)
#endif
    SET_VALUE(GF(11), 0)
    DELETE_EVENT(LW(10))
    // The claw grabs; when the game ends the crane lights go out and Mario gets control back.
    LOOP(0)
        IF_INT_EQUAL(GF(12), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 400, 2300, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        SET_FLOAT_VALUE(GW(10), LW(0))
        CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), 0, GW(11))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GW(10), 230)
    SET_VALUE(GW(11), 100)
    CALL(evt_mapobj_trans, 1, STRING("S_kure"), GW(10), 0, GW(11))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(GF(14), 1)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_sw)
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_1_PRESSED, 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_2_PRESSED, 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_3_PRESSED, 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 0)
        SET_VALUE(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
        CALL(evt_mario_key_onoff, 0)
        WAIT_MS(1000)
        CALL(evt_cam3d_evt_set, 20, 125, 394, 20, 55, 0, 1000, EASING_SINE_IN_OUT)
        WAIT_MS(1500)
        BEGIN_INLINE_EVENT
            CALL(evt_mapobj_get_position, STRING("S_pato4"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_SIREN1"), LW(0), LW(1), LW(2), LW(3))
            WAIT_MS(3000)
            CALL(evt_snd_sfxoff, LW(3))
        END_INLINE_EVENT
        SET_VALUE(LW(10), STRING("S_pato4"))
        SPAWN_EVENT(PTR(evt_kaitentou))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_1_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_2_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("meta_3_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("meta_1_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("meta_2_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("meta_3_2"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(8), -1)
        SET_VALUE(LW(9), -1)
        SET_VALUE(LW(10), STRING("S_hari01"))
        SPAWN_EVENT(PTR(evt_hari))
        SET_VALUE(LW(8), -1)
        SET_VALUE(LW(9), -1)
        SET_VALUE(LW(10), STRING("S_hari02"))
        SPAWN_EVENT(PTR(evt_hari))
        WAIT_MS(500)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui_01"), 0, PTR(evt_ufo_sousaban), 0)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_sw_reinit)
    SET_VALUE(GF(15), 1)
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_1_PRESSED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_1_PRESSED, 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui1"), 0, PTR(evt_sw0), 0)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_2_PRESSED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_2_PRESSED, 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui2"), 0, PTR(evt_sw1), 0)
    END_IF
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_3_PRESSED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_3_PRESSED, 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui3"), 0, PTR(evt_sw2), 0)
    END_IF
    DELETE_EVENT(GW(13))
    DELETE_EVENT(GW(14))
    DELETE_EVENT(GW(15))
    WAIT_FRAMES(5)
    SET_VALUE(GF(15), 0)
    SET_VALUE(LW(0), STRING("sui__1"))
    SET_VALUE(LW(1), STRING("sui__2"))
    SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(13))
    SET_VALUE(LW(0), STRING("sui_2_1"))
    SET_VALUE(LW(1), STRING("sui_2_2"))
    SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(14))
    SET_VALUE(LW(0), STRING("sui_3_1"))
    SET_VALUE(LW(1), STRING("sui_3_2"))
    SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(15))
    RETURN
EVT_END()

EVT_BEGIN(evt_sw0)
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_2_PRESSED, 0)
        IF_INT_EQUAL(GSWF_AJI_03_SWITCH_3_PRESSED, 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_BUTTON_PUSH1"), LW(0), LW(1), LW(2), 0)
    IF_INT_EQUAL(LF(0), 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_1_PRESSED, 1)
        DELETE_EVENT(GW(13))
        WAIT_FRAMES(1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui__1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("sui__2"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(10), STRING("S_pato1"))
        SPAWN_EVENT(PTR(evt_kaitentou2))
        SET_VALUE(LW(8), STRING("meta_1_1"))
        SET_VALUE(LW(9), STRING("meta_1_2"))
        SET_VALUE(LW(10), STRING("S_hari04"))
        SPAWN_EVENT(PTR(evt_hari))
        CALL_CHILD(PTR(evt_sw))
        CALL(evt_exit_case_evt)
        ELSE
        CALL_CHILD(PTR(evt_sw_reinit))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_sw1)
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_1_PRESSED, 1)
        IF_INT_EQUAL(GSWF_AJI_03_SWITCH_3_PRESSED, 1)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_BUTTON_PUSH1"), LW(0), LW(1), LW(2), 0)
    IF_INT_EQUAL(LF(0), 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_2_PRESSED, 1)
        DELETE_EVENT(GW(14))
        WAIT_FRAMES(1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_2_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("sui_2_2"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(10), STRING("S_pato2"))
        SPAWN_EVENT(PTR(evt_kaitentou2))
        SET_VALUE(LW(8), STRING("meta_2_1"))
        SET_VALUE(LW(9), STRING("meta_2_2"))
        SET_VALUE(LW(10), STRING("S_hari05"))
        SPAWN_EVENT(PTR(evt_hari))
        CALL_CHILD(PTR(evt_sw))
        CALL(evt_exit_case_evt)
        ELSE
        CALL_CHILD(PTR(evt_sw_reinit))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_sw2)
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_1_PRESSED, 1)
        IF_INT_EQUAL(GSWF_AJI_03_SWITCH_2_PRESSED, 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_CRANE_BUTTON_PUSH1"), LW(0), LW(1), LW(2), 0)
    IF_INT_EQUAL(LF(0), 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_3_PRESSED, 1)
        DELETE_EVENT(GW(15))
        WAIT_FRAMES(1)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_3_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("sui_3_2"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(10), STRING("S_pato3"))
        SPAWN_EVENT(PTR(evt_kaitentou2))
        SET_VALUE(LW(8), STRING("meta_3_1"))
        SET_VALUE(LW(9), STRING("meta_3_2"))
        SET_VALUE(LW(10), STRING("S_hari06"))
        SPAWN_EVENT(PTR(evt_hari))
        CALL_CHILD(PTR(evt_sw))
        CALL(evt_exit_case_evt)
        ELSE
        CALL_CHILD(PTR(evt_sw_reinit))
    END_IF
    RETURN
EVT_END()

/** Key item list for the item menu in the crane game room: the Cog. */
s32 haguruma_tbl[] = {
    85, -1,
};

static EVT_BEGIN(evt_haguruma)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_pouch_check_item, ITEM_COG, LW(0))
    IF_INT_LESS(LW(0), 1)
        CALL(evt_msg_print, 0, STRING("stg7_aji_00_00"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, PTR(haguruma_tbl), LW(0))
#else
    CALL(evt_win_item_select, 0, PTR(haguruma_tbl), LW(0), 0)
#endif
    IF_INT_NOT_EQUAL(LW(0), -1)
        BEGIN_INLINE_EVENT
            CALL(evt_snd_bgmoff, PSND_PAUSE)
            CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
            WAIT_MS(2000)
            CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        END_INLINE_EVENT
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_zenmais_nashi"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_zenmais_ari"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_zenmai)
#else
        SPAWN_EVENT(PTR(evt_zenmai))
#endif
        CALL(evt_pouch_remove_item, LW(0), LW(1))
#if VERSION <= VERSION_DEMO_US
        SET_VALUE(GSWF_AJI_03_COG_PLACED, 1)
#else
        SET_VALUE(GSWF_AJI_03_COG_PLACED, 1)
#endif
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui1"), 0, PTR(evt_sw0), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui2"), 0, PTR(evt_sw1), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui3"), 0, PTR(evt_sw2), 0)
        SET_VALUE(LW(0), STRING("sui__1"))
        SET_VALUE(LW(1), STRING("sui__2"))
        SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(13))
        SET_VALUE(LW(0), STRING("sui_2_1"))
        SET_VALUE(LW(1), STRING("sui_2_2"))
        SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(14))
        SET_VALUE(LW(0), STRING("sui_3_1"))
        SET_VALUE(LW(1), STRING("sui_3_2"))
        SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(15))
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_exit_case_evt)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(badge_evt)
    SET_VALUE(GSWF_AJI_03_FEELING_FINE, 1)
    RETURN
EVT_END()

EVT_BEGIN(badge_evt2)
    SET_VALUE(GSWF_AJI_03_FEELING_FINE_P, 1)
    RETURN
EVT_END()

EVT_BEGIN(badge_evt3)
    SET_VALUE(GSWF_STARPIECE_98, 1)
    RETURN
EVT_END()

EVT_BEGIN(aji_03_init_evt)
    // The room's music and ambience start, and the crane lights are hidden.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_DUN2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI3"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_snd_set_rev_mode, 1)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_bero_info_run)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
#endif
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("S_kure"), 230, 0, 100)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_tama_a"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_tama_b"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_rotate, 1, STRING("S_tanadoor_1"), 0, 70, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_tanadoor_2"), 0, 70, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_1_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_2_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_3_2"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_US
    // If the crane was already activated in an earlier visit, all three crane buttons count as pressed.
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_1_PRESSED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_2_PRESSED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_3_PRESSED, 1)
#else
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_1_PRESSED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_2_PRESSED, 1)
        SET_VALUE(GSWF_AJI_03_SWITCH_3_PRESSED, 1)
#endif
    END_IF
    // Before the cog is inserted, the shelf door is the place to use it; afterwards the clockwork runs and the
    // crane panel can be used.
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_03_COG_PLACED, 0)
#else
    IF_INT_EQUAL(GSWF_AJI_03_COG_PLACED, 0)
#endif
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_tanadoor_1"), 0, PTR(evt_haguruma), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_zenmais_ari"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_zenmais_nashi"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_zenmai)
        IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
#else
        SPAWN_EVENT(PTR(evt_zenmai))
        IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
#endif
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui_01"), 0, PTR(evt_ufo_sousaban), 0)
        END_IF
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_03_COG_PLACED, 1)
        IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 0)
#else
    IF_INT_EQUAL(GSWF_AJI_03_COG_PLACED, 1)
        IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 0)
#endif
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui1"), 0, PTR(evt_sw0), 0)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui2"), 0, PTR(evt_sw1), 0)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_sui3"), 0, PTR(evt_sw2), 0)
            SET_VALUE(LW(0), STRING("sui__1"))
            SET_VALUE(LW(1), STRING("sui__2"))
            SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(13))
            SET_VALUE(LW(0), STRING("sui_2_1"))
            SET_VALUE(LW(1), STRING("sui_2_2"))
            SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(14))
            SET_VALUE(LW(0), STRING("sui_3_1"))
            SET_VALUE(LW(1), STRING("sui_3_2"))
            SPAWN_EVENT_GET_ID(PTR(blink_sw), GW(15))
        END_IF
    END_IF
    // The three crane buttons blink until the player has pressed them, and the lit or unlit versions of their
    // parts are shown.
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 0)
#else
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 0)
#endif
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui__2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_2_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_3_2"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui__1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_2_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("sui_3_1"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_pato1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_pato2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_pato3"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_1_PRESSED, 1)
#else
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_1_PRESSED, 1)
#endif
        SET_VALUE(LW(10), STRING("S_pato1"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_kaitentou2)
#else
        SPAWN_EVENT(PTR(evt_kaitentou2))
#endif
        SET_VALUE(LW(8), STRING("meta_1_1"))
        SET_VALUE(LW(9), STRING("meta_1_2"))
        SET_VALUE(LW(10), STRING("S_hari04"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_hari)
#else
        SPAWN_EVENT(PTR(evt_hari))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_2_PRESSED, 1)
#else
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_2_PRESSED, 1)
#endif
        SET_VALUE(LW(10), STRING("S_pato2"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_kaitentou2)
#else
        SPAWN_EVENT(PTR(evt_kaitentou2))
#endif
        SET_VALUE(LW(8), STRING("meta_2_1"))
        SET_VALUE(LW(9), STRING("meta_2_2"))
        SET_VALUE(LW(10), STRING("S_hari05"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_hari)
#else
        SPAWN_EVENT(PTR(evt_hari))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_3_PRESSED, 1)
#else
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_3_PRESSED, 1)
#endif
        SET_VALUE(LW(10), STRING("S_pato3"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_kaitentou2)
#else
        SPAWN_EVENT(PTR(evt_kaitentou2))
#endif
        SET_VALUE(LW(8), STRING("meta_3_1"))
        SET_VALUE(LW(9), STRING("meta_3_2"))
        SET_VALUE(LW(10), STRING("S_hari06"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_hari)
#else
        SPAWN_EVENT(PTR(evt_hari))
#endif
    END_IF
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_pato4"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_US
    // Guess: the spinning lamps (kaitentou) and hands (hari) start moving once the crane is active.
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
#else
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
#endif
        SET_VALUE(LW(10), STRING("S_pato4"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_kaitentou)
#else
        SPAWN_EVENT(PTR(evt_kaitentou))
#endif
        SET_VALUE(LW(8), -1)
        SET_VALUE(LW(9), -1)
        SET_VALUE(LW(10), STRING("S_hari01"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_hari)
#else
        SPAWN_EVENT(PTR(evt_hari))
#endif
        SET_VALUE(LW(8), -1)
        SET_VALUE(LW(9), -1)
        SET_VALUE(LW(10), STRING("S_hari02"))
#if VERSION <= VERSION_DEMO_US
        SPAWN_EVENT(evt_hari)
#else
        SPAWN_EVENT(PTR(evt_hari))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
    // The metal covers are shown or hidden according to whether the crane is active.
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
#else
    IF_INT_EQUAL(GSWF_AJI_03_SWITCH_PUZZLE_SOLVED, 1)
#endif
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_1_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_2_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("meta_3_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("meta_1_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("meta_2_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("meta_3_2"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("meta_1_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("meta_2_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("meta_3_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_1_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_2_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("meta_3_2"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    // The prizes sit in a grid: coins, and the Feeling Fine badges and a Star Piece if not yet taken.
    CALL(evt_item_entry, STRING("keihin00"), ITEM_COIN, -150, 50, 100, ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_entry, STRING("keihin01"), ITEM_COIN, -100, 50, 100, ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_entry, STRING("keihin02"), ITEM_COIN, -50, 50, 100, ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_entry, STRING("keihin03"), ITEM_COIN, -150, 50, 0, ITEM_TYPE_STILL, -1, 0)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_03_FEELING_FINE, 0)
#else
    IF_INT_EQUAL(GSWF_AJI_03_FEELING_FINE, 0)
#endif
        CALL(evt_item_entry, STRING("keihin04"), ITEM_FEELING_FINE, -100, 50, 0, ITEM_TYPE_STILL, -1, PTR(badge_evt))
    END_IF
    CALL(evt_item_entry, STRING("keihin05"), ITEM_COIN, -50, 50, 0, ITEM_TYPE_STILL, -1, 0)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_AJI_03_FEELING_FINE_P, 0)
#else
    IF_INT_EQUAL(GSWF_AJI_03_FEELING_FINE_P, 0)
#endif
        CALL(evt_item_entry, STRING("keihin06"), ITEM_FEELING_FINE_P, -150, 50, -100, ITEM_TYPE_STILL, -1, PTR(badge_evt2))
    END_IF
    CALL(evt_item_entry, STRING("keihin07"), ITEM_COIN, -100, 50, -100, ITEM_TYPE_STILL, -1, 0)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSWF_STARPIECE_98, 0)
#else
    IF_INT_EQUAL(GSWF_STARPIECE_98, 0)
#endif
        CALL(evt_item_entry, STRING("keihin08"), ITEM_STAR_PIECE, -50, 50, -100, ITEM_TYPE_STILL, -1, PTR(badge_evt3))
    END_IF
    // The emergency alarm script starts.
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(evt_emergency)
#else
    SPAWN_EVENT(PTR(evt_emergency))
#endif
    RETURN
EVT_END()

USER_FUNC(evt_sousa_icon) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 x = evtGetValue(event, args[1]);
    s32 y = evtGetValue(event, args[2]);

    event->userdata[0] = type;
    event->userdata[1] = x;
    event->userdata[2] = y;
    dispEntry(CAMERA_2D, 0.0f, 0, icon_disp, event);
    return EVT_RETURN_YIELD;
}

#if VERSION <= VERSION_DEMO_US
static void icon_disp(CameraId camera, void* param) {
#else
void icon_disp(CameraId camera, void* param) {
#endif
    EventEntry* event = param;
    Mtx trans;
    Mtx scale;
    Mtx rot;

    camGetPtr(camera);
    if (event->userdata[0] == 0) {
        PSMTXTrans(trans, event->userdata[1] - 70, event->userdata[2], 0.0f);
        PSMTXScale(scale, 1.5f, 1.5f, 1.5f);
        PSMTXRotRad(rot, 'z', 0.0f);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(trans, rot, trans);
#if VERSION <= VERSION_DEMO_US
iconDispGx2(trans, 0x4, ICON_ARROW);
#else
        iconDispGx2(trans, ICON_FLAG_2D, ICON_ARROW);
#endif
        PSMTXTrans(trans, event->userdata[1], event->userdata[2], 0.0f);
        PSMTXScale(scale, 1.5f, 1.5f, 1.5f);
        PSMTXConcat(trans, scale, trans);
#if VERSION <= VERSION_DEMO_US
iconDispGx2(trans, 0x4, ICON_BUTTON_A);
#else
        iconDispGx2(trans, ICON_FLAG_2D, ICON_BUTTON_A);
#endif
    } else {
        PSMTXTrans(trans, event->userdata[1] - 30, event->userdata[2] + 100, 0.0f);
        PSMTXScale(scale, 1.5f, 1.5f, 1.5f);
        PSMTXRotRad(rot, 'z', 4.712389f);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(trans, rot, trans);
#if VERSION <= VERSION_DEMO_US
iconDispGx2(trans, 0x4, ICON_ARROW);
#else
        iconDispGx2(trans, ICON_FLAG_2D, ICON_ARROW);
#endif
        PSMTXTrans(trans, event->userdata[1], event->userdata[2], 0.0f);
        PSMTXScale(scale, 1.5f, 1.5f, 1.5f);
        PSMTXConcat(trans, scale, trans);
#if VERSION <= VERSION_DEMO_US
iconDispGx2(trans, 0x4, ICON_BUTTON_B);
#else
        iconDispGx2(trans, ICON_FLAG_2D, ICON_BUTTON_B);
#endif
    }
}

/** Script function: looks up the item named in args[0] and returns its item ID in args[1] and the name in args[2]. */
USER_FUNC(evt_itemNameToItemID) {
    static char tmp[64];
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    ItemEntry* item = itemNameToPtr(name);

    strncpy(tmp, name, sizeof(tmp));
    evtSetValue(event, args[1], item->itemId);
    evtSetValue(event, args[2], (EvtWord)name);
    return EVT_RETURN_DONE;
}

/**
 * Script function: finds the crane prize closest (within 20 units) to the claw position (GW(10), GW(11)) among
 * the items named keihin00 to keihin31.
 * Returns TRUE and the prize's name in args[0] and args[1], or FALSE and 0 when nothing is within reach.
 */
USER_FUNC(item_chk) {
    char name[256];
    EvtWord* args = event->args;
    f32 x = evtGetValue(event, GW(10));
    f32 z = evtGetValue(event, GW(11));
    f32 minDist = 20.0f;
    s32 i;
    ItemEntry* found = NULL;
    ItemEntry* item;

    for (i = 0; i < 32; i++) {
        sprintf(name, "keihin%02d", i);
        item = itemNameToPtr(name);
        if (item != NULL) {
            f32 dist = distABf(item->position.x, item->position.z, x, z);
            // Choose the nearest item strictly within 20 units in XZ; equal distances keep the earlier index.
            if (dist < minDist) {
                minDist = dist;
                found = item;
            }
        }
    }
    if (found != NULL) {
        evtSetValue(event, args[0], TRUE);
        evtSetValue(event, args[1], (EvtWord)found->name);
    } else {
        evtSetValue(event, args[0], FALSE);
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}


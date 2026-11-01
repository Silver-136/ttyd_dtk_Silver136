// gon_05 (ゴンババ, Hooktail Castle): hub room with doors to gon_04, gon_06, gon_09 and gon_10.
// Floating-block switch puzzles and locks opened with Castle Keys 0x0C and 0x0E (story 47).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_window.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "mario/mario_pouch.h"
#include "evt/evt_cam.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "gon.h"


static EvtWord door01_close[12];
static EvtWord door01_open[12];
static EvtWord door02_close[12];
static EvtWord door02_open[12];
static EvtWord door03_close[12];
static EvtWord door03_open[12];
EvtWord door04_close[22];
EvtWord door04_open[22];
EvtWord door05_close[12];
EvtWord door05_open[12];
static BeroEntry bero_entry_data[7];
static EvtWord init_0[37];
#if VERSION >= VERSION_US
static EvtWord main_0[465];
#else
static EvtWord main_0[491];
#endif
EvtWord init_1[37];
#if VERSION >= VERSION_US
EvtWord main_1[465];
#else
EvtWord main_1[491];
#endif
s32 key_tbl00[2];
EvtWord evt_chk00[23];
static EvtWord evt_unlock00[10];
s32 key_tbl01[2];
EvtWord evt_chk01[23];
static EvtWord evt_unlock01[10];
static EvtWord key_get[5];
EvtWord gon_05_init_evt[338];

/**
 * Door animation callback, closing for the w_bero_1 exit to gon_04:
 * rotates map object "door_01" about Y by 0.444 times the value in LW(0).
 */
static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_01"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the w_bero_1 exit to gon_04:
 * rotates map object "door_01" about Y by -0.444 times the value in LW(0).
 */
static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_01"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, closing for the w_bero_3 exit to gon_04:
 * rotates map object "door_02" about Y by 0.444 times the value in LW(0).
 */
static EVT_BEGIN(door02_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the w_bero_3 exit to gon_04:
 * rotates map object "door_02" about Y by -0.444 times the value in LW(0).
 */
static EVT_BEGIN(door02_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, closing for the w_bero_4 exit to gon_09:
 * rotates map object "door_03" about Y by 0.444 times the value in LW(0).
 */
static EVT_BEGIN(door03_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_03"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the w_bero_4 exit to gon_09:
 * rotates map object "door_03" about Y by -0.444 times the value in LW(0).
 */
static EVT_BEGIN(door03_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_03"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation for the double door (door04_1 and door04_2) in gon_05: LW(0) is the interpolated angle;
 * the two leaves rotate opposite ways to close.
 */
EVT_BEGIN(door04_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door04_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Door animation that opens the double door door04_1 and door04_2 in gon_05 (the reverse of door04_close). */
EVT_BEGIN(door04_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door04_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Door animation that closes the single door door_05 in gon_05. */
EVT_BEGIN(door05_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_05"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Door animation that opens the single door door_05 in gon_05. */
EVT_BEGIN(door05_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_05"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Exits (beros) of this room: w_bero_1 to gon_04, w_bero_2 to gon_04, w_bero_3 to gon_04, w_bero_4 to gon_09,
 * w_bero_5 to gon_10 and e_bero to gon_06.
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        12, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_04", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_04", // nextMapName
        "e_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_3", // hitName
        1, // type
        12, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_04", // nextMapName
        "e_bero_3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door02_close, // enterAnimeArg
        door02_open, // exitAnimeArg
    },
    {
        "w_bero_4", // hitName
        1, // type
        12, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_09", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door03_close, // enterAnimeArg
        door03_open, // exitAnimeArg
    },
    {
        "w_bero_5", // hitName
        1, // type
        12, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_10", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door04_close, // enterAnimeArg
        door04_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        12, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_06", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door05_close, // enterAnimeArg
        door05_open, // exitAnimeArg
    },
    {0},
};

/**
 * Map-init helper for the floating block blk_0 and its switch (sw_0):
 * if the local flag LSWF(4) says the switch has been used,
 * it lifts the block 525 units above its resting height.
 */
// Block 0 starts 525 units above its placed position when its area flag is clear.
static EVT_BEGIN(init_0)
    IF_INT_EQUAL(1, 0)
        IF_INT_NOT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 0)
            CALL(evt_mobj_get_y_position, STRING("blk_0"), LW(5))
            ADD_FLOAT(LW(5), FLOAT(525.0f))
            CALL(evt_mobj_set_y_position, STRING("blk_0"), LW(5))
        END_IF
        ELSE
        IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 0)
            CALL(evt_mobj_get_y_position, STRING("blk_0"), LW(5))
            ADD_FLOAT(LW(5), FLOAT(525.0f))
            CALL(evt_mobj_set_y_position, STRING("blk_0"), LW(5))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Handler of the sw_0 switch for the floating block blk_0:
 * the camera shakes with rumble while the block rises
 * or falls 525 units (the direction follows LSWF(4), which is toggled), with block-float sounds;
 * if Mario or his partner bumps the block with the head during the move,
 * it reverses and the switch is cancelled. Ends with a landing sound and the final shake.
 */
#if VERSION <= VERSION_DEMO_US
static EVT_BEGIN(main_0)
#else
static EVT_BEGIN(main_0)
    SET_TYPE(239)
#endif
    // The switch is pressed: the block stops reacting to hits while it moves.
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
    IF_INT_NOT_EQUAL(1, 0)
        // The block rises if it is down and falls if it is up (remembered in a room flag).
        SET_VALUE(LW(10), 0)
        IF_INT_EQUAL(1, 0)
            IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 0)
                SET_VALUE(LW(10), 0)
                ELSE
                SET_VALUE(LW(10), 1)
            END_IF
            ELSE
            IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 0)
                SET_VALUE(LW(10), 1)
                ELSE
                SET_VALUE(LW(10), 0)
            END_IF
        END_IF
        // Work out the target height, 525 units from here, and the speed.
        IF_INT_EQUAL(LW(10), 0)
            CALL(evt_mobj_get_y_position, STRING("blk_0"), LW(2))
            SET_FLOAT_VALUE(LW(3), LW(2))
            ADD_FLOAT(LW(3), FLOAT(525.0f))
            SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
            ELSE
            CALL(evt_mobj_get_y_position, STRING("blk_0"), LW(2))
            SET_FLOAT_VALUE(LW(3), LW(2))
            SUBTRACT_FLOAT(LW(3), FLOAT(525.0f))
            SET_FLOAT_VALUE(LW(8), FLOAT(-0.19921875f))
            SET_FLOAT_VALUE(LW(9), 0)
        END_IF
#if VERSION >= VERSION_US
        BEGIN_INLINE_EVENT
#else
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(10)
            CALL(evt_sub_rumble_onoff, 0, 0)
            WAIT_MS(300)
            CALL(evt_sub_rumble_onoff, 1, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
#endif
            WAIT_FRAMES(10)
            CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 3, 4)
            LABEL(0)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), LW(0), 100)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
#if VERSION >= VERSION_US
        END_INLINE_EVENT
#else
        END_SIBLING_EVENT
#endif
        // After the camera shake, the block starts to float and its movement sound plays.
        WAIT_MS(500)
        CALL(evt_mobj_get_position, STRING("blk_0"), LW(12), LW(13), LW(14))
        CALL(evt_mobj_get_kindname, STRING("blk_0"), LW(11))
        SET_VALUE(LF(0), 0)
        IF_STR_EQUAL(LW(11), STRING("MOBJ_green_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(11), STRING("MOBJ_purple_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(11), STRING("MOBJ_orange_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LF(1), 0)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_MOVE1"), LW(12), LW(13), LW(14), LW(15))
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_MOVE1"), LW(12), LW(13), LW(14), LW(15))
            END_IF
            ELSE
            SET_VALUE(LF(1), 1)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_DOWN1"), LW(12), LW(13), LW(14), LW(15))
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_DOWN1"), LW(12), LW(13), LW(14), LW(15))
            END_IF
        END_IF
        SET_FLOAT_VALUE(LW(4), LW(2))
        SET_VALUE(LF(0), 0)
        // Each frame the block moves toward its target height.
        LOOP(0)
            // BUG: with no partner out the head test uses the bare mobj name, but mobj hit objects are
            // named "MOBJ_" + name, so it never matches; with a partner it tests LW(7), which nothing sets,
            // instead of the partner's head object.
            CALL(evt_mario_get_party, LW(5))
            IF_INT_EQUAL(LW(5), 0)
                CALL(evt_mario_get_name_hitobj_head, LW(6))
                IF_STR_EQUAL(LW(6), STRING("blk_0"))
                    GOTO(1)
                END_IF
                ELSE
                CALL(evt_mario_get_name_hitobj_head, LW(6))
                IF_STR_EQUAL(LW(6), STRING("MOBJ_blk_0"))
                    GOTO(1)
                END_IF
                IF_STR_EQUAL(LW(7), STRING("MOBJ_blk_0"))
                    GOTO(1)
                END_IF
            END_IF
            GOTO(2)
            LABEL(1)
            // If a falling block lands on Mario or his partner it turns around and rises again, and the switch
            // is cancelled.
            IF_INT_EQUAL(LW(10), 1)
                SET_FLOAT_VALUE(LW(5), LW(2))
                SET_FLOAT_VALUE(LW(2), LW(3))
                SET_FLOAT_VALUE(LW(3), LW(5))
                SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
                SET_FLOAT_VALUE(LW(8), 0)
                SET_FLOAT_VALUE(LW(10), 0)
                IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 0)
                    SET_VALUE(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 1)
                    ELSE
                    SET_VALUE(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 0)
                END_IF
                CALL(evt_mobj_exec_cancel, STRING("sw_0"))
            END_IF
            LABEL(2)
            ADD_FLOAT(LW(4), LW(9))
            ADD_FLOAT(LW(9), LW(8))
            IF_FLOAT_GREATER_OR_EQUAL(LW(9), FLOAT(4.0f))
                SET_FLOAT_VALUE(LW(9), FLOAT(4.0f))
            END_IF
            CALL(evt_mobj_get_position, STRING("blk_0"), LW(11), LW(12), LW(13))
            CALL(evt_snd_sfx_pos, LW(15), LW(11), LW(12), LW(13))
            IF_INT_EQUAL(LW(10), 0)
                IF_INT_GREATER_OR_EQUAL(LW(4), LW(3))
                    SET_FLOAT_VALUE(LW(4), LW(3))
                    SET_VALUE(LF(0), 1)
                END_IF
                ELSE
                IF_INT_LESS_OR_EQUAL(LW(4), LW(3))
                    SET_FLOAT_VALUE(LW(4), LW(3))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(evt_mobj_set_y_position, STRING("blk_0"), LW(4))
            IF_INT_EQUAL(LF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        IF_INT_EQUAL(LF(1), 0)
            // The block has arrived: the moving sound stops.
            CALL(evt_snd_sfxoff, LW(15))
        END_IF
        // The landing thud plays for a block that came down.
        BEGIN_INLINE_EVENT
            CALL(evt_mobj_get_position, STRING("blk_0"), LW(13), LW(14), LW(15))
            CALL(evt_mobj_get_kindname, STRING("blk_0"), LW(12))
            SET_VALUE(LF(0), 0)
            IF_STR_EQUAL(LW(12), STRING("MOBJ_green_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(12), STRING("MOBJ_purple_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(12), STRING("MOBJ_orange_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LW(10), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_STOP1"), LW(13), LW(14), LW(15), 0)
            END_IF
        END_INLINE_EVENT
        // A block that came down shakes the camera again and the scene waits a moment.
        IF_INT_EQUAL(LW(10), 1)
#if VERSION >= VERSION_US
            BEGIN_INLINE_EVENT
#else
            BEGIN_SIBLING_EVENT
                CALL(evt_sub_rumble_onoff, 0, 0)
                WAIT_MS(300)
                CALL(evt_sub_rumble_onoff, 1, 0)
            END_SIBLING_EVENT
            BEGIN_SIBLING_EVENT
#endif
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 3, 10)
                LABEL(2)
                CALL(evt_sub_intpl_get_value)
                DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
                CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), LW(0), 100)
                IF_INT_EQUAL(LW(1), 1)
                    GOTO(2)
                END_IF
#if VERSION >= VERSION_US
            END_INLINE_EVENT
#else
            END_SIBLING_EVENT
#endif
            WAIT_MS(1000)
        END_IF
        // Remember the block's new position for the next press.
        IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 0)
            SET_VALUE(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 1)
            ELSE
            SET_VALUE(LSWF_GON_05_FLOAT_BLOCK_0_MOVED, 0)
        END_IF
        ELSE
        // Unused: cancel the switch and wait a few frames.
        CALL(evt_mobj_exec_cancel, STRING("sw_0"))
        SET_VALUE(LW(0), 0)
        LOOP(0)
            WAIT_FRAMES(2)
            SET_VALUE(LW(1), LW(0))
            MODULO(LW(1), 2)
            ADD(LW(0), 1)
            IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

/**
 * Map-init helper for the floating block blk_1:
 * moves it 489 units up if its saved state LSWF(5) says it was raised.
 */
// Block 1 starts 489 units above its placed position when its area flag is set.
EVT_BEGIN(init_1)
    IF_INT_EQUAL(0, 0)
        IF_INT_NOT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 0)
            CALL(evt_mobj_get_y_position, STRING("blk_1"), LW(5))
            ADD_FLOAT(LW(5), FLOAT(489.0f))
            CALL(evt_mobj_set_y_position, STRING("blk_1"), LW(5))
        END_IF
        ELSE
        IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 0)
            CALL(evt_mobj_get_y_position, STRING("blk_1"), LW(5))
            ADD_FLOAT(LW(5), FLOAT(489.0f))
            CALL(evt_mobj_set_y_position, STRING("blk_1"), LW(5))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Runs when Mario hits the floating-block switch sw_1:
 * lifts or lowers blk_1 by 489 units with its sound and a camera shake, and flips the saved state LSWF(5).
 * If the block cannot move the switch press is cancelled.
 */
#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(main_1)
    SET_TYPE(239)
    // The switch is pressed: the block stops reacting to hits while it moves.
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
#else
EVT_BEGIN(main_1)
    SET_TYPE(239)
    // The switch is pressed: the block stops reacting to hits while it moves.
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
#endif
    IF_INT_NOT_EQUAL(1, 0)
        // The block rises if it is down and falls if it is up (remembered in a room flag).
        SET_VALUE(LW(10), 0)
        IF_INT_EQUAL(0, 0)
            IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 0)
                SET_VALUE(LW(10), 0)
                ELSE
                SET_VALUE(LW(10), 1)
            END_IF
            ELSE
            IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 0)
                SET_VALUE(LW(10), 1)
                ELSE
                SET_VALUE(LW(10), 0)
            END_IF
        END_IF
        // Work out the target height, 525 units from here, and the speed.
        IF_INT_EQUAL(LW(10), 0)
            CALL(evt_mobj_get_y_position, STRING("blk_1"), LW(2))
            SET_FLOAT_VALUE(LW(3), LW(2))
            ADD_FLOAT(LW(3), FLOAT(489.0f))
            SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
            ELSE
            CALL(evt_mobj_get_y_position, STRING("blk_1"), LW(2))
            SET_FLOAT_VALUE(LW(3), LW(2))
            SUBTRACT_FLOAT(LW(3), FLOAT(489.0f))
            SET_FLOAT_VALUE(LW(8), FLOAT(-0.19921875f))
            SET_FLOAT_VALUE(LW(9), 0)
        END_IF
#if VERSION >= VERSION_US
        BEGIN_INLINE_EVENT
#else
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(10)
            CALL(evt_sub_rumble_onoff, 0, 0)
            WAIT_MS(300)
            CALL(evt_sub_rumble_onoff, 1, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
#endif
            WAIT_FRAMES(10)
            CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 3, 4)
            LABEL(0)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), LW(0), 100)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
#if VERSION >= VERSION_US
        END_INLINE_EVENT
#else
        END_SIBLING_EVENT
#endif
        // After the camera shake, the block starts to float and its movement sound plays.
        WAIT_MS(500)
        CALL(evt_mobj_get_position, STRING("blk_1"), LW(12), LW(13), LW(14))
        CALL(evt_mobj_get_kindname, STRING("blk_1"), LW(11))
        SET_VALUE(LF(0), 0)
        IF_STR_EQUAL(LW(11), STRING("MOBJ_green_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(11), STRING("MOBJ_purple_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(11), STRING("MOBJ_orange_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LF(1), 0)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_MOVE1"), LW(12), LW(13), LW(14), LW(15))
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_MOVE1"), LW(12), LW(13), LW(14), LW(15))
            END_IF
            ELSE
            SET_VALUE(LF(1), 1)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_DOWN1"), LW(12), LW(13), LW(14), LW(15))
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_DOWN1"), LW(12), LW(13), LW(14), LW(15))
            END_IF
        END_IF
        SET_FLOAT_VALUE(LW(4), LW(2))
        SET_VALUE(LF(0), 0)
        // Each frame the block moves toward its target height.
        LOOP(0)
            // BUG: with no partner out the head test uses the bare mobj name, but mobj hit objects are
            // named "MOBJ_" + name, so it never matches; with a partner it tests LW(7), which nothing sets,
            // instead of the partner's head object.
            CALL(evt_mario_get_party, LW(5))
            IF_INT_EQUAL(LW(5), 0)
                CALL(evt_mario_get_name_hitobj_head, LW(6))
                IF_STR_EQUAL(LW(6), STRING("blk_1"))
                    GOTO(1)
                END_IF
                ELSE
                CALL(evt_mario_get_name_hitobj_head, LW(6))
                IF_STR_EQUAL(LW(6), STRING("MOBJ_blk_1"))
                    GOTO(1)
                END_IF
                IF_STR_EQUAL(LW(7), STRING("MOBJ_blk_1"))
                    GOTO(1)
                END_IF
            END_IF
            GOTO(2)
            LABEL(1)
            // If a falling block lands on Mario or his partner it turns around and rises again, and the switch
            // is cancelled.
            IF_INT_EQUAL(LW(10), 1)
                SET_FLOAT_VALUE(LW(5), LW(2))
                SET_FLOAT_VALUE(LW(2), LW(3))
                SET_FLOAT_VALUE(LW(3), LW(5))
                SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
                SET_FLOAT_VALUE(LW(8), 0)
                SET_FLOAT_VALUE(LW(10), 0)
                IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 0)
                    SET_VALUE(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 1)
                    ELSE
                    SET_VALUE(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 0)
                END_IF
                CALL(evt_mobj_exec_cancel, STRING("sw_1"))
            END_IF
            LABEL(2)
            ADD_FLOAT(LW(4), LW(9))
            ADD_FLOAT(LW(9), LW(8))
            IF_FLOAT_GREATER_OR_EQUAL(LW(9), FLOAT(4.0f))
                SET_FLOAT_VALUE(LW(9), FLOAT(4.0f))
            END_IF
            CALL(evt_mobj_get_position, STRING("blk_1"), LW(11), LW(12), LW(13))
            CALL(evt_snd_sfx_pos, LW(15), LW(11), LW(12), LW(13))
            IF_INT_EQUAL(LW(10), 0)
                IF_INT_GREATER_OR_EQUAL(LW(4), LW(3))
                    SET_FLOAT_VALUE(LW(4), LW(3))
                    SET_VALUE(LF(0), 1)
                END_IF
                ELSE
                IF_INT_LESS_OR_EQUAL(LW(4), LW(3))
                    SET_FLOAT_VALUE(LW(4), LW(3))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(evt_mobj_set_y_position, STRING("blk_1"), LW(4))
            IF_INT_EQUAL(LF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        IF_INT_EQUAL(LF(1), 0)
            // The block has arrived: the moving sound stops.
            CALL(evt_snd_sfxoff, LW(15))
        END_IF
        // The landing thud plays for a block that came down.
        BEGIN_INLINE_EVENT
            CALL(evt_mobj_get_position, STRING("blk_1"), LW(13), LW(14), LW(15))
            CALL(evt_mobj_get_kindname, STRING("blk_1"), LW(12))
            SET_VALUE(LF(0), 0)
            IF_STR_EQUAL(LW(12), STRING("MOBJ_green_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(12), STRING("MOBJ_purple_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(12), STRING("MOBJ_orange_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LW(10), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_STOP1"), LW(13), LW(14), LW(15), 0)
            END_IF
        END_INLINE_EVENT
        // A block that came down shakes the camera again and the scene waits a moment.
        IF_INT_EQUAL(LW(10), 1)
#if VERSION >= VERSION_US
            BEGIN_INLINE_EVENT
#else
            BEGIN_SIBLING_EVENT
                CALL(evt_sub_rumble_onoff, 0, 0)
                WAIT_MS(300)
                CALL(evt_sub_rumble_onoff, 1, 0)
            END_SIBLING_EVENT
            BEGIN_SIBLING_EVENT
#endif
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 3, 10)
                LABEL(2)
                CALL(evt_sub_intpl_get_value)
                DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
                CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), LW(0), 100)
                IF_INT_EQUAL(LW(1), 1)
                    GOTO(2)
                END_IF
#if VERSION >= VERSION_US
            END_INLINE_EVENT
#else
            END_SIBLING_EVENT
#endif
            WAIT_MS(1000)
        END_IF
        // Remember the block's new position for the next press.
        IF_INT_EQUAL(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 0)
            SET_VALUE(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 1)
            ELSE
            SET_VALUE(LSWF_GON_05_FLOAT_BLOCK_1_MOVED, 0)
        END_IF
        ELSE
        // Unused: cancel the switch and wait a few frames.
        CALL(evt_mobj_exec_cancel, STRING("sw_1"))
        SET_VALUE(LW(0), 0)
        LOOP(0)
            WAIT_FRAMES(2)
            SET_VALUE(LW(1), LW(0))
            MODULO(LW(1), 2)
            ADD(LW(0), 1)
            IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

/** Items offered in the item menu at the first lock: only the Castle Key (0x0C). */
s32 key_tbl00[] = {
    ITEM_CASTLE_KEY_0x0C, -1,
};

/**
 * Check run when Mario tries the first lock (Castle Key 0x0C):
 * if he has the key it asks him to pick it from the item list and cancels the unlock when he picks nothing.
 */
EVT_BEGIN(evt_chk00)
    CALL(evt_pouch_check_item, ITEM_CASTLE_KEY_0x0C, LW(0))
    IF_INT_GREATER(LW(0), 0)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_win_item_select, PTR(key_tbl00), LW(0))
#else
        CALL(evt_win_item_select, 0, PTR(key_tbl00), LW(0), 0)
#endif
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Runs when the first locked door is opened with the key:
 * switches on the w_bero_3 bero case and sets the story to STG1_13_03_LOCK_2.
 */
static EVT_BEGIN(evt_unlock00)
    SET_VALUE(LW(0), STRING("w_bero_3"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, STG1_13_03_LOCK_2)
    RETURN
EVT_END()

/** Items offered in the item menu at the second lock: only the Castle Key (0x0E). */
s32 key_tbl01[] = {
    ITEM_CASTLE_KEY_0x0E, -1,
};

/** Check run when Mario tries the second lock (Castle Key 0x0E): same as evt_chk00 for that key. */
EVT_BEGIN(evt_chk01)
    CALL(evt_pouch_check_item, ITEM_CASTLE_KEY_0x0E, LW(0))
    IF_INT_GREATER(LW(0), 0)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_win_item_select, PTR(key_tbl01), LW(0))
#else
        CALL(evt_win_item_select, 0, PTR(key_tbl01), LW(0), 0)
#endif
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Runs when the second locked door is opened with the key:
 * switches on the w_bero_5 bero case and sets the story to STG1_15_04_LOCK_4.
 */
static EVT_BEGIN(evt_unlock01)
    SET_VALUE(LW(0), STRING("w_bero_5"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, STG1_15_04_LOCK_4)
    RETURN
EVT_END()

/** Runs when Mario picks up the fourth Hooktail Castle key in this room: sets the story to STG1_15_03_KEY_4. */
static EVT_BEGIN(key_get)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_15_03_KEY_4)
    RETURN
EVT_END()

/**
 * Map init for gon_05 (hub room): sets up the doors, dormant Dry Bones,
 * the two floating blocks (blk_0 and blk_1) with their switches, the Castle Key 0x0E,
 * the Star Piece and Last Stand P badge, the two keyed locks (Castle Key 0x0C to STG1_13_03_LOCK_2 and
 * 0x0E to STG1_15_04_LOCK_4, each with its door), the save block and the Shine Sprite power-up block.
 */
EVT_BEGIN(gon_05_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_npc_entry, STRING("\"カロン00\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン00\""), -203, 0, 150)
    CALL(evt_npc_set_rotate, STRING("\"カロン00\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン00\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン00\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン00\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン00\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン00\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン00\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン01\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン01\""), -120, 0, 51)
    CALL(evt_npc_set_rotate, STRING("\"カロン01\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン01\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン01\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン01\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン01\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン01\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン01\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン02\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン02\""), 25, 0, 119)
    CALL(evt_npc_set_rotate, STRING("\"カロン02\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン02\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン02\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン02\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン02\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン02\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン02\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン03\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン03\""), 87, 0, -129)
    CALL(evt_npc_set_rotate, STRING("\"カロン03\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン03\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン03\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン03\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン03\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン03\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン03\""), 7)
    CALL(evt_mobj_float_blk, STRING("blk_0"), 120, 815, 150, 2, 2, 0, LSWF_GON_05_FLOAT_BLOCK_0_MOVED)
    CALL_CHILD(PTR(init_0))
    CALL(evt_mobj_switch_float_blk, STRING("sw_0"), 20, 1399, 150, STRING("blk_0"), 2, PTR(main_0), LSWF_GON_05_FLOAT_BLOCK_0_MOVED)
    CALL(evt_mobj_flag_onoff, 1, STRING("blk_0"), MOBJ_FLAG_NO_CULL)
    CALL(evt_mobj_float_blk, STRING("blk_1"), 200, 0, -150, 0, 2, 0, LSWF_GON_05_FLOAT_BLOCK_1_MOVED)
    CALL_CHILD(PTR(init_1))
    CALL(evt_mobj_switch_float_blk, STRING("sw_1"), FLOAT(-2.5f), 100, -175, STRING("blk_1"), 0, PTR(main_1), LSWF_GON_05_FLOAT_BLOCK_1_MOVED)
    CALL(evt_mobj_flag_onoff, 1, STRING("blk_1"), MOBJ_FLAG_NO_CULL)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_15_03_KEY_4)
        CALL(evt_item_entry, STRING("key00"), ITEM_CASTLE_KEY_0x0E, 200, 1340, 150, ITEM_TYPE_STILL, -1, PTR(key_get))
    END_IF
    CALL(evt_item_entry, STRING("item00"), ITEM_STAR_PIECE, -200, 240, -150, ITEM_TYPE_STILL, GSWF_STARPIECE_39, 0)
    CALL(evt_item_entry, STRING("item01"), ITEM_LAST_STAND_P, 210, 915, 150, ITEM_TYPE_STILL, GSWF_GON_05_LAST_STAND_P, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_13_03_LOCK_2)
        CALL(evt_mobj_lock, STRING("mobj_lock00"), ITEM_CASTLE_KEY_0x0C, -240, 750, 0, 90, PTR(evt_chk00), PTR(evt_unlock00), GSWF_GON_05_LOWER_DOOR_UNLOCKED)
        SET_VALUE(LW(0), STRING("w_bero_3"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_15_04_LOCK_4)
        CALL(evt_mobj_lock, STRING("mobj_lock01"), ITEM_CASTLE_KEY_0x0E, -240, 1900, 0, 90, PTR(evt_chk01), PTR(evt_unlock01), GSWF_GON_05_UPPER_DOOR_UNLOCKED)
        SET_VALUE(LW(0), STRING("w_bero_5"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 0, 60, 0, 0, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -200, 1950, -150, 0, GSWF_SHINE_14, 0)
    CALL(evt_map_playanim, STRING("hokori"), MAP_ANIM_PLAY_LOOP, 0)
    RETURN
EVT_END()


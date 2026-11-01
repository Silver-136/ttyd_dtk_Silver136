// jin_02 (ダイム寺院, Creepy Steeple): side room with a red switch and a camera shake event; doors to jin_00 and jin_03.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "jin.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "system.h"
#include "pmario_sound.h"

USER_FUNC(evt_kagemario_init);

static EvtWord n2_close[12];
static EvtWord n2_open[12];
EvtWord n1_close[12];
EvtWord n1_open[12];
static BeroEntry bero_entry_data[3];
static EvtWord cam_shake[62];
#include "version.h"
#if VERSION <= VERSION_DEMO_US
extern EvtWord evt_kaidan_temae[];
#else
EvtWord evt_kaidan_temae[144];
#endif
EvtWord jin_02_init_evt[33];

/**
 * Door animation callback: closes door03 as Mario arrives through n_bero_2 (from jin_03);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(n2_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door03 as Mario leaves through n_bero_2 (to jin_03);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(n2_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Door animation callback for the door00 door's closing: rotates it by -0.444 times the value in LW(0). */
EVT_BEGIN(n1_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Door animation callback for the door00 door's opening: rotates it by 0.444 times the value in LW(0). */
EVT_BEGIN(n1_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Doors of this room: the north exit (n_bero_2) to jin_03 and the north exit (n_bero_1) to jin_00. */
static BeroEntry bero_entry_data[] = {
    {
        "n_bero_2", // hitName
        1, // type
        2, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_03", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n2_close, // enterAnimeArg
        n2_open, // exitAnimeArg
    },
    {
        "n_bero_1", // hitName
        1, // type
        2, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_00", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n1_close, // enterAnimeArg
        n1_open, // exitAnimeArg
    },
    {0},
};

/**
 * Rumble of the endless staircase moving one stage:
 * plays SFX_STG4_KAIDAN_MOVE1 at (300, 100, -200) while the camera shake ramps up to 0.010 over 30 steps
 * and back down to 0, then stops the sound.
 */
static EVT_BEGIN(cam_shake)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_KAIDAN_MOVE1"), 300, 100, -200, LW(9))
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 10, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 0, 30)
    LABEL(1)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    RETURN
EVT_END()

/**
 * Red-switch script that moves the endless staircase one stage on in the other direction:
 * same reaction as in jin_01 but the stage counter LSW(1) is raised by 1.
 */
// The front switch increments the staircase position unless its current value is 1.
EVT_BEGIN(evt_kaidan_temae)
#if VERSION <= VERSION_DEMO_US
    IF_INT_NOT_EQUAL(LSW(1), 1)
#else
    IF_INT_NOT_EQUAL(LSW_JIN_STAIRCASE_POS, 1)
#endif
        CALL(evt_mario_key_onoff, 0)
#if VERSION > VERSION_DEMO_US
        CALL(evt_mario_normalize)
#endif
        CALL(evt_party_stop, 0)
#if VERSION <= VERSION_DEMO_US
        IF_INT_EQUAL(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 0)
#else
        IF_INT_EQUAL(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 0)
#endif
            BEGIN_INLINE_EVENT_GET_ID(LW(3))
                WAIT_MS(500)
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(1000)
                LOOP(0)
                    CALL(evt_sub_random, 200, LW(0))
                    ADD(LW(0), 300)
                    CALL(evt_mario_set_dir, 90, LW(0), 0)
                    WAIT_MS(LW(0))
                    CALL(evt_sub_random, 300, LW(0))
                    ADD(LW(0), 400)
                    CALL(evt_mario_set_dir, 270, LW(0), 0)
                    WAIT_MS(LW(0))
                END_LOOP
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT_GET_ID(LW(4))
                WAIT_MS(500)
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(1000)
                LOOP(0)
                    CALL(evt_sub_random, 200, LW(0))
                    ADD(LW(0), 400)
                    CALL(evt_party_set_dir, 0, 90, LW(0))
                    WAIT_MS(LW(0))
                    CALL(evt_sub_random, 200, LW(0))
                    ADD(LW(0), 200)
                    CALL(evt_party_set_dir, 0, 270, LW(0))
                    WAIT_MS(LW(0))
                END_LOOP
            END_INLINE_EVENT
        END_IF
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(cam_shake)
        IF_INT_EQUAL(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 0)
#else
        CALL_CHILD(PTR(cam_shake))
        IF_INT_EQUAL(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 0)
#endif
            WAIT_MS(500)
            DELETE_EVENT(LW(3))
            DELETE_EVENT(LW(4))
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 1)
#else
            SET_VALUE(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 1)
#endif
        END_IF
#if VERSION <= VERSION_DEMO_US
        ADD(LSW(1), 1)
#else
        ADD(LSW_JIN_STAIRCASE_POS, 1)
#endif
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_party_run, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Creepy Steeple jin_02 map init: starts the steeple music and reverb, the shadow-Mario tint,
 * the exits and the red switch for the endless staircase.
 */
EVT_BEGIN(jin_02_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_JIN2"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_mobj_switch_red, 0, STRING("mobj_switch1"), 485, 30, -10, PTR(evt_kaidan_temae), 0)
    RETURN
EVT_END()


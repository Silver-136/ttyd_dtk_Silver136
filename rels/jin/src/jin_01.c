// jin_01 (ダイム寺院, Creepy Steeple): side room with a red switch and a Golden Leaf item; doors to jin_00 and jin_03.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "jin.h"
#include "data_tables.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static EvtWord s1_close[12];
static EvtWord s1_open[12];
static EvtWord s2_close[12];
static EvtWord s2_open[12];
static BeroEntry bero_entry_data[3];
static EvtWord cam_shake[62];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_kaidan_oku[];
#else
EvtWord evt_kaidan_oku[144];
#endif

static EvtWord ki_data_evt_01[20];
static TreeAccess ki_data[2];
EvtWord jin_01_init_evt[41];

/**
 * Door animation callback: closes door00 as Mario arrives through s_bero_1 (from jin_00);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(s1_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door00 as Mario leaves through s_bero_1 (to jin_00);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(s1_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: closes door03 as Mario arrives through s_bero_2 (from jin_03);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(s2_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door03 as Mario leaves through s_bero_2 (to jin_03);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(s2_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Doors of this room: the south exit (s_bero_1) to jin_00 and the south exit (s_bero_2) to jin_03. */
static BeroEntry bero_entry_data[] = {
    {
        "s_bero_1", // hitName
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
        "n_bero_3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s1_close, // enterAnimeArg
        s1_open, // exitAnimeArg
    },
    {
        "s_bero_2", // hitName
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
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s2_close, // enterAnimeArg
        s2_open, // exitAnimeArg
    },
    {0},
};

/**
 * Rumble of the endless staircase moving one stage:
 * plays SFX_STG4_KAIDAN_MOVE1 at (300, 100, 200) while the camera shake ramps up to 0.010 over 30 steps
 * and back down to 0, then stops the sound.
 */
static EVT_BEGIN(cam_shake)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_KAIDAN_MOVE1"), 300, 100, 200, LW(9))
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
 * Red-switch script that moves the endless staircase one stage on:
 * Mario and his partner react in confusion (exclamation mark and random turning the first time),
 * the camera shakes and the stage counter LSW(1) is lowered by 1.
 */
// The rear switch decrements the staircase position when the current value is at most 1.
EVT_BEGIN(evt_kaidan_oku)
    IF_INT_LESS_OR_EQUAL(LSW_JIN_STAIRCASE_POS, 1)
        CALL(evt_mario_key_onoff, 0)
#if VERSION >= VERSION_JP
        CALL(evt_mario_normalize)
#endif
        CALL(evt_party_stop, 0)
        IF_INT_EQUAL(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 0)
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
        CALL_CHILD(cam_shake)
        IF_INT_EQUAL(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 0)
            WAIT_MS(500)
            DELETE_EVENT(LW(3))
            DELETE_EVENT(LW(4))
            SET_VALUE(GSWF_JIN_STAIRS_SWITCH_FIRST_USE_SEEN, 1)
        END_IF
        SUBTRACT(LSW_JIN_STAIRCASE_POS, 1)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_party_run, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Reward script for the golden tree: if no item named item_ki01 is present,
 * drops a Golden Leaf from it (flag LSWF(2)) when Mario hits the tree.
 */
static EVT_BEGIN(ki_data_evt_01)
    CALL(evt_item_delete_check, STRING("item_ki01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ki01"), ITEM_GOLDEN_LEAF, 798, 188, -40, ITEM_TYPE_FALL_NO_PICKUP, LSWF_JIN_01_GOLDEN_LEAF_TREE, 0)
    END_IF
    RETURN
EVT_END()

/** The golden tree Mario can hit (A_gorden_tree); hitting it drops the Golden Leaf. */
static TreeAccess ki_data[] = {
    {2, "A_gorden_tree", "S_golden_tree", NULL, 0, ki_data_evt_01},
    {7, NULL, NULL, NULL, 0, NULL},
};

/**
 * Creepy Steeple jin_01 map init: starts the steeple music and reverb, the shadow-Mario tint, the exits,
 * the red switch for the endless staircase and the golden tree that drops a Golden Leaf.
 */
EVT_BEGIN(jin_01_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_JIN2"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL_CHILD(evt_usu_kagemario_party_kill)
    CALL(evt_mobj_switch_red, 0, STRING("mobj_switch1"), 485, 30, 10, PTR(evt_kaidan_oku), 0)
    SET_VALUE(LSWF_JIN_01_GOLDEN_LEAF_TREE, 0)
    SET_VALUE(LW(0), PTR(ki_data))
    CALL_CHILD(evt_sub_tree_access_entry)
    RETURN
EVT_END()


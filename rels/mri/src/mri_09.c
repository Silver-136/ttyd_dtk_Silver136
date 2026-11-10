// mri_09 (巨木の中, The Great Tree): the room with the four rolling wall panels (wall_roll), the slot
// switches, the breaking floor, and the blue jail key (get_blue_jail_key).

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "mri_puni.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_party.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

static BeroEntry bero_bluejail[1];
static BeroEntry bero_entry_data[5];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord wall_roll_sub[];
#else
EvtWord wall_roll_sub[96];
#endif

#if VERSION >= VERSION_US
EvtWord wall_roll[272];
#else
#if VERSION <= VERSION_DEMO_JP
EvtWord wall_roll[262];
#else
EvtWord wall_roll[264];
#endif
#endif

#if VERSION <= VERSION_DEMO_JP
static EvtWord breakyuka_init[11];
#else
static EvtWord breakyuka_init[17];
#endif

static EvtWord breakyuka_main[9];
static KusaAccess ueki_access_data[6];
static EvtWord slot00_event[52];
static EvtWord slot01_event[52];
static EvtWord slot02_event[52];
static EvtWord slot03_event[52];
static EvtWord slot_event[66];
static EvtWord get_blue_jail_key[143];

#if VERSION >= VERSION_US
EvtWord mri_09_init_evt[230];
#else
#if VERSION <= VERSION_DEMO_JP
EvtWord mri_09_init_evt[216];
#else
EvtWord mri_09_init_evt[228];
#endif
#endif

/** Exit of mri_09: west road w_bero to mri_03. */
static BeroEntry bero_bluejail[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_03", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

/**
 * Exits of mri_09: west road w_bero to mri_08 (e_bero); north road n_bero to mri_10 (s_bero); pipe dokan2
 * to dokan3 in this room; pipe dokan3 to dokan2 in this room.
 */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_08", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_10", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan2", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan3", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_W, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Plays the falling-wall paper animation for one wall panel:
 * LW(0) paper name, LW(1) map object, LW(2) image name, LW(3) offscreen name.
 * It captures the panel, shows the roll animation with a slot sound,
 * then hides the panel and releases the capture.
 */
EVT_BEGIN(wall_roll_sub)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_paper_entry, LW(0))
#endif
    CALL(evt_img_entry, LW(2))
    CALL(evt_img_set_paper, LW(2), LW(0))
    CALL(evt_offscreen_entry, LW(3))
    CALL(evt_mapobj_set_offscreen, 1, LW(1), LW(3))
    CALL(evt_offscreen_get_boundingbox, LW(3), LW(10), LW(11), LW(12), LW(13))
    CALL(evt_img_alloc_capture, LW(2), 0, 0, 1, LW(10), LW(11), LW(12), LW(13))
    CALL(evt_img_clear_virtual_point, LW(2))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mapobj_get_position, LW(1), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF2_SLOT_GOOUT1"), LW(10), LW(11), LW(12), LW(15))
        WAIT_MS(1800)
        CALL(evt_snd_sfxoff, LW(15))
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, LW(2), STRING("A_1"))
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_img_wait_animend, LW(2))
    CALL(evt_img_free_capture, LW(2), 0)
    CALL(evt_img_free_capture, LW(2), 1)
    CALL(evt_img_onoff, LW(2), 0)
    CALL(evt_img_release, LW(2))
    CALL(evt_mapobj_clear_offscreen, 1, LW(1))
    CALL(evt_offscreen_delete, LW(3))
    RETURN
EVT_END()

/**
 * Reward scene for the slot puzzle: with a quake and fanfare the four wall panels roll away one by one,
 * the floor and wall collisions change, small stars burst from the four switches, which are removed.
 * Sets the story to STG2_21_02_MATCHING_THE_PATTERNS.
 */
EVT_BEGIN(wall_roll)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
#if VERSION >= VERSION_US
    CALL(evt_party_cloud_lock_animations_on_off, 0, 1)
#endif
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG2_QUAKE2"), LW(10))
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.01953125f), FLOAT(0.0f), 1000)
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 500)
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 159, 399, 0, 86, -18, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    SET_VALUE(LW(0), STRING("OFF_d_roll_down"))
    SET_VALUE(LW(1), STRING("S_isk_01A01"))
    SET_VALUE(LW(2), STRING("img00"))
    SET_VALUE(LW(3), STRING("ofs00"))
    SPAWN_EVENT(wall_roll_sub)
    WAIT_MS(2000)
    SET_VALUE(LW(0), STRING("OFF_d_roll_left"))
    SET_VALUE(LW(1), STRING("S_isk_01A02"))
    SET_VALUE(LW(2), STRING("img01"))
    SET_VALUE(LW(3), STRING("ofs01"))
    SPAWN_EVENT(wall_roll_sub)
    WAIT_MS(2000)
    SET_VALUE(LW(0), STRING("OFF_d_roll_up"))
    SET_VALUE(LW(1), STRING("S_isk_01A03"))
    SET_VALUE(LW(2), STRING("img02"))
    SET_VALUE(LW(3), STRING("ofs02"))
    SPAWN_EVENT(wall_roll_sub)
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    SET_VALUE(LW(0), STRING("OFF_d_roll_right"))
    SET_VALUE(LW(1), STRING("S_isk_01A04"))
    SET_VALUE(LW(2), STRING("img03"))
    SET_VALUE(LW(3), STRING("ofs03"))
    CALL_CHILD(wall_roll_sub)
    CALL(evt_hitobj_onoff, STRING("A_isk_01A"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_f2_rak03"), 1, 1)
    CALL(evt_hitobj_onoff, STRING("A_f2_rak04"), 1, 0)
    SET_VALUE(GSWF_MRI_09_WALL_PUZZLE_SOLVED, 1)
#if VERSION >= VERSION_JP
    WAIT_FRAMES(1)
#endif
    CALL(evt_puni_all_fall_check)
    CALL(evt_puni_set_maze)
    CALL(evt_cam3d_evt_off, 300, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mobj_get_position, STRING("slot00"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("small_star"), 3, LW(0), LW(1), LW(2), 0, -20, 0, 3, 0, 0, 0, 0)
    CALL(evt_mobj_get_position, STRING("slot01"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("small_star"), 3, LW(0), LW(1), LW(2), 0, -20, 0, 3, 0, 0, 0, 0)
    CALL(evt_mobj_get_position, STRING("slot02"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("small_star"), 3, LW(0), LW(1), LW(2), 0, -20, 0, 3, 0, 0, 0, 0)
    CALL(evt_mobj_get_position, STRING("slot03"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("small_star"), 3, LW(0), LW(1), LW(2), 0, -20, 0, 3, 0, 0, 0, 0)
    CALL(evt_mobj_delete, STRING("slot00"))
    CALL(evt_mobj_delete, STRING("slot01"))
    CALL(evt_mobj_delete, STRING("slot02"))
    CALL(evt_mobj_delete, STRING("slot03"))
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_21_02_MATCHING_THE_PATTERNS)
#if VERSION >= VERSION_US
    CALL(evt_party_cloud_lock_animations_on_off, 0, 0)
#endif
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(breakyuka_init)
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF_MRI_09_BREAKING_FLOOR_BROKEN, 1)
#else
    IF_INT_EQUAL(GSWF_MRI_09_BREAKING_FLOOR_BROKEN, 0)
#endif
        CALL(evt_hitobj_onoff, STRING("A_f2_rak_02"), 1, 0)
#if VERSION >= VERSION_JP
        ELSE
        CALL(evt_hitobj_onoff, STRING("A_f2_rak_02"), 1, 1)
#endif
    END_IF
    RETURN
EVT_END()

/**
 * Main script when the breaking floor gives way:
 * turns on the collision of the lower floor piece and makes every Puni that is standing over the hole fall.
 */
static EVT_BEGIN(breakyuka_main)
    CALL(evt_hitobj_onoff, STRING("A_f2_rak_02"), 1, 1)
    CALL(evt_puni_all_fall_check)
    RETURN
EVT_END()

/** Plants Mario can search in mri_09: A_ueki_01, A_ueki_02, A_ueki_03, A_ueki_04, A_ueki_05. */
static KusaAccess ueki_access_data[] = {
    {1, "A_ueki_01", "S_ueki_01", NULL},
    {1, "A_ueki_02", "S_ueki_02", NULL},
    {1, "A_ueki_03", "S_ueki_03", NULL},
    {1, "A_ueki_04", "S_ueki_04", NULL},
    {1, "A_ueki_05", "S_ueki_05", NULL},
    {4, NULL, NULL, NULL},
};

/**
 * Hit script of red switch 0: while the puzzle is not solved,
 * each hit turns the first panel (GW(0)) by 90 degrees over half a second.
 */
static EVT_BEGIN(slot00_event)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG2_21_02_MATCHING_THE_PATTERNS)
        RETURN
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
    SET_VALUE(LW(10), GW(0))
    SET_VALUE(LW(11), GW(0))
    ADD(LW(11), 90)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(10), LW(11), 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(0), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_GREATER_OR_EQUAL(GW(0), 360)
        SUBTRACT(GW(0), 360)
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

/** Hit script of red switch 1: turns the second panel (GW(1)) by 90 degrees over half a second. */
static EVT_BEGIN(slot01_event)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG2_21_02_MATCHING_THE_PATTERNS)
        RETURN
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
    SET_VALUE(LW(10), GW(1))
    SET_VALUE(LW(11), GW(1))
    ADD(LW(11), 90)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(10), LW(11), 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(1), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_GREATER_OR_EQUAL(GW(1), 360)
        SUBTRACT(GW(1), 360)
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

/** Hit script of red switch 2: turns the third panel (GW(2)) by 90 degrees over half a second. */
static EVT_BEGIN(slot02_event)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG2_21_02_MATCHING_THE_PATTERNS)
        RETURN
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
    SET_VALUE(LW(10), GW(2))
    SET_VALUE(LW(11), GW(2))
    ADD(LW(11), 90)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(10), LW(11), 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(2), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_GREATER_OR_EQUAL(GW(2), 360)
        SUBTRACT(GW(2), 360)
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

/** Hit script of red switch 3: turns the fourth panel (GW(3)) by 90 degrees over half a second. */
static EVT_BEGIN(slot03_event)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG2_21_02_MATCHING_THE_PATTERNS)
        RETURN
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
    SET_VALUE(LW(10), GW(3))
    SET_VALUE(LW(11), GW(3))
    ADD(LW(11), 90)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(10), LW(11), 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(3), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_GREATER_OR_EQUAL(GW(3), 360)
        SUBTRACT(GW(3), 360)
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

/**
 * Runs the four-panel pattern puzzle: sets the panels' starting angles (GW(0) to GW(3)),
 * keeps their rotation up to date and starts wall_roll
 * when the panels show the correct pattern (270, 180, 0, 90).
 */
static EVT_BEGIN(slot_event)
    SET_VALUE(GW(0), 0)
#if VERSION >= VERSION_US
    SET_VALUE(GW(1), 90)
    SET_VALUE(GW(2), 180)
    SET_VALUE(GW(3), 270)
#else
    SET_VALUE(GW(1), 0)
    SET_VALUE(GW(2), 0)
    SET_VALUE(GW(3), 0)
#endif
    LOOP(0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_01A01a"), GW(0), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_01A02a"), GW(1), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_01A03a"), GW(2), 0, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_isk_01A04a"), GW(3), 0, 0)

        // The four panels must read 270, 180, 0 and 90 degrees in order.
        IF_INT_EQUAL(GW(0), 270)
            IF_INT_EQUAL(GW(1), 180)
                IF_INT_EQUAL(GW(2), 0)
                    IF_INT_EQUAL(GW(3), 90)
                        BREAK_LOOP
                    END_IF
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SPAWN_EVENT(wall_roll)
    RETURN
EVT_END()

/**
 * Chest script for the Blue Key: Mario opens the chest and receives the Blue Key, Punio comments,
 * and the west exit is changed to lead to the blue cell. Sets the story to STG2_18_JABBLE.
 */
static EVT_BEGIN(get_blue_jail_key)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_BLUE_KEY, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL_CHILD(mri_guide_goryu)
    CALL(evt_puni_flag_onoff, 1, 0, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_puni_set_mode, 1, PUNI_MODE_INERT)
    CALL(evt_npc_flag_onoff, 1, STRING("ガイドsp"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("ガイドsp"), LW(10), LW(11), LW(12))
        IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
            CALL(evt_mario_set_dir, 90, 200, 0)
        ELSE
            CALL(evt_mario_set_dir, 270, 200, 0)
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("ガイドsp"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
        CALL(evt_npc_set_ry, STRING("ガイドsp"), 90)
    ELSE
        CALL(evt_npc_set_ry, STRING("ガイドsp"), 270)
    END_IF
    CALL(evt_msg_print, 0, STRING("stg2_mri_e18_00"), 0, STRING("ガイドsp"))
    CALL(evt_puni_flag_onoff, 1, 1, PUNI_FLAG_SCRIPT_CONTROL)
    CALL(evt_puni_set_mode, 1, PUNI_MODE_JOIN)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_18_JABBLE)
    CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_IRIS, 500, FADE_IN_IRIS, 500)
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(bero_case_switch_off)
    CALL(evt_bero_overwrite, STRING("w_bero"), PTR(bero_bluejail))
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(bero_case_switch_on)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init script of mri_09, the slot-switch room:
 * sets up the Puni, planters and Piders checker, the four red switches and wall panels for the story state,
 * the Blue Key chest, the Charge badge, a Shine Sprite block and the breaking floor.
 */
EVT_BEGIN(mri_09_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 9, 100, 0, -310, -1000, 2, 2)
    CALL(evt_puni_set_hamari, -250, 100, 78, 102, -2, -230, -311)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(mri_puni_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    SPAWN_EVENT(puni_disp_info)
    SPAWN_EVENT(puni_piders_check)
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(evt_sub_kusa_access_entry)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_23_TIME_BOMB_ACTIVATED, STG2_23_01_X_NAUT_BATTLE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_npc_change_fbat_mode, FBAT_MODE_FIELD)
    CALL(evt_map_playanim, STRING("mizu_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_02"), MAP_ANIM_PLAY_LOOP, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG2_21_01_MYSTERY_PILLAR)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak03"), 1, 0)
#if VERSION >= VERSION_US
        SPAWN_EVENT(slot_event)
#endif
        CASE_EQUAL(STG2_21_01_MYSTERY_PILLAR)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak03"), 1, 0)
        SPAWN_EVENT(slot_event)
        CALL(evt_mobj_switch_red, 1, STRING("slot00"), -80, 0, 135, PTR(slot00_event), 0)
        CALL(evt_mobj_switch_red, 1, STRING("slot01"), -30, 0, 135, PTR(slot01_event), 0)
        CALL(evt_mobj_switch_red, 1, STRING("slot02"), 30, 0, 135, PTR(slot02_event), 0)
        CALL(evt_mobj_switch_red, 1, STRING("slot03"), 80, 0, 135, PTR(slot03_event), 0)
#if VERSION >= VERSION_JP
        CALL(evt_paper_entry, STRING("OFF_d_roll_down"))
        CALL(evt_paper_entry, STRING("OFF_d_roll_up"))
        CALL(evt_paper_entry, STRING("OFF_d_roll_left"))
        CALL(evt_paper_entry, STRING("OFF_d_roll_right"))
#endif
        CASE_GREATER_OR_EQUAL(STG2_21_02_MATCHING_THE_PATTERNS)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_isk_01A"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_isk_01A"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak03"), 1, 1)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak04"), 1, 0)
    END_SWITCH
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 125, 0, 25, MOBJ_ITEMBOX_NORMAL, 0, PTR(get_blue_jail_key), GSWF_MRI_09_BLUE_KEY_CHEST)
    CALL(evt_item_entry, STRING("item100"), ITEM_CHARGE, -250, -234, 75, ITEM_TYPE_STILL, GSWF_MRI_09_CHARGE, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk1"), 0, -174, -125, 0, GSWF_SHINE_18, 0)
    SPAWN_EVENT(breakyuka_init)
    CALL(evt_mobj_breaking_floor, STRING("mobj_bf00"), 0, 0, 75, 1, PTR(breakyuka_main), GSWF_MRI_09_BREAKING_FLOOR_BROKEN)
    RETURN
EVT_END()

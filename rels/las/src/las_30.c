// las_30 (ラストダンジョン, Palace of Shadow): the stairs between las_28 and las_29.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "stdio.h"
#include "evt/evt_bero.h"
#include "evt/evt_mobj.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "manager/evtmgr_cmd.h"

#include "version.h"
#if VERSION > VERSION_DEMO_US
static char name[8];
#endif

static USER_FUNC(make_name);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
static EvtWord rousoku_init[46];
static EvtWord mobj_box01[34];
static EvtWord mobj_box02[34];
#if VERSION <= VERSION_DEMO_US
extern EvtWord las_30_init_evt[];
#else
EvtWord las_30_init_evt[71];
#endif

/**
 * Door animation script (closing) for the west door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_door2_1 and S_door2_2 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the west door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_door2_1 and S_door2_2 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (closing) for the east door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels door_1_1 and door_1_2 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the east door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels door_1_1 and door_1_2 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Exits of las_30: west road w_bero to las_28 (e_bero); east door e_bero to las_29 (w_bero). */
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
        "las_28", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_29", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * Map-init helper: hides the 12 numbered torch map objects (names from make_name)
 * and puts a flame effect 7 units below the origin of each one.
 */
static EVT_BEGIN(rousoku_init)
    SET_VALUE(LW(0), 1)
    LOOP(12)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, LW(1), LW(2), LW(3), LW(4))
        SUBTRACT_FLOAT(LW(3), FLOAT(7.0f))
#if VERSION <= VERSION_DEMO_US
CALL(evt_eff, STRING(""), STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(0.5f), 1000, 0, 0, 0, 0, 0, 0)
#else
        CALL(evt_eff, 0, STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(0.5f), 1000, 0, 0, 0, 0, 0, 0)
#endif
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

/** Event script: the first treasure chest of las_30 opens and Mario receives an Ultra Shroom. */
static EVT_BEGIN(mobj_box01)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box01"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item01"), ITEM_ULTRA_SHROOM, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item01"), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box01"))
    CALL(evt_item_get_item, STRING("item01"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Event script: the second treasure chest of las_30 opens and Mario receives a Jammin' Jelly. */
static EVT_BEGIN(mobj_box02)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box02"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item02"), ITEM_JAMMIN_JELLY, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item02"), ITEM_FLAG_NO_TOUCH_GET)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box02"))
    CALL(evt_item_get_item, STRING("item02"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init script for las_30, the long stairs between las_28 and las_29:
 * at STORY_STG8_15_GRODUS the danger music plays, otherwise silence with the Palace ambience;
 * a save block, a recovery block, two chests (an Ultra Shroom and a Jammin' Jelly) and the torches.
 */
EVT_BEGIN(las_30_init_evt)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
#else
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
#endif
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER2"))
#if VERSION > VERSION_DEMO_US
        CALL(evt_snd_envoff, PSND_FADEOUT)
#endif
    ELSE
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
#if VERSION > VERSION_DEMO_US
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
#else
    CALL(evt_snd_set_rev_mode, 2)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 1150, 60, -125, 0, 0)
#if VERSION <= VERSION_DEMO_US
CALL(evt_mobj_recovery_blk, STRING("r_blk"), 1, -1050, 860, -75, 0, 0)
CALL(evt_mobj_itembox, STRING("mobj_box01"), -1150, 800, -75, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box01), GSWF_LAS_30_ULTRA_SHROOM_CHEST)
    CALL(evt_mobj_itembox, STRING("mobj_box02"), 1050, 0, -125, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box02), GSWF_LAS_30_JAMMIN_JELLY_CHEST)
#else
    CALL(evt_mobj_recovery_blk, STRING("r_blk"), 10, -1050, 860, -75, 0, 0)
    CALL(evt_mobj_itembox, STRING("mobj_box01"), -1150, 800, -75, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box01), GSWF_LAS_30_ULTRA_SHROOM_CHEST)
    CALL(evt_mobj_itembox, STRING("mobj_box02"), 1050, 0, -125, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box02), GSWF_LAS_30_JAMMIN_JELLY_CHEST)
#endif
    CALL_CHILD(rousoku_init)
    RETURN
#if VERSION <= VERSION_DEMO_US
EVT_END()
#else
EVT_END_PAD()

/** Script helper: builds the torch map-object name "hi<args[0]>" and returns it in args[1]. */
#endif
static USER_FUNC(make_name) {
    EvtWord* args = event->args;
#if VERSION <= VERSION_DEMO_US
char name[8];
s32 number;
#endif

#if VERSION <= VERSION_DEMO_US
    number = evtGetValue(event, args[0]);
    sprintf(name, "hi%d", number);
#else
    sprintf(name, "hi%d", evtGetValue(event, args[0]));
#endif
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}


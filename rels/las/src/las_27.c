// las_27 (ラストダンジョン, Palace of Shadow): the hall before the throne rooms.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "party/party_motion.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"

#include "stdio.h"
#include "evt/evt_bero.h"
#include "manager/evtmgr_cmd.h"

#if VERSION >= VERSION_JP
static char name[8];
#endif

static USER_FUNC(make_name);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
static EvtWord shan_init[46];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord las_27_init_evt[];
#else
EvtWord las_27_init_evt[38];
#endif

/**
 * Door animation script (closing) for the west door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_door1_1 and S_door1_2 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the west door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_door1_1 and S_door1_2 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (closing) for the east door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_door2_1 and S_door2_2 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the east door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panels S_door2_1 and S_door2_2 by it,
 * the second one the opposite way.
 */
static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_door2_2"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Exits of las_27: west door w_bero to las_25 (e_bero_2); east door e_bero to las_28 (w_bero). */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_25", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
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
        "las_28", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * Map-init helper: hides the 28 numbered torch map objects (names from make_name)
 * and puts a small flame effect 9 units below the origin of each one.
 */
static EVT_BEGIN(shan_init)
    SET_VALUE(LW(0), 1)
    LOOP(28)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, LW(1), LW(2), LW(3), LW(4))
        SUBTRACT_FLOAT(LW(3), FLOAT(9.0f))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_eff, STRING(""), STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(0.69921875f), 1000, 0, 0, 0, 0, 0, 0)
#else
        CALL(evt_eff, 0, STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(0.69921875f), 1000, 0, 0, 0, 0, 0, 0)
#endif
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Map init script for las_27, the hall before the throne rooms:
 * ambience only (no music), the doors to las_25 and las_28, a save block, a recovery block,
 * and 28 lit wall torches.
 */
EVT_BEGIN(las_27_init_evt)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS9"))
#if VERSION >= VERSION_JP
    CALL(evt_snd_set_rev_mode, 2)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 520, 60, -150, 0, 0)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_mobj_recovery_blk, STRING("r_blk"), 5, 450, 60, -150, 0, 0)
    CALL_CHILD(shan_init)
#else
    CALL(evt_mobj_recovery_blk, STRING("r_blk"), 10, 450, 60, -150, 0, 0)
    CALL_CHILD(shan_init)
#endif
    RETURN
EVT_END()

/** Script helper: builds the torch map-object name "hi<args[0]>" and returns it in args[1]. */
static USER_FUNC(make_name) {
    EvtWord* args = event->args;
#if VERSION <= VERSION_DEMO_JP
    char name[8];
    s32 number;

    number = evtGetValue(event, args[0]);
    sprintf(name, "hi%d", number);
#else
    sprintf(name, "hi%d", evtGetValue(event, args[0]));
#endif
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}


// las_02 (ラストダンジョン, Palace of Shadow): the second hall.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"

#include "stdio.h"
#include "evt/evt_bero.h"
#include "manager/evtmgr_cmd.h"

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

#if VERSION <= VERSION_DEMO_US
extern EvtWord las_02_init_evt[];
#else
EvtWord las_02_init_evt[44];
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

/** Exits of las_02: west door w_bero to las_01 (e_bero); east door e_bero to las_03 (w_bero). */
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
        "las_01", // nextMapName
        "e_bero", // nextBeroName
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
        "las_03", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

/**
 * Map-init helper: hides the 18 numbered candle ("rousoku") map objects (names from make_name)
 * and puts a flame effect 7 units below the origin of each one.
 */
static EVT_BEGIN(rousoku_init)
    SET_VALUE(LW(0), 1)
    LOOP(18)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, LW(1), LW(2), LW(3), LW(4))
        SUBTRACT_FLOAT(LW(3), FLOAT(7.0f))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_eff, STRING(""), STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(1.0f), 1000, 0, 0, 0, 0, 0, 0)
#else
        CALL(evt_eff, 0, STRING("fire"), 3, LW(2), LW(3), LW(4), FLOAT(1.0f), 1000, 0, 0, 0, 0, 0, 0)
#endif
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Map init script for las_02: music and ambience, the doors to las_01 and las_03, the Shooting Star ? block,
 * links the two Bombshell Bill Blasters if they exist, and lights the 18 torches.
 */
EVT_BEGIN(las_02_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_snd_set_rev_mode, 2)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_badgeblk, STRING("mobj00"), -550, 480, 60, ITEM_SHOOTING_STAR, 0, GSWF_LAS_02_SHOOTING_STAR_BLOCK, 0)
    CALL(evt_npc_check, STRING("スーパーキラー大砲１"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_link, STRING("スーパーキラー大砲１"), STRING("スーパーキラー大砲２"))
    END_IF
    CALL_CHILD(PTR(rousoku_init))
    RETURN
EVT_END()

static USER_FUNC(make_name) {
    EvtWord* args = event->args;
#if VERSION <= VERSION_DEMO_US
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


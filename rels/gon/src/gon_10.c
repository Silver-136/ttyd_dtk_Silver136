// gon_10 (ゴンババ, Hooktail Castle): room between gon_05 and Hooktail's lair (gon_11).
// Has a save block and a recovery block.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "party/party_motion.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"


static EvtWord door02_close[22];
static EvtWord door02_open[22];
static EvtWord door01_close[22];
static EvtWord door01_open[22];
static BeroEntry bero_entry_data[3];
EvtWord gon_10_init_evt[46];

/**
 * Door animation callback, closing for the w_bero exit to gon_11:
 * rotates map objects "door03" and "door04" about Y by -0.389 and 0.389 times the value in LW(0),
 * so the two leaves swing opposite ways.
 */
static EVT_BEGIN(door02_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the w_bero exit to gon_11:
 * rotates map objects "door03" and "door04" about Y by 0.389 and -0.389 times the value in LW(0),
 * so the two leaves swing opposite ways.
 */
static EVT_BEGIN(door02_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, closing for the e_bero exit to gon_05:
 * rotates map objects "door01" and "door02" about Y by -0.389 and 0.389 times the value in LW(0),
 * so the two leaves swing opposite ways.
 */
static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback, opening for the e_bero exit to gon_05:
 * rotates map objects "door01" and "door02" about Y by 0.389 and -0.389 times the value in LW(0),
 * so the two leaves swing opposite ways.
 */
static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Exits (beros) of this room: w_bero to gon_11 and e_bero to gon_05. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        13, // sfxId
        BERO_DIR_E, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_11", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door02_close, // enterAnimeArg
        door02_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        12, // sfxId
        BERO_DIR_AUTO, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_05", // nextMapName
        "w_bero_5", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(gon_10_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON3"))
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -460, 1010, -145, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 6, -400, 1010, -146, 0, 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_16_HOOKTAIL)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, STRING("en"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()


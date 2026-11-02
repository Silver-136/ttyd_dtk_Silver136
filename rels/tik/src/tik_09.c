// tik_09 (埋没した都市, Rogueport Sewers): a small pipe room between tik_06 and tik_10.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "system.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[3];
EvtWord evt_tik09_dokan[51];
EvtWord tik_09_init_evt[36];

/** Doors of this room: the pipes to tik_10 and to tik_06. */
static BeroEntry bero_entry_data[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_10", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_UP, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_06", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * Raises the pipe (dokan_1_s) in the room out of the floor:
 * after 1.5 seconds it slides up 30 units over 100 frames, moving its hit area with it.
 */
// Start the pipe at zero offset, then raise it while updating its bound collision each step.
EVT_BEGIN(evt_tik09_dokan)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
    CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, 0, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    WAIT_MS(1500)
    CALL(evt_sub_intpl_init, EASING_SINE_IN, 0, 30, 100)
    LOOP(100)
        CALL(evt_sub_intpl_get_value)
        CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_1"))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, 30, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    RETURN
EVT_END()

/**
 * Map init script of tik_09.
 * Starts with the pipe translated 30 units upward (hit area bound to it), starts the music and ambience,
 * sets up the exits and runs evt_tik09_dokan.
 */
EVT_BEGIN(tik_09_init_evt)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
    CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, 30, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(evt_tik09_dokan))
    RETURN
EVT_END()


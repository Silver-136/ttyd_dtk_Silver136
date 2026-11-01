// muj_11 (無人島, Keelhaul Key): the passage between muj_10 and Cortez's lair, with its warning
// event (warning_event).

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_map.h"
#include "pmario_sound.h"


static EvtWord w_close[12];
static EvtWord w_open[12];
static EvtWord e_close[12];
static EvtWord e_open[12];
static BeroEntry bero_entry_data[3];
EvtWord warning_event[38];
#include "version.h"
#if VERSION <= VERSION_DEMO_US
extern EvtWord muj_11_init_evt[];
#else
EvtWord muj_11_init_evt[35];
#endif

/**
 * Enter hook of the west door (w_bero, from muj_10, enter anime type 5): turns S_doa_l to LW(0) x 0.444 degrees;
 * the door code feeds 180 down to 0, so the door stands 80 degrees open and swings shut.
 */
static EVT_BEGIN(w_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Exit hook of the west door: turns S_doa_l to LW(0) x -0.444 degrees;
 * the door code feeds 0 up to 180, so it swings 80 degrees open the other way as Mario leaves.
 */
static EVT_BEGIN(w_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Enter hook of the east door (e_bero, from muj_12): turns S_doa_r to LW(0) x -0.444 degrees;
 * fed 180 down to 0, the door swings shut from 80 degrees open.
 */
static EVT_BEGIN(e_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Exit hook of the east door: turns S_doa_r to LW(0) x 0.444 degrees;
 * fed 0 up to 180, it swings 80 degrees open the other way as Mario leaves.
 */
static EVT_BEGIN(e_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Doors and exits of muj_11: w_bero to muj_10; e_bero to muj_12. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        2, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_10", // nextMapName
        "A_b_bero03", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        2, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_12", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_close, // enterAnimeArg
        e_open, // exitAnimeArg
    },
    {0},
};

/**
 * Cortez's warning (STG5_31_FOUR_EYES): when Mario enters the corridor a spoken narration (stg5_dou_20)
 * warns of the danger ahead and the story becomes STG5_32_CORTEZS_WARNING.
 */
EVT_BEGIN(warning_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
        WAIT_MS(800)
        CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg5_dou_20"), 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_32_CORTEZS_WARNING)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init for muj_11 (the corridor between the dock and Cortez's lair):
 * picks music by story stage, registers the exits and spawns warning_event at STG5_31_FOUR_EYES.
 */
EVT_BEGIN(muj_11_init_evt)
#if VERSION <= VERSION_DEMO_US
    IF_INT_LESS(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
#else
    IF_INT_LESS(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
#endif
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG5_GHOST1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ9"))
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_bero_info_run)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_31_FOUR_EYES)
        SPAWN_EVENT(warning_event)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
    // Only this story step starts the warning; the warning advances progress to the next step.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_31_FOUR_EYES)
        SPAWN_EVENT(PTR(warning_event))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_US
CALL(evt_mobj_recovery_blk, STRING("r_blk"), 12, 0, 60, -20, 0, 0)
    CALL(evt_mobj_save_blk, STRING("s_blk"), 50, 60, -20, 0, 0)
#endif
    RETURN
EVT_END()


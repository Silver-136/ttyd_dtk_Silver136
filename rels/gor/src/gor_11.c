// gor_11 (ゴロツキタウン, Rogueport): epilogue sea scene (ending, story flag 402); on finish returns to gor_00.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_snd.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_cam.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "mario/mario.h"
#include "party.h"


#if VERSION <= VERSION_DEMO_US
static EvtWord epilogue_evt[58];
extern EvtWord gor_11_init_evt[];
#else
static EvtWord epilogue_evt[62];
EvtWord gor_11_init_evt[29];
#endif

/**
 * Ending sequence on the sea view: fades out the music, starts the sea ambience,
 * freezes and hides Mario and the partner, fades the screen black for 5 seconds and back in over 1 second,
 * holds it for 5 more seconds (JP and later then fade the ambience out)
 * and changes the map to gor_00 (Rogueport).
 */
static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_END_MUJ1"))
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 1)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_entry, FADE_OUT_BLACK, 1, 0, 0, 0)
    WAIT_MS(2000)
    WAIT_MS(3000)
    CALL(evt_fade_in, 1000)
    WAIT_MS(5000)
#if VERSION > VERSION_DEMO_US
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
#endif
    CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_BLACK, 1500, -1, -1)
    CALL(evt_bero_mapchange, STRING("gor_00"), STRING(""))
    RETURN
EVT_END()

/**
 * Init script of gor_11, the sea view of the ending:
 * plays the sea animations and, when the story is at STG8_END_AFTER_THE_BATTLE,
 * removes the partner and runs the epilogue (fade in, ambience, fade out, then back to gor_00).
 * JP and later skip it if map events are being skipped.
 */
EVT_BEGIN(gor_11_init_evt)
    CALL(evt_map_playanim, STRING("anm_umi"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("anm_nami"), MAP_ANIM_PLAY_LOOP, 0)
#if VERSION <= VERSION_DEMO_US
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
#else
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_NO_ENTRANCE)
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
#endif
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    RETURN
EVT_END()


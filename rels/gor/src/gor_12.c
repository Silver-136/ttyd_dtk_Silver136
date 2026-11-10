// gor_12 (ゴロツキタウン, Rogueport): final epilogue sea scene (ending, story flag 402); on finish goes to aaa_00 (Mario Bros.' house).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_snd.h"
#include "evt/evt_cmd.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_sub.h"
#include "evt/evt_bero.h"
#include "evt/evt_fade.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_cam.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "mario/mario.h"
#include "party.h"
#include "driver/camdrv.h"

static EvtWord epilogue_evt[49];

#if VERSION == VERSION_DEMO_JP
EvtWord gor_12_init_evt[26];
#else
EvtWord gor_12_init_evt[34];
#endif

/**
 * Ending sequence on the final sea view: starts the sea ambience, freezes and hides Mario and the partner,
 * waits 7 seconds, then starts 8-second fades of the music and ambience,
 * fades to black and changes the map to aaa_00.
 */
static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_END_MUJ1"))
    CALL(evt_cam_ctrl_onoff, CAMERA_3D, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 1)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(7000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 8000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 8000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 4000, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("aaa_00"), STRING(""))
    RETURN
EVT_END()

/**
 * Init script of gor_12, the final sea view of the ending:
 * plays the sea and sparkle animations and, when the story is at STG8_END_AFTER_THE_BATTLE,
 * removes the partner and runs the epilogue (7 seconds, then everything fades out to aaa_00, Mario Bros.' house).
 * JP and later skip it if map events are being skipped.
 */
EVT_BEGIN(gor_12_init_evt)
    CALL(evt_map_playanim, STRING("anm_umi"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("anm_nami"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("anm_kirakira"), MAP_ANIM_PLAY_LOOP, 0)
#if VERSION >= VERSION_JP
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_NO_ENTRANCE)
        RETURN
    END_IF
#endif
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(epilogue_evt)
    END_IF
    RETURN
EVT_END()

// dou_01 (洞窟, Pirate's Grotto): passage between dou_00 and dou_02 with two blue jump pads.
// Plays a Cortez (korutesu) cutscene on first entry (story 243 to 244).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_eff.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_mobj.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[3];
#if VERSION <= VERSION_DEMO_US
static EvtWord korutesu_event[71];
#else
static EvtWord korutesu_event[74];
#endif
EvtWord dou_01_init_evt[48];

/** Exits (beros) of this room: w_bero to dou_00 and e_bero to dou_02. */
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
        "dou_00", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_02", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/**
 * First-entry cutscene in which Mario is startled (voice and exclamation balloon) and Cortez (コルテス) is heard:
 * shows stg5_dou_01 and sets the story progress to STG5_19_02_THE_CALL_OF_CORTEZ.
 */
static EVT_BEGIN(korutesu_event)
    CALL(evt_mario_key_onoff, 0)
#if VERSION > VERSION_DEMO_US
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
#endif
    CALL(evt_mario_normalize)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_seq_wait, 2)
#endif
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_01"), 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_19_02_THE_CALL_OF_CORTEZ)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Init script of dou_01, the passage between dou_00 and dou_02:
 * music and ambience, exits, two blue jump pads,
 * and the short Cortez cutscene on the first visit (Mario is startled, Cortez's voice is heard,
 * stg5_dou_01) (story step STG5_19_01_ENTERING_THE_GROTTO).
 */
EVT_BEGIN(dou_01_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_DOU2"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump00"), 400, 92, 375, -21, 0, 0)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump01"), 350, -45, 50, -15, 0, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_19_01_ENTERING_THE_GROTTO)
        SPAWN_EVENT(PTR(korutesu_event))
    END_IF
    RETURN
EVT_END()


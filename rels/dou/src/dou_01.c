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


static BeroEntry bero_entry_data[3];
static EvtWord korutesu_event[74];
EvtWord dou_01_init_evt[48];

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
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
        20000, // directionId
        100000, // centerPosition
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

static EVT_BEGIN(korutesu_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_normalize)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_ELMOS_VOICE1"), 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_01"), 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 244)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(dou_01_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG5_DOU2"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump00"), 400, 92, 375, -21, 0, 0)
    CALL(evt_mobj_jumpstand_blue, 0, STRING("mobj_jump01"), 350, -45, 50, -15, 0, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 243)
        SPAWN_EVENT(PTR(korutesu_event))
    END_IF
    RETURN
EVT_END()


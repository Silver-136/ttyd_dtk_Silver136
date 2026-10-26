#include "battle/battle_unit.h"
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

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_snd_envoff_f);

static EvtWord epilogue_evt[49];
EvtWord gor_12_init_evt[35];

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_END_MUJ1"))
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, 1, 1)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(7000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 8000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 8000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 4000, 9, 300)
    CALL(evt_bero_mapchange, STRING("aaa_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(gor_12_init_evt)
    CALL(evt_map_playanim, STRING("anm_umi"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_nami"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_kirakira"), 1, 0)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(epilogue_evt)
    END_IF
    RETURN
EVT_END_PAD()


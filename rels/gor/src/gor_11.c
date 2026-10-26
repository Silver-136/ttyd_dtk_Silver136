#include "battle/battle_unit.h"
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

USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_snd_envoff_f);

static EvtWord epilogue_evt[62];
EvtWord gor_11_init_evt[29];

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_END_MUJ1"))
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 1)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_entry, 10, 1, 0, 0, 0)
    WAIT_MS(2000)
    WAIT_MS(3000)
    CALL(evt_fade_in, 1000)
    WAIT_MS(5000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 0, 10, 1500, -1, -1)
    CALL(evt_bero_mapchange, STRING("gor_00"), STRING(""))
    RETURN
EVT_END()

EVT_BEGIN(gor_11_init_evt)
    CALL(evt_map_playanim, STRING("anm_umi"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_nami"), 1, 0)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(PTR(epilogue_evt))
    END_IF
    RETURN
EVT_END()


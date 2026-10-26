#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "hom.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "driver/mapdrv.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"


EvtWord hom_12_init_evt[140];

EVT_BEGIN(hom_12_init_evt)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_RSH2"))
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_party_set_pos, 1, 0, -1000, 0)
    IF_INT_EQUAL(GSWF(3578), 0)
        CALL_CHILD(PTR(hom_10_evt_resha_right))
        CALL_CHILD(PTR(hom_10_evt_resha_start3))
        SET_VALUE(LW(0), 500)
        SET_VALUE(LW(1), -500)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("renketu"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("renketu_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("renketu_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("renketu_3"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("renketu_4"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("renketu_5"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL_CHILD(PTR(hom_10_evt_resha_left))
        CALL_CHILD(PTR(hom_10_evt_resha_start4))
        SET_VALUE(LW(0), 0)
        SET_VALUE(LW(1), 1000)
        CALL(evt_mapobj_trans, 1, STRING("train"), -1500, 0, 0)
    END_IF
    CALL(evt_cam_letter_box_onoff, 1, 1)
    CALL(evt_cam3d_evt_set, LW(0), 300, 1400, LW(0), 125, 0, 0, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_cam3d_evt_set, LW(1), 300, 1400, LW(1), 125, 0, 8000, EASING_LINEAR)
        WAIT_MS(4000)
        CALL(evt_bero_mapchange, STRING("rsh_02_a"), STRING("nidome"))
    END_INLINE_EVENT
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_mario.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


EvtWord bero_in[45];
EvtWord bero_out[53];
static BeroEntry bero_data[2];
static EvtWord evt_daiza[28];
EvtWord pik_03_init_evt[113];

EVT_BEGIN(bero_in)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, 0, 0, 660)
    CALL(evt_party_set_pos, 0, -10, 0, 670)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 35, 610, FLOAT(120.0f))
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_cam_letter_box_disable, 0)
        CALL_CHILD(PTR(bero_case_entry))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 10, 620, FLOAT(120.0f))
    END_INLINE_EVENT
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(bero_out)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_hitobj_attr_onoff, 0, 1, STRING("s_bero"), 8388608)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 35, 610, FLOAT(120.0f))
        CALL(evt_mario_mov_pos2, -10, 670, FLOAT(120.0f))
        CALL(evt_bero_mapchange, STRING("pik_02"), STRING("h_bero"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_party_move_pos2, 0, 10, 620, FLOAT(120.0f))
        CALL(evt_party_move_pos2, 0, 0, 660, FLOAT(120.0f))
    END_INLINE_EVENT
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "s_bero", // hitName
        1, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_in, // enterEvent
        8, // caseType
        bero_out, // exitEvent
        "pik_02", // nextMapName
        "h_bero", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_daiza)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_53_THE_REAL_GARNET_STAR)
    CALL(evt_npc_stop_for_event)
    SET_VALUE(LW(0), 6)
    SET_VALUE(LW(1), STRING("star"))
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL_CHILD(PTR(evt_sub_starstone))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(pik_03_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG6_PIK2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_PIK3"))
    CALL(evt_snd_set_rev_mode, 2)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_53_THE_REAL_GARNET_STAR)
        CALL(evt_item_entry, STRING("star"), ITEM_GARNET_STAR, 0, 45, -950, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_53_THE_REAL_GARNET_STAR)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("dai_a"), 0, PTR(evt_daiza), 0)
    END_IF
    CALL(evt_item_entry, STRING("item01"), ITEM_L_EMBLEM, -305, 330, -990, ITEM_TYPE_STILL, GSWF_PIK_03_L_EMBLEM, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -265, 290, 550, 0, GSWF_SHINE_39, 0)
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hi1"))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("next"))
        CALL(evt_mario_set_pos, -25, 9, -925)
        CALL(evt_party_set_pos, 0, 25, 9, -925)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_cam_road_reset)
        CALL(evt_mario_key_onoff, 0)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, SEQ_GAME)
            CALL(evt_fade_end_wait)
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()


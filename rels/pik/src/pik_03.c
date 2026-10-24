#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"


USER_FUNC(evt_bero_get_entername);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_cam_road_reset);
USER_FUNC(evt_fade_end_wait);
USER_FUNC(evt_hitobj_attr_onoff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_npc_stop_for_event);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_yoshi_ride);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmoff_f);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);

f32 fd_pik_03_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_pik_03_neg_one_382;
f32 fd_pik_03___local_PI_383;
f32 fd_pik_03___local_3PIO2_385;
f32 fd_pik_03_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_pik_03___local_PIO2_384;
f32 fd_pik_03_one_381;
f32 fd_pik_03_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_pik_03_neg_one_365;
f32 fd_pik_03___local_PI_366;
f32 fd_pik_03___local_3PIO2_368;
f32 fd_pik_03_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_pik_03___local_PIO2_367;
f32 fd_pik_03_one_364;
EvtWord bero_in[45];
EvtWord bero_out[53];
static BeroEntry bero_data[2];
static EvtWord evt_daiza[28];
EvtWord pik_03_init_evt[113];

f32 fd_pik_03_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_pik_03_neg_one_382 = -1.0f;

f32 fd_pik_03___local_PI_383 = 3.1415927f;

f32 fd_pik_03___local_3PIO2_385 = 4.712389f;

f32 fd_pik_03_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_pik_03___local_PIO2_384 = 1.5707964f;

f32 fd_pik_03_one_381 = 1.0f;

f32 fd_pik_03_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_pik_03_neg_one_365 = -1.0f;

f32 fd_pik_03___local_PI_366 = 3.1415927f;

f32 fd_pik_03___local_3PIO2_368 = 4.712389f;

f32 fd_pik_03_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_pik_03___local_PIO2_367 = 1.5707964f;

f32 fd_pik_03_one_364 = 1.0f;

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
        (s32*)bero_in, // enterEvent
        8, // caseType
        (s32*)bero_out, // exitEvent
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
    SET_VALUE(GSW(0), 338)
    CALL(evt_npc_stop_for_event)
    SET_VALUE(LW(0), 6)
    SET_VALUE(LW(1), STRING("star"))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_snd_bgmoff, 513)
    CALL_CHILD(PTR(evt_sub_starstone))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(pik_03_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG6_PIK2"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG6_PIK3"))
    CALL(evt_snd_set_rev_mode, 2)
    IF_INT_LESS(GSW(0), 338)
        CALL(evt_item_entry, STRING("star"), 119, 0, 45, -950, 17, -1, 0)
    END_IF
    IF_INT_LESS(GSW(0), 338)
        CALL(evt_run_case_evt, 9, 1, STRING("dai_a"), 0, PTR(evt_daiza), 0)
    END_IF
    CALL(evt_item_entry, STRING("item01"), 327, -305, 330, -990, 16, GSWF(3277), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -265, 290, 550, 0, GSWF(5564), 0)
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
            CALL(evt_seq_wait, 2)
            CALL(evt_fade_end_wait)
            CALL(evt_mario_key_onoff, 1)
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()


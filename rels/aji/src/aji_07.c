#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "party.h"
#include "evt/evt_bero.h"
#include "aji.h"

USER_FUNC(evt_bero_get_entername);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_exit_case_evt);
USER_FUNC(evt_hit_get_position);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_checkanim);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_color);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_cam_off);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_get_dispdir);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_dispdir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_wait_landon);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_get_dispdir);
USER_FUNC(evt_party_get_pos);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_dispdir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);
static USER_FUNC(scale_func);

f32 fd_aji_07_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_aji_07_neg_one_382;
f32 fd_aji_07___local_PI_383;
f32 fd_aji_07___local_3PIO2_385;
f32 fd_aji_07_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_aji_07___local_PIO2_384;
f32 fd_aji_07_one_381;
f32 fd_aji_07_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_aji_07_neg_one_365;
f32 fd_aji_07___local_PI_366;
f32 fd_aji_07___local_3PIO2_368;
f32 fd_aji_07_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_aji_07___local_PIO2_367;
f32 fd_aji_07_one_364;
static EvtWord s_door_open[22];
static EvtWord s_door_close[22];
static BeroEntry bero_entry_data[2];
EvtWord tensou_ride[245];
EvtWord tensou_switch[62];
EvtWord evt_tensou2[216];
EvtWord tensou_init[69];
static EvtWord evt_haguruma[5];
EvtWord aji_07_init_evt[118];

f32 fd_aji_07_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_aji_07_neg_one_382 = -1.0f;

f32 fd_aji_07___local_PI_383 = 3.1415927f;

f32 fd_aji_07___local_3PIO2_385 = 4.712389f;

f32 fd_aji_07_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_aji_07___local_PIO2_384 = 1.5707964f;

f32 fd_aji_07_one_381 = 1.0f;

f32 fd_aji_07_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_aji_07_neg_one_365 = -1.0f;

f32 fd_aji_07___local_PI_366 = 3.1415927f;

f32 fd_aji_07___local_3PIO2_368 = 4.712389f;

f32 fd_aji_07_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_aji_07___local_PIO2_367 = 1.5707964f;

f32 fd_aji_07_one_364 = 1.0f;

static EVT_BEGIN(s_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_1"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.16601563f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_2"), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("S_door1_1"), LW(0), 0, 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_01", // nextMapName
        "n_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {0},
};

static USER_FUNC(scale_func) {
    f32 s = evtGetFloat(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);

    mario->scale = (Vec){s, s, s};
    if (party != NULL) {
        party->scale = (Vec){s, s, s};
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(tensou_ride)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -90, 0, FLOAT(100.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -115, 0, FLOAT(100.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    WAIT_MS(300)
    CALL(evt_hit_get_position, STRING("A_tensou"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP_LIGHT1"), LW(0), LW(1), LW(2), LW(9))
    SET_VALUE(GW(0), 0)
    CALL(evt_mario_get_dispdir, LW(0))
    CALL(evt_party_get_dispdir, 0, LW(1))
    BEGIN_INLINE_EVENT
        LOOP(0)
            ADD(LW(0), GW(0))
            ADD(LW(1), GW(0))
            CALL(evt_mario_set_dispdir, LW(0))
            CALL(evt_party_set_dispdir, 0, LW(1))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 11, 0, 30, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_cam_off)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_party_get_pos, 0, LW(5), LW(6), LW(7))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 12, 100, 0, 800)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(scale_func, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 12, 0, 70, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(8), LW(0))
        ADD(LW(0), LW(3))
        CALL(evt_mario_set_pos, LW(2), LW(0), LW(4))
        ADD(LW(8), LW(6))
        CALL(evt_party_set_pos, 0, LW(5), LW(8), LW(7))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_snd_sfxoff, LW(9))
    SWITCH(GSW(0))
        CASE_LESS(376)
        SET_VALUE(GSWF(4175), 1)
        CALL(evt_bero_mapchange, STRING("tik_00"), 0)
        CASE_EQUAL(376)
        CALL(evt_bero_mapchange, STRING("moo_04"), 0)
        CASE_DEFAULT
        SET_VALUE(GSWF(4175), 1)
        CALL(evt_bero_mapchange, STRING("tik_00"), 0)
    END_SWITCH
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(tensou_switch)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("sui__1"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("sui__2"), 1)
    CALL(evt_mapobj_get_position, STRING("sui__1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_SWITCH_WARP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("anm_saisho"), 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hikari"), 1)
    CALL(evt_map_checkanim, STRING("anm_saisho"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_map_playanim, STRING("anm_wa-p"), 1, 0)
    CALL(evt_run_case_evt, 0, 1, STRING("A_tensou"), 0, PTR(tensou_ride), 0)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_tensou2)
    CALL(evt_map_playanim, STRING("anm_wa-p"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("wa_pu"), 1)
    CALL(evt_hit_get_position, STRING("A_tensou"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP_LIGHT1"), LW(0), LW(1), LW(2), LW(9))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 5, 0, 5760, 4000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            MODULO(LW(0), 360)
            CALL(evt_party_set_dispdir, 0, LW(0))
            ADD(LW(0), 180)
            MODULO(LW(0), 360)
            CALL(evt_mario_set_dispdir, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 12, 0, 100, 800)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            CALL(scale_func, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_WARP1_2"), LW(2), LW(3), LW(4), 0)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_party_get_pos, 0, LW(5), LW(6), LW(7))
    CALL(evt_sub_intpl_msec_init, 12, 70, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(LW(8), LW(0))
        ADD(LW(0), LW(3))
        CALL(evt_mario_set_pos, LW(2), LW(0), LW(4))
        ADD(LW(8), LW(6))
        CALL(evt_party_set_pos, 0, LW(5), LW(8), LW(7))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_color, 1, STRING("wa_pu"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("wa_pu"), 1)
    CALL(evt_mapobj_color, 1, STRING("wa_pu"), 255, 255, 255, 255)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_cont_onoff, 1, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    RETURN
EVT_END()

EVT_BEGIN(tensou_init)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("sui__1"), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("sui__2"), 1)
    CALL(evt_run_case_evt, 9, 1, STRING("A__sui"), 0, PTR(tensou_switch), 0)
    IF_INT_EQUAL(GSWF(1326), 0)
        RETURN
    END_IF
    SET_VALUE(GSWF(1326), 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_cont_onoff, 1, 0)
    CALL(evt_mario_set_pos, -115, 9, 0)
    CALL(evt_party_set_pos, 0, -90, 9, 0)
    CALL(evt_party_set_pos, 1, -100, 9, -10)
    SPAWN_EVENT(PTR(evt_tensou2))
    RETURN
EVT_END()

static EVT_BEGIN(evt_haguruma)
    SET_VALUE(GSWF(4194), 1)
    RETURN
EVT_END()

EVT_BEGIN(aji_07_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN2"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG7_AJI4"))
    CALL(evt_snd_set_rev_mode, 1)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(tensou_init))
    CALL(evt_map_playanim, STRING("anm_PC"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_terop"), 1, 0)
    CALL(evt_map_playanim, STRING("anm_tiktik"), 1, 0)
    IF_INT_EQUAL(GSWF(4194), 0)
        CALL(evt_item_entry, STRING("haguruma"), 85, -65, 100, -120, 16, -1, PTR(evt_haguruma))
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("tenjo"))
        CALL(evt_cam_letter_box_disable, 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_party_cont_onoff, 1, 0)
        CALL(evt_mario_set_pos, 180, 200, -125)
        CALL(evt_party_set_pos, 0, 190, 200, -115)
        CALL(evt_party_set_pos, 1, 200, 200, -105)
        BEGIN_INLINE_EVENT
            CALL(evt_seq_wait, 2)
            CALL(evt_mario_cont_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_party_cont_onoff, 1, 1)
            CALL(evt_mario_wait_landon)
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_cam_letter_box_disable, 0)
        END_INLINE_EVENT
    END_IF
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()


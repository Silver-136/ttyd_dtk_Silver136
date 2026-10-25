#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "gra.h"
#include "cam_road.h"

USER_FUNC(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos);
USER_FUNC(camera_curve_off);
USER_FUNC(camera_curve_on);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set_now);
USER_FUNC(evt_cam_road_reset);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_cam_shift_reset);
USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);
USER_FUNC(evt_cloud_set_cap_size);
USER_FUNC(evt_eff);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_hit_get_position);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_iwamove_ent);
USER_FUNC(evt_iwamove_init);
USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_trans);
USER_FUNC(evt_mario_bgmode_off);
USER_FUNC(evt_mario_bgmode_on);
USER_FUNC(evt_mario_cam_y_off);
USER_FUNC(evt_mario_cam_y_on);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_jump_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mario_set_prev_party);
USER_FUNC(evt_mobj_breaking_floor);
USER_FUNC(evt_mobj_brick);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_party_force_reset_outofscreen);
USER_FUNC(evt_party_jump_pos);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_intpl_msec_get_value);
USER_FUNC(evt_sub_intpl_msec_init);

EvtWord gra_05_in_01[5];
EvtWord gra_05_out_01[149];
EvtWord gra_05_in_02[5];
EvtWord gra_05_out_02[149];
static EvtWord in_evt[5];
static EvtWord out_evt[113];
static EvtWord out_evt2[150];
static BeroEntry bero_entry_data[7];
EvtWord gra_05_futa_open_evt[28];
EvtWord evt_gra_05iwa_move_init[31];
EvtWord evt_gra_05iwa_move_evt[186];
EvtWord evt_break_floor[7];
static const char* fukitobashi_tbl[4];
EvtWord gra_05_init_evt[108];

EVT_BEGIN(gra_05_in_01)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(gra_05_out_01)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_cam3d_evt_set_now)
    CALL(evt_mario_cam_y_off)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_hit_get_position, STRING("A_ana_1"), LW(0), LW(1), LW(2))
        CALL(evt_party_force_reset_outofscreen, 0)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(150.0f))
    END_INLINE_EVENT
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_set_pos, 250, -50, 0)
    CALL(evt_party_set_pos, 0, 250, -50, 0)
    CALL(evt_party_set_pos, 1, 250, -50, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_party_set_dir, 1, 90, -1)
    CALL(evt_cam3d_evt_off, 500, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_party_jump_pos, 0, 310, 0, 0, 700, 0, -1000)
        CALL(evt_party_run, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        CALL(evt_party_jump_pos, 1, 310, 0, 0, 700, 0, -1000)
        CALL(evt_party_run, 1)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP2"), LW(1), LW(2), LW(3), 0)
    CALL(evt_mario_jump_pos, 310, 0, 0, 700, 0, -1000)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_cam_y_on)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(gra_05_in_02)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(gra_05_out_02)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_cam3d_evt_set_now)
    CALL(evt_mario_cam_y_off)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_hit_get_position, STRING("A_ana_2"), LW(0), LW(1), LW(2))
        CALL(evt_party_force_reset_outofscreen, 0)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(150.0f))
    END_INLINE_EVENT
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_set_pos, 0, -50, 0)
    CALL(evt_party_set_pos, 0, 0, -50, 0)
    CALL(evt_party_set_pos, 1, 0, -50, 0)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, -1)
    CALL(evt_party_set_dir, 1, 270, -1)
    CALL(evt_cam3d_evt_off, 500, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_party_jump_pos, 0, -60, 0, 0, 700, 0, -1000)
        CALL(evt_party_run, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        CALL(evt_party_jump_pos, 1, -60, 0, 0, 700, 0, -1000)
        CALL(evt_party_run, 1)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP4"), LW(1), LW(2), LW(3), 0)
    CALL(evt_mario_jump_pos, -60, 0, 0, 700, 0, -1000)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_cam_y_off)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static EVT_BEGIN(in_evt)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static EVT_BEGIN(out_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_cam3d_evt_set_now)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_hit_get_position, STRING("ana_1"), LW(0), LW(1), LW(2))
        CALL(evt_party_force_reset_outofscreen, 0)
        CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(150.0f))
    END_INLINE_EVENT
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_run, 0)
    CALL(evt_mario_bgmode_on)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_mario_cam_y_off)
    CALL(evt_cam_road_reset)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 700, 11)
    CALL(evt_hit_get_position, STRING("ana_2"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 50)
    ADD(LW(2), -10)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP5"), LW(1), LW(2), LW(3), 0)
    CALL(evt_mario_jump_pos, -140, 60, -450, 700, 0, -1000)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static EVT_BEGIN(out_evt2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_cam3d_evt_set_now)
    CALL(evt_hit_get_position, STRING("ana_2"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 50)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 500, 0, -1000)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    CALL(evt_mario_set_pos, -260, -60, 0)
    CALL(evt_party_set_pos, 0, -260, -70, -5)
    CALL(evt_party_set_pos, 1, -260, -80, -10)
    CALL(evt_mario_bgmode_off)
    CALL(evt_mario_set_prev_party, 0)
    CALL(evt_mario_set_prev_party, 1)
    CALL(evt_cam_road_reset)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 700, 11)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    CALL(evt_party_set_dir, 1, 90, -1)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_party_jump_pos, 0, -190, 0, -10, 700, 0, -1000)
        CALL(evt_party_run, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        CALL(evt_party_jump_pos, 1, -190, 0, -20, 700, 0, -1000)
        CALL(evt_party_run, 1)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FLD_JUMP3"), LW(1), LW(2), LW(3), 0)
    CALL(evt_mario_jump_pos, -190, 0, 0, 700, 0, -1000)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

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
        "gra_04", // nextMapName
        "e_bero_2", // nextBeroName
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
        "gra_06", // nextMapName
        "sw_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "ana_1", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)in_evt, // enterEvent
        0, // caseType
        (s32*)out_evt, // exitEvent
        "gra_05", // nextMapName
        "ana_2", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "ana_2", // hitName
        7936, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)in_evt, // enterEvent
        0, // caseType
        (s32*)out_evt2, // exitEvent
        "gra_05", // nextMapName
        "ana_1", // nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "A_ana_1", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)gra_05_in_01, // enterEvent
        0, // caseType
        (s32*)gra_05_out_01, // exitEvent
        0, NULL, // nextMapName..nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "A_ana_2", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)gra_05_in_01, // enterEvent
        0, // caseType
        (s32*)gra_05_out_02, // exitEvent
        0, NULL, // nextMapName..nextBeroName
        4, // enterAnimeType
        4, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(gra_05_futa_open_evt)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(1000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_INLINE_EVENT
    CALL(evt_hitobj_onoff, STRING("A_futa"), 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 190)
    RETURN
EVT_END()

EVT_BEGIN(evt_gra_05iwa_move_init)
    CALL(evt_hit_bind_mapobj, STRING("A_iwa"), STRING("S_iwa"))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 192)
        CALL(evt_mapobj_trans, 0, STRING("S_iwa"), FLOAT(120.0f), 0, 0)
        CALL(evt_hit_bind_update, STRING("A_iwa"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_futa"), 1)
        CALL(evt_hitobj_onoff, STRING("A_futa"), 0, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_gra_05iwa_move_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_get_position, STRING("S_iwa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_MOVE_ROCK1"), LW(0), LW(1), LW(2), LW(9))
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 3000)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_mario_set_pose, STRING("M_O_1"))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(evt_mapobj_get_position, STRING("S_iwa"), LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 30)
            LOOP(5)
                CALL(evt_eff, 0, STRING("kemuri"), 2, LW(0), 5, 49, 0, 0, 0, 0, 0, 0, 0, 0)
                ADD(LW(0), 20)
            END_LOOP
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(evt_mapobj_get_position, STRING("S_iwa"), LW(0), LW(1), LW(2))
            CALL(evt_eff, 0, STRING("kemuri"), 2, LW(0), 52, -449, 0, 0, 0, 0, 0, 0, 0, 0)
            ADD(LW(0), 20)
            CALL(evt_eff, 0, STRING("kemuri"), 2, LW(0), 52, -449, 0, 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 0, 0, 120, 3000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 0, STRING("S_iwa"), LW(0), 0, 0)
    SET_VALUE(LW(5), LW(2))
    ADD(LW(5), LW(0))
    CALL(evt_mario_set_pos, LW(5), LW(3), LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_hit_bind_update, STRING("A_iwa"))
    CALL(evt_snd_sfxoff, LW(9))
    SET_VALUE(GSW_STORY_PROGRESS, 192)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_break_floor)
    SET_VALUE(GSW_STORY_PROGRESS, 191)
    CALL(camera_curve_on)
    RETURN
EVT_END()

static const char* fukitobashi_tbl[] = {
    "S_futa",
    "S_kura",
    "S_gra04_kusa",
    NULL,
};

EVT_BEGIN(gra_05_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_GRA1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG4_GRA1"))
    CALL(evt_mobj_brick, STRING("blk00"), -475, 60, 25, 0, 13, 0, GSWF(2085))
    CALL(evt_mobj_powerupblk, STRING("blk01"), 630, 60, 40, 0, GSWF(5548), 0)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    IF_INT_LESS(GSW_STORY_PROGRESS, 190)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 1, PTR(&fukitobashi_tbl), STRING("A_futa"), PTR(gra_05_futa_open_evt))
        CALL(evt_cloud_set_cap_size, 256, 256)
        CALL(evt_hit_bind_mapobj, STRING("A_futa"), STRING("S_futa"))
        CALL(evt_mapobj_trans, 1, STRING("S_futa"), 0, FLOAT(0.099609375f), 0)
        CALL(evt_hit_bind_update, STRING("A_futa"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_futa"), 1)
        CALL(evt_hitobj_onoff, STRING("A_futa"), 0, 0)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 191)
        CALL(camera_curve_off)
        CALL(evt_mobj_breaking_floor, STRING("breaking_floor"), -250, -7, 0, 0, PTR(evt_break_floor), GSWF(2080))
    END_IF
    SPAWN_EVENT(PTR(evt_gra_05iwa_move_init))
    IF_INT_LESS(GSW_STORY_PROGRESS, 192)
        CALL(evt_iwamove_init)
        CALL(evt_iwamove_ent, PTR(evt_gra_05iwa_move_evt))
    END_IF
    RETURN
EVT_END()


USER_FUNC(camera_curve_off) {
    camRoadCurveOff("|MarioSt_CameraRoad|curve2");
    return EVT_RETURN_DONE;
}


USER_FUNC(camera_curve_on) {
    camRoadCurveOn("|MarioSt_CameraRoad|curve2");
    return EVT_RETURN_DONE;
}


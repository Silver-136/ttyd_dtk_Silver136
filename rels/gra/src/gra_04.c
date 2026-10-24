#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "gra.h"
#include "manager/evtmgr_cmd.h"

#pragma force_active on

USER_FUNC(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos);
USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set_now);
USER_FUNC(evt_cam_road_reset);
USER_FUNC(evt_cam_shift_reset);
USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);
USER_FUNC(evt_cloud_set_cap_size);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mario_bgmode_off);
USER_FUNC(evt_mario_bgmode_on);
USER_FUNC(evt_mario_cam_y_off);
USER_FUNC(evt_mario_cam_y_on);
USER_FUNC(evt_mario_get_exparty);
USER_FUNC(evt_mario_get_party);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_party_use_check);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_prev_party_pos);
USER_FUNC(evt_mario_wait_landon);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(roll_release_check);

f32 fd_gra_04_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_gra_04_neg_one_382;
f32 fd_gra_04___local_PI_383;
f32 fd_gra_04___local_3PIO2_385;
f32 fd_gra_04_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_gra_04___local_PIO2_384;
f32 fd_gra_04_one_381;
f32 fd_gra_04_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_gra_04_neg_one_365;
f32 fd_gra_04___local_PI_366;
f32 fd_gra_04___local_3PIO2_368;
f32 fd_gra_04_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_gra_04___local_PIO2_367;
f32 fd_gra_04_one_364;
static EvtWord in_evt[5];
static EvtWord out_evt[142];
static EvtWord out_evt2[110];
static BeroEntry bero_entry_data[5];
EvtWord gra_04_tree_open_evt[41];
static const char* fukitobashi_tbl[6];
EvtWord gra_04_init_evt[64];

f32 fd_gra_04_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gra_04_neg_one_382 = -1.0f;

f32 fd_gra_04___local_PI_383 = 3.1415927f;

f32 fd_gra_04___local_3PIO2_385 = 4.712389f;

f32 fd_gra_04_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gra_04___local_PIO2_384 = 1.5707964f;

f32 fd_gra_04_one_381 = 1.0f;

f32 fd_gra_04_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gra_04_neg_one_365 = -1.0f;

f32 fd_gra_04___local_PI_366 = 3.1415927f;

f32 fd_gra_04___local_3PIO2_368 = 4.712389f;

f32 fd_gra_04_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gra_04___local_PIO2_367 = 1.5707964f;

f32 fd_gra_04_one_364 = 1.0f;

static EVT_BEGIN(in_evt)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static EVT_BEGIN(out_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(roll_release_check, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        DEBUG_PUT_MSG(STRING("ロール解除できません"))
        CALL(evt_mario_wait_landon)
        CALL(evt_mario_mov_pos2, FLOAT(-315.0f), FLOAT(-15.0f), FLOAT(80.0f))
    END_IF
    CALL(evt_mario_normalize)
    SET_VALUE(LF(0), 0)
    SET_VALUE(LF(1), 0)
    CALL(evt_mario_get_exparty, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
        BEGIN_INLINE_EVENT_GET_ID(LW(10))
            CALL(evt_party_stop, 1)
            CALL(evt_party_move_pos2, 1, -375, -20, FLOAT(150.0f))
            CALL(evt_mario_kill_party, 1)
        END_INLINE_EVENT
    END_IF
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LF(1), 1)
        BEGIN_INLINE_EVENT_GET_ID(LW(11))
            CALL(evt_party_stop, 0)
            CALL(evt_party_move_pos2, 0, -375, -25, FLOAT(150.0f))
            CALL(evt_mario_kill_party, 0)
        END_INLINE_EVENT
    END_IF
    CALL(evt_mario_mov_pos2, -375, -30, FLOAT(150.0f))
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set_now)
    IF_INT_EQUAL(LF(0), 1)
        DELETE_EVENT(LW(10))
    END_IF
    IF_INT_EQUAL(LF(1), 1)
        DELETE_EVENT(LW(11))
    END_IF
    WAIT_FRAMES(1)
    CALL(evt_mario_bgmode_on)
    CALL(evt_mario_cam_y_off)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_mario_set_pos, -242, 10, -450)
    CALL(evt_mario_mov_pos2, -190, -450, FLOAT(150.0f))
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_road_reset)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 1000, 11)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static EVT_BEGIN(out_evt2)
    CALL(evt_mario_key_onoff, 0)
    CALL(roll_release_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_normalize)
        ELSE
        DEBUG_PUT_MSG(STRING("ロール解除できません"))
    END_IF
    CALL(evt_mario_mov_pos2, -242, -450, FLOAT(150.0f))
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set_now)
    CALL(evt_mario_bgmode_off)
    CALL(evt_mario_cam_y_on)
    CALL(evt_mario_set_pos, -380, 0, -10)
    CALL(evt_mario_set_prev_party_pos, 0, -380, 0, -15)
    CALL(evt_mario_set_prev_party_pos, 1, -380, 0, -20)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    WAIT_FRAMES(1)
    CALL(evt_party_set_pos, 0, -400, 5, -15)
    CALL(evt_party_set_pos, 1, -400, 5, -20)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_party_set_pos, 0, -373, 0, -32)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_party_move_pos2, 0, -295, -15, FLOAT(150.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -280, -10, FLOAT(150.0f))
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_road_reset)
    CALL(evt_cam_shift_reset)
    CALL(evt_cam3d_evt_off, 1000, 11)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
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
        "gra_03", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_1", // hitName
        0, 0, // type..sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)in_evt, // enterEvent
        -1, // caseType
        (s32*)out_evt, // exitEvent
        "gra_04", // nextMapName
        "w_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        4096, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        (s32*)in_evt, // enterEvent
        0, // caseType
        (s32*)out_evt2, // exitEvent
        "gra_04", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        4096, // type
        0, // sfxId
        2, // directionId
        -132, // centerPosition
        63, // centerPositionY
        -450, // centerPositionZ
        -1, // length
        NULL, 0, NULL, // enterEvent..exitEvent
        "gra_05", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(gra_04_tree_open_evt)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(1000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_mario_party_use_check, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_hitobj_onoff, STRING("w_bero_1"), 0, 1)
    SET_VALUE(GSW(0), 189)
    RETURN
EVT_END()

static const char* fukitobashi_tbl[] = {
    "S_futa",
    "S_gra04_kideka",
    "S_gra04_ishi",
    "S_gra04_kusa",
    "S_gra04_dai2",
    NULL,
};

EVT_BEGIN(gra_04_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_GRA1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG4_GRA1"))
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    IF_INT_LESS(GSW(0), 189)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 1, PTR(&fukitobashi_tbl), STRING("A_futa"), PTR(gra_04_tree_open_evt))
        CALL(evt_cloud_set_cap_size, 256, 256)
        CALL(evt_hitobj_onoff, STRING("w_bero_1"), 0, 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_futa"), 1)
        CALL(evt_hitobj_onoff, STRING("A_futa"), 0, 0)
    END_IF
    CALL(evt_mobj_badgeblk, STRING("m\0\0"), 250, 60, -20, 249, 0, GSWF(2086), 1)
    RETURN
EVT_END()

s32 marioRollCancelDisable(void);

USER_FUNC(roll_release_check) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], marioRollCancelDisable() == 0);
    return EVT_RETURN_DONE;
}

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_cam3d_evt_off);
USER_FUNC(evt_cam3d_evt_set);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_exit_case_evt);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_wait_landon);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon_3d);

static BeroEntry bero_data[3];
static EvtWord evt_dakuto[44];
EvtWord evt_nusumi[182];
static EvtWord evt_first[42];
EvtWord tou_13_init_evt[117];

static BeroEntry bero_data[] = {
    {
        "e_bero_", // hitName
        0, 0, // type..sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_07", // nextMapName
        "e_bero", // nextBeroName
        0, 0, 0, 0, // enterAnimeType..exitAnimeArg
    },
    {
        "w_bero_", // hitName
        0, 0, // type..sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_05", // nextMapName
        "w_bero", // nextBeroName
        0, 0, 0, 0, // enterAnimeType..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_dakuto)
    CALL(evt_cam_shake, 4, FLOAT(0.049804688f), FLOAT(0.0f), 500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_DUCT_KOUSHI_HIT1"), 188, 0, 0, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_deguti"), 1)
    CALL(evt_hitobj_onoff, STRING("deguti"), 1, 0)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_on))
    IF_INT_LESS(GSW_STORY_PROGRESS, 162)
        SET_VALUE(GSW_STORY_PROGRESS, 162)
        ELSE
        SET_VALUE(GSWF(2501), 1)
    END_IF
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(evt_nusumi)
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), 75)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_wait_landon)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_toge, 2, 200, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg3_tou_455_01"), 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING("m"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, 3, STRING("p"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 60, 0, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 100, 0, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_cam3d_evt_set, 125, 100, 337, 125, 40, -8, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_toge, 2, 200, 40, 0)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg3_tou_455_02"), 0, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_455_05"))
    WAIT_MS(1000)
    CALL(evt_msg_toge, 2, 200, 40, 0)
    CALL(evt_msg_print, 0, STRING("stg3_tou_455_03"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_msg_toge, 2, 200, 40, 0)
    CALL(evt_msg_print, 0, STRING("stg3_tou_455_04"), 0, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 161)
    RETURN
EVT_END()

static EVT_BEGIN(evt_first)
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), -120)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_wait_landon)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_455"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 160)
    RETURN
EVT_END()

EVT_BEGIN(tou_13_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(127, 137)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU7"))
        CALL(evt_snd_bgmoff, 26624)
        CASE_GREATER_OR_EQUAL(165)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU7"))
        CALL(evt_snd_bgmoff, 26624)
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU7"))
        CALL(evt_snd_set_rev_mode, 2)
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(162)
        SET_VALUE(LF(0), 0)
        CASE_LESS(172)
        SET_VALUE(LF(0), 1)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2501), 0)
            SET_VALUE(LF(0), 0)
            ELSE
            SET_VALUE(LF(0), 1)
        END_IF
    END_SWITCH
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_run_case_evt, 14, 1, STRING("deguti"), 0, PTR(evt_dakuto), 0)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(bero_case_switch_off))
        ELSE
        CALL(evt_hitobj_onoff, STRING("deguti"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_deguti"), 1)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 161)
        SPAWN_EVENT(PTR(evt_nusumi))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 160)
        SPAWN_EVENT(PTR(evt_first))
    END_IF
    RETURN
EVT_END()


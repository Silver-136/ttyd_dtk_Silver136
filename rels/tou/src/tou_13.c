#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"


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
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_deguti"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("deguti"), 1, 0)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(bero_case_switch_on))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_30_04_AIR_DUCT_EXIT)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_30_04_AIR_DUCT_EXIT)
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
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 0, 0)
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
    CALL(evt_cam3d_evt_set, 125, 100, 337, 125, 40, -8, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 40, 0)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg3_tou_455_02"), 0, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_455_05"))
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 40, 0)
    CALL(evt_msg_print, 0, STRING("stg3_tou_455_03"), 0, 0)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 40, 0)
    CALL(evt_msg_print, 0, STRING("stg3_tou_455_04"), 0, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG3_30_03_EAVESDROPPING)
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
    SET_VALUE(GSW_STORY_PROGRESS, STG3_30_02_AIR_DUCT_1ST_TIME)
    RETURN
EVT_END()

EVT_BEGIN(tou_13_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG3_00_WELCOME_TO_GLITZVILLE, STG3_12_THE_MINI_YOSHI_EGG)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU7"))
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_GREATER_OR_EQUAL(STG3_33_PEACH_THE_SECRET_WEAPON)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU7"))
        CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU7"))
        CALL(evt_snd_set_rev_mode, 2)
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_30_04_AIR_DUCT_EXIT)
        SET_VALUE(LF(0), 0)
        CASE_LESS(STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LF(0), 1)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2501), 0)
            SET_VALUE(LF(0), 0)
            ELSE
            SET_VALUE(LF(0), 1)
        END_IF
    END_SWITCH
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_run_case_evt, CASE_HAMMER_2, 1, STRING("deguti"), 0, PTR(evt_dakuto), 0)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(bero_case_switch_off))
        ELSE
        CALL(evt_hitobj_onoff, STRING("deguti"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_deguti"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_30_03_EAVESDROPPING)
        SPAWN_EVENT(PTR(evt_nusumi))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_30_02_AIR_DUCT_1ST_TIME)
        SPAWN_EVENT(PTR(evt_first))
    END_IF
    RETURN
EVT_END()


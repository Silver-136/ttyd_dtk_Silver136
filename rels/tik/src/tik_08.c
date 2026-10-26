#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_cam.h"
#include "evt/evt_mobj.h"
#include "pmario_sound.h"


EvtWord oshimodo_event_sub[33];
static EvtWord oshimodo_event[10];
static BeroEntry bero_oshimodo[1];
static BeroEntry bero_entry_data[3];
static BeroEntry bero_entry_data2[2];
static EvtWord evt_kanban[16];
EvtWord tik_08_init_evt[68];

EVT_BEGIN(oshimodo_event_sub)
    CALL(evt_cam_ctrl_onoff, 4, 0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(750)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("tik_08_01"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_ctrl_onoff, 4, 1)
    RETURN
EVT_END()

static EVT_BEGIN(oshimodo_event)
    SPAWN_EVENT(PTR(oshimodo_event_sub))
    SET_VALUE(GSWF(1341), 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_oshimodo[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        oshimodo_event, // exitEvent
        0, // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

static BeroEntry bero_entry_data[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "usu_00", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        1, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_07", // nextMapName
        "n_bero", // nextBeroName
        2, // enterAnimeType
        2, // exitAnimeType
        "door", // enterAnimeArg
        0, // exitAnimeArg
    },
    {0},
};

static BeroEntry bero_entry_data2[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "sys_01", // nextMapName
        "stg_4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_kanban)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_msg_print, 0, STRING("tik_08_02"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(tik_08_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG3_51_FRANKLYS_HOUSE_STG_3)
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan"), PTR(bero_oshimodo))
        SET_VALUE(LW(0), STRING("dokan"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_52_DARKLY)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG4_00_WELCOME_TO_STAGE_4)
            SET_VALUE(LW(0), STRING("dokan"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_bero_overwrite, STRING("dokan"), PTR(bero_entry_data2))
            SET_VALUE(LW(0), STRING("dokan"))
            CALL_CHILD(PTR(bero_case_switch_on))
        END_IF
    END_IF
    CALL(evt_mobj_signboard, STRING("board"), 85, 0, 0, PTR(evt_kanban), 0)
    RETURN
EVT_END()


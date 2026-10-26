#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_cam.h"
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "manager/evtmgr_cmd.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "evt/evt_bero.h"
#include "aji.h"

USER_FUNC(camAtY);
USER_FUNC(slit_chk);

static EvtWord s_door_open[22];
static EvtWord s_door_close[22];
EvtWord evt_slit0[15];
EvtWord evt_slit1[15];
EvtWord evt_slit2[15];
static BeroEntry bero_entry_data[5];
EvtWord heya_tenura_onoff[68];
EvtWord aji_08_init_evt[148];

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

EVT_BEGIN(evt_slit0)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_bero_mapchange, STRING("aji_06"), STRING("tenjo"))
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_slit1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_bero_mapchange, STRING("aji_07"), STRING("tenjo"))
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_slit2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_bero_mapchange, STRING("aji_09"), STRING("tenjo"))
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "s_bero_1", // hitName
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
        "n_bero_3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {
        "s_bero_2", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, 0, // enterEvent..caseType
        evt_slit0, // exitEvent
        "aji_06", // nextMapName
        "tenjo", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero_3", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, 0, // enterEvent..caseType
        evt_slit1, // exitEvent
        "aji_07", // nextMapName
        "tenjo", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero_4", // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, 0, // enterEvent..caseType
        evt_slit2, // exitEvent
        "aji_09", // nextMapName
        "tenjo", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(heya_tenura_onoff)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_heya"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_tenura"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(camAtY, LW(1))
        IF_INT_GREATER_OR_EQUAL(LW(1), 180)
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_heya"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_tenura"), MAP_OBJ_FLAG_HIDDEN)
                SET_VALUE(LW(0), 1)
            END_IF
            ELSE
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_heya"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_tenura"), MAP_OBJ_FLAG_HIDDEN)
                SET_VALUE(LW(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(aji_08_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI2"))
    CALL(evt_snd_set_rev_mode, 1)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT_GET_ID(PTR(heya_tenura_onoff), LW(0))
    SPAWN_EVENT(PTR(evt_emergency))
    BEGIN_INLINE_EVENT
        CALL(slit_chk)
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("bari"), 1, 0)
    CALL(evt_map_playanim, STRING("awa1"), 1, 0)
    CALL(evt_map_playanim, STRING("awa2"), 1, 0)
    CALL(evt_item_entry, STRING("item00"), ITEM_STAR_PIECE, -975, 170, 75, ITEM_TYPE_STILL, GSWF(5666), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mapobj_get_position, STRING("S_ita"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_AMB_CEILING_MOVE1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -100, 100, 1000)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_ita"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
            CALL(evt_mapobj_get_position, STRING("S_ita"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_AMB_CEILING_MOVE1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 100, -100, 1000)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
            CALL(evt_mapobj_rotate, 1, STRING("S_ita"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 373)
        CALL(evt_npc_check_delete, STRING("軍団ザコ魔法使い"))
    END_IF
    RETURN
EVT_END()

USER_FUNC(camAtY) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], camGetPtr(CAMERA_3D)->cameraAt.y);
    return EVT_RETURN_DONE;
}
USER_FUNC(slit_chk) {
    MarioWork* mario = marioGetPtr();

    if (mario->flags & MARIO_FLAG_PAPER_MODE) {
        if ((mario->position.x < -200.0f || mario->position.x > 300.0f) && mario->position.y < 169.0f &&
            mario->position.y > 160.0f) {
            camCtrlOff(CAMERA_3D);
            evtStopID(event->lwData[0]);
            return EVT_RETURN_DONE;
        }
    }
    return EVT_RETURN_BLOCK;
}

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static EvtWord e_close[12];
static EvtWord e_open[12];
static EvtWord s_close[12];
static EvtWord s_open[12];
static BeroEntry bero_data[3];
EvtWord evt_jump[21];
EvtWord eki_04_init_evt[66];

static EVT_BEGIN(e_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("e_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("e_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("s_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("s_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "e_bero", // hitName
        1, // type
        3, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_03", // nextMapName
        "w_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_close, // enterAnimeArg
        e_open, // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        3, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_05", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_close, // enterAnimeArg
        s_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_jump)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_jump_jumpstand, 360, -900, 70, 300, 700, 75, 310, -900, 65, 6000, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(eki_04_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_cam_road_reset)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_EKI2"))
    CALL(evt_mobj_jumpstand_red, 0, STRING("mobj_jump"), 360, -900, 70, PTR(evt_jump), 0)
    CALL(evt_item_entry, STRING("item01"), ITEM_DRIED_SHROOM, -250, -815, 70, ITEM_TYPE_STILL, GSWF_EKI_04_DRIED_SHROOM, 0)
    CALL(evt_item_entry, STRING("item02"), ITEM_P_UP_D_DOWN, -250, 230, 40, ITEM_TYPE_STILL, GSWF_EKI_04_P_UP_D_DOWN, 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("futi_"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hasira"), MAP_OBJ_FLAG_CLIP)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_33_BLOW_THE_POSTER)
        SET_VALUE(GSW_STORY_PROGRESS, STG6_33_BLOW_THE_POSTER)
    END_IF
    RETURN
EVT_END()


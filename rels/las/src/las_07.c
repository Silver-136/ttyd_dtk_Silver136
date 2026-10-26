#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"


static EvtWord w1_door_close[22];
static EvtWord w1_door_open[22];
static EvtWord w2_door_close[22];
static EvtWord w2_door_open[22];
static EvtWord e1_door_close[22];
static EvtWord e1_door_open[22];
static EvtWord e2_door_close[22];
static EvtWord e2_door_open[22];
static EvtWord door_matigai_evt[16];
static BeroEntry bero_entry_data[5];
EvtWord las_07_init_evt[57];

static EVT_BEGIN(w1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door_matigai_evt)
    IF_INT_EQUAL(GSWF_LAS_WRONG_DOOR_SFX_PENDING, 1)
        CALL(evt_snd_sfxon, STRING("SFX_STG8_DOOR_NG_OPEN1"), 0)
        SET_VALUE(GSWF_LAS_WRONG_DOOR_SFX_PENDING, 0)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        door_matigai_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_06", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w1_door_close, // enterAnimeArg
        w1_door_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        door_matigai_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_08", // nextMapName
        "w_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        door_matigai_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_08", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w2_door_close, // enterAnimeArg
        w2_door_open, // exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        door_matigai_evt, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_08", // nextMapName
        "w_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_door_close, // enterAnimeArg
        e2_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(las_07_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_cam_road_reset)
    END_INLINE_EVENT
    CALL(evt_mobj_badgeblk, STRING("mobj00"), -100, 60, -300, ITEM_COIN, 0, GSWF_LAS_07_COIN_BLOCK, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj01"), 100, 60, -300, ITEM_JAMMIN_JELLY, 0, GSWF_LAS_07_JAMMIN_JELLY_BLOCK, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_badge_gat"), 0, 60, -300, ITEM_P_UP_D_DOWN_P, 0, GSWF_LAS_07_P_UP_D_DOWN_P_BADGE_BLOCK, 1)
    RETURN
EVT_END()


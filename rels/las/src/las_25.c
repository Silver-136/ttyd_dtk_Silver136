#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "las.h"
#include "mario/mario_pouch.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/mobjdrv.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

USER_FUNC(evt_win_item_select);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e1_door_close[22];
static EvtWord e1_door_open[22];
static EvtWord e2_door_close[22];
static EvtWord e2_door_open[22];
static BeroEntry bero_entry_data[5];
static BeroEntry w_bero_mapchg[1];
static s32 key_tbl[2];
static EvtWord key_check_evt[23];
static EvtWord unlock_evt[10];
EvtWord las_25_init_evt[174];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
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
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_23", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_26", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_27", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_door_close, // enterAnimeArg
        e2_door_open, // exitAnimeArg
    },
    {
        "n_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_24", // nextMapName
        "s_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static BeroEntry w_bero_mapchg[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_23", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
};

static s32 key_tbl[] = {
    ITEM_PALACE_KEY_0x30, -1,
};

static EVT_BEGIN(key_check_evt)
    CALL(evt_pouch_check_item, ITEM_PALACE_KEY_0x30, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(unlock_evt)
    SET_VALUE(GSW_STORY_PROGRESS, STG8_14_LOCK)
    SET_VALUE(LW(0), STRING("e_bero_2"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(las_25_init_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST1"))
        ELSE
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST3"))
    END_IF
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_npc_check_delete, STRING("ファントム３"))
        CALL(evt_npc_check_delete, STRING("ファントム４"))
        CALL(evt_npc_check_delete, STRING("ファントム５"))
        CALL(evt_npc_check_delete, STRING("ファントム６"))
        ELSE
        CALL(evt_npc_check_delete, STRING("ファントム１"))
        CALL(evt_npc_check_delete, STRING("ファントム２"))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_under"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_under"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hikou"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_hi"), 1, 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yuka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_be"), 1, 0)
        CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hi"))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("w_bero"), PTR(w_bero_mapchg))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        SPAWN_EVENT(PTR(hikari_evt))
    END_IF
    CALL(evt_mobj_lock, STRING("lock"), 48, 690, -380, 0, -90, PTR(key_check_evt), PTR(unlock_evt), GSWF(4359))
    IF_INT_EQUAL(GSWF(4359), 0)
        SET_VALUE(LW(0), STRING("e_bero_2"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    CALL(evt_mobj_jumpstand_blue, 0, STRING("jump"), 720, -675, -600, 75, 0, 0)
    CALL(evt_mobj_flag_onoff, 0, STRING("jump"), MOBJ_FLAG_GRAVITY)
    CALL(evt_item_entry, STRING("item02"), ITEM_LIFE_SHROOM, 225, -350, 0, ITEM_TYPE_STILL, GSWF(4379), 0)
    CALL(evt_item_entry, STRING("item03"), ITEM_SHOOTING_STAR, 375, -450, 0, ITEM_TYPE_STILL, GSWF(4392), 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 600, 60, -75, 0, 0)
    RETURN
EVT_END()


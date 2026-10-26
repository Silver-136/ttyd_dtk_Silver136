#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "las.h"
#include "evt/evt_cam.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "driver/mobjdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"


static EvtWord w_door_close[12];
static EvtWord w_door_open[12];
static BeroEntry bero_entry_data[2];
static EvtWord box_evt[30];
static EvtWord switch_evt[46];
static EvtWord hint_plate[14];
EvtWord kabenuke_check_evt[42];
EvtWord las_12_init_evt[62];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        1, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_10", // nextMapName
        "ne_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x27, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(switch_evt)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1250)
    CALL(evt_cam3d_evt_set, -150, 100, 363, -150, 30, -30, 500, 11)
    WAIT_MS(1000)
    CALL(evt_mobj_set_position, STRING("box"), -150, -30, 0)
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_GRAVITY | MOBJ_FLAG_NO_DISP)
    CALL(evt_mobj_hit_onoff, STRING("box"), 1)
    SET_VALUE(LW(0), STRING("box"))
    CALL_CHILD(PTR(box_syutugen))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(hint_plate)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_37"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(kabenuke_check_evt)
    IF_INT_EQUAL(GSWF(4381), 1)
        RETURN
    END_IF
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    SET_VALUE(GSWF(4381), 1)
    RETURN
EVT_END()

EVT_BEGIN(las_12_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_floatswitch_blue, 0, STRING("switch"), 150, 60, 0, PTR(switch_evt), GSWF(4342))
    CALL(evt_mobj_itembox, STRING("box"), -150, 0, 0, 0, 0, PTR(box_evt), GSWF(4363))
    IF_INT_EQUAL(GSWF(4342), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("box"), MOBJ_FLAG_NO_DISP)
        CALL(evt_mobj_hit_onoff, STRING("box"), 0)
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_pure"), 0, PTR(hint_plate), 0)
    SPAWN_EVENT(PTR(kabenuke_check_evt))
    RETURN
EVT_END()


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
#include "driver/itemdrv.h"


static EvtWord e_door_close[12];
static EvtWord e_door_open[12];
static BeroEntry bero_entry_data[2];
static EvtWord box_evt[30];
static EvtWord switch_evt[62];
static EvtWord block_break[22];
static EvtWord hint_plate[14];
EvtWord las_15_init_evt[96];

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "e_bero_1", // hitName
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
        "nw_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x2A, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(switch_evt)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1250)
    CALL(evt_cam3d_evt_set, 0, 100, 363, 0, 30, -30, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_mobj_set_position, STRING("box"), 0, -30, 0)
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_GRAVITY | MOBJ_FLAG_NO_DISP)
    CALL(evt_mobj_hit_onoff, STRING("box"), 1)
    SET_VALUE(LW(0), STRING("box"))
    CALL_CHILD(PTR(box_syutugen))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(block_break)
    CALL(evt_mobj_get_position, STRING("me"), LW(0), LW(1), LW(2))
    WAIT_MS(900)
    WAIT_MS(500)
    SUBTRACT(LW(1), 1400)
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_OJAMA1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

static EVT_BEGIN(hint_plate)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_40"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(las_15_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_lv_blk, STRING("stone1"), 150, 0, 75, PTR(block_break), GSWF_LAS_11_15_BIG_BLOCK_3_BROKEN, STRING("MOBJ_Lv2BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("stone2"), -150, 50, 0, PTR(block_break), GSWF_LAS_11_15_BIG_BLOCK_4_BROKEN, STRING("MOBJ_Lv2BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("metal1"), 150, 50, 0, 0, GSWF_LAS_11_15_BIG_BLOCK_1_BROKEN, STRING("MOBJ_Lv3BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("metal2"), -150, 0, 75, 0, GSWF_LAS_11_15_BIG_BLOCK_2_BROKEN, STRING("MOBJ_Lv3BigBlock"))
    CALL(evt_mobj_floatswitch_blue, 0, STRING("switch"), 150, 110, -10, PTR(switch_evt), GSWF_LAS_15_SWITCH_PRESSED)
    CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, MOBJ_ITEMBOX_NORMAL, 0, PTR(box_evt), GSWF_LAS_15_PALACE_KEY_CHEST)
    IF_INT_EQUAL(GSWF_LAS_15_SWITCH_PRESSED, 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("box"), MOBJ_FLAG_NO_DISP)
        CALL(evt_mobj_hit_onoff, STRING("box"), 0)
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_pure"), 0, PTR(hint_plate), 0)
    RETURN
EVT_END()


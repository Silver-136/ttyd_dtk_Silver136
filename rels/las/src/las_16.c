#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_cam.h"
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static EvtWord w_door_close[12];
static EvtWord w_door_open[12];
static BeroEntry bero_entry_data[2];
static EvtWord box_evt[30];
static EvtWord bomb_wall_evt[206];
static EvtWord hint_plate[14];
EvtWord las_16_init_evt[84];

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
        "ne_bero_2", // nextBeroName
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
    CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x2B, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(bomb_wall_evt)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ana_ana"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_ana_ana"), STRING("ofs"))
    CALL(evt_offscreen_get_boundingbox2, STRING("ofs"), LW(0), LW(1), LW(2), LW(3))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(2), 0)
        IF_INT_EQUAL(LW(3), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("ofs"), 1, LW(0), LW(1), LW(2), LW(3))
        WAIT_FRAMES(1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_img_clear_virtual_point, STRING("img"))
        CALL(evt_img_onoff, STRING("img"), 1)
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        CALL(evt_img_wait_animend, STRING("img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_IF
    CALL(evt_hitobj_onoff, STRING("A_ana_ana"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_ana_before"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_ana_after"), 1, 1)
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_ana_ana"))
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_delete, STRING("ofs"))
    SET_VALUE(GSWF(4348), 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(hint_plate)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_41"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(las_16_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_EQUAL(GSWF(4348), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_ana_after"), 1, 0)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_ana_ana"), 0, PTR(bomb_wall_evt), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_ana"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_ana_ana"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_ana_before"), 1, 0)
    END_IF
    CALL(evt_mobj_itembox, STRING("box"), 150, 0, 0, 0, 0, PTR(box_evt), GSWF(4367))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_pure"), 0, PTR(hint_plate), 0)
    RETURN
EVT_END()


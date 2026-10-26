#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "npc_event.h"
#include "las.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"


static EvtWord e_door_close[12];
static EvtWord e_door_open[12];
static BeroEntry bero_entry_data[2];
static EvtWord box_evt[30];
static EvtWord switch_evt[46];
EvtWord enemy1_dead_evt[11];
EvtWord enemy2_dead_evt[11];
EvtWord enemy3_dead_evt[11];
EvtWord enemy4_dead_evt[102];
static EvtWord hint_plate[24];
EvtWord las_17_init_evt[123];

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
        "sw_bero_2", // nextBeroName
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
    CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x2C, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(switch_evt)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1250)
    CALL(evt_cam3d_evt_set, 0, 100, 363, 0, 30, -30, 500, 11)
    WAIT_MS(1000)
    CALL(evt_mobj_set_position, STRING("box"), 0, -30, 0)
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_GRAVITY | MOBJ_FLAG_NO_DISP)
    CALL(evt_mobj_hit_onoff, STRING("box"), 1)
    SET_VALUE(LW(0), STRING("box"))
    CALL_CHILD(PTR(box_syutugen))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(enemy1_dead_evt)
    CALL_CHILD(PTR(enemy_common_dead_event))
    IF_INT_EQUAL(GSW(1123), 0)
        SET_VALUE(GSW(1123), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(enemy2_dead_evt)
    CALL_CHILD(PTR(enemy_common_dead_event))
    IF_INT_EQUAL(GSW(1123), 1)
        SET_VALUE(GSW(1123), 2)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(enemy3_dead_evt)
    CALL_CHILD(PTR(enemy_common_dead_event))
    IF_INT_EQUAL(GSW(1123), 2)
        SET_VALUE(GSW(1123), 3)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(enemy4_dead_evt)
    CALL_CHILD(PTR(enemy_common_dead_event))
    IF_INT_EQUAL(GSW(1123), 3)
        SET_VALUE(GSW(1123), 4)
        SET_VALUE(GSWF(4350), 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_npc_change_fbat_mode, 7)
        CALL(evt_npc_stop_for_event)
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(1000)
        CALL(evt_cam3d_evt_set, 235, 90, 387, -147, 90, -8, 500, 11)
        WAIT_MS(1000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
        WAIT_MS(500)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_SWITCH_BIKKURI_APPEAR1"), -225, 60, 0, 0)
        CALL(evt_eff, 0, STRING("kemuri_test"), 7, -205, 60, 20, 1, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(100)
        CALL(evt_mobj_floatswitch_blue, 0, STRING("switch"), -225, 60, 0, PTR(switch_evt), GSWF(4351))
        WAIT_MS(500)
        WAIT_MS(1000)
        CALL(evt_cam3d_evt_off, 500, 11)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_npc_change_fbat_mode, 1)
        CALL(evt_npc_start_for_event)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(hint_plate)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_change_fbat_mode, 7)
    CALL(evt_npc_stop_for_event)
    CALL(evt_msg_print, 0, STRING("stg8_las_42"), 0, 0)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_npc_start_for_event)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(las_17_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST2"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS(GSW(1123), 4)
        SET_VALUE(GSW(1123), 0)
    END_IF
    CALL(evt_npc_flag_onoff, 0, STRING("クリボー"), NPC_FLAG_RECORD_DEFEAT)
    CALL(evt_npc_flag_onoff, 0, STRING("トゲクリボー"), NPC_FLAG_RECORD_DEFEAT)
    CALL(evt_npc_flag_onoff, 0, STRING("ノコノコ"), NPC_FLAG_RECORD_DEFEAT)
    CALL(evt_npc_flag_onoff, 0, STRING("シンノスケ"), NPC_FLAG_RECORD_DEFEAT)
    CALL(evt_npc_change_interrupt, STRING("クリボー"), 8, PTR(enemy1_dead_evt))
    CALL(evt_npc_change_interrupt, STRING("トゲクリボー"), 8, PTR(enemy2_dead_evt))
    CALL(evt_npc_change_interrupt, STRING("ノコノコ"), 8, PTR(enemy3_dead_evt))
    CALL(evt_npc_change_interrupt, STRING("シンノスケ"), 8, PTR(enemy4_dead_evt))
    IF_INT_EQUAL(GSW(1123), 4)
        CALL(evt_mobj_floatswitch_blue, 0, STRING("switch"), -225, 60, 0, PTR(switch_evt), GSWF(4351))
        CALL(evt_npc_check_delete, STRING("クリボー"))
        CALL(evt_npc_check_delete, STRING("トゲクリボー"))
        CALL(evt_npc_check_delete, STRING("ノコノコ"))
        CALL(evt_npc_check_delete, STRING("シンノスケ"))
    END_IF
    CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, 0, 0, PTR(box_evt), GSWF(4368))
    IF_INT_EQUAL(GSWF(4351), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("box"), MOBJ_FLAG_NO_DISP)
        CALL(evt_mobj_hit_onoff, STRING("box"), 0)
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_pure"), 0, PTR(hint_plate), 0)
    RETURN
EVT_END()


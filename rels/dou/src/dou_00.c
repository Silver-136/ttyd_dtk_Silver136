#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_telop.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "driver/mapdrv.h"
#include "driver/envdrv.h"
#include "evt/evt_bero.h"
#include "driver/seqdrv.h"

void water_tev(MapTevCallbackInfo* info);

static EvtWord bero_in_event[11];
static BeroEntry bero_entry_data[3];
static EvtWord back_light[35];
EvtWord totsunyu_event[46];
EvtWord dou_00_init_evt[61];

static EVT_BEGIN(bero_in_event)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 242)
        CALL_CHILD(PTR(totsunyu_event))
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_in_event, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "muj_05", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "dou_01", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(back_light)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 255, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_map_blend_set_flag, 0, STRING("mario"), 2048)
    CALL(evt_map_blend_off, 0)
    RETURN
EVT_END()

EVT_BEGIN(totsunyu_event)
    CALL(evt_map_set_blend, 0, 0, 0, 0, 255)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg5_dou_00"), 0, 0)
    SPAWN_EVENT(PTR(back_light))
    WAIT_MS(200)
    SET_VALUE(GSW_STORY_PROGRESS, 243)
    CALL(evt_mario_key_onoff, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_telop_entry, 10)
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(dou_00_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_DOU1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    WAIT_FRAMES(1)
    CALL(evt_map_playanim, STRING("nami"), 1, 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), 30, 85, -75, 0, 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_RUIN_POWDER, -245, 0, 85, ITEM_TYPE_STILL, GSWF(2990), 0)
    CALL(evt_map_set_tevcallback, 1, PTR(water_tev))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s00_suimen"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s00_suimen"), MAP_OBJ_FLAG_DISPLAY_LIST)
    RETURN
EVT_END()

void water_tev(MapTevCallbackInfo* info) {
    envSetWater(info);
}

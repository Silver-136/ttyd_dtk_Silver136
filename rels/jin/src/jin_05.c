#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "jin.h"
#include "evt/evt_item.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

USER_FUNC(evt_kagemario_init);

static EvtWord check_roll[16];
static BeroEntry bero_entry_data[2];
EvtWord item_box_00[34];
static EvtWord mobj_box01[34];
EvtWord jin_05_init_evt[63];

static EVT_BEGIN(check_roll)
    CALL(evt_mario_get_motion, LW(0))
    IF_INT_EQUAL(LW(0), 22)
        SET_VALUE(LW(0), 0)
        ELSE
        SET_VALUE(LW(0), 1)
    END_IF
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        check_roll, // exitEvent
        "jin_00", // nextMapName
        "n_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(item_box_00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("_item00"), ITEM_COOKBOOK, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("_item00"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("_item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box01)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box01"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("_item01"), ITEM_ICE_SMASH, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("_item01"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box01"))
    CALL(evt_item_get_item, STRING("_item01"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(jin_05_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_JIN1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_mobj_powerupblk, STRING("pblk00"), 65, 60, 35, 0, GSWF(5549), 0)
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 125, 0, 75, 0, 0, PTR(item_box_00), GSWF(2235))
    CALL(evt_mobj_itembox, STRING("mobj_box01"), -55, 0, 55, 0, 0, PTR(mobj_box01), GSWF(2237))
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, -10, 0, -40, ITEM_TYPE_STILL, GSWF(5635), 0)
    RETURN
EVT_END()


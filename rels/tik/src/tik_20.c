#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_movefloor.h"
#include "driver/hitdrv.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static BeroEntry bero_entry_data[2];
static HitDamageReturn hitDmgRetPoints[7];
EvtWord tik_20_init_evt[104];

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_15", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_1", {0.0f, -1000.0f, 0.0f}},
    {"mod_2", {0.0f, -1000.0f, 0.0f}},
    {"mod_3", {0.0f, -1000.0f, 0.0f}},
    {"mod_4", {0.0f, -1000.0f, 0.0f}},
    {"mod_5", {0.0f, -1000.0f, 0.0f}},
    {"ue", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(tik_20_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK7"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_playanim, STRING("anm_mizu"), 1, 0)
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change_01"), STRING("ue"))
    SET_VALUE(LW(0), STRING("dai_3"))
    SET_VALUE(LW(1), STRING("a_dai_3"))
    SET_VALUE(LW(2), -10)
    SET_VALUE(LW(3), 30)
    SET_VALUE(LW(4), 900)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    CALL(evt_item_entry, STRING("item_01"), ITEM_DEFEND_PLUS, 180, 100, -125, ITEM_TYPE_STILL, GSWF(1367), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk01"), -50, 60, -75, 0, GSWF(5535), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk02"), 0, 100, -25, 0, GSWF(5536), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk03"), 50, 60, -75, 0, GSWF(5537), 0)
    RETURN
EVT_END()


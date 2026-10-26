#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "data_tables.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static BeroEntry bero_entry_data[3];
static TreeAccess ki_data[5];
EvtWord evt_kusa_8[20];
static KusaAccess kusa_data[10];
EvtWord hei_06_init_evt[63];

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
        "hei_05", // nextMapName
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
        "hei_07", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static TreeAccess ki_data[] = {
    {0, "a_ki_1", "miki__1", "ha_temae_1", "ha_usiro_1", NULL},
    {0, "a_ki_2", "miki__2", "ha_temae_2", "ha_usiro_2", NULL},
    {0, "a_ki_3", "miki__3", "ha_temae_3", "ha_usiro_3", NULL},
    {0, "a_ki_4", "miki__4", "ha_temae_4", "ha_usiro_4", NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(evt_kusa_8)
    CALL(evt_item_delete_check, STRING("item_kusa_8"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_8"), ITEM_COIN, 200, 75, 60, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF(1794), 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess kusa_data[] = {
    {0, "a_kusa_2", "kusa_2", NULL},
    {0, "a_kusa_3", "kusa_3", NULL},
    {0, "a_kusa_4", "kusa_4", NULL},
    {0, "a_kusa_5", "kusa_5", NULL},
    {0, "a_kusa_6", "kusa_6", NULL},
    {0, "a_kusa_8", "kusa_8", evt_kusa_8},
    {0, "a_kusa_12", "kusa_12", NULL},
    {0, "a_kusa_13", "kusa_13", NULL},
    {0, "a_kusa_14", "kusa_14", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(hei_06_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_HEI1"))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_HEI1"))
    CALL(evt_npc_change_fbat_mode, 1)
    SET_VALUE(LW(0), PTR(ki_data))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    SET_VALUE(LW(0), PTR(kusa_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 5, 375, 160, -75, 0, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_hatenablk"), -25, 110, -75, 132, 0, GSWF(1795), 0)
    CALL(evt_item_entry, STRING("item_00"), ITEM_INN_COUPON, 50, 50, 105, ITEM_TYPE_STILL, GSWF(1806), 0)
    RETURN
EVT_END()


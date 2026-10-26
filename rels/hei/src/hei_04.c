#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "data_tables.h"
#include "evt/evt_map.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static BeroEntry bero_entry_data[3];
static EvtWord evt_ki_3[20];
static TreeAccess ki_data[4];
EvtWord evt_kusa_11[20];
EvtWord evt_kusa_12[20];
static KusaAccess kusa_data[12];
EvtWord hei_04_init_evt[40];

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
        "hei_03", // nextMapName
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
        "hei_05", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_ki_3)
    CALL(evt_item_delete_check, STRING("item_ki_3"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ki_3"), ITEM_COIN, 17, 90, -122, ITEM_TYPE_FALL_NO_PICKUP, GSWF_HEI_04_COIN_TREE, 0)
    END_IF
    RETURN
EVT_END()

static TreeAccess ki_data[] = {
    {0, "a_ki_1", "miki_1", "ha_temae", "ha_usiro", NULL},
    {0, "a_ki_2", "miki_2", "ha_temae_2", "ha_usiro_2", NULL},
    {0, "a_ki_3", "miki_3", "ha_temae_3", "ha_usiro_3", evt_ki_3},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(evt_kusa_11)
    CALL(evt_item_delete_check, STRING("item_kusa_11"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_11"), ITEM_COIN, 364, 75, -95, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_HEI_04_COIN_BUSH, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_kusa_12)
    CALL(evt_item_delete_check, STRING("item_kusa_12"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_12"), ITEM_POW_BLOCK, 320, 75, -55, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_HEI_04_POW_BLOCK_BUSH, 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess kusa_data[] = {
    {0, "a_kusa_1", "kusa_1", NULL},
    {0, "a_kusa_2", "kusa_2", NULL},
    {0, "a_kusa_3", "kusa_3", NULL},
    {0, "a_kusa_4", "kusa_4", NULL},
    {0, "a_kisa_5", "kusa_5", NULL},
    {0, "a_kusa_6", "kusa_6", NULL},
    {0, "a_kusa_7", "kusa_7", NULL},
    {0, "a_kusa_9", "kusa_9", NULL},
    {0, "a_kusa_10", "kusa_10", NULL},
    {0, "a_kusa_11", "kusa_11", evt_kusa_11},
    {0, "a_kusa_12", "kusa_12", evt_kusa_12},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(hei_04_init_evt)
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
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("group1"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()


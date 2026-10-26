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
#include "evt/evt_mobj.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);

static BeroEntry bero_entry_data[3];
EvtWord evt_ki_6[20];
static TreeAccess ki_data[8];
static EvtWord evt_kusa_1[20];
static KusaAccess kusa_data[16];
EvtWord hei_02_init_evt[129];

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
        "nok_01", // nextMapName
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
        "hei_03", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_ki_6)
    CALL(evt_item_delete_check, STRING("item_ki_6"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ki_6"), ITEM_COIN, 192, 92, -120, ITEM_TYPE_FALL_NO_PICKUP, GSWF_HEI_02_COIN_TREE, 0)
    END_IF
    RETURN
EVT_END()

static TreeAccess ki_data[] = {
    {0, "a_ki_1", "miki_1", "ha_temae_1", "ha_usiro_1", NULL},
    {0, "a_ki_2", "miki_2", "ha_temae_2", "ha_usiro_2", NULL},
    {0, "a_ki_3", "miki_3", "ha_temae_3", "ha_usiro_3", NULL},
    {0, "a_ki_4", "miki_4", "ha_temae_4", "ha_usiro_4", NULL},
    {0, "a_ki_5", "miki_5", "ha_temae_5", "ha_usiro_5", evt_ki_6},
    {0, "a_ki_6", "miki_6", "ha_temae_6", "ha_usiro_6", NULL},
    {0, "a_ki_7", "miki_7", "ha_temae_7", "ha_usiro_7", NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

static EVT_BEGIN(evt_kusa_1)
    CALL(evt_item_delete_check, STRING("item_kusa_1"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_1"), ITEM_STAR_PIECE, -645, 50, -70, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_STARPIECE_33, 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess kusa_data[] = {
    {0, "a_kusa_1", "kusa_1", evt_kusa_1},
    {0, "a_kusa_2", "kusa_2", NULL},
    {0, "a_kusa_3", "kusa_3", NULL},
    {0, "a_kusa_4", "kusa_4", NULL},
    {0, "a_kusa_5", "kusa_5", NULL},
    {0, "a_kusa_6", "kusa_6", NULL},
    {0, "a_kusa_7", "kusa_7", NULL},
    {0, "a_kusa_8", "kusa_8", NULL},
    {0, "a_kusa_9", "kusa_9", NULL},
    {0, "a_kusa_10", "kusa_10", NULL},
    {0, "a_kusa_11", "kusa_11", NULL},
    {0, "a_kusa_12", "kusa_12", NULL},
    {0, "a_kusa_13", "kusa_13", NULL},
    {0, "a_kusa_14", "kusa_14", NULL},
    {0, "a_ku_15", "kusa_15", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(hei_02_init_evt)
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
    CALL(evt_map_playanim, STRING("anm_mugi_1"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_mobj_badgeblk, STRING("m"), 425, 25, -50, ITEM_POW_BLOCK, 0, GSWF_HEI_02_POW_BLOCK_BLOCK, 0)
    CALL(evt_cloud_init)
    CALL(evt_cloud_ent, 0, STRING("kusa_1"), STRING("a_kusa_1"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_2"), STRING("a_kusa_2"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_3"), STRING("a_kusa_3"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_4"), STRING("a_kusa_4"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_5"), STRING("a_kusa_5"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_6"), STRING("a_kusa_6"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_7"), STRING("a_kusa_7"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_8"), STRING("a_kusa_8"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_9"), STRING("a_kusa_9"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_10"), STRING("a_kusa_10"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_11"), STRING("a_kusa_11"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_12"), STRING("a_kusa_12"), 0)
    CALL(evt_cloud_ent, 0, STRING("kusa_13"), STRING("a_kusa_13"), 0)
    RETURN
EVT_END()


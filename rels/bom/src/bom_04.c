#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "bom.h"
#include "data_tables.h"

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);

static TreeAccess tree_tbl[4];
static EvtWord evt_tree[7];
static BeroEntry bero_entry_data[3];
EvtWord bom_04_init_evt[47];

static TreeAccess tree_tbl[] = {
    {3, "A_ki1", "S_ki_1", "S_ha_1", 0, NULL},
    {3, "A_ki2", "S_ki_2", "S_ha_2", 0, NULL},
    {3, "A_ki3", "S_ki_3", "S_ha_3", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

static EVT_BEGIN(evt_tree)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero4", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "bom_03", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero4", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "bom_01", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(bom_04_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_bom_snowfall))
    CALL_CHILD(PTR(evt_tree))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_BOM1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG7_BOM1"))
    CALL(evt_item_entry, STRING("item00"), 125, 450, 5, 110, 16, GSWF(5663), 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_badge_gat"), 150, 0, -75, 279, 0, GSWF(3887), 1)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_bom04_hei2"), 2)
    RETURN
EVT_END()


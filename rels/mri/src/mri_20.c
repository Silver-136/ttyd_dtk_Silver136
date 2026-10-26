#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"
#include "evt/evt_map.h"
#include "evt/evt_npc.h"
#include "mri_puni.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


BeroEntry bero_black_shadow[1];
static BeroEntry bero_entry_data[5];
static EvtWord evt_ueki_08[20];
static KusaAccess ueki_access_data[7];
EvtWord mri_20_init_evt[123];

BeroEntry bero_black_shadow[] = {
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_04", // nextMapName
        "", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};

static BeroEntry bero_entry_data[] = {
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "mri_03", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan2", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_02", // nextMapName
        "dokan3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan4", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_ueki_08)
    CALL(evt_item_delete_check, STRING("item_ueki_08"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_08"), ITEM_POWER_PUNCH, 250, -320, 130, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF(2875), 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki02", "S_ueki02", NULL},
    {1, "A_ueki03", "S_ueki03", NULL},
    {1, "A_ueki04", "S_ueki04", NULL},
    {1, "A_ueki05", "S_ueki05", NULL},
    {1, "A_ueki08", "S_ueki08", evt_ueki_08},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(mri_20_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 20, -89, -319, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 45, -319, 92, -87, -91, -317, -321)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI4"))
    END_SWITCH
    IF_INT_GREATER(GSW_STORY_PROGRESS, 113)
        CALL(evt_npc_check_delete, STRING("軍団ザコ２"))
        CALL(evt_npc_check_delete, STRING("バリアーン"))
    END_IF
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 90)
        SET_VALUE(LW(0), STRING("dokan1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan1"), PTR(bero_black_shadow))
        SET_VALUE(LW(0), STRING("dokan1"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    RETURN
EVT_END()


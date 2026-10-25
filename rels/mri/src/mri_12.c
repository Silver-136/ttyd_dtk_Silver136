#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "mri.h"
#include "data_tables.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_npc.h"
#include "mri_puni.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"


static BeroEntry bero_entry_data[6];
static EvtWord punipuni_ball00_init[32];
static EvtWord punipuni_ball00[51];
static EvtWord evt_f1_ueki_01[20];
static KusaAccess ueki_access_data[8];
static EvtWord breakyuka_init[11];
static EvtWord breakyuka_main[9];
EvtWord mri_12_init_evt[140];

static BeroEntry bero_entry_data[] = {
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
        "mri_11", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_17", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan2", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan1", // nextBeroName
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
        NULL, // exitEvent
        "mri_18", // nextMapName
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(punipuni_ball00_init)
    CALL_CHILD(PTR(mri_punipuni_setup))
    IF_INT_EQUAL(GSWF(2859), 1)
        CALL(evt_item_set_position, LW(10), 5, 30, -95)
        CALL(evt_target_flag_onoff, LW(11), 0, 3)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sui_hikari"), 1)
    END_IF
    CALL(evt_target_set_position, LW(11), 5, 0, -65)
    RETURN
EVT_END()

static EVT_BEGIN(punipuni_ball00)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF(2859), 0)
        CALL_CHILD(PTR(mri_punipuni_itemselect))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(100)
        END_IF
        SET_VALUE(GSWF(2859), 1)
        CALL(evt_item_set_position, LW(10), 5, 30, -95)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_sui_hikari"), 1)
        CALL_CHILD(PTR(mri_punipuni_itemset))
        ELSE
        SET_VALUE(GSWF(2859), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_sui_hikari"), 1)
        CALL_CHILD(PTR(mri_punipuni_itemremove))
    END_IF
    LABEL(100)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_f1_ueki_01)
    CALL(evt_item_delete_check, STRING("item_ueki_01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_01"), 121, -190, -309, 65, 4, GSWF(2860), 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_f1_ueki01", "S_f1_ueki01", evt_f1_ueki_01},
    {1, "A_f1_ueki02", "S_f1_ueki02", NULL},
    {1, "A_f2_ueki01", "S_f2_ueki01", NULL},
    {1, "A_f2_ueki02", "S_f2_ueki02", NULL},
    {1, "A_f2_ueki03", "S_f2_ueki03", NULL},
    {1, "A_f2_ueki04", "S_f2_ueki04", NULL},
    {1, "A_f2_ueki05", "S_f2_ueki05", NULL},
    {4, NULL, NULL, NULL},
};

static EVT_BEGIN(breakyuka_init)
    IF_INT_EQUAL(GSWF(2858), 0)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak_01"), 1, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(breakyuka_main)
    CALL(evt_hitobj_onoff, STRING("A_f2_rak_01"), 1, 1)
    CALL(evt_puni_all_fall_check)
    RETURN
EVT_END()

EVT_BEGIN(mri_12_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 12, 0, -250, -310, -1000, 3, 0)
    CALL(evt_puni_set_hamari, 0, 0, 0, 6, -6, -248, -312)
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
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("sui_hikari"), 1, 0)
    SPAWN_EVENT(PTR(breakyuka_init))
    CALL(evt_mobj_breaking_floor, STRING("mobj_bf00"), -45, 0, -64, 1, PTR(breakyuka_main), GSWF(2858))
    SET_VALUE(LW(10), STRING("punipuni"))
    SET_VALUE(LW(11), 0)
    SPAWN_EVENT(PTR(punipuni_ball00_init))
    CALL(evt_run_case_evt, 9, 1, STRING("A_dai_mae"), 0, PTR(punipuni_ball00), 0)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_item_entry, STRING("item_00"), 125, -175, -310, -85, 16, GSWF(5615), 0)
    RETURN
EVT_END()


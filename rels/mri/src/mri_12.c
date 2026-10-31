// mri_12 (巨木の中, The Great Tree): a junction room with planters and a Puni-ball pad, joining mri_11, mri_17
// and mri_18.

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
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static BeroEntry bero_entry_data[6];
static EvtWord punipuni_ball00_init[32];
static EvtWord punipuni_ball00[51];
static EvtWord evt_f1_ueki_01[20];
static KusaAccess ueki_access_data[8];
static EvtWord breakyuka_init[11];
static EvtWord breakyuka_main[9];
EvtWord mri_12_init_evt[140];

/**
 * Exits of mri_12: east road e_bero to mri_11 (w_bero); east road e_bero_2 to mri_17 (w_bero); dokan1
 * (scripted pipe); dokan2 (scripted pipe); pipe dokan3 to mri_18 (dokan1).
 */
static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_W, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Init script of the orb pedestal in this room: runs the shared Puni-switch setup;
 * if the orb is already on the pedestal it is shown there and its target is on,
 * otherwise the beam of light is hidden. The target is placed at the pedestal.
 */
static EVT_BEGIN(punipuni_ball00_init)
    CALL_CHILD(PTR(mri_punipuni_setup))
    IF_INT_EQUAL(GSWF_MRI_12_PUNI_ORB_PLACED, 1)
        CALL(evt_item_set_position, LW(10), 5, 30, -95)
        CALL(evt_target_flag_onoff, LW(11), 0, 3)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_target_set_position, LW(11), 5, 0, -65)
    RETURN
EVT_END()

/**
 * Hit/talk script of the orb pedestal: if empty, Mario picks an item to put on it (cancel ends the script),
 * the orb appears and the light beam is shown; if full, Mario takes the orb back and the beam is hidden.
 * Mario is frozen during this.
 */
static EVT_BEGIN(punipuni_ball00)
    CALL(evt_mario_key_onoff, 0)
    IF_INT_EQUAL(GSWF_MRI_12_PUNI_ORB_PLACED, 0)
        CALL_CHILD(PTR(mri_punipuni_itemselect))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(100)
        END_IF
        SET_VALUE(GSWF_MRI_12_PUNI_ORB_PLACED, 1)
        CALL(evt_item_set_position, LW(10), 5, 30, -95)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemset))
        ELSE
        SET_VALUE(GSWF_MRI_12_PUNI_ORB_PLACED, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_sui_hikari"), MAP_OBJ_FLAG_HIDDEN)
        CALL_CHILD(PTR(mri_punipuni_itemremove))
    END_IF
    LABEL(100)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Planter hit script 1: the first time it is hit it launches a coin out of the planter (remembered with a flag). */
static EVT_BEGIN(evt_f1_ueki_01)
    CALL(evt_item_delete_check, STRING("item_ueki_01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_01"), ITEM_COIN, -190, -309, 65, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_MRI_12_COIN_PLANT, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Plants Mario can search in mri_12: A_f1_ueki01 (runs evt_f1_ueki_01 when searched), A_f1_ueki02,
 * A_f2_ueki01, A_f2_ueki02, A_f2_ueki03, A_f2_ueki04, A_f2_ueki05.
 */
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

/**
 * Init script of the breaking floor: until the floor has fallen (GSWF_MRI12_FLOOR_BROKEN is 0) the lower floor
 * piece (A_f2_rak_01) has its collision off.
 */
static EVT_BEGIN(breakyuka_init)
    IF_INT_EQUAL(GSWF_MRI_12_BREAKING_FLOOR_BROKEN, 0)
        CALL(evt_hitobj_onoff, STRING("A_f2_rak_01"), 1, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Main script when the floor gives way under the Puni weight:
 * turns on the collision of the lower floor piece and makes every Puni that is standing over the hole fall.
 */
static EVT_BEGIN(breakyuka_main)
    CALL(evt_hitobj_onoff, STRING("A_f2_rak_01"), 1, 1)
    CALL(evt_puni_all_fall_check)
    RETURN
EVT_END()

/**
 * Map init script of mri_12, a junction room joining mri_11, mri_17 and mri_18:
 * sets up the Puni, planters, the breaking floor and the Puni Orb pedestal (punipuni), places a Star Piece,
 * and plays the room music and water animations.
 */
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
        CASE_BETWEEN(STG2_23_TIME_BOMB_ACTIVATED, STG2_23_01_X_NAUT_BATTLE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("awa_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("sui_hikari"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(PTR(breakyuka_init))
    CALL(evt_mobj_breaking_floor, STRING("mobj_bf00"), -45, 0, -64, 1, PTR(breakyuka_main), GSWF_MRI_12_BREAKING_FLOOR_BROKEN)
    SET_VALUE(LW(10), STRING("punipuni"))
    SET_VALUE(LW(11), 0)
    SPAWN_EVENT(PTR(punipuni_ball00_init))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_dai_mae"), 0, PTR(punipuni_ball00), 0)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, -175, -310, -85, ITEM_TYPE_STILL, GSWF_STARPIECE_48, 0)
    RETURN
EVT_END()


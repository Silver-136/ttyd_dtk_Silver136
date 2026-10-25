#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "data_tables.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"

USER_FUNC(evt_mario_dispflag_onoff);

EvtWord evt_bom_snowfall[18];
static TreeAccess tree_tbl[4];
static EvtWord evt_tree[7];
static EvtWord first_evt_dokan[36];
static BeroEntry bero_entry_data[3];
static EvtWord first_evt[108];
EvtWord first_evt2[101];
EvtWord bom_00_init_evt[66];

EVT_BEGIN(evt_bom_snowfall)
    CALL(evt_eff64, 0, STRING("snowfall_n64"), 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

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

static EVT_BEGIN(first_evt_dokan)
    IF_INT_EQUAL(GSWF(3884), 1)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_party_dispflg_onoff, 0, 1, 1)
    SET_VALUE(GF(0), 1)
    WAIT_WHILE(GF(0))
    CALL(evt_mario_dispflag_onoff, 0, 1)
    CALL(evt_party_dispflg_onoff, 0, 1, 0)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "dokan", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        first_evt_dokan, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_07", // nextMapName
        "dokan__1", // nextBeroName
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
        "bom_03", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(first_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam_letter_box_onoff, 1, 0)
    CALL(evt_cam3d_evt_set, -176, 567, 533, -176, 471, -9, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, -176, 183, 533, -176, 87, -9, 6000, 11)
    WAIT_MS(6000)
    SET_VALUE(GF(0), 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_mov_pos2, -155, 31, FLOAT(60.0f))
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_party_move_pos2, 0, -188, 31, FLOAT(60.0f))
    CALL(evt_party_set_dir, 0, 90, 300)
    CALL(evt_cam3d_evt_set, -175, 90, 295, -175, 35, 0, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg7_bom_01"))
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSW_STORY_PROGRESS, 352)
    SET_VALUE(GSWF(3884), 1)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_snd_bgm_scope, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(first_evt2)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_cam3d_evt_set, -176, 567, 533, -176, 471, -9, 0, 11)
    CALL(evt_seq_wait, 2)
    CALL(evt_cam3d_evt_set, -176, 183, 533, -176, 87, -9, 6000, 11)
    WAIT_MS(6000)
    SET_VALUE(GF(0), 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_mov_pos2, -155, 31, FLOAT(60.0f))
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_party_move_pos2, 0, -188, 31, FLOAT(60.0f))
    CALL(evt_party_set_dir, 0, 90, 300)
    CALL(evt_cam3d_evt_set, -175, 90, 295, -175, 35, 0, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("stg7_bom_01_00"))
    CALL(evt_cam3d_evt_off, 500, 11)
    SET_VALUE(GSWF(3884), 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

EVT_BEGIN(bom_00_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_BOM1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG7_BOM1"))
    CALL_CHILD(PTR(evt_bom_snowfall))
    CALL_CHILD(PTR(evt_tree))
    CALL(evt_mobj_brick, STRING("mobj_badge_gat2"), 215, 65, -75, 255, 12, 0, GSWF(3891))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -300, 5, 0, STRING("S_gra02_dai"), 125, GSWF(5658))
    IF_INT_EQUAL(GSWF(3884), 0)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(349)
            SPAWN_EVENT(PTR(first_evt2))
            CASE_DEFAULT
            SPAWN_EVENT(PTR(first_evt))
        END_SWITCH
        ELSE
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 349)
            IF_INT_LESS(GSW_STORY_PROGRESS, 352)
                SET_VALUE(GSW_STORY_PROGRESS, 352)
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "kpa.h"
#include "evt/evt_npc.h"
#include "kpa_stage_data.h"
#include "evt/evt_bero.h"
#include "evt/evt_mario.h"
#include "evt/evt_seq.h"
#include "evt/evt_sub.h"
#include "evt/evt_cam.h"
#include "driver/seqdrv.h"


EvtWord kpa_act_stg1_mobj[151];
EvtWord kpa_act_stg1_item[31];
static EvtWord kpa_fall[41];
static EvtWord kpa_goal[38];
static NpcSetupInfo npcEnt[1];
EvtWord kpa_02_init_evt[36];

EvtWord kpa_act_stg1_mobj[] = {
    KPA_OBJ_BRICK_2(-1225.0f, 125.0f), KPA_OBJ_BRICK_2(-1100.0f, 125.0f), KPA_OBJ_OJAMA_BLK(-1050.0f, 125.0f),
    KPA_OBJ_BRICK_2(-1000.0f, 125.0f), KPA_OBJ_BRICK_2(-1000.0f, 275.0f), KPA_OBJ_BRICK_2(-950.0f, 125.0f),
    KPA_OBJ_BRICK_2(-900.0f, 125.0f), KPA_OBJ_DOKAN_0(-750.0f, 0.0f),
    KPA_OBJ_HIDDEN_BLK(-612.5f, 25.0f, ITEM_COIN), KPA_OBJ_DOKAN_1(-575.0f, 0.0f),
    KPA_OBJ_OJAMA_BLK(-375.0f, 125.0f), KPA_OBJ_BRICK_2(0.0f, 25.0f), KPA_OBJ_OJAMA_BLK(50.0f, 25.0f),
    KPA_OBJ_HIDDEN_BLK(212.5f, 25.0f, ITEM_COIN), KPA_OBJ_DOKAN_1(250.0f, 0.0f),
    KPA_OBJ_BRICK_2(-50.0f, 200.0f), KPA_OBJ_BRICK_2(0.0f, 200.0f), KPA_OBJ_BRICK_2(50.0f, 200.0f),
    KPA_OBJ_BRICK_2(100.0f, 200.0f), KPA_OBJ_OJAMA_BLK(500.0f, 225.0f), KPA_OBJ_DOKAN_0(575.0f, 0.0f),
    KPA_OBJ_DOKAN_0(750.0f, 0.0f), KPA_OBJ_BRICK_2(800.0f, -50.0f), KPA_OBJ_BRICK_2(800.0f, 0.0f),
    KPA_OBJ_BRICK_2(850.0f, -50.0f), KPA_OBJ_BRICK_2(850.0f, 0.0f), KPA_OBJ_BRICK_2(850.0f, 50.0f),
    KPA_OBJ_BRICK_2(900.0f, -50.0f), KPA_OBJ_BRICK_2(900.0f, 0.0f), KPA_OBJ_BRICK_2(900.0f, 50.0f),
    KPA_OBJ_BRICK_2(900.0f, 100.0f), KPA_OBJ_BRICK_2(950.0f, -50.0f), KPA_OBJ_BRICK_2(950.0f, 0.0f),
    KPA_OBJ_BRICK_2(950.0f, 50.0f), KPA_OBJ_BRICK_2(950.0f, 100.0f), KPA_OBJ_BRICK_2(950.0f, 150.0f),
    KPA_OBJ_BRICK_2(1000.0f, -50.0f), KPA_OBJ_BRICK_2(1000.0f, 0.0f), KPA_OBJ_BRICK_2(1000.0f, 50.0f),
    KPA_OBJ_BRICK_2(1000.0f, 100.0f), KPA_OBJ_BRICK_2(1000.0f, 150.0f), KPA_OBJ_BRICK_2(1000.0f, 200.0f),
    KPA_OBJ_BRICK_2(1050.0f, -50.0f), KPA_OBJ_BRICK_2(1050.0f, 0.0f), KPA_OBJ_BRICK_2(1050.0f, 50.0f),
    KPA_OBJ_BRICK_2(1050.0f, 100.0f), KPA_OBJ_BRICK_2(1050.0f, 150.0f), KPA_OBJ_BRICK_2(1050.0f, 200.0f),
    KPA_OBJ_POLE(1237.5f, 0.0f, kpa_goal), KPA_OBJ_END,
};

EvtWord kpa_act_stg1_item[] = {
    KPA_ITEM(-687.5f, 112.5f), KPA_ITEM(-650.0f, 112.5f), KPA_ITEM(-500.0f, 137.5f),
    KPA_ITEM(-462.5f, 137.5f), KPA_ITEM(-225.0f, 62.5f), KPA_ITEM(-187.5f, 62.5f), KPA_ITEM(150.0f, 125.0f),
    KPA_ITEM(187.5f, 125.0f), KPA_ITEM(325.0f, 162.5f), KPA_ITEM(362.5f, 162.5f), KPA_ITEM_END,
};

static EVT_BEGIN(kpa_fall)
    SET_VALUE(LF(0), 0)
    LOOP(0)
        CALL(evt_koopa_chk_dead, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LF(0), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(1), -20)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_EQUAL(LF(0), 0)
        CALL_CHILD(PTR(kpa_fall_sub))
        ELSE
        CALL_CHILD(PTR(kpa_dead))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(kpa_goal)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 116)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 4096)
        CALL(evt_cam_letter_box_disable, 0)
        CALL(evt_seq_set_seq, SEQ_GAME_OVER, 0, 0)
        ELSE
        BEGIN_INLINE_EVENT
            WAIT_MS(300)
            CALL(evt_cam_letter_box_disable, 0)
        END_INLINE_EVENT
        CALL(evt_bero_mapchange, STRING("nok_00"), STRING("w_bero"))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {0},
};

EVT_BEGIN(kpa_02_init_evt)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(kpa_init))
    CALL(evt_mario_set_pos, -1450, 0, -5)
    SET_VALUE(LW(0), PTR(kpa_act_stg1_mobj))
    CALL_CHILD(PTR(kpa_read_mobj))
    SET_VALUE(LW(0), PTR(kpa_act_stg1_item))
    CALL_CHILD(PTR(kpa_read_item))
    SPAWN_EVENT(PTR(kpa_fall))
    CALL(kpa_status_init, 1, GSW(1221), GSW(1222))
    SET_VALUE(LSW(1), 0)
    SPAWN_EVENT(PTR(kpa_stage_start))
    SPAWN_EVENT(PTR(kpa_time))
    RETURN
EVT_END()


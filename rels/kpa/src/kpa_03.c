#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "kpa.h"
#include "evt/evt_npc.h"
#include "kpa_stage_data.h"
#include "evt/evt_bero.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_seq.h"
#include "driver/seqdrv.h"


static EvtWord kpa_act_stg2_mobj[209];
EvtWord kpa_act_stg2_item[85];
static EvtWord kpa_fall[41];
EvtWord kairyu_evt[38];
static EvtWord jumpstand_evt[60];
static NpcSetupInfo npcEnt[1];
EvtWord kpa_03_init_evt[44];

static EvtWord kpa_act_stg2_mobj[] = {
    KPA_OBJ_SANGO(-1275.0f, 0.0f), KPA_OBJ_BRICK_2(-1100.0f, 200.0f), KPA_OBJ_SANGO(-1100.0f, 250.0f),
    KPA_OBJ_BRICK_2(-925.0f, 0.0f), KPA_OBJ_BRICK_2(-925.0f, 50.0f), KPA_OBJ_BRICK_2(-925.0f, 100.0f),
    KPA_OBJ_OJAMA_BLK(-775.0f, 100.0f), KPA_OBJ_BRICK_2(-725.0f, 0.0f), KPA_OBJ_BRICK_2(-725.0f, 50.0f),
    KPA_OBJ_BRICK_2(-725.0f, 100.0f), KPA_OBJ_BRICK_2(-475.0f, 25.0f), KPA_OBJ_BRICK_2(-425.0f, 25.0f),
    KPA_OBJ_OJAMA_BLK(-375.0f, 25.0f), KPA_OBJ_BRICK_2(-375.0f, 250.0f), KPA_OBJ_SANGO(-300.0f, 0.0f),
    KPA_OBJ_BRICK_2(-225.0f, 0.0f), KPA_OBJ_BRICK_2(-225.0f, 50.0f), KPA_OBJ_BRICK_2(-225.0f, 250.0f),
    KPA_OBJ_BRICK_2(-225.0f, 300.0f), KPA_OBJ_BRICK_2(-225.0f, 350.0f), KPA_OBJ_BRICK_2(-75.0f, 0.0f),
    KPA_OBJ_BRICK_2(-75.0f, 50.0f), KPA_OBJ_BRICK_2(-75.0f, 250.0f), KPA_OBJ_BRICK_2(-75.0f, 300.0f),
    KPA_OBJ_BRICK_2(-75.0f, 350.0f), KPA_OBJ_SANGO(50.0f, 0.0f), KPA_OBJ_BRICK_2(125.0f, 25.0f),
    KPA_OBJ_OJAMA_BLK(175.0f, 25.0f), KPA_OBJ_BRICK_2(225.0f, 25.0f), KPA_OBJ_OJAMA_BLK(275.0f, 25.0f),
    KPA_OBJ_SANGO(350.0f, 0.0f), KPA_OBJ_BRICK_2(500.0f, 25.0f), KPA_OBJ_BRICK_2(550.0f, 75.0f),
    KPA_OBJ_BRICK_2(600.0f, 75.0f), KPA_OBJ_BRICK_2(650.0f, 75.0f), KPA_OBJ_BRICK_2(700.0f, 75.0f),
    KPA_OBJ_BRICK_2(750.0f, 75.0f), KPA_OBJ_BRICK_2(800.0f, 75.0f), KPA_OBJ_BRICK_2(850.0f, 75.0f),
    KPA_OBJ_BRICK_2(900.0f, 25.0f), KPA_OBJ_BRICK_2(500.0f, 300.0f), KPA_OBJ_BRICK_2(500.0f, 350.0f),
    KPA_OBJ_BRICK_2(550.0f, 250.0f), KPA_OBJ_BRICK_2(600.0f, 250.0f), KPA_OBJ_BRICK_2(650.0f, 250.0f),
    KPA_OBJ_BRICK_2(700.0f, 250.0f), KPA_OBJ_BRICK_2(750.0f, 250.0f), KPA_OBJ_BRICK_2(800.0f, 250.0f),
    KPA_OBJ_BRICK_2(850.0f, 250.0f), KPA_OBJ_BRICK_2(900.0f, 300.0f), KPA_OBJ_BRICK_2(900.0f, 350.0f),
    KPA_OBJ_SANGO(975.0f, 0.0f), KPA_OBJ_JUMPSTAND_RED(1150.0f, 0.0f, (EvtWord)jumpstand_evt),
    KPA_OBJ_BRICK_2(1325.0f, 0.0f), KPA_OBJ_BRICK_2(1375.0f, 0.0f), KPA_OBJ_BRICK_2(1375.0f, 50.0f),
    KPA_OBJ_BRICK_2(1425.0f, 0.0f), KPA_OBJ_BRICK_2(1425.0f, 50.0f), KPA_OBJ_BRICK_2(1425.0f, 100.0f),
    KPA_OBJ_BRICK_2(1425.0f, 250.0f), KPA_OBJ_BRICK_2(1425.0f, 300.0f), KPA_OBJ_BRICK_2(1425.0f, 350.0f),
    KPA_OBJ_BRICK_2(1475.0f, 0.0f), KPA_OBJ_BRICK_2(1475.0f, 50.0f), KPA_OBJ_BRICK_2(1475.0f, 100.0f),
    KPA_OBJ_DOKAN_2(1500.0f, 150.0f), KPA_OBJ_BRICK_2(1475.0f, 250.0f), KPA_OBJ_BRICK_2(1475.0f, 300.0f),
    KPA_OBJ_BRICK_2(1475.0f, 350.0f), KPA_OBJ_END,
};

EvtWord kpa_act_stg2_item[] = {
    KPA_ITEM(-1050.0f, 212.5f), KPA_ITEM(-1000.0f, 212.5f), KPA_ITEM(-875.0f, 12.5f),
    KPA_ITEM(-875.0f, 62.5f), KPA_ITEM(-825.0f, 12.5f), KPA_ITEM(-825.0f, 62.5f), KPA_ITEM(-775.0f, 12.5f),
    KPA_ITEM(-775.0f, 62.5f), KPA_ITEM(125.0f, 0.0f), KPA_ITEM(175.0f, 0.0f), KPA_ITEM(225.0f, 0.0f),
    KPA_ITEM(275.0f, 0.0f), KPA_ITEM(600.0f, 25.0f), KPA_ITEM(650.0f, 25.0f), KPA_ITEM(700.0f, 25.0f),
    KPA_ITEM(750.0f, 25.0f), KPA_ITEM(800.0f, 25.0f), KPA_ITEM(625.0f, 150.0f), KPA_ITEM(625.0f, 200.0f),
    KPA_ITEM(675.0f, 150.0f), KPA_ITEM(675.0f, 200.0f), KPA_ITEM(725.0f, 150.0f), KPA_ITEM(725.0f, 200.0f),
    KPA_ITEM(775.0f, 150.0f), KPA_ITEM(775.0f, 200.0f), KPA_ITEM(1150.0f, 312.5f), KPA_ITEM(1150.0f, 350.0f),
    KPA_ITEM(1150.0f, 387.5f), KPA_ITEM_END,
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

EVT_BEGIN(kairyu_evt)
    CALL(evt_seq_wait, SEQ_GAME)
    LOOP(0)
        IF_INT_NOT_EQUAL(LSWF(3), 0)
            BREAK_LOOP
        END_IF
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), -175)
            IF_INT_LESS_OR_EQUAL(LW(0), -100)
                IF_INT_LESS(LW(1), 400)
                    CALL(evt_mario_set_hosei_xyz, 0, FLOAT(-0.7998047f), 0)
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(jumpstand_evt)
    DELETE_EVENT(GW(15))
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_koopa_get_level, GSW_KPA_BOWSER_LEVEL)
        CALL(evt_bero_mapchange, STRING("kpa_04"), STRING("kpa_next_map"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(1), 400)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_cam_off)
    END_INLINE_EVENT
    CALL(evt_mobj_get_position, STRING("me"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(1))
    ADD(LW(3), 700)
    CALL(evt_mario_jump_jumpstand, LW(0), LW(1), LW(2), LW(0), LW(3), LW(2), LW(0), LW(3), LW(2), 4000, 3)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {0},
};

EVT_BEGIN(kpa_03_init_evt)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(kpa_init))
    CALL(evt_mario_set_pos, -1400, 0, -5)
    SET_VALUE(LW(0), PTR(kpa_act_stg2_mobj))
    CALL_CHILD(PTR(kpa_read_mobj))
    SET_VALUE(LW(0), PTR(kpa_act_stg2_item))
    CALL_CHILD(PTR(kpa_read_item))
    CALL(evt_map_playanim, STRING("anm_umi"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(PTR(kpa_fall))
    CALL(kpa_status_init, 2, GSW_KPA_TRY_COUNT, GSW_KPA_TIME_LEFT)
    SET_VALUE(LSW(1), 1)
    SPAWN_EVENT(PTR(kpa_stage_start))
    SPAWN_EVENT(PTR(kairyu_evt))
    SPAWN_EVENT_GET_ID(PTR(kpa_time), GW(15))
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "kpa.h"
#include "evt/evt_npc.h"
#include "kpa_stage_data.h"
#include "koopa.h"
#include "evt/evt_bero.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"

USER_FUNC(evt_mario_get_name_hitobj_ride);
static USER_FUNC(kpa_get_level);

static EvtWord kpa_act_stg3_mobj[118];
EvtWord kpa_act_stg3_item[79];
static EvtWord kpa_fall[92];
static EvtWord jumpstand_evt[60];
static NpcSetupInfo npcEnt[1];
EvtWord kpa_05_init_evt[44];

static EvtWord kpa_act_stg3_mobj[] = {
    KPA_OBJ_FIREBAR_DODAI_0(-1062.5f, 100.0f, 3, -229998976),
    KPA_OBJ_FIREBAR_DODAI_0(-687.5f, 100.0f, 2, -229997952),
    KPA_OBJ_FIREBAR_DODAI_1(-512.5f, 225.0f, 3, -229998976), KPA_OBJ_OJAMA_BLK(-325.0f, 200.0f),
    KPA_OBJ_BRICK_3(-225.0f, 25.0f), KPA_OBJ_BRICK_3(-175.0f, 25.0f), KPA_OBJ_BRICK_3(-125.0f, 25.0f),
    KPA_OBJ_BRICK_3(-75.0f, 25.0f), KPA_OBJ_BRICK_3(-25.0f, 25.0f), KPA_OBJ_BRICK_3(25.0f, 25.0f),
    KPA_OBJ_BRICK_3(75.0f, 25.0f), KPA_OBJ_BRICK_3(125.0f, 75.0f), KPA_OBJ_STONE_BLK(250.0f, 25.0f),
    KPA_OBJ_STONE_BLK(300.0f, 25.0f), KPA_OBJ_STONE_BLK(300.0f, 75.0f), KPA_OBJ_STONE_BLK(350.0f, 75.0f),
    KPA_OBJ_OJAMA_BLK(400.0f, 0.0f), KPA_OBJ_STONE_BLK(400.0f, 75.0f),
    KPA_OBJ_FIREBAR_DODAI_1(387.5f, 125.0f, 6, -229999488), KPA_OBJ_STONE_BLK(450.0f, 75.0f),
    KPA_OBJ_STONE_BLK(500.0f, 25.0f), KPA_OBJ_STONE_BLK(500.0f, 75.0f), KPA_OBJ_STONE_BLK(550.0f, 25.0f),
    KPA_OBJ_BRICK_3(725.0f, 0.0f), KPA_OBJ_BRICK_3(775.0f, 0.0f), KPA_OBJ_OJAMA_BLK(700.0f, 175.0f),
    KPA_OBJ_BRICK_3(950.0f, 25.0f), KPA_OBJ_BRICK_3(1000.0f, 25.0f), KPA_OBJ_BRICK_3(1050.0f, 50.0f),
    KPA_OBJ_BRICK_3(1050.0f, 100.0f), KPA_OBJ_JUMPSTAND_RED(1125.0f, 0.0f, (EvtWord)jumpstand_evt),
    KPA_OBJ_BRICK_3(1200.0f, 50.0f), KPA_OBJ_BRICK_3(1200.0f, 100.0f), KPA_OBJ_BRICK_3(1250.0f, 25.0f),
    KPA_OBJ_BRICK_3(1300.0f, 25.0f), KPA_OBJ_DOKAN_2(1475.0f, 200.0f), KPA_OBJ_END,
};

EvtWord kpa_act_stg3_item[] = {
    KPA_ITEM(-175.0f, 175.0f), KPA_ITEM(-125.0f, 175.0f), KPA_ITEM(-25.0f, 175.0f), KPA_ITEM(25.0f, 175.0f),
    KPA_ITEM(250.0f, 0.0f), KPA_ITEM(300.0f, 0.0f), KPA_ITEM(350.0f, 0.0f), KPA_ITEM(350.0f, 25.0f),
    KPA_ITEM(350.0f, 50.0f), KPA_ITEM(400.0f, 50.0f), KPA_ITEM(450.0f, 50.0f), KPA_ITEM(450.0f, 25.0f),
    KPA_ITEM(450.0f, 0.0f), KPA_ITEM(500.0f, 0.0f), KPA_ITEM(550.0f, 0.0f), KPA_ITEM(600.0f, 0.0f),
    KPA_ITEM(700.0f, 87.5f), KPA_ITEM(750.0f, 87.5f), KPA_ITEM(800.0f, 87.5f), KPA_ITEM(1125.0f, 87.5f),
    KPA_ITEM(1125.0f, 125.0f), KPA_ITEM(1125.0f, 162.5f), KPA_ITEM(1375.0f, 12.5f), KPA_ITEM(1375.0f, 50.0f),
    KPA_ITEM(1425.0f, 12.5f), KPA_ITEM(1425.0f, 50.0f), KPA_ITEM_END,
};

static EVT_BEGIN(kpa_fall)
    SET_VALUE(LF(0), 0)
    LOOP(0)
        CALL(evt_koopa_chk_dead, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LF(0), 1)
            BREAK_LOOP
        END_IF
        CALL(evt_mario_get_name_hitobj_ride, LW(0))
        IF_STR_EQUAL(LW(0), STRING("a_magu"))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD_FLOAT(LW(1), FLOAT(3.0f))
        ADD_FLOAT(LW(2), FLOAT(5.0f))
        CALL(kpa_get_level, LW(3))
        SWITCH(LW(3))
            CASE_EQUAL(0)
            SET_FLOAT_VALUE(LW(3), FLOAT(0.7998047f))
            CASE_EQUAL(1)
            SET_FLOAT_VALUE(LW(3), FLOAT(1.6992188f))
            CASE_EQUAL(2)
            SET_FLOAT_VALUE(LW(3), FLOAT(3.0f))
            CASE_EQUAL(3)
            SET_FLOAT_VALUE(LW(3), FLOAT(4.5f))
        END_SWITCH
        CALL(evt_eff, STRING(""), STRING("splash"), 2, LW(0), LW(1), LW(2), LW(3), 0, 0, 0, 0, 0, 0, 0)
        CALL_CHILD(PTR(kpa_fall_sub))
        ELSE
        CALL_CHILD(PTR(kpa_dead))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(jumpstand_evt)
    DELETE_EVENT(GW(15))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_koopa_get_level, GSW_KPA_BOWSER_LEVEL)
        CALL(evt_bero_mapchange, STRING("kpa_06"), STRING("kpa_next_map"))
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
    ADD(LW(3), 800)
    CALL(evt_mario_jump_jumpstand, LW(0), LW(1), LW(2), LW(0), LW(3), LW(2), LW(0), LW(3), LW(2), 4000, 3)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {0},
};

EVT_BEGIN(kpa_05_init_evt)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(kpa_init))
    CALL(evt_mario_set_pos, -1500, 150, -5)
    CALL(evt_cam_road_reset)
    SET_VALUE(LW(0), PTR(kpa_act_stg3_mobj))
    CALL_CHILD(PTR(kpa_read_mobj))
    SET_VALUE(LW(0), PTR(kpa_act_stg3_item))
    CALL_CHILD(PTR(kpa_read_item))
    CALL(evt_map_playanim, STRING("anm_magu"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(PTR(kpa_fall))
    CALL(kpa_status_init, 3, GSW_KPA_TRY_COUNT, GSW_KPA_TIME_LEFT)
    SET_VALUE(LSW(1), 2)
    SPAWN_EVENT(PTR(kpa_stage_start))
    SPAWN_EVENT_GET_ID(PTR(kpa_time), GW(15))
    RETURN
EVT_END()


static USER_FUNC(kpa_get_level) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], kpaGetLevel());
    return EVT_RETURN_DONE;
}

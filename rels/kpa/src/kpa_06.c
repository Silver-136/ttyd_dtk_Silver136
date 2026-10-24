#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "kpa.h"
#include "evt/evt_npc.h"
#include "kpa_stage_data.h"


USER_FUNC(evt_bero_mapchange);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_koopa_chk_dead);
USER_FUNC(evt_koopa_set_level);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_npc_setup);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_sub_get_system_flag);
USER_FUNC(kpa_status_init);

f32 fd_kpa_06_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_kpa_06_neg_one_382;
f32 fd_kpa_06___local_PI_383;
f32 fd_kpa_06___local_3PIO2_385;
f32 fd_kpa_06_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_kpa_06___local_PIO2_384;
f32 fd_kpa_06_one_381;
f32 fd_kpa_06_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_kpa_06_neg_one_365;
f32 fd_kpa_06___local_PI_366;
f32 fd_kpa_06___local_3PIO2_368;
f32 fd_kpa_06_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_kpa_06___local_PIO2_367;
f32 fd_kpa_06_one_364;
static EvtWord kpa_act_stg3_mobj[68];
static EvtWord kpa_fall[41];
static EvtWord kpa_goal[22];
static NpcSetupInfo npcEnt[1];
EvtWord kpa_06_init_evt[56];

f32 fd_kpa_06_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_kpa_06_neg_one_382 = -1.0f;

f32 fd_kpa_06___local_PI_383 = 3.1415927f;

f32 fd_kpa_06___local_3PIO2_385 = 4.712389f;

f32 fd_kpa_06_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_kpa_06___local_PIO2_384 = 1.5707964f;

f32 fd_kpa_06_one_381 = 1.0f;

f32 fd_kpa_06_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_kpa_06_neg_one_365 = -1.0f;

f32 fd_kpa_06___local_PI_366 = 3.1415927f;

f32 fd_kpa_06___local_3PIO2_368 = 4.712389f;

f32 fd_kpa_06_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_kpa_06___local_PIO2_367 = 1.5707964f;

f32 fd_kpa_06_one_364 = 1.0f;

static EvtWord kpa_act_stg3_mobj[] = {
    KPA_OBJ_DOKAN_0(-225.0f, 0.0f), KPA_OBJ_BRICK_2(-175.0f, 0.0f), KPA_OBJ_BRICK_2(-125.0f, 0.0f),
    KPA_OBJ_BRICK_2(-125.0f, 50.0f), KPA_OBJ_BRICK_2(-75.0f, 0.0f), KPA_OBJ_BRICK_2(-75.0f, 50.0f),
    KPA_OBJ_BRICK_2(-75.0f, 100.0f), KPA_OBJ_BRICK_2(-25.0f, 0.0f), KPA_OBJ_BRICK_2(-25.0f, 50.0f),
    KPA_OBJ_BRICK_2(-25.0f, 100.0f), KPA_OBJ_BRICK_2(-25.0f, 150.0f), KPA_OBJ_BRICK_2(25.0f, 0.0f),
    KPA_OBJ_BRICK_2(25.0f, 50.0f), KPA_OBJ_BRICK_2(25.0f, 100.0f), KPA_OBJ_BRICK_2(25.0f, 150.0f),
    KPA_OBJ_BRICK_2(25.0f, 200.0f), KPA_OBJ_BRICK_2(75.0f, 0.0f), KPA_OBJ_BRICK_2(75.0f, 50.0f),
    KPA_OBJ_BRICK_2(75.0f, 100.0f), KPA_OBJ_BRICK_2(75.0f, 150.0f), KPA_OBJ_BRICK_2(75.0f, 200.0f),
    KPA_OBJ_POLE(262.5f, 0.0f, kpa_goal), KPA_OBJ_END,
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
    SET_VALUE(GSW(0), 343)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_cam_letter_box_disable, 0)
    END_INLINE_EVENT
    CALL(evt_bero_mapchange, STRING("kpa_07"), STRING(""))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {0},
};

EVT_BEGIN(kpa_06_init_evt)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(kpa_init))
    CALL(evt_mario_set_pos, -400, 500, -5)
    CALL(evt_mario_set_dir, 90, 0, 0)
    SET_VALUE(LW(0), PTR(kpa_act_stg3_mobj))
    CALL_CHILD(PTR(kpa_read_mobj))
    SPAWN_EVENT(PTR(kpa_fall))
    CALL(kpa_status_init, 3, GSW(1221), GSW(1222))
    SET_VALUE(LSW(1), 0)
    SET_VALUE(LSWF(2), 1)
    SPAWN_EVENT(PTR(kpa_bgm_change))
    CALL(evt_snd_envoff, 512)
    SPAWN_EVENT(PTR(kpa_time))
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    CALL(evt_koopa_set_level, GSW(1223))
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "driver/npcdrv.h"
#include "evt/evt_bero.h"
#include "npc_event.h"

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_map_set_tevcallback);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mario_wait_movable);
USER_FUNC(evt_npc_battle_start);
USER_FUNC(evt_npc_change_fbat_mode);
USER_FUNC(evt_npc_change_interrupt);
USER_FUNC(evt_npc_check);
USER_FUNC(evt_npc_check_escape_battle);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_battle_result);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_jump_position);
USER_FUNC(evt_npc_restart_regular_event);
USER_FUNC(evt_npc_set_battle_info);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_wait_battle_end);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon_3d);
static USER_FUNC(npc_dead);
static USER_FUNC(npc_set_me);
extern s32 hei_test_water_tev[];

f32 fd_hei_12_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_hei_12_neg_one_382;
f32 fd_hei_12___local_PI_383;
f32 fd_hei_12___local_3PIO2_385;
f32 fd_hei_12_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_hei_12___local_PIO2_384;
f32 fd_hei_12_one_381;
f32 fd_hei_12_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_hei_12_neg_one_365;
f32 fd_hei_12___local_PI_366;
f32 fd_hei_12___local_3PIO2_368;
f32 fd_hei_12_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_hei_12___local_PIO2_367;
f32 fd_hei_12_one_364;
static BeroEntry bero_entry_data[2];
static EvtWord evt_chorobon[136];
EvtWord hei_12_init_evt[97];

f32 fd_hei_12_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hei_12_neg_one_382 = -1.0f;

f32 fd_hei_12___local_PI_383 = 3.1415927f;

f32 fd_hei_12___local_3PIO2_385 = 4.712389f;

f32 fd_hei_12_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hei_12___local_PIO2_384 = 1.5707964f;

f32 fd_hei_12_one_381 = 1.0f;

f32 fd_hei_12_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hei_12_neg_one_365 = -1.0f;

f32 fd_hei_12___local_PI_366 = 3.1415927f;

f32 fd_hei_12___local_3PIO2_368 = 4.712389f;

f32 fd_hei_12_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hei_12___local_PIO2_367 = 1.5707964f;

f32 fd_hei_12_one_364 = 1.0f;

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
        "hei_11", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static USER_FUNC(npc_set_me) {
    // チョロボン
    event->ownerNPC = npcNameToPtr("\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93");
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(evt_chorobon)
    SET_VALUE(GSWF(1774), 1)
    CALL(evt_mario_wait_movable)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("チョロボン"), 8)
    CALL(evt_npc_flag_onoff, 1, STRING("チョロボン"), 131120)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("チョロボン"), LW(3), LW(4), LW(5))
    CALL(evt_npc_set_position, STRING("チョロボン"), LW(0), LW(4), LW(2))
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_CHORO_FALL1"), LW(0), LW(4), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position, STRING("チョロボン"), 0, 0, 0, 600, 0, FLOAT(30.0f), 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    WAIT_MS(500)
    CALL(evt_npc_change_interrupt, STRING("チョロボン"), 1, PTR(chorobon_move_event))
    CALL(evt_npc_restart_regular_event, STRING("チョロボン"))
    CALL(evt_npc_battle_start, STRING("チョロボン"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_npc_get_battle_result, LW(0))
    CALL(evt_npc_check_escape_battle, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(npc_set_me)
        CALL(npc_dead)
        CALL_CHILD(PTR(enemy_common_dead_event))
        ELSE
        BEGIN_INLINE_EVENT
            CALL(evt_npc_flag_onoff, 1, STRING("チョロボン"), 8)
            CALL(evt_npc_flag_onoff, 0, STRING("チョロボン"), 32)
        END_INLINE_EVENT
        CALL(evt_npc_change_fbat_mode, 1)
    END_IF
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(hei_12_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG1_TORIDE1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG1_HEI3"))
    CALL(evt_snd_set_rev_mode, 1)
    IF_INT_EQUAL(GSWF(1774), 0)
        CALL(evt_npc_change_interrupt, STRING("チョロボン"), 1, 0)
        CALL(evt_npc_restart_regular_event, STRING("チョロボン"))
        CALL(evt_npc_set_position, STRING("チョロボン"), 0, 400, 0)
        CALL(evt_npc_flag_onoff, 0, STRING("チョロボン"), 131088)
        CALL(evt_npc_change_fbat_mode, 7)
        ELSE
        CALL(evt_npc_change_fbat_mode, 1)
        CALL(evt_npc_check, STRING("チョロボン"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_npc_set_battle_info, STRING("チョロボン"), 23)
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1774), 0)
        CALL(evt_item_entry, STRING("sun_st"), 62, 0, 0, 0, 0, -1, PTR(evt_chorobon))
    END_IF
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    CALL(evt_map_set_tevcallback, 1, PTR(hei_test_water_tev))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("mizu"), 16777216)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("mizu"), 256)
    RETURN
EVT_END()

static USER_FUNC(npc_dead) {
    // チョロボン
    npcGroupDead(npcNameToPtr("\x83\x60\x83\x87\x83\x8D\x83\x7B\x83\x93"), FALSE);
    return EVT_RETURN_DONE;
}


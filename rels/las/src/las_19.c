#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "las.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/mapdrv.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"

USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e1_door_close[22];
static EvtWord e1_door_open[22];
static EvtWord e2_door_close[22];
static EvtWord e2_door_open[22];
static BeroEntry bero_entry_data[5];
const char* mapgrp_name_1[18];
const char* mapgrp_name_2[5];
static EvtWord cam_shake[8];
EvtWord change_map_evt[359];
static EvtWord switch1_evt[13];
static EvtWord switch2_evt[13];
static EvtWord cloud_evt[5];
EvtWord las_19_init_evt[205];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_09", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_21", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_21", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_door_close, // enterAnimeArg
        e2_door_open, // exitAnimeArg
    },
    {
        "n_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_20", // nextMapName
        "s_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

const char* mapgrp_name_1[] = {
    "S_kai_02",
    "S_kai_03",
    "S_kai_04",
    "S_kai_05",
    "S_kai_06",
    "S_kai_07",
    "S_kai_08",
    "S_kai_09",
    "S_kai_10",
    "S_kai_11",
    "S_kai_12",
    "S_kai_13",
    "S_kai_14",
    "S_kai_15",
    "S_kai_16",
    "S_kai_17",
    "S_kai_18",
    "S_kai_25",
};

const char* mapgrp_name_2[] = {
    "S_kai_20",
    "S_kai_21",
    "S_kai_22",
    "S_kai_23",
    "S_kai_24",
};

static EVT_BEGIN(cam_shake)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.029296875f), FLOAT(0.029296875f), 200)
    RETURN
EVT_END()

EVT_BEGIN(change_map_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam3d_evt_set, 575, 143, 531, 575, 67, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_yuka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_under"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_under"), 1, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kai_26"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_02"), 0, 30, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_03"), 0, 60, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_04"), 0, 90, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_05"), 0, 120, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_06"), 0, 150, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_07"), 0, 180, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_08"), 0, 210, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_09"), 0, 240, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_10"), 0, 270, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_11"), 0, 300, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_12"), 0, 330, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_13"), 0, 360, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_14"), 0, 390, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_15"), 0, 420, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_16"), 0, 450, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_17"), 0, 480, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_18"), 0, 510, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_20"), 0, 30, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_21"), 0, 60, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_22"), 0, 90, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_23"), 0, 120, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_24"), 0, 150, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_19"), 0, 150, 0)
    CALL(evt_mapobj_trans, 1, STRING("S_kai_25"), 0, 540, 0)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_cam3d_evt_set, -575, 143, 531, -575, 67, -13, 4000, EASING_LINEAR)
        WAIT_MS(4500)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    LOOP(0)
        IF_INT_NOT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yuka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, -5, -547, 531, -5, -623, -13, 9200, EASING_LINEAR)
    WAIT_MS(300)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 0)
        SET_FLOAT_VALUE(LW(15), FLOAT(1.25f))
        LOOP(0)
            SET_FLOAT_VALUE(LW(14), FLOAT(30.0f))
            LOOP(24)
                SET_INT_READ_ADDR(PTR(&mapgrp_name_1))
                SET_FLOAT_VALUE(LW(1), LW(14))
                SET_VALUE(LW(2), 0)
                LOOP(18)
                    READ_INT(LW(4))
                    IF_INT_GREATER_OR_EQUAL(LW(2), LW(0))
                        CALL(evt_mapobj_trans, 1, LW(4), 0, LW(1), 0)
                        ADD_FLOAT(LW(1), FLOAT(30.0f))
                    END_IF
                    ADD(LW(2), 1)
                END_LOOP
                SUBTRACT_FLOAT(LW(14), LW(15))
                WAIT_FRAMES(1)
            END_LOOP
            IF_INT_EQUAL(GF(0), 0)
                CALL(evt_snd_sfxon, STRING("SFX_STG8_MAP_MOVE1"), 0)
                SPAWN_EVENT(PTR(cam_shake))
            END_IF
            ADD(LW(0), 1)
            IF_INT_EQUAL(LW(0), 18)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(7600)
    SET_VALUE(GF(0), 1)
    CALL(evt_bero_mapchange, STRING("las_10"), STRING("ret_chgmap_evt"))
    RETURN
EVT_END()

static EVT_BEGIN(switch1_evt)
    IF_INT_EQUAL(GSWF_LAS_19_SWITCH_1_ON, 0)
        SET_VALUE(GSWF_LAS_19_SWITCH_1_ON, 1)
        ELSE
        SET_VALUE(GSWF_LAS_19_SWITCH_1_ON, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(switch2_evt)
    IF_INT_EQUAL(GSWF_LAS_19_SWITCH_2_ON, 0)
        SET_VALUE(GSWF_LAS_19_SWITCH_2_ON, 1)
        ELSE
        SET_VALUE(GSWF_LAS_19_SWITCH_2_ON, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(cloud_evt)
    SET_VALUE(GSW_STORY_PROGRESS, STG8_08_THE_NEW_PATH)
    RETURN
EVT_END()

EVT_BEGIN(las_19_init_evt)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("chgmap_evt"))
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        ELSE
        IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST1"))
            ELSE
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST3"))
        END_IF
    END_IF
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_08_THE_NEW_PATH)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 0, STRING("S_futa"), STRING("A_futa"), PTR(cloud_evt))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_futa"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_futa"), 0, 0)
    END_IF
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("chgmap_evt"))
        CALL(evt_npc_check_delete, STRING("マホーン１"))
        SPAWN_EVENT(PTR(change_map_evt))
        ELSE
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_under"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_under"), 1, 0)
        CALL(evt_npc_check_delete, STRING("マホーン２"))
        CALL(evt_npc_check_delete, STRING("マホーン３"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yuka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_be"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kai_20"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kai_21"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kai_22"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kai_23"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kai_24"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_npc_check_delete, STRING("マホーン１"))
        SPAWN_EVENT(PTR(hikari_evt))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG8_06_MOVING_PLANETS)
        CALL(evt_mobj_switch_float_blk, STRING("switch1"), 375, -320, -45, STRING(""), 2, PTR(switch1_evt), 0)
        CALL(evt_mobj_switch_float_blk, STRING("switch2"), 450, -320, -45, STRING(""), 0, PTR(switch2_evt), 0)
        IF_INT_EQUAL(GSWF_LAS_19_SWITCH_1_ON, 0)
            CALL(evt_mobj_set_anim, STRING("switch1"), STRING("A_3"))
            ELSE
            CALL(evt_mobj_set_anim, STRING("switch1"), STRING("S_1"))
        END_IF
        IF_INT_EQUAL(GSWF_LAS_19_SWITCH_2_ON, 0)
            CALL(evt_mobj_set_anim, STRING("switch2"), STRING("A_3"))
            ELSE
            CALL(evt_mobj_set_anim, STRING("switch2"), STRING("S_1"))
        END_IF
    END_IF
    CALL(evt_mobj_badgeblk, STRING("mobj_item00"), 30, -440, -40, ITEM_THUNDER_RAGE, 0, GSWF_LAS_19_THUNDER_RAGE_BLOCK, 0)
    RETURN
EVT_END()


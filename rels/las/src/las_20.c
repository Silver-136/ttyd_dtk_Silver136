#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "las.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"

USER_FUNC(get_hosi_name);

static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[4];
static const char* haguruma_hontai[14];
static const char* haguruma_hontai2[6];
static const char* haguruma_kage[12];
static const char* haguruma_kage2[5];
static EvtWord haguruma_evt[194];
static EvtWord dai_evt[13];
const char* hosi_name[14];
EvtWord hosi_sw_check[303];
EvtWord hosi_onoff[69];
EvtWord hosi_02_sw_evt[15];
EvtWord hosi_03_sw_evt[15];
EvtWord hosi_04_sw_evt[15];
EvtWord hosi_05_sw_evt[15];
EvtWord hosi_06_sw_evt[15];
EvtWord hosi_07_sw_evt[15];
EvtWord hosi_08_sw_evt[15];
EvtWord hosi_init[175];
EvtWord las_20_init_evt[103];

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_22", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {
        "s_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_19", // nextMapName
        "n_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_21", // nextMapName
        "n_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static const char* haguruma_hontai[] = {
    "S_hgm_01a",
    "S_hgm_02a",
    "S_hgm_03a",
    "S_hgm_04a",
    "S_hgm_06a",
    "S_hgm_07a",
    "S_hgm_08a",
    "S_hgm_10a",
    "S_hgm_11a",
    "S_hgm_14a",
    "S_hgm_15a",
    "S_hgm_16a",
    "S_hgm_19a",
    "S_hgm_20a",
};

static const char* haguruma_hontai2[] = {
    "S_hgm_05a",
    "S_hgm_09a",
    "S_hgm_12a",
    "S_hgm_13a",
    "S_hgm_17a",
    "S_hgm_18a",
};

static const char* haguruma_kage[] = {
    "S_hgm_01b",
    "S_hgm_02b",
    "S_hgm_03b",
    "S_hgm_04b",
    "S_hgm_06b",
    "S_hgm_07b",
    "S_hgm_08b",
    "S_hgm_10b",
    "S_hgm_11b",
    "S_hgm_14b",
    "S_hgm_15b",
    "S_hgm_16b",
};

static const char* haguruma_kage2[] = {
    "S_hgm_05b",
    "S_hgm_09b",
    "S_hgm_12b",
    "S_hgm_13b",
    "S_hgm_17b",
};

static EVT_BEGIN(haguruma_evt)
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(4), FLOAT(0.0f))
    LOOP(0)
        SET_INT_READ_ADDR(PTR(&haguruma_hontai))
        LOOP(14)
            READ_INT(LW(5))
            CALL(evt_mapobj_rotate, 0, LW(5), 0, 0, LW(0))
        END_LOOP
        SET_INT_READ_ADDR(PTR(&haguruma_kage))
        LOOP(12)
            READ_INT(LW(5))
            CALL(evt_mapobj_rotate, 0, LW(5), 0, 0, LW(0))
        END_LOOP
        SET_INT_READ_ADDR(PTR(&haguruma_hontai2))
        LOOP(6)
            READ_INT(LW(5))
            CALL(evt_mapobj_rotate, 0, LW(5), 0, 0, LW(1))
        END_LOOP
        SET_INT_READ_ADDR(PTR(&haguruma_kage2))
        LOOP(5)
            READ_INT(LW(5))
            CALL(evt_mapobj_rotate, 0, LW(5), 0, 0, LW(1))
        END_LOOP
        CALL(evt_sub_get_sincos, LW(2), LW(5), LW(6))
        MULTIPLY_FLOAT(LW(5), FLOAT(41.0f))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_21a"), 0, 0, LW(5))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_21b"), 0, 0, LW(5))
        CALL(evt_sub_get_sincos, LW(3), LW(5), LW(6))
        MULTIPLY_FLOAT(LW(5), FLOAT(22.0f))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_22a"), 0, 0, LW(5))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_22b"), 0, 0, LW(5))
        CALL(evt_sub_get_sincos, LW(4), LW(5), LW(6))
        MULTIPLY_FLOAT(LW(5), FLOAT(110.0f))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_23a"), 0, 0, LW(5))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_23b"), 0, 0, LW(5))
        ADD_FLOAT(LW(0), FLOAT(1.0f))
        IF_INT_GREATER_OR_EQUAL(LW(0), 360)
            SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
        END_IF
        ADD_FLOAT(LW(1), FLOAT(-1.0f))
        IF_INT_LESS_OR_EQUAL(LW(1), -360)
            ADD_FLOAT(LW(1), FLOAT(360.0f))
        END_IF
        ADD_FLOAT(LW(2), FLOAT(0.89941406f))
        IF_INT_GREATER_OR_EQUAL(LW(2), 360)
            SUBTRACT_FLOAT(LW(2), FLOAT(360.0f))
        END_IF
        ADD_FLOAT(LW(3), FLOAT(0.89941406f))
        IF_INT_GREATER_OR_EQUAL(LW(3), 360)
            SUBTRACT_FLOAT(LW(3), FLOAT(360.0f))
        END_IF
        ADD_FLOAT(LW(4), FLOAT(1.0f))
        IF_INT_GREATER_OR_EQUAL(LW(4), 360)
            SUBTRACT_FLOAT(LW(4), FLOAT(360.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(dai_evt)
    SET_VALUE(LW(0), STRING("S_move_dai"))
    SET_VALUE(LW(1), STRING("A_move_dai"))
    SET_VALUE(LW(2), 2700)
    CALL_CHILD(PTR(move_dai))
    RETURN
EVT_END()

const char* hosi_name[] = {
    "S_hosi_02a",
    "S_hosi_02b",
    "S_hosi_03a",
    "S_hosi_03b",
    "S_hosi_04a",
    "S_hosi_04b",
    "S_hosi_05a",
    "S_hosi_05b",
    "S_hosi_06a",
    "S_hosi_06b",
    "S_hosi_07a",
    "S_hosi_07b",
    "S_hosi_08a",
    "S_hosi_08b",
};

EVT_BEGIN(hosi_sw_check)
    SET_VALUE(LW(0), 0)
    IF_INT_EQUAL(LSWF(1), 1)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LSWF(2), 0)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LSWF(3), 1)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LSWF(4), 1)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LSWF(5), 0)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LSWF(6), 1)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LSWF(7), 0)
        ADD(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 7)
        CALL(evt_mario_key_onoff, 0)
        WAIT_MS(1000)
        CALL(evt_cam3d_evt_set, -289, 112, 1038, -289, -43, -63, 800, EASING_SINE_IN_OUT)
        WAIT_MS(800)
        SET_VALUE(LW(0), 0)
        LOOP(7)
            CALL(get_hosi_name, LW(0), LW(1), LW(2))
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, LW(2), MAP_OBJ_FLAG_HIDDEN)
            ADD(LW(0), 1)
        END_LOOP
        SET_VALUE(LW(0), 0)
        SET_VALUE(LW(1), 6)
        LOOP(50)
            CALL(get_hosi_name, LW(0), LW(2), LW(3))
            CALL(evt_mapobj_flag_onoff, 1, 0, LW(2), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(3), MAP_OBJ_FLAG_HIDDEN)
            CALL(get_hosi_name, LW(1), LW(2), LW(3))
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(2), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, LW(3), MAP_OBJ_FLAG_HIDDEN)
            WAIT_MS(50)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(0), 1)
            MODULO(LW(0), 7)
        END_LOOP
        LOOP(0)
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            CALL(get_hosi_name, LW(0), LW(2), LW(3))
            CALL(evt_mapobj_flag_onoff, 1, 0, LW(2), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(3), MAP_OBJ_FLAG_HIDDEN)
            CALL(get_hosi_name, LW(1), LW(2), LW(3))
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(2), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, LW(3), MAP_OBJ_FLAG_HIDDEN)
            WAIT_MS(50)
            SET_VALUE(LW(1), LW(0))
            ADD(LW(0), 1)
            MODULO(LW(0), 7)
        END_LOOP
        CALL(get_hosi_name, LW(1), LW(2), LW(3))
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(2), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(3), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(200)
        SET_VALUE(LW(0), 0)
        LOOP(7)
            CALL(get_hosi_name, LW(0), LW(1), LW(2))
            CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(2), MAP_OBJ_FLAG_HIDDEN)
            ADD(LW(0), 1)
        END_LOOP
        CALL(evt_snd_sfxon, STRING("SFX_STG8_STAR_LIGHTUP2"), 0)
        WAIT_MS(100)
        CALL(evt_snd_sfxon, STRING("SFX_STG8_QUAKE3"), LW(10))
        CALL(evt_cam_shake, 4, FLOAT(0.01953125f), FLOAT(0.0f), 1000)
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_mobj_delete, STRING("hosi_02_sw"))
        CALL(evt_mobj_delete, STRING("hosi_03_sw"))
        CALL(evt_mobj_delete, STRING("hosi_04_sw"))
        CALL(evt_mobj_delete, STRING("hosi_05_sw"))
        CALL(evt_mobj_delete, STRING("hosi_06_sw"))
        CALL(evt_mobj_delete, STRING("hosi_07_sw"))
        CALL(evt_mobj_delete, STRING("hosi_08_sw"))
        SET_VALUE(GSW_STORY_PROGRESS, STG8_11_STAR_PUZZLE)
        CALL(evt_cam3d_evt_off, 800, EASING_SINE_IN_OUT)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(hosi_onoff)
    GET_INT_INDEXED(LW(2), LW(3))
    IF_INT_EQUAL(LW(3), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(0), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        SET_INT_INDEXED(LW(2), 1)
        CALL(evt_mapobj_get_position, LW(0), LW(3), LW(4), LW(5))
        CALL(evt_eff64, 0, STRING("stardust_n64"), 2, LW(3), LW(4), LW(5), 30, 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_STAR_LIGHTUP1"), LW(3), LW(4), LW(5), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, LW(0), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_HIDDEN)
        SET_INT_INDEXED(LW(2), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(hosi_02_sw_evt)
    SET_VALUE(LW(0), STRING("S_hosi_02a"))
    SET_VALUE(LW(1), STRING("S_hosi_02b"))
    SET_RAW_VALUE(LW(2), LSWF(1))
    CALL_CHILD(PTR(hosi_onoff))
    CALL_CHILD(PTR(hosi_sw_check))
    RETURN
EVT_END()

EVT_BEGIN(hosi_03_sw_evt)
    SET_VALUE(LW(0), STRING("S_hosi_03a"))
    SET_VALUE(LW(1), STRING("S_hosi_03b"))
    SET_RAW_VALUE(LW(2), LSWF(2))
    CALL_CHILD(PTR(hosi_onoff))
    CALL_CHILD(PTR(hosi_sw_check))
    RETURN
EVT_END()

EVT_BEGIN(hosi_04_sw_evt)
    SET_VALUE(LW(0), STRING("S_hosi_04a"))
    SET_VALUE(LW(1), STRING("S_hosi_04b"))
    SET_RAW_VALUE(LW(2), LSWF(3))
    CALL_CHILD(PTR(hosi_onoff))
    CALL_CHILD(PTR(hosi_sw_check))
    RETURN
EVT_END()

EVT_BEGIN(hosi_05_sw_evt)
    SET_VALUE(LW(0), STRING("S_hosi_05a"))
    SET_VALUE(LW(1), STRING("S_hosi_05b"))
    SET_RAW_VALUE(LW(2), LSWF(4))
    CALL_CHILD(PTR(hosi_onoff))
    CALL_CHILD(PTR(hosi_sw_check))
    RETURN
EVT_END()

EVT_BEGIN(hosi_06_sw_evt)
    SET_VALUE(LW(0), STRING("S_hosi_06a"))
    SET_VALUE(LW(1), STRING("S_hosi_06b"))
    SET_RAW_VALUE(LW(2), LSWF(5))
    CALL_CHILD(PTR(hosi_onoff))
    CALL_CHILD(PTR(hosi_sw_check))
    RETURN
EVT_END()

EVT_BEGIN(hosi_07_sw_evt)
    SET_VALUE(LW(0), STRING("S_hosi_07a"))
    SET_VALUE(LW(1), STRING("S_hosi_07b"))
    SET_RAW_VALUE(LW(2), LSWF(6))
    CALL_CHILD(PTR(hosi_onoff))
    CALL_CHILD(PTR(hosi_sw_check))
    RETURN
EVT_END()

EVT_BEGIN(hosi_08_sw_evt)
    SET_VALUE(LW(0), STRING("S_hosi_08a"))
    SET_VALUE(LW(1), STRING("S_hosi_08b"))
    SET_RAW_VALUE(LW(2), LSWF(7))
    CALL_CHILD(PTR(hosi_onoff))
    CALL_CHILD(PTR(hosi_sw_check))
    RETURN
EVT_END()

EVT_BEGIN(hosi_init)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG8_11_STAR_PUZZLE)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_02a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_03a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_04a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_05a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_06a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_07a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_08a"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LSWF(1), 0)
        SET_VALUE(LSWF(2), 0)
        SET_VALUE(LSWF(3), 0)
        SET_VALUE(LSWF(4), 0)
        SET_VALUE(LSWF(5), 0)
        SET_VALUE(LSWF(6), 0)
        SET_VALUE(LSWF(7), 0)
        CALL(evt_mobj_floatswitch_red, 0, STRING("hosi_02_sw"), -545, -50, -120, PTR(hosi_02_sw_evt), 0)
        CALL(evt_mobj_floatswitch_red, 0, STRING("hosi_03_sw"), -460, -50, -120, PTR(hosi_03_sw_evt), 0)
        CALL(evt_mobj_floatswitch_red, 0, STRING("hosi_04_sw"), -376, -50, -120, PTR(hosi_04_sw_evt), 0)
        CALL(evt_mobj_floatswitch_red, 0, STRING("hosi_05_sw"), -289, -50, -120, PTR(hosi_05_sw_evt), 0)
        CALL(evt_mobj_floatswitch_red, 0, STRING("hosi_06_sw"), -205, -50, -120, PTR(hosi_06_sw_evt), 0)
        CALL(evt_mobj_floatswitch_red, 0, STRING("hosi_07_sw"), -121, -50, -120, PTR(hosi_07_sw_evt), 0)
        CALL(evt_mobj_floatswitch_red, 0, STRING("hosi_08_sw"), -37, -50, -120, PTR(hosi_08_sw_evt), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_02b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_03b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_04b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_05b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_06b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_07b"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hosi_08b"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(las_20_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST3"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS5"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_01a"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_08a"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_09a"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_12a"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_19a"), MAP_OBJ_FLAG_CLIP)
    SPAWN_EVENT(PTR(haguruma_evt))
    CALL(evt_map_playanim, STRING("kusari"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_hit_bind_mapobj, STRING("A_move_dai"), STRING("S_move_dai"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_move_dai"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_move_dai"), 0, 0)
    CALL(evt_mobj_switch_red, 0, STRING("dai_switch"), -250, -290, 100, PTR(dai_evt), 0)
    SPAWN_EVENT(PTR(hosi_init))
    CALL(evt_mobj_badgeblk, STRING("mobj_item00"), 295, -440, -15, ITEM_REPEL_CAPE, 0, GSWF(4388), 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_item01"), 640, -320, -135, ITEM_LIFE_SHROOM, 0, GSWF(4389), 0)
    RETURN
EVT_END()

USER_FUNC(get_hosi_name) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (EvtWord)hosi_name[idx * 2]);
    evtSetValue(event, args[2], (EvtWord)hosi_name[idx * 2 + 1]);
    return EVT_RETURN_DONE;
}

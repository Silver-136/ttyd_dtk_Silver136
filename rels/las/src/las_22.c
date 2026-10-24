#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "las.h"
#include "mario/mario_pouch.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_hit_bind_mapobj);
USER_FUNC(evt_hit_bind_update);
USER_FUNC(evt_hit_get_position);
USER_FUNC(evt_hitobj_onoff);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_item_flag_on);
USER_FUNC(evt_item_get_item);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mapobj_flag_onoff);
USER_FUNC(evt_mapobj_get_position);
USER_FUNC(evt_mapobj_rotate);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_mobj_exec_cancel);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_itembox);
USER_FUNC(evt_mobj_lock);
USER_FUNC(evt_mobj_save_blk);
USER_FUNC(evt_mobj_switch_red);
USER_FUNC(evt_mobj_wait_animation_end);
USER_FUNC(evt_npc_check_delete);
USER_FUNC(evt_party_get_name_hitobj_ride);
USER_FUNC(evt_party_get_pos);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_pouch_check_item);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_get_sincos);
USER_FUNC(evt_win_item_select);

f32 fd_las_22_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_las_22_neg_one_382;
f32 fd_las_22___local_PI_383;
f32 fd_las_22___local_3PIO2_385;
f32 fd_las_22_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_las_22___local_PIO2_384;
f32 fd_las_22_one_381;
f32 fd_las_22_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_las_22_neg_one_365;
f32 fd_las_22___local_PI_366;
f32 fd_las_22___local_3PIO2_368;
f32 fd_las_22_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_las_22___local_PIO2_367;
f32 fd_las_22_one_364;
static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
EvtWord door_dummy[2];
static BeroEntry bero_entry_data[4];
static const char* haguruma_hontai[13];
static const char* haguruma_hontai2[7];
static const char* haguruma_kage[11];
static const char* haguruma_kage2[6];
static EvtWord haguruma_evt[194];
EvtWord kaitendai_evt[312];
static EvtWord dai_evt[13];
static s32 key_tbl[2];
static EvtWord key_evt[5];
static EvtWord key_check_evt[23];
static EvtWord unlock_evt[10];
static EvtWord mobj_box00[34];
EvtWord las_22_init_evt[150];

f32 fd_las_22_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_las_22_neg_one_382 = -1.0f;

f32 fd_las_22___local_PI_383 = 3.1415927f;

f32 fd_las_22___local_3PIO2_385 = 4.712389f;

f32 fd_las_22_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_las_22___local_PIO2_384 = 1.5707964f;

f32 fd_las_22_one_381 = 1.0f;

f32 fd_las_22_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_las_22_neg_one_365 = -1.0f;

f32 fd_las_22___local_PI_366 = 3.1415927f;

f32 fd_las_22___local_3PIO2_368 = 4.712389f;

f32 fd_las_22_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_las_22___local_PIO2_367 = 1.5707964f;

f32 fd_las_22_one_364 = 1.0f;

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

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(door_dummy)
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
        "las_20", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
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
        "las_24", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_23", // nextMapName
        "n_bero_1", // nextBeroName
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
    "S_hgm_12a",
    "S_hgm_14a",
    "S_hgm_16a",
    "S_hgm_19a",
    "S_hgm_20a",
};

static const char* haguruma_hontai2[] = {
    "S_hgm_05a",
    "S_hgm_09a",
    "S_hgm_11a",
    "S_hgm_13a",
    "S_hgm_15a",
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
    "S_hgm_12b",
    "S_hgm_14b",
    "S_hgm_16b",
};

static const char* haguruma_kage2[] = {
    "S_hgm_05b",
    "S_hgm_09b",
    "S_hgm_11b",
    "S_hgm_13b",
    "S_hgm_15b",
    "S_hgm_17b",
};

static EVT_BEGIN(haguruma_evt)
    SET_FLOAT_VALUE(LW(0), FLOAT(20.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(4), FLOAT(0.0f))
    LOOP(0)
        SET_INT_READ_ADDR(PTR(&haguruma_hontai))
        LOOP(13)
            READ_INT(LW(5))
            CALL(evt_mapobj_rotate, 0, LW(5), 0, 0, LW(0))
        END_LOOP
        SET_INT_READ_ADDR(PTR(&haguruma_kage))
        LOOP(11)
            READ_INT(LW(5))
            CALL(evt_mapobj_rotate, 0, LW(5), 0, 0, LW(0))
        END_LOOP
        SET_INT_READ_ADDR(PTR(&haguruma_hontai2))
        LOOP(7)
            READ_INT(LW(5))
            CALL(evt_mapobj_rotate, 0, LW(5), 0, 0, LW(1))
        END_LOOP
        SET_INT_READ_ADDR(PTR(&haguruma_kage2))
        LOOP(6)
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

EVT_BEGIN(kaitendai_evt)
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(0.009765625f))
    CALL(evt_hit_bind_mapobj, STRING("A_hgm_dai"), STRING("S_hgm_dai"))
    CALL(evt_hit_bind_mapobj, STRING("A_hgm_dai_02"), STRING("S_hgm_dai_02"))
    CALL(evt_hit_bind_mapobj, STRING("A_hgm_dai_03"), STRING("S_hgm_dai_03"))
    CALL(evt_hit_bind_mapobj, STRING("A_hgm_dai_04"), STRING("S_hgm_dai_04"))
    CALL(evt_hit_bind_mapobj, STRING("A_hgm_dai_05"), STRING("S_hgm_dai_05"))
    CALL(evt_mapobj_get_position, STRING("A_hgm_dai_01"), LW(13), LW(14), LW(15))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_FLOOR_ROUND_START1"), LW(13), LW(14), LW(15), 0)
    LOOP(0)
        SET_VALUE(LF(0), 0)
        SET_VALUE(LF(1), 0)
        CALL(evt_mario_get_name_hitobj_ride, LW(2))
        IF_STR_EQUAL(LW(2), STRING("A_hgm_dai_02"))
            CALL(evt_hit_get_position, STRING("A_hgm_dai_02"), LW(3), LW(4), LW(5))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("A_hgm_dai_03"))
            CALL(evt_hit_get_position, STRING("A_hgm_dai_03"), LW(3), LW(4), LW(5))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("A_hgm_dai_04"))
            CALL(evt_hit_get_position, STRING("A_hgm_dai_04"), LW(3), LW(4), LW(5))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("A_hgm_dai_05"))
            CALL(evt_hit_get_position, STRING("A_hgm_dai_05"), LW(3), LW(4), LW(5))
            SET_VALUE(LF(0), 1)
        END_IF
        CALL(evt_party_get_name_hitobj_ride, 0, LW(6))
        IF_STR_EQUAL(LW(6), STRING("A_hgm_dai_02"))
            CALL(evt_hit_get_position, STRING("A_hgm_dai_02"), LW(7), LW(8), LW(9))
            SET_VALUE(LF(1), 1)
        END_IF
        IF_STR_EQUAL(LW(6), STRING("A_hgm_dai_03"))
            CALL(evt_hit_get_position, STRING("A_hgm_dai_03"), LW(7), LW(8), LW(9))
            SET_VALUE(LF(1), 1)
        END_IF
        IF_STR_EQUAL(LW(6), STRING("A_hgm_dai_04"))
            CALL(evt_hit_get_position, STRING("A_hgm_dai_04"), LW(7), LW(8), LW(9))
            SET_VALUE(LF(1), 1)
        END_IF
        IF_STR_EQUAL(LW(6), STRING("A_hgm_dai_05"))
            CALL(evt_hit_get_position, STRING("A_hgm_dai_05"), LW(7), LW(8), LW(9))
            SET_VALUE(LF(1), 1)
        END_IF
        CALL(evt_mapobj_rotate, 1, STRING("S_hgm_dai"), 0, 0, LW(0))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_dai_02"), 0, 0, LW(1))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_dai_03"), 0, 0, LW(1))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_dai_04"), 0, 0, LW(1))
        CALL(evt_mapobj_rotate, 0, STRING("S_hgm_dai_05"), 0, 0, LW(1))
        CALL(evt_hit_bind_update, STRING("A_hgm_dai"))
        CALL(evt_hit_bind_update, STRING("A_hgm_dai_02"))
        CALL(evt_hit_bind_update, STRING("A_hgm_dai_03"))
        CALL(evt_hit_bind_update, STRING("A_hgm_dai_04"))
        CALL(evt_hit_bind_update, STRING("A_hgm_dai_05"))
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_hit_get_position, LW(2), LW(10), LW(11), LW(12))
            SUBTRACT_FLOAT(LW(10), LW(3))
            SUBTRACT_FLOAT(LW(11), LW(4))
            SUBTRACT_FLOAT(LW(12), LW(5))
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            ADD_FLOAT(LW(3), LW(10))
            ADD_FLOAT(LW(4), LW(11))
            ADD_FLOAT(LW(5), LW(12))
            CALL(evt_mario_set_pos, LW(3), LW(4), LW(5))
        END_IF
        IF_INT_EQUAL(LF(1), 1)
            CALL(evt_hit_get_position, LW(6), LW(10), LW(11), LW(12))
            SUBTRACT_FLOAT(LW(10), LW(7))
            SUBTRACT_FLOAT(LW(11), LW(8))
            SUBTRACT_FLOAT(LW(12), LW(9))
            CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
            ADD_FLOAT(LW(3), LW(10))
            ADD_FLOAT(LW(4), LW(11))
            ADD_FLOAT(LW(5), LW(12))
            CALL(evt_party_set_pos, 0, LW(3), LW(4), LW(5))
        END_IF
        ADD_FLOAT(LW(0), FLOAT(0.5f))
        IF_INT_GREATER(LW(0), 360)
            SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
        END_IF
        ADD_FLOAT(LW(1), FLOAT(-0.5f))
        IF_INT_LESS(LW(1), -360)
            ADD_FLOAT(LW(1), FLOAT(360.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(dai_evt)
    SET_VALUE(LW(0), STRING("S_move_dai"))
    SET_VALUE(LW(1), STRING("A_move_dai"))
    SET_VALUE(LW(2), 2200)
    CALL_CHILD(PTR(move_dai))
    RETURN
EVT_END()

static s32 key_tbl[] = {
    ITEM_PALACE_KEY_0x2F, -1,
};

static EVT_BEGIN(key_evt)
    SET_VALUE(GSW(0), 393)
    RETURN
EVT_END()

static EVT_BEGIN(key_check_evt)
    CALL(evt_pouch_check_item, 47, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(unlock_evt)
    SET_VALUE(GSW(0), 394)
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), 130, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(las_22_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST3"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS6"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS(GSW(0), 390)
        CALL(evt_npc_check_delete, STRING("スーパーマホーン"))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW(0), 395)
        SPAWN_EVENT(PTR(kaitendai_evt))
    END_IF
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_01a"), 2)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_05a"), 2)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_08a"), 2)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_19a"), 2)
    SPAWN_EVENT(PTR(haguruma_evt))
    CALL(evt_map_playanim, STRING("kusari"), 1, 0)
    CALL(evt_hit_bind_mapobj, STRING("A_move_dai"), STRING("S_move_dai"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_move_dai"), 1)
    CALL(evt_hitobj_onoff, STRING("A_move_dai"), 0, 0)
    CALL(evt_mobj_switch_red, 0, STRING("dai_switch"), 100, -409, -10, PTR(dai_evt), 0)
    CALL(evt_mobj_itembox, STRING("mobj_box00"), -660, -500, 100, 0, 0, PTR(mobj_box00), GSWF(4377))
    CALL(evt_mobj_badgeblk, STRING("mobj_hatena_gat"), -150, 220, -100, 151, 0, GSWF(4378), 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -650, 100, -100, 0, 0)
    IF_INT_LESS(GSW(0), 393)
        CALL(evt_item_entry, STRING("key"), 47, -25, -80, -100, 16, -1, PTR(key_evt))
    END_IF
    IF_INT_LESS(GSW(0), 394)
        CALL(evt_mobj_lock, STRING("lock"), 47, -695, 50, 0, 90, PTR(key_check_evt), PTR(unlock_evt), GSWF(4355))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()


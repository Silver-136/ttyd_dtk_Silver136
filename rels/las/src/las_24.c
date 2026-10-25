#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "las.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"

USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);
#ifdef VERSION_G8ME01
USER_FUNC(las_24_adjust_for_framerate);
#endif

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static BeroEntry bero_entry_data[4];
static const char* haguruma_hontai[15];
static const char* haguruma_hontai2[6];
static const char* haguruma_kage[12];
static const char* haguruma_kage2[5];
static EvtWord haguruma_evt[194];
EvtWord cloud_blk_evt[10];
EvtWord break_floor_evt[5];
EvtWord jump_evt[68];
#ifdef VERSION_G8ME01
static EvtWord dai_evt[17];
#else
static EvtWord dai_evt[13];
#endif
static EvtWord box_evt[33];
EvtWord las_24_init_evt[190];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_02"), 0, LW(0), 0)
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
        "las_22", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
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
        "las_23", // nextMapName
        "n_bero_2", // nextBeroName
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
        "las_25", // nextMapName
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
    "S_hgm_11a",
    "S_hgm_14a",
    "S_hgm_15a",
    "S_hgm_16a",
    "S_hgm_19a",
    "S_hgm_20a",
    "S_hgm_24a",
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
    SET_FLOAT_VALUE(LW(0), FLOAT(20.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(4), FLOAT(0.0f))
    LOOP(0)
        SET_INT_READ_ADDR(PTR(&haguruma_hontai))
        LOOP(15)
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

EVT_BEGIN(cloud_blk_evt)
    SET_VALUE(GSWF(4356), 1)
    CALL(evt_mobj_flag_onoff, 0, STRING("blk"), 256)
    RETURN
EVT_END()

EVT_BEGIN(break_floor_evt)
    SET_VALUE(GSW_STORY_PROGRESS, 396)
    RETURN
EVT_END()

EVT_BEGIN(jump_evt)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            LOOP(90)
                CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
                IF_INT_GREATER_OR_EQUAL(LW(1), -201)
                    IF_INT_LESS_OR_EQUAL(LW(1), -199)
                        RETURN
                    END_IF
                END_IF
                CALL(evt_party_get_name_hitobj_ride, 0, LW(0))
                IF_STR_EQUAL(LW(0), STRING("MOBJ_jump"))
                    CALL(evt_party_jump_pos, 0, -455, -200, 50, 600, 0, -1000)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_IF
    END_INLINE_EVENT
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_jump_jumpstand, -500, -200, 0, -545, 100, 0, -450, -200, 0, 1000, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(dai_evt)
    SET_VALUE(LW(0), STRING("S_move_dai"))
    SET_VALUE(LW(1), STRING("A_move_dai"))
    SET_VALUE(LW(2), 7200)
#ifdef VERSION_G8ME01
    CALL(las_24_adjust_for_framerate, LW(2), LW(2))
#endif
    CALL_CHILD(PTR(move_dai))
    RETURN
EVT_END()

static EVT_BEGIN(box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("key"), 48, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    SET_VALUE(GSW_STORY_PROGRESS, 397)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(las_24_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST3"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS7"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_05a"), 2)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_19a"), 2)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hgm_24a"), 2)
    SPAWN_EVENT(PTR(haguruma_evt))
    CALL(evt_map_playanim, STRING("kusari"), 1, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 396)
        IF_INT_EQUAL(GSWF(4356), 0)
            CALL(evt_cloud_init)
            CALL(evt_cloud_ent, 0, STRING("S_hako"), STRING("A_hako"), PTR(cloud_blk_evt))
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hako"), 1)
            CALL(evt_hitobj_onoff, STRING("A_hako"), 1, 0)
        END_IF
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hako"), 1)
        CALL(evt_hitobj_onoff, STRING("A_hako"), 1, 0)
    END_IF
    CALL(evt_mobj_lv_blk, STRING("blk"), -495, 100, 0, 0, GSWF(4357), STRING("MOBJ_Lv2BigBigBlock"))
    IF_INT_EQUAL(GSWF(4357), 0)
        CALL(evt_mobj_flag_onoff, 0, STRING("blk"), 64)
        IF_INT_EQUAL(GSWF(4356), 0)
            CALL(evt_mobj_flag_onoff, 1, STRING("blk"), 256)
            CALL(evt_mobj_set_scale, STRING("blk"), FLOAT(0.9980469f), FLOAT(0.9980469f), FLOAT(0.9980469f))
        END_IF
    END_IF
    CALL(evt_mobj_breaking_floor, STRING("brkflr"), -500, 100, 0, 0, PTR(break_floor_evt), GSWF(4358))
    CALL(evt_mobj_jumpstand_red, 0, STRING("jump"), -500, -200, 0, PTR(jump_evt), 0)
    CALL(evt_hit_bind_mapobj, STRING("A_move_dai"), STRING("S_move_dai"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_move_dai"), 1)
    CALL(evt_hitobj_onoff, STRING("A_move_dai"), 0, 0)
    CALL(evt_mobj_floatswitch_red, 0, STRING("dai_switch"), -150, -440, 75, PTR(dai_evt), 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_hatena_gat"), -625, -200, -100, 121, 0, GSWF(4390), 0)
    CALL(evt_mobj_brick, STRING("mobj_kakusi"), -625, -115, -100, 161, 11, 0, GSWF(4391))
    CALL(evt_mobj_itembox, STRING("box"), -255, -380, 75, 0, 0, PTR(box_evt), GSWF(4370))
    RETURN
EVT_END()

#ifdef VERSION_G8ME01

USER_FUNC(las_24_adjust_for_framerate) {
    EvtWord* args = event->args;
    s32 value = evtGetValue(event, args[0]);

    if (gp->framerate != 60) {
        value *= 1.2f;
    }
    evtSetValue(event, args[1], value);
    return EVT_RETURN_DONE;
}
#endif

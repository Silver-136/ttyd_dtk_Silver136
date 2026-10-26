#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_movefloor.h"
#include "driver/hitdrv.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "driver/mobjdrv.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[7];
static HitDamageReturn hitDmgRetPoints[9];
static EvtWord evt_switch_00_init[26];
static EvtWord evt_switch_00_after[11];
static EvtWord evt_switch_00[56];
static EvtWord evt_switch_01_init[26];
static EvtWord evt_switch_01_after[11];
static EvtWord evt_switch_01[56];
EvtWord tik_16_init_evt[115];

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_17", // nextMapName
        "e_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_17", // nextMapName
        "e_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_15", // nextMapName
        "w_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_15", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "mri_00", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "nok_01", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_1", {0.0f, -1000.0f, 0.0f}},
    {"mod_2", {-250.0f, 0.0f, 0.0f}},
    {"mod_3", {-250.0f, 0.0f, 0.0f}},
    {"mod_4", {250.0f, 0.0f, 0.0f}},
    {"mod_5", {250.0f, 0.0f, 0.0f}},
    {"mod_6", {0.0f, -1000.0f, 0.0f}},
    {"a_dai_1", {0.0f, -1000.0f, 0.0f}},
    {"a_dai_2", {0.0f, -1000.0f, 0.0f}},
    {0},
};

static EVT_BEGIN(evt_switch_00_init)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("S_dakan_1"))
    IF_INT_EQUAL(GSWF_TIK_16_MRI_PIPE_RAISED, 1)
        CALL(evt_mapobj_trans, 1, STRING("S_dakan_1"), 0, 30, 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_1"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_00_after)
    IF_INT_EQUAL(GSWF_TIK_16_MRI_PIPE_RAISED, 0)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_00)
    CALL(evt_mapobj_get_position, STRING("S_dakan_1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 30, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_dakan_1"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_1"))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LW(0), STRING("dokan_1"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_01_init)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_2"), STRING("S_dakan_2"))
    IF_INT_EQUAL(GSWF_TIK_16_NOK_PIPE_RAISED, 1)
        CALL(evt_mapobj_trans, 1, STRING("S_dakan_2"), 0, 30, 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_2"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_2_k"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_01_after)
    IF_INT_EQUAL(GSWF_TIK_16_NOK_PIPE_RAISED, 0)
        SET_VALUE(LW(0), STRING("dokan_2"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_switch_01)
    CALL(evt_mapobj_get_position, STRING("S_dakan_2"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 30, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, STRING("S_dakan_2"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_2"))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_2_k"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LW(0), STRING("dokan_2"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(tik_16_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK7"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL_CHILD(PTR(evt_switch_00_init))
    CALL_CHILD(PTR(evt_switch_01_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_switch_00_after))
    CALL_CHILD(PTR(evt_switch_01_after))
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_playanim, STRING("anm_mizu"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_mobj_lv_blk, STRING("block"), 0, 0, 0, 0, GSWF_TIK_16_LV1_BIGBIG_BLOCK_BROKEN, STRING("MOBJ_Lv1BigBigBlock"))
    IF_INT_EQUAL(GSWF_TIK_16_LV1_BIGBIG_BLOCK_BROKEN, 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("block"), MOBJ_FLAG_NO_CULL)
    END_IF
    SET_VALUE(LW(0), STRING("dai_3"))
    SET_VALUE(LW(1), STRING("a_dai_3"))
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), -75)
    SET_VALUE(LW(4), 2000)
    SET_VALUE(LW(5), 500)
    SET_VALUE(LW(6), 1)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(11), 0)
    SET_VALUE(LW(12), 0)
    SPAWN_EVENT(PTR(moving_floor))
    CALL(evt_mobj_tornadoswitch_blue, 0, STRING("switch00"), -125, 0, 0, PTR(evt_switch_00), GSWF_TIK_16_MRI_PIPE_RAISED)
    CALL(evt_mobj_tornadoswitch_blue, 0, STRING("switch01"), 125, 0, 0, PTR(evt_switch_01), GSWF_TIK_16_NOK_PIPE_RAISED)
    CALL(evt_mobj_powerupblk, STRING("pblk1"), 125, 160, -100, 0, GSWF_SHINE_09, 0)
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "driver/hitdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_telop.h"
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"

USER_FUNC(evt_mario_dispflag_onoff);

static EvtWord door01_close[22];
static EvtWord door01_open[22];
static BeroEntry bero_entry_data[5];
EvtWord honenoko_oidasare_evt[466];
EvtWord gon_01_GATEEVENT_evt[151];
static HitDamageReturn hitDmgRetPoints[17];
EvtWord gon_01_init_evt[102];

static EVT_BEGIN(door01_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door01_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
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
        "gon_00", // nextMapName
        "e_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
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
        "gon_00", // nextMapName
        "e_bero_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_02", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door01_close, // enterAnimeArg
        door01_open, // exitAnimeArg
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
        "gon_02", // nextMapName
        "w_bero2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(honenoko_oidasare_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_party_set_dir, 0, 270, -1)
    CALL(evt_mario_set_dispdir, 0)
    CALL(evt_mario_set_pos, 500, 0, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_cont_onoff, 0)
    SET_FLOAT_VALUE(LW(0), FLOAT(180.0f))
    CALL_CHILD(PTR(door01_close))
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mario_set_pose, STRING("M_I_O"))
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_D_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_D_1"))
    END_SWITCH
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        SET_VALUE(LW(1), 2)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(door01_close))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_MS(500)
        SET_FLOAT_VALUE(LW(5), FLOAT(500.0f))
        SET_FLOAT_VALUE(LW(6), FLOAT(0.099609375f))
        SET_FLOAT_VALUE(LW(7), FLOAT(-5.0f))
        SET_FLOAT_VALUE(LW(8), FLOAT(4.0f))
        SET_FLOAT_VALUE(LW(9), FLOAT(-0.34960938f))
        LOOP(0)
            ADD_FLOAT(LW(5), LW(7))
            ADD_FLOAT(LW(6), LW(8))
            ADD_FLOAT(LW(8), LW(9))
            IF_INT_LESS_OR_EQUAL(LW(6), FLOAT(0.0f))
                SET_FLOAT_VALUE(LW(6), FLOAT(0.0f))
            END_IF
            CALL(evt_party_set_pos, 0, LW(5), LW(6), 0)
            IF_INT_LESS_OR_EQUAL(LW(6), FLOAT(0.0f))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_FLOAT_VALUE(LW(6), FLOAT(0.099609375f))
        SET_FLOAT_VALUE(LW(7), FLOAT(-2.0f))
        SET_FLOAT_VALUE(LW(8), FLOAT(1.5f))
        SET_FLOAT_VALUE(LW(9), FLOAT(-0.34960938f))
        LOOP(0)
            ADD_FLOAT(LW(5), LW(7))
            ADD_FLOAT(LW(6), LW(8))
            ADD_FLOAT(LW(8), LW(9))
            IF_INT_LESS_OR_EQUAL(LW(6), FLOAT(0.0f))
                SET_FLOAT_VALUE(LW(6), FLOAT(0.0f))
            END_IF
            CALL(evt_party_set_pos, 0, LW(5), LW(6), 0)
            IF_INT_LESS_OR_EQUAL(LW(6), FLOAT(0.0f))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_D_3"))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_D_3"))
        END_SWITCH
    END_SIBLING_EVENT
    SET_FLOAT_VALUE(LW(10), FLOAT(500.0f))
    SET_FLOAT_VALUE(LW(11), FLOAT(0.099609375f))
    SET_FLOAT_VALUE(LW(12), FLOAT(-5.0f))
    SET_FLOAT_VALUE(LW(13), FLOAT(5.0f))
    SET_FLOAT_VALUE(LW(14), FLOAT(-0.34960938f))
    LOOP(0)
        ADD_FLOAT(LW(10), LW(12))
        ADD_FLOAT(LW(11), LW(13))
        ADD_FLOAT(LW(13), LW(14))
        IF_INT_LESS_OR_EQUAL(LW(11), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(11), FLOAT(0.0f))
        END_IF
        CALL(evt_mario_set_pos, LW(10), LW(11), 0)
        IF_INT_LESS_OR_EQUAL(LW(11), FLOAT(0.0f))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_FLOAT_VALUE(LW(11), FLOAT(0.099609375f))
    SET_FLOAT_VALUE(LW(12), FLOAT(-2.0f))
    SET_FLOAT_VALUE(LW(13), FLOAT(1.5f))
    SET_FLOAT_VALUE(LW(14), FLOAT(-0.34960938f))
    LOOP(0)
        ADD_FLOAT(LW(10), LW(12))
        ADD_FLOAT(LW(11), LW(13))
        ADD_FLOAT(LW(13), LW(14))
        IF_INT_LESS_OR_EQUAL(LW(11), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(11), FLOAT(0.0f))
        END_IF
        CALL(evt_mario_set_pos, LW(10), LW(11), 0)
        IF_INT_LESS_OR_EQUAL(LW(11), FLOAT(0.0f))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_set_pose, STRING("M_D_2"))
    WAIT_FRAMES(120)
    BEGIN_SIBLING_EVENT
        SET_FLOAT_VALUE(LW(6), FLOAT(0.5f))
        SET_FLOAT_VALUE(LW(7), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(8), FLOAT(2.5f))
        SET_FLOAT_VALUE(LW(9), FLOAT(-0.34960938f))
        CALL(evt_party_get_pos, 0, LW(5), LW(6), LW(10))
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_A_1"))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_J_1A"))
        END_SWITCH
        LOOP(0)
            ADD_FLOAT(LW(5), LW(7))
            ADD_FLOAT(LW(6), LW(8))
            ADD_FLOAT(LW(8), LW(9))
            IF_INT_LESS_OR_EQUAL(LW(6), FLOAT(0.0f))
                SET_FLOAT_VALUE(LW(6), FLOAT(0.0f))
            END_IF
            CALL(evt_party_set_pos, 0, LW(5), LW(6), 0)
            IF_INT_LESS_OR_EQUAL(LW(6), FLOAT(0.0f))
                CALL(evt_party_set_pos, 0, LW(5), 0, 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_mario_set_pose, STRING("M_U_3"))
    WAIT_FRAMES(20)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING("PKR_S_1"))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING("PNK_S_1"))
    END_SWITCH
    WAIT_FRAMES(30)
    CALL(evt_mario_cont_onoff, 1)
    IF_INT_EQUAL(GSWF(1478), 1)
        CALL(evt_mario_set_dispdir, 180)
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_T_1"))
            CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_T_1"))
        END_SWITCH
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg1_gon_13"), 0, STRING("party"))
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg1_gon_14"), 0, STRING("party"))
        END_SWITCH
    END_IF
    WAIT_FRAMES(1)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF(1476), 0)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(gon_01_GATEEVENT_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_CASTLE1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(evt_cam3d_evt_set, -200, 525, 690, 350, 675, 37, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_telop_entry, 3)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -576, 27, 345, 310, 107, -6, 6000, 13)
    WAIT_MS(6700)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_mario_set_pos, -380, LW(11), 0)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_goodbye_party, 0)
        CALL(evt_mario_hello_party, 0, 1)
    END_IF
    CALL(evt_mario_set_pos, -400, LW(11), 0)
    CALL(evt_party_set_pos, 0, -435, LW(11), 0)
    CALL(evt_cam3d_evt_off, 3000, 4)
    WAIT_MS(3500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -335, 0, FLOAT(120.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -300, 0, FLOAT(120.0f))
    CALL(evt_party_stop, 0)
    WAIT_MS(500)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("stg1_gon_00"), 0, STRING("party"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("stg1_gon_01"), 0, STRING("party"))
    END_SWITCH
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_00", {0.0f, -1000.0f, 0.0f}},
    {"mod_01", {0.0f, -1000.0f, 0.0f}},
    {"mod_02", {0.0f, -1000.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {0.0f, -1000.0f, 0.0f}},
    {"mod_05", {0.0f, -1000.0f, 0.0f}},
    {"mod_06", {0.0f, -1000.0f, 0.0f}},
    {"mod_07", {0.0f, -1000.0f, 0.0f}},
    {"mod_08", {0.0f, -1000.0f, 0.0f}},
    {"a_hi", {0.0f, -1000.0f, 0.0f}},
    {"a_hi1", {0.0f, -1000.0f, 0.0f}},
    {"kabe_r_010", {385.0f, 0.0f, 0.0f}},
    {"polySurface810_q00097", {328.0f, 215.0f, 0.0f}},
    {"polySurface810_q00098", {-239.0f, 215.0f, 0.0f}},
    {"mod_09", {0.0f, -1000.0f, 0.0f}},
    {"mod_10", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(gon_01_init_evt)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON2"))
    CALL(evt_snd_set_rev_mode, 0)
    IF_INT_EQUAL(GSWF(1476), 1)
        SET_VALUE(LW(0), PTR(bero_entry_data))
        CALL(evt_bero_get_info)
        SPAWN_EVENT(PTR(bero_case_entry))
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SPAWN_EVENT(PTR(honenoko_oidasare_evt))
        ELSE
        SET_VALUE(LW(0), PTR(bero_entry_data))
        CALL(evt_bero_get_info)
        CALL_CHILD(PTR(evt_bero_info_run))
        IF_INT_LESS(GSW_STORY_PROGRESS, STG1_09_HOOKTAIL_CASTLE)
            SET_VALUE(GSW_STORY_PROGRESS, STG1_09_HOOKTAIL_CASTLE)
            SPAWN_EVENT(PTR(gon_01_GATEEVENT_evt))
        END_IF
    END_IF
    CALL(evt_item_entry, STRING("item00"), ITEM_HP_PLUS, 330, -150, 0, ITEM_TYPE_STILL, GSWF(1501), 0)
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_playanim, STRING("m_mizu01"), 1, 0)
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hi"))
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou1"), STRING("a_hi1"))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hasj"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kabe_l"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("kabe_r"), MAP_OBJ_FLAG_CLIP)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_09_02_LOCK)
        SET_VALUE(GSW_STORY_PROGRESS, STG1_09_02_LOCK)
    END_IF
    RETURN
EVT_END()


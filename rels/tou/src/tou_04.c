#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "evt/evt_npc.h"
#include "driver/msgdrv.h"
#include "memory.h"
#include "stdio.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "party/party_motion.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#include "manager/fontmgr.h"
#include "driver/seqdrv.h"

USER_FUNC(evt_tou_get_ranking);
USER_FUNC(evt_win_item_select);
USER_FUNC(kemuri_stop);
static USER_FUNC(make_list);
USER_FUNC(memoryRY);

static EvtWord w_door_open[12];
static EvtWord w_door_close[12];
EvtWord _1_door_open[12];
EvtWord _1_door_close[12];
EvtWord _2_door_open[12];
EvtWord _2_door_close[12];
EvtWord _3_door_open[12];
EvtWord _3_door_close[12];
EvtWord _4_door_open[12];
EvtWord _4_door_close[12];
EvtWord _5_door_open[12];
EvtWord _5_door_close[12];
EvtWord _6_door_open[12];
EvtWord _6_door_close[12];
EvtWord _7_door_open[12];
EvtWord _7_door_close[12];
static EvtWord e_door_open[12];
static EvtWord e_door_close[12];
static EvtWord n1_door_open[22];
static EvtWord n1_door_close[22];
static EvtWord n2_door_open[22];
static EvtWord n2_door_close[22];
static BeroEntry bero_data[12];
EvtWord move_g[233];
EvtWord move_g_1_dokimasen[25];
EvtWord init_g_1[59];
EvtWord move_g_1_dokimasu[51];
EvtWord talk_g_1[90];
EvtWord move_g_1[52];
EvtWord init_g_2[26];
EvtWord talk_g_2[59];
EvtWord move_g_2[56];
EvtWord init_g_3[30];
EvtWord talk_g_3[39];
EvtWord move_g_3[37];
EvtWord move_g_4_dokimasen[36];
EvtWord init_g_4[91];
EvtWord talk_g_4[271];
EvtWord move_g_4[81];
EvtWord init_g_5[2];
EvtWord talk_g_5[2];
EvtWord move_g_5[2];
EvtWord init_g_6[19];
EvtWord talk_g_6[31];
EvtWord move_g_6[30];
EvtWord init_g_7[8];
EvtWord talk_g_7[28];
EvtWord move_g_7[2];
EvtWord init_g_8[8];
EvtWord talk_g_8[39];
EvtWord move_g_8[2];
EvtWord init_g_9[8];
EvtWord talk_g_9[47];
EvtWord move_g_9[11];
EvtWord init_g_10[25];
EvtWord talk_g_10[25];
EvtWord init_g_11[29];
EvtWord talk_g_11[25];
static EvtWord init_champ[2];
static EvtWord init_brots[58];
static EvtWord talk_brots[149];
static NpcSetupInfo npcEnt[16];
EvtWord evt_drop[33];
EvtWord evt_gans[145];
static EvtWord evt_kinoshikowa[193];
EvtWord evt_kinoshikowa2[161];
EvtWord evt_kinoshikowa3[179];
static EvtWord evt_gardman[173];
EvtWord evt_gardman2[164];
EvtWord evt_gardman3[298];
EvtWord evt_gardman4[177];
static EvtWord evt_toile[107];
static EvtWord evt_toile2[107];
static s32 key_tbl[2];
static EvtWord evt_chk[23];
static EvtWord evt_lock[14];
EvtWord evt_souko_talk[272];
EvtWord evt_bred_nigeru[116];
EvtWord boss_msg[14];
EvtWord out_from_tou_10_re[22];
#ifdef VERSION_G8ME01
EvtWord tou_04_init_evt[652];
#else
EvtWord tou_04_init_evt[668];
#endif

USER_FUNC(memoryRY) {
    NpcEntry* npc = evtNpcNameToPtr(event, "me");

    npc->savedDirection = npc->direction;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_w"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_w"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_2"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_3_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_3"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_3_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_3"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_4_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_4"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_4_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_4"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_5_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_5"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_5_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_5"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_6_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_6"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_6_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_6"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_7_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_7"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(_7_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_7"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_e"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_e"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("ao_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ao_b"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("ao_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("ao_b"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("aka_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("aka_b"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING("aka_a"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("aka_b"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "w_bero", // hitName
        1, // type
        4, // sfxId
        4, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_02", // nextMapName
        "n_bero_4", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "s_bero_1", // hitName
        1, // type
        4, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_11", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        _1_door_close, // enterAnimeArg
        _1_door_open, // exitAnimeArg
    },
    {
        "s_bero_2", // hitName
        1, // type
        4, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_09", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        _2_door_close, // enterAnimeArg
        _2_door_open, // exitAnimeArg
    },
    {
        "s_bero_3", // hitName
        1, // type
        4, // sfxId
        0, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_07", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        _3_door_close, // enterAnimeArg
        _3_door_open, // exitAnimeArg
    },
    {
        "s_bero_4", // hitName
        1, // type
        4, // sfxId
        0, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_06", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        _4_door_close, // enterAnimeArg
        _4_door_open, // exitAnimeArg
    },
    {
        "s_bero_5", // hitName
        1, // type
        4, // sfxId
        0, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_05", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        _5_door_close, // enterAnimeArg
        _5_door_open, // exitAnimeArg
    },
    {
        "s_bero_6", // hitName
        1, // type
        4, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_08", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        _6_door_close, // enterAnimeArg
        _6_door_open, // exitAnimeArg
    },
    {
        "s_bero_7", // hitName
        1, // type
        4, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_10", // nextMapName
        "n_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        _7_door_close, // enterAnimeArg
        _7_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        4, // sfxId
        4, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_02", // nextMapName
        "n_bero_3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {
        "n_bero_1", // hitName
        1, // type
        6, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_03", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n1_door_close, // enterAnimeArg
        n1_door_open, // exitAnimeArg
    },
    {
        "n_bero_2", // hitName
        1, // type
        6, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_03", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n2_door_close, // enterAnimeArg
        n2_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(move_g)
    SET_VALUE(LF(0), 0)
    SWITCH(LW(0))
        CASE_EQUAL(3)
        SET_VALUE(LF(0), 1)
        CASE_EQUAL(4)
        SET_VALUE(LF(0), 1)
        CASE_EQUAL(5)
        SET_VALUE(LF(0), 1)
    END_SWITCH
    CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(10), -520)
        SET_VALUE(LW(11), 0)
        SET_VALUE(LW(12), 0)
        GOTO(1)
    END_IF
    LABEL(10)
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_sub_get_dist, LW(0), LW(2), LW(10), LW(12), LW(6))
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    IF_INT_GREATER(LW(6), 100)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("me"), LW(3), LW(4), LW(5))
        SET_VALUE(LW(6), LW(5))
        SET_VALUE(LW(7), LW(5))
        ADD(LW(6), 50)
        ADD(LW(7), -50)
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(6), LW(8))
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(7), LW(9))
        IF_INT_LESS(LW(8), LW(9))
            CALL(evt_npc_move_position, STRING("me"), LW(3), LW(7), 0, FLOAT(100.0f), 1)
            ELSE
            CALL(evt_npc_move_position, STRING("me"), LW(3), LW(6), 0, FLOAT(100.0f), 1)
        END_IF
        ELSE
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("me"), LW(3), LW(4), LW(5))
        SET_VALUE(LW(6), LW(3))
        SET_VALUE(LW(7), LW(3))
        ADD(LW(6), 50)
        ADD(LW(7), -50)
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(6), LW(5), LW(8))
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(7), LW(5), LW(9))
        IF_INT_LESS(LW(8), LW(9))
            CALL(evt_npc_move_position, STRING("me"), LW(7), LW(5), 0, FLOAT(100.0f), 1)
            ELSE
            CALL(evt_npc_move_position, STRING("me"), LW(6), LW(5), 0, FLOAT(100.0f), 1)
        END_IF
    END_IF
    LABEL(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_sub_get_dist, LW(0), LW(2), LW(10), LW(12), LW(6))
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    IF_INT_LESS(LW(6), 100)
        WAIT_FRAMES(1)
        GOTO(1)
    END_IF
    CALL(evt_npc_move_position, STRING("me"), LW(10), LW(12), 0, FLOAT(100.0f), 1)
    GOTO(10)
    RETURN
EVT_END()

EVT_BEGIN(move_g_1_dokimasen)
    CALL(evt_npc_set_position, STRING("me"), -580, 0, 350)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_move_position, STRING("me"), -580, 300, 0, FLOAT(80.0f), 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(init_g_1)
    IF_INT_EQUAL(GSWF(2379), 0)
        CALL(evt_npc_set_position, STRING("me"), -580, 0, 300)
        ELSE
        CALL(evt_npc_set_position, STRING("me"), -620, 0, 300)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 141)
        CALL(evt_npc_set_position, STRING("me"), -580, 0, 350)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 142)
        CALL(evt_npc_set_position, STRING("me"), -580, 0, 350)
    END_IF
    IF_INT_EQUAL(GSWF(2517), 0)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 143)
            CALL(evt_bero_get_entername, LW(0))
            IF_STR_EQUAL(LW(0), STRING("s_bero_7"))
                SET_VALUE(GSWF(2517), 1)
                SPAWN_EVENT(PTR(move_g_1_dokimasen))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(move_g_1_dokimasu)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
    IF_INT_LESS(LW(2), LW(12))
        ADD(LW(12), 50)
        ELSE
        ADD(LW(12), -50)
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("me"), LW(10), LW(12), 0, FLOAT(80.0f), 1)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    SET_VALUE(LW(0), STRING("s_bero_7"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, 141)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(133)
        CALL(evt_msg_print, 0, STRING("stg3_tou_39"), 0, 0)
        CASE_BETWEEN(140, 142)
        CALL(evt_msg_print, 0, STRING("stg3_tou_297_13"), 0, 0)
        IF_INT_LESS(GSW_STORY_PROGRESS, 141)
            CALL_CHILD(PTR(move_g_1_dokimasu))
        END_IF
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_96_01"), 0, 0)
        CASE_LESS(172)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 10)
            CALL(evt_msg_print, 0, STRING("stg3_tou_96"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_95"), 0, 0)
        END_IF
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_39"), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 10)
                CALL(evt_msg_print, 0, STRING("stg3_tou_96"), 0, 0)
                ELSE
                CALL(evt_msg_print, 0, STRING("stg3_tou_95"), 0, 0)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_1)
    IF_INT_EQUAL(GSWF(2379), 1)
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        IF_INT_EQUAL(GSWF(2376), 1)
            SET_VALUE(LW(0), 0)
            CALL_CHILD(PTR(move_g))
        END_IF
        CASE_LESS(172)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_GREATER(LW(0), 10)
            SET_VALUE(LW(0), 0)
            CALL_CHILD(PTR(move_g))
        END_IF
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 1)
            CALL(evt_tou_get_ranking, LW(0))
            IF_INT_GREATER(LW(0), 10)
                SET_VALUE(LW(0), 0)
                CALL_CHILD(PTR(move_g))
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_g_2)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 163)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("n_bero_2"))
            CALL(evt_npc_set_position, STRING("me"), -520, 0, 70)
            RETURN
        END_IF
    END_IF
    CALL(evt_npc_set_position, STRING("me"), -520, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(133)
        CALL(evt_msg_print, 0, STRING("stg3_tou_38"), 0, 0)
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_94_01"), 0, 0)
        CASE_EQUAL(157)
        IF_INT_EQUAL(GSWF(2399), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_423_01"), 0, 0)
            CALL(evt_mario_set_pose, STRING("M_I_Y"))
            CALL(evt_msg_print, 0, STRING("stg3_tou_423_02"), 0, 0)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_94"), 0, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg3_tou_94"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_2)
    IF_INT_EQUAL(GSWF(2379), 1)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(move_g))
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2380), 1)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(move_g))
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2382), 1)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(move_g))
        RETURN
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 157)
        IF_INT_EQUAL(GSWF(2399), 0)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(PTR(move_g))
            RETURN
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 163)
        SET_VALUE(LW(0), 1)
        CALL_CHILD(PTR(move_g))
        RETURN
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_g_3)
    IF_INT_EQUAL(GSWF(2380), 1)
        CALL(evt_npc_set_position, STRING("me"), -620, 0, -300)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2381), 1)
        CALL(evt_npc_set_position, STRING("me"), -620, 0, -300)
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("me"), -580, 0, -300)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_3)
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_LESS(1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_99"), 0, 0)
        CASE_LESS(11)
        CALL(evt_msg_print, 0, STRING("stg3_tou_98"), 0, 0)
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_99_01"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg3_tou_40"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_3)
    IF_INT_EQUAL(GSWF(2380), 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2381), 1)
        RETURN
    END_IF
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_LESS(1)
        IF_INT_EQUAL(GSWF(2378), 1)
            SET_VALUE(LW(0), 2)
            CALL_CHILD(PTR(move_g))
        END_IF
        CASE_LESS(11)
        SET_VALUE(LW(0), 2)
        CALL_CHILD(PTR(move_g))
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_4_dokimasen)
    CALL(evt_npc_set_position, STRING("me"), -300, 0, -530)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_move_position, STRING("me"), -350, -530, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_set_position, STRING("me"), -350, 0, -530)
    SET_VALUE(LW(0), 5)
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(init_g_4)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
        CALL(evt_npc_set_position, STRING("me"), -350, 0, -530)
        ELSE
        SET_VALUE(LF(0), 0)
        SET_VALUE(LF(1), 0)
        SET_VALUE(LF(2), 0)
        SWITCH(GSW(1420))
            CASE_EQUAL(13)
            SET_VALUE(LF(0), 1)
            CASE_EQUAL(23)
            SET_VALUE(LF(0), 1)
        END_SWITCH
        IF_INT_EQUAL(LF(0), 1)
            SET_VALUE(LF(1), GSWF(2499))
            ELSE
            IF_INT_EQUAL(GSWF(2500), 0)
                SET_VALUE(LF(1), GSWF(2498))
                ELSE
                CALL(evt_bero_get_entername, LW(0))
                IF_STR_EQUAL(LW(0), STRING("s_bero_5"))
                    SET_VALUE(LF(1), 1)
                    SET_VALUE(LF(2), 1)
                    ELSE
                    SET_VALUE(LF(1), 0)
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(LF(1), 0)
            CALL(evt_npc_set_position, STRING("me"), -350, 0, -530)
            ELSE
            CALL(evt_npc_set_position, STRING("me"), -300, 0, -530)
            IF_INT_EQUAL(LF(2), 1)
                SPAWN_EVENT(PTR(move_g_4_dokimasen))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(talk_g_4)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(130)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("me"), LW(3), LW(4), LW(5))
        IF_INT_EQUAL(LW(3), -350)
            CALL(evt_msg_print, 0, STRING("stg3_tou_42"), 0, 0)
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
            CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
            IF_INT_LESS(LW(0), LW(3))
                ADD(LW(3), 50)
                CALL(evt_npc_move_position, STRING("me"), LW(3), LW(5), 0, FLOAT(80.0f), 1)
                CALL(evt_npc_set_ry, STRING("me"), 270)
                ELSE
                ADD(LW(3), -50)
                CALL(evt_npc_move_position, STRING("me"), LW(3), LW(5), 0, FLOAT(80.0f), 1)
                CALL(evt_npc_set_ry, STRING("me"), 90)
            END_IF
            CALL(memoryRY)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
            SET_VALUE(LW(0), 5)
            CALL_CHILD(PTR(bero_case_switch_on))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_42_01"), 0, 0)
        END_IF
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING("stg3_tou_42_01"), 0, 0)
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_102_02"), 0, 0)
        CASE_LESS(172)
        CALL(evt_msg_print, 0, STRING("stg3_tou_102_01"), 0, 0)
        CASE_DEFAULT
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LW(0), -350)
            SWITCH(GSW(1420))
                CASE_EQUAL(13)
                CALL(evt_msg_print, 0, STRING("stg3_tou_re_311"), 0, 0)
                SET_VALUE(GSWF(2499), 1)
                CASE_EQUAL(23)
                CALL(evt_msg_print, 0, STRING("stg3_tou_re_311"), 0, 0)
                SET_VALUE(GSWF(2499), 1)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2500), 0)
                    CALL(evt_msg_print, 0, STRING("stg3_tou_re_310"), 0, 0)
                    SET_VALUE(GSWF(2498), 1)
                    ELSE
                    CALL(evt_msg_print, 0, STRING("stg3_tou_re_314"), 0, 0)
                    RETURN
                END_IF
            END_SWITCH
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(3), LW(4), LW(5))
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
            CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
            IF_INT_LESS(LW(0), LW(3))
                ADD(LW(3), 50)
                CALL(evt_npc_move_position, STRING("me"), LW(3), LW(5), 0, FLOAT(80.0f), 1)
                CALL(evt_npc_set_ry, STRING("me"), 270)
                ELSE
                ADD(LW(3), -50)
                CALL(evt_npc_move_position, STRING("me"), LW(3), LW(5), 0, FLOAT(80.0f), 1)
                CALL(evt_npc_set_ry, STRING("me"), 90)
            END_IF
            CALL(memoryRY)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
            SET_VALUE(LW(0), 5)
            CALL_CHILD(PTR(bero_case_switch_on))
            ELSE
            SWITCH(GSW(1420))
                CASE_EQUAL(13)
                CALL(evt_msg_print, 0, STRING("stg3_tou_re_313"), 0, 0)
                CASE_EQUAL(23)
                CALL(evt_msg_print, 0, STRING("stg3_tou_re_313"), 0, 0)
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg3_tou_re_312"), 0, 0)
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_4)
    IF_INT_EQUAL(GSWF(2375), 1)
        SET_VALUE(LW(0), 3)
        CALL_CHILD(PTR(move_g))
    END_IF
    IF_INT_EQUAL(GSWF(2376), 1)
        SET_VALUE(LW(0), 3)
        CALL_CHILD(PTR(move_g))
    END_IF
    IF_INT_EQUAL(GSWF(2377), 1)
        SET_VALUE(LW(0), 3)
        CALL_CHILD(PTR(move_g))
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(130)
        CASE_LESS(133)
        SET_VALUE(LW(0), 3)
        CALL_CHILD(PTR(move_g))
        CASE_EQUAL(162)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("s_bero_5"))
            CALL(evt_npc_set_position, STRING("me"), -400, 0, -530)
            CALL(evt_bero_exec_wait, 65536)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_npc_move_position, STRING("me"), -350, -530, 0, FLOAT(80.0f), 1)
            CALL(evt_mario_key_onoff, 1)
        END_IF
        CASE_BETWEEN(163, 171)
        SET_VALUE(LW(0), 3)
        CALL_CHILD(PTR(move_g))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_g_5)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_5)
    RETURN
EVT_END()

EVT_BEGIN(move_g_5)
    RETURN
EVT_END()

EVT_BEGIN(init_g_6)
    IF_INT_EQUAL(GSWF(2382), 1)
        CALL(evt_npc_set_position, STRING("me"), 350, 0, -560)
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("me"), 350, 0, -530)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_6)
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_LESS(1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_101"), 0, 0)
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_102_00"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg3_tou_102"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_6)
    IF_INT_EQUAL(GSWF(2382), 1)
        RETURN
    END_IF
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_LESS(1)
        SET_VALUE(LW(0), 5)
        CALL_CHILD(PTR(move_g))
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2375), 1)
            SET_VALUE(LW(0), 5)
            CALL_CHILD(PTR(move_g))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_g_7)
    CALL(evt_npc_set_position, STRING("me"), 580, 0, -300)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_7)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(133)
        CALL(evt_msg_print, 0, STRING("stg3_tou_40"), 0, 0)
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_100_01"), 0, 0)
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg3_tou_100"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_7)
    RETURN
EVT_END()

EVT_BEGIN(init_g_8)
    CALL(evt_npc_set_position, STRING("me"), 520, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_8)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(133)
        CALL(evt_msg_print, 0, STRING("stg3_tou_38"), 0, 0)
        CASE_EQUAL(157)
        IF_INT_EQUAL(GSWF(2399), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_423_03"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_94"), 0, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg3_tou_94"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_8)
    RETURN
EVT_END()

EVT_BEGIN(init_g_9)
    CALL(evt_npc_set_position, STRING("me"), 580, 0, 300)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_9)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(133)
        CALL(evt_msg_print, 0, STRING("stg3_tou_39"), 0, 0)
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_97_01"), 0, 0)
        CASE_LESS(172)
        CALL(evt_msg_print, 0, STRING("stg3_tou_97"), 0, 0)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_39"), 0, 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg3_tou_97"), 0, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(move_g_9)
    IF_INT_EQUAL(GSWF(2381), 1)
        SET_VALUE(LW(0), 8)
        CALL_CHILD(PTR(move_g))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_g_10)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(157)
        IF_INT_EQUAL(GSWF(2399), 0)
            CALL(evt_npc_set_position, STRING("me"), -550, 0, 530)
        END_IF
        CASE_EQUAL(163)
        CALL(evt_npc_set_position, STRING("me"), -550, 0, 530)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_g_10)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(157)
        IF_INT_EQUAL(GSWF(2399), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_102_03"), 0, STRING("me"))
        END_IF
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_102_04"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_g_11)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(157)
        IF_INT_EQUAL(GSWF(2399), 0)
            CALL(evt_npc_set_position, STRING("me"), 550, 0, 530)
        END_IF
        CASE_EQUAL(163)
        CALL(evt_npc_set_position, STRING("me"), 550, 0, 530)
    END_SWITCH
    CALL(evt_npc_set_ry, STRING("me"), 0)
    RETURN
EVT_END()

EVT_BEGIN(talk_g_11)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(157)
        IF_INT_EQUAL(GSWF(2399), 0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_102_03"), 0, STRING("me"))
        END_IF
        CASE_EQUAL(163)
        CALL(evt_msg_print, 0, STRING("stg3_tou_102_04"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_champ)
    RETURN
EVT_END()

static EVT_BEGIN(init_brots)
    IF_INT_LESS(GSW_STORY_PROGRESS, 172)
        IF_INT_EQUAL(GSWF(2421), 0)
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_LESS(5)
                CASE_LESS(19)
                CALL(evt_sub_random, 4, LW(1))
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    CALL(evt_npc_set_position, STRING("me"), -525, 0, 225)
                    CASE_EQUAL(1)
                    CALL(evt_npc_set_position, STRING("me"), -175, 0, -475)
                    CASE_EQUAL(2)
                    CALL(evt_npc_set_position, STRING("me"), 525, 0, -175)
                    CASE_DEFAULT
                    CALL(evt_npc_set_position, STRING("me"), 525, 0, 425)
                END_SWITCH
            END_SWITCH
        END_IF
    END_IF
    RETURN
EVT_END()

static USER_FUNC(make_list) {
    static char* tmp;
    static const char* list[] = {
        "stg3_tou_hushigi_list_1",
        "stg3_tou_hushigi_list_2",
        "stg3_tou_hushigi_list_3",
        "stg3_tou_hushigi_list_4",
        "stg3_tou_hushigi_list_5",
        "stg3_tou_hushigi_list_6",
        "stg3_tou_hushigi_list_7",
    };
    EvtWord* args = event->args;
    s32 i;

    if (tmp == NULL) {
        tmp = _mapAlloc(0x1000);
    }
#ifdef VERSION_G8ME01
    {
        s32 maxw = 0;
        for (i = 0; i < 7; i++) {
            s32 w = (u16)FontGetMessageWidth(msgSearch(list[i]));
            if (maxw < w) {
                maxw = w;
            }
        }
        sprintf(tmp, "<select %d %d %d %d>", 0, 7, maxw + 80, 140);
    }
#else
    sprintf(tmp, "<select %d %d %d %d>", 0, 7, 300, 140);
#endif
    for (i = 0; i < 7; i++) {
        strcat(tmp, "\n");
        strcat(tmp, msgSearch(list[i]));
    }
    evtSetValue(event, args[0], (EvtWord)tmp);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(talk_brots)
    IF_INT_EQUAL(GSWF(2444), 0)
        CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_00"), 0, 0)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(700)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_01"), 0, 0)
        CALL(evt_mario_set_pose, STRING("M_N_B"))
        WAIT_MS(700)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_02"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_03"), 0, 0)
    END_IF
    CALL(evt_msg_select, 0, STRING("stg3_tou_hushigi_02_yn"))
    IF_INT_EQUAL(LW(0), 1)
        IF_INT_EQUAL(GSWF(2444), 0)
            CALL(evt_msg_print_add, 0, STRING("stg3_tou_hushigi_06"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("stg3_tou_hushigi_07"))
        END_IF
        ELSE
        CALL(evt_msg_print_add, 0, STRING("stg3_tou_hushigi_05"))
        CALL(make_list, LW(0))
        CALL(evt_msg_select, 1, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_11"), 0, 0)
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_12"), 0, 0)
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_13"), 0, 0)
            CASE_EQUAL(3)
            CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_14"), 0, 0)
            CASE_EQUAL(4)
            CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_15"), 0, 0)
            CASE_EQUAL(5)
            CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_16"), 0, 0)
            CASE_EQUAL(6)
            CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_17"), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg3_tou_hushigi_08"), 0, 0)
        END_SWITCH
    END_IF
    SET_VALUE(GSWF(2444), 1)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"キノシコワ"*/"\x83\x4C\x83\x6D\x83\x56\x83\x52\x83\x8F", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガンス"*/"\x83\x4B\x83\x93\x83\x58", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_champ, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブロッツ"*/"\x83\x75\x83\x8D\x83\x62\x83\x63", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_brots, // initEvent
        NULL, // regularEvent
        talk_brots, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン１"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x50", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_1, // initEvent
        move_g_1, // regularEvent
        talk_g_1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン２"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_2, // initEvent
        move_g_2, // regularEvent
        talk_g_2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン３"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_3, // initEvent
        move_g_3, // regularEvent
        talk_g_3, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン４"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x53", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_4, // initEvent
        move_g_4, // regularEvent
        talk_g_4, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン５"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x54", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_5, // initEvent
        move_g_5, // regularEvent
        talk_g_5, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン６"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x55", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_6, // initEvent
        move_g_6, // regularEvent
        talk_g_6, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン７"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x56", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_7, // initEvent
        move_g_7, // regularEvent
        talk_g_7, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン８"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x57", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_8, // initEvent
        move_g_8, // regularEvent
        talk_g_8, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン９"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x58", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_9, // initEvent
        move_g_9, // regularEvent
        talk_g_9, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン１０"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x50\x82\x4F", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_10, // initEvent
        NULL, // regularEvent
        talk_g_10, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン１１"*/"\x83\x4B\x81\x5B\x83\x68\x83\x7D\x83\x93\x82\x50\x82\x50", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_RESUME_AFTER_TALK | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_g_11, // initEvent
        NULL, // regularEvent
        talk_g_11, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_drop)
    LOOP(0)
        CALL(evt_sub_random, 5000, LW(0))
        ADD(LW(0), 1000)
        WAIT_MS(LW(0))
        CALL(evt_map_playanim, STRING("anm_mizu"), 0, 0)
        CALL(evt_map_checkanim, STRING("anm_mizu"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AMB_WATERDROP1"), 500, 0, -405, 0)
    END_LOOP
    RETURN
EVT_END()

USER_FUNC(kemuri_stop) {
    return EVT_RETURN_DONE;
}

EVT_BEGIN(evt_gans)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("ガンス"), -350, 0, -570)
    CALL(evt_mario_set_pos, -350, 0, -580)
    CALL(evt_party_set_pos, 0, -350, 0, -600)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 5)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_5_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        SET_VALUE(LW(1), 5)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(_5_door_open))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_mov_pos2, -350, -500, FLOAT(90.0f))
        CALL(evt_mario_mov_pos2, 310, -500, FLOAT(90.0f))
    END_INLINE_EVENT
    CALL(evt_npc_status_onoff, 1, STRING("ガンス"), 2)
    CALL(evt_npc_move_position, STRING("ガンス"), -350, -500, 0, FLOAT(90.0f), 1)
    CALL(evt_npc_move_position, STRING("ガンス"), 320, -500, 0, FLOAT(90.0f), 1)
    CALL(evt_npc_move_position, STRING("ガンス"), 350, -530, 0, FLOAT(90.0f), 1)
    SET_VALUE(LW(1), 3)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_3_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_bero_mapchange, STRING("tou_07"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kinoshikowa)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -350, 0, -570)
    CALL(evt_mario_set_pos, -350, 0, -580)
    CALL(evt_party_set_pos, 0, -350, 0, -590)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 5)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_5_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        SET_VALUE(LW(1), 5)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(_5_door_open))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_mov_pos2, -350, -505, FLOAT(75.0f))
        CALL(evt_mario_mov_pos2, -550, -505, FLOAT(75.0f))
        CALL(evt_mario_mov_pos2, -550, 250, FLOAT(75.0f))
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_msg_print, 0, STRING("stg3_tou_62"), 0, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -350, -470, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -550, -470, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -550, 270, 0, FLOAT(75.0f), 1)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_move_position, STRING("キノシコワ"), -585, 290, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("M_1"))
    WAIT_MS(600)
    SET_VALUE(LW(1), 7)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_7_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GSWF(2376), 0)
    CALL(evt_bero_mapchange, STRING("tou_10"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_kinoshikowa2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -350, 0, -570)
    CALL(evt_mario_set_pos, -350, 0, -580)
    CALL(evt_party_set_pos, 0, -350, 0, -590)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 5)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_5_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        SET_VALUE(LW(1), 5)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(_5_door_open))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_mov_pos2, -350, -505, FLOAT(75.0f))
        CALL(evt_mario_mov_pos2, -550, -505, FLOAT(75.0f))
        CALL(evt_mario_mov_pos2, -550, -350, FLOAT(75.0f))
    END_INLINE_EVENT
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -350, -470, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -550, -470, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -550, -333, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -585, -290, 0, FLOAT(75.0f), 1)
    SET_VALUE(LW(1), 6)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_6_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GSWF(2377), 0)
    CALL(evt_bero_mapchange, STRING("tou_08"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_kinoshikowa3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_set_pos, -625, 0, -300)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -620, 0, -300)
    CALL(evt_npc_set_ry, STRING("キノシコワ"), 270)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 6)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_6_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        SET_VALUE(LW(1), 6)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL_CHILD(PTR(_6_door_open))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_mario_mov_pos2, -550, -301, FLOAT(75.0f))
        CALL(evt_mario_mov_pos2, -550, -500, FLOAT(75.0f))
        CALL(evt_mario_mov_pos2, 300, -500, FLOAT(75.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_party_move_pos2, 0, -550, -301, FLOAT(75.0f))
    END_INLINE_EVENT
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -550, -301, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -550, -500, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 340, -500, 0, FLOAT(75.0f), 1)
    CALL(evt_npc_move_position, STRING("キノシコワ"), 340, -530, 0, FLOAT(75.0f), 1)
    SET_VALUE(LW(1), 3)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_3_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GSWF(2378), 0)
    CALL(evt_bero_mapchange, STRING("tou_07"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_gardman)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 7)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_7_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("ガードマン１"), 2)
        CALL(evt_npc_move_position, STRING("ガードマン１"), -550, 300, 0, FLOAT(60.0f), 1)
        CALL(evt_npc_move_position, STRING("ガードマン１"), -550, -35, 0, FLOAT(60.0f), 1)
        CALL(evt_set_dir_to_target, STRING("ガードマン１"), STRING("mario"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -550, 300, FLOAT(60.0f))
        CALL(evt_mario_mov_pos2, -550, 0, FLOAT(60.0f))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_party_move_pos2, 0, -550, 300, FLOAT(60.0f))
    SET_VALUE(LW(1), 7)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_7_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg3_tou_76"), 0, STRING("ガードマン１"))
    SET_VALUE(LW(1), 10)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n2_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -400, 0, FLOAT(60.0f))
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(GSWF(2379), 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(132)
        SET_VALUE(GSW_STORY_PROGRESS, 132)
        CASE_DEFAULT
    END_SWITCH
    CALL(evt_bero_mapchange, STRING("tou_03"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_gardman2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 6)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_6_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("ガードマン３"), 2)
        CALL(evt_npc_move_position, STRING("ガードマン３"), -550, -300, 0, FLOAT(60.0f), 1)
        CALL(evt_npc_move_position, STRING("ガードマン３"), -550, 35, 0, FLOAT(60.0f), 1)
        CALL(evt_set_dir_to_target, STRING("ガードマン３"), STRING("mario"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -550, -300, FLOAT(60.0f))
        CALL(evt_mario_mov_pos2, -550, 0, FLOAT(60.0f))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_party_move_pos2, 0, -550, -300, FLOAT(60.0f))
    SET_VALUE(LW(1), 6)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_6_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg3_tou_328"), 0, STRING("ガードマン３"))
    SET_VALUE(LW(1), 10)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n2_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -400, 0, FLOAT(60.0f))
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(GSWF(2380), 0)
    CALL(evt_bero_mapchange, STRING("tou_03"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_gardman3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 6)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_6_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("ガードマン３"), 2)
        CALL(evt_npc_move_position, STRING("ガードマン３"), -550, -300, 0, FLOAT(60.0f), 1)
        CALL(evt_npc_move_position, STRING("ガードマン３"), -550, -400, 0, FLOAT(60.0f), 1)
    END_INLINE_EVENT
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -550, -300, FLOAT(60.0f))
        CALL(evt_mario_set_dir_npc, STRING("ガードマン３"))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_party_move_pos2, 0, -550, -300, FLOAT(60.0f))
    CALL(evt_party_move_pos2, 0, -550, -270, FLOAT(60.0f))
    CALL(evt_party_set_dir_npc, 0, STRING("ガードマン３"))
    SET_VALUE(LW(1), 6)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_6_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_407"))
    WAIT_MS(300)
    CALL(evt_party_run, 0)
    CALL(evt_set_dir_to_target, STRING("ガードマン３"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_408"), 0, STRING("ガードマン３"))
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン４"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン５"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン６"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン７"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン８"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ガードマン９"), NPC_FLAG_NO_HIT)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_sub_get_dist, LW(0), LW(2), 0, -500, LW(10))
            IF_INT_LESS(LW(10), 100)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print_party, STRING("stg3_tou_408_01"))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ガードマン３"), -550, -520, 0, FLOAT(90.0f), 1)
        CALL(evt_npc_move_position, STRING("ガードマン３"), 550, -520, 0, FLOAT(90.0f), 1)
        CALL(evt_npc_move_position, STRING("ガードマン３"), 570, 280, 0, FLOAT(90.0f), 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -550, -515, FLOAT(90.0f))
    CALL(evt_mario_mov_pos2, 550, -515, FLOAT(90.0f))
    CALL(evt_mario_mov_pos2, 570, 250, FLOAT(90.0f))
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(LW(1), 1)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_1_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ガードマン３"), 590, 300, 0, FLOAT(60.0f), 1)
        CALL(evt_mario_mov_pos2, 590, 300, FLOAT(90.0f))
    END_INLINE_EVENT
    SET_VALUE(GSWF(2381), 0)
    CALL(evt_bero_mapchange, STRING("tou_11"), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_gardman4)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 3)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_3_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("ガードマン６"), 2)
        CALL(evt_npc_move_position, STRING("ガードマン６"), 350, -510, 0, FLOAT(60.0f), 1)
        CALL(evt_npc_move_position, STRING("ガードマン６"), -550, -510, 0, FLOAT(60.0f), 1)
        CALL(evt_npc_move_position, STRING("ガードマン６"), -550, 35, 0, FLOAT(60.0f), 1)
        CALL(evt_set_dir_to_target, STRING("ガードマン６"), STRING("mario"))
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, 350, -510, FLOAT(60.0f))
        CALL(evt_mario_mov_pos2, -550, -510, FLOAT(60.0f))
        CALL(evt_mario_mov_pos2, -550, 0, FLOAT(60.0f))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_party_move_pos2, 0, 350, -510, FLOAT(60.0f))
    SET_VALUE(LW(1), 3)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(_3_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_msg_print, 0, STRING("stg3_tou_454"), 0, STRING("ガードマン６"))
    SET_VALUE(LW(1), 10)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(n2_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -400, 0, FLOAT(60.0f))
    END_INLINE_EVENT
    WAIT_MS(1000)
    SET_VALUE(GSWF(2382), 0)
    CALL(evt_bero_mapchange, STRING("tou_03"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_toile)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 450, 133, -1033, 450, 37, -492, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_DRAINING1"), 450, 0, -500, LW(3))
    BEGIN_INLINE_EVENT_GET_ID(LW(4))
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 450, 450, 5000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_snd_sfx_pos, LW(3), LW(0), 0, -500)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, -450, 133, -1033, -450, 37, -492, 5000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(333)
        CALL(evt_eff_fukidashi, 1, STRING("a"), STRING("ガードマン６"), 1, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(4000)
        CALL(evt_eff_fukidashi, 1, STRING("b"), STRING("ガードマン４"), 1, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(5000)
    DELETE_EVENT(LW(4))
    CALL(evt_snd_sfxoff, LW(3))
    CALL(evt_bero_mapchange, STRING("tou_08"), STRING("dokan"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_toile2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, -450, 133, -1033, -450, 37, -492, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_DRAINING1"), -450, 0, -500, LW(3))
    BEGIN_INLINE_EVENT_GET_ID(LW(4))
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, -450, 450, 5000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_snd_sfx_pos, LW(3), LW(0), 0, -500)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, 450, 133, -1033, 450, 37, -492, 5000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(333)
        CALL(evt_eff_fukidashi, 1, STRING("a"), STRING("ガードマン４"), 1, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(4000)
        CALL(evt_eff_fukidashi, 1, STRING("b"), STRING("ガードマン６"), 1, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(5000)
    DELETE_EVENT(LW(4))
    CALL(evt_snd_sfxoff, LW(3))
    CALL(evt_bero_mapchange, STRING("tou_09"), STRING("dokan"))
    RETURN
EVT_END()

static s32 key_tbl[] = {
    ITEM_STORAGE_KEY_0x12, -1,
};

static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, ITEM_STORAGE_KEY_0x12, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_lock)
    SET_VALUE(GSW_STORY_PROGRESS, 147)
    SET_VALUE(LW(0), STRING("s_bero_4"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_pouch_remove_item, ITEM_STORAGE_KEY_0x12, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_souko_talk)
    IF_INT_EQUAL(GSWF(2445), 1)
        RETURN
    END_IF
    CALL(evt_npc_set_position, STRING("キノシコワ"), 30, 0, -500)
    CALL(evt_npc_set_position, STRING("ガードマン５"), -30, 0, -500)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("ガードマン５"))
    CALL(evt_set_dir_to_target, STRING("ガードマン５"), STRING("キノシコワ"))
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), 2)
    CALL(evt_npc_status_onoff, 1, STRING("ガードマン５"), 2)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER(LW(0), -140)
            IF_INT_LESS(LW(0), 140)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_cam3d_evt_set, 0, 133, -1033, 0, 47, -490, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_240_02"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_240_03"), 0, STRING("ガードマン５"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_240_04"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), 0)
        CALL(evt_npc_move_position, STRING("ガードマン５"), 200, -490, 0, FLOAT(80.0f), 1)
        ELSE
        CALL(evt_npc_move_position, STRING("ガードマン５"), -200, -490, 0, FLOAT(80.0f), 1)
    END_IF
    CALL(evt_npc_set_position, STRING("ガードマン５"), 0, -1000, 0)
    CALL(evt_npc_reverse_ry, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("キノシコワ"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), 0)
        SET_VALUE(LW(0), -40)
        ELSE
        SET_VALUE(LW(0), 40)
    END_IF
    CALL(evt_cam3d_evt_set, LW(0), 133, -1033, LW(0), 47, -490, 1000, 11)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), 0)
        ADD(LW(0), 30)
        ELSE
        SUBTRACT(LW(0), 30)
    END_IF
    CALL(evt_npc_move_position, STRING("キノシコワ"), LW(0), -500, 0, FLOAT(60.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_240_05"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(0), 0)
        CALL(evt_npc_move_position, STRING("キノシコワ"), 200, -500, 0, FLOAT(60.0f), 1)
        ELSE
        CALL(evt_npc_move_position, STRING("キノシコワ"), -200, -500, 0, FLOAT(60.0f), 1)
    END_IF
    CALL(evt_npc_set_position, STRING("キノシコワ"), 0, -1000, 0)
    SET_VALUE(GSWF(2445), 1)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_bred_nigeru)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_status_onoff, 1, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"), 2)
    CALL(evt_npc_set_position, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"), -550, 0, -400)
    CALL(evt_set_dir_to_target, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"), STRING("mario"))
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_eff_fukidashi, 1, 0, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mario_set_dir, 270, 200, 0)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_297_27_00"), 0, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"))
    WAIT_MS(300)
    CALL(evt_npc_sound_data_set, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"), EVT_NULLPTR, EVT_NULLPTR, 20000, EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_npc_move_position, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"), -550, -500, 0, FLOAT(200.0f), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"), 0, -500, 0, FLOAT(200.0f), 1)
        CALL(evt_npc_set_position, STRING(/*"チャンピョン"*/"\x83\x60\x83\x83\x83\x93\x83\x73\x83\x87\x83\x93"), 0, -1000, 0)
    END_INLINE_EVENT
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_297_27_01"))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG3_TOU1"), 2000)
    SET_VALUE(GSW_STORY_PROGRESS, 145)
    RETURN
EVT_END()

EVT_BEGIN(boss_msg)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg3_tou_462_01"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(out_from_tou_10_re)
    CALL(evt_npc_set_position, STRING("ガードマン１"), -590, 0, 350)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_npc_move_position, STRING("ガードマン１"), -590, 300, 0, FLOAT(80.0f), 1)
    RETURN
EVT_END()

EVT_BEGIN(tou_04_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_cam_road_reset)
    SPAWN_EVENT(PTR(evt_drop))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_item_entry, STRING("item_01"), ITEM_LAST_STAND, 515, 15, 125, 16, GSWF(2520), 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save1"), -520, 60, -110, 0, 0)
    CALL(evt_mobj_save_blk, STRING("mobj_save3"), 450, 60, -465, 0, 0)
    SET_VALUE(LW(1), 0)
    IF_INT_EQUAL(GSWF(2375), 1)
        CALL(kemuri_stop)
        CALL(evt_mario_set_pos, -350, 0, -580)
        CALL(evt_party_set_pos, 0, -350, 0, -600)
        SET_VALUE(LW(1), 1)
        SPAWN_EVENT(PTR(evt_gans))
    END_IF
    IF_INT_EQUAL(GSWF(2376), 1)
        CALL(kemuri_stop)
        CALL(evt_mario_set_pos, -390, 0, -510)
        CALL(evt_party_set_pos, 0, -390, 0, -510)
        SPAWN_EVENT(PTR(evt_kinoshikowa))
    END_IF
    IF_INT_EQUAL(GSWF(2377), 1)
        CALL(kemuri_stop)
        CALL(evt_mario_set_pos, -390, 0, -510)
        CALL(evt_party_set_pos, 0, -390, 0, -510)
        SPAWN_EVENT(PTR(evt_kinoshikowa2))
    END_IF
    IF_INT_EQUAL(GSWF(2378), 1)
        CALL(kemuri_stop)
        CALL(evt_mario_set_pos, -625, 0, -300)
        CALL(evt_party_set_pos, 0, -630, 0, -300)
        SPAWN_EVENT(PTR(evt_kinoshikowa3))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 163)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envoff, PSND_FADEOUT)
        ELSE
        IF_INT_EQUAL(LW(1), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_BETWEEN(127, 137)
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU4"))
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
                CASE_GREATER_OR_EQUAL(165)
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU4"))
                CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
                CASE_DEFAULT
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU1"))
                CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU4"))
            END_SWITCH
            ELSE
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG3_GANCE1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU4"))
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(2383), 1)
        SET_VALUE(GSWF(2379), 0)
        SET_VALUE(GSWF(2380), 0)
        SET_VALUE(GSWF(2381), 1)
        SET_VALUE(GSWF(2382), 0)
    END_IF
    IF_INT_EQUAL(GSWF(2384), 1)
        SET_VALUE(GSWF(2379), 0)
        SET_VALUE(GSWF(2380), 0)
        SET_VALUE(GSWF(2381), 0)
        SET_VALUE(GSWF(2382), 0)
    END_IF
    IF_INT_EQUAL(GSWF(2385), 1)
        SET_VALUE(GSWF(2379), 0)
        SET_VALUE(GSWF(2380), 0)
        SET_VALUE(GSWF(2381), 0)
        SET_VALUE(GSWF(2382), 0)
    END_IF
    IF_INT_EQUAL(GSWF(2379), 1)
        CALL(evt_mario_set_pos, -625, 0, 300)
        CALL(evt_party_set_pos, 0, -640, 0, 300)
        SPAWN_EVENT(PTR(evt_gardman))
    END_IF
    IF_INT_EQUAL(GSWF(2380), 1)
        CALL(evt_mario_set_pos, -625, 0, -300)
        CALL(evt_party_set_pos, 0, -640, 0, -300)
        CALL(evt_party_set_pos, 1, 0, -1000, 0)
        SPAWN_EVENT(PTR(evt_gardman2))
    END_IF
    IF_INT_EQUAL(GSWF(2381), 1)
        CALL(evt_mario_set_pos, -625, 0, -300)
        CALL(evt_party_set_pos, 0, -640, 0, -300)
        CALL(evt_party_set_pos, 1, 0, -1000, 0)
        CALL(evt_mario_set_dir, 270, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
        SPAWN_EVENT(PTR(evt_gardman3))
    END_IF
    IF_INT_EQUAL(GSWF(2382), 1)
        CALL(evt_mario_set_pos, 350, 0, -580)
        CALL(evt_party_set_pos, 0, 350, 0, -600)
        CALL(evt_party_set_pos, 1, 0, -1000, 0)
        SPAWN_EVENT(PTR(evt_gardman4))
    END_IF
    IF_INT_EQUAL(GSWF(2384), 1)
        CALL(evt_mario_set_pos, 530, 0, 530)
        CALL(evt_party_set_pos, 0, 530, 0, 530)
        CALL(evt_cam_road_reset)
        SPAWN_EVENT(PTR(evt_toile))
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2385), 1)
        CALL(evt_mario_set_pos, 530, 0, 530)
        CALL(evt_party_set_pos, 0, 530, 0, 530)
        CALL(evt_cam_road_reset)
        SPAWN_EVENT(PTR(evt_toile2))
        RETURN
    END_IF
    SET_VALUE(LW(2), 0)
    LOOP(11)
        SET_VALUE(LW(0), LW(2))
        CALL_CHILD(PTR(bero_case_switch_off))
        ADD(LW(2), 1)
    END_LOOP
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(130)
        CASE_LESS(131)
        SET_VALUE(LW(0), 5)
        CALL_CHILD(PTR(bero_case_switch_on))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_LESS(1)
            SET_VALUE(LW(0), 3)
            CALL_CHILD(PTR(bero_case_switch_on))
            CASE_LESS(11)
            SET_VALUE(LW(0), 6)
            CALL_CHILD(PTR(bero_case_switch_on))
            CASE_DEFAULT
            SET_VALUE(LW(0), 7)
            CALL_CHILD(PTR(bero_case_switch_on))
        END_SWITCH
    END_SWITCH
    SET_VALUE(LW(0), 0)
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(LW(0), 8)
    CALL_CHILD(PTR(bero_case_switch_on))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 141)
        SET_VALUE(LW(0), STRING("s_bero_7"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 142)
        SET_VALUE(LW(0), STRING("s_bero_7"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 157)
        IF_INT_EQUAL(GSWF(2399), 0)
            SET_VALUE(LW(0), 10)
            CALL_CHILD(PTR(bero_case_switch_on))
            SET_VALUE(LW(0), 0)
            CALL_CHILD(PTR(bero_case_switch_off))
            SET_VALUE(LW(0), 8)
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("w_bero"), 0, PTR(boss_msg), 0)
            CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(boss_msg), 0)
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 163)
        SET_VALUE(LW(0), 10)
        CALL_CHILD(PTR(bero_case_switch_on))
        SET_VALUE(LW(0), 0)
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), 8)
        CALL_CHILD(PTR(bero_case_switch_off))
#ifndef VERSION_G8ME01
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("w_bero"), 0, PTR(boss_msg), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("e_bero"), 0, PTR(boss_msg), 0)
#endif
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 163)
        IF_INT_EQUAL(GSWF(2496), 0)
            SET_VALUE(LW(0), 5)
            CALL_CHILD(PTR(bero_case_switch_on))
            ELSE
            IF_INT_EQUAL(GSWF(2499), 1)
                SET_VALUE(LW(0), 5)
                CALL_CHILD(PTR(bero_case_switch_on))
            END_IF
            IF_INT_EQUAL(GSWF(2498), 1)
                IF_INT_EQUAL(GSWF(2500), 0)
                    SET_VALUE(LW(0), 5)
                    CALL_CHILD(PTR(bero_case_switch_on))
                END_IF
            END_IF
        END_IF
    END_IF
    CALL(evt_cam_road_reset)
    IF_INT_EQUAL(GSWF(2450), 1)
        IF_INT_EQUAL(GSWF(2451), 0)
            CALL(evt_mario_kill_party, 0)
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 1)
        CALL(evt_party_set_pos, 1, 0, -1000, 0)
        CALL(evt_party_stop, 1)
        CALL(evt_mario_clear_party, 1)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 147)
        CALL(evt_mobj_lock, STRING("lock"), 18, 0, 10, -550, 0, PTR(evt_chk), PTR(evt_lock), GSWF(2436))
        SET_VALUE(LW(0), STRING("s_bero_4"))
        CALL_CHILD(PTR(bero_case_switch_off))
        ELSE
        SET_VALUE(LW(0), STRING("s_bero_4"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_IF
    IF_INT_NOT_EQUAL(GSWF(2377), 1)
        IF_INT_NOT_EQUAL(GSWF(2378), 1)
            CALL(evt_tou_get_ranking, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), 11)
                IF_INT_LESS(GSW_STORY_PROGRESS, 147)
                    SPAWN_EVENT(PTR(evt_souko_talk))
                END_IF
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 144)
        SPAWN_EVENT(PTR(evt_bred_nigeru))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 172)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("s_bero_7"))
            CALL(evt_tou_get_ranking, LW(1))
            IF_INT_EQUAL(LW(1), 10)
                SPAWN_EVENT(PTR(out_from_tou_10_re))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()


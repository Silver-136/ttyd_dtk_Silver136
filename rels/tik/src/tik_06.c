#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_cam.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static EvtWord oshimodo_event[32];
static BeroEntry bero_entry_data[5];
EvtWord iri_12_kino_makkino_talk[284];
EvtWord iri_12_kino_talk[36];
static EvtWord iri_12_makkino_talk[7];
static EvtWord iri_12_init[85];
static EvtWord evt_kanban[16];
static EvtWord evt_kanban2[45];
EvtWord tik_06_init_evt[47];

static EVT_BEGIN(oshimodo_event)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_ctrl_onoff, 4, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_bero_exec_wait, 65536)
        WAIT_MS(750)
        CALL(evt_msg_print, 0, STRING("tik_06_01"), 0, 0)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_cam_ctrl_onoff, 4, 1)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
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
        "jon_00", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        8, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_18", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        oshimodo_event, // exitEvent
        0, // nextMapName
        "dokan_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_05", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(iri_12_kino_makkino_talk)
    CALL(evt_set_dir_to_target, STRING("キーノ"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("マッキノ"), STRING("mario"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("キーノ"))
    CALL(evt_npc_wait_pera, STRING("マッキノ"))
    CALL(evt_msg_print, 0, STRING("irai_12_001_3"), 0, STRING("キーノ"))
    CALL(evt_msg_print, 0, STRING("irai_12_001_4"), 0, STRING("マッキノ"))
    CALL(evt_msg_print, 0, STRING("irai_12_001_5"), 0, STRING("キーノ"))
    IF_INT_EQUAL(LW(0), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("マッキノ"), LW(3), LW(4), LW(5))
            CALL(evt_sub_get_dir, LW(0), LW(2), LW(3), LW(5), LW(6))
            DIVIDE(LW(6), 90)
            MULTIPLY(LW(6), 90)
            ADD(LW(6), 45)
            CALL(evt_npc_add_dirdist, LW(3), LW(5), LW(6), 20)
            CALL(evt_npc_status_onoff, 0, STRING("マッキノ"), 2)
            CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
            CALL(evt_npc_move_position, STRING("マッキノ"), LW(3), LW(5), 0, 70, 0)
            WAIT_FRAMES(1)
            CALL(evt_npc_status_onoff, 1, STRING("マッキノ"), 2)
            CALL(evt_set_dir_to_target, STRING("マッキノ"), STRING("mario"))
        END_INLINE_EVENT
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("キーノ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_get_dir, LW(0), LW(2), LW(3), LW(5), LW(6))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(6), 20)
    CALL(evt_npc_flag_onoff, 1, STRING("キーノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("キーノ"), LW(0), LW(2), 0, 60, 0)
    CALL(evt_mario_set_dir_npc, STRING("キーノ"))
    WAIT_MS(500)
    CALL(evt_item_entry, STRING("tf"), ITEM_SILVER_CARD, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_get_item, STRING("tf"))
    CALL(evt_item_get_item_end_wait, STRING("tf"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("irai_12_001_6"), 0, STRING("マッキノ"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), 1073750048)
        CALL(evt_npc_move_position, STRING("マッキノ"), 220, 0, 0, 60, 4)
        CALL(evt_npc_jump_position_nohit, STRING("マッキノ"), 254, 15, 0, 400, 15)
        CALL(evt_npc_jump_position_nohit, STRING("マッキノ"), 285, 30, 0, 400, 15)
        CALL(evt_npc_set_position, STRING("マッキノ"), 0, -1000, 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("キーノ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 25)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("マッキノ"), LW(3), LW(4), LW(5))
        IF_INT_GREATER_OR_EQUAL(LW(3), LW(0))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_flag_onoff, 1, STRING("キーノ"), 1073750048)
    CALL(evt_npc_move_position, STRING("キーノ"), 220, 0, 0, 60, 4)
    CALL(evt_npc_jump_position_nohit, STRING("キーノ"), 254, 15, 0, 400, 15)
    CALL(evt_npc_jump_position_nohit, STRING("キーノ"), 285, 30, 0, 400, 15)
    CALL(evt_npc_set_position, STRING("キーノ"), 0, -1000, 0)
    CALL(evt_sub_save_playtime, GSW(1429))
    SET_VALUE(GSW(1420), 0)
    SET_VALUE(GSWF(5386), 1)
    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(iri_12_kino_talk)
    IF_INT_EQUAL(GSW(1420), 12)
        IF_INT_EQUAL(GSWF(5407), 0)
            IF_INT_EQUAL(GSWF(5405), 0)
                CALL(evt_msg_print, 0, STRING("irai_12_001_1"), 0, 0)
                SET_VALUE(GSWF(5405), 1)
                ELSE
                CALL(evt_msg_print, 0, STRING("irai_12_001_2"), 0, 0)
            END_IF
            ELSE
            SET_VALUE(LW(0), 0)
            CALL_CHILD(PTR(iri_12_kino_makkino_talk))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(iri_12_makkino_talk)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(iri_12_kino_makkino_talk))
    RETURN
EVT_END()

static EVT_BEGIN(iri_12_init)
    IF_INT_EQUAL(GSW(1420), 12)
        CALL(evt_npc_entry, STRING("キーノ"), STRING("c_kokinopio"))
        CALL(evt_npc_set_tribe, STRING("キーノ"), STRING("キノピオ子"))
        CALL(evt_npc_set_position, STRING("キーノ"), 60, 0, -70)
        IF_INT_EQUAL(GSWF(5407), 0)
            CALL(evt_npc_set_ry, STRING("キーノ"), 270)
            ELSE
            CALL(evt_npc_set_ry, STRING("キーノ"), 90)
        END_IF
        CALL(evt_npc_flag_onoff, 1, STRING("キーノ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_status_onoff, 1, STRING("キーノ"), 2)
        CALL(evt_npc_change_interrupt, STRING("キーノ"), 6, PTR(iri_12_kino_talk))
        IF_INT_EQUAL(GSWF(5407), 1)
            CALL(evt_npc_entry, STRING("マッキノ"), STRING("c_suifu_k"))
            CALL(evt_npc_set_tribe, STRING("マッキノ"), STRING("水夫"))
            CALL(evt_npc_set_position, STRING("マッキノ"), 90, 0, -70)
            CALL(evt_npc_set_ry, STRING("マッキノ"), 90)
            CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
            CALL(evt_npc_status_onoff, 1, STRING("マッキノ"), 2)
            CALL(evt_npc_change_interrupt, STRING("マッキノ"), 6, PTR(iri_12_makkino_talk))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_kanban)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_msg_print, 0, STRING("tik_06_02"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kanban2)
    CALL(evt_npc_stop_for_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    SET_VALUE(LW(0), GSW(33))
    SET_VALUE(LW(1), GSW(32))
    SET_VALUE(LW(4), GSW(35))
    MULTIPLY(LW(4), 256)
    ADD(LW(1), LW(4))
    SET_VALUE(LW(2), GSW(34))
    SET_VALUE(LW(3), 100)
    CALL(evt_msg_print_insert, 0, STRING("msg_jon_kanban_3"), 0, 0, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_start_for_event)
    RETURN
EVT_END()

EVT_BEGIN(tik_06_init_evt)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(iri_12_init))
    CALL(evt_mobj_signboard, STRING("board"), -50, 0, 50, PTR(evt_kanban), LSWF(0))
    CALL(evt_mobj_signboard, STRING("board2"), 50, 0, -250, PTR(evt_kanban2), LSWF(0))
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 290, 0, -50, ITEM_TYPE_STILL, GSWF(5593), 0)
    RETURN
EVT_END()


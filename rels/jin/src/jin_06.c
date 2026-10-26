#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "jin.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);
USER_FUNC(evt_kagemario_init);

static EvtWord w_close[12];
static EvtWord w_open[12];
static BeroEntry bero_entry_data[2];
EvtWord jin_06_wall_open_evt[5];
EvtWord jin_06_jumpstand[53];
static EvtWord mobj_box00[34];
EvtWord jin_06_init_evt[165];

static EVT_BEGIN(w_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door07"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door07"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_07", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_close, // enterAnimeArg
        w_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(jin_06_wall_open_evt)
    SET_VALUE(GSWF_JIN_06_WALL_PANEL_BLOWN_AWAY, 1)
    RETURN
EVT_END()

EVT_BEGIN(jin_06_jumpstand)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            LOOP(0)
                CALL(evt_party_get_name_hitobj_ride, 0, LW(0))
                IF_STR_EQUAL(LW(0), STRING("MOBJ_mobj_jump"))
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_party_jump_pos, 0, 30, 0, 50, 600, 0, -1000)
        END_IF
    END_INLINE_EVENT
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_jump_jumpstand, 0, 0, 0, 0, 400, 0, 0, 0, 50, 1000, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_FLOWER_SAVER, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(jin_06_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_JIN1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 650, 0, -150, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box00), GSWF_JIN_06_FLOWER_SAVER_CHEST)
    CALL(evt_mobj_jumpstand_red, 0, STRING("mobj_jump"), 0, 0, 0, PTR(jin_06_jumpstand), 0)
    IF_INT_NOT_EQUAL(GSWF_JIN_SCRIPTED_ENTRY_PENDING, 0)
        SET_VALUE(GSWF_JIN_SCRIPTED_ENTRY_PENDING, 0)
        BEGIN_INLINE_EVENT
            CALL(evt_cam_letter_box_disable, 1)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mario_set_pos, 5, 365, 0)
            CALL(evt_party_set_pos, 0, 5, 645, 0)
            CALL(evt_cam_road_reset)
            CALL(evt_seq_wait, SEQ_GAME)
            CALL(evt_mario_wait_landon)
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_cam_letter_box_disable, 0)
        END_INLINE_EVENT
        SET_VALUE(LW(0), 1)
        ELSE
        SET_VALUE(LW(0), 0)
    END_IF
    BEGIN_INLINE_EVENT
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_seq_wait, SEQ_GAME)
            LOOP(0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                IF_INT_LESS(LW(1), 365)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_IF
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(1), 365)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_bero_mapchange, STRING("jin_00"), STRING("ana"))
    END_INLINE_EVENT
    IF_INT_EQUAL(GSWF_JIN_06_WALL_PANEL_BLOWN_AWAY, 0)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 0, STRING("mekure"), STRING("a_mekure"), PTR(jin_06_wall_open_evt))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("mekure"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_mekure"), 0, 0)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_09_01_PUSH_THE_STATUE)
        SET_VALUE(GSW_STORY_PROGRESS, STG4_09_01_PUSH_THE_STATUE)
    END_IF
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "mario/mario_pouch.h"
#include "data_tables.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "gon.h"
typedef struct KemuriPos {
    s32 x;
    s32 z;
} KemuriPos;

USER_FUNC(evt_mario_get_name_hitobj_head);
USER_FUNC(evt_win_item_select);

static EvtWord w1_door_close[12];
static EvtWord w1_door_open[12];
static EvtWord w2_door_close[12];
static EvtWord w2_door_open[12];
static EvtWord e1_door_close[12];
static EvtWord e1_door_open[12];
static EvtWord e3_door_close[12];
static EvtWord e3_door_open[12];
static BeroEntry bero_entry_data[6];
EvtWord evt_gon_04flowblock_updown1_init[37];
#ifdef VERSION_G8ME01
EvtWord evt_gon_04flowblock_updown1[465];
#else
EvtWord evt_gon_04flowblock_updown1[491];
#endif
EvtWord evt_gon_04flowblock_updown2_init[37];
#ifdef VERSION_G8ME01
EvtWord evt_gon_04flowblock_updown2[465];
#else
EvtWord evt_gon_04flowblock_updown2[491];
#endif
KemuriPos kemuri_pos1[5];
KemuriPos kemuri_pos2[5];
EvtWord evt_kaidan_pera_anime[203];
EvtWord evt_kaidan_pera_init[29];
EvtWord evt_kaidan_pera[214];
EvtWord evt_hashi_pera[209];
EvtWord evt_hashi_pera0[16];
EvtWord evt_hashi_pera1[16];
EvtWord evt_hashi[50];
static s32 key_tbl[2];
static EvtWord evt_chk[23];
static EvtWord evt_unlock[10];
static EvtWord key_get[5];
EvtWord gon_04_init_evt[382];

static EVT_BEGIN(w1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e3_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e3_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door04"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_03", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w1_door_close, // enterAnimeArg
        w1_door_open, // exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_08", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w2_door_close, // enterAnimeArg
        w2_door_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_05", // nextMapName
        "w_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
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
        "gon_05", // nextMapName
        "w_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero_3", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_05", // nextMapName
        "w_bero_3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e3_door_close, // enterAnimeArg
        e3_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_gon_04flowblock_updown1_init)
    IF_INT_EQUAL(1, 0)
        IF_INT_NOT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 0)
            CALL(evt_mobj_get_y_position, STRING("mobj_fltbl01"), LW(5))
            ADD_FLOAT(LW(5), FLOAT(225.0f))
            CALL(evt_mobj_set_y_position, STRING("mobj_fltbl01"), LW(5))
        END_IF
        ELSE
        IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 0)
            CALL(evt_mobj_get_y_position, STRING("mobj_fltbl01"), LW(5))
            ADD_FLOAT(LW(5), FLOAT(225.0f))
            CALL(evt_mobj_set_y_position, STRING("mobj_fltbl01"), LW(5))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_gon_04flowblock_updown1)
    SET_TYPE(239)
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
    IF_INT_NOT_EQUAL(1, 0)
        SET_VALUE(LW(10), 0)
        IF_INT_EQUAL(1, 0)
            IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 0)
                SET_VALUE(LW(10), 0)
                ELSE
                SET_VALUE(LW(10), 1)
            END_IF
            ELSE
            IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 0)
                SET_VALUE(LW(10), 1)
                ELSE
                SET_VALUE(LW(10), 0)
            END_IF
        END_IF
        IF_INT_EQUAL(LW(10), 0)
            CALL(evt_mobj_get_y_position, STRING("mobj_fltbl01"), LW(2))
            SET_FLOAT_VALUE(LW(3), LW(2))
            ADD_FLOAT(LW(3), FLOAT(225.0f))
            SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
            ELSE
            CALL(evt_mobj_get_y_position, STRING("mobj_fltbl01"), LW(2))
            SET_FLOAT_VALUE(LW(3), LW(2))
            SUBTRACT_FLOAT(LW(3), FLOAT(225.0f))
            SET_FLOAT_VALUE(LW(8), FLOAT(-0.19921875f))
            SET_FLOAT_VALUE(LW(9), 0)
        END_IF
#ifdef VERSION_G8ME01
        BEGIN_INLINE_EVENT
#else
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(10)
            CALL(evt_sub_rumble_onoff, 0, 0)
            WAIT_MS(300)
            CALL(evt_sub_rumble_onoff, 1, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
#endif
            WAIT_FRAMES(10)
            CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 3, 4)
            LABEL(0)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), LW(0), 100)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
#ifdef VERSION_G8ME01
        END_INLINE_EVENT
#else
        END_SIBLING_EVENT
#endif
        WAIT_MS(500)
        CALL(evt_mobj_get_position, STRING("mobj_fltbl01"), LW(12), LW(13), LW(14))
        CALL(evt_mobj_get_kindname, STRING("mobj_fltbl01"), LW(11))
        SET_VALUE(LF(0), 0)
        IF_STR_EQUAL(LW(11), STRING("MOBJ_green_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(11), STRING("MOBJ_purple_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(11), STRING("MOBJ_orange_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LF(1), 0)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_MOVE1"), LW(12), LW(13), LW(14), LW(15))
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_MOVE1"), LW(12), LW(13), LW(14), LW(15))
            END_IF
            ELSE
            SET_VALUE(LF(1), 1)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_DOWN1"), LW(12), LW(13), LW(14), LW(15))
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_DOWN1"), LW(12), LW(13), LW(14), LW(15))
            END_IF
        END_IF
        SET_FLOAT_VALUE(LW(4), LW(2))
        SET_VALUE(LF(0), 0)
        LOOP(0)
            CALL(evt_mario_get_party, LW(5))
            IF_INT_EQUAL(LW(5), 0)
                CALL(evt_mario_get_name_hitobj_head, LW(6))
                IF_STR_EQUAL(LW(6), STRING("mobj_fltbl01"))
                    GOTO(1)
                END_IF
                ELSE
                CALL(evt_mario_get_name_hitobj_head, LW(6))
                IF_STR_EQUAL(LW(6), STRING("MOBJ_mobj_fltbl01"))
                    GOTO(1)
                END_IF
                IF_STR_EQUAL(LW(7), STRING("MOBJ_mobj_fltbl01"))
                    GOTO(1)
                END_IF
            END_IF
            GOTO(2)
            LABEL(1)
            IF_INT_EQUAL(LW(10), 1)
                SET_FLOAT_VALUE(LW(5), LW(2))
                SET_FLOAT_VALUE(LW(2), LW(3))
                SET_FLOAT_VALUE(LW(3), LW(5))
                SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
                SET_FLOAT_VALUE(LW(8), 0)
                SET_FLOAT_VALUE(LW(10), 0)
                IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 0)
                    SET_VALUE(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 1)
                    ELSE
                    SET_VALUE(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 0)
                END_IF
                CALL(evt_mobj_exec_cancel, STRING("mobj_fltsw01"))
            END_IF
            LABEL(2)
            ADD_FLOAT(LW(4), LW(9))
            ADD_FLOAT(LW(9), LW(8))
            IF_FLOAT_GREATER_OR_EQUAL(LW(9), FLOAT(4.0f))
                SET_FLOAT_VALUE(LW(9), FLOAT(4.0f))
            END_IF
            CALL(evt_mobj_get_position, STRING("mobj_fltbl01"), LW(11), LW(12), LW(13))
            CALL(evt_snd_sfx_pos, LW(15), LW(11), LW(12), LW(13))
            IF_INT_EQUAL(LW(10), 0)
                IF_INT_GREATER_OR_EQUAL(LW(4), LW(3))
                    SET_FLOAT_VALUE(LW(4), LW(3))
                    SET_VALUE(LF(0), 1)
                END_IF
                ELSE
                IF_INT_LESS_OR_EQUAL(LW(4), LW(3))
                    SET_FLOAT_VALUE(LW(4), LW(3))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(evt_mobj_set_y_position, STRING("mobj_fltbl01"), LW(4))
            IF_INT_EQUAL(LF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        IF_INT_EQUAL(LF(1), 0)
            CALL(evt_snd_sfxoff, LW(15))
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_mobj_get_position, STRING("mobj_fltbl01"), LW(13), LW(14), LW(15))
            CALL(evt_mobj_get_kindname, STRING("mobj_fltbl01"), LW(12))
            SET_VALUE(LF(0), 0)
            IF_STR_EQUAL(LW(12), STRING("MOBJ_green_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(12), STRING("MOBJ_purple_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(12), STRING("MOBJ_orange_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LW(10), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_STOP1"), LW(13), LW(14), LW(15), 0)
            END_IF
        END_INLINE_EVENT
        IF_INT_EQUAL(LW(10), 1)
#ifdef VERSION_G8ME01
            BEGIN_INLINE_EVENT
#else
            BEGIN_SIBLING_EVENT
                CALL(evt_sub_rumble_onoff, 0, 0)
                WAIT_MS(300)
                CALL(evt_sub_rumble_onoff, 1, 0)
            END_SIBLING_EVENT
            BEGIN_SIBLING_EVENT
#endif
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 3, 10)
                LABEL(2)
                CALL(evt_sub_intpl_get_value)
                DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
                CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), LW(0), 100)
                IF_INT_EQUAL(LW(1), 1)
                    GOTO(2)
                END_IF
#ifdef VERSION_G8ME01
            END_INLINE_EVENT
#else
            END_SIBLING_EVENT
#endif
            WAIT_MS(1000)
        END_IF
        IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 0)
            SET_VALUE(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 1)
            ELSE
            SET_VALUE(LSWF_GON_04_FLOAT_BLOCK_1_MOVED, 0)
        END_IF
        ELSE
        CALL(evt_mobj_exec_cancel, STRING("mobj_fltsw01"))
        SET_VALUE(LW(0), 0)
        LOOP(0)
            WAIT_FRAMES(2)
            SET_VALUE(LW(1), LW(0))
            MODULO(LW(1), 2)
            ADD(LW(0), 1)
            IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_gon_04flowblock_updown2_init)
    IF_INT_EQUAL(0, 0)
        IF_INT_NOT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 0)
            CALL(evt_mobj_get_y_position, STRING("mobj_fltbl02"), LW(5))
            ADD_FLOAT(LW(5), FLOAT(200.0f))
            CALL(evt_mobj_set_y_position, STRING("mobj_fltbl02"), LW(5))
        END_IF
        ELSE
        IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 0)
            CALL(evt_mobj_get_y_position, STRING("mobj_fltbl02"), LW(5))
            ADD_FLOAT(LW(5), FLOAT(200.0f))
            CALL(evt_mobj_set_y_position, STRING("mobj_fltbl02"), LW(5))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_gon_04flowblock_updown2)
    SET_TYPE(239)
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
    IF_INT_NOT_EQUAL(GSWF_GON_04_LV1_BIG_BLOCK_BROKEN, 0)
        SET_VALUE(LW(10), 0)
        IF_INT_EQUAL(0, 0)
            IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 0)
                SET_VALUE(LW(10), 0)
                ELSE
                SET_VALUE(LW(10), 1)
            END_IF
            ELSE
            IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 0)
                SET_VALUE(LW(10), 1)
                ELSE
                SET_VALUE(LW(10), 0)
            END_IF
        END_IF
        IF_INT_EQUAL(LW(10), 0)
            CALL(evt_mobj_get_y_position, STRING("mobj_fltbl02"), LW(2))
            SET_FLOAT_VALUE(LW(3), LW(2))
            ADD_FLOAT(LW(3), FLOAT(200.0f))
            SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
            ELSE
            CALL(evt_mobj_get_y_position, STRING("mobj_fltbl02"), LW(2))
            SET_FLOAT_VALUE(LW(3), LW(2))
            SUBTRACT_FLOAT(LW(3), FLOAT(200.0f))
            SET_FLOAT_VALUE(LW(8), FLOAT(-0.19921875f))
            SET_FLOAT_VALUE(LW(9), 0)
        END_IF
#ifdef VERSION_G8ME01
        BEGIN_INLINE_EVENT
#else
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(10)
            CALL(evt_sub_rumble_onoff, 0, 0)
            WAIT_MS(300)
            CALL(evt_sub_rumble_onoff, 1, 0)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
#endif
            WAIT_FRAMES(10)
            CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 3, 4)
            LABEL(0)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), LW(0), 100)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
#ifdef VERSION_G8ME01
        END_INLINE_EVENT
#else
        END_SIBLING_EVENT
#endif
        WAIT_MS(500)
        CALL(evt_mobj_get_position, STRING("mobj_fltbl02"), LW(12), LW(13), LW(14))
        CALL(evt_mobj_get_kindname, STRING("mobj_fltbl02"), LW(11))
        SET_VALUE(LF(0), 0)
        IF_STR_EQUAL(LW(11), STRING("MOBJ_green_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(11), STRING("MOBJ_purple_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_STR_EQUAL(LW(11), STRING("MOBJ_orange_big02_FlowBlock"))
            SET_VALUE(LF(0), 1)
        END_IF
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LF(1), 0)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_MOVE1"), LW(12), LW(13), LW(14), LW(15))
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_MOVE1"), LW(12), LW(13), LW(14), LW(15))
            END_IF
            ELSE
            SET_VALUE(LF(1), 1)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_DOWN1"), LW(12), LW(13), LW(14), LW(15))
                ELSE
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_DOWN1"), LW(12), LW(13), LW(14), LW(15))
            END_IF
        END_IF
        SET_FLOAT_VALUE(LW(4), LW(2))
        SET_VALUE(LF(0), 0)
        LOOP(0)
            CALL(evt_mario_get_party, LW(5))
            IF_INT_EQUAL(LW(5), 0)
                CALL(evt_mario_get_name_hitobj_head, LW(6))
                IF_STR_EQUAL(LW(6), STRING("mobj_fltbl02"))
                    GOTO(1)
                END_IF
                ELSE
                CALL(evt_mario_get_name_hitobj_head, LW(6))
                IF_STR_EQUAL(LW(6), STRING("MOBJ_mobj_fltbl02"))
                    GOTO(1)
                END_IF
                IF_STR_EQUAL(LW(7), STRING("MOBJ_mobj_fltbl02"))
                    GOTO(1)
                END_IF
            END_IF
            GOTO(2)
            LABEL(1)
            IF_INT_EQUAL(LW(10), 1)
                SET_FLOAT_VALUE(LW(5), LW(2))
                SET_FLOAT_VALUE(LW(2), LW(3))
                SET_FLOAT_VALUE(LW(3), LW(5))
                SET_FLOAT_VALUE(LW(9), FLOAT(2.0f))
                SET_FLOAT_VALUE(LW(8), 0)
                SET_FLOAT_VALUE(LW(10), 0)
                IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 0)
                    SET_VALUE(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 1)
                    ELSE
                    SET_VALUE(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 0)
                END_IF
                CALL(evt_mobj_exec_cancel, STRING("mobj_fltsw02"))
            END_IF
            LABEL(2)
            ADD_FLOAT(LW(4), LW(9))
            ADD_FLOAT(LW(9), LW(8))
            IF_FLOAT_GREATER_OR_EQUAL(LW(9), FLOAT(4.0f))
                SET_FLOAT_VALUE(LW(9), FLOAT(4.0f))
            END_IF
            CALL(evt_mobj_get_position, STRING("mobj_fltbl02"), LW(11), LW(12), LW(13))
            CALL(evt_snd_sfx_pos, LW(15), LW(11), LW(12), LW(13))
            IF_INT_EQUAL(LW(10), 0)
                IF_INT_GREATER_OR_EQUAL(LW(4), LW(3))
                    SET_FLOAT_VALUE(LW(4), LW(3))
                    SET_VALUE(LF(0), 1)
                END_IF
                ELSE
                IF_INT_LESS_OR_EQUAL(LW(4), LW(3))
                    SET_FLOAT_VALUE(LW(4), LW(3))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(evt_mobj_set_y_position, STRING("mobj_fltbl02"), LW(4))
            IF_INT_EQUAL(LF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        IF_INT_EQUAL(LF(1), 0)
            CALL(evt_snd_sfxoff, LW(15))
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_mobj_get_position, STRING("mobj_fltbl02"), LW(13), LW(14), LW(15))
            CALL(evt_mobj_get_kindname, STRING("mobj_fltbl02"), LW(12))
            SET_VALUE(LF(0), 0)
            IF_STR_EQUAL(LW(12), STRING("MOBJ_green_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(12), STRING("MOBJ_purple_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_STR_EQUAL(LW(12), STRING("MOBJ_orange_big02_FlowBlock"))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LW(10), 1)
                CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_STOP1"), LW(13), LW(14), LW(15), 0)
            END_IF
        END_INLINE_EVENT
        IF_INT_EQUAL(LW(10), 1)
#ifdef VERSION_G8ME01
            BEGIN_INLINE_EVENT
#else
            BEGIN_SIBLING_EVENT
                CALL(evt_sub_rumble_onoff, 0, 0)
                WAIT_MS(300)
                CALL(evt_sub_rumble_onoff, 1, 0)
            END_SIBLING_EVENT
            BEGIN_SIBLING_EVENT
#endif
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 3, 10)
                LABEL(2)
                CALL(evt_sub_intpl_get_value)
                DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
                CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), LW(0), 100)
                IF_INT_EQUAL(LW(1), 1)
                    GOTO(2)
                END_IF
#ifdef VERSION_G8ME01
            END_INLINE_EVENT
#else
            END_SIBLING_EVENT
#endif
            WAIT_MS(1000)
        END_IF
        IF_INT_EQUAL(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 0)
            SET_VALUE(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 1)
            ELSE
            SET_VALUE(LSWF_GON_04_FLOAT_BLOCK_2_MOVED, 0)
        END_IF
        ELSE
        CALL(evt_mobj_exec_cancel, STRING("mobj_fltsw02"))
        SET_VALUE(LW(0), 0)
        LOOP(0)
            WAIT_FRAMES(2)
            SET_VALUE(LW(1), LW(0))
            MODULO(LW(1), 2)
            ADD(LW(0), 1)
            IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_IF
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

KemuriPos kemuri_pos1[] = {
    {150, 20},
    {130, 55},
    {120, 100},
    {220, 20},
    {185, 75},
};

KemuriPos kemuri_pos2[] = {
    {315, 75},
    {270, 30},
    {380, 100},
    {370, 55},
    {340, 20},
};

EVT_BEGIN(evt_kaidan_pera_anime)
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_gon_04_2"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_set_offscreen, 0, STRING("neta_02"), STRING("offscreen"))
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
        CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("offscreen"), 0, 0, 0, 608, 480)
        CALL(evt_img_alloc_capture, STRING("img"), 1, STRING("offscreen"), 0, 0, 0, 608, 480)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("neta_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_neta_01"), 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_STG1_PERA_STEP1"), 0)
        ELSE
        CALL(evt_mapobj_set_offscreen, 0, STRING("neta_02"), STRING("offscreen"))
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
        CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("offscreen"), 0, 0, 0, 608, 480)
        CALL(evt_img_alloc_capture, STRING("img"), 1, STRING("offscreen"), 0, 0, 0, 608, 480)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("neta_02"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_neta_02"), 0, 0)
        CALL(evt_snd_sfxon, STRING("SFX_STG1_PERA_STEP2"), 0)
    END_IF
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_snd_sfxon, STRING("SFX_STG1_QUAK_PERA1"), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.015625f), FLOAT(0.0f), 250)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("neta_02"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_neta_02"), 0, 1)
        SET_INT_READ_ADDR(PTR(kemuri_pos1))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("neta_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_neta_01"), 0, 1)
        SET_INT_READ_ADDR(PTR(kemuri_pos2))
    END_IF
    LOOP(5)
        READ_INT2(LW(0), LW(1))
        CALL(evt_sub_random, 10, LW(2))
        SUBTRACT(LW(2), 5)
        ADD(LW(2), LW(0))
        CALL(evt_sub_random, 20, LW(3))
        SUBTRACT(LW(3), 10)
        ADD(LW(3), LW(1))
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(2), LW(3), 60, FLOAT(1.5996094f), 0, 0, 0, 0, 0, 0, 0)
    END_LOOP
    CALL(evt_img_release, STRING("img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    CALL(evt_mapobj_clear_offscreen, 0, STRING("neta_02"))
    RETURN
EVT_END()

EVT_BEGIN(evt_kaidan_pera_init)
    IF_INT_EQUAL(GSWF_GON_04_PAPER_STAIRS_FLIPPED, 0)
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("neta_02"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_neta_02"), 0, 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 0, 1, STRING("neta_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_neta_01"), 0, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_kaidan_pera)
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 0)
    CALL(evt_mario_key_onoff, 0)
    LOOP(0)
        IF_INT_EQUAL(GF(10), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_party_use_check, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CONVERT_MS_TO_FRAME(LW(0), 1500)
        LOOP(LW(0))
            WAIT_FRAMES(1)
            CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
            CALL(evt_party_get_pos, 0, LW(13), LW(14), LW(15))
            IF_INT_GREATER_OR_EQUAL(LW(10), LW(13))
                SET_VALUE(LW(8), LW(10))
                SET_VALUE(LW(9), LW(13))
                ELSE
                SET_VALUE(LW(8), LW(13))
                SET_VALUE(LW(9), LW(10))
            END_IF
            SUBTRACT(LW(8), LW(9))
            IF_INT_LESS_OR_EQUAL(LW(8), 15)
                BREAK_LOOP
            END_IF
        END_LOOP
        ELSE
        WAIT_MS(300)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_12_PAPER_STAIRS)
        SET_VALUE(GSW_STORY_PROGRESS, STG1_12_PAPER_STAIRS)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_map_get_fog, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
        CONVERT_MS_TO_FRAME(LW(10), 500)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 250, LW(10))
        LOOP(0)
            CALL(evt_sub_intpl_get_value)
            SET_VALUE(LW(2), LW(5))
            ADD(LW(2), LW(0))
            CALL(evt_map_set_fog, LW(4), LW(2), LW(6), LW(7), LW(8), LW(9))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 250, 125, 650, 250, 80, -140, 450, EASING_SINE_IN_OUT)
    WAIT_MS(1500)
    SET_VALUE(LW(0), GSWF_GON_04_PAPER_STAIRS_FLIPPED)
    CALL_CHILD(PTR(evt_kaidan_pera_anime))
    WAIT_MS(1000)
    IF_INT_EQUAL(GSWF_GON_04_PAPER_STAIRS_FLIPPED, 0)
        SET_VALUE(GSWF_GON_04_PAPER_STAIRS_FLIPPED, 1)
        ELSE
        SET_VALUE(GSWF_GON_04_PAPER_STAIRS_FLIPPED, 0)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_map_get_fog, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
        CONVERT_MS_TO_FRAME(LW(10), 500)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 250, LW(10))
        LOOP(0)
            CALL(evt_sub_intpl_get_value)
            SET_VALUE(LW(2), LW(5))
            SUBTRACT(LW(2), LW(0))
            CALL(evt_map_set_fog, LW(4), LW(2), LW(6), LW(7), LW(8), LW(9))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_off, 450, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_mobj_hitevt_onoff, STRING("me"), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_hashi_pera)
    CALL(evt_mario_key_onoff, 0)
    LOOP(0)
        WAIT_FRAMES(1)
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_cam3d_evt_set, 280, 1000, 450, 280, 900, -140, 300, EASING_SINE_IN_OUT)
        ELSE
        CALL(evt_cam3d_evt_set, 0, 1000, 450, 0, 900, -140, 300, EASING_SINE_IN_OUT)
    END_IF
    WAIT_MS(500)
    CALL(evt_paper_entry, STRING("OFF_gon_04_3"))
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_gon_04_3"))
    CALL(evt_offscreen_entry, STRING("offscreen2"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_set_offscreen, 1, STRING("tuuro_02"), STRING("offscreen2"))
        ELSE
        CALL(evt_mapobj_set_offscreen, 1, STRING("tuuro_01"), STRING("offscreen2"))
    END_IF
    CALL(evt_offscreen_get_boundingbox, STRING("offscreen2"), LW(10), LW(11), LW(12), LW(13))
    ADD(LW(10), -40)
    ADD(LW(11), -100)
    ADD(LW(12), 80)
    ADD(LW(13), 100)
    CALL(evt_img_alloc_capture, STRING("capture_img"), 0, 0, 1, LW(10), LW(11), LW(12), LW(13))
    CALL(evt_img_alloc_capture, STRING("capture_img"), 2, STRING("offscreen2"), 1, LW(10), LW(11), LW(12), LW(13))
    CALL(evt_img_clear_virtual_point, STRING("capture_img"))
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("Z_1"))
    WAIT_MS(500)
    CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("tuuro_02"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_tuuro_02"), 0, 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("tuuro_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_tuuro_01"), 0, 1)
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_STG1_PERA2"), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1900)
        CALL(evt_snd_sfxon, STRING("SFX_STG1_PERA2"), 0)
    END_INLINE_EVENT
    CALL(evt_img_wait_animend, STRING("capture_img"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_clear_offscreen, 1, STRING("tuuro_02"))
        ELSE
        CALL(evt_mapobj_clear_offscreen, 1, STRING("tuuro_01"))
    END_IF
    CALL(evt_offscreen_delete, STRING("offscreen2"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 300, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_hashi_pera0)
    SET_VALUE(GF(10), 1)
    SET_VALUE(LW(0), 0)
    CALL_CHILD(PTR(evt_hashi_pera))
    SET_VALUE(GSW_STORY_PROGRESS, STG1_14_PAPER_BRIDGE_RIGHT)
    SET_VALUE(GF(10), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_hashi_pera1)
    SET_VALUE(GF(10), 1)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(PTR(evt_hashi_pera))
    SET_VALUE(GSW_STORY_PROGRESS, STG1_14_01_PAPER_BRIDGE_LEFT)
    SET_VALUE(GF(10), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_hashi)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_14_PAPER_BRIDGE_RIGHT)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tuuro_02"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_tuuro_02"), 0, 0)
        CALL(evt_mobj_switch_blue, 0, STRING("mobj_hashi0"), 400, 900, -100, PTR(evt_hashi_pera0), GSWF_GON_04_BRIDGE_SWITCH_RIGHT_PRESSED)
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_14_01_PAPER_BRIDGE_LEFT)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("tuuro_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_tuuro_01"), 0, 0)
        CALL(evt_mobj_switch_blue, 0, STRING("mobj_hashi1"), -80, 900, -100, PTR(evt_hashi_pera1), GSWF_GON_04_BRIDGE_SWITCH_LEFT_PRESSED)
    END_IF
    RETURN
EVT_END()

static s32 key_tbl[] = {
    ITEM_CASTLE_KEY_0x0D, -1,
};

static EVT_BEGIN(evt_chk)
    CALL(evt_pouch_check_item, ITEM_CASTLE_KEY_0x0D, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_unlock)
    SET_VALUE(LW(0), STRING("e_bero_1"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, STG1_12_02_LOCK)
    RETURN
EVT_END()

static EVT_BEGIN(key_get)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_12_01_KEY)
    RETURN
EVT_END()

EVT_BEGIN(gon_04_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_cam_road_reset)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_npc_entry, STRING("\"カロン00\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン00\""), -502, 0, -68)
    CALL(evt_npc_set_rotate, STRING("\"カロン00\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン00\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン00\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン00\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン00\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン00\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン00\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン01\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン01\""), -359, 0, 95)
    CALL(evt_npc_set_rotate, STRING("\"カロン01\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン01\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン01\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン01\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン01\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン01\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン01\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン02\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン02\""), -191, 0, -1)
    CALL(evt_npc_set_rotate, STRING("\"カロン02\""), 0, 0, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン02\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン02\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン02\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン02\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン02\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン02\""), 7)
    CALL(evt_npc_entry, STRING("\"カロン03\""), STRING("c_karon_g"))
    CALL(evt_npc_set_position, STRING("\"カロン03\""), 9, 0, 75)
    CALL(evt_npc_set_rotate, STRING("\"カロン03\""), 0, 180, 0)
    CALL(evt_npc_set_anim, STRING("\"カロン03\""), STRING("KRN_Z_2"))
    CALL(evt_npc_pera_onoff, STRING("\"カロン03\""), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\"カロン03\""), NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("\"カロン03\""), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_set_width, STRING("\"カロン03\""), 30)
    CALL(evt_npc_set_height, STRING("\"カロン03\""), 7)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_mobj_switch_red, 0, STRING("mobj1"), 250, 150, -120, PTR(evt_kaidan_pera), 0)
    SPAWN_EVENT(PTR(evt_kaidan_pera_init))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuyu_box01"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_fuyu_box01"), 1, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("fuyu_box02"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_fuyu_box02"), 1, 0)
    CALL(evt_mobj_float_blk, STRING("mobj_fltbl01"), FLOAT(582.5f), 150, FLOAT(-137.5f), 1, 0, 0, LSWF_GON_04_FLOAT_BLOCK_1_MOVED)
    CALL(evt_mobj_float_blk, STRING("mobj_fltbl02"), 395, 150, -125, 2, 1, 0, LSWF_GON_04_FLOAT_BLOCK_2_MOVED)
    CALL_CHILD(PTR(evt_gon_04flowblock_updown1_init))
    CALL_CHILD(PTR(evt_gon_04flowblock_updown2_init))
    CALL(evt_mobj_switch_float_blk, STRING("mobj_fltsw01"), 550, 75, 137, STRING("mobj_fltbl01"), 1, PTR(evt_gon_04flowblock_updown1), LSWF_GON_04_FLOAT_BLOCK_1_MOVED)
    CALL(evt_mobj_switch_float_blk, STRING("mobj_fltsw02"), 450, 75, 60, STRING("mobj_fltbl02"), 2, PTR(evt_gon_04flowblock_updown2), LSWF_GON_04_FLOAT_BLOCK_2_MOVED)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_12_01_KEY)
        CALL(evt_item_entry, STRING("key00"), ITEM_CASTLE_KEY_0x0D, -150, 150, -125, ITEM_TYPE_STILL, -1, PTR(key_get))
    END_IF
    CALL(evt_item_entry, STRING("item00"), ITEM_STAR_PIECE, 150, 400, -125, ITEM_TYPE_STILL, GSWF_STARPIECE_37, 0)
#ifdef VERSION_G8ME01
    CALL(evt_item_entry, STRING("item01"), ITEM_STAR_PIECE, 420, 910, -175, ITEM_TYPE_STILL, GSWF_STARPIECE_38, 0)
#else
    CALL(evt_item_entry, STRING("item01"), ITEM_STAR_PIECE, 400, 900, -175, ITEM_TYPE_STILL, GSWF_STARPIECE_38, 0)
#endif
    CALL(evt_mobj_lv_blk, STRING("mobj_blk00"), 395, 200, -125, 0, GSWF_GON_04_LV1_BIG_BLOCK_BROKEN, STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("mobj_blk01"), 445, 150, -125, 0, GSWF_GON_04_LV2_BIG_BLOCK_1_BROKEN, STRING("MOBJ_Lv2BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("mobj_blk02"), FLOAT(545.0f), 150, -125, 0, GSWF_GON_04_LV2_BIG_BLOCK_2_BROKEN, STRING("MOBJ_Lv2BigBlock"))
    SPAWN_EVENT(PTR(evt_hashi))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_12_02_LOCK)
        CALL(evt_mobj_lock, STRING("mobj_lock00"), 13, 595, 160, -50, 270, PTR(evt_chk), PTR(evt_unlock), GSWF_GON_04_DOOR_UNLOCKED)
        SET_VALUE(LW(0), STRING("e_bero_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 6, 550, 960, -125, 0, 0)
    CALL(evt_mobj_powerupblk, STRING("pblk1"), -250, 60, -130, 0, GSWF_SHINE_13, 0)
    CALL(evt_map_playanim, STRING("hokori"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_paper_entry, STRING("OFF_gon_04_2"))
    SET_VALUE(GF(10), 0)
    RETURN
EVT_END()


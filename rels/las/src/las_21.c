#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_party.h"
#include "driver/mapdrv.h"

#include "party.h"
#include "manager/evtmgr_cmd.h"
#include "driver/mobjdrv.h"
#include "evt/evt_bero.h"
#include "las.h"

USER_FUNC(evt_mario_get_name_hitobj_head);
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(set_mobj_y_position);
static USER_FUNC(set_party_y_pos);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e1_door_close[22];
static EvtWord e1_door_open[22];
static EvtWord e2_door_close[22];
static EvtWord e2_door_open[22];
static BeroEntry bero_entry_data[5];
static BeroEntry w_bero_mapchg[1];
EvtWord floatblk_stop_se[47];
EvtWord floatblk_shake[27];
EvtWord floatblk_evt[336];
EvtWord floatblk21_1_evt[62];
EvtWord floatblk21_2_evt[62];
EvtWord las_21_init_evt[211];

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
        "las_19", // nextMapName
        "e_bero_1", // nextBeroName
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
        "las_23", // nextMapName
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
        "las_23", // nextMapName
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
        "s_bero_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static BeroEntry w_bero_mapchg[] = {
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
        "las_19", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
};

EVT_BEGIN(floatblk_stop_se)
    CALL(evt_mobj_get_position, LW(15), LW(0), LW(1), LW(2))
    CALL(evt_mobj_get_kindname, LW(15), LW(3))
    SET_VALUE(LF(0), 0)
    IF_STR_EQUAL(LW(3), STRING("MOBJ_green_big02_FlowBlock"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(3), STRING("MOBJ_purple_big02_FlowBlock"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(3), STRING("MOBJ_orange_big02_FlowBlock"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_STOP1"), LW(0), LW(1), LW(2), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(floatblk_shake)
    CALL(evt_sub_intpl_init, 11, 10, 3, 4)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, FLOAT(0.0f), LW(0), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(floatblk_evt)
    SET_VALUE(LW(15), LW(0))
    SET_VALUE(LW(14), LW(1))
    SET_VALUE(LW(13), LW(2))
    SET_VALUE(LW(12), LW(3))
    SET_VALUE(LW(11), LW(4))
    IF_INT_LESS(LW(12), LW(11))
        SET_FLOAT_VALUE(LW(10), FLOAT(0.099609375f))
        SET_FLOAT_VALUE(LW(9), FLOAT(0.049804688f))
        SET_VALUE(LF(1), 0)
        ELSE
        SET_FLOAT_VALUE(LW(10), FLOAT(-0.099609375f))
        SET_FLOAT_VALUE(LW(9), FLOAT(-0.049804688f))
        SET_VALUE(LF(1), 1)
    END_IF
    SPAWN_EVENT(PTR(floatblk_shake))
    WAIT_MS(500)
    CALL(evt_mobj_get_position, LW(15), LW(0), LW(1), LW(2))
    CALL(evt_mobj_get_kindname, LW(15), LW(3))
    SET_VALUE(LF(0), 0)
    IF_STR_EQUAL(LW(3), STRING("MOBJ_green_big02_FlowBlock"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(3), STRING("MOBJ_purple_big02_FlowBlock"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_STR_EQUAL(LW(3), STRING("MOBJ_orange_big02_FlowBlock"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(1), 0)
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_MOVE1"), LW(0), LW(1), LW(2), LW(8))
            ELSE
            CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_MOVE1"), LW(0), LW(1), LW(2), LW(8))
        END_IF
        ELSE
        IF_INT_EQUAL(LF(0), 1)
            CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_B_FLOAT_DOWN1"), LW(0), LW(1), LW(2), 0)
            ELSE
            CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLOCK_S_FLOAT_DOWN1"), LW(0), LW(1), LW(2), 0)
        END_IF
        SET_VALUE(LW(8), -1)
    END_IF
    CALL(evt_mobj_get_y_position, LW(15), LW(7))
    LOOP(0)
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(LF(1), 1)
            CALL(evt_mario_get_name_hitobj_head, LW(0))
            IF_STR_EQUAL(LW(0), LW(14))
                SET_VALUE(LF(0), 1)
            END_IF
            CALL(evt_mario_get_party, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_party_get_name_hitobj_head, 0, LW(0))
                IF_STR_EQUAL(LW(0), LW(14))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(LF(0), 0)
            GOTO(50)
        END_IF
        CALL(evt_mobj_exec_cancel, LW(13))
        SET_VALUE(LW(0), LW(12))
        SET_VALUE(LW(12), LW(11))
        SET_VALUE(LW(11), LW(0))
        IF_INT_EQUAL(LF(1), 0)
            SET_FLOAT_VALUE(LW(10), FLOAT(-0.099609375f))
            SET_FLOAT_VALUE(LW(9), FLOAT(-0.049804688f))
            SET_VALUE(LF(1), 1)
            ELSE
            SET_FLOAT_VALUE(LW(10), FLOAT(0.099609375f))
            SET_FLOAT_VALUE(LW(9), FLOAT(0.099609375f))
            SET_VALUE(LF(1), 0)
        END_IF
        LABEL(50)
        CALL(evt_mobj_get_y_position, LW(15), LW(0))
        ADD_FLOAT(LW(0), LW(10))
        CALL(set_mobj_y_position, LW(15), LW(0))
        SET_VALUE(LF(0), 0)
        IF_INT_EQUAL(LF(1), 0)
            IF_INT_GREATER_OR_EQUAL(LW(0), LW(11))
                CALL(evt_mobj_set_y_position, LW(15), LW(11))
                SET_VALUE(LF(0), 1)
            END_IF
            ELSE
            IF_INT_LESS_OR_EQUAL(LW(0), LW(11))
                CALL(evt_mobj_set_y_position, LW(15), LW(11))
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        CALL(evt_mobj_get_position, LW(15), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfx_pos, LW(8), LW(0), LW(1), LW(2))
        IF_INT_NOT_EQUAL(LF(1), 0)
            CALL(evt_mario_get_name_hitobj_ride, LW(1))
            IF_STR_EQUAL(LW(1), LW(14))
                CALL(evt_mario_set_hosei_xyz, FLOAT(0.0f), LW(10), FLOAT(0.0f))
            END_IF
            CALL(evt_mario_get_party, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_party_get_name_hitobj_ride, 0, LW(0))
                IF_STR_EQUAL(LW(0), LW(14))
                    CALL(evt_party_set_hosei_xyz, 0, FLOAT(0.0f), LW(10), FLOAT(0.0f))
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(LF(0), 1)
            BREAK_LOOP
        END_IF
        ADD_FLOAT(LW(10), LW(9))
        IF_INT_LESS_OR_EQUAL(LW(10), FLOAT(-2.0f))
            SET_FLOAT_VALUE(LW(10), FLOAT(-2.0f))
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(10), FLOAT(2.0f))
            SET_FLOAT_VALUE(LW(10), FLOAT(2.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(8))
    SPAWN_EVENT(PTR(floatblk_stop_se))
    IF_INT_EQUAL(LF(1), 1)
        SPAWN_EVENT(PTR(floatblk_shake))
        WAIT_MS(1000)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(floatblk21_1_evt)
    SET_TYPE(239)
    IF_INT_EQUAL(LSWF(8), 0)
        SET_VALUE(LW(0), STRING("21blk1"))
        SET_VALUE(LW(1), STRING("MOBJ_21blk1"))
        SET_VALUE(LW(2), STRING("21switch1"))
        SET_VALUE(LW(3), -130)
        SET_VALUE(LW(4), -580)
        SET_VALUE(LSWF(8), 1)
        ELSE
        SET_VALUE(LW(0), STRING("21blk1"))
        SET_VALUE(LW(1), STRING("MOBJ_21blk1"))
        SET_VALUE(LW(2), STRING("21switch1"))
        SET_VALUE(LW(3), -580)
        SET_VALUE(LW(4), -130)
        SET_VALUE(LSWF(8), 0)
    END_IF
    CALL_CHILD(PTR(floatblk_evt))
    CALL(evt_mobj_get_y_position, STRING("21blk1"), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), -130)
        SET_VALUE(LSWF(8), 0)
        ELSE
        SET_VALUE(LSWF(8), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(floatblk21_2_evt)
    SET_TYPE(239)
    IF_INT_EQUAL(LSWF(9), 0)
        SET_VALUE(LW(0), STRING("21blk2"))
        SET_VALUE(LW(1), STRING("MOBJ_21blk2"))
        SET_VALUE(LW(2), STRING("21switch2"))
        SET_VALUE(LW(3), -130)
        SET_VALUE(LW(4), -580)
        SET_VALUE(LSWF(9), 1)
        ELSE
        SET_VALUE(LW(0), STRING("21blk2"))
        SET_VALUE(LW(1), STRING("MOBJ_21blk2"))
        SET_VALUE(LW(2), STRING("21switch2"))
        SET_VALUE(LW(3), -580)
        SET_VALUE(LW(4), -130)
        SET_VALUE(LSWF(9), 0)
    END_IF
    CALL_CHILD(PTR(floatblk_evt))
    CALL(evt_mobj_get_y_position, STRING("21blk2"), LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), -130)
        SET_VALUE(LSWF(9), 0)
        ELSE
        SET_VALUE(LSWF(9), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(las_21_init_evt)
    IF_INT_LESS(GSW_STORY_PROGRESS, 390)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST1"))
        ELSE
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST3"))
    END_IF
    CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_LESS(GSW_STORY_PROGRESS, 390)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_under"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_under"), 1, 0)
        CALL(evt_npc_check_delete, STRING("ワンワン２"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_yuka"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_be"), 1, 0)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("w_bero"), PTR(w_bero_mapchg))
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_npc_check_delete, STRING("ワンワン１"))
        SPAWN_EVENT(PTR(hikari_evt))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 390)
        IF_INT_EQUAL(GSWF(4353), 0)
            SET_VALUE(LW(0), -220)
            ELSE
            SET_VALUE(LW(0), -580)
        END_IF
        IF_INT_EQUAL(GSWF(4354), 0)
            SET_VALUE(LW(1), -120)
            ELSE
            SET_VALUE(LW(1), -580)
        END_IF
        SET_VALUE(LW(2), -580)
        SET_VALUE(LSWF(8), 1)
        SET_VALUE(LW(3), -580)
        SET_VALUE(LSWF(9), 1)
        CALL(evt_mobj_float_blk, STRING("19blk1"), -350, LW(0), 0, 2, 2, 0, 0)
        CALL(evt_mobj_float_blk, STRING("19blk2"), -250, LW(1), 0, 0, 2, 0, 0)
        CALL(evt_mobj_float_blk, STRING("21blk1"), 50, LW(2), 0, 1, 2, 0, 0)
        CALL(evt_mobj_float_blk, STRING("21blk2"), 150, LW(3), 0, 3, 2, 0, 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("19blk1"), MOBJ_FLAG_NO_CULL)
        CALL(evt_mobj_flag_onoff, 1, STRING("19blk2"), MOBJ_FLAG_NO_CULL)
        CALL(evt_mobj_flag_onoff, 1, STRING("21blk1"), MOBJ_FLAG_NO_CULL)
        CALL(evt_mobj_flag_onoff, 1, STRING("21blk2"), MOBJ_FLAG_NO_CULL)
        CALL(evt_mobj_switch_float_blk, STRING("21switch1"), 175, -480, -67, STRING("21blk1"), 1, PTR(floatblk21_1_evt), 0)
        CALL(evt_mobj_switch_float_blk, STRING("21switch2"), 300, -30, 15, STRING("21blk2"), 3, PTR(floatblk21_2_evt), 0)
    END_IF
    CALL(evt_mobj_jumpstand_blue, 0, STRING("jump"), 700, -650, -580, 75, 0, 0)
    CALL(evt_mobj_flag_onoff, 0, STRING("jump"), MOBJ_FLAG_GRAVITY)
    RETURN
EVT_END()

static USER_FUNC(set_party_y_pos) {
    EvtWord* args = event->args;
    PartyEntry* party = partyGetPtr(0);

    if (party == NULL) {
        return EVT_RETURN_DONE;
    }
    party->position.y = evtGetFloat(event, args[0]);
    return EVT_RETURN_DONE;
}
USER_FUNC(set_mobj_y_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    MapObjectEntry* mobj = mobjNameToPtr(name);

    mobj->position.y = y;
    mobjCalcMtx(mobj);
    return EVT_RETURN_DONE;
}

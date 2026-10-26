#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_mobj.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "party.h"
#include "evt/evt_bero.h"
#include "jin.h"

USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mario_get_name_hitobj_push);
static USER_FUNC(get_pos);
USER_FUNC(get_width);
USER_FUNC(set_pos);

static EvtWord door_close[12];
static EvtWord door_open[12];
static BeroEntry bero_entry_data[3];
static EvtWord push_evt[132];
EvtWord unpush_evt[252];
EvtWord vivi_chk[42];
EvtWord jin_10_init_evt[59];

static USER_FUNC(get_pos) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();

    evtSetFloat(event, args[0], mario->position.x);
    evtSetFloat(event, args[1], mario->position.y);
    evtSetFloat(event, args[2], mario->position.z);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door11"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door11"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_09", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_11", // nextMapName
        "s_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door_close, // enterAnimeArg
        door_open, // exitAnimeArg
    },
    {0},
};

USER_FUNC(set_pos) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    MarioWork* mario = marioGetPtr();

    mario->position = (Vec){x, y, z};
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(push_evt)
    IF_INT_EQUAL(GF(0), 0)
        CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_GRID_MOVE1"), LW(0), LW(1), LW(2), GW(3))
        SET_VALUE(GF(0), 1)
        ELSE
        CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfx_pos, GW(3), LW(0), LW(1), LW(2))
    END_IF
    CALL(evt_mario_push)
    CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    IF_INT_LESS(LW(3), LW(0))
        SET_FLOAT_VALUE(LW(10), FLOAT(0.5f))
        ELSE
        SET_FLOAT_VALUE(LW(10), FLOAT(-1.0f))
    END_IF
    ADD_FLOAT(GW(10), LW(10))
    IF_FLOAT_GREATER_OR_EQUAL(GW(10), FLOAT(265.0f))
        SET_FLOAT_VALUE(GW(10), FLOAT(265.0f))
        SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
        IF_INT_EQUAL(GF(0), 1)
            CALL(evt_snd_sfxoff, GW(3))
            SET_VALUE(GF(0), 0)
        END_IF
    END_IF
    IF_FLOAT_LESS_OR_EQUAL(GW(10), FLOAT(0.0f))
        SET_FLOAT_VALUE(GW(10), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
        IF_INT_EQUAL(GF(0), 1)
            CALL(evt_snd_sfxoff, GW(3))
            SET_VALUE(GF(0), 0)
        END_IF
    END_IF
    SET_FLOAT_VALUE(GW(11), GW(10))
    DIVIDE_FLOAT(GW(11), FLOAT(4.0f))
    CALL(evt_mapobj_trans, 1, STRING("saku"), GW(10), GW(11), 0)
    CALL(evt_hit_bind_update, STRING("a_saku"))
    CALL(get_pos, LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(0), LW(10))
    CALL(set_pos, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

EVT_BEGIN(unpush_evt)
    LOOP(0)
        LOOP(0)
            CALL(evt_mario_get_name_hitobj_push, LW(0))
            IF_STR_NOT_EQUAL(LW(0), STRING("a_saku"))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LF(0), 1)
        CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        IF_INT_LESS(LW(3), LW(0))
            LOOP(0)
                CALL(evt_party_get_status, 6, LW(0))
                IF_INT_EQUAL(LW(0), 2)
                    BREAK_LOOP
                END_IF
                CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
                CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
                SUBTRACT_FLOAT(LW(0), LW(3))
                IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
                END_IF
                IF_FLOAT_LESS(LW(0), FLOAT(20.0f))
                    SET_VALUE(LF(0), 0)
                    BREAK_LOOP
                END_IF
                CALL(evt_mario_get_party, LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
                    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
                    CALL(get_width, 0, LW(6))
                    SUBTRACT_FLOAT(LW(0), LW(3))
                    IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
                    END_IF
                    IF_FLOAT_LESS(LW(0), LW(6))
                        SET_VALUE(LF(0), 0)
                        BREAK_LOOP
                    END_IF
                END_IF
                CALL(evt_mario_get_exparty, LW(0))
                IF_INT_NOT_EQUAL(LW(0), 0)
                    CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
                    CALL(evt_party_get_pos, 1, LW(3), LW(4), LW(5))
                    CALL(get_width, 1, LW(6))
                    SUBTRACT_FLOAT(LW(0), LW(3))
                    IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
                    END_IF
                    IF_FLOAT_LESS(LW(0), LW(6))
                        SET_VALUE(LF(0), 0)
                        BREAK_LOOP
                    END_IF
                END_IF
                BREAK_LOOP
            END_LOOP
        END_IF
        IF_INT_EQUAL(LF(0), 0)
            IF_INT_EQUAL(GF(0), 1)
                CALL(evt_snd_sfxoff, GW(3))
                SET_VALUE(GF(0), 0)
            END_IF
            ELSE
            SUBTRACT_FLOAT(GW(10), FLOAT(0.5f))
            IF_FLOAT_LESS(GW(10), FLOAT(0.0f))
                SET_FLOAT_VALUE(GW(10), FLOAT(0.0f))
                IF_INT_EQUAL(GF(0), 1)
                    CALL(evt_snd_sfxoff, GW(3))
                    SET_VALUE(GF(0), 0)
                END_IF
                ELSE
                IF_INT_EQUAL(GF(0), 0)
                    CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
                    CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_GRID_MOVE1"), LW(0), LW(1), LW(2), GW(3))
                    SET_VALUE(GF(0), 1)
                END_IF
            END_IF
            SET_FLOAT_VALUE(GW(11), GW(10))
            DIVIDE_FLOAT(GW(11), FLOAT(4.0f))
            CALL(evt_mapobj_trans, 1, STRING("saku"), GW(10), GW(11), 0)
            CALL(evt_hit_bind_update, STRING("a_saku"))
            IF_INT_EQUAL(GF(0), 1)
                CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfx_pos, GW(3), LW(0), LW(1), LW(2))
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(vivi_chk)
    LOOP(0)
        CALL(evt_party_get_status, 6, LW(0))
        IF_INT_EQUAL(LW(0), 2)
            CALL(evt_mapobj_get_position, STRING("saku"), LW(0), LW(1), LW(2))
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            SUBTRACT_FLOAT(LW(0), LW(3))
            IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
            END_IF
            IF_FLOAT_LESS(LW(0), FLOAT(25.0f))
                ELSE
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(jin_10_init_evt)
    SET_VALUE(GF(0), 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_JIN1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_mobj_badgeblk, STRING("m"), 60, 60, -25, ITEM_TORNADO_JUMP, 0, GSWF_JIN_10_TORNADO_JUMP_BADGE_BLOCK, 1)
    SET_FLOAT_VALUE(GW(10), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(11), FLOAT(0.0f))
    CALL(evt_hit_bind_mapobj, STRING("a_saku"), STRING("saku"))
    CALL(evt_run_case_evt, CASE_PUSH, 1, STRING("a_saku"), 0, PTR(push_evt), 0)
    SPAWN_EVENT(PTR(unpush_evt))
    SPAWN_EVENT(PTR(vivi_chk))
    RETURN
EVT_END()

USER_FUNC(get_width) {
    EvtWord* args = event->args;
    PartyEntry* party = partyGetPtr(evtGetValue(event, args[0]));

    evtSetFloat(event, args[1], 0.8f * party->width);
    return EVT_RETURN_DONE;
}


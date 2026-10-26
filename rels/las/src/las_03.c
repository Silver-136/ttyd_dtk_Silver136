#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "driver/hitdrv.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "system.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"

u16 toge_flag;

USER_FUNC(get_toge_name);
USER_FUNC(toge_check_flag);
USER_FUNC(toge_flag_onoff);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
static HitDamageReturn dmgRetPoint[3];
const char* toge_data[18];
EvtWord toge_evt_sub[121];
EvtWord toge_evt[86];
EvtWord las_03_init_evt[32];

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
        "las_02", // nextMapName
        "e_bero", // nextBeroName
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
        "las_04", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

static HitDamageReturn dmgRetPoint[] = {
    {"A_yuka_02", {0.0f, -1000.0f, 0.0f}},
    {"A_yuka_03", {0.0f, -1000.0f, 0.0f}},
    {0},
};

const char* toge_data[] = {
    "S_toge_01",
    "A_toge_01",
    "S_toge_02",
    "A_toge_02",
    "S_toge_03",
    "A_toge_03",
    "S_toge_04",
    "A_toge_04",
    "S_toge_05",
    "A_toge_05",
    "S_toge_06",
    "A_toge_06",
    "S_toge_07",
    "A_toge_07",
    "S_toge_08",
    "A_toge_08",
    "S_toge_09",
    "A_toge_09",
};

EVT_BEGIN(toge_evt_sub)
    SET_VALUE(LW(2), LW(0))
    CALL(get_toge_name, LW(2), LW(3), LW(4))
    CALL(toge_check_flag, LW(2), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(toge_flag_onoff, LW(2), 1)
    CALL(evt_mapobj_get_position, LW(3), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_FLOOR_TRAP1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 30, 150)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 0, LW(3), 0, LW(0), 0)
    CALL(evt_hit_bind_update, LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    WAIT_MS(1000)
    CALL(evt_mapobj_get_position, LW(3), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_FLOOR_TRAP2"), LW(7), LW(8), LW(9), 0)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 30, 0, 400)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 0, LW(3), 0, LW(0), 0)
    CALL(evt_hit_bind_update, LW(4))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_mapobj_trans, 0, LW(3), 0, 0, 0)
    CALL(evt_hit_bind_update, LW(4))
    CALL(toge_flag_onoff, LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(toge_evt)
    CALL(evt_hit_damage_return_set, PTR(dmgRetPoint))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    SET_INT_READ_ADDR(PTR(&toge_data))
    LOOP(9)
        READ_INT2(LW(0), LW(1))
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_LOOP
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    LOOP(0)
        CALL(evt_mario_get_motion, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 32)
            CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
            SET_VALUE(LW(4), 0)
            LOOP(0)
                CALL(get_toge_name, LW(4), LW(5), LW(6))
                CALL(evt_mapobj_get_position, LW(5), LW(6), LW(7), LW(8))
                CALL(evt_sub_area_check, 1, LW(6), LW(8), 90, LW(1), LW(3), LW(9))
                IF_INT_EQUAL(LW(9), 1)
                    SET_VALUE(LW(0), LW(4))
                    SPAWN_EVENT(PTR(toge_evt_sub))
                END_IF
                ADD(LW(4), 1)
                IF_INT_GREATER_OR_EQUAL(LW(4), 9)
                    BREAK_LOOP
                END_IF
            END_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(las_03_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_mobj_badgeblk, STRING("mobj_badge_gat"), -350, 0, -100, 264, 0, GSWF(4375), 1)
    SPAWN_EVENT(PTR(toge_evt))
    RETURN
EVT_END()

USER_FUNC(get_toge_name) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (EvtWord)toge_data[idx * 2]);
    evtSetValue(event, args[2], (EvtWord)toge_data[idx * 2 + 1]);
    return EVT_RETURN_DONE;
}
USER_FUNC(toge_check_flag) {
    EvtWord* args = event->args;
    s32 bit = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], toge_flag & (1 << bit));
    return EVT_RETURN_DONE;
}
USER_FUNC(toge_flag_onoff) {
    EvtWord* args = event->args;
    s32 bit = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);

    if (on) {
        toge_flag |= (1 << bit);
    } else {
        toge_flag &= ~(1 << bit);
    }
    return EVT_RETURN_DONE;
}

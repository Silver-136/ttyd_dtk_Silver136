#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_torch.h"
#include "evt/evt_map.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "pmario_sound.h"

#include "driver/mapdrv.h"
#include "driver/hitdrv.h"
#include "driver/effdrv.h"
#include "evt/evt_bero.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"

USER_FUNC(hi_onoff);
USER_FUNC(mugen_kairou_get_list);

static EvtWord w1_door_close[22];
static EvtWord w1_door_open[22];
static EvtWord w2_door_close[22];
static EvtWord w2_door_open[22];
static EvtWord e1_door_close[22];
static EvtWord e1_door_open[22];
static EvtWord e2_door_close[22];
static EvtWord e2_door_open[22];
static EvtWord door_matigai_evt[15];
EvtWord door_seikai_evt[19];
static BeroEntry bero_entry_data[5];
BeroEntry e_bero_1_matigai[1];
BeroEntry e_bero_1_seikai[1];
BeroEntry e_bero_2_seikai[1];
const char* taimatu_data[16];
EvtWord hikari_anim[52];
EvtWord taimatu_evt[18];
s32 mugen_kairou_list[7];
EvtWord mugen_kairou_init[96];
EvtWord las_08_init_evt[22];

static EVT_BEGIN(w1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l1_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l2_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r1_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r1_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r1_02"), 0, LW(0), 0)
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

static EVT_BEGIN(door_matigai_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_01_02_LOCK)
        SET_VALUE(GSW_LAS_08_CORRECT_DOOR_COUNT, 0)
        SET_VALUE(GSWF_LAS_WRONG_DOOR_SFX_PENDING, 1)
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(door_seikai_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_01_02_LOCK)
        ADD(GSW_LAS_08_CORRECT_DOOR_COUNT, 1)
        IF_INT_GREATER_OR_EQUAL(GSW_LAS_08_CORRECT_DOOR_COUNT, 7)
            SET_VALUE(GSW_STORY_PROGRESS, STG8_02_FOLLOW_THE_LIT_TORCHES)
        END_IF
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        door_matigai_evt, // exitEvent
        "las_07", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w1_door_close, // enterAnimeArg
        w1_door_open, // exitAnimeArg
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
        door_seikai_evt, // exitEvent
        "las_09", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        door_matigai_evt, // exitEvent
        "las_07", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w2_door_close, // enterAnimeArg
        w2_door_open, // exitAnimeArg
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
        door_matigai_evt, // exitEvent
        "las_07", // nextMapName
        "w_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_door_close, // enterAnimeArg
        e2_door_open, // exitAnimeArg
    },
    {0},
};

BeroEntry e_bero_1_matigai[] = {
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
        door_matigai_evt, // exitEvent
        "las_07", // nextMapName
        "w_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
};

BeroEntry e_bero_1_seikai[] = {
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
        door_seikai_evt, // exitEvent
        "las_08", // nextMapName
        "w_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e1_door_close, // enterAnimeArg
        e1_door_open, // exitAnimeArg
    },
};

BeroEntry e_bero_2_seikai[] = {
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
        door_seikai_evt, // exitEvent
        "las_08", // nextMapName
        "w_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e2_door_close, // enterAnimeArg
        e2_door_open, // exitAnimeArg
    },
};

const char* taimatu_data[] = {
    "S_hikari_01",
    "A_hi_01",
    "S_hikari_02",
    "A_hi_02",
    "S_hikari_03",
    "A_hi_03",
    "S_hikari_04",
    "A_hi_04",
    "S_hikari_05",
    "A_hi_05",
    "S_hikari_06",
    "A_hi_06",
    "S_hikari_07",
    "A_hi_07",
    "S_hikari_08",
    "A_hi_08",
};

EVT_BEGIN(hikari_anim)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 65536)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_sub_get_sincos, LW(0), LW(1), LW(2))
        MULTIPLY_FLOAT(LW(1), FLOAT(0.19921875f))
        ADD_FLOAT(LW(1), FLOAT(0.7998047f))
        SET_INT_READ_ADDR(PTR(&taimatu_data))
        LOOP(8)
            READ_INT2(LW(3), LW(4))
            CALL(evt_mapobj_scale, 0, LW(3), LW(1), LW(1), LW(1))
        END_LOOP
        ADD(LW(0), 2)
        IF_INT_GREATER_OR_EQUAL(LW(0), 360)
            SUBTRACT(LW(0), 360)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(taimatu_evt)
    SET_INT_READ_ADDR(PTR(&taimatu_data))
    LOOP(8)
        READ_INT2(LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, 0, 1, LW(0), MAP_OBJ_FLAG_HIDDEN)
    END_LOOP
    SPAWN_EVENT(PTR(hikari_anim))
    RETURN
EVT_END()

s32 mugen_kairou_list[] = {
    7, 7, 3, 3, 7, 3, 7,
};

EVT_BEGIN(mugen_kairou_init)
    CALL_CHILD(PTR(taimatu_evt))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_01_02_LOCK)
        CALL(mugen_kairou_get_list, GSW_LAS_08_CORRECT_DOOR_COUNT, LW(0))
        CALL(hi_onoff, LW(0), 1)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_bero_exec_wait, 65536)
        CALL(mugen_kairou_get_list, GSW_LAS_08_CORRECT_DOOR_COUNT, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(7)
            IF_INT_LESS(GSW_LAS_08_CORRECT_DOOR_COUNT, 6)
                SET_VALUE(LW(0), STRING("e_bero_1"))
                CALL_CHILD(PTR(bero_case_switch_off))
                CALL(evt_bero_overwrite, STRING("e_bero_1"), PTR(e_bero_1_seikai))
                SET_VALUE(LW(0), STRING("e_bero_1"))
                CALL_CHILD(PTR(bero_case_switch_on))
            END_IF
            CASE_EQUAL(3)
            SET_VALUE(LW(0), STRING("e_bero_2"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_bero_overwrite, STRING("e_bero_2"), PTR(e_bero_2_seikai))
            SET_VALUE(LW(0), STRING("e_bero_2"))
            CALL_CHILD(PTR(bero_case_switch_on))
            SET_VALUE(LW(0), STRING("e_bero_1"))
            CALL_CHILD(PTR(bero_case_switch_off))
            CALL(evt_bero_overwrite, STRING("e_bero_1"), PTR(e_bero_1_matigai))
            SET_VALUE(LW(0), STRING("e_bero_1"))
            CALL_CHILD(PTR(bero_case_switch_on))
        END_SWITCH
        ELSE
        CALL(hi_onoff, 4, 1)
        CALL(hi_onoff, 5, 1)
        CALL(hi_onoff, 6, 1)
        CALL(hi_onoff, 7, 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(las_08_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(mugen_kairou_init))
    RETURN
EVT_END()


USER_FUNC(hi_onoff) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    MapObject* obj = mapGetMapObj((const char*)taimatu_data[index * 2]);
    Vec pos;

    if (on) {
        obj->flags &= ~MAP_OBJ_FLAG_HIDDEN;
        hitObjGetPos((const char*)taimatu_data[index * 2 + 1], &pos);
        effSetName(effTorchEntry(0, pos.x, pos.y, pos.z, 0.7f), (const char*)taimatu_data[index * 2 + 1]);
    } else {
        obj->flags |= MAP_OBJ_FLAG_HIDDEN;
        effSoftDelete(effNameToPtr((const char*)taimatu_data[index * 2 + 1]));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(mugen_kairou_get_list) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], mugen_kairou_list[index]);
    return EVT_RETURN_DONE;
}

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "jin.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"

USER_FUNC(evt_kagemario_init);

static EvtWord n_close[12];
static EvtWord n_open[12];
static EvtWord s_close[12];
static EvtWord s_open[12];
static EvtWord w1_close[12];
static EvtWord w1_open[12];
static EvtWord w2_close[12];
static EvtWord w2_open[12];
EvtWord w3_close[12];
EvtWord w3_open[12];
static BeroEntry bero_entry_data[6];
EvtWord jin_03_kaidan_temae_kesu[13];
EvtWord jin_03_kaidan_naka_kesu[13];
EvtWord jin_03_kaidan_oku_kesu[13];
EvtWord jin_03_init_evt[55];

static EVT_BEGIN(n_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w1_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00A"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w1_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00A"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00B"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w2_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00B"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(w3_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00C"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(w3_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00C"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
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
        "jin_01", // nextMapName
        "s_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_close, // enterAnimeArg
        n_open, // exitAnimeArg
    },
    {
        "s_bero", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_02", // nextMapName
        "n_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_close, // enterAnimeArg
        s_open, // exitAnimeArg
    },
    {
        "w_bero3", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_00", // nextMapName
        "e_bero_3", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w1_close, // enterAnimeArg
        w1_open, // exitAnimeArg
    },
    {
        "w_bero1", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_00", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w3_close, // enterAnimeArg
        w3_open, // exitAnimeArg
    },
    {
        "w_bero2", // hitName
        1, // type
        2, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "jin_00", // nextMapName
        "e_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w2_close, // enterAnimeArg
        w2_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(jin_03_kaidan_temae_kesu)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kaidan01"), 1)
    CALL(evt_hitobj_onoff, STRING("a_kaidan01"), 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(jin_03_kaidan_naka_kesu)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kaidan02"), 1)
    CALL(evt_hitobj_onoff, STRING("a_kaidan02"), 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(jin_03_kaidan_oku_kesu)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("kaidan03"), 1)
    CALL(evt_hitobj_onoff, STRING("a_kaidan03"), 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(jin_03_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG4_JIN1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    IF_INT_EQUAL(LSW(1), 0)
        SPAWN_EVENT(PTR(jin_03_kaidan_temae_kesu))
        SPAWN_EVENT(PTR(jin_03_kaidan_oku_kesu))
        ELSE
        IF_INT_EQUAL(LSW(1), 1)
            SPAWN_EVENT(PTR(jin_03_kaidan_naka_kesu))
            SPAWN_EVENT(PTR(jin_03_kaidan_oku_kesu))
            ELSE
            SPAWN_EVENT(PTR(jin_03_kaidan_temae_kesu))
            SPAWN_EVENT(PTR(jin_03_kaidan_naka_kesu))
        END_IF
    END_IF
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 125, 0, 0, STRING("S_yuka"), 125, GSWF(5634))
    RETURN
EVT_END()


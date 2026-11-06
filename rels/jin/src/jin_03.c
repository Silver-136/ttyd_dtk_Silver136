// jin_03 (ダイム寺院, Creepy Steeple): staircase room joining jin_00, jin_01 and jin_02,
// with a spinning (kururin) floor tile.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "jin.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"


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

/**
 * Door animation callback: closes door01 as Mario arrives through n_bero (from jin_01);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(n_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door01 as Mario leaves through n_bero (to jin_01);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(n_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door01"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: closes door02 as Mario arrives through s_bero (from jin_02);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(s_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door02 as Mario leaves through s_bero (to jin_02);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(s_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door02"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: closes door00A as Mario arrives through w_bero3 (from jin_00);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(w1_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00A"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door00A as Mario leaves through w_bero3 (to jin_00);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(w1_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00A"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: closes door00B as Mario arrives through w_bero2 (from jin_00);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it.
 */
static EVT_BEGIN(w2_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00B"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation callback: opens door00B as Mario leaves through w_bero2 (to jin_00);
 * LW(0) is the door angle and the door turns around Y by 0.44 times it, negated.
 */
static EVT_BEGIN(w2_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00B"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Door animation callback for the door00C door's closing: rotates it by 0.444 times the value in LW(0). */
EVT_BEGIN(w3_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00C"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Door animation callback for the door00C door's opening: rotates it by -0.444 times the value in LW(0). */
EVT_BEGIN(w3_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door00C"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Doors of this room: the north exit (n_bero) to jin_01, the south exit (s_bero) to jin_02, the west exit
 * (w_bero3) to jin_00, the west exit (w_bero1) to jin_00 and the west exit (w_bero2) to jin_00.
 */
static BeroEntry bero_entry_data[] = {
    {
        "n_bero", // hitName
        1, // type
        2, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Hides the first staircase section (kaidan01) and its collision. */
EVT_BEGIN(jin_03_kaidan_temae_kesu)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kaidan01"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_kaidan01"), 1, 0)
    RETURN
EVT_END()

/** Hides the middle staircase section (kaidan02) and its collision. */
EVT_BEGIN(jin_03_kaidan_naka_kesu)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kaidan02"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_kaidan02"), 1, 0)
    RETURN
EVT_END()

/** Hides the last staircase section (kaidan03) and its collision. */
EVT_BEGIN(jin_03_kaidan_oku_kesu)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("kaidan03"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_kaidan03"), 1, 0)
    RETURN
EVT_END()

/**
 * Creepy Steeple jin_03 map init: starts the steeple music and reverb, the shadow-Mario tint and the exits,
 * hides two of the three staircase sections according to the stair stage LSW_JIN_STAIRCASE_POS (set by the switches in jin_01
 * and jin_02) and adds the spinning floor tile that hides a Star Piece.
 */
// Position 0 keeps the middle stairs, 1 keeps the front stairs, and other values keep the rear stairs.
EVT_BEGIN(jin_03_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_JIN1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_JIN1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL_CHILD(evt_usu_kagemario_party_kill)
    IF_INT_EQUAL(LSW_JIN_STAIRCASE_POS, 0)
        SPAWN_EVENT(jin_03_kaidan_temae_kesu)
        SPAWN_EVENT(jin_03_kaidan_oku_kesu)
    ELSE
        IF_INT_EQUAL(LSW_JIN_STAIRCASE_POS, 1)
            SPAWN_EVENT(jin_03_kaidan_naka_kesu)
            SPAWN_EVENT(jin_03_kaidan_oku_kesu)
        ELSE
            SPAWN_EVENT(jin_03_kaidan_temae_kesu)
            SPAWN_EVENT(jin_03_kaidan_naka_kesu)
        END_IF
    END_IF
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), 125, 0, 0, STRING("S_yuka"), 125, GSWF_STARPIECE_67)
    RETURN
EVT_END()


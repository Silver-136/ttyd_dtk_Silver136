// moo_05 (月, the Moon): a crater field between moo_01 and moo_02.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "moo.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[3];

#if VERSION < VERSION_US
static EvtWord rock_main[79];
static EvtWord rock_evt0[21];
#else
static EvtWord rock_evt0[15];
#endif

EvtWord moo_05_init_evt[59];

/** Exits of moo_05: west road w_bero to moo_01 (e_bero); east road e_bero to moo_02 (w_bero). */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "moo_01", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "moo_02", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

#if VERSION < VERSION_US

/**
 * Rock-blasting script (JP and demos): the rock crumbles into an offscreen animation,
 * then the model and collision of the rock in LW(10)/LW(11) are removed.
 */
static EVT_BEGIN(rock_main)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 0, LW(10), STRING("offscreen"))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_SINGLE, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("offscreen"), 0, 0, 0, 0, 0)
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_set_shadow, STRING("img"), 2)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_release, STRING("img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    CALL(evt_mapobj_clear_offscreen, 0, LW(10))
    CALL(evt_hitobj_onoff, LW(11), 1, 0)
    RETURN
EVT_END()
#endif

static EVT_BEGIN(rock_evt0)
#if VERSION >= VERSION_US
    SET_VALUE(GSWF_MOO_05_ROCK_07_BLOWN, 1)
#else
    CALL(evt_mario_key_onoff, 0)
#endif
    SET_VALUE(LW(10), STRING("S_iwa_07"))
    SET_VALUE(LW(11), STRING("A_iwa_07"))
    CALL_CHILD(rock_main)
#if VERSION < VERSION_US
    SET_VALUE(GSWF_MOO_05_ROCK_07_BLOWN, 1)
    CALL(evt_mario_key_onoff, 1)
#endif
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Map init script for moo_05: entry bero, Moon music and ambience,
 * star animations and the Bobbery rock (or its removal).
 */
EVT_BEGIN(moo_05_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_MON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_MOO1"))
    CALL(evt_map_playanim, STRING("hosi_01"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hosi_02"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hosi_03"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    IF_INT_EQUAL(GSWF_MOO_05_ROCK_07_BLOWN, 0)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_iwa_07"), 0, PTR(rock_evt0), 0)
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_iwa_07"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_iwa_07"), 1, 0)
    END_IF
    RETURN
EVT_END()


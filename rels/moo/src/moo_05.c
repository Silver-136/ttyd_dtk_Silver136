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

#ifdef VERSION_G8ME01
#endif

static BeroEntry bero_entry_data[3];
#ifdef VERSION_G8ME01
#else
static EvtWord rock_main[79];
#endif
#ifdef VERSION_G8ME01
static EvtWord rock_evt0[15];
#else
static EvtWord rock_evt0[21];
#endif
EvtWord moo_05_init_evt[59];

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
        "moo_01", // nextMapName
        "e_bero", // nextBeroName
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
        "moo_02", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

#ifndef VERSION_G8ME01
static EVT_BEGIN(rock_main)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, LW(10), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 0, LW(10), STRING("offscreen"))
    CALL(evt_mapobj_flag_onoff, 0, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
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
#ifdef VERSION_G8ME01
    SET_VALUE(GSWF(4031), 1)
#else
    CALL(evt_mario_key_onoff, 0)
#endif
    SET_VALUE(LW(10), STRING("S_iwa_07"))
    SET_VALUE(LW(11), STRING("A_iwa_07"))
    CALL_CHILD(PTR(rock_main))
#ifndef VERSION_G8ME01
    SET_VALUE(GSWF(4031), 1)
    CALL(evt_mario_key_onoff, 1)
#endif
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(moo_05_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_MON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_MOO1"))
    CALL(evt_map_playanim, STRING("hosi_01"), 1, 0)
    CALL(evt_map_playanim, STRING("hosi_02"), 1, 0)
    CALL(evt_map_playanim, STRING("hosi_03"), 1, 0)
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    IF_INT_EQUAL(GSWF(4031), 0)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_iwa_07"), 0, PTR(rock_evt0), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_iwa_07"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_iwa_07"), 1, 0)
    END_IF
    RETURN
EVT_END()


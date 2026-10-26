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
#include "evt/evt_item.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


EvtWord n_bero_in[25];
EvtWord n_bero_out[12];
static BeroEntry bero_entry_data[6];
#ifdef VERSION_G8ME01
static EvtWord dokan_evt[122];
#else
static EvtWord dokan_evt[102];
#endif
#ifdef VERSION_G8ME01
#else
static EvtWord rock_main[79];
#endif
#ifdef VERSION_G8ME01
static EvtWord rock_evt0[15];
#else
static EvtWord rock_evt0[21];
#endif
#ifdef VERSION_G8ME01
static EvtWord rock_evt1[15];
#else
static EvtWord rock_evt1[21];
#endif
EvtWord moo_02_init_evt[132];

EVT_BEGIN(n_bero_in)
    CALL(evt_mario_bgmode_on)
    CALL(evt_mario_kill_party, 0)
    CALL(evt_mario_cam_y_off)
    CALL(evt_hit_get_position, STRING("n_bero"), LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    CALL_CHILD(PTR(bero_case_entry))
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

EVT_BEGIN(n_bero_out)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_mapchange, STRING("aji_19"), STRING("w_bero"))
    SET_VALUE(LW(0), 1)
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
        "moo_05", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        4098, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        4099, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        n_bero_in, // enterEvent
        5, // caseType
        n_bero_out, // exitEvent
        "aji_00", // nextMapName
        "w_bero", // nextBeroName
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
        "moo_06", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(dokan_evt)
#ifdef VERSION_G8ME01
    SET_VALUE(GSW_STORY_PROGRESS, 359)
#else
    CALL(evt_mario_key_onoff, 0)
#endif
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_iwa_04"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 0, STRING("S_iwa_04"), STRING("offscreen"))
#ifdef VERSION_G8ME01
    CALL(evt_offscreen_get_boundingbox2, STRING("offscreen"), LW(0), LW(1), LW(2), LW(3))
    IF_INT_GREATER(LW(2), 0)
        IF_INT_GREATER(LW(3), 0)
#endif
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom_ver2"))
#ifdef VERSION_G8ME01
            CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("offscreen"), 1, 0, 0, 0, 0)
#else
    CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("offscreen"), 0, 0, 0, 0, 0)
#endif
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_set_shadow, STRING("img"), 2)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_iwa_04"), MAP_OBJ_FLAG_HIDDEN)
#ifdef VERSION_G8ME01
            CALL(evt_hitobj_onoff, STRING("A_iwa_04"), 1, 0)
#endif
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_free_capture, STRING("img"), 0)
    CALL(evt_img_set_shadow, STRING("img"), 0)
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_release, STRING("img"))
#ifdef VERSION_G8ME01
        END_IF
    END_IF
#endif
    CALL(evt_offscreen_delete, STRING("offscreen"))
    CALL(evt_mapobj_clear_offscreen, 0, STRING("S_iwa_04"))
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_iwa_04"), MAP_OBJ_FLAG_HIDDEN)
#else
    SET_VALUE(GSW_STORY_PROGRESS, 359)
#endif
    CALL(evt_hitobj_onoff, STRING("A_iwa_04"), 1, 0)
#ifndef VERSION_G8ME01
    CALL(evt_mario_key_onoff, 1)
#endif
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

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
    SET_VALUE(GSWF(4029), 1)
#else
    CALL(evt_mario_key_onoff, 0)
#endif
    SET_VALUE(LW(10), STRING("S_iwa_01"))
    SET_VALUE(LW(11), STRING("A_iwa_01"))
    CALL_CHILD(PTR(rock_main))
#ifndef VERSION_G8ME01
    SET_VALUE(GSWF(4029), 1)
    CALL(evt_mario_key_onoff, 1)
#endif
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(rock_evt1)
#ifdef VERSION_G8ME01
    SET_VALUE(GSWF(4030), 1)
#else
    CALL(evt_mario_key_onoff, 0)
#endif
    SET_VALUE(LW(10), STRING("S_iwa_07"))
    SET_VALUE(LW(11), STRING("A_iwa_07"))
    CALL_CHILD(PTR(rock_main))
#ifndef VERSION_G8ME01
    SET_VALUE(GSWF(4030), 1)
    CALL(evt_mario_key_onoff, 1)
#endif
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(moo_02_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_MON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_MOO2"))
    CALL(evt_map_playanim, STRING("hosi_01"), 1, 0)
    CALL(evt_map_playanim, STRING("hosi_02"), 1, 0)
    CALL(evt_map_playanim, STRING("hosi_03"), 1, 0)
    CALL(evt_map_playanim, STRING("light"), 1, 0)
    CALL(evt_item_entry, STRING("item00"), ITEM_STAR_PIECE, -55, 0, -135, ITEM_TYPE_STILL, GSWF(5664), 0)
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    IF_INT_LESS(GSW_STORY_PROGRESS, 359)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_iwa_04"), 0, PTR(dokan_evt), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_iwa_04"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_iwa_04"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(4029), 0)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_iwa_01"), 0, PTR(rock_evt0), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_iwa_01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_iwa_01"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSWF(4030), 0)
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_iwa_07"), 0, PTR(rock_evt1), 0)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_iwa_07"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_iwa_07"), 1, 0)
    END_IF
    CALL(evt_item_entry, STRING("item_01"), ITEM_POWER_PUNCH, 330, 0, 403, ITEM_TYPE_STILL, GSWF(4037), 0)
    RETURN
EVT_END()


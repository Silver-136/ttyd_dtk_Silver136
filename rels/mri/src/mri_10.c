#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "mri.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "mri_puni.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_lecture.h"
#include "evt/evt_item.h"


static BeroEntry bero_entry_data[2];
static EvtWord item_evt[68];
EvtWord mri_10_init_evt[100];

static BeroEntry bero_entry_data[] = {
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_09", // nextMapName
        "n_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(item_evt)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_envoff, 1024)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_puni_puninum_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(lect_juyoitem_rel_set, PTR(all_juyoitem_lecture_rel))
    CALL(evt_item_entry, STRING("item00"), 7, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_important_item, STRING("item00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_item_get_item_end_wait, STRING("item00"))
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 105)
    CALL(evt_puni_puninum_onoff, 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(mri_10_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 10, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 110, 0, 60, 2, -2, -1000, -1000)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(109, 110)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG2_MRI1"))
    END_SWITCH
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_mobj_itembox, STRING("mobj_box00"), 0, 0, 25, 1, 0, PTR(item_evt), GSWF(2854))
    RETURN
EVT_END()


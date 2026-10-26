#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "system.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[3];
EvtWord evt_tik09_dokan[51];
EvtWord tik_09_init_evt[36];

static BeroEntry bero_entry_data[] = {
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
        NULL, // exitEvent
        "tik_10", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        8, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, // exitEvent
        "tik_06", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_tik09_dokan)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
    CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, 0, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    WAIT_MS(1500)
    CALL(evt_sub_intpl_init, EASING_COS_IN, 0, 30, 100)
    LOOP(100)
        CALL(evt_sub_intpl_get_value)
        CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, LW(0), 0)
        CALL(evt_hit_bind_update, STRING("a_dokan_1"))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_trans, 0, STRING("dokan_1_s"), 0, 30, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    RETURN
EVT_END()

EVT_BEGIN(tik_09_init_evt)
    CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
    CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, 30, 0)
    CALL(evt_hit_bind_update, STRING("a_dokan_1"))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SPAWN_EVENT(PTR(evt_tik09_dokan))
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "data_tables.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"

USER_FUNC(evt_hit_get_position);
USER_FUNC(evt_item_entry);
USER_FUNC(evt_map_entry_airport_harbor);
USER_FUNC(evt_mapobj_scale);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_mobj_brick);
USER_FUNC(evt_mobj_powerupblk);
USER_FUNC(evt_mobj_switch_blue);

static BeroEntry bero_entry_data[5];
EvtWord evt_hikou_nobiru_init[89];
EvtWord evt_hikou_nobiru[203];
static TreeAccess tree_access_data[3];
EvtWord win_02_init_evt[87];

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
        "win_01", // nextMapName
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
        "win_03", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan2", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_hikou_nobiru_init)
    CALL(evt_hit_bind_mapobj, STRING("a_dai"), STRING("hikou"))
    CALL(evt_hit_bind_mapobj, STRING("a_hi"), STRING("hikou"))
    CALL(evt_hit_bind_mapobj, STRING("A_dokan1"), STRING("S_dokan2"))
    CALL(evt_hit_bind_mapobj, STRING("A_hi_kabe"), STRING("S_nob_tree"))
    CALL(evt_hit_bind_mapobj, STRING("A_dokan_nobi"), STRING("S_dokan2"))
    SET_VALUE(LW(0), 175)
    CALL(evt_mapobj_trans, 1, STRING("S_dokan_ha"), 0, LW(0), 0)
    SET_FLOAT_VALUE(LW(2), LW(0))
    ADD_FLOAT(LW(2), 90)
    DIVIDE_FLOAT(LW(2), 90)
    CALL(evt_mapobj_scale, 1, STRING("S_dokan2"), 1, LW(2), 1)
    SET_VALUE(LW(0), 175)
    CALL(evt_mapobj_trans, 1, STRING("hikou"), 0, LW(0), 0)
    SET_FLOAT_VALUE(LW(2), LW(0))
    ADD_FLOAT(LW(2), 116)
    DIVIDE_FLOAT(LW(2), 116)
    CALL(evt_mapobj_scale, 1, STRING("S_nob_tree"), 1, LW(2), 1)
    CALL(evt_hit_bind_update, STRING("a_dai"))
    CALL(evt_hit_bind_update, STRING("a_hi"))
    CALL(evt_hit_bind_update, STRING("A_dokan1"))
    CALL(evt_hit_bind_update, STRING("A_dokan_nobi"))
    CALL(evt_hit_bind_update, STRING("A_hi_kabe"))
    RETURN
EVT_END()

EVT_BEGIN(evt_hikou_nobiru)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_tree_ha04"), 1)
    CALL(evt_cam3d_evt_set, -307, 220, 533, -307, 124, -7, 500, 11)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon, STRING("SFX_STG2_SWITCH_QUAKE1"), LW(10))
    CALL(evt_cam_shake, 4, FLOAT(0.0146484375f), FLOAT(0.0f), 1000)
    CALL(evt_snd_sfxoff, LW(10))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -307, 393, 533, -307, 297, -7, 3000, 0)
    CALL(evt_hit_bind_mapobj, STRING("a_dai"), STRING("hikou"))
    CALL(evt_hit_bind_mapobj, STRING("a_hi"), STRING("hikou"))
    CALL(evt_hit_bind_mapobj, STRING("A_dokan1"), STRING("S_dokan2"))
    CALL(evt_hit_bind_mapobj, STRING("A_hi_kabe"), STRING("S_nob_tree"))
    CALL(evt_hit_bind_mapobj, STRING("A_dokan_nobi"), STRING("S_dokan2"))
    CALL(evt_hit_get_position, STRING("A_dokan1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_STEP_EXTEND1"), LW(0), LW(1), LW(2), LW(9))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 0, 175, 3000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_trans, 1, STRING("S_dokan_ha"), 0, LW(0), 0)
            SET_FLOAT_VALUE(LW(2), LW(0))
            ADD_FLOAT(LW(2), 90)
            DIVIDE_FLOAT(LW(2), 90)
            CALL(evt_mapobj_scale, 1, STRING("S_dokan2"), 1, LW(2), 1)
            CALL(evt_hit_bind_update, STRING("A_dokan1"))
            CALL(evt_hit_bind_update, STRING("A_dokan_nobi"))
            CALL(evt_hit_bind_update, STRING("A_hi_kabe"))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 0, 0, 175, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("hikou"), 0, LW(0), 0)
        SET_FLOAT_VALUE(LW(2), LW(0))
        ADD_FLOAT(LW(2), 116)
        DIVIDE_FLOAT(LW(2), 116)
        CALL(evt_mapobj_scale, 1, STRING("S_nob_tree"), 1, LW(2), 1)
        CALL(evt_hit_bind_update, STRING("a_dai"))
        CALL(evt_hit_bind_update, STRING("a_hi"))
        CALL(evt_hit_bind_update, STRING("A_dokan1"))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(9))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_tree_ha04"), 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static TreeAccess tree_access_data[] = {
    {4, "A_tree_01", "S_tree_mki_01", "S_tree_ha01", 0, NULL},
    {4, "A_tree_02", "S_tree_mki_02", "S_tree_ha02", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(win_02_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG2_WIN1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG2_WIN1"))
    SET_VALUE(LW(0), PTR(tree_access_data))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    CALL(evt_map_entry_airport_harbor, 0, STRING("hikou"), STRING("a_hi"))
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -470, 300, 50, 0, GSWF(5541), 0)
    CALL(evt_mobj_brick, STRING("mobj_box01"), 67, 110, -130, 271, 12, 0, GSWF(2683))
    CALL(evt_mobj_badgeblk, STRING("mobj_badge_gat"), 600, 185, -75, 248, 0, GSWF(2677), 1)
    CALL(evt_item_entry, STRING("item_00"), 125, -472, 50, -134, 16, GSWF(5608), 0)
    IF_INT_EQUAL(GSWF(2678), 1)
        SPAWN_EVENT(PTR(evt_hikou_nobiru_init))
    END_IF
    CALL(evt_mobj_switch_blue, 0, STRING("mobj_hashi0"), -415, 50, -125, PTR(evt_hikou_nobiru), GSWF(2678))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hikou"), 2)
    RETURN
EVT_END()


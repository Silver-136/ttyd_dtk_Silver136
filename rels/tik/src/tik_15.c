#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "driver/hitdrv.h"


USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_hit_damage_return_set);
USER_FUNC(evt_map_playanim);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_mobj_lv_blk);
USER_FUNC(evt_npc_check_delete);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_set_rev_mode);

f32 fd_tik_15_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_15_neg_one_382;
f32 fd_tik_15___local_PI_383;
f32 fd_tik_15___local_3PIO2_385;
f32 fd_tik_15_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik_15___local_PIO2_384;
f32 fd_tik_15_one_381;
f32 fd_tik_15_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_15_neg_one_365;
f32 fd_tik_15___local_PI_366;
f32 fd_tik_15___local_3PIO2_368;
f32 fd_tik_15_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik_15___local_PIO2_367;
f32 fd_tik_15_one_364;
static BeroEntry bero_entry_data[7];
static HitDamageReturn hitDmgRetPoints[13];
EvtWord tik_15_init_evt[91];

f32 fd_tik_15_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_15_neg_one_382 = -1.0f;

f32 fd_tik_15___local_PI_383 = 3.1415927f;

f32 fd_tik_15___local_3PIO2_385 = 4.712389f;

f32 fd_tik_15_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_15___local_PIO2_384 = 1.5707964f;

f32 fd_tik_15_one_381 = 1.0f;

f32 fd_tik_15_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_15_neg_one_365 = -1.0f;

f32 fd_tik_15___local_PI_366 = 3.1415927f;

f32 fd_tik_15___local_3PIO2_368 = 4.712389f;

f32 fd_tik_15_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik_15___local_PIO2_367 = 1.5707964f;

f32 fd_tik_15_one_364 = 1.0f;

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
        "tik_04", // nextMapName
        "dokan", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        6, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        NULL, // exitEvent
        "tik_01", // nextMapName
        "dokan_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_16", // nextMapName
        "e_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_16", // nextMapName
        "e_bero_2", // nextBeroName
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
        "tik_20", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "n_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tik_21", // nextMapName
        "s_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_1", {0.0f, -1000.0f, 0.0f}},
    {"mod_2", {0.0f, -1000.0f, 0.0f}},
    {"mod_3", {0.0f, -1000.0f, 0.0f}},
    {"mod_4", {0.0f, -1000.0f, 0.0f}},
    {"mod_5", {0.0f, -1000.0f, 0.0f}},
    {"mod_6", {0.0f, -1000.0f, 0.0f}},
    {"mod_7", {0.0f, -1000.0f, 0.0f}},
    {"mod_8", {0.0f, -1000.0f, 0.0f}},
    {"mod_9", {0.0f, -1000.0f, 0.0f}},
    {"mod_10", {0.0f, -1000.0f, 0.0f}},
    {"mod_11", {0.0f, -1000.0f, 0.0f}},
    {"mod_12", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(tik_15_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG0_TIK7"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_map_playanim, STRING("anm_mizu"), 1, 0)
    IF_INT_GREATER_OR_EQUAL(GSW(0), 56)
        CALL(evt_npc_check_delete, STRING("クリボー"))
        CALL(evt_npc_check_delete, STRING("パタクリボー"))
        CALL(evt_npc_check_delete, STRING("トゲクリボー"))
    END_IF
    IF_INT_LESS(GSW(0), 261)
        CALL(evt_npc_check_delete, STRING("ハンマーブロス"))
        CALL(evt_npc_check_delete, STRING("カメック"))
    END_IF
    CALL(evt_mobj_lv_blk, STRING("block00"), -370, 0, -25, 0, GSWF(1343), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("block01"), -420, 0, -25, 0, GSWF(1344), STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_badgeblk, STRING("mobj_01"), 50, 0, -38, 148, 0, GSWF(1362), 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_02"), -140, 0, -38, 132, 0, GSWF(1363), 0)
    RETURN
EVT_END()


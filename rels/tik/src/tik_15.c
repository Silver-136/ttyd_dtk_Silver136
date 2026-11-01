// tik_15 (埋没した都市, Rogueport Sewers): the large junction with pipes to tik_01 and tik_04 and exits to tik_16, tik_20 and
// tik_21; Goombas, Paragoombas, Spiky Goombas, a Hammer Bro and a Magikoopa.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "driver/hitdrv.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_mobj.h"
#include "pmario_sound.h"


static BeroEntry bero_entry_data[7];
static HitDamageReturn hitDmgRetPoints[13];
#if VERSION <= VERSION_DEMO_US
extern EvtWord tik_15_init_evt[];
#else
EvtWord tik_15_init_evt[91];
#endif

/**
 * Doors of this room: the pipes to tik_04 and tik_01, two west exits to tik_16, the east exit to tik_20
 * and the north exit to tik_21.
 */
static BeroEntry bero_entry_data[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_W, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Spots Mario is put back on after falling into one of this room's hazards (each object's own position). */
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

/**
 * Map init script of tik_15, the large junction room.
 * Sets the fall-recovery points (hit damage return), the wall- climbing gazigazi hooks and the water animation.
 * Removes the resident Goombas, Paragoombas and Spiky Goombas once the father
 * and son are reunited (STG1_17), and the Hammer Bro and Magikoopa before STG5_43_X_NAUTS_RETREAT.
 * Places two large blocks and two badge blocks (Mushroom, Fire Flower).
 */
EVT_BEGIN(tik_15_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_TIK1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_TIK7"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_bero_info_run)
#else
    CALL_CHILD(PTR(evt_bero_info_run))
#endif
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(evt_gazigazi_entry)
#else
    CALL_CHILD(PTR(evt_gazigazi_entry))
#endif
    CALL(evt_map_playanim, STRING("anm_mizu"), MAP_ANIM_PLAY_LOOP, 0)
#if VERSION <= VERSION_DEMO_US
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
#else
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
#endif
        CALL(evt_npc_check_delete, STRING("クリボー"))
        CALL(evt_npc_check_delete, STRING("パタクリボー"))
        CALL(evt_npc_check_delete, STRING("トゲクリボー"))
    END_IF
#if VERSION <= VERSION_DEMO_US
    IF_INT_LESS(GSW_STORY_PROGRESS, STG5_43_X_NAUTS_RETREAT)
#else
    IF_INT_LESS(GSW_STORY_PROGRESS, STG5_43_X_NAUTS_RETREAT)
#endif
        CALL(evt_npc_check_delete, STRING("ハンマーブロス"))
        CALL(evt_npc_check_delete, STRING("カメック"))
    END_IF
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mobj_lv_blk, STRING("block00"), -370, 0, -25, 0, GSWF_TIK_15_LV1_BIG_BLOCK_1_BROKEN, STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("block01"), -420, 0, -25, 0, GSWF_TIK_15_LV1_BIG_BLOCK_2_BROKEN, STRING("MOBJ_Lv1BigBlock"))
CALL(evt_mobj_badgeblk, STRING("mobj_01"), 50, 0, -30, ITEM_MUSHROOM, 0, GSWF_TIK_15_MUSHROOM_BLOCK, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_02"), -140, 0, -30, ITEM_FIRE_FLOWER, 0, GSWF_TIK_15_FIRE_FLOWER_BLOCK, 0)
#else
    CALL(evt_mobj_lv_blk, STRING("block00"), -370, 0, -25, 0, GSWF_TIK_15_LV1_BIG_BLOCK_1_BROKEN, STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_lv_blk, STRING("block01"), -420, 0, -25, 0, GSWF_TIK_15_LV1_BIG_BLOCK_2_BROKEN, STRING("MOBJ_Lv1BigBlock"))
    CALL(evt_mobj_badgeblk, STRING("mobj_01"), 50, 0, -38, ITEM_MUSHROOM, 0, GSWF_TIK_15_MUSHROOM_BLOCK, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_02"), -140, 0, -38, ITEM_FIRE_FLOWER, 0, GSWF_TIK_15_FIRE_FLOWER_BLOCK, 0)
#endif
    RETURN
EVT_END()


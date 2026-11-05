// dou_11 (洞窟, Pirate's Grotto): large room with stranded sailors (suifu), Monie and Peeton (US text: Francesca and Frankie), floating islands, bobbing platforms and a bomb wall (story STG5_29_WAVE_DODGING to STG5_36_HOLE_IN_THE_WALL).
// Doors to dou_10 and to Keelhaul Key (muj_10, muj_00).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_mobj.h"
#include "driver/npcdrv.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"

#include "driver/hitdrv.h"
#include "string.h"
#include "mario/mario.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_nannpc.h"
#include "dou.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "driver/seqdrv.h"

static BOOL _hitIsGroupSub(HitEntry* target, HitEntry* node, BOOL checkSibling);
BOOL hitIsGroup(HitEntry* target, HitEntry* group);

USER_FUNC(dou_mario_kaiten_hosei);
static BOOL hitIsGroup(HitEntry* target, HitEntry* group);

static ExtTexInfo ext_entry_data[31];
static s16 suifu_animtbl[42];
static s16 suifu_run_animtbl[22];
static EvtWord suifu_c_init[22];
static EvtWord suifu_c_talk[22];
static EvtWord suifu_d_init[22];
static EvtWord suifu_d_talk[22];
static EvtWord suifu_e_init[22];
static EvtWord suifu_e_talk[22];
static EvtWord mony_init[14];
static EvtWord mony_talk[42];
static EvtWord peton_init[14];
static EvtWord peton_talk[58];
static NpcSetupInfo npcEnt[6];
static BeroEntry bero_entry_data[12];
s32 suifudata[400];
s32 suifudata_after[400];
EvtWord watarikiri_suifu_init[128];
EvtWord ukishima_watari_run[458];
EvtWord ukishima_watari[186];
EvtWord ukishima_check[40];
static EvtWord ukishima_apper[286];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord ukishima_init1[];
extern EvtWord ukishima_part1[];
extern EvtWord ukishima_init2[];
extern EvtWord ukishima_part2[];
#else
EvtWord ukishima_init1[34];
EvtWord ukishima_part1[50];
EvtWord ukishima_init2[34];
EvtWord ukishima_part2[50];
EvtWord ukishima_switch1_evt[23];
EvtWord ukishima_switch2_evt[23];
#endif

EvtWord torinoko_suifu_pukapuka[69];
EvtWord _suifu_sakebu[101];
EvtWord torinoko_suifu_sakebu[103];
EvtWord torinoko_suifu_init[192];
EvtWord torinoko_suifu[172];
EvtWord suifu_orei[339];
EvtWord escapeway_event[98];

#if VERSION >= VERSION_JP
EvtWord bombwall_escape_move[58];
static EvtWord bombwall_bomb[187];
#else
static EvtWord bombwall_bomb[181];
#endif

static EvtWord bombwall_init[128];
EvtWord bombwall_suifuout_secheck[77];
EvtWord bombwall_suifuout[212];
EvtWord bombwall_after[410];
static EvtWord dai3_shurai_after[46];
EvtWord haikei_funeyure[100];
static EvtWord evt_ride_taru[15];
static EvtWord dou_puka_event_main0[60];
EvtWord dou_puka_event_main10[60];
EvtWord dou_puka_event_main11[60];
EvtWord dou_puka_event_main12[60];
EvtWord dou_puka_event_main13[60];
EvtWord dou_puka_event_main14[60];
EvtWord dou_puka_event_main15[60];
EvtWord dou_puka_event_main16[60];
EvtWord dou_puka_event_main17[60];
static EvtWord dou_puka_event_init1[8];
static EvtWord dou_puka_event_1[24];
static EvtWord dou_puka_event_rev_1[10];
static EvtWord dou_puka_event_main1[21];
static EvtWord dou_puka_event_sub1[45];
static EvtWord dou_puka_event_init2[8];
static EvtWord dou_puka_event_2[24];
static EvtWord dou_puka_event_rev_2[10];
static EvtWord dou_puka_event_main2[21];
static EvtWord dou_puka_event_sub2[45];
static EvtWord dou_puka_event_init3[8];
static EvtWord dou_puka_event_3[24];
static EvtWord dou_puka_event_rev_3[10];
static EvtWord dou_puka_event_main3[21];
static EvtWord dou_puka_event_sub3[45];
EvtWord dou_puka_event_init4[8];
EvtWord dou_puka_event_4[24];
EvtWord dou_puka_event_rev_4[10];
EvtWord dou_puka_event_main4[21];
EvtWord dou_puka_event_sub4[45];
EvtWord pukapuka_watari_init01[70];
EvtWord pukapuka_watari_init02[70];
HitDamageReturn hitDmgRetPoints_1st[17];
static HitDamageReturn hitDmgRetPoints[17];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord dou_11_init_evt[];
#else
EvtWord dou_11_init_evt[307];
#endif

/**
 * Recursive helper for hitIsGroup: returns TRUE if the hit entry target has the same joint name as node,
 * or as one of node's descendants (and, below the first level, its siblings).
 */
static BOOL _hitIsGroupSub(HitEntry* target, HitEntry* node, BOOL checkSibling) {
    if (strcmp(target->joint->name, node->joint->name) == 0) {
        return TRUE;
    }
    if (node->child != NULL) {
        return _hitIsGroupSub(target, node->child, TRUE);
    }
    if (checkSibling && node->sibling != NULL) {
        return _hitIsGroupSub(target, node->sibling, TRUE);
    }
    return FALSE;
}

/**
 * Returns whether the hit entry target is the hit group (or one of the hit entries below it), by joint name;
 * used so the swaying ship's rotation moves Mario while he stands on any of its hit parts.
 */
static BOOL hitIsGroup(HitEntry* target, HitEntry* group) {
    // Match joint names along the recursive child-first search; a child result returns without resuming this node's siblings.
    return _hitIsGroupSub(target, group, FALSE);
}

/**
 * Sprite entries for the crowd of sailors (evt_nannpc_init): the c_boro_k model with its S_1 (20 entries) and R_1 (10) animations, one
 * entry per animation phase offset.
 */
static ExtTexInfo ext_entry_data[] = {
    {"c_boro_k", "S_1", 0.0f},
    {"c_boro_k", "S_1", 2.0f},
    {"c_boro_k", "S_1", 4.0f},
    {"c_boro_k", "S_1", 6.0f},
    {"c_boro_k", "S_1", 8.0f},
    {"c_boro_k", "S_1", 10.0f},
    {"c_boro_k", "S_1", 12.0f},
    {"c_boro_k", "S_1", 14.0f},
    {"c_boro_k", "S_1", 16.0f},
    {"c_boro_k", "S_1", 18.0f},
    {"c_boro_k", "S_1", 20.0f},
    {"c_boro_k", "S_1", 22.0f},
    {"c_boro_k", "S_1", 24.0f},
    {"c_boro_k", "S_1", 26.0f},
    {"c_boro_k", "S_1", 28.0f},
    {"c_boro_k", "S_1", 30.0f},
    {"c_boro_k", "S_1", 32.0f},
    {"c_boro_k", "S_1", 34.0f},
    {"c_boro_k", "S_1", 36.0f},
    {"c_boro_k", "S_1", 38.0f},
    {"c_boro_k", "R_1", 0.0f},
    {"c_boro_k", "R_1", 2.0f},
    {"c_boro_k", "R_1", 4.0f},
    {"c_boro_k", "R_1", 6.0f},
    {"c_boro_k", "R_1", 8.0f},
    {"c_boro_k", "R_1", 10.0f},
    {"c_boro_k", "R_1", 12.0f},
    {"c_boro_k", "R_1", 14.0f},
    {"c_boro_k", "R_1", 16.0f},
    {"c_boro_k", "R_1", 18.0f},
    {0},
};

/**
 * Animation frame table for a sailor of the crowd while it stands or hops about: each of the frames 0 to 19
 * appears twice.
 */
static s16 suifu_animtbl[] = {
    0, 0, 1, 1, 2, 2, 3, 3,
    4, 4, 5, 5, 6, 6, 7, 7,
    8, 8, 9, 9, 10, 10, 11, 11,
    12, 12, 13, 13, 14, 14, 15, 15,
    16, 16, 17, 17, 18, 18, 19, 19,
    -1, 0,
};

/** Animation frame table for a sailor of the crowd while it runs, ending with -1. */
static s16 suifu_run_animtbl[] = {
    20, 20, 21, 21, 22, 22, 23, 23,
    24, 24, 25, 25, 26, 26, 27, 27,
    28, 28, 29, 29, -1, 0,
};

/**
 * Init of sailor C (水夫C): from STG5_30_SHIPWRECKED_SAILORS to STG5_35_ESCAPE_ROUTE he stands at
 * (825, 100, -125), facing east at STG5_34_KICKED_OFF.
 */
static EVT_BEGIN(suifu_c_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_35_ESCAPE_ROUTE)
        CALL(evt_npc_set_position, STRING("me"), 825, 100, -125)
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_34_KICKED_OFF)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_IF
    RETURN
EVT_END()

/**
 * Talk script of sailor C: stg5_dou_16_09 while the sailors are shipwrecked (STG5_30 to 34),
 * stg5_dou_17_09 at STG5_35_ESCAPE_ROUTE.
 */
static EVT_BEGIN(suifu_c_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_09"), 0, STRING("me"))
        CASE_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init of sailor D (水夫D): from STG5_30_SHIPWRECKED_SAILORS to STG5_35_ESCAPE_ROUTE he stands at
 * (775, 50, 50), facing east at STG5_34_KICKED_OFF.
 */
static EVT_BEGIN(suifu_d_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_35_ESCAPE_ROUTE)
        CALL(evt_npc_set_position, STRING("me"), 775, 50, 50)
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_34_KICKED_OFF)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_IF
    RETURN
EVT_END()

/**
 * Talk script of sailor D: stg5_dou_16_07 while the sailors are shipwrecked (STG5_30 to 34),
 * stg5_dou_17_07 at STG5_35_ESCAPE_ROUTE.
 */
static EVT_BEGIN(suifu_d_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_07"), 0, STRING("me"))
        CASE_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_07"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init of sailor E (水夫E): from STG5_30_SHIPWRECKED_SAILORS to STG5_35_ESCAPE_ROUTE he stands at
 * (736, 50,
 * 113) in the US and PAL releases and at (750, 50, -25) in the earlier ones, facing east at STG5_34_KICKED_OFF.
 */
static EVT_BEGIN(suifu_e_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_35_ESCAPE_ROUTE)
#if VERSION >= VERSION_US
#if VERSION >= VERSION_DEMO_EU
        CALL(evt_npc_set_position, STRING("水夫E"), 736, 50, 113)
#else
        CALL(evt_npc_set_position, STRING("me"), 736, 50, 113)
#endif
#else
        CALL(evt_npc_set_position, STRING("me"), 750, 50, -25)
#endif
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_34_KICKED_OFF)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_IF
    RETURN
EVT_END()

/**
 * Talk script of sailor E: stg5_dou_16_08 while the sailors are shipwrecked (STG5_30 to 34),
 * stg5_dou_17_08 at STG5_35_ESCAPE_ROUTE.
 */
static EVT_BEGIN(suifu_e_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_08"), 0, STRING("me"))
        CASE_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init of Monie (モニー): from STG5_30_SHIPWRECKED_SAILORS to STG5_35_ESCAPE_ROUTE she stands at (700,
 * 50, 0).
 */
static EVT_BEGIN(mony_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_35_ESCAPE_ROUTE)
        CALL(evt_npc_set_position, STRING("me"), 700, 50, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of Monie: she and Peeton (ピートン) speak in turn,
 * stg5_dou_16_10 and 16_11 while the sailors are shipwrecked and stg5_dou_17_10
 * and 17_11 at STG5_35_ESCAPE_ROUTE.
 */
static EVT_BEGIN(mony_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_10"), 0, STRING("モニー"))
        CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("モニー"))
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_11"), 0, STRING("ピートン"))
        CASE_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_10"), 0, STRING("モニー"))
        CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("モニー"))
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_11"), 0, STRING("ピートン"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init of Peeton (ピートン): from STG5_30_SHIPWRECKED_SAILORS to STG5_35_ESCAPE_ROUTE he stands at (750,
 * 50, 0).
 */
static EVT_BEGIN(peton_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_35_ESCAPE_ROUTE)
        CALL(evt_npc_set_position, STRING("me"), 750, 50, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of Peeton: turns Monie to face Mario and the two speak in turn,
 * stg5_dou_16_10 and 16_11 while the sailors are shipwrecked and stg5_dou_17_10
 * and 17_11 at STG5_35_ESCAPE_ROUTE.
 */
static EVT_BEGIN(peton_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_34_KICKED_OFF)
        CALL(evt_set_dir_to_target, STRING("モニー"), STRING("mario"))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_10"), 0, STRING("モニー"))
        CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("モニー"))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_11"), 0, STRING("ピートン"))
        CASE_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_set_dir_to_target, STRING("モニー"), STRING("mario"))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_10"), 0, STRING("モニー"))
        CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("モニー"))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_11"), 0, STRING("ピートン"))
    END_SWITCH
    RETURN
EVT_END()

/** NPC setup of dou_11: the named sailors C, D and E of the crowd, Monie and Peeton. */
static NpcSetupInfo npcEnt[] = {
    {
        "水夫C", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        suifu_c_init, // initEvent
        NULL, // regularEvent
        suifu_c_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "水夫D", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        suifu_d_init, // initEvent
        NULL, // regularEvent
        suifu_d_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "水夫E", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        suifu_e_init, // initEvent
        NULL, // regularEvent
        suifu_e_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "モニー", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        mony_init, // initEvent
        NULL, // regularEvent
        mony_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ピートン", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        peton_init, // initEvent
        NULL, // regularEvent
        peton_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Exits (beros) of this room: w_bero to dou_10, e_bero to muj_10, n_bero to muj_00, and pipes with no next map
 * that name each other: dokan_1/dokan_4, dokan_2/dokan_6, dokan_3/dokan_5 and dokan_7/dokan_8.
 */
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
        "dou_10", // nextMapName
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
        "muj_10", // nextMapName
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
        "muj_00", // nextMapName
        "n_bero_02", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
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
        NULL, 0, // exitEvent..nextMapName
        "dokan_4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_4", // hitName
        4098, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_6", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_6", // hitName
        4098, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_5", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_5", // hitName
        4098, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_7", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_8", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_8", // hitName
        4098, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, // exitEvent..nextMapName
        "dokan_7", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Starting positions of the 100 sailors on the left platform: x, y, z and facing direction for each. */
s32 suifudata[] = {
    155, 63, -3, 270, 162, 60, -50, 270,
    122, 86, -17, 90, 180, 96, 2, 90,
    130, 90, -9, 270, 191, 104, 3, 90,
    170, 38, -20, 90, 184, 63, -12, 270,
    192, 48, 2, 90, 137, 112, 14, 270,
    189, 73, -10, 90, 142, 66, -50, 270,
    166, 97, 19, 270, 139, 79, 19, 90,
    128, 41, -27, 90, 165, 85, -2, 270,
    152, 72, -10, 270, 169, 43, 22, 90,
    131, 88, -42, 90, 178, 82, -43, 270,
    178, 87, 16, 90, 187, 47, -35, 90,
    142, 69, -10, 270, 167, 102, 16, 90,
    166, 68, -22, 270, 141, 89, 16, 90,
    184, 80, -12, 270, 194, 71, -57, 270,
    135, 81, 14, 90, 130, 114, -43, 90,
    162, 62, 3, 270, 134, 70, -31, 270,
    185, 63, -16, 90, 188, 50, -49, 90,
    166, 60, 19, 270, 140, 37, 14, 90,
    179, 111, -20, 90, 140, 96, -33, 270,
    179, 115, -50, 90, 135, 87, -46, 270,
    170, 46, -46, 90, 153, 74, -23, 270,
    148, 111, -46, 90, 137, 107, -47, 90,
    158, 78, 3, 270, 179, 58, -56, 90,
    180, 79, -46, 270, 137, 71, -33, 270,
    145, 99, -40, 90, 180, 52, 18, 270,
    146, 76, 11, 90, 161, 53, -59, 270,
    140, 116, -40, 270, 139, 96, -38, 90,
    117, 58, -7, 90, 152, 96, -45, 270,
    128, 46, -10, 270, 151, 74, -7, 270,
    175, 111, 6, 90, 132, 107, -25, 270,
    142, 55, -19, 90, 165, 107, 13, 270,
    193, 53, 6, 270, 158, 102, 17, 90,
    183, 39, -36, 90, 154, 39, -36, 270,
    131, 113, -12, 270, 163, 64, -28, 270,
    161, 55, 16, 90, 127, 51, -16, 270,
    161, 81, -5, 90, 125, 76, -37, 270,
    142, 70, -27, 90, 128, 77, 3, 90,
    133, 72, 5, 270, 149, 69, 1, 90,
    125, 111, -6, 270, 162, 107, -44, 270,
    168, 105, 4, 90, 150, 64, -49, 270,
    142, 44, 8, 90, 120, 47, -32, 270,
    182, 116, -13, 270, 178, 84, -29, 90,
    135, 88, -15, 90, 147, 64, -47, 270,
    117, 115, -38, 270, 166, 90, -43, 270,
    121, 87, -38, 90, 157, 89, 5, 270,
    132, 83, -60, 90, 140, 85, 8, 270,
    128, 54, -40, 270, 164, 78, -33, 90,
    132, 99, -14, 90, 170, 49, -39, 90,
    135, 49, -39, 90, 126, 24, 25, 270,
    140, 24, 20, 270, 190, 24, 25, 270,
};

/**
 * Resting positions of the sailors on the right-hand platform after they have crossed: x, y, z and facing
 * direction for each.
 */
s32 suifudata_after[] = {
    637, 70, -109, 270, 694, 70, -139, 270,
    688, 70, -130, 270, 656, 70, -138, 90,
    647, 70, -129, 270, 637, 70, -123, 270,
    674, 70, -137, 90, 655, 70, -121, 90,
    671, 70, -119, 270, 690, 70, -110, 90,
    702, 70, -108, 90, 688, 70, -99, 90,
    681, 70, -94, 270, 635, 70, -99, 90,
    655, 70, -108, 270, 659, 70, -96, 90,
    687, 50, -50, 270, 647, 50, -35, 270,
    623, 50, -39, 270, 625, 50, -56, 90,
    630, 50, -68, 90, 638, 50, -83, 270,
    658, 50, -80, 270, 654, 50, -62, 90,
    659, 50, -40, 90, 672, 50, -46, 270,
    677, 50, -57, 90, 674, 50, -71, 90,
    680, 50, -82, 270, 693, 50, -65, 270,
    667, 50, -63, 90, 647, 50, -70, 270,
    639, 50, -66, 90, 646, 50, -45, 270,
    695, 50, -84, 90, 685, 50, -31, 90,
    699, 50, -54, 270, 826, 50, -80, 270,
    807, 50, -83, 270, 812, 50, -30, 90,
    835, 50, -30, 90, 811, 50, -50, 270,
    845, 50, -47, 90, 821, 50, -46, 90,
    821, 50, -46, 270, 822, 50, -72, 270,
    835, 50, -77, 270, 803, 50, -67, 90,
    805, 50, -36, 270, 817, 50, -56, 270,
    834, 50, -66, 270, 829, 50, -89, 90,
    818, 50, -88, 270, 813, 50, -77, 90,
    702, 50, -67, 270, 702, 50, -67, 270,
    726, 50, -63, 270, 747, 50, -67, 90,
    769, 50, -62, 270, 780, 50, -54, 90,
    781, 50, -79, 270, 760, 50, -66, 90,
    732, 50, -69, 90, 796, 50, -78, 270,
    772, 50, -77, 90, 757, 50, -75, 270,
    740, 50, -64, 270, 723, 50, -40, 90,
    705, 50, -43, 90, 796, 50, -45, 90,
    786, 50, -27, 270, 762, 50, -31, 90,
    754, 50, -57, 270, 749, 50, -38, 90,
    726, 50, -82, 270, 776, 50, -63, 270,
    791, 50, -57, 90, 738, 50, -83, 90,
    775, 50, -86, 270, 749, 50, -85, 90,
    738, 50, -33, 90, 720, 50, -57, 270,
    712, 50, -39, 90, 712, 50, -75, 90,
    712, 50, -62, 270, 707, 50, -30, 90,
    726, 50, -29, 270, 733, 50, -43, 90,
    747, 50, -76, 90, 766, 50, -86, 270,
    785, 50, -87, 90, 788, 50, -70, 270,
    784, 50, -36, 270, 763, 50, -40, 90,
    774, 50, -30, 270, 681, 50, -75, 90,
    714, 50, -78, 90, 0, -1000, 0, 0,
    0, -1000, 0, 0, 0, -1000, 0, 0,
};

/**
 * Creates the crowd of 95 sailors (nannpc extension sprites c_boro_k) at their resting places on the right-hand
 * platform (suifudata_after) when they have already crossed or the story is past that point,
 * faces them and gives them a hit size; also adds one invisible blocker (id 100) that follows Mario.
 */
EVT_BEGIN(watarikiri_suifu_init)
    CALL(evt_nannpc_init, PTR(ext_entry_data), 0, 0, 0, 101)
    CALL(evt_nannpc_set_subfunc, 1, PTR(nannpc_ext_main_sub_fast))
    CALL(evt_nannpc_set_subfunc, 2, PTR(nanNPCOption))
    SET_VALUE(LW(0), 0)
    LOOP(95)
        CALL(evt_nannpc_entry, LW(0))
        CALL(evt_nannpc_set_func, LW(0), 0)
        CALL(evt_nannpc_set_animtbl, LW(0), PTR(suifu_animtbl))
        SET_INT_READ_ADDR(PTR(suifudata_after))
        SET_VALUE(LW(10), LW(0))
        MULTIPLY(LW(10), 4)
        READ_INT_INDEXED(LW(1), LW(10))
        ADD(LW(10), 1)
        READ_INT_INDEXED(LW(2), LW(10))
        ADD(LW(10), 1)
        READ_INT_INDEXED(LW(3), LW(10))
        ADD(LW(10), 1)
        READ_INT_INDEXED(LW(4), LW(10))
        CALL(evt_nannpc_set_position, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_nannpc_flag_onoff, LW(0), 0, 4)
        CALL(evt_nannpc_set_size, LW(0), 30, 8)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_34_KICKED_OFF)
            SET_VALUE(LW(4), 90)
        END_IF
        CALL(evt_nannpc_set_dir, LW(0), LW(4))
        ADD(LW(0), 1)
    END_LOOP
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 100)
        CALL(evt_nannpc_entry, LW(0))
        CALL(evt_nannpc_flag_onoff, LW(0), 0, 6)
        CALL(evt_nannpc_set_size, LW(0), 30, 15)
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
            CALL(evt_nannpc_set_position, LW(0), LW(1), LW(2), LW(3))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Script for one sailor (nannpc id in LW(0)) of the crowd crossing the floating islands:
 * hops across the barrels in six random-sized jumps, pausing between them,
 * and finally lands on the right-hand platform; the last three sailors (97 to
 * 99) are swapped for the named NPCs Sailor C, D and E, the others go to their resting place from suifudata_after.
 */
EVT_BEGIN(ukishima_watari_run)
    // A sailor of the crowd faces right and sets off across the floating barrels.
    CALL(evt_nannpc_set_work, LW(0), 0, 0)
    CALL(evt_nannpc_set_dir, LW(0), 90)
    CALL(evt_sub_random, 20, LW(10))
    ADD(LW(10), 160)
    CALL(evt_sub_random, 10, LW(12))
    ADD(LW(12), -10)
    CALL(evt_sub_random, 30, LW(13))
    ADD(LW(13), 30)
    CALL(evt_sub_random, 300, LW(14))
    ADD(LW(14), 300)

    // First hop: a short random-sized jump onto the first barrel, then a short random pause.
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), LW(10), 37, LW(12), LW(14), LW(13))
    CALL(evt_sub_random, 30, LW(10))
    ADD(LW(10), 50)
    WAIT_MS(LW(10))
    CALL(evt_sub_random, 15, LW(10))
    ADD(LW(10), 250)
    CALL(evt_sub_random, 20, LW(12))
    ADD(LW(12), -10)
    CALL(evt_sub_random, 30, LW(13))
    ADD(LW(13), 30)
    CALL(evt_sub_random, 300, LW(14))
    ADD(LW(14), 300)

    // Second hop onto the next barrel.
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), LW(10), 25, LW(12), LW(14), LW(13))
    CALL(evt_sub_random, 30, LW(10))
    ADD(LW(10), 50)
    WAIT_MS(LW(10))
    CALL(evt_sub_random, 15, LW(10))
    ADD(LW(10), 300)
    CALL(evt_sub_random, 20, LW(12))
    ADD(LW(12), -10)
    CALL(evt_sub_random, 30, LW(13))
    ADD(LW(13), 30)
    CALL(evt_sub_random, 300, LW(14))
    ADD(LW(14), 300)
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), LW(10), 25, LW(12), LW(14), LW(13))
    CALL(evt_sub_random, 30, LW(10))
    ADD(LW(10), 50)
    WAIT_MS(LW(10))
    CALL(evt_sub_random, 15, LW(10))
    ADD(LW(10), 395)
    CALL(evt_sub_random, 20, LW(12))
    ADD(LW(12), -10)
    CALL(evt_sub_random, 30, LW(13))
    ADD(LW(13), 30)
    CALL(evt_sub_random, 300, LW(14))
    ADD(LW(14), 300)

    // Fourth and fifth hops, each a little farther with random size and pause.
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), LW(10), 25, LW(12), LW(14), LW(13))
    CALL(evt_sub_random, 30, LW(10))
    ADD(LW(10), 50)
    WAIT_MS(LW(10))
    CALL(evt_sub_random, 20, LW(10))
    ADD(LW(10), 490)
    CALL(evt_sub_random, 20, LW(12))
    ADD(LW(12), -10)
    CALL(evt_sub_random, 30, LW(13))
    ADD(LW(13), 30)
    CALL(evt_sub_random, 300, LW(14))
    ADD(LW(14), 300)
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), LW(10), 25, LW(12), LW(14), LW(13))
    CALL(evt_sub_random, 30, LW(10))
    ADD(LW(10), 50)
    WAIT_MS(LW(10))
    CALL(evt_sub_random, 15, LW(10))
    ADD(LW(10), 550)
    CALL(evt_sub_random, 20, LW(12))
    ADD(LW(12), -10)
    CALL(evt_sub_random, 30, LW(13))
    ADD(LW(13), 30)
    CALL(evt_sub_random, 300, LW(14))
    ADD(LW(14), 300)

    // Last random hop onto the final barrel.
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), LW(10), 25, LW(12), LW(14), LW(13))
    CALL(evt_sub_random, 30, LW(10))
    ADD(LW(10), 50)
    WAIT_MS(LW(10))

    // The final long jump lands on the right-hand platform.
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), 625, 50, 0, 250, 50)

    // The last three sailors turn into the named Sailors C, D and E on the platform; the others go to their
    // resting spots.
    SWITCH(LW(0))
        CASE_EQUAL(97)
        CALL(evt_nannpc_set_position, 97, 0, -1000, 0)
        CALL(evt_nannpc_ppflag_onoff, 97, 0, 4)
        CALL(evt_npc_set_position, STRING("水夫C"), 825, 100, -125)
        CALL(evt_npc_set_ry, STRING("水夫C"), 270)
        CALL(evt_npc_flag_onoff, 0, STRING("水夫C"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CASE_EQUAL(98)
        CALL(evt_nannpc_set_position, 98, 0, -1000, 0)
        CALL(evt_nannpc_ppflag_onoff, 98, 0, 4)
        CALL(evt_npc_set_position, STRING("水夫D"), 775, 50, 50)
        CALL(evt_npc_set_ry, STRING("水夫D"), 270)
        CALL(evt_npc_flag_onoff, 0, STRING("水夫D"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CASE_EQUAL(99)
        CALL(evt_nannpc_set_position, 99, 0, -1000, 0)
        CALL(evt_nannpc_ppflag_onoff, 99, 0, 4)
#if VERSION >= VERSION_US
        CALL(evt_npc_set_position, STRING("水夫E"), 736, 50, 113)
#else
        CALL(evt_npc_set_position, STRING("水夫E"), 750, 50, -25)
#endif
        CALL(evt_npc_set_ry, STRING("水夫E"), 270)
        CALL(evt_npc_flag_onoff, 0, STRING("水夫E"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CASE_DEFAULT
        SET_INT_READ_ADDR(PTR(suifudata_after))
        SET_VALUE(LW(10), LW(0))
        MULTIPLY(LW(10), 4)
        READ_INT_INDEXED(LW(1), LW(10))
        ADD(LW(10), 1)
        READ_INT_INDEXED(LW(2), LW(10))
        ADD(LW(10), 1)
        READ_INT_INDEXED(LW(3), LW(10))
        ADD(LW(10), 1)
        READ_INT_INDEXED(LW(4), LW(10))
        CALL(evt_nannpc_set_position, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_nannpc_set_dir, LW(0), LW(4))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Cutscene when both island switches have been pressed:
 * the camera pans over the islands, the crowd speaks (stg5_dou_14 and 15),
 * then the 100 crowd members cross one after another (released from the highest position down: the Y threshold
 * starts at 110 and drops by 10 per pass) until all have gone.
 * Sets GSWF_DOU_11_SAILORS_CROSSED, removes the invisible ship hit area (fune_atari),
 * gives control back and starts suifu_orei.
 */
EVT_BEGIN(ukishima_watari)
    CALL(evt_cam3d_evt_set, 160, 174, 527, 160, 79, -14, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_nannpc_get_position, 99, LW(1), LW(2), LW(3))
    CALL(evt_nannpc_set_dir, 99, 90)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(1), LW(2), LW(3))
    CALL(evt_msg_print, 0, STRING("stg5_dou_14"), 0, 0)
    SET_VALUE(LW(0), 0)
    LOOP(100)
        CALL(evt_nannpc_set_dir, LW(0), 90)
        ADD(LW(0), 1)
    END_LOOP
    CALL(evt_nannpc_get_position, 98, LW(1), LW(2), LW(3))
    CALL(evt_nannpc_set_dir, 98, 90)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(1), LW(2), LW(3))
    CALL(evt_msg_print, 0, STRING("stg5_dou_15"), 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KINOPIO_MOVE1_2"), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 110)
    LOOP(0)
        SET_VALUE(LW(0), 0)
        LOOP(100)
            CALL(evt_nannpc_get_work, LW(0), 1, LW(4))
            CALL(evt_nannpc_get_position, LW(0), LW(10), LW(11), LW(12))
            IF_INT_GREATER_OR_EQUAL(LW(11), LW(6))
                IF_INT_EQUAL(LW(4), 0)
                    CALL(evt_nannpc_set_work, LW(0), 1, 1)
                    CALL(evt_sub_random, 50, LW(10))
                    ADD(LW(10), 100)
                    WAIT_MS(LW(10))
                    SPAWN_EVENT(PTR(ukishima_watari_run))
                    ADD(LW(5), 1)
                END_IF
            END_IF
            IF_INT_EQUAL(LW(5), 100)
                GOTO(100)
            END_IF
            ADD(LW(0), 1)
        END_LOOP
        ADD(LW(6), -10)
    END_LOOP
    LABEL(100)
    WAIT_MS(1500)
    CALL(evt_nannpc_set_subfunc, 2, PTR(nanNPCOption))
    SET_VALUE(LW(0), 0)
    LOOP(95)
        CALL(evt_nannpc_flag_onoff, LW(0), 0, 4)
        CALL(evt_nannpc_set_size, LW(0), 30, 8)
        ADD(LW(0), 1)
    END_LOOP
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSWF_DOU_11_SAILORS_CROSSED, 1)
    CALL(evt_hitobj_onoff, STRING("fune_atari"), 1, 0)
    CALL(evt_mario_key_onoff, 1)
    SPAWN_EVENT(PTR(suifu_orei))
    RETURN
EVT_END()

/**
 * Checks the two island switches (A and B) after one is pressed: if only one is down the camera goes back;
 * if both are down, plays the success jingle and starts the sailors' crossing (ukishima_watari).
 */
EVT_BEGIN(ukishima_check)
    SET_VALUE(LW(0), GSWF_DOU_11_SWITCH_1_PRESSED)
    ADD(LW(0), GSWF_DOU_11_SWITCH_2_PRESSED)
    IF_INT_NOT_EQUAL(LW(0), 2)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        WAIT_MS(1000)
    ELSE
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        SPAWN_EVENT(PTR(ukishima_watari))
    END_IF
    RETURN
EVT_END()

/**
 * Makes a floating island or barrel appear: shows two ripple effects on the water at its position,
 * plays SFX_STG5_BARREL_APPEAR1 after 0.5 seconds,
 * and unhides the map object in LW(2) and enables its hit area LW(3).
 */
static EVT_BEGIN(ukishima_apper)
    // A splash sound plays as the island or barrel comes up.
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mapobj_get_position, LW(2), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BARREL_APPEAR1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT

    // Two ripples spread on the water above it.
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(2), LW(10), LW(11), LW(12))
        CALL(evt_eff64, STRING(""), STRING("ripple_n64"), 0, LW(10), 5, LW(12), 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(400)
        CALL(evt_eff64, STRING(""), STRING("ripple_n64"), 0, LW(10), 5, LW(12), 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT

    // The island becomes visible and solid, and rises out of the water.
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(2), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, LW(3), 1, 1)
    CALL(evt_hit_bind_mapobj, LW(3), LW(2))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, -100, 0, 800)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, LW(2), 0, LW(0), 0)
        CALL(evt_hit_bind_update, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP

    // It dips below its rest height and comes back a few times until it settles.
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -10, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, LW(2), 0, LW(0), 0)
        CALL(evt_hit_bind_update, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -10, 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, LW(2), 0, LW(0), 0)
        CALL(evt_hit_bind_update, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -5, 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, LW(2), 0, LW(0), 0)
        CALL(evt_hit_bind_update, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -5, 0, 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, LW(2), 0, LW(0), 0)
        CALL(evt_hit_bind_update, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -1, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, LW(2), 0, LW(0), 0)
        CALL(evt_hit_bind_update, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1, 0, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_trans, 1, LW(2), 0, LW(0), 0)
        CALL(evt_hit_bind_update, LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * At map load: hides the first pair of barrels (S_taru_n01, n02) and their hit areas
 * until one island switch has been pressed (in the early demos, mobj01).
 */
EVT_BEGIN(ukishima_init1)
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF_DOU_11_SWITCH_1_PRESSED, 0)
#else
    SET_VALUE(LW(0), GSWF_DOU_11_SWITCH_1_PRESSED)
    ADD(LW(0), GSWF_DOU_11_SWITCH_2_PRESSED)
    IF_INT_LESS(LW(0), 1)
#endif
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_taru_n01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n01"), 1, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_taru_n02"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n02"), 1, 0)
    END_IF
    RETURN
EVT_END()

/**
 * The first of the two blue island switches pressed (in the early demos, mobj01): the camera looks at the first stretch,
 * the first pair of barrels pops up out of the water (ukishima_apper) and starts bobbing,
 * then ukishima_check runs.
 */
EVT_BEGIN(ukishima_part1)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 280, 174, 527, 280, 79, -14, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(GSWF_DOU_11_SWITCH_1_PRESSED, 1)
#endif
    SET_VALUE(LW(2), STRING("S_taru_n01"))
    SET_VALUE(LW(3), STRING("A_taru_n01"))
    SPAWN_EVENT(PTR(ukishima_apper))
    BEGIN_INLINE_EVENT
        WAIT_MS(700)
        SET_VALUE(LW(2), STRING("S_taru_n02"))
        SET_VALUE(LW(3), STRING("A_taru_n02"))
        SPAWN_EVENT(PTR(ukishima_apper))
    END_INLINE_EVENT
    WAIT_MS(2500)
    SPAWN_EVENT(PTR(pukapuka_watari_init01))
    WAIT_MS(1000)
    CALL_CHILD(PTR(ukishima_check))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * At map load: hides the second pair of barrels (S_taru_n03, n04) and their hit areas
 * until both island switches have been pressed (or in the early demos, the second switch).
 */
EVT_BEGIN(ukishima_init2)
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF_DOU_11_SWITCH_2_PRESSED, 0)
#else
    SET_VALUE(LW(0), GSWF_DOU_11_SWITCH_1_PRESSED)
    ADD(LW(0), GSWF_DOU_11_SWITCH_2_PRESSED)
    IF_INT_LESS(LW(0), 2)
#endif
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_taru_n03"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n03"), 1, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_taru_n04"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n04"), 1, 0)
    END_IF
    RETURN
EVT_END()

/**
 * The second of the two blue island switches pressed (in the early demos, mobj02): the camera looks at the second stretch,
 * the second pair of barrels pops up out of the water and starts bobbing, then ukishima_check runs.
 */
EVT_BEGIN(ukishima_part2)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 525, 174, 527, 525, 79, -14, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(GSWF_DOU_11_SWITCH_2_PRESSED, 1)
#endif
    SET_VALUE(LW(2), STRING("S_taru_n03"))
    SET_VALUE(LW(3), STRING("A_taru_n03"))
    SPAWN_EVENT(PTR(ukishima_apper))
    BEGIN_INLINE_EVENT
        WAIT_MS(700)
        SET_VALUE(LW(2), STRING("S_taru_n04"))
        SET_VALUE(LW(3), STRING("A_taru_n04"))
        CALL_CHILD(PTR(ukishima_apper))
    END_INLINE_EVENT
    WAIT_MS(2500)
    SPAWN_EVENT(PTR(pukapuka_watari_init02))
    WAIT_MS(1000)
    CALL_CHILD(PTR(ukishima_check))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

#if VERSION >= VERSION_JP
/**
 * Event of the first blue island switch (mobj01):
 * sets its flag and runs ukishima_part1 or ukishima_part2 depending on how many switches are now down.
 * (JP and later; the demos call ukishima_part1 directly.)
 */
EVT_BEGIN(ukishima_switch1_evt)
    SET_VALUE(GSWF_DOU_11_SWITCH_1_PRESSED, 1)
    SET_VALUE(LW(0), GSWF_DOU_11_SWITCH_1_PRESSED)
    ADD(LW(0), GSWF_DOU_11_SWITCH_2_PRESSED)
    IF_INT_EQUAL(LW(0), 1)
        SPAWN_EVENT(PTR(ukishima_part1))
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SPAWN_EVENT(PTR(ukishima_part2))
    END_IF
    RETURN
EVT_END()

/**
 * Event of the second blue island switch (mobj02):
 * sets its flag and runs ukishima_part1 or ukishima_part2 depending on how many switches are now down.
 * (JP and later.)
 */
EVT_BEGIN(ukishima_switch2_evt)
    SET_VALUE(GSWF_DOU_11_SWITCH_2_PRESSED, 1)
    SET_VALUE(LW(0), GSWF_DOU_11_SWITCH_1_PRESSED)
    ADD(LW(0), GSWF_DOU_11_SWITCH_2_PRESSED)
    IF_INT_EQUAL(LW(0), 1)
        SPAWN_EVENT(PTR(ukishima_part1))
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SPAWN_EVENT(PTR(ukishima_part2))
    END_IF
    RETURN
EVT_END()
#endif

/**
 * Keeps the stranded sailors bobbing with the barrels:
 * for every sailor with work flag 0 set, offsets it by GW(0) vertically
 * and puts its shadow at the matching height (higher for sailors on the Z <= 0 side).
 */
EVT_BEGIN(torinoko_suifu_pukapuka)
    LOOP(0)
        SET_VALUE(LW(0), 0)
        LOOP(100)
            CALL(evt_nannpc_get_work, LW(0), 0, LW(10))
            IF_INT_EQUAL(LW(10), 1)
                SET_FLOAT_VALUE(LW(10), GW(0))
                CALL(evt_nannpc_set_hosei_position, LW(0), 0, LW(10), 0)
                CALL(evt_nannpc_get_position, LW(0), LW(1), LW(2), LW(3))
                IF_INT_LESS_OR_EQUAL(LW(3), 0)
                    SET_FLOAT_VALUE(LW(1), FLOAT(39.0f))
                ELSE
                    SET_FLOAT_VALUE(LW(1), FLOAT(26.0f))
                END_IF
                ADD_FLOAT(LW(1), LW(10))
                CALL(evt_nannpc_set_shadow_position, LW(0), LW(1))
            ELSE
                CALL(evt_nannpc_get_position, LW(0), LW(1), LW(2), LW(3))
                CALL(evt_nannpc_set_shadow_position, LW(0), LW(2))
            END_IF
            ADD(LW(0), 1)
        END_LOOP
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * The stranded sailors keep shouting for help: forever shows six speech bubbles at fixed positions
 * (stg5_dou_13_01 to
 * 06) over the crowd, one after another.
 */
EVT_BEGIN(_suifu_sakebu)
    LOOP(0)
        WAIT_MS(700)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, 115, 130, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_01"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(600)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, 105, 80, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_03"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(600)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, 123, 30, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_05"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(500)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, 210, 135, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_02"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(800)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, 225, 85, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_06"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(400)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, 195, 37, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_04"), 0, 0)
        END_INLINE_EVENT
    END_LOOP
    RETURN
EVT_END()

/**
 * Runs while the sailors are still stranded: whenever Mario stands on the left platform (X 0 to 300, Z -140 to
 * 140, on the ground) it runs the shouting (_suifu_sakebu), and stops it when he leaves.
 * Ends once both island switches are pressed.
 */
EVT_BEGIN(torinoko_suifu_sakebu)
    LOOP(0)
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(0), 0)
                IF_INT_LESS_OR_EQUAL(LW(0), 300)
                    IF_INT_GREATER_OR_EQUAL(LW(1), -2)
                        IF_INT_LESS_OR_EQUAL(LW(1), 2)
                            IF_INT_GREATER_OR_EQUAL(LW(2), -140)
                                IF_INT_LESS_OR_EQUAL(LW(2), 140)
                                    BREAK_LOOP
                                END_IF
                            END_IF
                        END_IF
                    END_IF
                END_IF
            END_IF
            IF_INT_EQUAL(GSWF_DOU_11_SWITCH_1_PRESSED, 1)
                IF_INT_EQUAL(GSWF_DOU_11_SWITCH_2_PRESSED, 1)
                    RETURN
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(_suifu_sakebu), LW(10))
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            SET_VALUE(LF(0), 1)
            IF_INT_GREATER_OR_EQUAL(LW(0), 0)
                IF_INT_LESS_OR_EQUAL(LW(0), 300)
                    IF_INT_GREATER_OR_EQUAL(LW(1), -2)
                        IF_INT_LESS_OR_EQUAL(LW(1), 2)
                            IF_INT_GREATER_OR_EQUAL(LW(2), -140)
                                IF_INT_LESS_OR_EQUAL(LW(2), 140)
                                    SET_VALUE(LF(0), 0)
                                END_IF
                            END_IF
                        END_IF
                    END_IF
                END_IF
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(10))
        WAIT_MS(1000)
    END_LOOP
    RETURN
EVT_END()

/**
 * Set-up of the stranded sailors for STG5_29_WAVE_DODGING:
 * makes Sailors C, D, E and Monie and Peeton intangible,
 * spawns the crowd of 100 sailors from suifudata on the left platform with their animations,
 * adds a hidden crowd entry (100) that follows Mario as a blocker, and hides crowd members 95 and 96,
 * keeping Monie and Peeton at their places every frame so the two stand in the crowd.
 */
EVT_BEGIN(torinoko_suifu_init)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫C"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫D"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫E"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("モニー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_change_interrupt, STRING("モニー"), 6, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ピートン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_change_interrupt, STRING("ピートン"), 6, 0)
    CALL(evt_nannpc_init, PTR(ext_entry_data), 0, 0, 0, 101)
    CALL(evt_nannpc_set_subfunc, 1, PTR(nannpc_ext_main_sub_fast))
    SPAWN_EVENT(PTR(torinoko_suifu_pukapuka))
    SET_INT_READ_ADDR(PTR(suifudata))
    SET_VALUE(LW(0), 0)
    LOOP(100)
        READ_INT3(LW(1), LW(2), LW(3))
        READ_INT(LW(4))
        CALL(evt_nannpc_entry, LW(0))
        CALL(evt_nannpc_set_func, LW(0), 0)
        CALL(evt_nannpc_set_position, LW(0), LW(1), LW(2), LW(3))
        SWITCH(LW(0))
            CASE_EQUAL(96)
            CALL(evt_nannpc_flag_onoff, LW(0), 0, 2)
            CALL(evt_nannpc_ppflag_onoff, LW(0), 0, 4)
            CASE_EQUAL(95)
            CALL(evt_nannpc_flag_onoff, LW(0), 0, 2)
            CALL(evt_nannpc_ppflag_onoff, LW(0), 0, 4)
        END_SWITCH
        CALL(evt_nannpc_set_animtbl, LW(0), PTR(suifu_animtbl))
        CALL(evt_nannpc_set_dir, LW(0), LW(4))
        CALL(evt_nannpc_set_work, LW(0), 0, 1)
        CALL(evt_nannpc_set_work, LW(0), 1, 0)
        ADD(LW(0), 1)
    END_LOOP
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 100)
        CALL(evt_nannpc_entry, LW(0))
        CALL(evt_nannpc_flag_onoff, LW(0), 0, 6)
        CALL(evt_nannpc_set_size, LW(0), 30, 15)
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
            CALL(evt_nannpc_set_position, LW(0), LW(1), LW(2), LW(3))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(GW(10))
        LOOP(0)
            CALL(evt_nannpc_get_position, 96, LW(1), LW(2), LW(3))
            CALL(evt_npc_set_position, STRING("ピートン"), LW(1), LW(2), LW(3))
            CALL(evt_nannpc_get_position, 95, LW(1), LW(2), LW(3))
            CALL(evt_npc_set_position, STRING("モニー"), LW(1), LW(2), LW(3))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * First-sight cutscene of the stranded sailors:
 * hides Sailors C to E, Mario sees the crowd shouting (stg5_dou_11 and the small_00 to 05 lines),
 * the camera pans over them, and then the message stg5_dou_13. Sets GSWF_DOU_11_SAILORS_SHOUT_SEEN.
 */
EVT_BEGIN(torinoko_suifu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("水夫C"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("水夫D"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("水夫E"), 0, -1000, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, -400, 60, 30)
    CALL(evt_msg_print, 0, STRING("stg5_dou_11"), 0, 0)
    CALL(evt_cam3d_evt_set, 160, 180, 559, 160, 85, 17, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        WAIT_MS(1800)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 115, 130, 0)
        CALL(evt_msg_print, 0, STRING("small_00"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1200)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 105, 80, 0)
        CALL(evt_msg_print, 0, STRING("small_01"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(900)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 123, 30, 0)
        CALL(evt_msg_print, 0, STRING("small_02"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 210, 135, 0)
        CALL(evt_msg_print, 0, STRING("small_03"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 225, 85, 0)
        CALL(evt_msg_print, 0, STRING("small_04"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(2100)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 195, 37, 0)
        CALL(evt_msg_print, 0, STRING("small_05"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(4000)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, -400, 60, 30)
    CALL(evt_msg_print, 0, STRING("stg5_dou_13"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF_DOU_11_SAILORS_SHOUT_SEEN, 1)
    RETURN
EVT_END()

/**
 * After the sailors have crossed: Mario walks to the right platform, Sailors D and E talk to him (stg5_dou_16),
 * Monie recognises him and runs over, Peeton follows, she tells how they got here and asks why he came,
 * Mario reacts, and then both stand ready for conversation.
 * Sets STG5_30_SHIPWRECKED_SAILORS.
 */
EVT_BEGIN(suifu_orei)
    // Sailors C, D and E are placed on the right-hand platform facing west.
    CALL(evt_npc_set_position, STRING("水夫C"), 825, 100, -125)
    CALL(evt_npc_set_ry, STRING("水夫C"), 270)
    CALL(evt_npc_set_position, STRING("水夫D"), 775, 50, 50)
    CALL(evt_npc_set_ry, STRING("水夫D"), 270)
#if VERSION >= VERSION_US
    CALL(evt_npc_set_position, STRING("水夫E"), 736, 50, 113)
#else
    CALL(evt_npc_set_position, STRING("水夫E"), 750, 50, -25)
#endif
    CALL(evt_npc_set_ry, STRING("水夫E"), 270)

    // Wait until Mario reaches the right-hand platform.
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mario_get_name_hitobj_ride, LW(4))
        IF_STR_EQUAL(LW(4), STRING("mod_01"))
            IF_INT_GREATER_OR_EQUAL(LW(0), 630)
                BREAK_LOOP
            END_IF
            IF_INT_LESS_OR_EQUAL(LW(2), -20)
                IF_INT_GREATER_OR_EQUAL(LW(2), -140)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    DELETE_EVENT(GW(10))

    // Control is taken from Mario; he and his partner turn to face the sailors.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_npc_flag_onoff, 1, STRING("モニー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ピートン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_wait_landon, 0)
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_dir, 0, 90, 200)
    END_INLINE_EVENT

    // Sailor D thanks Mario and Sailor E tells how they came to be here.
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_set_dir_to_target, STRING("水夫D"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_16"), 0, STRING("水夫D"))
    CALL(evt_set_dir_to_target, STRING("水夫E"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_01"), 0, STRING("水夫E"))

    // Monie appears, startled, recognises Mario and runs across the platform.
    SET_INT_READ_ADDR(PTR(suifudata_after))
    SET_VALUE(LW(10), 95)
    MULTIPLY(LW(10), 4)
    READ_INT_INDEXED(LW(1), LW(10))
    ADD(LW(10), 1)
    READ_INT_INDEXED(LW(2), LW(10))
    ADD(LW(10), 1)
    READ_INT_INDEXED(LW(3), LW(10))
    ADD(LW(10), 1)
    READ_INT_INDEXED(LW(4), LW(10))
    CALL(evt_npc_set_position, STRING("モニー"), LW(1), LW(2), LW(3))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("モニー"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_02"), 0, STRING("モニー"))
    CALL(evt_npc_move_position, STRING("モニー"), 700, 0, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_ry, STRING("モニー"), 270)
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_03"), 0, STRING("モニー"))

    // Peeton runs in after her; Monie keeps talking while Mario reacts with surprise and a raised hand.
    SET_INT_READ_ADDR(PTR(suifudata_after))
    SET_VALUE(LW(10), 96)
    MULTIPLY(LW(10), 4)
    READ_INT_INDEXED(LW(1), LW(10))
    ADD(LW(10), 1)
    READ_INT_INDEXED(LW(2), LW(10))
    ADD(LW(10), 1)
    READ_INT_INDEXED(LW(3), LW(10))
    ADD(LW(10), 1)
    READ_INT_INDEXED(LW(4), LW(10))
    CALL(evt_npc_set_position, STRING("ピートン"), LW(1), LW(2), LW(3))
    CALL(evt_npc_move_position, STRING("ピートン"), 750, 0, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_ry, STRING("ピートン"), 270)
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_04"), 0, STRING("モニー"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_05"), 0, STRING("モニー"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NO1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_B"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_06"), 0, STRING("モニー"))

    // Monie and Peeton become talkable, the story moves on to the stranded sailors step and control returns.
    CALL(evt_npc_flag_onoff, 0, STRING("モニー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_change_interrupt, STRING("モニー"), 6, PTR(mony_talk))
    CALL(evt_npc_flag_onoff, 0, STRING("ピートン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_change_interrupt, STRING("ピートン"), 6, PTR(peton_talk))
    SET_VALUE(GSW_STORY_PROGRESS, STG5_30_SHIPWRECKED_SAILORS)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Cutscene on entering at STG5_34_KICKED_OFF:
 * Sailors D, E and C discuss the situation with Mario (stg5_dou_17_01 to 06),
 * then the story moves to STG5_35_ESCAPE_ROUTE.
 */
EVT_BEGIN(escapeway_event)
    CALL(evt_mario_key_onoff, 0)
#if VERSION >= VERSION_JP
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
#endif
    CALL(evt_mario_normalize)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
#endif
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_01"), 0, STRING("水夫D"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_02"), 0, STRING("水夫D"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_03"), 0, STRING("水夫E"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_04"), 0, STRING("水夫C"))
    CALL(evt_cam3d_evt_set, 775, 220, 331, 775, 150, -63, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("水夫C"), 270)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_05"), 0, STRING("水夫C"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_06"), 0, STRING("水夫E"))
    SET_VALUE(GSW_STORY_PROGRESS, STG5_35_ESCAPE_ROUTE)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

#if VERSION >= VERSION_JP
/**
 * After the wall is blown open: Sailor C is made intangible and Mario
 * and the partner are walked to their places at (730, -120) and (712, -126) to watch. JP and later only.
 */
EVT_BEGIN(bombwall_escape_move)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫C"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    BEGIN_INLINE_EVENT
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(1), 100)
            RETURN
        END_IF
        CALL(evt_party_stop, 0)
        CALL(evt_party_move_pos2, 0, 712, -126, FLOAT(120.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
        CALL(evt_party_run, 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS(LW(1), 100)
        RETURN
    END_IF
    CALL(evt_mario_mov_pos2, 730, -120, FLOAT(120.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    RETURN
EVT_END()
#endif

/**
 * Case script run when Bobbery bombs the cracked wall at n_bero in the grotto's big room:
 * breaks the wall, starts the sailors' escape (bombwall_escape_move and bombwall_after),
 * enables the n_bero exit and sets the story progress to STG5_36_HOLE_IN_THE_WALL.
 */
static EVT_BEGIN(bombwall_bomb)
    CALL(evt_mario_key_onoff, 0)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_mario_normalize)
#endif
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ana_before"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
#if VERSION >= VERSION_JP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_wait_landon)
        CALL(evt_mario_normalize)
        SPAWN_EVENT(PTR(bombwall_escape_move))
#endif
    END_INLINE_EVENT
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_ana_before"), STRING("offscreen"))
    CALL(evt_offscreen_get_boundingbox2, STRING("offscreen"), LW(10), LW(11), LW(12), LW(13))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(12), 0)
        IF_INT_EQUAL(LW(13), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("capture_img"), 0, STRING("offscreen"), 1, LW(10), LW(11), LW(12), LW(13))
        CALL(evt_img_clear_virtual_point, STRING("capture_img"))
        CALL(evt_img_set_paper_anim, STRING("capture_img"), STRING("A_1"))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_img_wait_animend, STRING("capture_img"))
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_ana_before"))
    CALL(evt_img_free_capture, STRING("capture_img"), 0)
    CALL(evt_img_free_capture, STRING("capture_img"), 1)
    CALL(evt_img_onoff, STRING("capture_img"), 0)
    CALL(evt_img_release, STRING("capture_img"))
    CALL(evt_offscreen_delete, STRING("offscreen"))
    LOOP(0)
        CALL(evt_mario_party_use_check, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_wait_landon)
    SPAWN_EVENT(PTR(bombwall_after))
    SET_VALUE(LW(0), STRING("n_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("n_bero"), HIT_ATTR_NPC_ONLY)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_36_HOLE_IN_THE_WALL)
    WAIT_FRAMES(2)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Sets up the bomb wall by story progress: up to STG5_33_CORTEZ_AND_THE_TREASURE the n_bero exit is
 * closed, from STG5_34_KICKED_OFF to STG5_35_ESCAPE_ROUTE it is closed
 * and Bobbery's bomb triggers bombwall_bomb, and from STG5_36_HOLE_IN_THE_WALL on it is open.
 */
static EVT_BEGIN(bombwall_init)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ana_before1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_hibiato"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_hibimae"), MAP_OBJ_FLAG_HIDDEN)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(STG5_33_CORTEZ_AND_THE_TREASURE)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_hibimae"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("n_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("n_bero"), HIT_ATTR_NPC_ONLY)
        CASE_BETWEEN(STG5_34_KICKED_OFF, STG5_35_ESCAPE_ROUTE)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_hibiato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ana_before1"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("n_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("n_bero"), HIT_ATTR_NPC_ONLY)
        CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("n_bero"), 0, PTR(bombwall_bomb), 0)
        CASE_GREATER_OR_EQUAL(STG5_36_HOLE_IN_THE_WALL)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_hibiato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("n_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_hitobj_attr_onoff, 1, 1, STRING("n_bero"), HIT_ATTR_NPC_ONLY)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Written as the sound loop for the sailors streaming out through the hole, but LW(12) is set to 1 on every pass,
 * so it never starts SFX_STG5_KINOPIO_MOVE1 and never stops it.
 */
// Both sound-state flags reset to 1 each pass, so neither sound-start nor sound-stop branch is reached here.
EVT_BEGIN(bombwall_suifuout_secheck)
    LOOP(0)
        SET_VALUE(LW(0), 0)
        SET_VALUE(LW(11), 1)
        SET_VALUE(LW(12), 1)
        LOOP(0)
            CALL(evt_nannpc_get_work, LW(0), 0, LW(10))
            IF_INT_EQUAL(LW(10), 1)
                SET_VALUE(LW(11), 1)
                SET_VALUE(LW(14), LW(0))
            END_IF
            ADD(LW(0), 1)
            IF_INT_EQUAL(LW(0), 95)
                BREAK_LOOP
            END_IF
        END_LOOP
        IF_INT_EQUAL(LW(11), 1)
            IF_INT_EQUAL(LW(12), 0)
                CALL(evt_nannpc_get_position, LW(14), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_KINOPIO_MOVE1"), LW(0), LW(1), LW(2), LW(13))
                SET_VALUE(LW(12), 1)
            END_IF
        ELSE
            IF_INT_EQUAL(LW(12), 1)
                CALL(evt_snd_sfxoff, LW(13))
                SET_VALUE(LW(12), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Script for one sailor (nannpc id in LW(0)) fleeing through the new hole in the wall:
 * depending on where it stands it walks back along the lower level (X >= 710 on Y = 50), then jumps up in one or
 * two hops (Y = 70, Y = 100) with random spacing and speed, runs 50 units toward -Z and is moved below the map.
 */
EVT_BEGIN(bombwall_suifuout)
    CALL(evt_nannpc_set_work, LW(0), 0, 0)
    CALL(evt_nannpc_get_position, LW(0), LW(1), LW(2), LW(3))
    IF_INT_EQUAL(LW(2), 50)
        IF_INT_GREATER_OR_EQUAL(LW(1), 710)
            GOTO(10)
        ELSE
            GOTO(20)
        END_IF
    ELSE
        GOTO(30)
    END_IF
    LABEL(10)
    CALL(evt_nannpc_set_dir, LW(0), 270)
    CALL(evt_nannpc_get_position, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_sub_random, 50, LW(1))
    ADD(LW(1), 630)
    CALL(evt_nannpc_set_work, LW(0), 0, 1)
    CALL(evt_nannpc_move_position2, LW(0), LW(1), LW(2), LW(3), FLOAT(240.0f))
    CALL(evt_nannpc_set_work, LW(0), 0, 0)
    CALL(evt_sub_random, 150, LW(10))
    ADD(LW(10), 100)
    WAIT_MS(LW(10))
    LABEL(20)
    CALL(evt_nannpc_set_dir, LW(0), 90)
    CALL(evt_nannpc_get_position, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_sub_random, 25, LW(3))
    ADD(LW(3), -125)
    CALL(evt_sub_random, 200, LW(10))
    ADD(LW(10), 400)
    CALL(evt_sub_random, 20, LW(11))
    ADD(LW(11), 30)
    CALL(evt_nannpc_jump_position, LW(0), LW(1), 70, LW(3), LW(10), LW(11))
    CALL(evt_sub_random, 150, LW(10))
    ADD(LW(10), 100)
    WAIT_MS(LW(10))
    LABEL(30)
    CALL(evt_nannpc_set_dir, LW(0), 90)
    CALL(evt_nannpc_get_position, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_sub_random, 60, LW(1))
    ADD(LW(1), 750)
    CALL(evt_sub_random, 200, LW(10))
    ADD(LW(10), 400)
    CALL(evt_sub_random, 30, LW(11))
    ADD(LW(11), 40)
    CALL(evt_nannpc_jump_position, LW(0), LW(1), 100, LW(3), LW(10), LW(11))
    CALL(evt_sub_random, 150, LW(10))
    ADD(LW(10), 100)
    WAIT_MS(LW(10))
    LABEL(40)
    CALL(evt_nannpc_set_animtbl, LW(0), PTR(suifu_run_animtbl))
    CALL(evt_nannpc_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(3), -50)
    CALL(evt_nannpc_set_work, LW(0), 0, 1)
    CALL(evt_nannpc_move_position2, LW(0), LW(1), LW(2), LW(3), FLOAT(240.0f))
    CALL(evt_nannpc_set_work, LW(0), 0, 0)
    CALL(evt_nannpc_set_position, LW(0), 0, -1000, 0)
    RETURN
EVT_END()

/**
 * After Bobbery has blown the wall: the camera shows Sailor C, D, Peeton,
 * Monie and the crowd (95 sailors) leaving through the hole one
 * after another with a line each (stg5_dou_17_12 to 15), then Sailor E; the camera returns to Mario.
 */
EVT_BEGIN(bombwall_after)
    // Control is taken from Mario, who turns toward the new hole in the wall.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(0), 780)
        CALL(evt_mario_set_dir, 90, 200, 0)
    ELSE
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_IF

    // The camera shows the platform; Sailor C goes through the hole first with a line to Mario.
    CALL(evt_npc_flag_onoff, 1, STRING("水夫C"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫D"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("水夫E"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("モニー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ピートン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_cam3d_evt_set, 742, 233, 429, 742, 147, -63, 500, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING("水夫C"), 780, -135, 0, FLOAT(180.0f), 0)
    CALL(evt_set_dir_to_target, STRING("水夫C"), STRING("mario"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_12"), 0, STRING("水夫C"))
    CALL(evt_npc_move_position, STRING("水夫C"), 780, -185, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_position, STRING("水夫C"), 0, -1000, 0)

    // Sailor D follows through the hole and calls the others.
    CALL(evt_npc_set_ry, STRING("水夫D"), 270)
    WAIT_MS(200)
    CALL(evt_npc_move_position, STRING("水夫D"), 673, -38, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_ry, STRING("水夫D"), 90)
    WAIT_MS(200)
    CALL(evt_npc_jump_position_nohit, STRING("水夫D"), 676, 70, -110, 400, 50)
    WAIT_MS(200)
    CALL(evt_npc_jump_position_nohit, STRING("水夫D"), 780, 100, -135, 500, 70)
    CALL(evt_set_dir_to_target, STRING("水夫D"), STRING("mario"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_13"), 0, STRING("水夫D"))
    CALL(evt_npc_move_position, STRING("水夫D"), 780, -185, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_position, STRING("水夫D"), 0, -1000, 0)

    // Peeton and Monie follow and Monie says they are leaving.
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_ry, STRING("ピートン"), 270)
        WAIT_MS(200)
        CALL(evt_npc_move_position, STRING("ピートン"), 685, -48, 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_ry, STRING("ピートン"), 90)
        WAIT_MS(200)
        CALL(evt_npc_jump_position_nohit, STRING("ピートン"), 680, 70, -110, 400, 50)
        WAIT_MS(200)
        CALL(evt_npc_jump_position_nohit, STRING("ピートン"), 800, 100, -135, 500, 50)
        CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("mario"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("モニー"), 270)
    WAIT_MS(200)
    CALL(evt_npc_move_position, STRING("モニー"), 655, -48, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_ry, STRING("モニー"), 90)
    WAIT_MS(200)
    CALL(evt_npc_jump_position_nohit, STRING("モニー"), 650, 70, -110, 400, 50)
    WAIT_MS(200)
    CALL(evt_npc_jump_position_nohit, STRING("モニー"), 770, 100, -135, 500, 50)
    CALL(evt_set_dir_to_target, STRING("モニー"), STRING("mario"))
    WAIT_MS(300)
    WAIT_WHILE(GF(0))
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_14"), 0, STRING("モニー"))

    // Peeton and Monie leave, then the crowd of 95 sailors streams out of the hole one after another.
    CALL(evt_nannpc_set_subfunc, 2, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("ピートン"), 800, -185, 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, STRING("ピートン"), 0, -1000, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("モニー"), 770, -185, 0, FLOAT(180.0f), 0)
        CALL(evt_npc_set_position, STRING("モニー"), 0, -1000, 0)
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(bombwall_suifuout_secheck))
    SET_VALUE(LW(0), 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG5_KINOPIO_MOVE3"), 0)
    LOOP(0)
        SPAWN_EVENT(PTR(bombwall_suifuout))
        ADD(LW(0), 1)
        IF_INT_EQUAL(LW(0), 95)
            BREAK_LOOP
        END_IF
        WAIT_MS(60)
        SPAWN_EVENT(PTR(bombwall_suifuout))
        ADD(LW(0), 1)
        IF_INT_EQUAL(LW(0), 95)
            BREAK_LOOP
        END_IF
        WAIT_MS(60)
    END_LOOP

    // Sailor E is last to go, thanking Mario.
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("水夫E"), 270)
    WAIT_MS(200)
    CALL(evt_npc_move_position, STRING("水夫E"), 673, -38, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_ry, STRING("水夫E"), 90)
    WAIT_MS(200)
    CALL(evt_npc_jump_position_nohit, STRING("水夫E"), 676, 70, -110, 400, 50)
    WAIT_MS(200)
    CALL(evt_npc_jump_position_nohit, STRING("水夫E"), 780, 100, -135, 500, 70)
    CALL(evt_set_dir_to_target, STRING("水夫E"), STRING("mario"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("stg5_dou_17_15"), 0, STRING("水夫E"))
    CALL(evt_npc_move_position, STRING("水夫E"), 780, -185, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_position, STRING("水夫E"), 0, -1000, 0)

    // The camera returns to Mario and control is restored.
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(dai3_shurai_after)
    LOOP(0)
        CALL(evt_sub_random, 2500, LW(10))
        WAIT_MS(LW(10))
        WAIT_MS(1500)
        BEGIN_INLINE_EVENT
            CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 500)
        END_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 250)
        CALL(evt_eff64, STRING(""), STRING("hokori_n64"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    END_LOOP
    RETURN
EVT_END()

// Carry the rider by the ship's XY rotation delta only after a previous sample on the same named hit group.
USER_FUNC(dou_mario_kaiten_hosei) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);
    HitEntry* hit = hitNameToPtr(name);
    MarioWork* mario = marioGetPtr();
    Mtx m;
    Vec v;
    f32 angle;
    f32 delta;
    f32 dist;
    f32 rot;

    if (isFirstCall) {
        event->userdataf[1] = 0.0f;
        event->userdata[2] = 0;
    }
    if (mario->hitobjStandOn == NULL) {
        event->userdataf[1] = 0.0f;
        event->userdata[2] = 0;
        return EVT_RETURN_BLOCK;
    }
    if (hitIsGroup(mario->hitobjStandOn, hit)) {
        v = (Vec){0.0f, 1.0f, 0.0f};
        mtxGetRotationElement(hit->mapObj->transformMtx, m, 'x', 'y');
        PSMTXMultVec(m, &v, &v);
        angle = angleABf(0.0f, 0.0f, v.x, v.y);
        delta = angle - event->userdataf[1];
        hitObjGetPos(name, &v);
        dist = distABf(mario->position.x, mario->position.y, v.x, v.y);
        rot = angleABf(v.x, v.y, mario->position.x, mario->position.y);
        if ((HitEntry*)event->userdata[2] != NULL && strcmp(hit->joint->name, ((HitEntry*)event->userdata[2])->joint->name) == 0) {
            rot = 6.2831855f * (rot + delta) / 360.0f;
            mario->floorMove.x = (v.x + dist * (f32)sin(rot)) - mario->position.x;
            mario->floorMove.y = (v.y - dist * (f32)cos(rot)) - mario->position.y;
        }
        event->userdataf[1] = angle;
        event->userdata[2] = (EvtWord)hit;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Rocks the background ship (S_h_fune) between -10 and +10 degrees every 3 seconds forever with a shaking sound,
 * keeping its hit area bound and carrying Mario.
 */
EVT_BEGIN(haikei_funeyure)
    BEGIN_INLINE_EVENT
        CALL(dou_mario_kaiten_hosei, STRING("A_h_fune"))
    END_INLINE_EVENT
    CALL(evt_hit_bind_mapobj, STRING("A_h_fune"), STRING("S_h_fune"))
    LOOP(0)
        CALL(evt_mapobj_get_position, STRING("S_h_fune"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SHIP_SHAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -10, 10, 3000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, STRING("S_h_fune"), 0, 0, LW(0))
            CALL(evt_hit_bind_update, STRING("A_h_fune"))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_mapobj_get_position, STRING("S_h_fune"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_SHIP_SHAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 10, -10, 3000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, STRING("S_h_fune"), 0, 0, LW(0))
            CALL(evt_hit_bind_update, STRING("A_h_fune"))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

/**
 * Extra callback when Mario steps onto a floating barrel platform:
 * plays SFX_STG5_BARREL_ON1 at the map object named in LW(0).
 */
static EVT_BEGIN(evt_ride_taru)
    CALL(evt_mapobj_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BARREL_ON1"), LW(10), LW(11), LW(12), 0)
    RETURN
EVT_END()

/**
 * Bobbing loop for floating object 0: binds the hit area in LW(1) (if any) to the map object in LW(0),
 * waits a random 0.2 to 0.6 seconds, then every 130 ms steps through the 16 heights of the bobbing table
 * (evt_dou_normal_puka), moves the object to that height, copies it to GW(0) for torinoko_suifu_pukapuka
 * and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main0)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), GW(0))
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(GW(0), LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/**
 * Bobbing barrel 1 (object name in LW(0), hit area in LW(1), 0 for none):
 * after a random delay it bobs up and down forever using the shared dou_normal_puka table.
 */
EVT_BEGIN(dou_puka_event_main10)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/** Bobbing barrel 2: same as dou_puka_event_main10 for its own object. */
EVT_BEGIN(dou_puka_event_main11)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/** Bobbing barrel 3: same as dou_puka_event_main10 for its own object. */
EVT_BEGIN(dou_puka_event_main12)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/** Bobbing barrel 4: same as dou_puka_event_main10 for its own object. */
EVT_BEGIN(dou_puka_event_main13)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/** Bobbing bottle 1: same as dou_puka_event_main10 for its own object. */
EVT_BEGIN(dou_puka_event_main14)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/** Bobbing bottle 2: same as dou_puka_event_main10 for its own object. */
EVT_BEGIN(dou_puka_event_main15)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/** Bobbing bottle 3: same as dou_puka_event_main10 for its own object. */
EVT_BEGIN(dou_puka_event_main16)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/** Bobbing bottle 4: same as dou_puka_event_main10 for its own object. */
EVT_BEGIN(dou_puka_event_main17)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    END_IF
    SET_VALUE(LW(10), LW(0))
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(11))
        CALL(evt_mapobj_trans, 1, LW(10), 0, LW(11), 0)
        SET_RAW_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_VALUE(0, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(evt_hit_bind_update, LW(1))
        END_IF
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/**
 * Clears the state and offset registers of floating platform 1 (state 0 = idle, 1 = dipping,
 * 2 = dip finished; offset = its current height).
 */
static EVT_BEGIN(dou_puka_event_init1)
    SET_VALUE(GW(1), 0)
    SET_VALUE(GW(2), 0)
    RETURN
EVT_END()

/**
 * Stand-on case script for floating platform 1:
 * if idle it marks the platform as ridden, runs the extra callback in LW(3)
 * when there is one (e.g. the boat or barrel creak sound), then the shared dip animation dou_puka_event,
 * and marks the dip as finished (state 2).
 */
static EVT_BEGIN(dou_puka_event_1)
    IF_INT_NOT_EQUAL(GW(1), 0)
        RETURN
    END_IF
    SET_VALUE(GW(1), 1)
    SET_RAW_VALUE(LW(6), GW(2))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL_CHILD(LW(3))
    END_IF
    CALL_CHILD(PTR(dou_puka_event))
    SET_VALUE(GW(1), 2)
    RETURN
EVT_END()

/**
 * Step-off case script for floating platform 1:
 * once the dip has finished (state 2) it sets the platform back to idle so it can dip again.
 */
static EVT_BEGIN(dou_puka_event_rev_1)
    IF_INT_NOT_EQUAL(GW(1), 2)
        RETURN
    END_IF
    SET_VALUE(GW(1), 0)
    RETURN
EVT_END()

/**
 * Frame loop for floating platform 1: binds the hit area in LW(1) to the map object in LW(0),
 * then every frame moves the object to the height held in the platform's offset register
 * and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main1)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(2), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Idle bobbing of floating platform 1: after a random 0.2 to 0.6 second delay it runs the shared bobbing loop
 * dou_normal_puka, stops it during a dip and restarts it when the dip has finished.
 */
static EVT_BEGIN(dou_puka_event_sub1)
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_RAW_VALUE(LW(6), GW(2))
    SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(GW(1), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(0))
        LOOP(0)
            IF_INT_EQUAL(GW(1), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Clears the state and offset registers of floating platform 2 (state 0 = idle, 1 = dipping,
 * 2 = dip finished; offset = its current height).
 */
static EVT_BEGIN(dou_puka_event_init2)
    SET_VALUE(GW(3), 0)
    SET_VALUE(GW(4), 0)
    RETURN
EVT_END()

/**
 * Stand-on case script for floating platform 2:
 * if idle it marks the platform as ridden, runs the extra callback in LW(3)
 * when there is one (e.g. the boat or barrel creak sound), then the shared dip animation dou_puka_event,
 * and marks the dip as finished (state 2).
 */
static EVT_BEGIN(dou_puka_event_2)
    IF_INT_NOT_EQUAL(GW(3), 0)
        RETURN
    END_IF
    SET_VALUE(GW(3), 1)
    SET_RAW_VALUE(LW(6), GW(4))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL_CHILD(LW(3))
    END_IF
    CALL_CHILD(PTR(dou_puka_event))
    SET_VALUE(GW(3), 2)
    RETURN
EVT_END()

/**
 * Step-off case script for floating platform 2:
 * once the dip has finished (state 2) it sets the platform back to idle so it can dip again.
 */
static EVT_BEGIN(dou_puka_event_rev_2)
    IF_INT_NOT_EQUAL(GW(3), 2)
        RETURN
    END_IF
    SET_VALUE(GW(3), 0)
    RETURN
EVT_END()

/**
 * Frame loop for floating platform 2: binds the hit area in LW(1) to the map object in LW(0),
 * then every frame moves the object to the height held in the platform's offset register
 * and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main2)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(4), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Idle bobbing of floating platform 2: after a random 0.2 to 0.6 second delay it runs the shared bobbing loop
 * dou_normal_puka, stops it during a dip and restarts it when the dip has finished.
 */
static EVT_BEGIN(dou_puka_event_sub2)
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_RAW_VALUE(LW(6), GW(4))
    SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(GW(3), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(0))
        LOOP(0)
            IF_INT_EQUAL(GW(3), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Clears the state and offset registers of floating platform 3 (state 0 = idle, 1 = dipping,
 * 2 = dip finished; offset = its current height).
 */
static EVT_BEGIN(dou_puka_event_init3)
    SET_VALUE(GW(5), 0)
    SET_VALUE(GW(6), 0)
    RETURN
EVT_END()

/**
 * Stand-on case script for floating platform 3:
 * if idle it marks the platform as ridden, runs the extra callback in LW(3)
 * when there is one (e.g. the boat or barrel creak sound), then the shared dip animation dou_puka_event,
 * and marks the dip as finished (state 2).
 */
static EVT_BEGIN(dou_puka_event_3)
    IF_INT_NOT_EQUAL(GW(5), 0)
        RETURN
    END_IF
    SET_VALUE(GW(5), 1)
    SET_RAW_VALUE(LW(6), GW(6))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL_CHILD(LW(3))
    END_IF
    CALL_CHILD(PTR(dou_puka_event))
    SET_VALUE(GW(5), 2)
    RETURN
EVT_END()

/**
 * Step-off case script for floating platform 3:
 * once the dip has finished (state 2) it sets the platform back to idle so it can dip again.
 */
static EVT_BEGIN(dou_puka_event_rev_3)
    IF_INT_NOT_EQUAL(GW(5), 2)
        RETURN
    END_IF
    SET_VALUE(GW(5), 0)
    RETURN
EVT_END()

/**
 * Frame loop for floating platform 3: binds the hit area in LW(1) to the map object in LW(0),
 * then every frame moves the object to the height held in the platform's offset register
 * and updates the hit area.
 */
static EVT_BEGIN(dou_puka_event_main3)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(6), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Idle bobbing of floating platform 3: after a random 0.2 to 0.6 second delay it runs the shared bobbing loop
 * dou_normal_puka, stops it during a dip and restarts it when the dip has finished.
 */
static EVT_BEGIN(dou_puka_event_sub3)
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_RAW_VALUE(LW(6), GW(6))
    SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(GW(5), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(0))
        LOOP(0)
            IF_INT_EQUAL(GW(5), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    END_LOOP
    RETURN
EVT_END()

/** Resets the state (GW(7)) and offset (GW(8)) registers of the fifth dipping platform to 0. */
EVT_BEGIN(dou_puka_event_init4)
    SET_VALUE(GW(7), 0)
    SET_VALUE(GW(8), 0)
    RETURN
EVT_END()

/**
 * Case event for standing on the fifth dipping platform:
 * runs the shared dip once (guarded by GW(7)) with the offset register GW(8),
 * and runs the callback in LW(3) first (evt_ride_taru plays a sound).
 */
EVT_BEGIN(dou_puka_event_4)
    IF_INT_NOT_EQUAL(GW(7), 0)
        RETURN
    END_IF
    SET_VALUE(GW(7), 1)
    SET_RAW_VALUE(LW(6), GW(8))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL_CHILD(LW(3))
    END_IF
    CALL_CHILD(PTR(dou_puka_event))
    SET_VALUE(GW(7), 2)
    RETURN
EVT_END()

/** Case event for leaving the fifth platform: resets GW(7) to 0 once the dip has finished (GW(7) = 2). */
EVT_BEGIN(dou_puka_event_rev_4)
    IF_INT_NOT_EQUAL(GW(7), 2)
        RETURN
    END_IF
    SET_VALUE(GW(7), 0)
    RETURN
EVT_END()

/**
 * Applies the fifth platform's offset GW(8) to its map object every frame,
 * binding the hit area LW(1) to the object LW(0).
 */
EVT_BEGIN(dou_puka_event_main4)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(8), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Idle bobbing of the fifth platform: after a random delay runs dou_normal_puka on GW(8),
 * and stops it during a dip (GW(7) = 1) until the dip has finished (GW(7) = 2).
 */
EVT_BEGIN(dou_puka_event_sub4)
    CALL(evt_sub_random, 400, LW(0))
    ADD(LW(0), 200)
    WAIT_MS(LW(0))
    SET_RAW_VALUE(LW(6), GW(8))
    SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(GW(7), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        DELETE_EVENT(LW(0))
        LOOP(0)
            IF_INT_EQUAL(GW(7), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SPAWN_EVENT_GET_ID(PTR(dou_normal_puka), LW(0))
    END_LOOP
    RETURN
EVT_END()

/**
 * Makes the first two barrels (S_taru_n01 and n02) dip when stood on, using the shared dip scripts,
 * once one island switch has been pressed.
 */
EVT_BEGIN(pukapuka_watari_init01)
    SET_VALUE(LW(0), STRING("S_taru_n01"))
    SET_VALUE(LW(1), STRING("A_taru_n01"))
    SET_VALUE(LW(2), STRING("A_taru_n01"))
    SET_VALUE(LW(3), PTR(evt_ride_taru))
    CALL_CHILD(PTR(dou_puka_event_init1))
    SPAWN_EVENT(PTR(dou_puka_event_main1))
    SPAWN_EVENT(PTR(dou_puka_event_sub1))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("A_taru_n01"), 0, PTR(dou_puka_event_1), 0)
    CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("A_taru_n01"), 0, PTR(dou_puka_event_rev_1), 0)
    SET_VALUE(LW(0), STRING("S_taru_n02"))
    SET_VALUE(LW(1), STRING("A_taru_n02"))
    SET_VALUE(LW(2), STRING("A_taru_n02"))
    SET_VALUE(LW(3), PTR(evt_ride_taru))
    CALL_CHILD(PTR(dou_puka_event_init2))
    SPAWN_EVENT(PTR(dou_puka_event_main2))
    SPAWN_EVENT(PTR(dou_puka_event_sub2))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("A_taru_n02"), 0, PTR(dou_puka_event_2), 0)
    CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("A_taru_n02"), 0, PTR(dou_puka_event_rev_2), 0)
    RETURN
EVT_END()

/**
 * Makes the second two barrels (S_taru_n03 and n04) dip when stood on,
 * once both island switches have been pressed.
 */
EVT_BEGIN(pukapuka_watari_init02)
    SET_VALUE(LW(0), STRING("S_taru_n03"))
    SET_VALUE(LW(1), STRING("A_taru_n03"))
    SET_VALUE(LW(2), STRING("A_taru_n03"))
    SET_VALUE(LW(3), PTR(evt_ride_taru))
    CALL_CHILD(PTR(dou_puka_event_init3))
    SPAWN_EVENT(PTR(dou_puka_event_main3))
    SPAWN_EVENT(PTR(dou_puka_event_sub3))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("A_taru_n03"), 0, PTR(dou_puka_event_3), 0)
    CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("A_taru_n03"), 0, PTR(dou_puka_event_rev_3), 0)
    SET_VALUE(LW(0), STRING("S_taru_n04"))
    SET_VALUE(LW(1), STRING("A_taru_n04"))
    SET_VALUE(LW(2), STRING("A_taru_n04"))
    SET_VALUE(LW(3), PTR(evt_ride_taru))
    CALL_CHILD(PTR(dou_puka_event_init4))
    SPAWN_EVENT(PTR(dou_puka_event_main4))
    SPAWN_EVENT(PTR(dou_puka_event_sub4))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("A_taru_n04"), 0, PTR(dou_puka_event_4), 0)
    CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("A_taru_n04"), 0, PTR(dou_puka_event_rev_4), 0)
    RETURN
EVT_END()

/**
 * Hazard-water floors of this room (map hit names) and the point each puts Mario back at after he is bitten.
 */
HitDamageReturn hitDmgRetPoints_1st[] = {
    {"mod_01", {400.0f, 15.0f, 0.0f}},
    {"mod_02", {-650.0f, 200.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {-208.0f, 50.0f, -96.0f}},
    {"mod_010", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_01", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_02", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_03", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_04", {0.0f, -1000.0f, 0.0f}},
    {"a11_a_usiro", {160.0f, 37.0f, 0.0f}},
    {"a11_a_kaidana_01", {160.0f, 37.0f, 0.0f}},
    {"a11_a_kaidanb_01", {160.0f, 37.0f, 0.0f}},
    {"dokan_1", {0.0f, -1000.0f, 0.0f}},
    {"dokan_2", {0.0f, -1000.0f, 0.0f}},
    {"dokan_3", {0.0f, -1000.0f, 0.0f}},
    {"dokan_7", {0.0f, -1000.0f, 0.0f}},
    {0},
};

/**
 * Hazard-water floors of this room (map hit names) and the point each puts Mario back at after he is bitten.
 */
static HitDamageReturn hitDmgRetPoints[] = {
    {"mod_01", {725.0f, 50.0f, 90.0f}},
    {"mod_02", {-650.0f, 200.0f, 0.0f}},
    {"mod_03", {0.0f, -1000.0f, 0.0f}},
    {"mod_04", {-208.0f, 50.0f, -96.0f}},
    {"mod_010", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_01", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_02", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_03", {0.0f, -1000.0f, 0.0f}},
    {"Ship_Change_04", {0.0f, -1000.0f, 0.0f}},
    {"a11_a_usiro", {160.0f, 37.0f, 0.0f}},
    {"a11_a_kaidana_01", {160.0f, 37.0f, 0.0f}},
    {"a11_a_kaidanb_01", {160.0f, 37.0f, 0.0f}},
    {"dokan_1", {0.0f, -1000.0f, 0.0f}},
    {"dokan_2", {0.0f, -1000.0f, 0.0f}},
    {"dokan_3", {0.0f, -1000.0f, 0.0f}},
    {"dokan_7", {0.0f, -1000.0f, 0.0f}},
    {0},
};

/**
 * Init script of dou_11, the big room with the stranded sailors:
 * music and ambience, exits, NPCs (Sailors C, D, E; Monie and Peeton use their ring models),
 * the bombable wall by story step, the stranded-sailors cutscenes (STG5_29_WAVE_DODGING and
 * STG5_34_KICKED_OFF), the two blue island switches, the swaying background ship,
 * bobbing barrels and bottles, and the dust shaking during the X-Naut raid (muj_00 dai3_shurai; STG5_40_TRUE_IDENTITY).
 */
EVT_BEGIN(dou_11_init_evt)
    // Music, ambience and exits for the room; the NPCs are set up and Monie and Peeton get their ring models.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_DOU12"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_set_tribe, STRING("モニー"), STRING("モニー（指輪）"))
    CALL(evt_npc_set_tribe, STRING("ピートン"), STRING("ピートン（指輪）"))
    CALL(evt_map_playanim, STRING("nami"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("moku"), MAP_ANIM_PLAY_LOOP, 0)

    // The bombable wall is set up; at the wave-dodging step the stranded sailors cutscenes are prepared, and at
    // the kicked-off step the escape-route scene starts.
    SPAWN_EVENT(PTR(bombwall_init))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_29_WAVE_DODGING)
        IF_INT_EQUAL(GSWF_DOU_11_SAILORS_SHOUT_SEEN, 0)
            SPAWN_EVENT(PTR(torinoko_suifu))
        END_IF
        IF_INT_EQUAL(GSWF_DOU_11_SAILORS_CROSSED, 0)
            SPAWN_EVENT(PTR(torinoko_suifu_init))
            SPAWN_EVENT(PTR(torinoko_suifu_sakebu))
        END_IF
        CASE_EQUAL(STG5_34_KICKED_OFF)
        SPAWN_EVENT(PTR(escapeway_event))
    END_SWITCH
    IF_INT_EQUAL(GSWF_DOU_11_SAILORS_CROSSED, 1)
        CALL(evt_hitobj_onoff, STRING("fune_atari"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_29_WAVE_DODGING)
        IF_INT_EQUAL(GSWF_DOU_11_SAILORS_CROSSED, 1)
            SPAWN_EVENT(PTR(watarikiri_suifu_init))
            CALL(evt_npc_set_position, STRING("モニー"), 681, 50, -75)
            CALL(evt_npc_set_ry_lr, STRING("モニー"), 1)
            CALL(evt_npc_set_position, STRING("ピートン"), 714, 50, -78)
            CALL(evt_npc_set_ry_lr, STRING("ピートン"), 1)
            SET_VALUE(GW(10), 0)
            SPAWN_EVENT(PTR(suifu_orei))
        END_IF
    END_IF

    // Between the shipwrecked-sailors and escape-route steps the sailors stand in their finished positions.
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_30_SHIPWRECKED_SAILORS)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG5_35_ESCAPE_ROUTE)
            SPAWN_EVENT(PTR(watarikiri_suifu_init))
        END_IF
    END_IF
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_mobj_switch_blue, 2, STRING("mobj01"), -220, 120, -450, PTR(ukishima_part1), GSWF_DOU_11_SWITCH_1_PRESSED)
    CALL(evt_mobj_switch_blue, 2, STRING("mobj02"), 80, 120, -450, PTR(ukishima_part2), GSWF_DOU_11_SWITCH_2_PRESSED)
    SPAWN_EVENT(ukishima_init1)
        SPAWN_EVENT(ukishima_init2)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_32_CORTEZS_WARNING)
#else
    CALL(evt_mobj_switch_blue, 2, STRING("mobj01"), -220, 120, -450, PTR(ukishima_switch1_evt), GSWF_DOU_11_SWITCH_1_PRESSED)
    CALL(evt_mobj_switch_blue, 2, STRING("mobj02"), 80, 120, -450, PTR(ukishima_switch2_evt), GSWF_DOU_11_SWITCH_2_PRESSED)
    SPAWN_EVENT(PTR(ukishima_init1))
    SPAWN_EVENT(PTR(ukishima_init2))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
#endif
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_taru_n03"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n03"), 1, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_taru_n04"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n04"), 1, 0)
    END_IF

    // The background ship sways, the damage-return points depend on the story step, and the bobbing barrels and
    // bottles start.
    SPAWN_EVENT(PTR(haikei_funeyure))
    CALL(evt_map_entry_airport_harbor, 0, STRING("S_hikou"), STRING("A_hikou"))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG5_30_SHIPWRECKED_SAILORS)
        CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints_1st))
    ELSE
        CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    END_IF
    CALL_CHILD(PTR(evt_gazigazi_entry))
    SET_VALUE(LW(0), STRING("s11fune1"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main0), LW(10))
    SET_VALUE(LW(0), STRING("S_taru01"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main10), LW(10))
    SET_VALUE(LW(0), STRING("S_taru02"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main11), LW(10))
    SET_VALUE(LW(0), STRING("S_taru03"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main12), LW(10))
    SET_VALUE(LW(0), STRING("S_taru04"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main13), LW(10))
    SET_VALUE(LW(0), STRING("S_bin01"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main14), LW(10))
    SET_VALUE(LW(0), STRING("S_bin02"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main15), LW(10))
    SET_VALUE(LW(0), STRING("S_bin03"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main16), LW(10))
    SET_VALUE(LW(0), STRING("S_bin04"))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT_GET_ID(PTR(dou_puka_event_main17), LW(10))
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF_DOU_11_SWITCH_1_PRESSED, 1)
        SPAWN_EVENT(pukapuka_watari_init01)
#else
    SET_VALUE(LW(0), GSWF_DOU_11_SWITCH_1_PRESSED)
    ADD(LW(0), GSWF_DOU_11_SWITCH_2_PRESSED)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        SPAWN_EVENT(PTR(pukapuka_watari_init01))
#endif
    END_IF
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF_DOU_11_SWITCH_2_PRESSED, 1)
        SPAWN_EVENT(pukapuka_watari_init02)
#else
    IF_INT_GREATER_OR_EQUAL(LW(0), 2)
        SPAWN_EVENT(PTR(pukapuka_watari_init02))
#endif
    END_IF

    // Ship-change platform; during the X-Naut raid, dust shaking starts.
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change_01"), STRING("Ship_Change_01"))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_40_TRUE_IDENTITY)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG5_41_RETURN_TO_THE_GROTTO)
            SPAWN_EVENT(PTR(dai3_shurai_after))
        END_IF
    END_IF
    CALL(evt_hitobj_onoff, STRING("A_ana_before"), 1, 0)
    RETURN
EVT_END()


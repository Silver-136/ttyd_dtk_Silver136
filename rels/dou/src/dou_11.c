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
USER_FUNC(evt_mario_get_name_hitobj_ride);
USER_FUNC(evt_nannpc_entry);
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
EvtWord ukishima_init1[34];
EvtWord ukishima_part1[50];
EvtWord ukishima_init2[34];
EvtWord ukishima_part2[50];
EvtWord ukishima_switch1_evt[23];
EvtWord ukishima_switch2_evt[23];
EvtWord torinoko_suifu_pukapuka[69];
EvtWord _suifu_sakebu[101];
EvtWord torinoko_suifu_sakebu[103];
EvtWord torinoko_suifu_init[192];
EvtWord torinoko_suifu[172];
EvtWord suifu_orei[339];
EvtWord escapeway_event[98];
EvtWord bombwall_escape_move[58];
static EvtWord bombwall_bomb[187];
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
EvtWord dou_11_init_evt[307];

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

static BOOL hitIsGroup(HitEntry* target, HitEntry* group) {
    // whether target is group or one of its descendants
    return _hitIsGroupSub(target, group, FALSE);
}

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

static s16 suifu_animtbl[] = {
    0, 0, 1, 1, 2, 2, 3, 3,
    4, 4, 5, 5, 6, 6, 7, 7,
    8, 8, 9, 9, 10, 10, 11, 11,
    12, 12, 13, 13, 14, 14, 15, 15,
    16, 16, 17, 17, 18, 18, 19, 19,
    -1, 0,
};

static s16 suifu_run_animtbl[] = {
    20, 20, 21, 21, 22, 22, 23, 23,
    24, 24, 25, 25, 26, 26, 27, 27,
    28, 28, 29, 29, -1, 0,
};

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

static EVT_BEGIN(suifu_c_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_09"), 0, STRING("me"))
        CASE_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

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

static EVT_BEGIN(suifu_d_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_07"), 0, STRING("me"))
        CASE_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_07"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(suifu_e_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_35_ESCAPE_ROUTE)
#ifdef VERSION_G8ME01
        CALL(evt_npc_set_position, STRING("me"), 736, 50, 113)
#else
        CALL(evt_npc_set_position, STRING("me"), 750, 50, -25)
#endif
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_34_KICKED_OFF)
        CALL(evt_npc_set_ry, STRING("me"), 90)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(suifu_e_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_34_KICKED_OFF)
        CALL(evt_msg_print, 0, STRING("stg5_dou_16_08"), 0, STRING("me"))
        CASE_EQUAL(STG5_35_ESCAPE_ROUTE)
        CALL(evt_msg_print, 0, STRING("stg5_dou_17_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(mony_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_35_ESCAPE_ROUTE)
        CALL(evt_npc_set_position, STRING("me"), 700, 50, 0)
    END_SWITCH
    RETURN
EVT_END()

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

static EVT_BEGIN(peton_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG5_30_SHIPWRECKED_SAILORS, STG5_35_ESCAPE_ROUTE)
        CALL(evt_npc_set_position, STRING("me"), 750, 50, 0)
    END_SWITCH
    RETURN
EVT_END()

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

static NpcSetupInfo npcEnt[] = {
    {
        /*"水夫C"*/"\x90\x85\x95\x76\x43", // name
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
        /*"水夫D"*/"\x90\x85\x95\x76\x44", // name
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
        /*"水夫E"*/"\x90\x85\x95\x76\x45", // name
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
        /*"モニー"*/"\x83\x82\x83\x6A\x81\x5B", // name
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
        /*"ピートン"*/"\x83\x73\x81\x5B\x83\x67\x83\x93", // name
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
        "dou_10", // nextMapName
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
        "muj_10", // nextMapName
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
        9, // directionId
        100000, // centerPosition
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
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
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
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
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
        9, // directionId
        100000, // centerPosition
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
        9, // directionId
        100000, // centerPosition
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
        9, // directionId
        100000, // centerPosition
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

EVT_BEGIN(ukishima_watari_run)
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
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), LW(10), 25, LW(12), LW(14), LW(13))
    CALL(evt_sub_random, 30, LW(10))
    ADD(LW(10), 50)
    WAIT_MS(LW(10))
    CALL(evt_nannpc_get_position, LW(0), LW(7), LW(8), LW(9))
    CALL(evt_nannpc_jump_position, LW(0), 625, 50, 0, 250, 50)
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
#ifdef VERSION_G8ME01
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

EVT_BEGIN(ukishima_watari)
    CALL(evt_cam3d_evt_set, 160, 174, 527, 160, 79, -14, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_nannpc_get_position, 99, LW(1), LW(2), LW(3))
    CALL(evt_nannpc_set_dir, 99, 90)
    CALL(evt_msg_toge, 2, LW(1), LW(2), LW(3))
    CALL(evt_msg_print, 0, STRING("stg5_dou_14"), 0, 0)
    SET_VALUE(LW(0), 0)
    LOOP(100)
        CALL(evt_nannpc_set_dir, LW(0), 90)
        ADD(LW(0), 1)
    END_LOOP
    CALL(evt_nannpc_get_position, 98, LW(1), LW(2), LW(3))
    CALL(evt_nannpc_set_dir, 98, 90)
    CALL(evt_msg_toge, 2, LW(1), LW(2), LW(3))
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
    SET_VALUE(GSWF(2992), 1)
    CALL(evt_hitobj_onoff, STRING("fune_atari"), 1, 0)
    CALL(evt_mario_key_onoff, 1)
    SPAWN_EVENT(PTR(suifu_orei))
    RETURN
EVT_END()

EVT_BEGIN(ukishima_check)
    SET_VALUE(LW(0), GSWF(2983))
    ADD(LW(0), GSWF(2984))
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

static EVT_BEGIN(ukishima_apper)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_mapobj_get_position, LW(2), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BARREL_APPEAR1"), LW(10), LW(11), LW(12), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, LW(2), LW(10), LW(11), LW(12))
        CALL(evt_eff64, STRING(""), STRING("ripple_n64"), 0, LW(10), 5, LW(12), 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(400)
        CALL(evt_eff64, STRING(""), STRING("ripple_n64"), 0, LW(10), 5, LW(12), 0, 0, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(2), MAP_OBJ_FLAG_HIDDEN)
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

EVT_BEGIN(ukishima_init1)
    SET_VALUE(LW(0), GSWF(2983))
    ADD(LW(0), GSWF(2984))
    IF_INT_LESS(LW(0), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_taru_n01"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n01"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_taru_n02"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n02"), 1, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ukishima_part1)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 280, 174, 527, 280, 79, -14, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
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

EVT_BEGIN(ukishima_init2)
    SET_VALUE(LW(0), GSWF(2983))
    ADD(LW(0), GSWF(2984))
    IF_INT_LESS(LW(0), 2)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_taru_n03"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n03"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_taru_n04"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n04"), 1, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ukishima_part2)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 525, 174, 527, 525, 79, -14, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
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

EVT_BEGIN(ukishima_switch1_evt)
    SET_VALUE(GSWF(2983), 1)
    SET_VALUE(LW(0), GSWF(2983))
    ADD(LW(0), GSWF(2984))
    IF_INT_EQUAL(LW(0), 1)
        SPAWN_EVENT(PTR(ukishima_part1))
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SPAWN_EVENT(PTR(ukishima_part2))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(ukishima_switch2_evt)
    SET_VALUE(GSWF(2984), 1)
    SET_VALUE(LW(0), GSWF(2983))
    ADD(LW(0), GSWF(2984))
    IF_INT_EQUAL(LW(0), 1)
        SPAWN_EVENT(PTR(ukishima_part1))
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SPAWN_EVENT(PTR(ukishima_part2))
    END_IF
    RETURN
EVT_END()

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

EVT_BEGIN(_suifu_sakebu)
    LOOP(0)
        WAIT_MS(700)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, 2, 115, 130, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_01"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(600)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, 2, 105, 80, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_03"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(600)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, 2, 123, 30, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_05"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(500)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, 2, 210, 135, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_02"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(800)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, 2, 225, 85, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_06"), 0, 0)
        END_INLINE_EVENT
        WAIT_MS(400)
        BEGIN_INLINE_EVENT
            CALL(evt_msg_toge, 2, 195, 37, 0)
            CALL(evt_msg_print, 0, STRING("stg5_dou_13_04"), 0, 0)
        END_INLINE_EVENT
    END_LOOP
    RETURN
EVT_END()

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
            IF_INT_EQUAL(GSWF(2983), 1)
                IF_INT_EQUAL(GSWF(2984), 1)
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

EVT_BEGIN(torinoko_suifu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("水夫C"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("水夫D"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("水夫E"), 0, -1000, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)
    CALL(evt_msg_toge, 2, -400, 60, 30)
    CALL(evt_msg_print, 0, STRING("stg5_dou_11"), 0, 0)
    CALL(evt_cam3d_evt_set, 160, 180, 559, 160, 85, 17, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        WAIT_MS(1800)
        CALL(evt_msg_toge, 2, 115, 130, 0)
        CALL(evt_msg_print, 0, STRING("small_00"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1200)
        CALL(evt_msg_toge, 2, 105, 80, 0)
        CALL(evt_msg_print, 0, STRING("small_01"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(900)
        CALL(evt_msg_toge, 2, 123, 30, 0)
        CALL(evt_msg_print, 0, STRING("small_02"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_msg_toge, 2, 210, 135, 0)
        CALL(evt_msg_print, 0, STRING("small_03"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_msg_toge, 2, 225, 85, 0)
        CALL(evt_msg_print, 0, STRING("small_04"), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(2100)
        CALL(evt_msg_toge, 2, 195, 37, 0)
        CALL(evt_msg_print, 0, STRING("small_05"), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(4000)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_toge, 2, -400, 60, 30)
    CALL(evt_msg_print, 0, STRING("stg5_dou_13"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF(2982), 1)
    RETURN
EVT_END()

EVT_BEGIN(suifu_orei)
    CALL(evt_npc_set_position, STRING("水夫C"), 825, 100, -125)
    CALL(evt_npc_set_ry, STRING("水夫C"), 270)
    CALL(evt_npc_set_position, STRING("水夫D"), 775, 50, 50)
    CALL(evt_npc_set_ry, STRING("水夫D"), 270)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_position, STRING("水夫E"), 736, 50, 113)
#else
    CALL(evt_npc_set_position, STRING("水夫E"), 750, 50, -25)
#endif
    CALL(evt_npc_set_ry, STRING("水夫E"), 270)
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
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_set_dir_to_target, STRING("水夫D"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_16"), 0, STRING("水夫D"))
    CALL(evt_set_dir_to_target, STRING("水夫E"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_01"), 0, STRING("水夫E"))
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
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("モニー"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_02"), 0, STRING("モニー"))
    CALL(evt_npc_move_position, STRING("モニー"), 700, 0, 0, FLOAT(180.0f), 0)
    CALL(evt_npc_set_ry, STRING("モニー"), 270)
    CALL(evt_msg_print, 0, STRING("stg5_dou_16_03"), 0, STRING("モニー"))
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
    CALL(evt_npc_flag_onoff, 0, STRING("モニー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_change_interrupt, STRING("モニー"), 6, PTR(mony_talk))
    CALL(evt_npc_flag_onoff, 0, STRING("ピートン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_change_interrupt, STRING("ピートン"), 6, PTR(peton_talk))
    SET_VALUE(GSW_STORY_PROGRESS, STG5_30_SHIPWRECKED_SAILORS)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(escapeway_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_normalize)
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

static EVT_BEGIN(bombwall_bomb)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_ana_before"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_wait_landon)
        CALL(evt_mario_normalize)
        SPAWN_EVENT(PTR(bombwall_escape_move))
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
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_img_wait_animend, STRING("capture_img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
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
    CALL(evt_hitobj_attr_onoff, 1, 1, STRING("n_bero"), 4)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_36_HOLE_IN_THE_WALL)
    WAIT_FRAMES(2)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(bombwall_init)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ana_before1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hibiato"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hibimae"), MAP_OBJ_FLAG_HIDDEN)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(STG5_33_CORTEZ_AND_THE_TREASURE)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hibimae"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("n_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("n_bero"), 4)
        CASE_BETWEEN(STG5_34_KICKED_OFF, STG5_35_ESCAPE_ROUTE)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hibiato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_before1"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("n_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("n_bero"), 4)
        CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
        CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("n_bero"), 0, PTR(bombwall_bomb), 0)
        CASE_GREATER_OR_EQUAL(STG5_36_HOLE_IN_THE_WALL)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_hibiato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ana_after"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("n_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_hitobj_attr_onoff, 1, 1, STRING("n_bero"), 4)
    END_SWITCH
    RETURN
EVT_END()

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

EVT_BEGIN(bombwall_after)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(0), 780)
        CALL(evt_mario_set_dir, 90, 200, 0)
        ELSE
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_IF
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
            CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 500)
        END_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 250)
        CALL(evt_eff64, STRING(""), STRING("hokori_n64"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    END_LOOP
    RETURN
EVT_END()

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

static EVT_BEGIN(evt_ride_taru)
    CALL(evt_mapobj_get_position, LW(0), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_BARREL_ON1"), LW(10), LW(11), LW(12), 0)
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_init1)
    SET_VALUE(GW(1), 0)
    SET_VALUE(GW(2), 0)
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_rev_1)
    IF_INT_NOT_EQUAL(GW(1), 2)
        RETURN
    END_IF
    SET_VALUE(GW(1), 0)
    RETURN
EVT_END()

static EVT_BEGIN(dou_puka_event_main1)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(2), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_init2)
    SET_VALUE(GW(3), 0)
    SET_VALUE(GW(4), 0)
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_rev_2)
    IF_INT_NOT_EQUAL(GW(3), 2)
        RETURN
    END_IF
    SET_VALUE(GW(3), 0)
    RETURN
EVT_END()

static EVT_BEGIN(dou_puka_event_main2)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(4), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_init3)
    SET_VALUE(GW(5), 0)
    SET_VALUE(GW(6), 0)
    RETURN
EVT_END()

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

static EVT_BEGIN(dou_puka_event_rev_3)
    IF_INT_NOT_EQUAL(GW(5), 2)
        RETURN
    END_IF
    SET_VALUE(GW(5), 0)
    RETURN
EVT_END()

static EVT_BEGIN(dou_puka_event_main3)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(6), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

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

EVT_BEGIN(dou_puka_event_init4)
    SET_VALUE(GW(7), 0)
    SET_VALUE(GW(8), 0)
    RETURN
EVT_END()

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

EVT_BEGIN(dou_puka_event_rev_4)
    IF_INT_NOT_EQUAL(GW(7), 2)
        RETURN
    END_IF
    SET_VALUE(GW(7), 0)
    RETURN
EVT_END()

EVT_BEGIN(dou_puka_event_main4)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    LOOP(0)
        CALL(evt_mapobj_trans, 1, LW(0), 0, GW(8), 0)
        CALL(evt_hit_bind_update, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

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

EVT_BEGIN(dou_11_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_DOU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_DOU12"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_set_tribe, STRING("モニー"), STRING("モニー（指輪）"))
    CALL(evt_npc_set_tribe, STRING("ピートン"), STRING("ピートン（指輪）"))
    CALL(evt_map_playanim, STRING("nami"), 1, 0)
    CALL(evt_map_playanim, STRING("moku"), 1, 0)
    SPAWN_EVENT(PTR(bombwall_init))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_29_WAVE_DODGING)
        IF_INT_EQUAL(GSWF(2982), 0)
            SPAWN_EVENT(PTR(torinoko_suifu))
        END_IF
        IF_INT_EQUAL(GSWF(2992), 0)
            SPAWN_EVENT(PTR(torinoko_suifu_init))
            SPAWN_EVENT(PTR(torinoko_suifu_sakebu))
        END_IF
        CASE_EQUAL(STG5_34_KICKED_OFF)
        SPAWN_EVENT(PTR(escapeway_event))
    END_SWITCH
    IF_INT_EQUAL(GSWF(2992), 1)
        CALL(evt_hitobj_onoff, STRING("fune_atari"), 1, 0)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_29_WAVE_DODGING)
        IF_INT_EQUAL(GSWF(2992), 1)
            SPAWN_EVENT(PTR(watarikiri_suifu_init))
            CALL(evt_npc_set_position, STRING("モニー"), 681, 50, -75)
            CALL(evt_npc_set_ry_lr, STRING("モニー"), 1)
            CALL(evt_npc_set_position, STRING("ピートン"), 714, 50, -78)
            CALL(evt_npc_set_ry_lr, STRING("ピートン"), 1)
            SET_VALUE(GW(10), 0)
            SPAWN_EVENT(PTR(suifu_orei))
        END_IF
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_30_SHIPWRECKED_SAILORS)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG5_35_ESCAPE_ROUTE)
            SPAWN_EVENT(PTR(watarikiri_suifu_init))
        END_IF
    END_IF
    CALL(evt_mobj_switch_blue, 2, STRING("mobj01"), -220, 120, -450, PTR(ukishima_switch1_evt), GSWF(2983))
    CALL(evt_mobj_switch_blue, 2, STRING("mobj02"), 80, 120, -450, PTR(ukishima_switch2_evt), GSWF(2984))
    SPAWN_EVENT(PTR(ukishima_init1))
    SPAWN_EVENT(PTR(ukishima_init2))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_33_CORTEZ_AND_THE_TREASURE)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_taru_n03"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n03"), 1, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_taru_n04"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_taru_n04"), 1, 0)
    END_IF
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
    SET_VALUE(LW(0), GSWF(2983))
    ADD(LW(0), GSWF(2984))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        SPAWN_EVENT(PTR(pukapuka_watari_init01))
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(0), 2)
        SPAWN_EVENT(PTR(pukapuka_watari_init02))
    END_IF
    CALL(evt_map_entry_airport_harbor, 1, STRING("S_Ship_Change_01"), STRING("Ship_Change_01"))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_40_TRUE_IDENTITY)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG5_41_RETURN_TO_THE_GROTTO)
            SPAWN_EVENT(PTR(dai3_shurai_after))
        END_IF
    END_IF
    CALL(evt_hitobj_onoff, STRING("A_ana_before"), 1, 0)
    RETURN
EVT_END()


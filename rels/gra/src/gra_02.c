#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"

#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "evt/evt_bero.h"
#include "gra.h"
#include "evt/evt_npc.h"

static USER_FUNC(booboo_main);
USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_init);
USER_FUNC(evt_kagemario_init);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);

EvtWord shopmaster_pig_init[14];
EvtWord shopmaster_talk[33];
static EvtWord urouro_move[24];
static NpcSetupInfo npcEnt[2];
EvtWord w_bero_2_in[5];
static BeroEntry bero_entry_data[5];
EvtWord gra02_get_key[5];
EvtWord gra02_straw_blow_evt[23];
static const char* booboo_name[1];
EvtWord gra_02_init_evt[86];

EVT_BEGIN(shopmaster_pig_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタお店の主人"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(shopmaster_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_gra_17"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_gra_18"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(urouro_move)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), 1073741824)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(50.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ブタお店の主人"*/"\x83\x75\x83\x5E\x82\xA8\x93\x58\x82\xCC\x8E\xE5\x90\x6C", // name
        0x600, // flags
        0, // reactionFlags
        shopmaster_pig_init, // initEvent
        urouro_move, // regularEvent
        shopmaster_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(w_bero_2_in)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
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
        "gra_01", // nextMapName
        "e_bero1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "w_bero_2", // hitName
        4096, // type
        0, // sfxId
        6, // directionId
        242, // centerPosition
        21, // centerPositionY
        -450, // centerPositionZ
        -1, // length
        w_bero_2_in, // enterEvent
        0, NULL, // caseType..exitEvent
        "gra_03", // nextMapName
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
    {0},
};

EVT_BEGIN(gra02_get_key)
    SET_VALUE(GSW_STORY_PROGRESS, 184)
    RETURN
EVT_END()

EVT_BEGIN(gra02_straw_blow_evt)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(1000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_INLINE_EVENT
    SET_VALUE(GSW_STORY_PROGRESS, 188)
    RETURN
EVT_END()

static const char* booboo_name[] = {
    /*"ブタお店の主人"*/"\x83\x75\x83\x5E\x82\xA8\x93\x58\x82\xCC\x8E\xE5\x90\x6C",
};

EVT_BEGIN(gra_02_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_GRA1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG4_USU1"))
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_item_entry, STRING("item00"), 125, -125, 0, -110, 16, GSWF(5631), 0)
    CALL(evt_item_entry, STRING("item01"), 125, 305, 0, -35, 16, GSWF(5632), 0)
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_npc_setup, PTR(npcEnt))
    IF_INT_LESS(GSW_STORY_PROGRESS, 184)
        CALL(evt_item_entry, STRING("key00"), 21, -30, 0, 0, 0, -1, PTR(gra02_get_key))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 188)
        CALL(evt_cloud_init)
        CALL(evt_cloud_ent, 0, STRING("S_wara"), STRING("polySurface981_q023"), PTR(gra02_straw_blow_evt))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_wara"), 1)
        CALL(evt_hitobj_onoff, STRING("polySurface981_q023"), 0, 0)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(booboo_main)
    END_INLINE_EVENT
    RETURN
EVT_END()

static USER_FUNC(booboo_main) {
    animPoseSetMaterialLightFlagOn(npcNameToPtr((const char*)booboo_name[0])->poseId, 0x10);
    return EVT_RETURN_DONE;
}

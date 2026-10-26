#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"


static EvtWord w_door_open[12];
static EvtWord w_door_close[12];
static BeroEntry bero_data[2];
static EvtWord evt_kinoshikowa[111];
static EvtWord evt_block_broken[57];
static EvtWord init_michel[22];
EvtWord init_michel_dummy[12];
static EvtWord talk_michel[8];
static EvtWord init_brots[35];
EvtWord init_brots_dummy[12];
static EvtWord talk_brots[91];
static EvtWord init_kinoshikowa[2];
static EvtWord talk_kinoshikowa[2];
static NpcSetupInfo npcEnt[6];
EvtWord tou_12_init_evt[133];

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "w_bero", // hitName
        1, // type
        4, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_06", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_kinoshikowa)
    CALL(evt_npc_set_position, STRING("キノシコワ"), -200, 0, 10)
    CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, 30, 0)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(0), -55)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_eff_fukidashi, 1, 0, STRING("キノシコワ"), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_move_position, STRING("キノシコワ"), -250, 10, 0, FLOAT(80.0f), 1)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING("door_1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD2"), LW(0), LW(1), LW(2), 0)
    SET_VALUE(LW(0), 30)
    LOOP(10)
        SUBTRACT(LW(0), 3)
        CALL(evt_mapobj_rotate, 1, STRING("door_1"), 0, LW(0), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_307_13_00"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_block_broken)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_move_pos2, 0, 95, 10, FLOAT(80.0f))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SURPRISED2"))
    CALL(evt_party_set_dir, 0, 90, 200)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg3_tou_307_10"))
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_307_11"), 0, STRING("ブロッツダミー"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    SPAWN_EVENT(PTR(evt_kinoshikowa))
    SET_VALUE(GSW_STORY_PROGRESS, 156)
    RETURN
EVT_END()

static EVT_BEGIN(init_michel)
    IF_INT_LESS(GSW_STORY_PROGRESS, 164)
        CALL(evt_npc_set_anim, STRING("マイケル"), STRING("NKT_Z_1"))
        CALL(evt_npc_set_position, STRING("マイケル"), 175, 0, -20)
        CALL(evt_npc_set_rotate, STRING("マイケル"), -80, 0, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_michel_dummy)
    IF_INT_LESS(GSW_STORY_PROGRESS, 164)
        CALL(evt_npc_set_position, STRING("マイケルダミー"), 175, 0, -20)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_michel)
    CALL(evt_msg_print, 0, STRING("stg3_tou_307_14_01"), 0, STRING("マイケル"))
    RETURN
EVT_END()

static EVT_BEGIN(init_brots)
    IF_INT_LESS(GSW_STORY_PROGRESS, 164)
        CALL(evt_npc_set_position, STRING("ブロッツ"), 110, 3, -60)
        CALL(evt_npc_set_rotate, STRING("ブロッツ"), -70, 0, -80)
        CALL(evt_npc_flag_onoff, 0, STRING("ブロッツ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_set_anim, STRING("ブロッツ"), STRING("S_1"))
        IF_INT_EQUAL(GSWF(2518), 1)
            CALL(evt_npc_set_anim, STRING("ブロッツ"), STRING("X_1"))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_brots_dummy)
    IF_INT_LESS(GSW_STORY_PROGRESS, 164)
        CALL(evt_npc_set_position, STRING("ブロッツダミー"), 125, 4, -60)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(talk_brots)
    IF_INT_EQUAL(GSWF(2518), 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_party_set_breed_pose, 0, 1)
        CALL(evt_npc_get_position, STRING("ブロッツ"), LW(0), LW(1), LW(2))
        CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
        IF_INT_LESS(LW(0), LW(3))
            CALL(evt_party_set_dir, 0, 270, 200)
            ELSE
            CALL(evt_party_set_dir, 0, 90, 200)
        END_IF
        CALL(evt_npc_flag_onoff, 1, STRING("ブロッツ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_msg_print, 0, STRING("stg3_tou_307_12"), 0, 0)
        CALL(evt_npc_flag_onoff, 0, STRING("ブロッツ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_set_anim, STRING("ブロッツ"), STRING("X_1"))
        CALL(evt_msg_print_party, STRING("stg3_tou_307_13"))
        SET_VALUE(GSWF(2518), 1)
        CALL(evt_party_cont_onoff, 0, 1)
        ELSE
        CALL(evt_npc_flag_onoff, 0, STRING("ブロッツ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_set_anim, STRING("ブロッツ"), STRING("S_1"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_307_14"), 0, 0)
        CALL(evt_npc_set_anim, STRING("ブロッツ"), STRING("X_1"))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(init_kinoshikowa)
    RETURN
EVT_END()

static EVT_BEGIN(talk_kinoshikowa)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"マイケル"*/"\x83\x7D\x83\x43\x83\x50\x83\x8B", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_NO_TALK_TURN | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_michel, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブロッツ"*/"\x83\x75\x83\x8D\x83\x62\x83\x63", // name
        NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_TALK_TURN | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_brots, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マイケルダミー"*/"\x83\x7D\x83\x43\x83\x50\x83\x8B\x83\x5F\x83\x7E\x81\x5B", // name
        NPC_FLAG_NO_DRAW | NPC_FLAG_NO_TALK_TURN | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_michel_dummy, // initEvent
        NULL, // regularEvent
        talk_michel, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブロッツダミー"*/"\x83\x75\x83\x8D\x83\x62\x83\x63\x83\x5F\x83\x7E\x81\x5B", // name
        NPC_FLAG_NO_DRAW | NPC_FLAG_NO_TALK_TURN | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_brots_dummy, // initEvent
        NULL, // regularEvent
        talk_brots, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノシコワ"*/"\x83\x4C\x83\x6D\x83\x56\x83\x52\x83\x8F", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kinoshikowa, // initEvent
        NULL, // regularEvent
        talk_kinoshikowa, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(tou_12_init_evt)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 163)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envoff, PSND_FADEOUT)
        ELSE
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(127, 137)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            CASE_GREATER_OR_EQUAL(165)
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
            CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(6))
            CASE_DEFAULT
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU1"))
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU6"))
        END_SWITCH
    END_IF
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_item_entry, STRING("item01"), ITEM_STAR_PIECE, 190, 0, 30, ITEM_TYPE_STILL, GSWF(5627), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 156)
        CALL(evt_mobj_lv_blk, STRING("aa"), 125, 0, 0, PTR(evt_block_broken), GSWF(2441), STRING("MOBJ_Lv1BigBigBlock"))
        CALL(evt_mobj_flag_onoff, 0, STRING("aa"), MOBJ_FLAG_GRAVITY)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 164)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("gor_hae"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
        CALL(evt_map_playanim, STRING("anm_hae"), 1, 0)
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(evt_sub_random, 1000, LW(0))
                ADD(LW(0), 3000)
                WAIT_MS(LW(0))
                CALL(evt_mapobj_get_position, STRING("gor_hae"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_FLY1"), LW(0), LW(1), LW(2), 0)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()


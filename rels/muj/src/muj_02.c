#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_item.h"
#include "evt/evt_mobj.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "pmario_sound.h"

#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "manager/fontmgr.h"
#include "driver/camdrv.h"
#include "stdio.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "evt/evt_sub.h"
#include "muj.h"
#include "evt/evt_npc.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#include "evt/evt_npc.h"
#include "driver/hitdrv.h"
void FontDrawString(f32 x, f32 y, const char* str);
#endif

void printCount(CameraId camera, void* param);

USER_FUNC(countDisp);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_win_item_select);
#ifdef VERSION_G8ME01
#endif

static BeroEntry bero_entry_data[4];
EvtWord mony_peton_setup[139];
EvtWord lost_wedding_ring_sub[4];
EvtWord lost_wedding_ring[395];
static s32 itemsel_table00[2];
EvtWord wedding_ring_consign[30];
EvtWord find_wedding_ring[180];
static EvtWord mony_init[19];
EvtWord mony_peton_regl[97];
EvtWord mony_regl[13];
static EvtWord mony_talk[50];
static EvtWord peton_init[19];
EvtWord peton_regl[13];
static EvtWord peton_talk[50];
static NpcSetupInfo npcEnt[3];
static HitDamageReturn hitDmgRetPoints[2];
static EvtWord evt_switch_init[17];
EvtWord evt_treeb_01[20];
static EvtWord evt_kusa_2[20];
static EvtWord evt_kusa_6[20];
static TreeAccess tree_tbl[7];
static KusaAccess kusa_tbl[7];
#ifdef VERSION_G8ME01
EvtWord muj_02_init_evt[115];
#else
EvtWord muj_02_init_evt[104];
#endif

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
        "muj_01", // nextMapName
        "e_bero1", // nextBeroName
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
        "muj_03", // nextMapName
        "w_bero", // nextBeroName
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
        NULL, // exitEvent
        "tik_17", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(mony_peton_setup)
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_npc_get_position, STRING("モニー"), LW(0), LW(1), LW(2))
        BEGIN_INLINE_EVENT
            ADD(LW(0), 40)
            ADD(LW(2), 30)
            CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(120.0f))
            WAIT_FRAMES(1)
            CALL(evt_mario_set_dir, 270, 200, 0)
            CALL(evt_mario_dispflag_onoff, 1, 2)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            ADD(LW(0), 75)
            ADD(LW(2), 30)
            CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
            WAIT_FRAMES(1)
            CALL(evt_party_set_dir, 0, 270, -1)
        END_INLINE_EVENT
        SUBTRACT(LW(0), 55)
        CALL(evt_npc_set_ry, STRING("ピートン"), 90)
        CALL(evt_npc_move_position, STRING("ピートン"), LW(0), LW(2), 0, FLOAT(120.0f), 0)
        ELSE
        CALL(evt_npc_get_position, STRING("ピートン"), LW(0), LW(1), LW(2))
        BEGIN_INLINE_EVENT
            ADD(LW(0), -40)
            ADD(LW(2), 30)
            CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(120.0f))
            WAIT_FRAMES(1)
            CALL(evt_mario_set_dir, 90, 200, 0)
            CALL(evt_mario_dispflag_onoff, 1, 2)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            ADD(LW(0), -75)
            ADD(LW(2), 30)
            CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(120.0f))
            WAIT_FRAMES(1)
            CALL(evt_party_set_dir, 0, 90, -1)
        END_INLINE_EVENT
        ADD(LW(0), 55)
        CALL(evt_npc_set_ry, STRING("モニー"), 270)
        CALL(evt_npc_move_position, STRING("モニー"), LW(0), LW(2), 0, FLOAT(120.0f), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(lost_wedding_ring_sub)
    CALL(countDisp)
    RETURN
EVT_END()

EVT_BEGIN(lost_wedding_ring)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ピートン"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("モニー"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_get_reglid, STRING("モニー"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ピートン"), LW(0))
    DELETE_EVENT(LW(0))
    CALL_CHILD(PTR(mony_peton_setup))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("mario"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_171"), 0, STRING("ピートン"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_172"), 0, STRING("モニー"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING("モニー"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("ピートン"), LW(10), LW(11), LW(12))
    ADD(LW(0), LW(10))
    DIVIDE(LW(0), 2)
    CALL(evt_cam3d_evt_set, LW(0), 118, 343, LW(0), 54, -18, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_173"), 0, STRING("モニー"))
    CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("モニー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_174"), 0, STRING("ピートン"))
    CALL(evt_set_dir_to_target, STRING("モニー"), STRING("ピートン"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_175"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_176"), 0, STRING("ピートン"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_177"), 0, STRING("モニー"))
    CALL(evt_npc_get_position, STRING("ピートン"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("ピートン"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("stg5_muj_178"), 0, STRING("ピートン"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_179"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_180"), 0, STRING("ピートン"))
    CALL(evt_get_target_dir, STRING("モニー"), STRING("ピートン"), LW(0))
    ADD(LW(0), 180)
    CALL(evt_npc_set_ry, STRING("モニー"), LW(0))
    CALL(evt_msg_print, 0, STRING("stg5_muj_181"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_182"), 0, STRING("ピートン"))
    CALL(evt_set_dir_to_target, STRING("モニー"), STRING("ピートン"))
    WAIT_MS(500)
    SET_VALUE(GW(0), 10)
    SPAWN_EVENT_GET_ID(PTR(lost_wedding_ring_sub), LW(0))
    LOOP(90)
        CALL(evt_msg_print_add, 0, STRING("stg5_muj_182_01"))
        ADD(GW(0), 1)
    END_LOOP
    CALL(evt_msg_print_add, 0, STRING("stg5_muj_182_02"))
    DELETE_EVENT(LW(0))
    CALL(evt_msg_print_add, 0, STRING("stg5_muj_182_03"))
    CALL(evt_set_dir_to_target, STRING("モニー"), STRING("ピートン"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_183"), 0, STRING("モニー"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_184"), 0, STRING("ピートン"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_get_pos, 0, LW(10), LW(11), LW(12))
    ADD(LW(0), LW(10))
    DIVIDE(LW(0), 2)
    CALL(evt_cam3d_evt_set, LW(0), 118, 343, LW(0), 54, -18, 500, 11)
    WAIT_MS(500)
    CALL(evt_pouch_check_item, ITEM_WEDDING_RING, LW(10))
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_msg_print_party, STRING("stg5_muj_185"))
        ELSE
        CALL(evt_msg_print_party, STRING("stg5_muj_185_01"))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    CALL(evt_cam3d_evt_off, 500, 11)
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_npc_set_ry, STRING("ピートン"), 270)
        CALL(evt_npc_move_position, STRING("ピートン"), 400, -40, 0, FLOAT(120.0f), 0)
        ELSE
        CALL(evt_npc_set_ry, STRING("モニー"), 90)
        CALL(evt_npc_move_position, STRING("モニー"), 500, -40, 0, FLOAT(120.0f), 0)
    END_IF
    CALL(evt_npc_flag_onoff, 0, STRING("ピートン"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("モニー"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_restart_regular_event, STRING("モニー"))
    CALL(evt_npc_restart_regular_event, STRING("ピートン"))
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

static s32 itemsel_table00[] = {
    ITEM_WEDDING_RING, -1,
};

EVT_BEGIN(wedding_ring_consign)
    SET_VALUE(LW(0), 0)
    CALL(evt_pouch_check_item, ITEM_WEDDING_RING, LW(10))
    IF_INT_NOT_EQUAL(LW(10), 0)
        CALL(evt_win_item_select, 0, PTR(itemsel_table00), LW(10), 0)
        IF_INT_NOT_EQUAL(LW(10), -1)
            CALL(evt_pouch_remove_item, ITEM_WEDDING_RING, LW(10))
            SET_VALUE(LW(0), 1)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(find_wedding_ring)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ピートン"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("モニー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_reglid, STRING("モニー"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_reglid, STRING("ピートン"), LW(0))
    DELETE_EVENT(LW(0))
    CALL_CHILD(PTR(mony_peton_setup))
    CALL(evt_set_dir_to_target, STRING("モニー"), STRING("ピートン"))
    CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("モニー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_196"), 0, STRING("モニー"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_197"), 0, STRING("ピートン"))
    CALL(evt_set_dir_to_target, STRING("モニー"), STRING("mario"))
    CALL(evt_set_dir_to_target, STRING("ピートン"), STRING("mario"))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg5_muj_198"), 0, STRING("モニー"))
    SET_VALUE(GF(0), 1)
    SET_VALUE(GF(1), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_get_position, STRING("モニー"), LW(0), LW(1), LW(2))
        ADD(LW(0), -10)
        ADD(LW(2), 110)
        CALL(evt_npc_set_ry, STRING("モニー"), 270)
        CALL(evt_npc_move_position, STRING("モニー"), LW(0), LW(2), 0, FLOAT(120.0f), 0)
        ADD(LW(0), -200)
        CALL(evt_npc_move_position, STRING("モニー"), LW(0), LW(2), 0, FLOAT(120.0f), 0)
        CALL(evt_npc_set_position, STRING("モニー"), 0, -1000, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ピートン"), LW(0), LW(1), LW(2))
        ADD(LW(0), -10)
        ADD(LW(2), 110)
        CALL(evt_npc_set_ry, STRING("ピートン"), 270)
        CALL(evt_npc_move_position, STRING("ピートン"), LW(0), LW(2), 0, FLOAT(120.0f), 0)
        ADD(LW(0), -200)
        CALL(evt_npc_move_position, STRING("ピートン"), LW(0), LW(2), 0, FLOAT(120.0f), 0)
        CALL(evt_npc_set_position, STRING("ピートン"), 0, -1000, 0)
        SET_VALUE(GF(1), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(0))
    WAIT_WHILE(GF(1))
    CALL(evt_party_run, 0)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    RETURN
EVT_END()

static EVT_BEGIN(mony_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(274, 275)
        CALL(evt_npc_set_position, STRING("me"), 500, 0, -40)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(mony_peton_regl)
    LOOP(0)
        SET_VALUE(LW(3), LW(0))
        CALL(evt_sub_random, 15, LW(10))
        ADD(LW(10), 10)
        SUBTRACT(LW(3), LW(10))
        SET_VALUE(LW(4), LW(0))
        CALL(evt_sub_random, 15, LW(10))
        ADD(LW(10), 10)
        ADD(LW(4), LW(10))
        CALL(evt_sub_random, 500, LW(5))
        ADD(LW(5), 1000)
        CALL(evt_sub_random, 500, LW(6))
        ADD(LW(6), 1000)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        WAIT_MS(600)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        WAIT_MS(600)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_move_position, STRING("me"), LW(3), LW(2), 0, FLOAT(45.0f), 0)
        WAIT_MS(LW(5))
        CALL(evt_npc_set_ry, STRING("me"), 90)
        WAIT_MS(600)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        WAIT_MS(600)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        CALL(evt_npc_move_position, STRING("me"), LW(4), LW(2), 0, FLOAT(45.0f), 0)
        WAIT_MS(LW(6))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mony_regl)
    SET_VALUE(LW(0), 500)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), -40)
    CALL_CHILD(PTR(mony_peton_regl))
    RETURN
EVT_END()

static EVT_BEGIN(mony_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(274)
        CALL(evt_msg_print, 0, STRING("stg5_muj_169"), 0, STRING("me"))
        SET_VALUE(LW(10), 0)
        CALL_CHILD(PTR(lost_wedding_ring))
        SET_VALUE(GSW_STORY_PROGRESS, 275)
        CASE_EQUAL(275)
        CALL_CHILD(PTR(wedding_ring_consign))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_192"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_194"), 0, STRING("me"))
            SET_VALUE(LW(10), 0)
            CALL_CHILD(PTR(find_wedding_ring))
            SET_VALUE(GSW_STORY_PROGRESS, 276)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(peton_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(274, 275)
        CALL(evt_npc_set_position, STRING("me"), 400, 0, -40)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(peton_regl)
    SET_VALUE(LW(0), 400)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), -40)
    CALL_CHILD(PTR(mony_peton_regl))
    RETURN
EVT_END()

static EVT_BEGIN(peton_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(274)
        CALL(evt_msg_print, 0, STRING("stg5_muj_170"), 0, STRING("me"))
        SET_VALUE(LW(10), 1)
        CALL_CHILD(PTR(lost_wedding_ring))
        SET_VALUE(GSW_STORY_PROGRESS, 275)
        CASE_EQUAL(275)
        CALL_CHILD(PTR(wedding_ring_consign))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_193"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_195"), 0, STRING("me"))
            SET_VALUE(LW(10), 1)
            CALL_CHILD(PTR(find_wedding_ring))
            SET_VALUE(GSW_STORY_PROGRESS, 276)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"モニー"*/"\x83\x82\x83\x6A\x81\x5B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        mony_init, // initEvent
        mony_regl, // regularEvent
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
        peton_regl, // regularEvent
        peton_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static HitDamageReturn hitDmgRetPoints[] = {
    {"A_yuka_01", {0}},
    {0},
};

static EVT_BEGIN(evt_switch_init)
    IF_INT_EQUAL(GSWF(1349), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_dakan"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_dokan"), 1, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_treeb_01)
    CALL(evt_item_delete_check, STRING("item_treeb_01"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_treeb_01"), ITEM_KEEL_MANGO, -350, 90, -70, 14, GSWF(3152), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_kusa_2)
    CALL(evt_item_delete_check, STRING("item_kusa_2"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_2"), ITEM_COIN, 323, 0, 50, 4, GSWF(3154), 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_kusa_6)
    CALL(evt_item_delete_check, STRING("item_kusa_6"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_6"), ITEM_STAR_PIECE, -500, 0, 110, 4, GSWF(5641), 0)
    END_IF
    RETURN
EVT_END()

static TreeAccess tree_tbl[] = {
    {5, "A_treeA_01", "treeA_01_m", "treeA_01_ha", 0, NULL},
    {5, "A_treeA_02", "treeA_02_m", "treeA_02_ha", 0, NULL},
#ifdef VERSION_G8ME01
    {5, "A_treeB_01", "treeB_01_m", "treeB_01_ha", 0, NULL},
    {5, "A_treeB_02", "treeB_02_m", "treeB_02_ha", 0, NULL},
    {5, "A_treeB_03", "treeB_03_m", "treeB_03_ha", 0, NULL},
    {5, "A_treeB_04", "treeB_04_m", "treeB_04_ha", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

TreeAccess tree_tbl_treeb_evt[] = {
    {5, "A_treeA_01", "treeA_01_m", "treeA_01_ha", 0, NULL},
    {5, "A_treeA_02", "treeA_02_m", "treeA_02_ha", 0, NULL},
#endif
    {5, "A_treeB_01", "treeB_01_m", "treeB_01_ha", 0, evt_treeb_01},
    {5, "A_treeB_02", "treeB_02_m", "treeB_02_ha", 0, NULL},
    {5, "A_treeB_03", "treeB_03_m", "treeB_03_ha", 0, NULL},
    {5, "A_treeB_04", "treeB_04_m", "treeB_04_ha", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

static KusaAccess kusa_tbl[] = {
    {1, "A_kuwa_move_01", "S_kuwa_move_01", NULL},
    {1, "A_kuwa_move_02", "S_kuwa_move_02", evt_kusa_2},
    {1, "A_kuwa_move_03", "S_kuwa_move_03", NULL},
    {1, "A_kuwa_move_04", "S_kuwa_move_04", NULL},
    {1, "A_kuwa_move_05", "S_kuwa_move_05", NULL},
    {1, "A_kuwa_move_06", "S_kuwa_move_06", evt_kusa_6},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(muj_02_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ4"))
    SET_VALUE(GSWF(3152), 0)
    CALL_CHILD(PTR(evt_switch_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_set_tribe, STRING("ピートン"), STRING("ピートン（指輪）"))
#ifdef VERSION_G8ME01
    CALL(evt_npc_check_delete, STRING("チョロボン２"))
#endif
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 269)
        CALL(evt_npc_check_delete, STRING("チョロボン１"))
#ifndef VERSION_G8ME01
        CALL(evt_npc_check_delete, STRING("チョロボン２"))
#endif
        CALL(evt_npc_check_delete, STRING("チョロボン７"))
    END_IF
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    CALL(evt_hit_damage_return_set, PTR(hitDmgRetPoints))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mobj_badgeblk, STRING("mobj_badge_gat"), -200, 60, -90, 251, 0, GSWF(3146), 1)
    CALL(evt_mobj_badgeblk, STRING("mobj_hatena"), -135, 0, 135, 143, 0, GSWF(3150), 0)
#ifdef VERSION_G8ME01
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 261)
        SET_VALUE(LW(0), PTR(tree_tbl_treeb_evt))
        ELSE
#endif
    SET_VALUE(LW(0), PTR(tree_tbl))
#ifdef VERSION_G8ME01
        SET_VALUE(GSWF(3152), 1)
    END_IF
#endif
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    SET_VALUE(LW(0), PTR(kusa_tbl))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()

// (body lives in countDisp.c to keep the original definition order)
// printCount comes first so the deferred compile emits both functions in the original order
void printCount(CameraId camera, void* param) {
#ifdef VERSION_G8ME01
    char buf[32];
#else
    char buf[36];
#endif
    s32 n;

#ifdef VERSION_G8ME01
    n = evtGetValue(NULL, GW(0));
    FontDrawStart();
    switch ((u32)gp->language) {
        case 0:
            // %3d回
            sprintf(buf, "%3d\x89\xF1", n);
            FontDrawStringPitch(90.0f, 120.0f, 20.0f, buf);
            // かい
            n = evtGetValue(NULL, GW(0));
            sprintf(buf, "\x82\xA9\x82\xA2", n);
            FontDrawScale(0.55f);
            FontDrawStringPitch(148.0f, 131.0f, 20.0f, buf);
            break;
        case 1:
        case 6:
            sprintf(buf, "%3d", n);
            FontDrawStringPitch(68.0f, 120.0f, 16.0f, buf);
            FontDrawString(125.0f, 120.0f, "times");
            break;
        case 2:
            sprintf(buf, "%3d", n);
            FontDrawStringPitch(73.0f, 120.0f, 16.0f, buf);
            FontDrawString(125.0f, 120.0f, "-mal");
            break;
        case 4:
            sprintf(buf, "%3d", n);
            FontDrawStringPitch(68.0f, 120.0f, 16.0f, buf);
            FontDrawString(125.0f, 120.0f, "veces");
            break;
        case 3:
        case 5:
            sprintf(buf, "%3d", n);
            FontDrawString(110.0f, 120.0f, buf);
            break;
    }
#else
    // %3d回
    n = evtGetValue(NULL, GW(0));
    sprintf(buf, "%3d\x89\xF1", n);
    FontDrawStart();
    FontDrawStringPitch(90.0f, 120.0f, 20.0f, buf);
    // かい
    n = evtGetValue(NULL, GW(0));
    sprintf(buf, "\x82\xA9\x82\xA2", n);
    FontDrawScale(0.55f);
    FontDrawStringPitch(148.0f, 131.0f, 20.0f, buf);
#endif
}

USER_FUNC(countDisp) {
    dispEntry(CAMERA_2D, 399.0f, 0, printCount, NULL);
    return EVT_RETURN_BLOCK;
}

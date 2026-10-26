#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"

#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "mario/mariost.h"
#include "system.h"
#include "evt/evt_bero.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"

static USER_FUNC(evt_chuchu_back_loop);

static EvtWord door_close[12];
static EvtWord door_open[12];
static BeroEntry bero_entry_data[2];
static EvtWord chuchurina_sogu[610];
EvtWord chuchurina_talk[2];
static NpcSetupInfo npcEnt[2];
static EvtWord item_get00[37];
EvtWord item_get01[34];
EvtWord item_get02[34];
EvtWord gon_12_init_evt[93];

static EVT_BEGIN(door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("door03"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
        1, // type
        12, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gon_08", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        door_close, // enterAnimeArg
        door_open, // exitAnimeArg
    },
    {0},
};

static USER_FUNC(evt_chuchu_back_loop) {
    NpcEntry* npc;
    EvtWord* args = event->args;
    OSTime now;
    s32 total;
    s32 elapsed;

    // チュチュリーナ
    npc = evtNpcNameToPtr(event, "\x83\x60\x83\x85\x83\x60\x83\x85\x83\x8A\x81\x5B\x83\x69");
    total = evtGetValue(event, args[0]);
    if (gp->inBattle) {
        now = gp->renderTime;
    } else {
        now = gp->renderFieldTime;
    }
    if (isFirstCall) {
        event->userdata[0] = ((u64)now >> 32) & 0xFFFFFFFF;
        event->userdata[1] = (u64)now & 0xFFFFFFFF;
    }
    elapsed = ((s32)now - event->userdata[1]) / (u64)(OS_TIMER_CLOCK / 1000);
    if (elapsed < total) {
        npc->rotation.z = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 360.0f, elapsed, total);
        return EVT_RETURN_BLOCK;
    }
    npc->rotation.z = 0.0f;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(chuchurina_sogu)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_15_MS_MOWZ_W_GOOMBELLA)
        CALL(evt_npc_set_position, STRING("チュチュリーナ"), 0, -1000, 0)
        RETURN
    END_IF
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_set_pos, 200, 0, 0)
    CALL(evt_party_set_pos, 0, 230, 0, 15)
    CALL(evt_bero_mario_go_init)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_play_enter_anime))
    CALL(evt_bero_mario_go_wait)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, 160, 15, FLOAT(150.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 130, 0, FLOAT(150.0f))
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_party_stop, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_15_MS_MOWZ_W_GOOMBELLA)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_heya"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 0, STRING("チュチュリーナ"), NPC_FLAG_AUTO_MOVE_ANIM)
    WAIT_MS(750)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, -78, 152, 528, -78, 57, -13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_npc_reverse_ry, STRING("チュチュリーナ"))
    WAIT_MS(500)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_CYUCYURINA1"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_18_01"), 0, STRING("チュチュリーナ"))
    CALL(evt_cam3d_evt_set, 141, 91, 263, 141, 45, -2, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg1_gon_18_02"))
    CALL(evt_cam3d_evt_set, -110, 75, 225, -110, 35, 0, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg1_gon_18_04"), 0, STRING("チュチュリーナ"))
    CALL(evt_cam3d_evt_set, -3, 88, 170, 134, 35, -7, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg1_gon_18_05"))
    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 58, 13, 151, -138, 38, -31, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg1_gon_18_07"), 0, STRING("チュチュリーナ"))
    CALL(evt_cam3d_evt_set, 141, 91, 263, 141, 45, -2, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg1_gon_18_08"))
    CALL(evt_cam3d_evt_set, -110, 75, 225, -110, 35, 0, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg1_gon_18_10"), 0, STRING("チュチュリーナ"))
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_W_1"))
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), 0, 0, 0, FLOAT(60.0f), 0)
    CALL(evt_cam3d_evt_set, 115, 91, 263, 115, 45, -2, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_position, STRING("チュチュリーナ"), 0, 0, 0)
    WAIT_MS(200)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -25)
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
    CALL(evt_msg_print_party, STRING("stg1_gon_18_11"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -15)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_W_1"))
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_eff, 0, STRING("kiss"), 0, LW(0), LW(1), LW(2), -45, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_CHURINA_KISS1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_CHURINA_KISS2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_K_1"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_O"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_R_1"))
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), 85, 0, 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
    CALL(evt_msg_print, 0, STRING("stg1_gon_18_13"), 0, STRING("チュチュリーナ"))
    CALL(evt_mario_set_pose, STRING("M_Z_1"))
    CALL(evt_cam3d_evt_set, 217, 14, 228, 101, 52, -35, 400, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_rotate_offset, STRING("チュチュリーナ"), 0, 15, 0)
        CALL(evt_chuchu_back_loop, 450)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_CHURINA_JUMP1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), 0, 67, -125, 500, 30)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg1_gon_18_14"))
    CALL(evt_cam3d_evt_set, 91, 66, 99, 43, 82, -10, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg1_gon_18_16"), 0, STRING("チュチュリーナ"))
    CALL(evt_cam3d_evt_set, 179, 36, 299, 43, 82, -10, 300, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_npc_set_rotate_offset, STRING("チュチュリーナ"), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING("チュチュリーナ"), 0, 0, 355)
    WAIT_MS(50)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_W_1"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_CHURINA_ESCAPE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), 35, 0, -145, 300, 20)
    WAIT_MS(700)
    CALL(evt_cam3d_evt_set, 141, 91, 263, 141, 45, -2, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg1_gon_18_17"))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_msg_continue)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    SPAWN_EVENT(PTR(bero_case_entry))
    CALL(evt_cam3d_evt_off, 500, EASING_QUADRATIC_OUT)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        SET_VALUE(GSWF(1511), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(chuchurina_talk)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"チュチュリーナ"*/"\x83\x60\x83\x85\x83\x60\x83\x85\x83\x8A\x81\x5B\x83\x69", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        chuchurina_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(item_get00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_CASTLE_KEY_0x0F, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG1_15_01_KEY_3)
    RETURN
EVT_END()

EVT_BEGIN(item_get01)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box01"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item01"), ITEM_MUSHROOM, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item01"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box01"))
    CALL(evt_item_get_item, STRING("item01"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(item_get02)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box02"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item02"), ITEM_HONEY_SYRUP, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item02"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box02"))
    CALL(evt_item_get_item, STRING("item02"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(gon_12_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_15_MS_MOWZ_W_GOOMBELLA)
        CALL_CHILD(PTR(evt_bero_info_run))
    END_IF
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG1_GON1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG1_GON1"))
    CALL(evt_snd_set_rev_mode, 2)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_change_fbat_mode, 1)
    SPAWN_EVENT(PTR(chuchurina_sogu))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_15_01_KEY_3)
        SET_VALUE(GSWF(1507), 1)
    END_IF
    CALL(evt_mobj_itembox, STRING("mobj_box00"), -117, 0, -75, MOBJ_ITEMBOX_NORMAL, 0, PTR(item_get00), GSWF(1507))
    CALL(evt_mobj_itembox, STRING("mobj_box01"), -56, 0, -25, MOBJ_ITEMBOX_NORMAL, 0, PTR(item_get01), GSWF(1508))
    CALL(evt_mobj_itembox, STRING("mobj_box02"), -175, 0, 50, MOBJ_ITEMBOX_NORMAL, 0, PTR(item_get02), GSWF(1509))
    CALL(evt_mobj_itembox, STRING("mobj_box03"), -150, 0, 20, MOBJ_ITEMBOX_NORMAL, 0, 0, 1)
    CALL(evt_mobj_powerupblk, STRING("pblk00"), -150, 60, -25, 0, GSWF_SHINE_15, 0)
    CALL(evt_map_playanim, STRING("hokori"), MAP_ANIM_PLAY_LOOP, 0)
    RETURN
EVT_END()


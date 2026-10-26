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
#include "mri_puni.h"
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
#include "evt/evt_sub.h"
#include "npc_event.h"
#include "mri.h"
#include "data_tables.h"
#include "manager/evtmgr_cmd.h"
#include "driver/seqdrv.h"

USER_FUNC(bero_get_position_normal);
static USER_FUNC(evt_chuchu_back_loop);

static EvtWord bero_custom[13];
static BeroEntry bero_entry_data[2];
static EvtWord chibitoge_init[21];
static EvtWord chibitoge_talk[111];
static NpcSetupInfo npcEnt[2];
static EvtWord chuchurina_sogu[814];
EvtWord black_shadow[341];
static EvtWord evt_ueki_02[20];
static KusaAccess ueki_access_data[5];
EvtWord item_mri_04[34];
EvtWord mri_04_init_evt[178];

static EVT_BEGIN(bero_custom)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_12_02_PUNI_ELDER_IMPRISONED)
        SET_VALUE(LW(0), 1)
        RETURN
    END_IF
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        bero_custom, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "mri_03", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(chibitoge_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG2_26_PRAISE_FROM_THE_PUNI)
        IF_INT_EQUAL(GSWF(2884), 1)
            IF_INT_EQUAL(GSWF(2840), 0)
                CALL(evt_npc_set_position, STRING("me"), -120, 0, -15)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(chibitoge_talk)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_jump_position_nohit, STRING("me"), -60, 67, -40, 500, 80)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("stg2_mri_re_019"), 0, 0)
    WAIT_WHILE(GF(0))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 25)
    CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 500, 30)
    WAIT_MS(1000)
    CALL(evt_item_entry, STRING("chibitoge_item"), ITEM_MUSHROOM, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_get_item, STRING("chibitoge_item"))
    CALL(evt_npc_set_ry, STRING("me"), 90)
    WAIT_MS(200)
    CALL(evt_npc_move_position, STRING("me"), -60, 40, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_move_position, STRING("me"), 75, 40, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    SET_VALUE(GSWF(2840), 1)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ちびトゲ"*/"\x82\xBF\x82\xD1\x83\x67\x83\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        chibitoge_init, // initEvent
        NULL, // regularEvent
        chibitoge_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static USER_FUNC(evt_chuchu_back_loop) {
    EvtWord* args = event->args;
    // チュチュリーナ
    NpcEntry* npc = evtNpcNameToPtr(event, "\x83\x60\x83\x85\x83\x60\x83\x85\x83\x8A\x81\x5B\x83\x69");
    s32 total = evtGetValue(event, args[0]);
    s32 elapsed;

    if (isFirstCall) {
        event->userdata[0] = gp->renderTime;
    }
    elapsed = OSTicksToMilliseconds((u32)gp->renderTime - event->userdata[0]);
    if (elapsed < total) {
        npc->rotation.z = intplGetValue(EASING_SINE_IN_OUT, 0.0f, 360.0f, elapsed, total);
        return EVT_RETURN_BLOCK;
    }
    npc->rotation.z = 0.0f;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(chuchurina_sogu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_onoff, 0, 2)
    CALL(evt_mario_normalize)
    CALL(evt_npc_set_position, STRING("チュチュリーナ"), -75, 0, 30)
    CALL(evt_npc_set_ry, STRING("チュチュリーナ"), 90)
    CALL(evt_npc_set_position, STRING("第三勢力部下"), 0, 0, 40)
    CALL(evt_npc_set_ry, STRING("第三勢力部下"), 270)
    CALL(evt_npc_get_position, STRING("第三勢力部下"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 25)
    ADD(LW(1), 17)
    CALL(evt_snd_sfxon_3d, STRING("SFX_GURUGURU_MARK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 2, STRING("eff_moja"), 0, 2, 0, LW(0), LW(1), LW(2), 0, 45, -1)
    CALL(evt_bero_get_into_info)
    CALL(bero_get_position_normal, 0, FLOAT(60.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_bero_get_start_position, LW(0), LW(1), LW(2))
        ADD(LW(0), 40)
        CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
        CALL(evt_mario_set_dir, 270, 0, 0)
        CALL(evt_bero_get_end_position, LW(0), LW(1), LW(2))
        CALL(evt_mario_mov_pos2, 110, LW(2), FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_bero_get_start_position, LW(0), LW(1), LW(2))
        ADD(LW(0), 70)
        ADD(LW(2), -20)
        CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_bero_get_end_position, LW(0), LW(1), LW(2))
        ADD(LW(2), -20)
        CALL(evt_party_move_pos2, 0, 145, LW(2), FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_puni_wait_systemmode, 2)
        CALL(evt_puni_set_mode, 1, 30)
        CALL(evt_puni_set_position, 1, 0, -1000, 0)
        WAIT_MS(400)
        CALL(evt_bero_get_start_position, LW(0), LW(1), LW(2))
        ADD(LW(0), 70)
        CALL(evt_puni_set_position, 1, LW(0), LW(1), LW(2))
        CALL(evt_puni_set_dir, 1, 270)
        CALL(evt_puni_move_position2, 1, 175, LW(2), FLOAT(120.0f))
        CALL(evt_puni_set_anim, 1, 0)
        CALL(evt_bero_exec_onoff, 1, 2)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("第三勢力部下"), STRING("D_2"))
    CALL(evt_bero_exec_wait, 65536)
    WAIT_MS(1500)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_CYUCYURINA1"))
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 0, STRING("チュチュリーナ"), NPC_FLAG_AUTO_MOVE_ANIM)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_eff_fukidashi, 4, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    WAIT_MS(800)
    CALL(evt_cam3d_evt_set, -84, 152, 548, -84, 57, 7, 1000, 11)
    WAIT_MS(1200)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e13_00"), 0, STRING("チュチュリーナ"))
    CALL(evt_cam3d_evt_set, 125, 90, 262, 125, 40, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg2_mri_e13_01"))
    CALL(evt_cam3d_evt_set, -75, 83, 261, -75, 33, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e13_02"), 0, STRING("チュチュリーナ"))
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_W_1"))
    CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), 0, LW(2), 0, FLOAT(120.0f), 0)
    CALL(evt_cam3d_evt_set, 100, 90, 262, 100, 40, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_position, STRING("チュチュリーナ"), 0, 0, 0)
    WAIT_MS(200)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), LW(0), LW(2), 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_K_1"))
    WAIT_MS(150)
    ADD(LW(0), 10)
    ADD(LW(1), 20)
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kiss"), 0, LW(0), LW(1), LW(2), -45, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_CHURINA_KISS1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_CHURINA_KISS2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_R_1"))
    CALL(evt_npc_move_position, STRING("チュチュリーナ"), 65, 0, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e13_03"), 0, STRING("チュチュリーナ"))
    CALL(evt_cam3d_evt_set, FLOAT(137.39941f), FLOAT(25.299805f), FLOAT(635.6992f), FLOAT(36.69922f), FLOAT(131.19922f), FLOAT(-83.69922f), 400, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_rotate_offset, STRING("チュチュリーナ"), 0, 15, 0)
        CALL(evt_chuchu_back_loop, 450)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), -60, 67, -53, 500, 30)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_rotate_offset, STRING("チュチュリーナ"), 0, 15, 0)
        CALL(evt_chuchu_back_loop, 450)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), -53, 135, -97, 500, 30)
    WAIT_MS(300)
    CALL(evt_party_set_dir, 0, 270, -1)
    CALL(evt_msg_print_party, STRING("stg2_mri_e13_04"))
    CALL(evt_cam3d_evt_set, 67, 125, 180, -6, 150, 11, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e13_05"), 0, STRING("チュチュリーナ"))
    CALL(evt_cam3d_evt_set, FLOAT(123.19922f), FLOAT(105.69922f), FLOAT(310.0996f), FLOAT(-6.0f), FLOAT(150.0f), FLOAT(11.0f), 300, 11)
    WAIT_MS(200)
    CALL(evt_npc_set_rotate_offset, STRING("チュチュリーナ"), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING("チュチュリーナ"), 0, 0, 355)
    WAIT_MS(50)
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_W_1"))
    CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHURINA_ESCAPE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), -18, 135, -200, 300, 10)
    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_NO_DRAW)
    WAIT_MS(700)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI5"))
    CALL(evt_cam3d_evt_set, 125, 90, 262, 125, 40, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg2_mri_e13_06"))
    CALL(evt_mario_set_dir, 270, 100, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -10, 114, 400, -10, 45, 7, 300, 11)
    WAIT_MS(2000)
    CALL(evt_eff_softdelete, STRING("eff_moja"))
    CALL(evt_npc_get_position, STRING("第三勢力部下"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_3RD_WAKE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("第三勢力部下"), STRING("D_3"))
    WAIT_MS(700)
    CALL(evt_npc_set_ry, STRING("第三勢力部下"), 90)
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("第三勢力部下"), 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_npc_flag_onoff, 1, STRING("第三勢力部下"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 1, STRING("第三勢力部下"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e13_07"), 0, STRING("第三勢力部下"))
    CALL(evt_npc_set_anim, STRING("第三勢力部下"), STRING("R_1"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -35)
    CALL(evt_cam3d_evt_set, FLOAT(90.899414f), FLOAT(114.0f), FLOAT(400.0f), FLOAT(90.899414f), FLOAT(45.0f), FLOAT(7.0f), 500, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("第三勢力部下"), LW(0), LW(2), 0, FLOAT(180.0f), 0)
    END_INLINE_EVENT
    WAIT_MS(400)
    CALL(evt_npc_set_battle_info, STRING("第三勢力部下"), 5)
    CALL(evt_npc_battle_start, STRING("第三勢力部下"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_cam3d_evt_off, 0, 4)
    SET_VALUE(LW(10), STRING("第三勢力部下"))
    SPAWN_EVENT(PTR(enemy_event_dead_event_sub))
    WAIT_MS(500)
    CALL(evt_item_entry, STRING("item_key"), ITEM_RED_KEY, 60, 0, 0, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF(2839), 0)
    CALL(evt_puni_set_mode, 1, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_13_MS_MOWZ)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(black_shadow)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    SET_VALUE(LW(8), STRING("チュチュリーナ"))
    CALL(evt_npc_set_ry, LW(8), 90)
    CALL(evt_npc_set_position, LW(8), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, LW(8), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_color, LW(8), 20, 20, 20, 255)
    SET_VALUE(LW(9), STRING("第三勢力部下"))
    CALL(evt_npc_set_ry, LW(9), 270)
    CALL(evt_npc_set_position, LW(9), 0, 0, 40)
    CALL(evt_npc_flag_onoff, 1, LW(9), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_puni_wait_systemmode, 2)
    SET_VALUE(LW(10), 1)
    LOOP(0)
        CALL(evt_puni_get_flag, LW(10), LW(0))
        AND(LW(0), 1024)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_puni_init_puni, LW(10), 30, 0, -1000, 0, 0)
        END_IF
        ADD(LW(10), 1)
        IF_INT_EQUAL(LW(10), 103)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam3d_evt_set, 10, 150, 550, 0, 55, 7, 0, 11)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 115, 400, 0, 45, 7, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, LW(9), 90)
    WAIT_MS(700)
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_00_01"), 0, LW(9))
    CALL(evt_npc_set_position, LW(8), -55, 135, -100)
    CALL(evt_npc_get_position, LW(8), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHURINA_JUMP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_jump_position_nohit, LW(8), -60, 67, -53, 500, 30)
    CALL(evt_npc_get_position, LW(8), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHURINA_LANDING1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(50)
        CALL(evt_npc_get_position, LW(8), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHURINA_JUMP1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, LW(8), -25, 0, 23, 500, 30)
    CALL(evt_npc_get_position, LW(8), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHURINA_LANDING1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_ry, LW(9), 270)
    WAIT_MS(200)
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(9), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(500)
    CALL(evt_npc_get_position, LW(8), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_CHURINA_ATTACK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, LW(8), STRING("PCH_A1_2"))
    CALL(evt_npc_get_position, LW(9), LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(evt_eff, STRING(""), STRING("small_star"), 3, LW(0), LW(1), LW(2), 0, -1, 0, 3, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 50)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(mri_npc_buruburu, LW(9), 0, FLOAT(1.0f), 100, 300)
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 0, LW(9), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, LW(9), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG2_3RD_DOWN1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_npc_set_anim, LW(9), STRING("D_4"))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_npc_set_anim, LW(8), STRING("PCH_S_1"))
    CALL(evt_msg_print, 0, STRING("stg2_mri_e12_26_00_02"), 0, LW(9))
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG2_12_01_A_DARK_FIGURE)
    CALL(evt_bero_mapchange, STRING("mri_03"), STRING("dokan"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_ueki_02)
    CALL(evt_item_delete_check, STRING("item_ueki_02"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_ueki_02"), ITEM_COIN, 85, 0, 85, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF(2838), 0)
    END_IF
    RETURN
EVT_END()

static KusaAccess ueki_access_data[] = {
    {1, "A_ueki01", "S_ueki01", NULL},
    {1, "A_ueki02", "S_ueki02", evt_ueki_02},
    {1, "A_ueki04", "S_ueki04", NULL},
    {1, "A_ueki05", "S_ueki05", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(item_mri_04)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_ULTRA_SHROOM, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(mri_04_init_evt)
    CALL(evt_puni_mem_init)
    CALL(evt_puni_set_yuka, 4, 0, -310, -1000, -1000, 0, 0)
    CALL(evt_puni_set_hamari, 0, 0, 0, 2, -2, -1000, -1000)
    SET_VALUE(LW(6), 0)
    SET_VALUE(LW(7), 0)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 0)
    CALL_CHILD(PTR(mri_puni_init))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    SPAWN_EVENT(PTR(puni_disp_info))
    SPAWN_EVENT(PTR(mri_countdown))
    SET_VALUE(LW(0), PTR(ueki_access_data))
    CALL_CHILD(PTR(evt_sub_kusa_access_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG2_12_RIVAL_ARRIVAL)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CASE_BETWEEN(STG2_23_TIME_BOMB_ACTIVATED, STG2_23_01_X_NAUT_BATTLE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI9"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG2_MRI1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG2_MRI5"))
    END_SWITCH
    CALL(evt_mobj_itembox, STRING("mobj_box00"), -155, 0, 35, MOBJ_ITEMBOX_NORMAL, 0, PTR(item_mri_04), GSWF(2837))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_12_RIVAL_ARRIVAL)
        CALL(evt_mobj_itembox, STRING("mobj_box01"), -105, 0, 70, MOBJ_ITEMBOX_NORMAL, 0, 0, 0)
        ELSE
        CALL(evt_mobj_itembox, STRING("mobj_box01"), -105, 0, 70, MOBJ_ITEMBOX_NORMAL, 0, 0, 1)
    END_IF
    CALL(evt_map_playanim, STRING("mizu_01"), 1, 0)
    CALL(evt_map_playanim, STRING("mizu_02"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_01"), 1, 0)
    CALL(evt_map_playanim, STRING("awa_02"), 1, 0)
    CALL(evt_map_playanim, STRING("hikari_01"), 1, 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_12_RIVAL_ARRIVAL)
        SPAWN_EVENT(PTR(black_shadow))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_12_02_PUNI_ELDER_IMPRISONED)
        SPAWN_EVENT(PTR(chuchurina_sogu))
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG2_13_MS_MOWZ)
        CALL(evt_item_entry, STRING("item_key"), ITEM_RED_KEY, 60, 0, 0, ITEM_TYPE_FALL_NO_PICKUP, GSWF(2839), 0)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_CLIP)
    RETURN
EVT_END()


#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "pmario_sound.h"

#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "aji.h"
#include "evt/evt_npc.h"
#include "driver/seqdrv.h"

USER_FUNC(waku);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static BeroEntry bero_entry_data[2];
EvtWord evt_robot_waku[34];
static EvtWord evt_boss[797];
EvtWord evt_boss2[147];
static NpcSetupInfo npcEnt[3];
EvtWord aji_14_init_evt[71];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.25f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_1"), 0, 0, LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 0, STRING("S_door1_2"), 0, 0, LW(0))
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        10, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "aji_13", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

USER_FUNC(waku) {
    NpcEntry* npc;
    AnimationPose* pose;
    s32 i;

    // ロボット
    npc = npcNameToPtr_NoAssert("\x83\x8D\x83\x7B\x83\x62\x83\x67");
    if (npc != NULL) {
        pose = animPoseGetAnimPosePtr(npc->poseId);
        animPoseWorldPositionEvalOn(npc->poseId);
        if (pose->worldPositions != NULL) {
            Vec* positions = *pose->worldPositions;
            s32 idx = animPoseGetGroupIdx(npc->poseId, "locator17");

            if (npc->position.y >= 0.0f) {
                // 腕
                npcNameToPtr("\x98\x67")->position = positions[idx];
            } else {
                Vec hidden = {0.0f, -1000.0f, 0.0f};
                // 腕
                npcNameToPtr("\x98\x67")->position = hidden;
            }
            positions = *pose->worldPositions;
            for (i = 0; i < pose->numWorldPositions; i++) {
                positions[i] = (Vec){0.0f, -1000.0f, 0.0f};
            }
        }
    }

    // ロボット2
    npc = npcNameToPtr_NoAssert("\x83\x8D\x83\x7B\x83\x62\x83\x67\x82\x51");
    if (npc != NULL) {
        pose = animPoseGetAnimPosePtr(npc->poseId);
        animPoseWorldPositionEvalOn(npc->poseId);
        if (pose->worldPositions != NULL) {
            Vec* positions = *pose->worldPositions;
            s32 idx = animPoseGetGroupIdx(npc->poseId, "locator17");

            if (npc->position.y >= 0.0f) {
                // 腕2
                npcNameToPtr("\x98\x67\x82\x51")->position = positions[idx];
            } else {
                Vec hidden = {0.0f, -1000.0f, 0.0f};
                // 腕2
                npcNameToPtr("\x98\x67\x82\x51")->position = hidden;
            }
            positions = *pose->worldPositions;
            for (i = 0; i < pose->numWorldPositions; i++) {
                positions[i] = (Vec){0.0f, -1000.0f, 0.0f};
            }
        }
    }
    return EVT_RETURN_BLOCK;
}

EVT_BEGIN(evt_robot_waku)
    CALL(evt_npc_entry, STRING("枠"), STRING("c_mb_robo_2a"))
    CALL(evt_npc_flag_onoff, 1, STRING("枠"), NPC_FLAG_NO_SHADOW | NPC_FLAG_SHADOW_NO_FLOOR)
    CALL(evt_npc_set_anim, STRING("枠"), STRING("L_1"))
    CALL(evt_npc_entry, STRING("枠２"), STRING("c_mb_robo_2a"))
    CALL(evt_npc_flag_onoff, 1, STRING("枠２"), NPC_FLAG_NO_SHADOW | NPC_FLAG_SHADOW_NO_FLOOR)
    CALL(evt_npc_set_anim, STRING("枠２"), STRING("L_1"))
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(waku)
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(evt_boss)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("幹部"), 150, 60, 0)
    CALL(evt_npc_set_position, STRING("ロボット"), 260, 60, 0)
    CALL(evt_npc_set_ry, STRING("幹部"), 270)
    CALL(evt_npc_set_ry, STRING("ロボット"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ロボット"), NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_flag_onoff, 0, STRING("ロボット"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_pera_onoff, STRING("ロボット"), 0)
    CALL(evt_npc_set_anim, STRING("ロボット"), STRING("Z_1"))
    CALL(evt_cam3d_evt_set, 375, 420, 935, -225, -30, 0, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, 375, 500, 935, -225, 50, 0, 4000, 11)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_mov_pos2, -420, 0, FLOAT(80.0f))
    WAIT_MS(1000)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_MAGNUM_APP1"))
    CALL(evt_msg_print, 0, STRING("stg7_aji_36"), 0, STRING("幹部"))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1300)
    CALL(evt_cam3d_evt_set, 10, 190, 275, 175, 60, -142, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 1, STRING("幹部"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ロボット"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg7_aji_37"), 0, STRING("幹部"))
    WAIT_MS(300)
    CALL(evt_party_stop, 0)
    CALL(evt_party_set_pos, 0, -460, 0, 15)
    CALL(evt_cam3d_evt_set, -435, 40, 287, -435, 40, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg7_aji_37_01"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -211, 169, 250, 275, 80, -90, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg7_aji_37_02"), 0, STRING("幹部"))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -435, 40, 287, -435, 40, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print_party, STRING("stg7_aji_38"))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 140, 100, 295, 140, 110, -105, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg7_aji_39"), 0, STRING("幹部"))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("幹部"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("ロボット"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 250, 124, 545, 250, 140, -105, 500, 11)
    WAIT_MS(500)
    CALL(evt_set_dir_to_target, STRING("幹部"), STRING("ロボット"))
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("幹部"), 260, -50, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_get_position, STRING("ロボット"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("幹部"), 270)
    WAIT_MS(200)
    ADD(LW(1), 50)
    SUBTRACT(LW(2), 5)
    CALL(evt_npc_jump_position_nohit, STRING("幹部"), LW(0), LW(1), LW(2), 300, FLOAT(50.0f))
    CALL(evt_npc_set_position, STRING("幹部"), 0, -1000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(966)
        CALL(evt_npc_get_position, STRING("ロボット"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_BOSS_ACTIVE1"), LW(0), LW(1), LW(2), LW(9))
        WAIT_MS(2533)
        CALL(evt_snd_sfxoff, LW(9))
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ロボット"), STRING("R_1"))
    CALL(evt_npc_wait_anim, STRING("ロボット"), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING("ロボット"), STRING("S_1"))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(20)
            ADD_FLOAT(LW(0), FLOAT(-2.25f))
            CALL(evt_npc_set_rotate, STRING("ロボット"), 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 165, 183, 285, 281, 193, -98, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 1, STRING("ロボット"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_msg_print, 0, STRING("stg7_aji_41"), 0, STRING("ロボット"))
    CALL(evt_npc_flag_onoff, 0, STRING("ロボット"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_cam3d_evt_set, 250, 124, 545, 250, 140, -105, 300, 11)
    WAIT_MS(300)
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("ロボット"), STRING("W_1"))
        CALL(evt_npc_move_position, STRING("ロボット"), 130, 0, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_set_anim, STRING("ロボット"), STRING("S_1"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_get_position, STRING("ロボット"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_BOSS_MOVE1L"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -475, 12, 300, 140, 90, -123, 0, 11)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -100, 0, FLOAT(100.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -140, 15, FLOAT(100.0f))
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ロボット"), STRING("J_1"))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_npc_set_anim, STRING("ロボット"), STRING("J_2"))
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("ロボット"), 0, 0, 0, 500, FLOAT(80.0f))
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 50)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ロボット"), STRING("J_3"))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("ロボット"), STRING("S_1"))
    WAIT_MS(500)
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, -50, 475, 1104, -50, 35, -13, 1000, 11)
    WAIT_MS(300)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(evt_npc_set_battle_info, STRING("ロボット"), 5)
    CALL(evt_npc_battle_start, STRING("ロボット"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, -100, 0, 0)
    CALL(evt_party_set_pos, 0, -140, 0, 15)
    CALL(evt_npc_set_position, STRING("ロボット"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ロボット２"), 0, 0, 0)
    CALL(evt_cam3d_evt_set, 0, 200, 675, 0, 75, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_get_position, STRING("ロボット２"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_3RD_EXPLSION1"), LW(0), LW(1), LW(2), 0)
    ADD(LW(1), 80)
    CALL(evt_eff64, STRING("eff1"), STRING("akari_charge_n64"), 1, LW(0), LW(1), LW(2), FLOAT(3.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_3RD_EXPLSION2"), LW(0), LW(1), LW(2), LW(10))
    CALL(evt_eff64, STRING("eff2"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_eff_softdelete, STRING("eff1"))
    ADD(LW(2), 100)
    CALL(evt_eff, 0, STRING("kemuri_test"), 8, LW(0), LW(1), LW(2), FLOAT(1.75f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_random, 300, LW(0))
            CALL(evt_sub_random, 200, LW(1))
            CALL(evt_sub_random, 200, LW(2))
            SUBTRACT(LW(0), 150)
            CALL(evt_npc_get_position, STRING("ロボット２"), LW(10), LW(11), LW(12))
            ADD(LW(0), LW(10))
            ADD(LW(1), LW(11))
            ADD(LW(2), LW(12))
            CALL(evt_eff, 0, STRING("kemuri_test"), 8, LW(0), LW(1), LW(2), FLOAT(1.75f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("ロボット２"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_3RD_EXPLSION3"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(1000)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(3000)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, STRING("幹部"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_set_anim, STRING("幹部"), STRING("D_1"))
        CALL(evt_npc_set_position, STRING("幹部"), 0, 70, 0)
        CALL(evt_npc_jump_position_nohit, STRING("幹部"), -200, 200, 100, 1000, FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg7_aji_42"), 0, STRING("ロボット"))
    CALL(evt_fade_set_mapchange_type, 1, 17, 900, 16, 900)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    CALL(evt_bero_mapchange, STRING("moo_04"), STRING("evt_aji_14"))
    RETURN
EVT_END()

EVT_BEGIN(evt_boss2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, -100, 0, 0)
    CALL(evt_party_set_pos, 0, -140, 0, 15)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_item_entry, STRING("dummy"), ITEM_CRYSTAL_STAR, 0, 1000, 0, 17, -1, 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 1000, 25, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_item_set_position, STRING("dummy"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        SET_VALUE(LW(10), 0)
        CALL(evt_item_get_position, STRING("dummy"), LW(0), LW(1), LW(2))
        LOOP(0)
            CALL(evt_sub_get_sincos, LW(10), LW(11), LW(12))
            MULTIPLY_FLOAT(LW(11), FLOAT(10.0f))
            SET_FLOAT_VALUE(LW(12), LW(1))
            ADD_FLOAT(LW(12), LW(11))
            ADD(LW(10), 5)
            MODULO(LW(10), 360)
            CALL(evt_item_set_position, STRING("dummy"), LW(0), LW(12), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("stg7_aji_42_01"))
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(200)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -40, 15, FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 0, 0, FLOAT(80.0f))
    SET_VALUE(GSW_STORY_PROGRESS, 373)
    DELETE_EVENT(LW(10))
    SET_VALUE(LW(0), 7)
    SET_VALUE(LW(1), STRING("dummy"))
    CALL_CHILD(PTR(evt_sub_starstone))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"幹部"*/"\x8A\xB2\x95\x94", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ロボット"*/"\x83\x8D\x83\x7B\x83\x62\x83\x67", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(aji_14_init_evt)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_NOT_EQUAL(LW(0), STRING("tuzuki"))
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG7_DUN2"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_AJI2"))
        ELSE
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envoff, PSND_FADEOUT)
    END_IF
    CALL(evt_snd_set_rev_mode, 1)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING("tuzuki"))
        SPAWN_EVENT(PTR(evt_boss2))
        RETURN
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 373)
        CALL(evt_npc_entry, STRING("ロボット２"), STRING("c_mb_robo_2b"))
        CALL(evt_npc_set_anim, STRING("ロボット２"), STRING("MGN_Q_1"))
        CALL(evt_npc_set_position, STRING("ロボット２"), 0, -1000, 0)
        CALL_CHILD(PTR(evt_robot_waku))
        SPAWN_EVENT(PTR(evt_boss))
    END_IF
    SPAWN_EVENT(PTR(evt_emergency))
    RETURN
EVT_END()


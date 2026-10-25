#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "evt/evt_pouch.h"
#include "driver/npcdrv.h"
#include "driver/mapdrv.h"

#include "string.h"
#include "memory.h"
#include "evt/evt_bero.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "muj.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "manager/evtmgr_cmd.h"

s32* command_work;

USER_FUNC(evt_dokuroiwa_command_check);
USER_FUNC(evt_dokuroiwa_command_clear);
USER_FUNC(evt_dokuroiwa_command_init);
USER_FUNC(evt_dokuroiwa_command_set);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_snd_bgm_freq);
USER_FUNC(evt_win_item_select);

EvtWord sanders_rotate[28];
EvtWord sanders_nakama_sub[13];
EvtWord sanders_nakama[173];
static EvtWord sanders_init[51];
static s32 itemsel_table01[2];
EvtWord sanders_talk_sub[70];
static EvtWord sanders_talk[178];
static EvtWord marco_init[24];
static EvtWord marco_talk[146];
static EvtWord erumosu_regl[4];
EvtWord erumosu_a_dead[23];
EvtWord erumosu_b_dead[12];
static NpcSetupInfo npcEnt[5];
NpcSetupInfo npcEnt_after[3];
static BeroEntry bero_entry_data[3];
EvtWord hammer_sub[27];
EvtWord hammer_event[10];
EvtWord sanders_kyushutsu_sub[250];
EvtWord erumosu_guruguru[13];
EvtWord erumosu_guruguru_dir[11];
EvtWord sanders_kyushutsu[63];
EvtWord marco_hanauta[2];
EvtWord marco_and_jewel[84];
EvtWord blueeye_flush[78];
EvtWord redeye_flush[78];
EvtWord dokuroiwa_flush[51];
s32 command_seikai[7];
static s32 itemsel_table[2];
EvtWord dokuroiwa_touch[127];
EvtWord case_event_entry[45];
EvtWord case_event_delete[42];
EvtWord penalty_event[187];
static EvtWord cam_shake[33];
static EvtWord cam_shake2[27];
EvtWord answer_event[125];
EvtWord iwa01_hammer[73];
EvtWord iwa01_ride_a[20];
EvtWord iwa02_ride[66];
EvtWord iwa02_hammer[20];
EvtWord iwa02_ride_a[28];
EvtWord iwa02_nonride_a[5];
static EvtWord bombwall_bomb[143];
EvtWord dokuroiwa_bomb[309];
EvtWord dokuroiwa_init[123];
EvtWord muj_05_first[26];
EvtWord yure_yashi_ha1[180];
EvtWord yure_yashi[88];
static EvtWord evt_kusa_1[20];
static TreeAccess tree_tbl[6];
static KusaAccess kusa_tbl[8];
EvtWord muj_05_init_evt[146];

USER_FUNC(evt_dokuroiwa_command_check) {
    s32 result;
    s32 i;
    EvtWord* args = event->args;

    for (i = 0; i < 7; i++) {
    }
    result = 2;
    for (i = 0; i < 7; i++) {
        s32 cmd = command_work[i];
        if (cmd == 0) {
            result = 0;
            break;
        }
        if (cmd != command_seikai[i]) {
            result = 1;
            break;
        }
    }
    evtSetValue(event, args[0], result);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_dokuroiwa_command_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    if (index < 7) {
        command_work[index] = evtGetValue(event, args[1]);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_dokuroiwa_command_clear) {
    memset(command_work, 0, 7 * sizeof(s32));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_dokuroiwa_command_init) {
    command_work = _mapAlloc(7 * sizeof(s32));
    memset(command_work, 0, 7 * sizeof(s32));
    return EVT_RETURN_DONE;
}

EVT_BEGIN(sanders_rotate)
    CALL(evt_sub_intpl_msec_init, 0, LW(1), LW(2), LW(3))
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_npc_set_rotate, LW(10), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(sanders_nakama_sub)
    SET_VALUE(LW(8), STRING("me"))
    SET_VALUE(LW(9), 3)
    CALL_CHILD(PTR(all_party_lecture))
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

EVT_BEGIN(sanders_nakama)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_move_behind_mario, 0)
    CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダース"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_THUNDERS_HIT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg5_muj_55"), 0, STRING("me"))
    CALL(evt_cam3d_evt_set, -117, 125, 357, -117, 63, 6, 500, 11)
    WAIT_MS(600)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 225, 30)
    SET_VALUE(LW(10), STRING("me"))
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 180)
    SET_VALUE(LW(3), 200)
    SPAWN_EVENT(PTR(sanders_rotate))
    WAIT_MS(500)
    SET_VALUE(LW(10), STRING("me"))
    SET_VALUE(LW(1), 180)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 200)
    SPAWN_EVENT(PTR(sanders_rotate))
    WAIT_MS(500)
    SET_VALUE(LW(10), STRING("me"))
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 180)
    SET_VALUE(LW(3), 200)
    SPAWN_EVENT(PTR(sanders_rotate))
    WAIT_MS(500)
    SET_VALUE(LW(10), STRING("me"))
    SET_VALUE(LW(1), 180)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 200)
    SPAWN_EVENT(PTR(sanders_rotate))
    WAIT_MS(500)
    CALL(evt_party_stop, 0)
    CALL(evt_msg_print_party, STRING("stg5_muj_56"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_62"), 0, STRING("me"))
    CALL(evt_msg_print_party, STRING("stg5_muj_63"))
    CALL(evt_party_run, 0)
    CALL(evt_msg_print, 0, STRING("stg5_muj_69"), 0, STRING("me"))
    CALL_CHILD(PTR(sanders_nakama_sub))
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, 239)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

static EVT_BEGIN(sanders_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(235)
        CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダースへろへろ"))
        CASE_BETWEEN(235, 237)
        CALL(evt_npc_set_position, STRING("me"), -114, 22, 22)
        CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダースへろへろ"))
        CASE_EQUAL(238)
        CALL(evt_npc_reaction_flag_onoff, 1, STRING("me"), 2)
        CALL(evt_npc_change_interrupt, STRING("me"), 8, PTR(sanders_nakama))
        CALL(evt_npc_set_position, STRING("me"), -114, 22, 22)
        CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダースねむる"))
        CALL(evt_npc_flag_onoff, 0, STRING("サンダース"), NPC_FLAG_TALK_POSE)
    END_SWITCH
    RETURN
EVT_END()

static s32 itemsel_table01[] = {
    ITEM_CHUCKOLA_COLA, -1,
};

EVT_BEGIN(sanders_talk_sub)
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg5_muj_45"), 0, STRING("me"))
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_ORGEL1"))
    CALL(evt_cam3d_evt_set, -117, 125, 357, -117, 63, 6, 500, 11)
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg5_muj_46"), 0, STRING("me"))
    CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダースねむる"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("サンダース"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_reaction_flag_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_change_interrupt, STRING("me"), 8, PTR(sanders_nakama))
    SET_VALUE(GSW_STORY_PROGRESS, 238)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(sanders_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(235, 237)
        CALL(evt_pouch_check_item, 70, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_34_03"), 0, STRING("me"))
            ELSE
            CALL(evt_win_item_select, 0, PTR(itemsel_table01), LW(0), 0)
            IF_INT_EQUAL(LW(0), -1)
                RETURN
            END_IF
            CALL(evt_pouch_remove_item, 70, LW(0))
            SPAWN_EVENT(PTR(sanders_talk_sub))
        END_IF
        CASE_EQUAL(238)
        IF_INT_EQUAL(GSWF(3138), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_47"), 0, STRING("me"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
            END_INLINE_EVENT
            CALL(evt_eff_fukidashi, 0, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
            CALL(evt_eff_fukidashi, 3, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
            WAIT_MS(1000)
            SET_VALUE(GSWF(3138), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_48"), 0, STRING("me"))
            BEGIN_INLINE_EVENT
                SET_VALUE(LW(0), 32000)
                CALL(evt_snd_bgmoff_f, 512, 3000)
                LOOP(100)
                    SUBTRACT(LW(0), 300)
                    CALL(evt_snd_bgm_freq, 0, LW(0))
                    WAIT_MS(50)
                END_LOOP
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
            END_INLINE_EVENT
            CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
            CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
            WAIT_MS(1000)
            CALL(evt_party_stop, 0)
            CALL(evt_msg_print_party, STRING("stg5_muj_49"))
            CALL(evt_party_run, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(marco_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(241)
        CALL(evt_npc_set_position, STRING("me"), 279, 0, -67)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        IF_INT_EQUAL(GSWF(3141), 0)
            SPAWN_EVENT_GET_ID(PTR(marco_hanauta), GW(2))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(marco_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(241)
        IF_INT_EQUAL(GSWF(3141), 0)
            DELETE_EVENT(GW(2))
            CALL(evt_msg_close, STRING("stg5_muj_93_02_00"))
        END_IF
        SET_VALUE(GSWF(3141), 1)
        IF_INT_EQUAL(GSWF(3139), 0)
            CALL(evt_pouch_check_item, 71, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                IF_INT_EQUAL(GSWF(3142), 0)
                    CALL(evt_msg_print, 0, STRING("stg5_muj_93_02"), 0, STRING("マルコ"))
                    IF_INT_EQUAL(GSWF(3140), 0)
                        RETURN
                    END_IF
                    CALL(evt_mario_dispflag_onoff, 1, 2)
                    CALL(evt_mario_set_pose, STRING("M_I_2"))
                    WAIT_MS(1000)
                    CALL(evt_mario_dispflag_onoff, 0, 2)
                END_IF
                SET_VALUE(GSWF(3142), 1)
                CALL(evt_msg_print, 0, STRING("stg5_muj_93_03"), 0, STRING("マルコ"))
                CALL(evt_msg_select, 0, STRING("stg5_muj_93_03_select"))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print_add, 0, STRING("stg5_muj_93_03_01"))
                    CASE_EQUAL(1)
                    CALL(evt_msg_print_add, 0, STRING("stg5_muj_93_03_02"))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print_add, 0, STRING("stg5_muj_93_03_03"))
                    CALL(evt_item_entry, STRING("jewel"), 71, 0, -1000, 0, 16, -1, 0)
                    CALL(evt_item_flag_on, STRING("jewel"), 2)
                    CALL(evt_item_get_item, STRING("jewel"))
                END_SWITCH
                ELSE
                CALL(evt_msg_print, 0, STRING("stg5_muj_93_05"), 0, STRING("マルコ"))
            END_IF
            ELSE
            IF_INT_EQUAL(GW(12), 0)
                CALL(evt_msg_print, 0, STRING("stg5_muj_93_06"), 0, STRING("マルコ"))
                ELSE
                CALL(evt_msg_print, 0, STRING("stg5_muj_93_07"), 0, STRING("マルコ"))
                SET_VALUE(GW(12), 0)
            END_IF
        END_IF
        CASE_EQUAL(242)
        CALL(evt_msg_print, 0, STRING("stg5_muj_93_09"), 0, STRING("マルコ"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(erumosu_regl)
    WAIT_WHILE(1)
    RETURN
EVT_END()

EVT_BEGIN(erumosu_a_dead)
    SET_VALUE(GSWF(3137), 1)
    DELETE_EVENT(GW(0))
    DELETE_EVENT(GW(2))
    CALL(evt_npc_set_position, STRING("エルモス１"), 0, -1000, 0)
    CALL(evt_run_case_evt, 13, 1, STRING("A_ki_02_01"), 0, PTR(sanders_kyushutsu_sub), 0)
    RETURN
EVT_END()

EVT_BEGIN(erumosu_b_dead)
    DELETE_EVENT(GW(1))
    DELETE_EVENT(GW(3))
    CALL(evt_npc_set_position, STRING("エルモス２"), 0, -1000, 0)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        sanders_init, // initEvent
        NULL, // regularEvent
        sanders_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        marco_init, // initEvent
        NULL, // regularEvent
        marco_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"エルモス１"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x82\x50", // name
        NPC_FLAG_ENEMY | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        erumosu_regl, // regularEvent
        NULL, // talkEvent
        erumosu_a_dead, // deadEvent
        NULL, NULL, NULL, NULL, 0, // findEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        1.0f, // searchRange
        1.0f, // searchAngle
        1.0f, // homingRange
        1.0f, // homingAngle
        7, // battleInfoId
    },
    {
        /*"エルモス２"*/"\x83\x47\x83\x8B\x83\x82\x83\x58\x82\x51", // name
        NPC_FLAG_ENEMY | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        erumosu_regl, // regularEvent
        NULL, // talkEvent
        erumosu_b_dead, // deadEvent
        NULL, NULL, NULL, NULL, 0, // findEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        1.0f, // searchRange
        1.0f, // searchAngle
        1.0f, // homingRange
        1.0f, // homingAngle
        7, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_after[] = {
    {
        /*"サンダース"*/"\x83\x54\x83\x93\x83\x5F\x81\x5B\x83\x58", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        sanders_init, // initEvent
        NULL, // regularEvent
        sanders_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        marco_init, // initEvent
        NULL, // regularEvent
        marco_talk, // talkEvent
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
        "muj_04", // nextMapName
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
        "dou_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(hammer_sub)
    CALL(evt_mapobj_get_position, STRING("S_treeA_05"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_TREE_SHAKE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_LEAF_SHAKE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("S_ki"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(hammer_event)
    CALL(evt_run_case_evt, 13, 1, STRING("A_ki_02_01"), 0, PTR(hammer_sub), 0)
    RETURN
EVT_END()

EVT_BEGIN(sanders_kyushutsu_sub)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 235)
    CALL(evt_msg_print, 0, STRING("stg5_muj_33_01"), 0, STRING("サンダース"))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(0), -110)
        CALL(evt_npc_set_ry, STRING("サンダース"), 270)
        CALL(evt_npc_set_position, STRING("サンダース"), -85, 200, 23)
        BEGIN_INLINE_EVENT
            CALL(evt_party_move_pos2, 0, -167, 38, FLOAT(150.0f))
            WAIT_FRAMES(1)
            CALL(evt_party_set_dir, 0, 90, -1)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_mario_set_dir, 90, 200, 0)
        END_INLINE_EVENT
        ELSE
        CALL(evt_npc_set_ry, STRING("サンダース"), 90)
        CALL(evt_npc_set_position, STRING("サンダース"), -150, 200, 23)
        BEGIN_INLINE_EVENT
            CALL(evt_party_move_pos2, 0, -55, 67, FLOAT(150.0f))
            WAIT_FRAMES(1)
            CALL(evt_party_set_dir, 0, 270, -1)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_mario_set_dir, 270, 200, 0)
        END_INLINE_EVENT
    END_IF
    CALL(evt_hitobj_onoff, STRING("A_ki_02_02"), 0, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("サンダース"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, STRING("サンダース"), NPC_FLAG_AUTO_MOVE_ANIM)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_THUNDERS_FALL1"), 85, 40, 23, LW(10))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("サンダース"), LW(0), LW(1), LW(2))
            IF_INT_LESS_OR_EQUAL(LW(1), 23)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_THUNDERS_DOWN1"), 85, 40, 23, 0)
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_npc_get_position, STRING("サンダース"), LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(1), 23)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_anim, STRING("サンダース"), STRING("F_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("サンダース"), STRING("S_1"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("サンダース"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_ORGEL1"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_34"), 0, STRING("サンダース"))
    CALL(evt_cam3d_evt_set, -105, 120, 324, -105, 62, 3, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_34_01"), 0, STRING("me"))
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(1000)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_msg_print, 0, STRING("stg5_muj_34_02"), 0, STRING("サンダース"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
    SPAWN_EVENT(PTR(hammer_event))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

EVT_BEGIN(erumosu_guruguru)
    LOOP(0)
        CALL(muj_npc_guruguru, LW(0), -115, -15, 1, FLOAT(-5.0f), 3000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(erumosu_guruguru_dir)
    LOOP(0)
        CALL(evt_set_dir_to_target, LW(0), STRING("サンダース"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(sanders_kyushutsu)
    CALL(evt_npc_set_position, STRING("サンダース"), -88, 200, -34)
    IF_INT_EQUAL(GSWF(3137), 0)
        CALL(evt_npc_set_position, STRING("エルモス１"), -158, 23, 0)
        CALL(evt_npc_set_position, STRING("エルモス２"), -68, 23, 0)
        CALL(evt_npc_set_link, STRING("エルモス１"), STRING("エルモス２"))
        CALL(evt_npc_set_battle_info, STRING("エルモス１"), 8)
        CALL(evt_npc_set_battle_info, STRING("エルモス２"), 8)
        SET_VALUE(LW(0), STRING("エルモス１"))
        SPAWN_EVENT_GET_ID(PTR(erumosu_guruguru), GW(0))
        SPAWN_EVENT_GET_ID(PTR(erumosu_guruguru_dir), GW(2))
        SET_VALUE(LW(0), STRING("エルモス２"))
        SPAWN_EVENT_GET_ID(PTR(erumosu_guruguru), GW(1))
        SPAWN_EVENT_GET_ID(PTR(erumosu_guruguru_dir), GW(3))
        ELSE
        CALL(evt_run_case_evt, 13, 1, STRING("A_ki_02_01"), 0, PTR(sanders_kyushutsu_sub), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(marco_hanauta)
    RETURN
EVT_END()

EVT_BEGIN(marco_and_jewel)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 500)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    WAIT_MS(1000)
    CALL(evt_party_stop, 1)
    CALL(evt_party_set_dir, 1, 90, -1)
    CALL(evt_party_move_pos2, 1, 537, 40, FLOAT(120.0f))
    CALL(evt_msg_print, 0, STRING("stg5_muj_93_00"), 0, STRING("extparty"))
    CALL(evt_party_set_dir, 1, 270, -1)
    CALL(evt_party_move_pos2, 1, 279, -67, FLOAT(120.0f))
    CALL(evt_party_get_pos, 1, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("マルコ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("マルコ"), 90)
    SPAWN_EVENT_GET_ID(PTR(marco_hanauta), GW(2))
    CALL(evt_mario_clear_party, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 241)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(blueeye_flush)
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa03_me"), LW(0), LW(1), LW(2))
    ADD(LW(0), -12)
    ADD(LW(1), 2)
    ADD(LW(2), 10)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ROCK_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 2, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa03_me"), LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    ADD(LW(1), 2)
    ADD(LW(2), 10)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ROCK_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 2, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(redeye_flush)
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa02_me"), LW(0), LW(1), LW(2))
    ADD(LW(0), -12)
    ADD(LW(1), 2)
    ADD(LW(2), 10)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ROCK_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 1, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa02_me"), LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    ADD(LW(1), 2)
    ADD(LW(2), 10)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ROCK_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 1, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(dokuroiwa_flush)
    CALL(evt_mapobj_get_position, STRING("S_dkr_iwa02"), LW(0), LW(1), LW(2))
    ADD(LW(0), 0)
    ADD(LW(2), 20)
    CALL(evt_sub_random, 10, LW(10))
    CALL(evt_sub_random, 5, LW(11))
    ADD(LW(0), LW(10))
    ADD(LW(1), LW(11))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_ROCK_EYE_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff64, STRING(""), STRING("hagetaka_flush_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 60, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

s32 command_seikai[] = {
    2, 2, 2, 1, 1, 1, 1,
};

static s32 itemsel_table[] = {
    ITEM_SKULL_GEM, -1,
};

EVT_BEGIN(dokuroiwa_touch)
    CALL(evt_pouch_check_item, 71, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg5_muj_93_01"), 0, 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 240)
            CALL(evt_party_stop, 0)
            CALL(evt_msg_print_party, STRING("stg5_muj_93_01_01"))
            CALL(evt_party_run, 0)
            SET_VALUE(GSWF(3140), 1)
        END_IF
        CALL(evt_mario_key_onoff, 1)
        ELSE
        CALL(evt_win_item_select, 0, PTR(itemsel_table), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            RETURN
        END_IF
        CALL(evt_mapobj_get_position, STRING("S_jel"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_JUWEL_SET1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_jel"), MAP_OBJ_FLAG_HIDDEN)
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
            SPAWN_EVENT(PTR(blueeye_flush))
            SPAWN_EVENT(PTR(redeye_flush))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_sub_intpl_msec_init, 11, FLOAT(0.0f), FLOAT(1.0f), 500)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
                CALL(evt_mapobj_scale, 1, STRING("S_hige_iwa02_me"), FLOAT(1.0f), LW(0), FLOAT(1.0f))
                CALL(evt_mapobj_scale, 1, STRING("S_hige_iwa03_me"), FLOAT(1.0f), LW(0), FLOAT(1.0f))
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_mario_dump_item, 71)
        SET_VALUE(GSWF(3139), 1)
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(case_event_entry)
    SET_VALUE(GF(10), 0)
    CALL(evt_run_case_evt, 13, 1, STRING("A_iwa01"), 0, PTR(iwa01_hammer), GW(5))
    CALL(evt_run_case_evt, 4, 1, STRING("A_iwa01"), 0, PTR(iwa01_ride_a), GW(6))
    CALL(evt_run_case_evt, 0, 1, STRING("A_iwa_atari"), 0, PTR(iwa02_ride), GW(7))
    CALL(evt_run_case_evt, 13, 1, STRING("A_iwa_atari"), 0, PTR(iwa02_hammer), GW(8))
    CALL(evt_run_case_evt, 4, 1, STRING("A_iwa_atari"), 0, PTR(iwa02_ride_a), GW(9))
    RETURN
EVT_END()

EVT_BEGIN(case_event_delete)
    IF_INT_NOT_EQUAL(GW(5), -1)
        CALL(evt_del_case_evt, 0, GW(5))
    END_IF
    IF_INT_NOT_EQUAL(GW(6), -1)
        CALL(evt_del_case_evt, 0, GW(6))
    END_IF
    IF_INT_NOT_EQUAL(GW(7), -1)
        CALL(evt_del_case_evt, 0, GW(7))
    END_IF
    IF_INT_NOT_EQUAL(GW(8), -1)
        CALL(evt_del_case_evt, 0, GW(8))
    END_IF
    IF_INT_NOT_EQUAL(GW(9), -1)
        CALL(evt_del_case_evt, 0, GW(9))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(penalty_event)
    CALL(evt_mario_key_onoff, 0)
    DELETE_EVENT(GW(2))
    CALL(evt_msg_close, STRING("stg5_muj_93_02_00"))
    CALL(evt_npc_flag_onoff, 1, STRING("エルモスA"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("エルモスB"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("エルモスC"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    BEGIN_INLINE_EVENT
        ADD(LW(0), -23)
        ADD(LW(2), -20)
        CALL(evt_npc_set_position, STRING("エルモスB"), LW(0), 220, LW(2))
        CALL(evt_npc_set_ry, STRING("エルモスB"), 90)
        CALL(evt_npc_glide_position, STRING("エルモスB"), LW(0), 20, LW(2), 1000, 0, 0, 11, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        ADD(LW(0), -28)
        ADD(LW(2), 48)
        CALL(evt_npc_set_position, STRING("エルモスC"), LW(0), 210, LW(2))
        CALL(evt_npc_set_ry, STRING("エルモスC"), 90)
        CALL(evt_npc_glide_position, STRING("エルモスC"), LW(0), 10, LW(2), 1000, 0, 0, 11, 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_position, STRING("エルモスA"), LW(0), 200, LW(2))
    CALL(evt_npc_set_ry, STRING("エルモスA"), 90)
    CALL(evt_npc_glide_position, STRING("エルモスA"), LW(0), 0, LW(2), 1000, 0, 0, 11, 0)
    CALL(evt_mario_set_dir, 270, 200, 0)
    WAIT_MS(500)
    LOOP(0)
        CALL(evt_mario_get_motion, LW(10))
        IF_INT_EQUAL(LW(10), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_npc_set_battle_info, STRING("エルモスA"), 7)
    CALL(evt_npc_battle_start, STRING("エルモスA"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_npc_set_position, STRING("エルモスA"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("エルモスB"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("エルモスC"), 0, -1000, 0)
    SET_VALUE(GW(10), 0)
    SET_VALUE(GW(12), 1)
    SET_VALUE(GW(13), 0)
    CALL(evt_dokuroiwa_command_clear)
    SPAWN_EVENT_GET_ID(PTR(marco_hanauta), GW(2))
    CALL_CHILD(PTR(case_event_delete))
    CALL_CHILD(PTR(case_event_entry))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(cam_shake)
    CALL(evt_sub_intpl_init, 11, 0, 10, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 0)
    RETURN
EVT_END()

static EVT_BEGIN(cam_shake2)
    CALL(evt_sub_intpl_init, 11, 10, 0, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(answer_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 525, 119, 584, 525, 23, 42, 500, 11)
    WAIT_MS(500)
    WAIT_MS(500)
    SPAWN_EVENT(PTR(dokuroiwa_flush))
    WAIT_MS(300)
    SPAWN_EVENT(PTR(dokuroiwa_flush))
    WAIT_MS(300)
    SPAWN_EVENT(PTR(dokuroiwa_flush))
    WAIT_MS(300)
    SPAWN_EVENT(PTR(dokuroiwa_flush))
    WAIT_MS(300)
    SPAWN_EVENT(PTR(dokuroiwa_flush))
    WAIT_MS(300)
    CALL(evt_mapobj_get_position, STRING("S_dkr_iwa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_QUAKE1"), LW(0), LW(1), LW(2), LW(9))
    SPAWN_EVENT_GET_ID(PTR(cam_shake), LW(10))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 357, 125, 335, 551, 69, 90, 1000, 11)
    WAIT_MS(2000)
    DELETE_EVENT(LW(10))
    CALL_CHILD(PTR(cam_shake2))
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_mapobj_get_position, STRING("S_dokuro_iwa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_ROCK_WINDOW1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("S-dkr_iwa"), 0, 0)
    CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa02"), 1, 1)
    CALL(evt_map_checkanim, STRING("S-dkr_iwa"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(1000)
    SET_VALUE(GSWF(3149), 1)
    CALL_CHILD(PTR(case_event_delete))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(iwa01_hammer)
    IF_INT_EQUAL(GF(10), 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3139), 0)
        RETURN
    END_IF
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa03_me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_BIG_ROCK_HIT1"), LW(0), LW(1), LW(2), 0)
    SPAWN_EVENT(PTR(blueeye_flush))
    CALL(evt_dokuroiwa_command_set, GW(10), 1)
    ADD(GW(10), 1)
    CALL(evt_dokuroiwa_command_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), -30)
        SPAWN_EVENT(PTR(penalty_event))
        SET_VALUE(GW(5), -1)
        CALL(evt_exit_case_evt)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SPAWN_EVENT(PTR(answer_event))
        SET_VALUE(GW(5), -1)
        CALL(evt_exit_case_evt)
    END_IF
    WAIT_MS(300)
    SET_VALUE(GF(10), 0)
    RETURN
EVT_END()

EVT_BEGIN(iwa01_ride_a)
    IF_INT_EQUAL(GSWF(3139), 0)
        RETURN
    END_IF
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa03_me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_BIG_ROCK_JUMP1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(iwa02_ride)
    CALL(evt_mario_chk_hipbump, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GW(13), 1)
        RETURN
    END_IF
    SET_VALUE(GW(13), 1)
    IF_INT_EQUAL(GSWF(3139), 0)
        RETURN
    END_IF
    SPAWN_EVENT(PTR(redeye_flush))
    CALL(evt_dokuroiwa_command_set, GW(10), 2)
    ADD(GW(10), 1)
    CALL(evt_dokuroiwa_command_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), -30)
        SPAWN_EVENT(PTR(penalty_event))
        SET_VALUE(GW(7), -1)
        CALL(evt_exit_case_evt)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SPAWN_EVENT(PTR(answer_event))
        SET_VALUE(GW(7), -1)
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iwa02_hammer)
    IF_INT_EQUAL(GSWF(3139), 0)
        RETURN
    END_IF
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa02_me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_SMALL_ROCK_HIT1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(iwa02_ride_a)
    CALL(evt_mario_chk_hipbump, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(3139), 0)
        RETURN
    END_IF
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa02_me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_SMALL_ROCK_JUMP1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(iwa02_nonride_a)
    SET_VALUE(GW(13), 0)
    RETURN
EVT_END()

static EVT_BEGIN(bombwall_bomb)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("S_dokuro_iwa"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_entry, STRING("capture_img"))
    CALL(evt_img_set_paper, STRING("capture_img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("offscreen"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("S_dokuro_iwa"), STRING("offscreen"))
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
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_dokuro_iwa"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_dokuro_iwa"), 1, 0)
        CALL(evt_img_wait_animend, STRING("capture_img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_dokuro_iwa"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_dokuro_iwa"), 1, 0)
    END_IF
    CALL(evt_mapobj_clear_offscreen, 1, STRING("S_dokuro_iwa"))
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
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(dokuroiwa_bomb)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_onoff, STRING("e_bero"), 1, 1)
    CALL(evt_mapobj_get_position, STRING("S_dokuro_iwa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, 1024)
        CALL(evt_snd_bgmon, 1, STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.029296875f), FLOAT(0.0f), 1000)
    END_INLINE_EVENT
    CALL_CHILD(PTR(bombwall_bomb))
    CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa02"), 1, 0)
    IF_INT_EQUAL(GSWF(3141), 0)
        DELETE_EVENT(GW(2))
        CALL(evt_msg_close, STRING("stg5_muj_93_02_00"))
    END_IF
    CALL(evt_npc_set_ry, STRING("マルコ"), 90)
    CALL(evt_cam3d_evt_set, 280, 115, 333, 280, 55, -11, 500, 11)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("マルコ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_get_position, STRING("マルコ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("マルコ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_jump_position_nohit, STRING("マルコ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("stg5_muj_93_08"), 0, STRING("マルコ"))
    CALL(evt_item_entry, STRING("jewel00"), 71, 225, 200, -25, 13, -1, 0)
    SET_VALUE(LW(8), STRING("jewel00"))
    CALL(evt_item_flag_on, LW(8), 16)
    CALL(evt_item_set_gravity, LW(8), FLOAT(1.0f))
    CALL(evt_item_set_bound_rate, LW(8), FLOAT(0.69921875f))
    CALL(evt_item_set_jump_power, LW(8), FLOAT(0.0f))
    CALL(evt_item_set_bound_limit, LW(8), 0)
    ADD(LW(0), 35)
    CALL(evt_item_set_bound_next_speed, LW(8), LW(0))
    BEGIN_INLINE_EVENT
        CALL(evt_item_get_position, STRING("jewel00"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_JUWEL_FALL1"), LW(0), LW(1), LW(2), LW(10))
        LOOP(0)
            CALL(evt_item_get_position, STRING("jewel00"), LW(0), LW(1), LW(2))
            IF_INT_LESS_OR_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(10))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_JUWEL_LANDING1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("マルコ"), 270)
    WAIT_MS(1500)
    CALL(evt_cam3d_evt_set, 225, 115, 333, 225, 55, -11, 500, 11)
    CALL(evt_npc_move_position, STRING("マルコ"), 225, -50, 0, FLOAT(120.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_93_08_01"), 0, STRING("マルコ"))
    CALL(evt_item_set_position, STRING("jewel00"), 0, -1000, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("マルコ"), 90)
    WAIT_MS(500)
    CALL(evt_npc_set_ry, STRING("マルコ"), 270)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("マルコ"), 90)
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING("マルコ"), 240, -50, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("stg5_muj_93_08_02"), 0, STRING("マルコ"))
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_npc_set_ry, STRING("マルコ"), 270)
    CALL(evt_npc_move_position, STRING("マルコ"), 100, -50, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_position, STRING("マルコ"), 0, -1000, 0)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, 242)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(dokuroiwa_init)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_jel"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa02"), 1, 0)
    CALL(evt_dokuroiwa_command_init)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 242)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_dokuro_iwa"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_dokuro_iwa"), 1, 0)
        RETURN
    END_IF
    CALL(evt_hitobj_onoff, STRING("e_bero"), 1, 0)
    IF_INT_EQUAL(GSWF(3139), 0)
        CALL(evt_mapobj_scale, 1, STRING("S_hige_iwa02_me"), 1, 0, 1)
        CALL(evt_mapobj_scale, 1, STRING("S_hige_iwa03_me"), 1, 0, 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_jel"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    SET_VALUE(GW(10), 0)
    SET_VALUE(GW(12), 0)
    SET_VALUE(GW(13), 0)
    IF_INT_EQUAL(GSWF(3139), 0)
        CALL(evt_run_case_evt, 9, 1, STRING("A_dkr_iwa_01"), 0, PTR(dokuroiwa_touch), 0)
    END_IF
    IF_INT_EQUAL(GSWF(3149), 0)
        CALL_CHILD(PTR(case_event_entry))
        CALL(evt_run_case_evt, 1, 1, STRING("A_iwa_atari"), 0, PTR(iwa02_nonride_a), 0)
        ELSE
        CALL(evt_map_playanim, STRING("S-dkr_iwa"), 2, 0)
        CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa02"), 1, 1)
    END_IF
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    CALL(evt_run_case_evt, 15, 1, STRING("A_dkr_iwa_02_oku_01"), 0, PTR(dokuroiwa_bomb), 0)
    RETURN
EVT_END()

EVT_BEGIN(muj_05_first)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_mario_normalize)
    CALL(evt_msg_print, 0, STRING("stg5_muj_33"), 0, STRING("サンダース"))
    SET_VALUE(GSWF(3136), 1)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(yure_yashi_ha1)
    CALL(evt_sub_intpl_msec_init, 11, 0, 2, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_01"), 0, 0, LW(0))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_05"), 0, 0, LW(0))
        SET_FLOAT_VALUE(LW(2), LW(0))
        MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_03"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_04"), 0, 0, LW(2))
        SET_FLOAT_VALUE(LW(2), 0)
        SUBTRACT_FLOAT(LW(2), LW(0))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_07"), 0, 0, LW(2))
        MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_02"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_06"), 0, 0, LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 235)
            RETURN
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 13, 2, 0, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_01"), 0, 0, LW(0))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_05"), 0, 0, LW(0))
        SET_FLOAT_VALUE(LW(2), LW(0))
        MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_03"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_04"), 0, 0, LW(2))
        SET_FLOAT_VALUE(LW(2), 0)
        SUBTRACT_FLOAT(LW(2), LW(0))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_07"), 0, 0, LW(2))
        MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_02"), 0, 0, LW(2))
        CALL(evt_mapobj_rotate, 0, STRING("treeA_05_ha_06"), 0, 0, LW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 235)
            RETURN
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(yure_yashi)
    LOOP(0)
        CALL(evt_mapobj_get_position, STRING("S_treeA_05"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_TREE_SHAKE2"), LW(0), LW(1), LW(2), 0)
        SPAWN_EVENT(PTR(yure_yashi_ha1))
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, 11, 1, -1, 800)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_treeA_05"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 235)
                RETURN
            END_IF
        END_LOOP
        CALL(evt_sub_intpl_msec_init, 11, -1, 1, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_treeA_05"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 235)
                RETURN
            END_IF
        END_LOOP
        WAIT_MS(500)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_kusa_1)
    CALL(evt_item_delete_check, STRING("item_kusa_1"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_1"), 177, 235, 0, 90, 4, GSWF(3157), 0)
    END_IF
    RETURN
EVT_END()

static TreeAccess tree_tbl[] = {
    {5, "A_treeA_01", "treeA_01_m", "treeA_01_ha", 0, NULL},
    {5, "A_treeA_02", "treeA_02_m", "treeA_02_ha", 0, NULL},
    {5, "A_treeA_03", "treeA_03_m", "treeA_03_ha", 0, NULL},
    {5, "A_treeA_04", "treeA_04_m", "treeA_04_ha", 0, NULL},
    {5, "A_treeB_03", "treeB_03_m", "treeB_03_ha", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

static KusaAccess kusa_tbl[] = {
    {1, "A_kuwa_move_01", "S_kuwa_move_01", evt_kusa_1},
    {1, "A_kuwa_move_02", "S_kuwa_move_02", NULL},
    {1, "A_kuwa_move_03", "S_kuwa_move_03", NULL},
    {1, "A_kuwa_move_04", "S_kuwa_move_04", NULL},
    {1, "A_kuwa_move_05", "S_kuwa_move_05", NULL},
    {1, "A_kuwa_move_06", "S_kuwa_move_06", NULL},
    {1, "A_kuwa_move_07", "S_kuwa_move_07", NULL},
    {4, NULL, NULL, NULL},
};

EVT_BEGIN(muj_05_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(234)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG5_MUJ7"))
        CASE_EQUAL(238)
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG5_MUJ7"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG5_MUJ1"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG5_MUJ7"))
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_EQUAL(GSWF(3137), 0)
        CALL(evt_npc_setup, PTR(npcEnt))
    ELSE
        CALL(evt_npc_setup, PTR(npcEnt_after))
    END_IF
    CALL(evt_npc_sound_data_set, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("SFX_STG5_MARCO_MOVE1L"), STRING("SFX_STG5_MARCO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_map_playanim, STRING("S_mizu"), 1, 0)
    SPAWN_EVENT(hammer_event)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(234)
        IF_INT_EQUAL(GSWF(3136), 0)
            SPAWN_EVENT(muj_05_first)
        END_IF
        SPAWN_EVENT(sanders_kyushutsu)
        SPAWN_EVENT(yure_yashi)
        CASE_EQUAL(240)
        SPAWN_EVENT(marco_and_jewel)
    END_SWITCH
    SPAWN_EVENT(dokuroiwa_init)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 274)
        CALL(evt_item_entry, STRING("item00"), 73, -250, 0, -110, 0, GSWF(3143), 0)
    END_IF
    CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 12, 125, 60, -90, 0, 0)
    CALL(evt_item_entry, STRING("item_01"), 125, 395, -15, 120, 16, GSWF(5643), 0)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(evt_sub_tree_access_entry)
    SET_VALUE(LW(0), PTR(kusa_tbl))
    CALL_CHILD(evt_sub_kusa_access_entry)
    RETURN
EVT_END()


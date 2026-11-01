// muj_05 (無人島, Keelhaul Key): the skull rock: Admiral Bobbery joins (sanders_nakama), the gem
// eyes and the skull rock's opening into Pirate's Grotto (dou_00).

#include "battle/battle_unit.h"
#include "evt/evt_window.h"
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
#include "system.h"
#include "driver/casedrv.h"
#include "pmario_sound.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"

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

EvtWord sanders_rotate[28];
EvtWord sanders_nakama_sub[13];
#if VERSION <= VERSION_DEMO_US
extern EvtWord sanders_nakama[];
#else
EvtWord sanders_nakama[173];
#endif
static EvtWord sanders_init[51];
static s32 itemsel_table01[2];
EvtWord sanders_talk_sub[70];
#if VERSION <= VERSION_DEMO_US
static EvtWord sanders_talk[];
#else
static EvtWord sanders_talk[178];
#endif
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
#if VERSION <= VERSION_DEMO_US
extern EvtWord dokuroiwa_touch[];
#else
EvtWord dokuroiwa_touch[127];
#endif
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
#if VERSION <= VERSION_DEMO_US
extern EvtWord muj_05_first[];
#else
EvtWord muj_05_first[26];
#endif
EvtWord yure_yashi_ha1[180];
EvtWord yure_yashi[88];
static EvtWord evt_kusa_1[20];
static TreeAccess tree_tbl[6];
static KusaAccess kusa_tbl[8];
EvtWord muj_05_init_evt[146];

/**
 * Script helper for the skull rock puzzle: compares the seven eye strikes Mario has made (command_work) with the
 * correct sequence (command_seikai).
 * Writes 0 to args[0] if fewer than seven have been entered, 1 if one of them is wrong and 2 if all seven match.
 */
USER_FUNC(evt_dokuroiwa_command_check) {
    s32 result;
    s32 i;
    EvtWord* args = event->args;

    // FAKE: empty seven-iteration loop; removing it changes the object.
    for (i = 0; i < 7; i++) {
    }
    // Report 0 for the first unfilled slot, 1 for a mismatch, or 2 when all seven match.
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

/**
 * Script helper: records one eye strike of the skull rock puzzle:
 * args[0] is the slot (0 to 6), args[1] the eye hit (1 = blue, 2 = red).
 */
USER_FUNC(evt_dokuroiwa_command_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    // The caller must supply a nonnegative index; only the upper bound is checked here.
    if (index < 7) {
        command_work[index] = evtGetValue(event, args[1]);
    }
    return EVT_RETURN_DONE;
}

/** Script helper: clears the seven recorded eye strikes so the puzzle starts over. */
USER_FUNC(evt_dokuroiwa_command_clear) {
    memset(command_work, 0, 7 * sizeof(s32));
    return EVT_RETURN_DONE;
}

/** Script helper: allocates the seven-word eye strike buffer from map memory and clears it. */
USER_FUNC(evt_dokuroiwa_command_init) {
    command_work = _mapAlloc(7 * sizeof(s32));
    memset(command_work, 0, 7 * sizeof(s32));
    return EVT_RETURN_DONE;
}

/** Turns the NPC in LW(10) from LW(1) to LW(2) degrees over LW(3) ms; Bobbery uses it to look around while he talks. */
EVT_BEGIN(sanders_rotate)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, LW(1), LW(2), LW(3))
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

/**
 * Runs the partner lecture for Admiral Bobbery (all_party_lecture) when he joins,
 * then sends the partner back out running.
 */
EVT_BEGIN(sanders_nakama_sub)
    SET_VALUE(LW(8), STRING("me"))
    SET_VALUE(LW(9), 3)
    CALL_CHILD(PTR(all_party_lecture))
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

/**
 * Admiral Bobbery joins the party (STG5_18_01_BOBBERY_JOINS):
 * after talk with Mario and his partner (stg5_muj_55 to 69) he becomes a partner and the story advances.
 */
EVT_BEGIN(sanders_nakama)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_move_behind_mario, 0)
    CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダース"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_THUNDERS_HIT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg5_muj_55"), 0, STRING("me"))
    CALL(evt_cam3d_evt_set, -117, 125, 357, -117, 63, 6, 500, EASING_SINE_IN_OUT)
    WAIT_MS(600)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("me"), LW(0), LW(1), LW(2), 225, 30)
    SET_VALUE(LW(10), STRING("me"))
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 180)
    SET_VALUE(LW(3), 200)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(sanders_rotate)
#else
    SPAWN_EVENT(PTR(sanders_rotate))
#endif
    WAIT_MS(500)
    SET_VALUE(LW(10), STRING("me"))
    SET_VALUE(LW(1), 180)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 200)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(sanders_rotate)
#else
    SPAWN_EVENT(PTR(sanders_rotate))
#endif
    WAIT_MS(500)
    SET_VALUE(LW(10), STRING("me"))
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 180)
    SET_VALUE(LW(3), 200)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(sanders_rotate)
#else
    SPAWN_EVENT(PTR(sanders_rotate))
#endif
    WAIT_MS(500)
    SET_VALUE(LW(10), STRING("me"))
    SET_VALUE(LW(1), 180)
    SET_VALUE(LW(2), 0)
    SET_VALUE(LW(3), 200)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(sanders_rotate)
#else
    SPAWN_EVENT(PTR(sanders_rotate))
#endif
    WAIT_MS(500)
#if VERSION > VERSION_DEMO_US
    CALL(evt_party_stop, 0)
#endif
    CALL(evt_msg_print_party, STRING("stg5_muj_56"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_62"), 0, STRING("me"))
    CALL(evt_msg_print_party, STRING("stg5_muj_63"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_party_run, 0)
#endif
    CALL(evt_msg_print, 0, STRING("stg5_muj_69"), 0, STRING("me"))
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(sanders_nakama_sub)
#else
    CALL_CHILD(PTR(sanders_nakama_sub))
#endif
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_MUJ1"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSW(GSW_STORY_PROGRESS), STG5_18_01_BOBBERY_JOINS)
#else
    SET_VALUE(GSW_STORY_PROGRESS, STG5_18_01_BOBBERY_JOINS)
#endif
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

/**
 * Init event of Bobbery (サンダース): before STG5_18_BOBBERY_RESCUE_2 he uses the worn-out tribe (サンダースへろへろ,
 * pose D_2), from STG5_15_RUNAWAY_SAILORS placed at (-114, 22, 22);
 * at STG5_18_BOBBERY_RESCUE_2 he is placed there asleep (サンダースねむる) with sanders_nakama as his interrupt
 * event.
 */
static EVT_BEGIN(sanders_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG5_15_RUNAWAY_SAILORS)
        CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダースへろへろ"))
        CASE_BETWEEN(STG5_15_RUNAWAY_SAILORS, STG5_17_WINE_OBTAINED)
        CALL(evt_npc_set_position, STRING("me"), -114, 22, 22)
        CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダースへろへろ"))
        CASE_EQUAL(STG5_18_BOBBERY_RESCUE_2)
        CALL(evt_npc_reaction_flag_onoff, 1, STRING("me"), 2)
        CALL(evt_npc_change_interrupt, STRING("me"), 8, PTR(sanders_nakama))
        CALL(evt_npc_set_position, STRING("me"), -114, 22, 22)
        CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダースねむる"))
        CALL(evt_npc_flag_onoff, 0, STRING("サンダース"), NPC_FLAG_TALK_POSE)
    END_SWITCH
    RETURN
EVT_END()

/** Items the player may offer Bobbery in the Chuckola Cola scene: the Chuckola Cola only, ended by -1. */
static s32 itemsel_table01[] = {
    ITEM_CHUCKOLA_COLA, -1,
};

/**
 * Bobbery's first talk after being found in the cave (STG5_15_RUNAWAY_SAILORS):
 * he falls asleep to the music-box tune (stg5_muj_45, 46) and the story moves to STG5_18_BOBBERY_RESCUE_2.
 */
EVT_BEGIN(sanders_talk_sub)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg5_muj_45"), 0, STRING("me"))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_ORGEL1"))
    CALL(evt_cam3d_evt_set, -117, 125, 357, -117, 63, 6, 500, EASING_SINE_IN_OUT)
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg5_muj_46"), 0, STRING("me"))
    CALL(evt_npc_set_tribe, STRING("me"), STRING("サンダースねむる"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("サンダース"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_reaction_flag_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_change_interrupt, STRING("me"), 8, PTR(sanders_nakama))
    SET_VALUE(GSW_STORY_PROGRESS, STG5_18_BOBBERY_RESCUE_2)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(sanders_talk)
#if VERSION <= VERSION_DEMO_US
    SWITCH(GSW(GSW_STORY_PROGRESS))
#else
    SWITCH(GSW_STORY_PROGRESS)
#endif
        CASE_BETWEEN(STG5_15_RUNAWAY_SAILORS, STG5_17_WINE_OBTAINED)
        CALL(evt_pouch_check_item, ITEM_CHUCKOLA_COLA, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("stg5_muj_34_03"), 0, STRING("me"))
            ELSE
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, PTR(itemsel_table01), LW(0))
#else
            CALL(evt_win_item_select, 0, PTR(itemsel_table01), LW(0), 0)
#endif
            IF_INT_EQUAL(LW(0), -1)
                RETURN
            END_IF
            CALL(evt_pouch_remove_item, ITEM_CHUCKOLA_COLA, LW(0))
#if VERSION <= VERSION_DEMO_US
            SPAWN_EVENT(sanders_talk_sub)
#else
            SPAWN_EVENT(PTR(sanders_talk_sub))
#endif
        END_IF
        CASE_EQUAL(STG5_18_BOBBERY_RESCUE_2)
#if VERSION <= VERSION_DEMO_US
        IF_INT_EQUAL(GSWF_MUJ_05_BOBBERY_FIRST_TALK_DONE, 0)
#else
        IF_INT_EQUAL(GSWF_MUJ_05_BOBBERY_FIRST_TALK_DONE, 0)
#endif
            CALL(evt_msg_print, 0, STRING("stg5_muj_47"), 0, STRING("me"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_QUESTION1"), LW(0), LW(1), LW(2), 0)
            END_INLINE_EVENT
            CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
            CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 1, 0, 0, 0, 0, 0, 0, 60)
            WAIT_MS(1000)
#if VERSION <= VERSION_DEMO_US
            SET_VALUE(GSWF_MUJ_05_BOBBERY_FIRST_TALK_DONE, 1)
#else
            SET_VALUE(GSWF_MUJ_05_BOBBERY_FIRST_TALK_DONE, 1)
#endif
            ELSE
            CALL(evt_msg_print, 0, STRING("stg5_muj_48"), 0, STRING("me"))
            BEGIN_INLINE_EVENT
                SET_VALUE(LW(0), 32000)
                CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
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
            CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
            CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
            WAIT_MS(1000)
#if VERSION > VERSION_DEMO_US
            CALL(evt_party_stop, 0)
#endif
            CALL(evt_msg_print_party, STRING("stg5_muj_49"))
#if VERSION > VERSION_DEMO_US
            CALL(evt_party_run, 0)
#endif
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(marco_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_18_03_MARCOS_JEWEL)
        CALL(evt_npc_set_position, STRING("me"), 279, 0, -67)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        IF_INT_EQUAL(GSWF_MUJ_05_FLAVIO_HUMMING_STOPPED, 0)
            SPAWN_EVENT_GET_ID(PTR(marco_hanauta), GW(2))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk event of Flavio at the skull rock at STG5_18_03_MARCOS_JEWEL: stops his humming for good.
 * Before the Skull Gem is placed: if Mario lacks it, Flavio sings his skull-captain song (stg5_muj_93_02) and,
 * once the rock has been examined, asks what Mario wants (stg5_muj_93_03);
 * asking for the Skull Gem makes him lend it. If Mario has it he sings again (stg5_muj_93_05).
 * After the gem is placed he cheers Mario on (stg5_muj_93_06),
 * or says stg5_muj_93_07 once after a failed attempt (GW(12) set by penalty_event).
 * At STG5_19_GEMS_AND_JINGLES he says stg5_muj_93_09.
 */
static EVT_BEGIN(marco_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_18_03_MARCOS_JEWEL)
        IF_INT_EQUAL(GSWF_MUJ_05_FLAVIO_HUMMING_STOPPED, 0)
            DELETE_EVENT(GW(2))
            CALL(evt_msg_close, STRING("stg5_muj_93_02_00"))
        END_IF
        SET_VALUE(GSWF_MUJ_05_FLAVIO_HUMMING_STOPPED, 1)
        IF_INT_EQUAL(GSWF_MUJ_05_SKULL_GEM_PLACED, 0)
            CALL(evt_pouch_check_item, ITEM_SKULL_GEM, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                IF_INT_EQUAL(GSWF_MUJ_05_FLAVIO_FAVOR_ASKED, 0)
                    CALL(evt_msg_print, 0, STRING("stg5_muj_93_02"), 0, STRING("マルコ"))
                    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_ROCK_EXAMINED, 0)
                        RETURN
                    END_IF
                    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
                    CALL(evt_mario_set_pose, STRING("M_I_2"))
                    WAIT_MS(1000)
                    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
                END_IF
                SET_VALUE(GSWF_MUJ_05_FLAVIO_FAVOR_ASKED, 1)
                CALL(evt_msg_print, 0, STRING("stg5_muj_93_03"), 0, STRING("マルコ"))
                CALL(evt_msg_select, 0, STRING("stg5_muj_93_03_select"))
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print_add, 0, STRING("stg5_muj_93_03_01"))
                    CASE_EQUAL(1)
                    CALL(evt_msg_print_add, 0, STRING("stg5_muj_93_03_02"))
                    CASE_EQUAL(2)
                    CALL(evt_msg_print_add, 0, STRING("stg5_muj_93_03_03"))
                    CALL(evt_item_entry, STRING("jewel"), ITEM_SKULL_GEM, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
                    CALL(evt_item_flag_on, STRING("jewel"), ITEM_FLAG_NO_TOUCH_GET)
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
        CASE_EQUAL(STG5_19_GEMS_AND_JINGLES)
        CALL(evt_msg_print, 0, STRING("stg5_muj_93_09"), 0, STRING("マルコ"))
    END_SWITCH
    RETURN
EVT_END()

/** Regular event of the Embers エルモス１ and エルモス２: it only waits forever, so they do nothing on their own. */
static EVT_BEGIN(erumosu_regl)
    WAIT_WHILE(1)
    RETURN
EVT_END()

EVT_BEGIN(erumosu_a_dead)
    SET_VALUE(GSWF_MUJ_05_EMBERS_DEFEATED, 1)
    DELETE_EVENT(GW(0))
    DELETE_EVENT(GW(2))
    CALL(evt_npc_set_position, STRING("エルモス１"), 0, -1000, 0)
    CALL(evt_run_case_evt, CASE_HAMMER, 1, STRING("A_ki_02_01"), 0, PTR(sanders_kyushutsu_sub), 0)
    RETURN
EVT_END()

/** Death event of the second Ember (Elmos B): stops its circling events. */
EVT_BEGIN(erumosu_b_dead)
    DELETE_EVENT(GW(1))
    DELETE_EVENT(GW(3))
    CALL(evt_npc_set_position, STRING("エルモス２"), 0, -1000, 0)
    RETURN
EVT_END()

/** NPCs of the skull rock before the Embers are beaten: Bobbery, Flavio and the two Embers. */
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

/** NPCs of the skull rock once the Embers have been beaten: Bobbery and Flavio. */
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

/** Doors and exits of muj_05: w_bero to muj_04; e_bero to dou_00 (Pirate's Grotto). */
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
        "muj_04", // nextMapName
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
        "dou_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

/** Hammering the tree A_ki_02_01: sways the palm (S_treeA_05) and plays its map animation. */
EVT_BEGIN(hammer_sub)
    CALL(evt_mapobj_get_position, STRING("S_treeA_05"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_TREE_SHAKE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_LEAF_SHAKE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("S_ki"), MAP_ANIM_PLAY_ONCE, 0)
    RETURN
EVT_END()

/** Registers hammer_sub as the hammer case event of the tree A_ki_02_01. */
EVT_BEGIN(hammer_event)
    CALL(evt_run_case_evt, CASE_HAMMER, 1, STRING("A_ki_02_01"), 0, PTR(hammer_sub), 0)
    RETURN
EVT_END()

/**
 * Rescue of Bobbery from the tree (STG5_15_RUNAWAY_SAILORS):
 * Bobbery thanks Mario (stg5_muj_33_01), the party walks to his side and the story advances.
 */
EVT_BEGIN(sanders_kyushutsu_sub)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_15_RUNAWAY_SAILORS)
    CALL(evt_msg_print, 0, STRING("stg5_muj_33_01"), 0, STRING("サンダース"))
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
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
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_ORGEL1"))
    CALL(evt_msg_print, 0, STRING("stg5_muj_34"), 0, STRING("サンダース"))
    CALL(evt_cam3d_evt_set, -105, 120, 324, -105, 62, 3, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg5_muj_34_01"), 0, STRING("me"))
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(1000)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_msg_print, 0, STRING("stg5_muj_34_02"), 0, STRING("サンダース"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_MUJ1"))
    SPAWN_EVENT(PTR(hammer_event))
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/** Makes an Ember circle around the point (-115, -15) at 5 degrees per frame for three seconds (muj_npc_guruguru). */
EVT_BEGIN(erumosu_guruguru)
    LOOP(0)
        CALL(muj_npc_guruguru, LW(0), -115, -15, 1, FLOAT(-5.0f), 3000)
    END_LOOP
    RETURN
EVT_END()

/** Keeps the Ember in LW(0) facing Bobbery (Sanders) while it circles. */
EVT_BEGIN(erumosu_guruguru_dir)
    LOOP(0)
        CALL(evt_set_dir_to_target, LW(0), STRING("サンダース"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Starts the Ember fight near Bobbery: unless the Embers are already defeated,
 * links Embers 1 and 2 as a battle pair (battle info 8) and starts them circling.
 */
EVT_BEGIN(sanders_kyushutsu)
    CALL(evt_npc_set_position, STRING("サンダース"), -88, 200, -34)
    IF_INT_EQUAL(GSWF_MUJ_05_EMBERS_DEFEATED, 0)
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
        CALL(evt_run_case_evt, CASE_HAMMER, 1, STRING("A_ki_02_01"), 0, PTR(sanders_kyushutsu_sub), 0)
    END_IF
    RETURN
EVT_END()

/**
 * Flavio's humming event (hanauta), tracked in GW(2) and deleted when he is talked to;
 * its body is only RETURN, so it does nothing.
 */
EVT_BEGIN(marco_hanauta)
    RETURN
EVT_END()

/**
 * Flavio reaches the skull rock (STG5_18_02_FLAVIOS_FAVOR_2):
 * once Mario passes x = 500, Flavio (following as the extra party member) steps forward
 * and says this must be the skull rock Bobbery spoke of (stg5_muj_93_00),
 * walks back to (279, -67) and stays there as an NPC;
 * he leaves the party and the story moves to STG5_18_03_MARCOS_JEWEL.
 */
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
    SET_VALUE(GSW_STORY_PROGRESS, STG5_18_03_MARCOS_JEWEL)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Flashes the blue eye of the skull rock (S_hige_iwa03_me) twice with a sparkle effect. */
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

/** Flashes the red eye of the skull rock (S_hige_iwa02_me) twice with a sparkle effect. */
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

/**
 * Flashes the skull rock's face (S_dkr_iwa02) with random jitter and a hagetaka flush effect;
 * answer_event spawns it five times.
 */
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

/** The correct sequence of the seven eye strikes on the skull rock, in order. */
s32 command_seikai[] = {
    2, 2, 2, 1, 1, 1, 1,
};

/** Items the player may offer at the skull rock: the Skull Gem only, ended by -1. */
static s32 itemsel_table[] = {
    ITEM_SKULL_GEM, -1,
};

/**
 * Touching the skull rock: if Mario has no Skull Gem he reads the inscription (stg5_muj_93_01);
 * with the gem he can pick it from the item window and place it in the rock,
 * which flashes both eyes and sets GSWF_MUJ05_SKULL_GEM_PLACED.
 */
EVT_BEGIN(dokuroiwa_touch)
    CALL(evt_pouch_check_item, ITEM_SKULL_GEM, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg5_muj_93_01"), 0, 0)
#if VERSION <= VERSION_DEMO_US
        IF_INT_GREATER_OR_EQUAL(GSW(GSW_STORY_PROGRESS), STG5_18_02_FLAVIOS_FAVOR_2)
#else
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_18_02_FLAVIOS_FAVOR_2)
            CALL(evt_party_stop, 0)
#endif
            CALL(evt_msg_print_party, STRING("stg5_muj_93_01_01"))
#if VERSION <= VERSION_DEMO_US
SET_VALUE(GSWF_MUJ_05_SKULL_ROCK_EXAMINED, 1)
#else
            CALL(evt_party_run, 0)
            SET_VALUE(GSWF_MUJ_05_SKULL_ROCK_EXAMINED, 1)
#endif
        END_IF
        CALL(evt_mario_key_onoff, 1)
        ELSE
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, PTR(itemsel_table), LW(0))
#else
        CALL(evt_win_item_select, 0, PTR(itemsel_table), LW(0), 0)
#endif
        IF_INT_EQUAL(LW(0), -1)
            RETURN
        END_IF
        CALL(evt_mapobj_get_position, STRING("S_jel"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_JUWEL_SET1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_jel"), MAP_OBJ_FLAG_HIDDEN)
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
#if VERSION <= VERSION_DEMO_US
            SPAWN_EVENT(blueeye_flush)
            SPAWN_EVENT(redeye_flush)
#else
            SPAWN_EVENT(PTR(blueeye_flush))
            SPAWN_EVENT(PTR(redeye_flush))
#endif
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, FLOAT(0.0f), FLOAT(1.0f), 500)
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
        CALL(evt_mario_dump_item, ITEM_SKULL_GEM)
#if VERSION <= VERSION_DEMO_US
        SET_VALUE(GSWF_MUJ_05_SKULL_GEM_PLACED, 1)
#else
        SET_VALUE(GSWF_MUJ_05_SKULL_GEM_PLACED, 1)
#endif
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

/**
 * Registers the hammer, jump-from-A and stand-on case events on the two eye rocks (A_iwa01 and A_iwa_atari),
 * saving their ids in GW(5) to GW(9).
 */
EVT_BEGIN(case_event_entry)
    SET_VALUE(GF(10), 0)
    CALL(evt_run_case_evt, CASE_HAMMER, 1, STRING("A_iwa01"), 0, PTR(iwa01_hammer), GW(5))
    CALL(evt_run_case_evt, CASE_JUMP_FROM_A, 1, STRING("A_iwa01"), 0, PTR(iwa01_ride_a), GW(6))
    CALL(evt_run_case_evt, CASE_STAND_ON, 1, STRING("A_iwa_atari"), 0, PTR(iwa02_ride), GW(7))
    CALL(evt_run_case_evt, CASE_HAMMER, 1, STRING("A_iwa_atari"), 0, PTR(iwa02_hammer), GW(8))
    CALL(evt_run_case_evt, CASE_JUMP_FROM_A, 1, STRING("A_iwa_atari"), 0, PTR(iwa02_ride_a), GW(9))
    RETURN
EVT_END()

/** Removes the case events registered by case_event_entry whose ids are still in GW(5) to GW(9). */
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

/**
 * The penalty for a wrong eye sequence: Flavio's humming stops,
 * the three Embers (A, B, C) glide down beside Mario,
 * he turns to them with a "!" bubble and fights them (battle info 7).
 * Afterwards they are hidden, the sequence is reset, GW(12) is set so Flavio comments on the failure,
 * the humming restarts and the case events are registered again.
 */
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
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

/**
 * Builds up the skull rock's opening shake: shakes the 3D camera sideways with a strength rising from 0 to 0.01
 * over 30 steps; answer_event runs it during the opening and deletes it.
 */
static EVT_BEGIN(cam_shake)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 10, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 0)
    RETURN
EVT_END()

/**
 * Winds down the skull rock's opening shake: the sideways strength falls from 0.01 to 0 over 30 steps;
 * answer_event runs it after cam_shake.
 */
static EVT_BEGIN(cam_shake2)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 10, 0, 30)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 100)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

/**
 * Correct skull rock answer: Mario is frozen, the rock flashes five times,
 * the camera shakes and the rock opens (GSWF_MUJ05_SKULL_ROCK_OPENED).
 */
EVT_BEGIN(answer_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, 525, 119, 584, 525, 23, 42, 500, EASING_SINE_IN_OUT)
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
    CALL(evt_cam3d_evt_set, 357, 125, 335, 551, 69, 90, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    DELETE_EVENT(LW(10))
    CALL_CHILD(PTR(cam_shake2))
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_mapobj_get_position, STRING("S_dokuro_iwa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_ROCK_WINDOW1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("S-dkr_iwa"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa"), 1, 0)
    CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa02"), 1, 1)
    CALL(evt_map_checkanim, STRING("S-dkr_iwa"), LW(0), LW(1))
    WAIT_MS(LW(1))
    WAIT_MS(1000)
    SET_VALUE(GSWF_MUJ_05_SKULL_ROCK_OPENED, 1)
    CALL_CHILD(PTR(case_event_delete))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Hammering the big rock with the blue eye (A_iwa01):
 * plays the rock hit sound and, while the Skull Gem is in place,
 * flashes the blue eye and records a blue strike (1), then checks the sequence:
 * a wrong one starts penalty_event, a complete one starts answer_event.
 */
EVT_BEGIN(iwa01_hammer)
    IF_INT_EQUAL(GF(10), 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_GEM_PLACED, 0)
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

/** Jumping onto the big blue-eye rock with A: plays the rock jump sound while the Skull Gem is not yet placed. */
EVT_BEGIN(iwa01_ride_a)
    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_GEM_PLACED, 0)
        RETURN
    END_IF
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa03_me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_BIG_ROCK_JUMP1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

/**
 * Landing with a ground pound (hip bump) on the red-eye rock (A_iwa_atari):
 * once per landing (GW(13)), flashes the red eye and records a red strike (2),
 * then checks the sequence like iwa01_hammer.
 */
EVT_BEGIN(iwa02_ride)
    CALL(evt_mario_chk_hipbump, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GW(13), 1)
        RETURN
    END_IF
    SET_VALUE(GW(13), 1)
    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_GEM_PLACED, 0)
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

/** Hammering the small red-eye rock: plays the small rock hit sound while the Skull Gem is not yet placed. */
EVT_BEGIN(iwa02_hammer)
    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_GEM_PLACED, 0)
        RETURN
    END_IF
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa02_me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_SMALL_ROCK_HIT1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

/**
 * Jumping onto the red-eye rock with A after a hip bump:
 * plays the small rock jump sound while the Skull Gem is not yet placed.
 */
EVT_BEGIN(iwa02_ride_a)
    CALL(evt_mario_chk_hipbump, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_GEM_PLACED, 0)
        RETURN
    END_IF
    CALL(evt_mapobj_get_position, STRING("S_hige_iwa02_me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_SMALL_ROCK_JUMP1"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

/** Clears GW(13) when Mario leaves the red-eye rock, so the next ground pound counts as a new strike. */
EVT_BEGIN(iwa02_nonride_a)
    SET_VALUE(GW(13), 0)
    RETURN
EVT_END()

/**
 * Breaks the skull rock (S_dokuro_iwa) with the bomb-wall paper effect:
 * plays the wall-break sound, captures the rock into an off-screen image
 * and plays the OFF_d_bom_ver2 animation A_1 on it, hides the rock and turns off its hit (A_dokuro_iwa),
 * then waits until no partner action is running before giving control back.
 */
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
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_dokuro_iwa"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_dokuro_iwa"), 1, 0)
        CALL(evt_img_wait_animend, STRING("capture_img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_dokuro_iwa"), MAP_OBJ_FLAG_HIDDEN)
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

/**
 * Bobbery's bomb at the opened skull rock (case event on A_dkr_iwa_02_oku_01):
 * opens the cave entrance e_bero, plays the success jingle,
 * shakes the camera and breaks the rock with bombwall_bomb.
 * Flavio stops humming, jumps for joy at the cave entrance (stg5_muj_93_08),
 * then the Skull Gem falls out and bounces; he picks it up (stg5_muj_93_08_01),
 * says he must head back to camp (stg5_muj_93_08_02) and leaves,
 * and the story moves to STG5_19_GEMS_AND_JINGLES.
 */
EVT_BEGIN(dokuroiwa_bomb)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_onoff, STRING("e_bero"), 1, 1)
    CALL(evt_mapobj_get_position, STRING("S_dokuro_iwa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_ROCK_BREAK1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
        WAIT_MS(2000)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.029296875f), FLOAT(0.0f), 1000)
    END_INLINE_EVENT
    CALL_CHILD(PTR(bombwall_bomb))
    CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa02"), 1, 0)
    IF_INT_EQUAL(GSWF_MUJ_05_FLAVIO_HUMMING_STOPPED, 0)
        DELETE_EVENT(GW(2))
        CALL(evt_msg_close, STRING("stg5_muj_93_02_00"))
    END_IF
    CALL(evt_npc_set_ry, STRING("マルコ"), 90)
    CALL(evt_cam3d_evt_set, 280, 115, 333, 280, 55, -11, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("マルコ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_get_position, STRING("マルコ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position_nohit, STRING("マルコ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_npc_jump_position_nohit, STRING("マルコ"), LW(0), LW(1), LW(2), 225, 30)
    CALL(evt_msg_print, 0, STRING("stg5_muj_93_08"), 0, STRING("マルコ"))
    CALL(evt_item_entry, STRING("jewel00"), ITEM_SKULL_GEM, 225, 200, -25, ITEM_TYPE_FALL, -1, 0)
    SET_VALUE(LW(8), STRING("jewel00"))
    CALL(evt_item_flag_on, LW(8), ITEM_FLAG_CUSTOM_BOUND)
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
    CALL(evt_cam3d_evt_set, 225, 115, 333, 225, 55, -11, 500, EASING_SINE_IN_OUT)
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
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG5_19_GEMS_AND_JINGLES)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Initialises the skull rock: hides the jewel, closes the lid hit box, clears the puzzle buffer and,
 * once STG5_19_GEMS_AND_JINGLES is reached, hides the rock and opens the exit hit box.
 */
EVT_BEGIN(dokuroiwa_init)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_jel"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa02"), 1, 0)
    CALL(evt_dokuroiwa_command_init)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_19_GEMS_AND_JINGLES)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_dokuro_iwa"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_dokuro_iwa"), 1, 0)
        RETURN
    END_IF
    CALL(evt_hitobj_onoff, STRING("e_bero"), 1, 0)
    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_GEM_PLACED, 0)
        CALL(evt_mapobj_scale, 1, STRING("S_hige_iwa02_me"), 1, 0, 1)
        CALL(evt_mapobj_scale, 1, STRING("S_hige_iwa03_me"), 1, 0, 1)
        ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_jel"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    SET_VALUE(GW(10), 0)
    SET_VALUE(GW(12), 0)
    SET_VALUE(GW(13), 0)
    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_GEM_PLACED, 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_dkr_iwa_01"), 0, PTR(dokuroiwa_touch), 0)
    END_IF
    IF_INT_EQUAL(GSWF_MUJ_05_SKULL_ROCK_OPENED, 0)
        CALL_CHILD(PTR(case_event_entry))
        CALL(evt_run_case_evt, CASE_NOT_STAND_ON, 1, STRING("A_iwa_atari"), 0, PTR(iwa02_nonride_a), 0)
        ELSE
        CALL(evt_map_playanim, STRING("S-dkr_iwa"), MAP_ANIM_PLAY_AT_END, 0)
        CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_dkr_iwa_futa02"), 1, 1)
    END_IF
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("A_dkr_iwa_02_oku_01"), 0, PTR(dokuroiwa_bomb), 0)
    RETURN
EVT_END()

EVT_BEGIN(muj_05_first)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
    CALL(evt_mario_key_onoff, 0)
#if VERSION > VERSION_DEMO_US
    CALL(evt_bero_exec_wait, 65536)
#endif
    CALL(evt_mario_normalize)
#if VERSION <= VERSION_DEMO_US
CALL(evt_bero_exec_wait, 65536)
#endif
    CALL(evt_msg_print, 0, STRING("stg5_muj_33"), 0, STRING("サンダース"))
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(GSWF_MUJ_05_BOBBERY_FIRST_SCENE_SEEN, 1)
#else
    SET_VALUE(GSWF_MUJ_05_BOBBERY_FIRST_SCENE_SEEN, 1)
#endif
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Sways the leaves of the palm tree (S_treeA_05) back and forth, spawned by yure_yashi. */
EVT_BEGIN(yure_yashi_ha1)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 2, 500)
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
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_15_RUNAWAY_SAILORS)
            RETURN
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN, 2, 0, 2000)
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
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_15_RUNAWAY_SAILORS)
            RETURN
        END_IF
    END_LOOP
    RETURN
EVT_END()

/**
 * Sways the palm tree (S_treeA_05) and its leaves when it is hit;
 * after STG5_15_RUNAWAY_SAILORS it keeps swaying until the tree is stopped.
 */
EVT_BEGIN(yure_yashi)
    LOOP(0)
        CALL(evt_mapobj_get_position, STRING("S_treeA_05"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG5_OBJ_TREE_SHAKE2"), LW(0), LW(1), LW(2), 0)
        SPAWN_EVENT(PTR(yure_yashi_ha1))
        WAIT_MS(100)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 1, -1, 800)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_treeA_05"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_15_RUNAWAY_SAILORS)
                RETURN
            END_IF
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1, 1, 2000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_treeA_05"), 0, 0, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_15_RUNAWAY_SAILORS)
                RETURN
            END_IF
        END_LOOP
        WAIT_MS(500)
    END_LOOP
    RETURN
EVT_END()

/**
 * Case event for grass patch 1: when the grass is hit,
 * launches a Spite Pouch out of it unless it has been taken (item_kusa_1).
 */
static EVT_BEGIN(evt_kusa_1)
    CALL(evt_item_delete_check, STRING("item_kusa_1"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_item_entry, STRING("item_kusa_1"), ITEM_SPITE_POUCH, 235, 0, 90, ITEM_TYPE_SCATTER_NO_PICKUP, GSWF_MUJ_05_SPITE_POUCH_BUSH, 0)
    END_IF
    RETURN
EVT_END()

/** Trees of muj_05 that Mario can shake with his hammer (5 in all). */
static TreeAccess tree_tbl[] = {
    {5, "A_treeA_01", "treeA_01_m", "treeA_01_ha", 0, NULL},
    {5, "A_treeA_02", "treeA_02_m", "treeA_02_ha", 0, NULL},
    {5, "A_treeA_03", "treeA_03_m", "treeA_03_ha", 0, NULL},
    {5, "A_treeA_04", "treeA_04_m", "treeA_04_ha", 0, NULL},
    {5, "A_treeB_03", "treeB_03_m", "treeB_03_ha", 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

/** Bushes of muj_05 that Mario can check for hidden things (7 in all); A_kuwa_move_01 runs evt_kusa_1. */
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

/**
 * Map init for muj_05 (the skull rock): picks music
 * and ambience by story stage (danger music while Bobbery is stuck), sets up the Embers, Bobbery,
 * the skull rock puzzle and the exits.
 */
EVT_BEGIN(muj_05_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_13_BOBBERY_THE_BRIDGE)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_DANGER4"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ7"))
        CASE_EQUAL(STG5_18_BOBBERY_RESCUE_2)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ7"))
        CASE_DEFAULT
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG5_MUJ1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG5_MUJ7"))
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_EQUAL(GSWF_MUJ_05_EMBERS_DEFEATED, 0)
        CALL(evt_npc_setup, PTR(npcEnt))
    ELSE
        CALL(evt_npc_setup, PTR(npcEnt_after))
    END_IF
    CALL(evt_npc_sound_data_set, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("SFX_STG5_MARCO_MOVE1L"), STRING("SFX_STG5_MARCO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_map_playanim, STRING("S_mizu"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(hammer_event)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG5_13_BOBBERY_THE_BRIDGE)
        IF_INT_EQUAL(GSWF_MUJ_05_BOBBERY_FIRST_SCENE_SEEN, 0)
            SPAWN_EVENT(muj_05_first)
        END_IF
        SPAWN_EVENT(sanders_kyushutsu)
        SPAWN_EVENT(yure_yashi)
        CASE_EQUAL(STG5_18_02_FLAVIOS_FAVOR_2)
        SPAWN_EVENT(marco_and_jewel)
    END_SWITCH
    SPAWN_EVENT(dokuroiwa_init)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG5_63_KEELHAUL_ANYTIME)
        CALL(evt_item_entry, STRING("item00"), ITEM_WEDDING_RING, -250, 0, -110, ITEM_TYPE_GROUND, GSWF_MUJ_05_WEDDING_RING, 0)
    END_IF
    CALL(evt_eff64, STRING("sunlight"), STRING("sunlight_n64"), 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mobj_recovery_blk, STRING("mobj_recover"), 12, 125, 60, -90, 0, 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, 395, -15, 120, ITEM_TYPE_STILL, GSWF_STARPIECE_76, 0)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(evt_sub_tree_access_entry)
    SET_VALUE(LW(0), PTR(kusa_tbl))
    CALL_CHILD(evt_sub_kusa_access_entry)
    RETURN
EVT_END()


// usu_01 (ウスグラタウン, Twilight Town): the east side of town with the shop, the villagers' houses and the crows; the gate
// leads to Twilight Trail (gra_00).

#include "version.h"
#include "evt/evt_story.h"
#include "evt/evt_urouro.h"
#include "evt/evt_window.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "driver/mapdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_case.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_lecture.h"
#include "evt/evt_pouch.h"
#include "system.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"

#include "mario/mario.h"
#include "driver/animdrv.h"
#include "pmario_sound.h"
#include "string.h"
#include "mario/mario_pouch.h"
#include "memory.h"
#include "data/item_data.h"
#include "driver/npcdrv.h"
#include "driver/mobjdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_sub.h"
#include "usu.h"
#include "evt/evt_npc.h"
#include "evt/evt_shop.h"
#include "data_tables.h"
#include "driver/seqdrv.h"


static USER_FUNC(bgm_start_wait);
static USER_FUNC(booboo_main);
USER_FUNC(box_r);
USER_FUNC(check_tabemono);
USER_FUNC(chk_fuki);
USER_FUNC(delete_item_list);
USER_FUNC(get_item_list);
USER_FUNC(ie2_npc_disp_off);
USER_FUNC(ie2_npc_disp_on);
USER_FUNC(mobj_ry);
static USER_FUNC(npc_process);
static USER_FUNC(set_fook_evt);
USER_FUNC(shop_item_disp_off);
USER_FUNC(shop_item_disp_on);
USER_FUNC(sleep_voice);
static void majin_callback(s32 type);

EvtWord villagerF_init[7];
EvtWord villagerF_talk[52];
EvtWord villagerG_init[7];
EvtWord villagerG_talk[52];
EvtWord villagerH_init[7];
EvtWord villagerH_talk[44];
EvtWord villagerI_init[19];
EvtWord villagerI_pig_init[19];
EvtWord villagerI_talk[57];
EvtWord villagerJ_init[19];
EvtWord villagerJ_pig_init[19];
EvtWord villagerJ_talk[57];
EvtWord shop_man_init[36];
EvtWord shop_man_talk[28];
EvtWord shop_woman_init[7];
EvtWord shop_woman_talk[163];
static EvtWord kizayarou_init[38];
EvtWord kizayarou_pig_init[14];
static EvtWord kizayarou_talk[33];
EvtWord childABC_talk_01[28];
EvtWord childABC_talk_02[28];
EvtWord childABC_talk_03[28];
EvtWord childABC_talk_04[28];
EvtWord aunt_init[22];
EvtWord aunt_pig_init[29];
EvtWord iri_20_out_house[407];
EvtWord iri_20_aunt_to_house[149];
#if VERSION <= VERSION_DEMO_US
extern EvtWord aunt_talk[];
#else
EvtWord aunt_talk[687];
#endif
EvtWord childA_init[7];
EvtWord childA_talk[31];
EvtWord childB_init[7];
EvtWord childB_talk[31];
EvtWord childC_init[7];
EvtWord childC_talk[31];
EvtWord gatekeeper_init[38];
EvtWord gatekeeper_pig_init[33];
EvtWord gatekeeper_talk[145];
EvtWord crow_f_init[8];
EvtWord crow_g_init[8];
EvtWord crow_h_init[8];
EvtWord crow_i_init[8];
EvtWord crow_j_init[8];
static EvtWord musume_talk[84];
EvtWord musume_a_init[40];
static EvtWord musume_a_entry[15];
EvtWord musume_a_pig_entry[15];
EvtWord musume_b_init[40];
static EvtWord musume_b_entry[15];
EvtWord musume_b_pig_entry[15];
EvtWord musume_c_init[40];
static EvtWord musume_c_entry[15];
EvtWord musume_c_pig_entry[15];
static EvtWord urouro_move[24];
EvtWord urouro_move_semai[24];
static NpcSetupInfo npcEnt[24];
#if VERSION <= VERSION_DEMO_US
extern EvtWord village_chief_change_pig_event[];
#else
EvtWord village_chief_change_pig_event[166];
#endif
EvtWord evt_mon_open[112];
static EvtWord cam_up[34];
static EvtWord cam_down[8];
EvtWord usu01_check_vivian[36];
#if VERSION == VERSION_DEMO_JP
EvtWord usu01_crowFG_talk_event[347];
EvtWord usu01_crowHI_talk_event[246];
EvtWord usu01_crowJ_talk_event[222];
#else
EvtWord usu01_crowFG_talk_event[VERSION_PICK(355, 394)];
EvtWord usu01_crowHI_talk_event[VERSION_PICK(250, 289)];
EvtWord usu01_crowJ_talk_event[VERSION_PICK(226, 265)];
#endif
const char* minka_npc[6];
const char* minka_item[2];
const char* shop_npc[6];
const char* shop_mobj[4];
const char* shop_item[5];
static EvtWord evt_door_1_1[12];
static EvtWord evt_door_1_2[12];
static EvtWord evt_wall_1[28];
static EvtWord door_1_open[12];
static EvtWord door_1_close[14];
static DoorParam door_1;
EvtWord evt_door_2_1_1[15];
EvtWord evt_door_2_1_2[2];
EvtWord evt_wall_2_1[28];
EvtWord loop_msg[84];
EvtWord mise_kiza_msg[32];
EvtWord mawari_house_delay[46];
EvtWord door_2_1_open[23];
DoorParam door_2_1;
EvtWord evt_ie2_door2_1[244];
EvtWord evt_ie2_door2_2[226];
static EvtWord evt_door_3_1[12];
static EvtWord evt_door_3_2[12];
static EvtWord evt_wall_3[30];
const char* door_3_item[2];
static EvtWord door_3_open[12];
static EvtWord door_3_close[14];
static DoorParam door_3;
static BeroEntry bero_entry_data[3];
EvtWord usu01_gusun[80];
EvtWord evt_monban_buta_main[34];
EvtWord evt_monban_buta[18];
EvtWord evt_monban_buta2[17];
EvtWord evt_monban_look[119];
static s32 lock_tbl[2];
#if VERSION <= VERSION_DEMO_US
static EvtWord lock_item_check[39];
#else
static EvtWord lock_item_check[41];
#endif
EvtWord unlock[13];
static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[12];
static ShopperData35 shopper_data;
EvtWord shopper_talk1[11];
EvtWord shopper_talk2[11];
static BoobooName booboo_name[8];
EvtWord evt_roll_get[2];
static s32 key_tbl[2];
#if VERSION <= VERSION_DEMO_US
static EvtWord evt_majin[325];
static EvtWord evt_majin2[239];
#else
static EvtWord evt_majin[330];
static EvtWord evt_majin2[237];
#endif
EvtWord tree_onoff[57];
static EvtWord evt_ki_1[12];
static EvtWord evt_ki_2[20];
static EvtWord evt_ki_3[20];
static TreeAccess tree_tbl[5];
EvtWord usu_01_tree_init[7];
static EvtWord mobj_box00[34];
EvtWord usu_01_init_evt[350];

/** Init script of villager F: enables move/talk animation. */
EVT_BEGIN(villagerF_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｆ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/**
 * Talk script of villager F, the novelist: text by story progress.
 * Before the Doopliss fight he wonders whether pigs can read;
 * until Mario leaves Twilight Town he asks what books Mario likes (a four-way choice, one shared reply);
 * later lines are about his writing, his finished love story (until the end of the game)
 * and its film adaptation (after).
 */
EVT_BEGIN(villagerF_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_82"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_30"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("stg4_usu_30_yn"))
        CALL(evt_msg_print_add, 0, STRING("stg4_usu_30_00"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_84"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_84_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_84_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of villager G: enables move/talk animation. */
EVT_BEGIN(villagerG_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｇ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/**
 * Talk script of villager G: text by Chapter 4 progress (before the Doopliss fight, until leaving Twilight Town,
 * later).
 */
EVT_BEGIN(villagerG_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_85"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_86"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("stg4_usu_86_yn"))
        CALL(evt_msg_print_add, 0, STRING("stg4_usu_86_00"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_87"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_87_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_87_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of villager H: enables move/talk animation. */
EVT_BEGIN(villagerH_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｈ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/**
 * Talk script of villager H: text by Chapter 4 progress (before the Doopliss fight, until leaving Twilight Town,
 * later).
 */
EVT_BEGIN(villagerH_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_88"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_89"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_90"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_90_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_90_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of villager I (human form): enables move/talk animation and keeps him below the map
 * until the Doopliss fight (STG4_10).
 */
EVT_BEGIN(villagerI_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｉ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("村人Ｉ"), 0, -2000, 0)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

/** Init script of the pig form of villager I: shown until STG4_10, then moved below the map. */
EVT_BEGIN(villagerI_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｉ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村人Ｉ"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Talk script of villager I: an oink plus pig text before the Doopliss fight, then human text by story progress. */
EVT_BEGIN(villagerI_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_91"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_92"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_93"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_93_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_93_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of villager J (human form): enables move/talk animation and keeps him below the map
 * until the Doopliss fight (STG4_10).
 */
EVT_BEGIN(villagerJ_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｊ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("村人Ｊ"), 0, -2000, 0)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

/** Init script of the pig form of villager J: shown until STG4_10, then moved below the map. */
EVT_BEGIN(villagerJ_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｊ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村人Ｊ"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/** Talk script of villager J: an oink plus pig text before the Doopliss fight, then human text by story progress. */
EVT_BEGIN(villagerJ_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_94"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_95"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_96"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_96_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_96_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of the shopkeeper (the shop woman's husband): enables move/talk animation.
 * From the Doopliss fight until Mario leaves Twilight Town he stands in the shop at (-125, 0, -475),
 * turned to 90 degrees towards the dandy he quarrels with (see mise_kiza_msg);
 * later he stands at (-130, 0, -480). Before the fight his placement is left as set up.
 */
EVT_BEGIN(shop_man_init)
    CALL(evt_npc_flag_onoff, 1, STRING("店の主人"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_npc_set_position, STRING("店の主人"), -125, 0, -475)
        CALL(evt_npc_set_ry, STRING("店の主人"), 90)
        CALL(evt_npc_flag_onoff, 1, STRING("店の主人"), NPC_FLAG_RESUME_AFTER_TALK)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("店の主人"), -130, 0, -480)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of the shopkeeper: one line until Mario leaves Twilight Town (stg4_usu_61_5),
 * another after the story's end (stg4_usu_61_6).
 */
EVT_BEGIN(shop_man_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_61_5"), 0, STRING("店の主人"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_61_6"), 0, STRING("店の主人"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_61_7"), 0, STRING("店の主人"))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of the shopkeeper's wife: enables move/talk animation. */
EVT_BEGIN(shop_woman_init)
    CALL(evt_npc_flag_onoff, 1, STRING("お店の奥さん"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(shop_woman_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_03_SHOP_KEY)
        IF_INT_EQUAL(GSWF_USU_01_SHOP_WIFE_FIRST_TALK_DONE, 0)
            CALL(evt_msg_print, 0, STRING("stg4_usu_52"), 0, STRING("me"))
            WAIT_MS(200)
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(1000)
            CALL(evt_msg_print, 0, STRING("stg4_usu_53"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_I_N"))
            WAIT_MS(1000)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            BEGIN_INLINE_EVENT_GET_ID(LW(10))
                CALL(sleep_voice)
            END_INLINE_EVENT
            SET_VALUE(GF(0), 0)
            CALL(evt_msg_print, 0, STRING("stg4_usu_54"), 0, STRING("me"))
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_YAWN1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_mario_set_pose, STRING("M_I_5"))
                WAIT_MS(1000)
                CALL(evt_mario_set_pose, STRING("M_N_5A"))
                WAIT_MS(1000)
                SET_VALUE(GF(0), 1)
            END_INLINE_EVENT
            CALL(evt_msg_continue)
            LOOP(0)
                WAIT_FRAMES(1)
                IF_INT_EQUAL(GF(0), 1)
                    BREAK_LOOP
                END_IF
            END_LOOP
            DELETE_EVENT(LW(10))
            CALL(evt_snd_sfxoff, GW(10))
            WAIT_MS(200)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_AWAKE1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_set_pose, STRING("M_N_5B"))
            WAIT_MS(600)
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(600)
            CALL(evt_msg_print, 0, STRING("stg4_usu_55"), 0, STRING("me"))
            SET_VALUE(GSWF_USU_01_SHOP_WIFE_FIRST_TALK_DONE, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("stg4_usu_56"), 0, STRING("me"))
        END_IF
        CASE_LESS(STG4_04_STORAGE_ROOM_UNLOCKED)
        CALL(evt_msg_print, 0, STRING("stg4_usu_58"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_58_01"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_59"), 0, STRING("me"))
        CALL(evt_msg_continue)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of the dandy ("kizayarou", human form): enables move/talk animation.
 * He is kept below the map before the Doopliss fight (his pig form stands in instead)
 * and once Mario leaves Twilight Town; in between he stands in the shop at (-100, 0, -450).
 */
static EVT_BEGIN(kizayarou_init)
    CALL(evt_npc_flag_onoff, 1, STRING("キザ野郎"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("キザ野郎"), 0, -2000, 0)
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_npc_set_position, STRING("キザ野郎"), -100, 0, -450)
        CALL(evt_npc_flag_onoff, 1, STRING("キザ野郎"), NPC_FLAG_RESUME_AFTER_TALK)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("キザ野郎"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of the pig form of the dandy ("kizayarou") in the shop:
 * shown until STG4_10, then moved below the map.
 */
EVT_BEGIN(kizayarou_pig_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタキザ野郎"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of the dandy, shared by his pig form:
 * before the Doopliss fight an oink sound and a small "L'oink" line (stg4_usu_61_00);
 * afterwards he says, in his French accent, that he came by the pipe
 * and does not know why he is in the shop (stg4_usu_61).
 */
static EVT_BEGIN(kizayarou_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_61_00"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_61"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Group chat of the three children (A, B and C) with staggered messages stg4_usu_63 and up;
 * used while the children still have their mother.
 */
EVT_BEGIN(childABC_talk_01)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg4_usu_63"), 0, STRING("子供Ａ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_msg_print, 0, STRING("stg4_usu_64"), 0, STRING("子供Ｂ"))
    END_INLINE_EVENT
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_usu_65"), 0, STRING("子供Ｃ"))
    RETURN
EVT_END()

/**
 * Group chat of the three children, staggered by 400 ms and 600 ms:
 * small lines saying they miss their mommy, are hungry and want pork chops
 * (stg4_usu_79 to stg4_usu_81); used from the Doopliss fight until STG4_31_FRANKLYS_HOUSE_STG_4.
 */
EVT_BEGIN(childABC_talk_02)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg4_usu_79"), 0, STRING("子供Ａ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_msg_print, 0, STRING("stg4_usu_80"), 0, STRING("子供Ｂ"))
    END_INLINE_EVENT
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_usu_81"), 0, STRING("子供Ｃ"))
    RETURN
EVT_END()

/**
 * Group chat of the three children, staggered the same way: they greet Mario, say he is back and ask for a gift
 * (stg4_usu_81_2 to stg4_usu_81_4); used from STG4_31_FRANKLYS_HOUSE_STG_4 until the end of the game.
 */
EVT_BEGIN(childABC_talk_03)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg4_usu_81_2"), 0, STRING("子供Ａ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_msg_print, 0, STRING("stg4_usu_81_3"), 0, STRING("子供Ｂ"))
    END_INLINE_EVENT
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_usu_81_4"), 0, STRING("子供Ｃ"))
    RETURN
EVT_END()

/**
 * Group chat of the three children with the same greeting, welcome-back and gift lines as childABC_talk_03
 * (stg4_usu_81_4_2 to stg4_usu_81_4_4); used after the end of the game.
 */
EVT_BEGIN(childABC_talk_04)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg4_usu_81_4_2"), 0, STRING("子供Ａ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_msg_print, 0, STRING("stg4_usu_81_4_3"), 0, STRING("子供Ｂ"))
    END_INLINE_EVENT
    WAIT_MS(600)
    CALL(evt_msg_print, 0, STRING("stg4_usu_81_4_4"), 0, STRING("子供Ｃ"))
    RETURN
EVT_END()

EVT_BEGIN(aunt_init)
    CALL(evt_npc_flag_onoff, 1, STRING("おばさん"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        IF_INT_EQUAL(GSWF_USU_01_AUNT_TURNED_INTO_PIG, 1)
            CALL(evt_npc_set_position, STRING("おばさん"), 0, -2000, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(aunt_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタおばさん"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        IF_INT_EQUAL(GSWF_USU_01_AUNT_TURNED_INTO_PIG, 0)
            CALL(evt_npc_set_position, STRING("ブタおばさん"), 0, -2000, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタおばさん"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Trouble 20 helper (door in LW(0)): with Mario's controls off, pans the camera,
 * opens the house door and walks the aunt out to (-368, 0) facing Mario,
 * while Mario and the partner follow her out to -420 and -450;
 * then closes the door, returns the camera and restores the house's map objects.
 */
EVT_BEGIN(iri_20_out_house)
    // Mario's controls are off and the camera swings to a spot in front of the house.
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set_xyz, -420, 1, 0, 0, 150, EASING_SINE_IN_OUT)
    END_INLINE_EVENT

    // Set the door up for the exit: its glow and sound start and the door parts swing to their starting
    // position.
    IF_INT_EQUAL(LW(14), 0)
        SPAWN_EVENT(PTR(door_light2))
    END_IF
    SPAWN_EVENT(PTR(door_up_se))
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 180, 0, 500)
            LABEL(3)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LF(0), 1)
            SPAWN_EVENT(LW(8))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(3)
            END_IF
        END_INLINE_EVENT
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(9), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
            SPAWN_EVENT(PTR(house_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            SET_VALUE(LW(6), 0)
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 180, 0, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 3)
            SET_VALUE(LF(0), 1)
            SPAWN_EVENT(LW(9))
            SET_VALUE(LW(6), 1)
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(2)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)

    // A moment later the door opens: the glow, the opening sound and the door parts swinging open.
    IF_INT_EQUAL(LW(14), 0)
        SPAWN_EVENT(PTR(door_light))
    END_IF
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 2)
            SET_VALUE(LF(0), 1)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(1)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)

    // The aunt walks out of the door, along the path to (-368, 0), and turns toward Mario;
    // Mario and his partner follow her out to x -420 and -450 and face her.
    CALL(evt_door_param, LW(0), DOOR_PARAM_OUT_POS, LW(1), LW(2), LW(3))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("おばさん"), LW(1), LW(3), 0, 60, 0)
        CALL(evt_npc_move_position, STRING("おばさん"), -368, 0, 0, 60, 0)
        CALL(evt_npc_set_ry, STRING("おばさん"), 270)
        WAIT_MS(300)
    END_INLINE_EVENT
    CALL(evt_mario_party_door_move, LW(1), LW(3), 60)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -420, 0, 60)
        CALL(evt_mario_set_dir_npc, STRING("おばさん"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -450, -1, 60)
        CALL(evt_party_set_dir_npc, 0, STRING("おばさん"))
    END_INLINE_EVENT

    // After a long pause the door closes again with its sound.
    WAIT_MS(2000)
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(PTR(door_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 180, 0, 500)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 3)
            SET_VALUE(LF(0), 1)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(0)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)

    // The camera returns, the door's map objects are restored and Mario's controls come back.
    CALL(evt_cam3d_evt_xyz_off, 100, EASING_SINE_IN_OUT)
    CALL(door_dark_flag, LW(0), 0)
    CALL(evt_door_param, LW(0), DOOR_PARAM_INSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        IF_INT_NOT_EQUAL(LW(15), 1)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_OUTSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_HITS, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    CALL(evt_door_param, LW(0), DOOR_PARAM_OUTSIDE, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_load_mapflag, LW(1))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Trouble 20 helper: the aunt walks back to house 1's door, which opens; she walks inside to (-390, -185)
 * and the door closes behind her.
 */
EVT_BEGIN(iri_20_aunt_to_house)
    CALL(evt_npc_move_position, STRING("おばさん"), -390, -40, 0, 60, 0)
    SET_VALUE(LW(0), PTR(&door_1))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_move_position, STRING("me"), -390, -20, 0, 60, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 500)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 0)
            SET_VALUE(LF(0), 0)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(0)
            END_IF
        END_INLINE_EVENT
    END_IF
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("おばさん"), -390, -185, 0, 60, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(PTR(door_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), DOOR_PARAM_EVTS, LW(7), LW(8), LW(9))
            CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 180, 0, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(LW(5), LW(1))
            SET_VALUE(LW(1), 1)
            SET_VALUE(LF(0), 0)
            SPAWN_EVENT(LW(7))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(5), 1)
                GOTO(1)
            END_IF
        END_INLINE_EVENT
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(aunt_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_TELL_THAT_PERSON)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            // Trouble 20 (Podley's old flame): the aunt asks Mario to step outside because she can't talk in
            // front of the children (irai_20_001).
            CALL(evt_msg_print, 0, STRING("irai_20_001"), 0, STRING("me"))

            // She and Mario move into position by her door and she is walked out
            // through it (iri_20_out_house).
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
            BEGIN_INLINE_EVENT
                CALL(evt_npc_move_position, STRING("me"), -390, -130, 0, 60, 0)
            END_INLINE_EVENT
            CALL(evt_mario_mov_pos2, -390, -100, 60)
            WAIT_MS(500)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
            SET_VALUE(LW(0), PTR(&door_1))
            SET_VALUE(LW(14), 0)
            SET_VALUE(LW(15), 0)
            CALL_CHILD(PTR(iri_20_out_house))

            // The town music fades out and music-box music takes over.
            CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
            WAIT_MS(1000)
#if VERSION > VERSION_DEMO_US
            BEGIN_INLINE_EVENT
                WAIT_MS(2000)
#endif
                CALL(evt_snd_bgmoff, PSND_PAUSE)
                CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_ORGEL1"))
#if VERSION > VERSION_DEMO_US
            END_INLINE_EVENT

// She explains she wants to meet her former love Podley (irai_20_002);
// "Purehearted Eve" was her stage name (irai_20_003).
#endif
            CALL(evt_msg_print, 0, STRING("irai_20_002"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_N_1"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("irai_20_003"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_S_1"))
#if VERSION <= VERSION_DEMO_US
            CALL_CHILD(iri_20_aunt_to_house)
#else
            CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 3000)
            CALL_CHILD(PTR(iri_20_aunt_to_house))
#endif
            CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
            CALL(evt_snd_bgmon_f, PSND_CONTINUE | PSND_FADEIN, 0, 2000)
#if VERSION > VERSION_DEMO_US
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
            CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
            CALL(evt_snd_env_lpf, 0, 0)
#endif
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
        ELSE
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                // Mario hasn't met Podley yet: she repeats the request (irai_20_004).
                CALL(evt_msg_print, 0, STRING("irai_20_004"), 0, STRING("me"))
            ELSE
                // Mario has met Podley: he and his partner walk up to the aunt and the
                // three children gather round.
                CALL(evt_npc_flag_onoff, 1, STRING("おばさん"), NPC_FLAG_NO_HIT)
                CALL(evt_npc_flag_onoff, 1, STRING("子供Ｂ"), NPC_FLAG_NO_HIT)
                SET_VALUE(GF(0), 1)
                BEGIN_INLINE_EVENT
                    CALL(evt_party_move_pos2, 0, -443, -140, 60)
                    CALL(evt_party_set_dir_npc, 0, STRING("おばさん"))
                    WAIT_MS(300)
                    SET_VALUE(GF(0), 0)
                END_INLINE_EVENT
                CALL(evt_mario_mov_pos2, -415, -150, 60)
                CALL(evt_mario_set_dir_npc, STRING("おばさん"))
                WAIT_MS(400)
                CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
                WAIT_FRAMES(1)
                CALL(evt_npc_wait_pera, STRING("me"))
                WAIT_WHILE(GF(0))
                CALL(evt_npc_flag_onoff, 0, STRING("おばさん"), NPC_FLAG_NO_HIT)
                CALL(evt_npc_flag_onoff, 0, STRING("子供Ｂ"), NPC_FLAG_NO_HIT)

                // She asks how Podley feels about her (irai_20_005);
                // Mario can tell her the truth (irai_20_007) or make something up (irai_20_008).
                CALL(evt_msg_print, 0, STRING("irai_20_005"), 0, STRING("me"))
                CALL(evt_msg_select, 0, STRING("irai_20_006"))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("irai_20_007"))
                ELSE
                    CALL(evt_msg_continue)
                    CALL(evt_mario_set_pose, STRING("M_I_2"))
                    WAIT_MS(1200)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_msg_print, 0, STRING("irai_20_008"), 0, STRING("me"))
                END_IF

                // The children clamor for food (irai_20_009 to 011).
                BEGIN_INLINE_EVENT
                    CALL(evt_msg_print, 0, STRING("irai_20_009"), 0, STRING("子供Ａ"))
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    WAIT_MS(500)
                    CALL(evt_msg_print, 0, STRING("irai_20_010"), 0, STRING("子供Ｂ"))
                END_INLINE_EVENT
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("irai_20_011"), 0, STRING("子供Ｃ"))

                // She announces a feast and gives Mario a Meteor Meal (irai_20_012) if his items have room
                // (10 slots, 20 with the Strange Sack).
                CALL(evt_msg_print, 0, STRING("irai_20_012"), 0, STRING("me"))
                CALL(evt_pouch_get_haveitemcnt, LW(0))
                CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
                IF_INT_EQUAL(LW(1), 0)
                    SET_VALUE(LW(1), HELD_ITEM_COUNT_BASE)
                ELSE
                    SET_VALUE(LW(1), MAX_HELD_ITEMS)
                END_IF
                IF_INT_LESS(LW(0), LW(1))
                    CALL(evt_msg_continue)
                    CALL(evt_item_entry, STRING("sd"), ITEM_METEOR_MEAL, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                    CALL(evt_item_get_item, STRING("sd"))
                    CALL(evt_item_get_item_end_wait, STRING("sd"))
                    SET_VALUE(GSWF_USU_01_METEOR_MEAL, 1)
                ELSE
                    // Items full: she leaves the Meteor Meal on the ground for him (irai_20_012_1).
                    CALL(evt_msg_print_add, 0, STRING("irai_20_012_1"))
                    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                    CALL(evt_npc_move_position, STRING("me"), -363, -152, 0, 40, 0)
                    WAIT_MS(300)
                    CALL(evt_item_entry, STRING("iri_20"), ITEM_METEOR_MEAL, -345, 20, -132, ITEM_TYPE_STILL, GSWF_USU_01_METEOR_MEAL, 0)
                    WAIT_MS(300)
                    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, 40, 0)
                    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
                END_IF

                // She thanks Mario (irai_20_014), the playtime is saved,
                // the trouble is solved and the children cheer (irai_20_015 to 017).
                CALL(evt_msg_print, 0, STRING("irai_20_014"), 0, STRING("me"))
                CALL(evt_sub_save_playtime, GSW_USU_TROUBLE_20_DONE_TIME)
                SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_TELL_THAT_PERSON), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                BEGIN_INLINE_EVENT
                    CALL(evt_msg_print, 0, STRING("irai_20_015"), 0, STRING("子供Ａ"))
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    WAIT_MS(500)
                    CALL(evt_msg_print, 0, STRING("irai_20_016"), 0, STRING("子供Ｂ"))
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    WAIT_MS(1000)
                    CALL(evt_msg_print, 0, STRING("irai_20_017"), 0, STRING("子供Ｃ"))
                END_INLINE_EVENT
            END_IF
        END_IF
        RETURN
    END_IF

    // No trouble active: her words depend on how far the story has gone.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        IF_INT_EQUAL(GSWF_USU_01_AUNT_TURNED_INTO_PIG, 0)
            // Before the Doopliss fight, first talk: she says she can't buy food for the children since her
            // husband turned into a pig (stg4_usu_62), and they pipe up.
            CALL(evt_msg_print, 0, STRING("stg4_usu_62"), 0, STRING("me"))
            CALL_CHILD(PTR(childABC_talk_01))
            WAIT_MS(1400)

            // She begs for spare food (stg4_usu_66) with a yes/no menu;
            // a food item from Mario's pouch earns a Shooting Star (stg4_usu_68).
            CALL(evt_msg_print, 0, STRING("stg4_usu_66"), 0, STRING("me"))
            CALL(evt_msg_select, 0, STRING("stg4_usu_66_yn"))
            CALL(evt_msg_continue)
            WAIT_FRAMES(1)
            CALL(evt_pouch_get_haveitemcnt, LW(1))
            IF_INT_EQUAL(LW(1), 0)
                SET_VALUE(LW(0), 1)
            END_IF
            IF_INT_EQUAL(LW(0), 0)
                CALL(get_item_list, LW(0))
                LOOP(0)
#if VERSION <= VERSION_DEMO_US
                    CALL(evt_win_item_select, LW(0), LW(1))
#else
                    CALL(evt_win_item_select, 0, LW(0), LW(1), 0)
#endif
                    IF_INT_NOT_EQUAL(LW(1), -1)
                        CALL(check_tabemono, LW(1), LW(2))
                        IF_INT_EQUAL(LW(2), 1)
                            CALL(evt_pouch_remove_item, LW(1), LW(2))
                            BREAK_LOOP
                        ELSE
                            CALL(evt_msg_print, 0, STRING("stg4_usu_67"), 0, 0)
                        END_IF
                    ELSE
                        BREAK_LOOP
                    END_IF
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(delete_item_list, LW(0))
                IF_INT_NOT_EQUAL(LW(1), -1)
                    CALL(evt_msg_print, 0, STRING("stg4_usu_68"), 0, STRING("me"))
                    CALL(evt_item_entry, STRING("orei"), ITEM_SHOOTING_STAR, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                    CALL(evt_item_get_item, STRING("orei"))
                    CALL(evt_item_get_item_end_wait, STRING("orei"))
                    CALL(evt_msg_print, 0, STRING("stg4_usu_69"), 0, STRING("me"))
                ELSE
                    CALL(evt_msg_print, 0, STRING("stg4_usu_70"), 0, STRING("me"))
                END_IF
            ELSE
                CALL(evt_msg_print, 0, STRING("stg4_usu_70"), 0, STRING("me"))
            END_IF

            // The temple bell tolls and she turns into a pig in a puff of smoke;
            // the children scream (stg4_usu_71 to 73).
            WAIT_MS(500)
            CALL(evt_snd_sfxon, STRING("SFX_STG4_TEMPLE_BELL1"), 0)
            WAIT_MS(2000)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CHANGE_PIG1"), LW(0), LW(1), LW(2), 0)
            BEGIN_INLINE_EVENT
                ADD(LW(2), 20)
                CALL(evt_eff, 0, STRING("kemuri_test"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
            END_INLINE_EVENT
            WAIT_MS(300)
            CALL(evt_npc_set_position, STRING("ブタおばさん"), LW(0), LW(1), LW(2))
            CALL(evt_npc_set_position, STRING("me"), 0, -2000, 0)
            CALL(evt_npc_get_reglid, STRING("ブタおばさん"), LW(0))
            DELETE_EVENT(LW(0))
            CALL(evt_npc_restart_regular_event, STRING("ブタおばさん"))
            BEGIN_INLINE_EVENT
                CALL(evt_msg_print, 0, STRING("stg4_usu_71"), 0, STRING("子供Ａ"))
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                WAIT_MS(400)
                CALL(evt_msg_print, 0, STRING("stg4_usu_72"), 0, STRING("子供Ｂ"))
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                WAIT_MS(600)
                CALL(evt_msg_print, 0, STRING("stg4_usu_73"), 0, STRING("子供Ｃ"))
            END_INLINE_EVENT
            SET_VALUE(GSWF_USU_01_AUNT_TURNED_INTO_PIG, 1)
        ELSE
            // As a pig she only oinks (stg4_usu_73_01).
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_msg_print, 0, STRING("stg4_usu_73_01"), 0, STRING("me"))
        END_IF
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        // After the Doopliss fight she gives story text: thanks for restoring the village
        // (stg4_usu_77), then comments by progress.
        CALL(evt_msg_print, 0, STRING("stg4_usu_77"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_78"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_81_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_81_4_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of child A: enables move/talk animation. */
EVT_BEGIN(childA_init)
    CALL(evt_npc_flag_onoff, 1, STRING("子供Ａ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/** Talk script of child A: a line about the mother turning into a pig, the three-children chat, or later story text. */
EVT_BEGIN(childA_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        IF_INT_EQUAL(GSWF_USU_01_AUNT_TURNED_INTO_PIG, 1)
            CALL(evt_msg_print, 0, STRING("stg4_usu_74"), 0, STRING("me"))
        ELSE
            CALL_CHILD(PTR(childABC_talk_01))
        END_IF
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL_CHILD(PTR(childABC_talk_02))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL_CHILD(PTR(childABC_talk_03))
        CASE_DEFAULT
        CALL_CHILD(PTR(childABC_talk_04))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of child B: enables move/talk animation. */
EVT_BEGIN(childB_init)
    CALL(evt_npc_flag_onoff, 1, STRING("子供Ｂ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/** Talk script of child B: a line about the mother turning into a pig, the three-children chat, or later story text. */
EVT_BEGIN(childB_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        IF_INT_EQUAL(GSWF_USU_01_AUNT_TURNED_INTO_PIG, 1)
            CALL(evt_msg_print, 0, STRING("stg4_usu_75"), 0, STRING("me"))
        ELSE
            CALL_CHILD(PTR(childABC_talk_01))
        END_IF
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL_CHILD(PTR(childABC_talk_02))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL_CHILD(PTR(childABC_talk_03))
        CASE_DEFAULT
        CALL_CHILD(PTR(childABC_talk_04))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of child C: enables move/talk animation. */
EVT_BEGIN(childC_init)
    CALL(evt_npc_flag_onoff, 1, STRING("子供Ｃ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

/** Talk script of child C: a line about the mother turning into a pig, the three-children chat, or later story text. */
EVT_BEGIN(childC_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        IF_INT_EQUAL(GSWF_USU_01_AUNT_TURNED_INTO_PIG, 1)
            CALL(evt_msg_print, 0, STRING("stg4_usu_76"), 0, STRING("me"))
        ELSE
            CALL_CHILD(PTR(childABC_talk_01))
        END_IF
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL_CHILD(PTR(childABC_talk_02))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL_CHILD(PTR(childABC_talk_03))
        CASE_DEFAULT
        CALL_CHILD(PTR(childABC_talk_04))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of the gatekeeper: enables move/talk animation;
 * before STG4_02_DINGE it blocks the e_bero exit and moves him to his post.
 */
EVT_BEGIN(gatekeeper_init)
    CALL(evt_npc_flag_onoff, 1, STRING("門番"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_02_DINGE)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_npc_set_position, STRING("門番"), 400, 0, -55)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("門番"), 0, -2000, 0)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("門番"), 400, 0, -55)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init script of the pig gatekeeper: parked below the map before STG4_02_DINGE,
 * shown at the gate until the Doopliss fight.
 */
EVT_BEGIN(gatekeeper_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ門番"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_02_DINGE)
        CALL(evt_npc_set_position, STRING("ブタ門番"), 0, -2000, 0)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_set_position, STRING("ブタ門番"), 400, 0, -55)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ門番"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of the gatekeeper, shared by his pig form.
 * The first talk walks Mario up to him: he warns of a monster in the woods
 * and says passing needs the mayor's permission, the partner answers (stg4_usu_45),
 * the pig-mayor scene (village_chief_change_pig_event) starts
 * and the story advances to STG4_01_00_GATEKEEPER_CONVERSATION.
 * Later he repeats the warning, refuses passage once the mayor is a pig,
 * oinks as a pig (until the Doopliss fight) and then gives story text.
 */
EVT_BEGIN(gatekeeper_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_01_00_GATEKEEPER_CONVERSATION)
        CALL(evt_mario_mov_pos2, 350, -50, 120)
        WAIT_FRAMES(1)
        CALL(evt_mario_set_dir, 90, 300, 0)
        CALL(evt_party_move_behind_mario, 0)
        CALL(evt_cam3d_evt_set, 372, 124, 321, 372, 57, -61, 500, EASING_SINE_IN_OUT)
        WAIT_MS(500)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_msg_print, 0, STRING("stg4_usu_44"), 0, STRING("me"))
        CALL(evt_cam3d_evt_set, 342, 124, 321, 342, 57, -61, 500, EASING_SINE_IN_OUT)
        WAIT_MS(500)
        CALL(evt_party_stop, 0)
        WAIT_MS(200)
        CALL(evt_msg_print_party, STRING("stg4_usu_45"))
        CALL(evt_party_run, 0)
        CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
        SPAWN_EVENT(PTR(village_chief_change_pig_event))
        SET_VALUE(GSW_STORY_PROGRESS, STG4_01_00_GATEKEEPER_CONVERSATION)
        RETURN
        CASE_LESS(STG4_01_THE_MAYORS_A_PIG)
        CALL(evt_msg_print, 0, STRING("stg4_usu_45_01"), 0, STRING("me"))
        CASE_LESS(STG4_02_DINGE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_46"), 0, STRING("me"))
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_48"), 0, STRING("me"))
        CASE_LESS(STG4_16_LEAVING_TWILIGHT_TOWN)
        CALL(evt_msg_print, 0, STRING("stg4_usu_49"), 0, STRING("me"))
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_msg_print, 0, STRING("stg4_usu_49_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
        CALL(evt_msg_print, 0, STRING("stg4_usu_49_2"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_49_2_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init script of crow F: puts it on its perch at (275, 83, 355). */
EVT_BEGIN(crow_f_init)
    CALL(evt_npc_set_position, STRING("me"), 275, 83, 355)
    RETURN
EVT_END()

/** Init script of crow G: puts it on its perch at (250, 112, 355). */
EVT_BEGIN(crow_g_init)
    CALL(evt_npc_set_position, STRING("me"), 250, 112, 355)
    RETURN
EVT_END()

/** Init script of crow H: puts it on its perch at (-147, 110, 346). */
EVT_BEGIN(crow_h_init)
    CALL(evt_npc_set_position, STRING("me"), -147, 110, 346)
    RETURN
EVT_END()

/** Init script of crow I: puts it on its perch at (-175, 110, 348). */
EVT_BEGIN(crow_i_init)
    CALL(evt_npc_set_position, STRING("me"), -175, 110, 348)
    RETURN
EVT_END()

/** Init script of crow J: puts it on its perch at (-195, 100, -157). */
EVT_BEGIN(crow_j_init)
    CALL(evt_npc_set_position, STRING("me"), -195, 100, -157)
    RETURN
EVT_END()

/**
 * Talk interrupt of the three young women: the pig versions speak "stg4_usu_148_1_01" to "03"
 * before the Doopliss fight; as dancers afterwards they speak "stg4_usu_148_1_04" to "06"
 * until the Peach report (STG4_15_05).
 */
static EVT_BEGIN(musume_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_10_DOOPLISS_BATTLE)
        CALL(evt_npc_get_position, STRING("ブタ３人娘Ａ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_01"), 0, STRING("ブタ３人娘Ａ"))
        CALL(evt_npc_get_position, STRING("ブタ３人娘Ｂ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_02"), 0, STRING("ブタ３人娘Ｂ"))
        CALL(evt_npc_get_position, STRING("ブタ３人娘Ｃ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_03"), 0, STRING("ブタ３人娘Ｃ"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_04"), 0, STRING("３人娘Ａ"))
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_05"), 0, STRING("３人娘Ｂ"))
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_06"), 0, STRING("３人娘Ｃ"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Sets up the first of the three young women (the "musume" trio; NPC name in LW(0), dancer or pig form):
 * places her at (-440, 0, 180) facing 90 degrees, enables move/talk animation,
 * makes musume_talk her talk event and briefly turns on instant turning while the map starts.
 */
EVT_BEGIN(musume_a_init)
    CALL(evt_npc_set_position, LW(0), -440, 0, 180)
    CALL(evt_npc_set_ry, LW(0), 90)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(musume_talk))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_INSTANT_TURN)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_npc_wait_pera, LW(0))
        CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_INSTANT_TURN)
    END_INLINE_EVENT
    RETURN
EVT_END()

/** Spawns the first young woman in her dancer form (model "c_dance_r") and runs musume_a_init on her. */
static EVT_BEGIN(musume_a_entry)
    CALL(evt_npc_entry, STRING("３人娘Ａ"), STRING("c_dance_r"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ａ"), STRING("踊り子"))
    SET_VALUE(LW(0), STRING("３人娘Ａ"))
    CALL_CHILD(PTR(musume_a_init))
    RETURN
EVT_END()

/** Spawns the pig form of the first young woman ("c_bubusan" model) and runs musume_a_init on her. */
EVT_BEGIN(musume_a_pig_entry)
    CALL(evt_npc_entry, STRING("ブタ３人娘Ａ"), STRING("c_bubusan"))
    CALL(evt_npc_set_tribe, STRING("ブタ３人娘Ａ"), STRING("ブタ"))
    SET_VALUE(LW(0), STRING("ブタ３人娘Ａ"))
    CALL_CHILD(PTR(musume_a_init))
    RETURN
EVT_END()

/**
 * Sets up the second of the three young women (NPC name in LW(0)): as musume_a_init, at (-410, 0, 150).
 */
EVT_BEGIN(musume_b_init)
    CALL(evt_npc_set_position, LW(0), -410, 0, 150)
    CALL(evt_npc_set_ry, LW(0), 90)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(musume_talk))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_INSTANT_TURN)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_npc_wait_pera, LW(0))
        CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_INSTANT_TURN)
    END_INLINE_EVENT
    RETURN
EVT_END()

/** Spawns the second young woman in her pink dancer form (model "c_dance_p") and runs musume_b_init on her. */
static EVT_BEGIN(musume_b_entry)
    CALL(evt_npc_entry, STRING("３人娘Ｂ"), STRING("c_dance_p"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ｂ"), STRING("踊り子ピンク"))
    SET_VALUE(LW(0), STRING("３人娘Ｂ"))
    CALL_CHILD(PTR(musume_b_init))
    RETURN
EVT_END()

/** Spawns the pig form of the second young woman and runs musume_b_init on her. */
EVT_BEGIN(musume_b_pig_entry)
    CALL(evt_npc_entry, STRING("ブタ３人娘Ｂ"), STRING("c_bubusan"))
    CALL(evt_npc_set_tribe, STRING("ブタ３人娘Ｂ"), STRING("ブタ"))
    SET_VALUE(LW(0), STRING("ブタ３人娘Ｂ"))
    CALL_CHILD(PTR(musume_b_init))
    RETURN
EVT_END()

/**
 * Sets up the third of the three young women (NPC name in LW(0)): as musume_a_init, at (-380, 0, 180).
 */
EVT_BEGIN(musume_c_init)
    CALL(evt_npc_set_position, LW(0), -380, 0, 180)
    CALL(evt_npc_set_ry, LW(0), 90)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_change_interrupt, LW(0), 6, PTR(musume_talk))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_INSTANT_TURN)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_npc_wait_pera, LW(0))
        CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_INSTANT_TURN)
    END_INLINE_EVENT
    RETURN
EVT_END()

/** Spawns the third young woman in her green dancer form (model "c_dance_k") and runs musume_c_init on her. */
static EVT_BEGIN(musume_c_entry)
    CALL(evt_npc_entry, STRING("３人娘Ｃ"), STRING("c_dance_k"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ｃ"), STRING("踊り子黄緑"))
    SET_VALUE(LW(0), STRING("３人娘Ｃ"))
    CALL_CHILD(PTR(musume_c_init))
    RETURN
EVT_END()

/** Spawns the pig form of the third young woman and runs musume_c_init on her. */
EVT_BEGIN(musume_c_pig_entry)
    CALL(evt_npc_entry, STRING("ブタ３人娘Ｃ"), STRING("c_bubusan"))
    CALL(evt_npc_set_tribe, STRING("ブタ３人娘Ｃ"), STRING("ブタ"))
    SET_VALUE(LW(0), STRING("ブタ３人娘Ｃ"))
    CALL_CHILD(PTR(musume_c_init))
    RETURN
EVT_END()

/**
 * Regular script of the wandering NPCs (villagers G and J and the pig forms): wanders around its start position
 * within a 50-unit radius at speed 30 using the urouro helpers.
 */
static EVT_BEGIN(urouro_move)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(50.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/**
 * Narrow-range ("semai") variant of urouro_move:
 * wanders around the start position within a 15-unit radius at speed 30.
 * Nothing in the tree uses it.
 */
EVT_BEGIN(urouro_move_semai)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(15.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/**
 * NPC setup table of the east side of Twilight Town: villagers F to J, the shopkeeper and his wife,
 * the dandy, the aunt and the three children, the gatekeeper, the pig forms of the dandy, the aunt,
 * the gatekeeper and villagers I and J (shown only before the Doopliss fight) and the five crows F to J.
 */
static NpcSetupInfo npcEnt[] = {
    {
        "村人Ｆ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        villagerF_init, // initEvent
        NULL, // regularEvent
        villagerF_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "村人Ｇ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerG_init, // initEvent
        urouro_move, // regularEvent
        villagerG_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "村人Ｈ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        villagerH_init, // initEvent
        NULL, // regularEvent
        villagerH_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "村人Ｉ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        villagerI_init, // initEvent
        NULL, // regularEvent
        villagerI_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "村人Ｊ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerJ_init, // initEvent
        urouro_move, // regularEvent
        villagerJ_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "店の主人", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        shop_man_init, // initEvent
        NULL, // regularEvent
        shop_man_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "お店の奥さん", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        shop_woman_init, // initEvent
        NULL, // regularEvent
        shop_woman_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "キザ野郎", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        kizayarou_init, // initEvent
        NULL, // regularEvent
        kizayarou_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "おばさん", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        aunt_init, // initEvent
        NULL, // regularEvent
        aunt_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "子供Ａ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        childA_init, // initEvent
        NULL, // regularEvent
        childA_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "子供Ｂ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        childB_init, // initEvent
        NULL, // regularEvent
        childB_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "子供Ｃ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        childC_init, // initEvent
        NULL, // regularEvent
        childC_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "門番", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        gatekeeper_init, // initEvent
        NULL, // regularEvent
        gatekeeper_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ブタキザ野郎", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        kizayarou_pig_init, // initEvent
        urouro_move, // regularEvent
        kizayarou_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ブタおばさん", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        aunt_pig_init, // initEvent
        urouro_move, // regularEvent
        aunt_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ブタ門番", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        gatekeeper_pig_init, // initEvent
#if VERSION <= VERSION_DEMO_US
        urouro_move,
#else
        NULL, // regularEvent
#endif
        gatekeeper_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ブタ村人Ｉ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerI_pig_init, // initEvent
        urouro_move, // regularEvent
        villagerI_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "ブタ村人Ｊ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        villagerJ_pig_init, // initEvent
        urouro_move, // regularEvent
        villagerJ_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "カラスＦ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_f_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "カラスＧ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_g_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "カラスＨ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_h_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "カラスＩ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_i_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "カラスＪ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_j_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Spawned by the first gatekeeper talk: waits until Mario walks to x <= -300, then the temple bell rings;
 * Mario is startled (the partner is called out if absent), turns to the partner,
 * and the partner says the bell means someone is being turned into a pig
 * and is glad it was not them (stg4_usu_50).
 */
EVT_BEGIN(village_chief_change_pig_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_LESS_OR_EQUAL(LW(0), -300)
        GOTO(10)
    END_IF
    GOTO(0)
    LABEL(10)
    CALL(evt_mario_key_onoff, 0)
#if VERSION > VERSION_DEMO_US
    CALL(evt_mario_normalize)
#endif
    CALL(evt_snd_sfxon, STRING("SFX_STG4_TEMPLE_BELL1"), 0)
    WAIT_MS(500)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_hello_party, 0, 1)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_mario_set_dir, 90, 100, 0)
    ELSE
        CALL(evt_mario_set_dir, 270, 100, 0)
    END_IF
    ADD(LW(0), LW(3))
    DIVIDE(LW(0), 2)
    ADD(LW(2), LW(5))
    DIVIDE(LW(2), 2)
    SET_VALUE(LW(4), LW(1))
    SET_VALUE(LW(5), LW(2))
    ADD(LW(1), 35)
    SUBTRACT(LW(2), 13)
    ADD(LW(4), 100)
    ADD(LW(5), 330)
    CALL(evt_cam3d_evt_set, LW(0), LW(4), LW(5), LW(0), LW(1), LW(2), 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg4_usu_50"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Gate script: before the gatekeeper has become a pig the gatekeeper refuses ("stg4_usu_45_02");
 * afterwards the gate halves rotate open and the story moves to STG4_02_03_OPENING_THE_GATE.
 */
EVT_BEGIN(evt_mon_open)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_02_02_THE_GATEKEEPERS_A_PIG)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_45_02"), 0, STRING("門番"))
        CALL(evt_mario_key_onoff, 1)
    ELSE
        CALL(evt_cam_letter_box_disable, 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_monato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, STRING("S_mon01"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_FENCE_OPEN1"), LW(10), LW(11), LW(12), LW(9))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 900, 1000)
        LABEL(123)
        CALL(evt_sub_intpl_msec_get_value)
        SET_FLOAT_VALUE(LW(2), LW(0))
        MULTIPLY_FLOAT(LW(2), FLOAT(0.0546875f))
        SET_FLOAT_VALUE(LW(3), LW(0))
        MULTIPLY_FLOAT(LW(3), FLOAT(-0.09375f))
        CALL(evt_mapobj_rotate, 1, STRING("S_mon01"), 0, LW(2), 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_mon02"), 0, LW(3), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(123)
        END_IF
        CALL(evt_hitobj_onoff, STRING("A_mon"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("A_mon02"), 1, 1)
        CALL(evt_snd_sfxoff, LW(9))
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_cam_letter_box_disable, 0)
        SET_VALUE(GSW_STORY_PROGRESS, STG4_02_03_OPENING_THE_GATE)
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

/**
 * Raises the 3D camera and its target by 85 units over 500 ms, then waits 600 ms;
 * the crow watchers use it before a crow speaks from its perch.
 */
static EVT_BEGIN(cam_up)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_get_pos, CAMERA_3D, LW(0), LW(1), LW(2))
        CALL(evt_cam_get_at, CAMERA_3D, LW(3), LW(4), LW(5))
        ADD(LW(1), 85)
        ADD(LW(4), 85)
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 500, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    WAIT_MS(600)
    RETURN
EVT_END()

/** Returns the 3D camera to its normal position over 500 ms (after cam_up) and waits for it. */
static EVT_BEGIN(cam_down)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    RETURN
EVT_END()

/**
 * Background loop: while Vivian is the active partner and Mario is not hidden in her shadow,
 * it clears the crow-talk gates GW(0), GW(1) and GW(2) so the crows talk again next time.
 */
EVT_BEGIN(usu01_check_vivian)
    LABEL(0)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(GW(0), 0)
        SET_VALUE(GW(1), 0)
        SET_VALUE(GW(2), 0)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(usu01_crowFG_talk_event)
    // No balloon is showing yet; GW(0) keeps the watcher quiet once the crows have talked.
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // Check whether Mario is within 20 units of crow F.
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＦ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_area_check, 1, LW(3), LW(5), 20, LW(0), LW(2), LW(6))
#if VERSION >= VERSION_US
    CALL(evt_mario_get_party, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 6)
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            SET_VALUE(LW(6), 0)
        END_IF
    END_IF
    CALL(evt_mario_get_motion, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 28)
        IF_INT_NOT_EQUAL(LW(1), 0)
            IF_INT_NOT_EQUAL(LW(1), 1)
                IF_INT_NOT_EQUAL(LW(1), 2)
                    SET_VALUE(LW(6), 0)
                END_IF
            END_IF
        END_IF
    END_IF
#endif

    // Show the "..." balloon over Mario while he is in range and hide it when he leaves.
    IF_INT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_balloon_tenten)
            SET_VALUE(LW(15), 1)
        END_IF
    ELSE
        IF_INT_EQUAL(LW(15), 1)
            CALL(evt_mario_balloon_off)
            SET_VALUE(LW(15), 0)
        END_IF
    END_IF

    // A pressed with the balloon up: Mario's controls lock,
    // the camera rises to the crows and crow F caws (stg4_jin_153).
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            IF_BITS_SET(LW(0), 256)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(15), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_cont_onoff, 0, 0)
#if VERSION > VERSION_DEMO_US
                CALL(mainwindow_opendisable)
#endif
                CALL_CHILD(PTR(cam_up))
                CALL(evt_npc_get_position, STRING("カラスＦ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_msg_print, 0, STRING("stg4_jin_153"), 0, STRING("カラスＦ"))
                CALL_CHILD(PTR(cam_down))
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_party_cont_onoff, 0, 1)
#if VERSION > VERSION_DEMO_US
                CALL(mainwindow_openenable)
#endif
            END_IF
        END_IF
    END_IF

    // The crows only speak when Vivian is the partner and Mario hides in her shadow.
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // Once Vivian has joined, the camera rises and the crows hold a conversation chosen by story progress.
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
#if VERSION > VERSION_DEMO_US
            CALL(mainwindow_opendisable)
#endif
            CALL_CHILD(PTR(cam_up))
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＦ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(STG4_14_NAME_GUESSING)
                // Before the name guessing, crow F says the Mario with the mayor is really the
                // villain and crow G adds that only a soul locked under Creepy Steeple knows
                // his name (stg4_usu_137 to 140).
                CALL(evt_msg_print, 0, STRING("stg4_usu_137"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_138"), 0, STRING("カラスＧ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_139"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_140"), 0, STRING("カラスＧ"))
                CALL_CHILD(PTR(cam_down))

                // The first time, Vivian and Mario step aside and she proposes searching under Creepy Steeple
                // for the villain's real name (stg4_usu_149_viv, 150_viv).
                IF_INT_EQUAL(GSWF_USU_01_CROWS_FG_PARTNER_TALK_DONE, 0)
                    CALL(evt_party_cont_onoff, 0, 1)
                    LOOP(0)
                        CALL(evt_party_get_status, 6, LW(0))
                        IF_INT_EQUAL(LW(0), 0)
                            BREAK_LOOP
                        END_IF
                        WAIT_FRAMES(1)
                    END_LOOP
                    WAIT_MS(400)
                    CALL(evt_party_stop, 0)
                    CALL(evt_party_move_pos2, 0, 280, 350, FLOAT(80.0f))
                    WAIT_FRAMES(1)
                    CALL(evt_party_set_dir, 0, 270, 200)
                    CALL(evt_mario_set_dir, 90, 200, 0)
                    WAIT_MS(300)
                    CALL(evt_msg_print_party, STRING("stg4_usu_149"))
                    WAIT_MS(300)
                    CALL(evt_mario_set_pose, STRING("M_N_2"))
                    WAIT_MS(500)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_msg_print_party, STRING("stg4_usu_150"))
                    WAIT_MS(300)
                    SET_VALUE(GSWF_USU_01_CROWS_FG_PARTNER_TALK_DONE, 1)
                    CALL(evt_party_cont_onoff, 0, 0)
                    CALL(evt_party_run, 0)
                END_IF
                CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
                // Later the crows gossip about Peach being kidnapped again (stg4_usu_141 and 142),
                // repeated in new versions after Franklys's house (142_1, 142_2) and after the ending (142_2_1, 142_2_2).
                // .
                CALL(evt_msg_print, 0, STRING("stg4_usu_141"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_142"), 0, STRING("カラスＧ"))
                CALL_CHILD(PTR(cam_down))
                CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
                CALL(evt_msg_print, 0, STRING("stg4_usu_142_1"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_142_2"), 0, STRING("カラスＧ"))
                CALL_CHILD(PTR(cam_down))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_142_2_1"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_142_2_2"), 0, STRING("カラスＧ"))
                CALL_CHILD(PTR(cam_down))
            END_SWITCH

            // Controls come back; GW(0) keeps the crows quiet until usu01_check_vivian resets it.
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_mario_key_onoff, 1)
#if VERSION > VERSION_DEMO_US
            CALL(mainwindow_openenable)
#endif
            SET_VALUE(GW(0), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Background watcher for crows H and I: when Mario stands within 80 units of crow H,
 * a "..." balloon shows over him
 * (US and later: only while he stands, walks, dashes or rides in Vivian's shadow,
 * and no other partner's action is in use); pressing A makes the crow caw (stg4_jin_153).
 * When Mario is hidden in Vivian's shadow (after STG4_13_VIVIAN_JOINS) the crows talk instead,
 * by story progress (stg4_usu_143 to 146_2_2); that sets GW(1),
 * which keeps the watcher quiet until usu01_check_vivian clears it.
 */
EVT_BEGIN(usu01_crowHI_talk_event)
    // No balloon is showing yet; GW(1) keeps the watcher quiet once the crows have talked.
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(1), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // Check whether Mario is within 80 units of crow H.
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＨ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_area_check, 1, LW(3), LW(5), 80, LW(0), LW(2), LW(6))
#if VERSION >= VERSION_US
    CALL(evt_mario_get_party, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 6)
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            SET_VALUE(LW(6), 0)
        END_IF
    END_IF
    CALL(evt_mario_get_motion, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 28)
        IF_INT_NOT_EQUAL(LW(1), 0)
            IF_INT_NOT_EQUAL(LW(1), 1)
                IF_INT_NOT_EQUAL(LW(1), 2)
                    SET_VALUE(LW(6), 0)
                END_IF
            END_IF
        END_IF
    END_IF
#endif

    // Show the "..." balloon over Mario while he is in range and hide it when he leaves.
    IF_INT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_balloon_tenten)
            SET_VALUE(LW(15), 1)
        END_IF
    ELSE
        IF_INT_EQUAL(LW(15), 1)
            CALL(evt_mario_balloon_off)
            SET_VALUE(LW(15), 0)
        END_IF
    END_IF

    // A pressed with the balloon up: Mario's controls lock,
    // the camera rises to the crows and crow H caws (stg4_jin_153).
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            IF_BITS_SET(LW(0), 256)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(15), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL_CHILD(PTR(cam_up))
                CALL(evt_npc_get_position, STRING("カラスＨ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_msg_print, 0, STRING("stg4_jin_153"), 0, STRING("カラスＨ"))
                CALL_CHILD(PTR(cam_down))
                CALL(evt_mario_key_onoff, 1)
            END_IF
        END_IF
    END_IF

    // The crows only speak when Vivian is the partner and Mario hides in her shadow.
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // Once Vivian has joined, the camera rises and the crows chat about their children.
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
#if VERSION > VERSION_DEMO_US
            CALL(mainwindow_opendisable)
#endif
            CALL_CHILD(PTR(cam_up))
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＨ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(STG4_14_NAME_GUESSING)
                // Before the name guessing, crow H tells crow I that her oldest, Ronnie,
                // is taking his SATs (stg4_usu_143) and crow I is impressed (144).
                CALL(evt_msg_print, 0, STRING("stg4_usu_143"), 0, STRING("カラスＨ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_144"), 0, STRING("カラスＩ"))
                CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
                // Later, crow H says he is even going to a test prep center (145) and crow I thinks about a
                // college fund for little Billy (146).
                CALL(evt_msg_print, 0, STRING("stg4_usu_145"), 0, STRING("カラスＨ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_146"), 0, STRING("カラスＩ"))
                CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
                // After Franklys's house, crow H laments that Ronnie did poorly on his SATs (146_1) and crow
                // I consoles her (146_2); the last lines follow the ending.
                CALL(evt_msg_print, 0, STRING("stg4_usu_146_1"), 0, STRING("カラスＨ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_146_2"), 0, STRING("カラスＩ"))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_146_2_1"), 0, STRING("カラスＨ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_146_2_2"), 0, STRING("カラスＩ"))
            END_SWITCH
            CALL_CHILD(PTR(cam_down))

            // Controls come back; GW(1) keeps the crows quiet until usu01_check_vivian resets it.
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_mario_key_onoff, 1)
#if VERSION > VERSION_DEMO_US
            CALL(mainwindow_openenable)
#endif
            SET_VALUE(GW(1), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Background watcher for crow J: when Mario stands within 20 units of crow J, a "..." balloon shows over him
 * (US and later: only while he stands, walks, dashes or rides in Vivian's shadow,
 * and no other partner's action is in use); pressing A makes the crow caw (stg4_jin_153).
 * When Mario is hidden in Vivian's shadow (after STG4_13_VIVIAN_JOINS) the crow talks instead,
 * by story progress (stg4_usu_147 to 148_1_1); that sets GW(2),
 * which keeps the watcher quiet until usu01_check_vivian clears it.
 */
EVT_BEGIN(usu01_crowJ_talk_event)
    // No balloon is showing yet; GW(2) keeps the watcher quiet once the crow has talked.
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(2), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // Check whether Mario is within 20 units of crow J.
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＪ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_area_check, 1, LW(3), LW(5), 20, LW(0), LW(2), LW(6))
#if VERSION >= VERSION_US
    CALL(evt_mario_get_party, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 6)
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            SET_VALUE(LW(6), 0)
        END_IF
    END_IF
    CALL(evt_mario_get_motion, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 28)
        IF_INT_NOT_EQUAL(LW(1), 0)
            IF_INT_NOT_EQUAL(LW(1), 1)
                IF_INT_NOT_EQUAL(LW(1), 2)
                    SET_VALUE(LW(6), 0)
                END_IF
            END_IF
        END_IF
    END_IF
#endif

    // Show the "..." balloon over Mario while he is in range and hide it when he leaves.
    IF_INT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_balloon_tenten)
            SET_VALUE(LW(15), 1)
        END_IF
    ELSE
        IF_INT_EQUAL(LW(15), 1)
            CALL(evt_mario_balloon_off)
            SET_VALUE(LW(15), 0)
        END_IF
    END_IF

    // A pressed with the balloon up: Mario's controls lock,
    // the camera rises to the crow and it caws (stg4_jin_153).
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            IF_BITS_SET(LW(0), 256)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(15), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL_CHILD(PTR(cam_up))
                CALL(evt_npc_get_position, STRING("カラスＪ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_msg_print, 0, STRING("stg4_jin_153"), 0, STRING("カラスＪ"))
                CALL_CHILD(PTR(cam_down))
                CALL(evt_mario_key_onoff, 1)
            END_IF
        END_IF
    END_IF

    // The crow only speaks when Vivian is the partner and Mario hides in her shadow.
    CALL(evt_mario_get_party, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 6)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_party_get_status, 6, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // Once Vivian has joined, the camera rises and crow J muses aloud by story progress.
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
#if VERSION > VERSION_DEMO_US
            CALL(mainwindow_opendisable)
#endif
            CALL_CHILD(PTR(cam_up))
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＪ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(STG4_14_NAME_GUESSING)
                // Before the name guessing, crow J laments that the twilit world is
                // full of pain (stg4_usu_147).
                CALL(evt_msg_print, 0, STRING("stg4_usu_147"), 0, STRING("カラスＪ"))
                CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)
                // Later it finds small sparks of hope in the gloom (stg4_usu_148),
                // then (after Franklys's house) decides to go on an epic journey (148_1 and 148_1_1).
                CALL(evt_msg_print, 0, STRING("stg4_usu_148"), 0, STRING("カラスＪ"))
                CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
                CALL(evt_msg_print, 0, STRING("stg4_usu_148_1"), 0, STRING("カラスＪ"))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_1"), 0, STRING("カラスＪ"))
            END_SWITCH

            // Camera returns, Mario's controls come back and the partner is released.
            CALL_CHILD(PTR(cam_down))
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
#if VERSION > VERSION_DEMO_US
            CALL(mainwindow_openenable)
#endif
            SET_VALUE(GW(2), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/**
 * NPCs inside house 1, the aunt's home: the aunt in her human and pig forms and the three children;
 * door_1 lists them as the house's interior NPCs.
 */
const char* minka_npc[] = {
    "おばさん",
    "ブタおばさん",
    "子供Ａ",
    "子供Ｂ",
    "子供Ｃ",
    NULL,
};

/**
 * Item objects inside house 1: the Meteor Meal "iri_20" the aunt leaves for Mario when his items are full;
 * door_1 lists them as the house's contents.
 */
const char* minka_item[] = {
    "iri_20",
    NULL,
};

/**
 * NPCs inside the shop (house 2): the shopkeeper's wife, the dandy and his pig form,
 * the shopkeeper and the black chest's demon (Peramajin); door_2_1 lists them as the interior NPCs.
 */
const char* shop_npc[] = {
    "お店の奥さん",
    "キザ野郎",
    "ブタキザ野郎",
    "店の主人",
    "ペラ魔人",
    NULL,
};

/**
 * Map objects inside the shop (house 2): the storeroom lock, the black chest "pera_box" and the item chest
 * "mobj_box00"; door_2_1 lists them as the shop's contents.
 */
const char* shop_mobj[] = {
    "usu_00_lock",
    "pera_box",
    "mobj_box00",
    NULL,
};

/**
 * Pickup items lying in the shop (house 2): item_02 (Life Shroom), item_03 (Boo's Sheet),
 * item_04 (Jammin' Jelly) and item_05 (Inn Coupon); door_2_1 lists them as the shop's contents.
 */
const char* shop_item[] = {
    "item_02",
    "item_03",
    "item_04",
    "item_05",
    NULL,
};

/**
 * Door script for house 1's door "S_usu01_ie1_door" (evtIn of door_1):
 * turns it about y by LW(0) scaled to door units.
 */
static EVT_BEGIN(evt_door_1_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie1_door"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door script for house 1's door (evtOut of door_1,
 * which the door code runs with the door-fall sound while the house front opens or closes):
 * tilts "S_usu01_ie1_door" about x by half of LW(0) (0 to 180, so up to 90 degrees).
 */
static EVT_BEGIN(evt_door_1_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie1_door"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Wall script of house 1 (evtWall of door_1, run by the door code as the house front swings open or shut,
 * LW(0) 0 to 180):
 * tilts the front panels "S_usu01_ie1_kai1" and "S_usu01_ie1_kai2" about x by -LW(0)/2 and +LW(0)/2.
 */
static EVT_BEGIN(evt_wall_1)
    SET_FLOAT_VALUE(LW(1), LW(0))
    MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie1_kai1"), LW(1), 0, 0)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie1_kai2"), LW(2), 0, 0)
    RETURN
EVT_END()

/** House 1 open callback: dips BGM and ambience to 50 and low-pass filters the environment sound. */
static EVT_BEGIN(door_1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** House 1 close callback: restores BGM and ambience and removes the low-pass filter. */
static EVT_BEGIN(door_1_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/**
 * Door of house 1 (the aunt's home, "S_usu01_ie1_door"): its collision,
 * the inside and outside models and the open and close scripts,
 * with the interior NPCs and items listed in minka_npc and minka_item.
 */
static DoorParam door_1 = {
    0, // flags
    3, // type
    2, // se
    "S_usu01_ie1_door", // mapobj
    "A_door46", // hitIn
    "A_door45", // hitOut
    "S_usu01_ie1", // outside
    NULL, // outsideHit
    "S_usu01_ie1_naka", // inside
    NULL, // insideHit
    evt_door_1_1, // evtIn
    evt_door_1_2, // evtOut
    evt_wall_1, // evtWall
    (const char**)minka_npc, // npcs
    NULL, // mobjs
    (const char**)minka_item, // items
    door_1_open, // evtOpen
    door_1_close, // evtClose
};

/** Door script for house 2's front door: rotates "S_usu01_ie2_door1" by LW(0) scaled to door units. */
EVT_BEGIN(evt_door_2_1_1)
    SET_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Empty door script for house 2 (just returns). */
EVT_BEGIN(evt_door_2_1_2)
    RETURN
EVT_END()

/**
 * Wall script of house 2, the shop (evtWall of door_2_1, LW(0) 0 to 180 as the front swings open or shut):
 * turns the front panels "S_usu01_ie2_kai1" and "S_usu01_ie2_kai2" about z by +LW(0)/2 and -LW(0)/2.
 */
EVT_BEGIN(evt_wall_2_1)
    SET_FLOAT_VALUE(LW(1), LW(0))
    MULTIPLY_FLOAT(LW(1), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_kai1"), 0, 0, LW(1))
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_kai2"), 0, 0, LW(2))
    RETURN
EVT_END()

/**
 * Background chatter for the shop quarrel (LW(0) speaker, LW(1) message id,
 * LW(2) a flag used as the "may speak" switch, LW(3) start delay in ms);
 * runs only from the Doopliss fight to STG4_15_05.
 * Once Mario can move it prints the message over and over;
 * a sibling loop closes the speaker's bubble and pauses it
 * while that speaker is the NPC Mario would talk to (chk_fuki), so he can be spoken to.
 */
EVT_BEGIN(loop_msg)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_10_DOOPLISS_BATTLE)
        RETURN
    END_IF
    IF_INT_GREATER(GSW_STORY_PROGRESS, STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        RETURN
    END_IF
    CALL(evt_mario_wait_movable)
    WAIT_MS(LW(3))
    SET_INT_INDEXED(LW(2), 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(chk_fuki, LW(0), LW(4))
            IF_INT_EQUAL(LW(4), 1)
                GET_INT_INDEXED(LW(2), LW(5))
                IF_INT_EQUAL(LW(5), 1)
                    CALL(evt_msg_close, LW(1))
                END_IF
                SET_INT_INDEXED(LW(2), 0)
            ELSE
                SET_INT_INDEXED(LW(2), 1)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    LOOP(0)
        LOOP(0)
            GET_INT_INDEXED(LW(2), LW(5))
            IF_INT_EQUAL(LW(5), 1)
                WAIT_FRAMES(1)
                GET_INT_INDEXED(LW(2), LW(5))
                IF_INT_EQUAL(LW(5), 1)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_msg_print, 0, LW(1), 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Shop quarrel chatter, started when Mario enters the shop or comes back from the storeroom:
 * through loop_msg the dandy (stg4_usu_61_3, swearing he just woke up in the shop)
 * and the shopkeeper (stg4_usu_61_4, demanding why he was there with his wife) repeat their lines.
 */
EVT_BEGIN(mise_kiza_msg)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), STRING("キザ野郎"))
        SET_VALUE(LW(1), STRING("stg4_usu_61_3"))
        SET_RAW_VALUE(LW(2), LSWF_USU_01_KIZAYAROU_BUBBLE_ON)
        SET_VALUE(LW(3), 700)
        CALL_CHILD(PTR(loop_msg))
    END_SIBLING_EVENT
    SET_VALUE(LW(0), STRING("店の主人"))
    SET_VALUE(LW(1), STRING("stg4_usu_61_4"))
    SET_RAW_VALUE(LW(2), LSWF_USU_01_SHOPKEEPER_BUBBLE_ON)
    SET_VALUE(LW(3), 300)
    CALL_CHILD(PTR(loop_msg))
    RETURN
EVT_END()

EVT_BEGIN(mawari_house_delay)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_01_THE_MAYORS_A_PIG)
        IF_INT_EQUAL(GSWF_USU_01_SHOP_WIFE_FIRST_TALK_DONE, 1)
            SPAWN_EVENT(PTR(evt_monban_buta2))
        END_IF
    END_IF
    DELETE_EVENT(GW(3))
    CALL(evt_msg_close, STRING("stg4_usu_61_3"))
    CALL(evt_msg_close, STRING("stg4_usu_61_4"))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_usu01_ie3"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_usu01_ie3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/**
 * Shop open callback (evtOpen of door_2_1): dips the BGM and ambience and low-pass filters the ambience,
 * stops the sounds heard from outside (usu01_gusun) and starts the quarrel chatter (mise_kiza_msg).
 */
EVT_BEGIN(door_2_1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    DELETE_EVENT(GW(4))
    CALL(evt_msg_close, STRING("stg4_usu_61_1"))
    CALL(evt_msg_close, STRING("stg4_usu_61_2"))
    SPAWN_EVENT_GET_ID(PTR(mise_kiza_msg), GW(3))
    RETURN
EVT_END()

/**
 * Front door of house 2, the shop ("S_usu01_ie2_door1"): its collision,
 * the inside and outside models and its scripts, with the interior NPCs,
 * map objects and items listed in shop_npc, shop_mobj and shop_item.
 */
DoorParam door_2_1 = {
    0, // flags
    3, // type
    2, // se
    "S_usu01_ie2_door1", // mapobj
    "A_usu01_ie2_door2_u", // hitIn
    "A_usu01_ie2_door2", // hitOut
    "S_usu01_ie2", // outside
    NULL, // outsideHit
    "S_usu01_ie2_naka", // inside
    NULL, // insideHit
    evt_door_2_1_1, // evtIn
    NULL, // evtOut
    evt_wall_2_1, // evtWall
    (const char**)shop_npc, // npcs
    (const char**)shop_mobj, // mobjs
    (const char**)shop_item, // items
    door_2_1_open, // evtOpen
    mawari_house_delay, // evtClose
};

/**
 * Script helper that blocks forever: each time Mario's sleep animation (M_N_5A) loops it plays
 * "SFX_VOICE_MARIO_SLEEP1" at his position and stores the sound id in GW(10).
 */
USER_FUNC(sleep_voice) {
    MarioWork* mario = marioGetPtr();
    s32 loops;

    if (isFirstCall) {
        event->userdata[0] = -1;
    }
    if (strcmp(mario->animName, "M_N_5A") == 0) {
        loops = animPoseGetLoopTimes(mario->paperAnimGroupId[marioAnimeId()]);
        if (loops != event->userdata[0]) {
            event->userdata[0] = loops;
            evtSetValue(event, GW(10), psndSFXOn_3D("SFX_VOICE_MARIO_SLEEP1", &mario->position));
        }
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: builds a -1 terminated list of the items in Mario's pouch (first 20 slots) in newly allocated
 * memory, returns the pointer in args[0].
 */
USER_FUNC(get_item_list) {
    EvtWord* args = event->args;
#if VERSION >= VERSION_US
    s32 cnt = pouchGetHaveItemCnt();
    s32* list = _mapAlloc((cnt + 1) * sizeof(s32));
#else
    s32* list = _mapAlloc((pouchGetHaveItemCnt() + 1) * sizeof(s32));
#endif
    s32 n;
    s32 i;

    for (i = 0, n = 0; i < MAX_HELD_ITEMS; i++) {
        if (pouchHaveItem(i)) {
            list[n] = pouchHaveItem(i);
            n++;
        }
    }
    list[n] = -1;
    evtSetValue(event, args[0], (EvtWord)list);
    return EVT_RETURN_DONE;
}

/** Script helper: frees the list that get_item_list allocated (pointer in args[0]). */
USER_FUNC(delete_item_list) {
#if VERSION >= VERSION_US
    void* p = (void*)evtGetValue(event, event->args[0]);
    _mapFree(p);
#else
    _mapFree((void*)evtGetValue(event, event->args[0]));
#endif
    return EVT_RETURN_DONE;
}

/**
 * Script helper: returns in args[1] whether item args[0] counts as food for the aunt's request:
 * its usable_locations has the menu-use bit (4); aunt_talk turns anything else down as not edible.
 */
USER_FUNC(check_tabemono) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    BOOL food = FALSE;

    // FAKE: the u16 view of the u16 field is a no-op; the plain field read schedules the table load differently
    if (*(u16*)&itemDataTable[id].usable_locations & 4) {
        food = TRUE;
    }
    evtSetValue(event, args[1], food);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: returns in args[1] TRUE if the NPC that the A button would talk to this frame (talkTarget)
 * is the one named args[0].
 */
USER_FUNC(chk_fuki) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = npcGetWorkPtr()->talkTarget;

    if (npc != NULL && strcmp(npc->description, name) == 0) {
        evtSetValue(event, args[1], TRUE);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[1], FALSE);
    return EVT_RETURN_DONE;
}

/** Script helper: hides the six shop display objects in obj_list. */
USER_FUNC(shop_item_disp_off) {
    s32 i;

    for (i = 0; i < 6; i++) {
        mapObjFlagOn(obj_list[i].mobj, MAP_OBJ_FLAG_HIDDEN);
    }
    return EVT_RETURN_DONE;
}

/** Script helper: shows the six shop display objects in obj_list. */
USER_FUNC(shop_item_disp_on) {
    s32 i;

    for (i = 0; i < 6; i++) {
        mapObjFlagOff(obj_list[i].mobj, MAP_OBJ_FLAG_HIDDEN);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper run when Mario goes into the storeroom:
 * lets the shop's people (the wife, the dandy in both forms and the shopkeeper) take the map's blend colour
 * again,
 * so they darken with the shop, and makes the black chest's demon ("ペラ魔人") ignore it.
 */
USER_FUNC(ie2_npc_disp_off) {
    const char* show[] = {"\202\250\223X\202\314\211\234\202\263\202\361", "\203L\203U\226\354\230Y",
                          "\203u\203^\203L\203U\226\354\230Y", "\223X\202\314\216\345\220l", NULL};
    const char* hide[] = {"\203y\203\211\226\202\220l", NULL};
    s32 i;

    for (i = 0; show[i] != NULL; i++) {
        animPoseSetMaterialFlagOff(npcNameToPtr(show[i])->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    }
    for (i = 0; hide[i] != NULL; i++) {
        animPoseSetMaterialFlagOn(npcNameToPtr(hide[i])->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper run when Mario comes back from the storeroom: the reverse of ie2_npc_disp_off
 * (the shop's people ignore the map's blend colour again and the chest demon takes it).
 */
USER_FUNC(ie2_npc_disp_on) {
    const char* hide[] = {"\202\250\223X\202\314\211\234\202\263\202\361", "\203L\203U\226\354\230Y",
                          "\203u\203^\203L\203U\226\354\230Y", "\223X\202\314\216\345\220l", NULL};
    const char* show[] = {"\203y\203\211\226\202\220l", NULL};
    s32 i;

    for (i = 0; hide[i] != NULL; i++) {
        animPoseSetMaterialFlagOn(npcNameToPtr(hide[i])->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    }
    for (i = 0; show[i] != NULL; i++) {
        animPoseSetMaterialFlagOff(npcNameToPtr(show[i])->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: sets the talk hook script (fook_evt) of the ShopperData at args[0] to args[1];
 * the init script picks shopper_talk1 or shopper_talk2 by story progress.
 */
static USER_FUNC(set_fook_evt) {
    EvtWord* args = event->args;
    s32* work = (s32*)evtGetValue(event, args[0]);

    work[1] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: gives each pig NPC in booboo_name[] that is present in the room (up to eight) the "paper,
 * no ambient light" material flag.
 */
static USER_FUNC(booboo_main) {
    u32 i;

    for (i = 0; i < 8; i++) {
        if (npcNameToPtr_NoAssert(booboo_name[i].name) != NULL) {
            animPoseSetMaterialLightFlagOn(npcNameToPtr(booboo_name[i].name)->poseId, MATERIAL_LIGHT_PAPER_PLAIN);
        }
    }
    return EVT_RETURN_DONE;
}

/** Script helper that runs until blocked: each frame it hides any NPC parked at y <= -1000 and shows the rest. */
static USER_FUNC(npc_process) {
    NpcWork* wp = npcGetWorkPtr();
    NpcEntry* npc = wp->entries;
    s32 i;

    for (i = 0; i < wp->total; i++, npc++) {
        if (npc->flags & NPC_FLAG_IN_USE) {
            if (npc->position.y <= -1000.0f) {
                npc->flags |= NPC_FLAG_HIDE;
            } else {
                npc->flags &= ~NPC_FLAG_HIDE;
            }
        }
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Going from the shop into the storeroom (A at "A_usu01_ie2_door3", once it is unlocked):
 * the storeroom stays lit, the Inn Coupon ("item_05") is moved out of sight,
 * Mario and the partner walk through the door "S_usu01_ie2_door3", which swings open and shut;
 * the quarrel chatter stops, the shop's goods (once STG4_04) and people are dimmed,
 * the wall "S_usu01_ie2_mkabe" slides away and the door folds down.
 */
EVT_BEGIN(evt_ie2_door2_1)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_usu01_ie2_naka2"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_usu01_ie2_naka2"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_item_delete_check, STRING("item_05"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_05"), LW(0), LW(1), LW(2))
        CALL(evt_item_set_position, STRING("item_05"), LW(0), -1000, LW(2))
    END_IF
    CALL(evt_mario_mov_pos2, -213, -471, FLOAT(135.0f))
    CALL(evt_party_set_homing_dist, 0, FLOAT(20.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door3"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_party_door_move, -297, -416, FLOAT(135.0f))
    CALL(evt_party_move_behind_mario, 0)
    CALL(evt_party_set_homing_dist, 0, FLOAT(50.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door3"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    DELETE_EVENT(GW(3))
    CALL(evt_msg_close, STRING("stg4_usu_61_3"))
    CALL(evt_msg_close, STRING("stg4_usu_61_4"))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_04_STORAGE_ROOM_UNLOCKED)
        CALL(shop_item_disp_off)
    END_IF
    CALL(ie2_npc_disp_off)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_WALL_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 300, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_usu01_ie2_mkabe"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door3"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_FALL1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Going back from the storeroom into the shop (A at "A_usu01_ie2_door3_u"): the reverse of evt_ie2_door2_1.
 * The door stands up, the wall "S_usu01_ie2_mkabe" slides back,
 * the shop's goods (once STG4_04) and people are lit again and the quarrel chatter restarts;
 * Mario and the partner walk back through the door, and the Inn Coupon ("item_05") is put back.
 */
EVT_BEGIN(evt_ie2_door2_2)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
        CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door3"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_set_homing_dist, 0, FLOAT(20.0f))
    CALL(evt_party_move_behind_mario, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_WALL_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 300, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_usu01_ie2_mkabe"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_04_STORAGE_ROOM_UNLOCKED)
        CALL(shop_item_disp_on)
    END_IF
    CALL(ie2_npc_disp_on)
    SPAWN_EVENT_GET_ID(PTR(mise_kiza_msg), GW(3))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door3"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_party_door_move, -213, -471, FLOAT(135.0f))
    CALL(evt_party_move_behind_mario, 0)
    CALL(evt_party_set_homing_dist, 0, FLOAT(50.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door3"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_usu01_ie2_naka2"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_usu01_ie2_naka2"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_item_delete_check, STRING("item_05"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_05"), LW(0), LW(1), LW(2))
        CALL(evt_item_set_position, STRING("item_05"), LW(0), 0, LW(2))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Door script for house 3's door "S_usu01_ie3_door" (evtIn of door_3):
 * turns it about y by LW(0) scaled to door units.
 */
static EVT_BEGIN(evt_door_3_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie3_door"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door script for house 3's door (evtOut of door_3,
 * run with the door-fall sound as the house front opens or closes):
 * tilts "S_usu01_ie3_door" about x by half of LW(0) (0 to 180).
 */
static EVT_BEGIN(evt_door_3_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie3_door"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Wall script of house 3 (evtWall of door_3, LW(0) 0 to 180):
 * tilts the front panel "S_usu01_ie3_kai1" about x by -LW(0)/2,
 * then 10 frames later "S_usu01_ie3_kai2" by +LW(0)/2, so the second panel trails the first.
 */
static EVT_BEGIN(evt_wall_3)
    SET_FLOAT_VALUE(LW(1), LW(0))
    MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie3_kai1"), LW(1), 0, 0)
    WAIT_FRAMES(10)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie3_kai2"), LW(2), 0, 0)
    RETURN
EVT_END()

/** Item objects inside house 3: "item01"; door_3 lists them as the house's contents. */
const char* door_3_item[] = {
    "item01",
    NULL,
};

/** House 3 open callback: dips BGM and ambience to 50 and low-pass filters the environment sound. */
static EVT_BEGIN(door_3_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** House 3 close callback: restores BGM and ambience and removes the low-pass filter. */
static EVT_BEGIN(door_3_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/**
 * Door of house 3 ("S_usu01_ie3_door"): its collision, the inside and outside models and the open and close
 * scripts that dip the music and ambience while Mario is indoors, with the item list door_3_item.
 */
static DoorParam door_3 = {
    0, // flags
    3, // type
    2, // se
    "S_usu01_ie3_door", // mapobj
    "A_usu01_ie3_doaa_u", // hitIn
    "A_usu01_ie3_doaa", // hitOut
    "S_usu01_ie3", // outside
    NULL, // outsideHit
    "S_usu01_ie3_naka", // inside
    NULL, // insideHit
    evt_door_3_1, // evtIn
    evt_door_3_2, // evtOut
    evt_wall_3, // evtWall
    NULL, NULL, // npcs..mobjs
    (const char**)door_3_item, // items
    door_3_open, // evtOpen
    door_3_close, // evtClose
};

/**
 * Map exits of this part of Twilight Town: the east exit "e_bero" leads to gra_00 (Twilight Trail) and the
 * west exit "w_bero" leads back to usu_00.
 */
static BeroEntry bero_entry_data[] = {
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
        "gra_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
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
        "usu_00", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(usu01_gusun)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_16_LEAVING_TWILIGHT_TOWN)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_JIN_08_PARROT_SECRET_HEARD, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(3), 0)
        IF_INT_LESS_OR_EQUAL(LW(3), 100)
            IF_INT_LESS(GSW_STORY_PROGRESS, STG4_10_DOOPLISS_BATTLE)
                CALL(evt_msg_toge, MSG_TOGE_AT_POS, 37, 30, -244)
                CALL(evt_msg_print, 0, STRING("stg4_usu_51"), 0, 0)
            ELSE
                CALL(evt_msg_toge, MSG_TOGE_AT_POS, 37, 30, -244)
                CALL(evt_msg_print, 0, STRING("stg4_usu_61_1"), 0, 0)
                CALL(evt_msg_toge, MSG_TOGE_AT_POS, 37, 30, -244)
                CALL(evt_msg_print, 0, STRING("stg4_usu_61_2"), 0, 0)
            END_IF
            RETURN
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Cutscene body: replaces the gatekeeper with the gatekeeper pig at (400, 0, -55),
 * rings the temple bell and returns control.
 */
EVT_BEGIN(evt_monban_buta_main)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("ブタ門番"), 400, 0, -55)
    CALL(evt_npc_set_position, STRING("門番"), 0, -2000, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG4_TEMPLE_BELL1"), 0)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, STG4_02_DINGE)
    RETURN
EVT_END()

/**
 * Map-entry trigger, spawned by the init script at STG4_01 once the shop woman has made her plea:
 * waits for Mario's entry move to finish (evt_bero_exec_wait),
 * then runs the gatekeeper-turns-pig scene (evt_monban_buta_main) unless STG4_02_DINGE is reached.
 */
EVT_BEGIN(evt_monban_buta)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_02_DINGE)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    CALL_CHILD(PTR(evt_monban_buta_main))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Variant of evt_monban_buta spawned by the shop's close callback (mawari_house_delay):
 * waits one second instead of for the entry move.
 */
EVT_BEGIN(evt_monban_buta2)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_02_DINGE)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1000)
    CALL_CHILD(PTR(evt_monban_buta_main))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Watcher loop: when Mario nears the gatekeeper pig after STG4_02_DINGE,
 * the party comments ("stg4_usu_47") and the story moves to STG4_02_02_THE_GATEKEEPERS_A_PIG.
 */
EVT_BEGIN(evt_monban_look)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_02_02_THE_GATEKEEPERS_A_PIG)
            RETURN
        END_IF
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_02_DINGE)
            SET_VALUE(LW(0), 420)
            SET_VALUE(LW(1), 0)
            SET_VALUE(LW(2), 0)
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(6))
            IF_INT_LESS(LW(6), 100)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_mario_normalize)
                CALL(evt_party_yoshi_ride, 0, 0)
                CALL(evt_mario_wait_rideon)
                CALL(evt_mario_set_dir_npc, STRING("ブタ門番"))
                CALL(evt_party_set_dir_npc, 0, STRING("ブタ門番"))
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
                CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
                WAIT_MS(1000)
                CALL(evt_party_stop, 0)
                WAIT_MS(200)
                CALL(evt_msg_print_party, STRING("stg4_usu_47"))
                CALL(evt_party_run, 0)
                CALL(evt_mario_key_onoff, 1)
                SET_VALUE(GSW_STORY_PROGRESS, STG4_02_02_THE_GATEKEEPERS_A_PIG)
                RETURN
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Item menu offered at the storeroom lock: only the Shop Key (-1 ends the list). */
static s32 lock_tbl[] = {
    ITEM_SHOP_KEY, -1,
};

/**
 * Check script of the storeroom lock ("usu_00_lock"):
 * with the Shop Key in the pouch Mario picks it from the item menu (backing out cancels the unlock);
 * without it, once the shopkeeper's wife has made her plea she turns to Mario,
 * says her husband must have lost the key in the woods (stg4_usu_57) and the unlock is cancelled.
 */
static EVT_BEGIN(lock_item_check)
    CALL(evt_pouch_check_item, ITEM_SHOP_KEY, LW(0))
    IF_INT_GREATER(LW(0), 0)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_win_item_select, PTR(lock_tbl), LW(0))
#else
        CALL(evt_win_item_select, 0, PTR(lock_tbl), LW(0), 0)
#endif
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    ELSE
        IF_INT_EQUAL(GSWF_USU_01_SHOP_WIFE_FIRST_TALK_DONE, 1)
            CALL(evt_set_dir_to_target, STRING("お店の奥さん"), STRING("mario"))
            CALL(evt_msg_print, 0, STRING("stg4_usu_57"), 0, STRING("お店の奥さん"))
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Unlocks the shop storage door: runs the door script when Mario faces door A_usu01_ie2_door3
 * and sets STG4_04_STORAGE_ROOM_UNLOCKED.
 */
EVT_BEGIN(unlock)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_usu01_ie2_door3"), 0, PTR(evt_ie2_door2_1), 0)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_04_STORAGE_ROOM_UNLOCKED)
    RETURN
EVT_END()

/** Shop display objects: the six item stands (map object and hit area) of the Twilight Town shop. */
static ShopObj obj_list[] = {
    {"mi02_S_item_01", "A_item_01"},
    {"mi02_mi02_S_item_02", "A_item_02"},
    {"mi02_S_item_03", "A_item_03"},
    {"mi02_S_item_04", "A_item_04"},
    {"mi02_S_item_05", "A_item_05"},
    {"mi02_S_item_06", "A_item_06"},
};

static ShopItem goods_list[] = {
    {ITEM_THUNDER_RAGE, 20},
#if VERSION <= VERSION_DEMO_US
    {ITEM_BOOS_SHEET, 10},
#else
    {ITEM_SPITE_POUCH, 10},
#endif
    {ITEM_STOPWATCH, 30},
    {ITEM_MAPLE_SYRUP, 20},
    {ITEM_SUPER_SHROOM, 15},
    {ITEM_LIFE_SHROOM, 40},
};

static ShopDb trade_list[] = {
    {ITEM_THUNDER_RAGE, 10, 0},
#if VERSION <= VERSION_DEMO_US
    {ITEM_BOOS_SHEET, 5, 0},
#else
    {ITEM_SPITE_POUCH, 5, 0},
#endif
    {ITEM_STOPWATCH, 15, 0},
    {ITEM_MAPLE_SYRUP, 10, 0},
    {ITEM_SUPER_SHROOM, 7, 0},
    {ITEM_LIFE_SHROOM, 20, 0},
    {ITEM_MISTAKE, 3, 0},
    {ITEM_DRIED_BOUQUET, 14, 0},
    {ITEM_DRIED_SHROOM, 4, 0},
    {ITEM_JELLY_ULTRA, 200, 0},
    {ITEM_HOT_SAUCE, 12, 0},
    {0},
};

/**
 * Shopper data of the shop woman (the shopkeeper's wife): her NPC name,
 * no talk or buy hooks of its own (the init script adds them by story progress) and the shop's message
 * keys "usugurashop_00" onward.
 */
static ShopperData35 shopper_data = {
    "お店の奥さん", // name
    NULL, NULL, // fook_evt..buy_evt
    {"usugurashop_00", "usugurashop_01", "usugurashop_02", "usugurashop_03", "usugurashop_04", "usugurashop_05", "usugurashop_06", "usugurashop_07", "usugurashop_08", "usugurashop_09", "usugurashop_10", "usugurashop_11", "usugurashop_12", "usugurashop_13", "usugurashop_14", "usugurashop_15", "usugurashop_16", "usugurashop_17", "usugurashop_18", "usugurashop_19", "usugurashop_20", "usugurashop_21", "usugurashop_22", "usugurashop_23", "usugurashop_24", "usugurashop_25", "usugurashop_26", "usugurashop_27", "usugurashop_28", "usugurashop_29", "usugurashop_30", "usugurashop_31", "usugurashop_32", "usugurashop_33", "usugurashop_34"}, // msgs
};

/**
 * Talk hook (fook_evt) of the shop woman from the storeroom unlock to the Doopliss fight:
 * she begs Mario to restore her husband (stg4_usu_59);
 * LW(0) = -100 makes the shop talk continue in the same message box.
 */
EVT_BEGIN(shopper_talk1)
    CALL(evt_msg_print, 0, STRING("stg4_usu_59"), 0, STRING("me"))
    SET_VALUE(LW(0), -100)
    RETURN
EVT_END()

/**
 * Talk hook of the shop woman from the Doopliss fight to STG4_15_03:
 * the pig was not her husband, and she fancies the younger man (stg4_usu_60);
 * LW(0) = -100 as in shopper_talk1.
 */
EVT_BEGIN(shopper_talk2)
    CALL(evt_msg_print, 0, STRING("stg4_usu_60"), 0, STRING("me"))
    SET_VALUE(LW(0), -100)
    RETURN
EVT_END()

/**
 * NPC names of the pig forms (the dandy, the aunt, the gatekeeper, villagers I and J and the three young women)
 * that booboo_main lights without scene ambient light.
 */
static BoobooName booboo_name[] = {
    {"ブタキザ野郎", 7, 0},
    {"ブタおばさん", 1, 0},
    {"ブタ門番", 3, 1},
    {"ブタ村人Ｉ", 5, 1},
    {"ブタ村人Ｊ", 4, 1},
    {"ブタ３人娘Ａ", 4, 1},
    {"ブタ３人娘Ｂ", 4, 1},
    {"ブタ３人娘Ｃ", 4, 1},
};

/** Empty script (just returns) that nothing in the tree uses. Guess: by its name, meant for getting the tube ("roll") curse. */
EVT_BEGIN(evt_roll_get)
    RETURN
EVT_END()

/** Item menu offered at the black chest: only the Black Key (-1 ends the list). */
static s32 key_tbl[] = {
    ITEM_BLACK_KEY_0x23, -1,
};

static EVT_BEGIN(evt_majin)
    // Mario's controls are off; he and his partner walk up to the black chest.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_adjust_dir)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -260, -395, FLOAT(80.0f))
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -235, -370, FLOAT(80.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    IF_INT_EQUAL(GSWF_USU_01_BLACK_CHEST_FIRST_TALK_DONE, 0)
        // First visit: a voice inside the chest startles Mario (stg4_usu_majin3_01).
        CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_01"), 0, 0)

        // The chest rattles. The voice insists it is not like other black chests (majin3_02 to 04),
        // then begs to be let out (05 to 07) while Mario shrugs it off.
        CALL(evt_mobj_get_position, STRING("pera_box"), LW(3), LW(4), LW(5))
        ADD(LW(4), 30)
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLACKBOX_SWING1"), LW(3), LW(4), LW(5), LW(15))
        CALL(evt_mobj_set_anim, STRING("pera_box"), STRING("A_3"))
        CALL(evt_mobj_wait_animation_end, STRING("pera_box"))
        CALL(evt_snd_sfxoff, LW(15))
        WAIT_MS(300)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_02"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_G"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_03"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_04"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_05"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_06"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_Z"))
        WAIT_MS(1600)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_07"), PTR(majin_callback), 0)

        // The chest is left alone and the first talk is recorded.
        CALL(evt_mobj_exec_cancel, STRING("pera_box"))
        SET_VALUE(GSWF_USU_01_BLACK_CHEST_FIRST_TALK_DONE, 1)
    ELSE
        // Later visits: without the Black Key the voice just begs Mario to find it (majin3_08).
        CALL(evt_pouch_check_item, ITEM_BLACK_KEY_0x23, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
            ADD(LW(1), 30)
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_08"), PTR(majin_callback), 0)
            CALL(evt_mobj_exec_cancel, STRING("pera_box"))

            // With the Black Key the voice is overjoyed (majin3_09) and Mario may pick the key from the item
            // menu; declining draws a shocked "No way!" (majin3_10).
        ELSE
            CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
            ADD(LW(1), 30)
            CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_09"), PTR(majin_callback), 0)
#if VERSION <= VERSION_DEMO_US
            CALL(evt_win_item_select, PTR(key_tbl), LW(0))
#else
            CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
#endif
            IF_INT_EQUAL(LW(0), -1)
                CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
                CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_10"), PTR(majin_callback), 0)
                CALL(evt_mobj_exec_cancel, STRING("pera_box"))
            ELSE
                // Giving the key uses it up, and the demon cackles that Mario is a fool (majin3_11).
                CALL(evt_pouch_remove_item, ITEM_BLACK_KEY_0x23, LW(0))
                CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
                CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_11"), PTR(majin_callback), 0)
#if VERSION > VERSION_DEMO_US
                CALL(evt_mario_set_pose, STRING("M_I_N"))
#endif
            END_IF
        END_IF
    END_IF

    // The partner is released and Mario's controls come back.
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_majin2)
    CALL(evt_mario_key_onoff, 0)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_mario_normalize)
#endif
    CALL(evt_party_stop, 0)
    CALL(evt_mobj_wait_animation_end, STRING("pera_box"))
    CALL(evt_pouch_add_item, ITEM_TUBE_CURSE, LW(0))
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_PERAMAJIN1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(bgm_start_wait)
    WAIT_MS(1000)
    CALL(evt_paper_entry, STRING("OFF_d_kusya"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_kusya"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_set_virtual_point, STRING("img"), 0, 0, 100)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_img_clear_virtual_point, STRING("img"))
    END_INLINE_EVENT
    CALL(evt_img_onoff, STRING("img"), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_APPEAR1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_COME1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_mario_set_camid, CAMERA_3D_EFFECTB)
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(lect_cam_save)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    WAIT_FRAMES(1)
    CALL_CHILD(PTR(roll_majin_lecture_init))
    CALL_CHILD(PTR(roll_majin_lecture))
    LOOP(6)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_PERAMAJIN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mario_init_camid)
    CALL(lect_cam_load, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_GOOUT1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_2"))
    CALL(evt_img_wait_animend, STRING("img"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PERAMAJIN_GOOUT2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_kusya"))
    SET_VALUE(GSW_STORY_PROGRESS, STG4_04_01_PAPER_MAGE_TUBE_MOD)
    SET_VALUE(GSWF_USU_01_BLACK_CHEST_OPENED, 1)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Message callback for the black chest's ("pera_box") lines:
 * starts its A_2 animation when the message opens (type 0),
 * restarts it each time it has looped while the text runs (type 5) and returns it to S_1
 * when the message ends (type 2).
 */
static void majin_callback(s32 type) {
    MapObjectEntry* mobj = mobjNameToPtr("pera_box");

    if (type == 0) {
        animPoseSetAnim(mobj->poseId, "A_2", TRUE);
    } else if (type == 5) {
        if (animPoseGetLoopTimes(mobj->poseId) >= 1.0f) {
            animPoseSetAnim(mobj->poseId, "A_2", TRUE);
        }
    } else if (type == 2) {
        animPoseSetAnim(mobj->poseId, "S_1", TRUE);
    }
}

/** Script helper: turns the "pera_box" chest to face 135 degrees. */
USER_FUNC(box_r) {
    MapObjectEntry* mobj = mobjNameToPtr("pera_box");

    mobj->rotation.y = 135.0f;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}


/** Script helper: blocks until the channel 1 BGM has started. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/** Script helper: turns the "mobj_box00" chest to face 135 degrees. */
USER_FUNC(mobj_ry) {
    MapObjectEntry* mobj = mobjNameToPtr("mobj_box00");

    mobj->rotation.y = 135.0f;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}

/**
 * Script: shows or hides a tree map object depending on Mario's distance (LW(10) object, LW(11) radius),
 * toggling LF(0).
 */
EVT_BEGIN(tree_onoff)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mapobj_get_position, LW(10), LW(3), LW(4), LW(5))
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(9))
        IF_INT_GREATER(LW(9), LW(11))
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
                SET_VALUE(LF(0), 0)
            END_IF
        ELSE
            IF_INT_EQUAL(LF(0), 0)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(10), MAP_OBJ_FLAG_HIDDEN)
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Hammer hook of tree "S_usu01_ki01" (tree_tbl): crow J plays A_1 for 1.5 seconds, then S_1. Guess: A_1 is the crow flapping. */
static EVT_BEGIN(evt_ki_1)
    CALL(evt_npc_set_anim, STRING("カラスＪ"), STRING("A_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カラスＪ"), STRING("S_1"))
    RETURN
EVT_END()

/**
 * Hammer hook of tree "S_usu01_ki02" (tree_tbl): crows H and I play A_1 for 1.5 seconds, then S_1.
 * Guess: A_1 is the crows flapping.
 */
static EVT_BEGIN(evt_ki_2)
    CALL(evt_npc_set_anim, STRING("カラスＨ"), STRING("A_1"))
    CALL(evt_npc_set_anim, STRING("カラスＩ"), STRING("A_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カラスＨ"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("カラスＩ"), STRING("S_1"))
    RETURN
EVT_END()

/**
 * Hammer hook of tree "S_usu01_ki03" (tree_tbl): crows F and G play A_1 for 1.5 seconds, then S_1.
 * Guess: A_1 is the crows flapping.
 */
static EVT_BEGIN(evt_ki_3)
    CALL(evt_npc_set_anim, STRING("カラスＦ"), STRING("A_1"))
    CALL(evt_npc_set_anim, STRING("カラスＧ"), STRING("A_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カラスＦ"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("カラスＧ"), STRING("S_1"))
    RETURN
EVT_END()

/**
 * Shakeable trees of this part of Twilight Town: three whose hammer hook startles crows F to J and a fourth
 * with no hook; the type 7 row ends the list.
 */
static TreeAccess tree_tbl[] = {
    {2, "A_ki_1", "S_usu01_ki01", NULL, 0, evt_ki_1},
    {2, "A_ki_2", "S_usu01_ki02", NULL, 0, evt_ki_2},
    {2, "A_ki_3", "S_usu01_ki03", NULL, 0, evt_ki_3},
    {2, "A_ki_4", "S_usu01_ki05", NULL, 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

/** Registers tree_tbl with the shared tree-shaking script. */
EVT_BEGIN(usu_01_tree_init)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    RETURN
EVT_END()

/**
 * Opening script of the item chest "mobj_box00":
 * gives the Defend Plus badge as a fixed item with the pickup fanfare (badge-return flag outside the early
 * demos).
 */
static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_DEFEND_PLUS, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH_GET)
#else
    CALL(evt_item_flag_on, STRING("item00"), ITEM_FLAG_NO_TOUCH_GET | ITEM_FLAG_LOST_BADGE_TO_SHOP)
#endif
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init script of usu_01, the part of Twilight Town whose east exit leads to gra_00:
 * starts BGM and ambience, registers the exits and NPCs,
 * spawns the three young women (pigs before the Doopliss fight, dancers until STG4_15_05),
 * locks the storeroom with the Shop Key until it is unlocked, registers the three house doors,
 * and starts the gatekeeper-pig trigger, the shop sounds, the shop (from STG4_04) and the crow watchers.
 * It also sets up the town gate, trees, items, the Defend Plus chest and the black chest by story progress.
 */
EVT_BEGIN(usu_01_init_evt)
    // Start the town music and ambience, set up the map exits and spawn the NPCs.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_npc_setup, PTR(npcEnt))

    // The three young women appear as pigs before the Doopliss fight and as dancers until the Peach report.
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_00_WELCOME_TO_STAGE_4, STG4_10_DOOPLISS_BATTLE)
        CALL_CHILD(PTR(musume_a_pig_entry))
        CALL_CHILD(PTR(musume_b_pig_entry))
        CALL_CHILD(PTR(musume_c_pig_entry))
        CASE_BETWEEN(STG4_10_DOOPLISS_BATTLE, STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL_CHILD(PTR(musume_a_entry))
        CALL_CHILD(PTR(musume_b_entry))
        CALL_CHILD(PTR(musume_c_entry))
    END_SWITCH
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_04_STORAGE_ROOM_UNLOCKED)
        CALL(evt_mobj_lock, STRING("usu_00_lock"), ITEM_SHOP_KEY, FLOAT(-263.5f), FLOAT(10.099609f), FLOAT(-432.0498f), 135, PTR(lock_item_check), PTR(unlock), GSWF_USU_01_STOREROOM_UNLOCKED)
    ELSE
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_usu01_ie2_door3"), 0, PTR(evt_ie2_door2_1), 0)
    END_IF

    // Register the doors of the three houses.
    SET_VALUE(LW(0), PTR(&door_1))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_2_1))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_3))
    CALL_CHILD(PTR(evt_door_entry))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_usu01_ie2_door3_u"), 0, PTR(evt_ie2_door2_2), 0)

    // Gatekeeper-pig scenes, the shop sounds and the shop itself (from the storeroom unlock on).
    SPAWN_EVENT(PTR(evt_monban_look))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_01_THE_MAYORS_A_PIG)
        IF_INT_EQUAL(GSWF_USU_01_SHOP_WIFE_FIRST_TALK_DONE, 1)
            SPAWN_EVENT(PTR(evt_monban_buta))
        END_IF
    END_IF
    SPAWN_EVENT_GET_ID(PTR(usu01_gusun), GW(4))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_04_STORAGE_ROOM_UNLOCKED)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG4_10_DOOPLISS_BATTLE)
            CALL(set_fook_evt, PTR(&shopper_data), PTR(shopper_talk1))
        ELSE
            IF_INT_LESS(GSW_STORY_PROGRESS, STG4_15_03_THE_RUBY_STAR)
                CALL(set_fook_evt, PTR(&shopper_data), PTR(shopper_talk2))
            END_IF
        END_IF
        CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    END_IF
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("mi02_S_items"), MAP_OBJ_FLAG_DISABLED)

    // Reset the crow gates and start the watchers that let the crows talk when Mario hides in Vivian's
    // shadow.
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    SET_VALUE(GW(2), 0)
    SPAWN_EVENT(PTR(usu01_check_vivian))
    SPAWN_EVENT(PTR(usu01_crowFG_talk_event))
    SPAWN_EVENT(PTR(usu01_crowHI_talk_event))
    SPAWN_EVENT(PTR(usu01_crowJ_talk_event))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_02_03_OPENING_THE_GATE)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_mon"), 0, PTR(evt_mon_open), 0)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_monato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_mon02"), 1, 0)
    ELSE
        CALL(evt_mapobj_rotate, 1, STRING("S_mon01"), 0, 50, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_mon02"), 0, -85, 0)
        CALL(evt_hitobj_onoff, STRING("A_mon"), 1, 0)
    END_IF

    // Light the pig NPCs, set up the trees and start the tree-distance check.
    BEGIN_INLINE_EVENT
        CALL(booboo_main)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki1"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki2"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki3"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki4"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("ki5"), MAP_OBJ_FLAG_CLIP)
    CALL_CHILD(PTR(usu_01_tree_init))
    SET_VALUE(LW(10), STRING("ki3"))
    SET_VALUE(LW(11), 450)
    SPAWN_EVENT(PTR(tree_onoff))

    // Place the items: a Star Piece, a Life Shroom, a Boo's Sheet,
    // a Jammin' Jelly and an Inn Coupon, plus the Defend Plus chest.
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, 210, 0, 265, ITEM_TYPE_STILL, GSWF_STARPIECE_63, 0)
    CALL(evt_item_entry, STRING("item_02"), ITEM_LIFE_SHROOM, -275, 0, -310, ITEM_TYPE_STILL, GSWF_USU_01_LIFE_SHROOM, 0)
    CALL(evt_item_entry, STRING("item_03"), ITEM_BOOS_SHEET, -305, 0, -380, ITEM_TYPE_STILL, GSWF_USU_01_BOOS_SHEET, 0)
    CALL(evt_item_entry, STRING("item_04"), ITEM_JAMMIN_JELLY, -335, 0, -410, ITEM_TYPE_STILL, GSWF_USU_01_JAMMIN_JELLY, 0)
    CALL(evt_item_entry, STRING("item_05"), ITEM_INN_COUPON, -250, 0, -500, ITEM_TYPE_STILL, GSWF_USU_01_INN_COUPON, 0)
    CALL(evt_mobj_itembox, STRING("mobj_box00"), -255, 0, -288, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box00), GSWF_USU_01_DEFEND_PLUS_CHEST)
    CALL(mobj_ry)
    BEGIN_INLINE_EVENT
        CALL(npc_process)
    END_INLINE_EVENT

    // The black chest holds the Tube Curse and opens with the Black Key;
    // the aunt's Meteor Meal is placed if trouble 20 is solved.
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_04_01_PAPER_MAGE_TUBE_MOD)
        SET_VALUE(GSWF_USU_01_BLACK_CHEST_OPENED, 1)
    END_IF
    CALL(evt_mobj_itembox, STRING("pera_box"), -289, 0, -341, MOBJ_ITEMBOX_BLACK, PTR(evt_majin), PTR(evt_majin2), GSWF_USU_01_BLACK_CHEST_OPENED)
    CALL(evt_mobj_flag_onoff, 0, STRING("pera_box"), MOBJ_FLAG_GRAVITY)
    CALL(box_r)
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_TELL_THAT_PERSON), 1)
        CALL(evt_item_entry, STRING("iri_20"), ITEM_METEOR_MEAL, -345, 20, -132, ITEM_TYPE_STILL, GSWF_USU_01_METEOR_MEAL, 0)
    END_IF
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    RETURN
EVT_END()


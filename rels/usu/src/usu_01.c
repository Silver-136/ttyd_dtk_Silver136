#include "version.h"
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

extern ItemData itemDataTable[];

static USER_FUNC(bgm_start_wait);
static USER_FUNC(booboo_main);
USER_FUNC(box_r);
USER_FUNC(check_tabemono);
USER_FUNC(chk_fuki);
USER_FUNC(delete_item_list);
USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_pouch_get_haveitemcnt);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_win_item_select);
USER_FUNC(get_item_list);
USER_FUNC(ie2_npc_disp_off);
USER_FUNC(ie2_npc_disp_on);
USER_FUNC(mainwindow_opendisable);
USER_FUNC(mainwindow_openenable);
USER_FUNC(mobj_ry);
static USER_FUNC(npc_process);
static USER_FUNC(set_fook_evt);
USER_FUNC(shop_item_disp_off);
USER_FUNC(shop_item_disp_on);
USER_FUNC(sleep_voice);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
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
EvtWord aunt_talk[687];
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
EvtWord village_chief_change_pig_event[166];
EvtWord evt_mon_open[112];
static EvtWord cam_up[34];
static EvtWord cam_down[8];
EvtWord usu01_check_vivian[36];
EvtWord usu01_crowFG_talk_event[VERSION_PICK(355, 394)];
EvtWord usu01_crowHI_talk_event[VERSION_PICK(250, 289)];
EvtWord usu01_crowJ_talk_event[VERSION_PICK(226, 265)];
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
static EvtWord lock_item_check[41];
EvtWord unlock[13];
static ShopObj obj_list[6];
static ShopItem goods_list[6];
static ShopDb trade_list[12];
static ShopperData35 shopper_data;
EvtWord shopper_talk1[11];
EvtWord shopper_talk2[11];
typedef struct BoobooName { const char* name; u8 field_0x4; u8 field_0x5; u8 pad_0x6[2]; } BoobooName; /* field_0x4/0x5 never read */
static BoobooName booboo_name[8];
EvtWord evt_roll_get[2];
static s32 key_tbl[2];
static EvtWord evt_majin[330];
static EvtWord evt_majin2[237];
EvtWord tree_onoff[57];
static EvtWord evt_ki_1[12];
static EvtWord evt_ki_2[20];
static EvtWord evt_ki_3[20];
static TreeAccess tree_tbl[5];
EvtWord usu_01_tree_init[7];
static EvtWord mobj_box00[34];
EvtWord usu_01_init_evt[350];

EVT_BEGIN(villagerF_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｆ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(villagerF_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_msg_print, 0, STRING("stg4_usu_82"), 0, STRING("me"))
        CASE_LESS(219)
        CALL(evt_msg_print, 0, STRING("stg4_usu_30"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("stg4_usu_30_yn"))
        CALL(evt_msg_print_add, 0, STRING("stg4_usu_30_00"))
        CASE_LESS(222)
        CALL(evt_msg_print, 0, STRING("stg4_usu_84"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg4_usu_84_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_84_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(villagerG_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｇ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(villagerG_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_msg_print, 0, STRING("stg4_usu_85"), 0, STRING("me"))
        CASE_LESS(219)
        CALL(evt_msg_print, 0, STRING("stg4_usu_86"), 0, STRING("me"))
        CALL(evt_msg_select, 0, STRING("stg4_usu_86_yn"))
        CALL(evt_msg_print_add, 0, STRING("stg4_usu_86_00"))
        CASE_LESS(222)
        CALL(evt_msg_print, 0, STRING("stg4_usu_87"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg4_usu_87_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_87_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(villagerH_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｈ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(villagerH_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_msg_print, 0, STRING("stg4_usu_88"), 0, STRING("me"))
        CASE_LESS(219)
        CALL(evt_msg_print, 0, STRING("stg4_usu_89"), 0, STRING("me"))
        CASE_LESS(222)
        CALL(evt_msg_print, 0, STRING("stg4_usu_90"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg4_usu_90_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_90_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(villagerI_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｉ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_set_position, STRING("村人Ｉ"), 0, -2000, 0)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(villagerI_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｉ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村人Ｉ"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(villagerI_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_91"), 0, STRING("me"))
        CASE_LESS(219)
        CALL(evt_msg_print, 0, STRING("stg4_usu_92"), 0, STRING("me"))
        CASE_LESS(222)
        CALL(evt_msg_print, 0, STRING("stg4_usu_93"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg4_usu_93_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_93_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(villagerJ_init)
    CALL(evt_npc_flag_onoff, 1, STRING("村人Ｊ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_set_position, STRING("村人Ｊ"), 0, -2000, 0)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(villagerJ_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ村人Ｊ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ村人Ｊ"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(villagerJ_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_94"), 0, STRING("me"))
        CASE_LESS(219)
        CALL(evt_msg_print, 0, STRING("stg4_usu_95"), 0, STRING("me"))
        CASE_LESS(222)
        CALL(evt_msg_print, 0, STRING("stg4_usu_96"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg4_usu_96_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_96_1_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(shop_man_init)
    CALL(evt_npc_flag_onoff, 1, STRING("店の主人"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CASE_LESS(219)
        CALL(evt_npc_set_position, STRING("店の主人"), -125, 0, -475)
        CALL(evt_npc_set_ry, STRING("店の主人"), 90)
        CALL(evt_npc_flag_onoff, 1, STRING("店の主人"), NPC_FLAG_RESUME_AFTER_TALK)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("店の主人"), -130, 0, -480)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(shop_man_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(219)
        CALL(evt_msg_print, 0, STRING("stg4_usu_61_5"), 0, STRING("店の主人"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg4_usu_61_6"), 0, STRING("店の主人"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_61_7"), 0, STRING("店の主人"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(shop_woman_init)
    CALL(evt_npc_flag_onoff, 1, STRING("お店の奥さん"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(shop_woman_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(184)
        IF_INT_EQUAL(GSWF(1925), 0)
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
            SET_VALUE(GSWF(1925), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("stg4_usu_56"), 0, STRING("me"))
        END_IF
        CASE_LESS(185)
        CALL(evt_msg_print, 0, STRING("stg4_usu_58"), 0, STRING("me"))
        CASE_LESS(200)
        CALL(evt_msg_print, 0, STRING("stg4_usu_58_01"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_59"), 0, STRING("me"))
        CALL(evt_msg_continue)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(kizayarou_init)
    CALL(evt_npc_flag_onoff, 1, STRING("キザ野郎"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_set_position, STRING("キザ野郎"), 0, -2000, 0)
        CASE_LESS(219)
        CALL(evt_npc_set_position, STRING("キザ野郎"), -100, 0, -450)
        CALL(evt_npc_flag_onoff, 1, STRING("キザ野郎"), NPC_FLAG_RESUME_AFTER_TALK)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("キザ野郎"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kizayarou_pig_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタキザ野郎"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(kizayarou_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_61_00"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_61"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

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
        CASE_LESS(200)
        IF_INT_EQUAL(GSWF(1927), 1)
            CALL(evt_npc_set_position, STRING("おばさん"), 0, -2000, 0)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(aunt_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタおばさん"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        IF_INT_EQUAL(GSWF(1927), 0)
            CALL(evt_npc_set_position, STRING("ブタおばさん"), 0, -2000, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタおばさん"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(iri_20_out_house)
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set_xyz, -420, 1, 0, 0, 150, 11)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(14), 0)
        SPAWN_EVENT(PTR(door_light2))
    END_IF
    SPAWN_EVENT(PTR(door_up_se))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
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
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(9), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
            SPAWN_EVENT(PTR(house_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
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
    IF_INT_EQUAL(LW(14), 0)
        SPAWN_EVENT(PTR(door_light))
    END_IF
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
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
    CALL(evt_door_param, LW(0), 5, LW(1), LW(2), LW(3))
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
    WAIT_MS(2000)
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(PTR(door_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
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
    CALL(evt_cam3d_evt_xyz_off, 100, 11)
    CALL(door_dark_flag, LW(0), 0)
    CALL(evt_door_param, LW(0), 3, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        IF_INT_NOT_EQUAL(LW(15), 1)
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(1), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), 0)
        CALL(evt_hitobj_onoff, LW(2), 1, 0)
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
    END_IF
    CALL(evt_door_param, LW(0), 1, LW(1), LW(2), LW(3))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND)
        CALL(evt_mapobj_flag_onoff, 1, 0, LW(1), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_IF
    CALL(evt_door_param, LW(0), 2, LW(1), LW(2))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(evt_door_load_mapflag, LW(1))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(iri_20_aunt_to_house)
    CALL(evt_npc_move_position, STRING("おばさん"), -390, -40, 0, 60, 0)
    SET_VALUE(LW(0), PTR(&door_1))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_move_position, STRING("me"), -390, -20, 0, 60, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
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
    CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
    IF_INT_NOT_EQUAL(LW(7), 0)
        BEGIN_INLINE_EVENT
            SPAWN_EVENT(PTR(door_close_se))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_door_param, LW(0), 6, LW(7), LW(8), LW(9))
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
            CALL(evt_msg_print, 0, STRING("irai_20_001"), 0, STRING("me"))
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
            CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
            WAIT_MS(1000)
            BEGIN_INLINE_EVENT
                WAIT_MS(2000)
                CALL(evt_snd_bgmoff, PSND_PAUSE)
                CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_ORGEL1"))
            END_INLINE_EVENT
            CALL(evt_msg_print, 0, STRING("irai_20_002"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_N_1"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("irai_20_003"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 3000)
            CALL_CHILD(PTR(iri_20_aunt_to_house))
            CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
            CALL(evt_snd_bgmon_f, PSND_CONTINUE | PSND_FADEIN, 0, 2000)
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
            CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
            CALL(evt_snd_env_lpf, 0, 0)
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            ELSE
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                CALL(evt_msg_print, 0, STRING("irai_20_004"), 0, STRING("me"))
                ELSE
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
                BEGIN_INLINE_EVENT
                    CALL(evt_msg_print, 0, STRING("irai_20_009"), 0, STRING("子供Ａ"))
                END_INLINE_EVENT
                BEGIN_INLINE_EVENT
                    WAIT_MS(500)
                    CALL(evt_msg_print, 0, STRING("irai_20_010"), 0, STRING("子供Ｂ"))
                END_INLINE_EVENT
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("irai_20_011"), 0, STRING("子供Ｃ"))
                CALL(evt_msg_print, 0, STRING("irai_20_012"), 0, STRING("me"))
                CALL(evt_pouch_get_haveitemcnt, LW(0))
                CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
                IF_INT_EQUAL(LW(1), 0)
                    SET_VALUE(LW(1), 10)
                    ELSE
                    SET_VALUE(LW(1), 20)
                END_IF
                IF_INT_LESS(LW(0), LW(1))
                    CALL(evt_msg_continue)
                    CALL(evt_item_entry, STRING("sd"), ITEM_METEOR_MEAL, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                    CALL(evt_item_get_item, STRING("sd"))
                    CALL(evt_item_get_item_end_wait, STRING("sd"))
                    SET_VALUE(GSWF(1938), 1)
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("irai_20_012_1"))
                    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
                    CALL(evt_npc_move_position, STRING("me"), -363, -152, 0, 40, 0)
                    WAIT_MS(300)
                    CALL(evt_item_entry, STRING("iri_20"), ITEM_METEOR_MEAL, -345, 20, -132, ITEM_TYPE_STILL, GSWF(1938), 0)
                    WAIT_MS(300)
                    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, 40, 0)
                    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
                END_IF
                CALL(evt_msg_print, 0, STRING("irai_20_014"), 0, STRING("me"))
                CALL(evt_sub_save_playtime, GSW(1453))
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
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        IF_INT_EQUAL(GSWF(1927), 0)
            CALL(evt_msg_print, 0, STRING("stg4_usu_62"), 0, STRING("me"))
            CALL_CHILD(PTR(childABC_talk_01))
            WAIT_MS(1400)
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
                    CALL(evt_win_item_select, 0, LW(0), LW(1), 0)
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
            SET_VALUE(GSWF(1927), 1)
            ELSE
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
            CALL(evt_msg_print, 0, STRING("stg4_usu_73_01"), 0, STRING("me"))
        END_IF
        CASE_LESS(219)
        CALL(evt_msg_print, 0, STRING("stg4_usu_77"), 0, STRING("me"))
        CASE_LESS(222)
        CALL(evt_msg_print, 0, STRING("stg4_usu_78"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg4_usu_81_1"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_81_4_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(childA_init)
    CALL(evt_npc_flag_onoff, 1, STRING("子供Ａ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(childA_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        IF_INT_EQUAL(GSWF(1927), 1)
            CALL(evt_msg_print, 0, STRING("stg4_usu_74"), 0, STRING("me"))
            ELSE
            CALL_CHILD(PTR(childABC_talk_01))
        END_IF
        CASE_LESS(222)
        CALL_CHILD(PTR(childABC_talk_02))
        CASE_LESS_OR_EQUAL(402)
        CALL_CHILD(PTR(childABC_talk_03))
        CASE_DEFAULT
        CALL_CHILD(PTR(childABC_talk_04))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(childB_init)
    CALL(evt_npc_flag_onoff, 1, STRING("子供Ｂ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(childB_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        IF_INT_EQUAL(GSWF(1927), 1)
            CALL(evt_msg_print, 0, STRING("stg4_usu_75"), 0, STRING("me"))
            ELSE
            CALL_CHILD(PTR(childABC_talk_01))
        END_IF
        CASE_LESS(222)
        CALL_CHILD(PTR(childABC_talk_02))
        CASE_LESS_OR_EQUAL(402)
        CALL_CHILD(PTR(childABC_talk_03))
        CASE_DEFAULT
        CALL_CHILD(PTR(childABC_talk_04))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(childC_init)
    CALL(evt_npc_flag_onoff, 1, STRING("子供Ｃ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    RETURN
EVT_END()

EVT_BEGIN(childC_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        IF_INT_EQUAL(GSWF(1927), 1)
            CALL(evt_msg_print, 0, STRING("stg4_usu_76"), 0, STRING("me"))
            ELSE
            CALL_CHILD(PTR(childABC_talk_01))
        END_IF
        CASE_LESS(222)
        CALL_CHILD(PTR(childABC_talk_02))
        CASE_LESS_OR_EQUAL(402)
        CALL_CHILD(PTR(childABC_talk_03))
        CASE_DEFAULT
        CALL_CHILD(PTR(childABC_talk_04))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(gatekeeper_init)
    CALL(evt_npc_flag_onoff, 1, STRING("門番"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(181)
        SET_VALUE(LW(0), STRING("e_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_npc_set_position, STRING("門番"), 400, 0, -55)
        CASE_LESS(200)
        CALL(evt_npc_set_position, STRING("門番"), 0, -2000, 0)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("門番"), 400, 0, -55)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(gatekeeper_pig_init)
    CALL(evt_npc_flag_onoff, 1, STRING("ブタ門番"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(181)
        CALL(evt_npc_set_position, STRING("ブタ門番"), 0, -2000, 0)
        CASE_LESS(200)
        CALL(evt_npc_set_position, STRING("ブタ門番"), 400, 0, -55)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("ブタ門番"), 0, -2000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(gatekeeper_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(179)
        CALL(evt_mario_mov_pos2, 350, -50, 120)
        WAIT_FRAMES(1)
        CALL(evt_mario_set_dir, 90, 300, 0)
        CALL(evt_party_move_behind_mario, 0)
        CALL(evt_cam3d_evt_set, 372, 124, 321, 372, 57, -61, 500, 11)
        WAIT_MS(500)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_msg_print, 0, STRING("stg4_usu_44"), 0, STRING("me"))
        CALL(evt_cam3d_evt_set, 342, 124, 321, 342, 57, -61, 500, 11)
        WAIT_MS(500)
        CALL(evt_party_stop, 0)
        WAIT_MS(200)
        CALL(evt_msg_print_party, STRING("stg4_usu_45"))
        CALL(evt_party_run, 0)
        CALL(evt_cam3d_evt_off, 1000, 11)
        SPAWN_EVENT(PTR(village_chief_change_pig_event))
        SET_VALUE(GSW_STORY_PROGRESS, 179)
        RETURN
        CASE_LESS(180)
        CALL(evt_msg_print, 0, STRING("stg4_usu_45_01"), 0, STRING("me"))
        CASE_LESS(181)
        CALL(evt_msg_print, 0, STRING("stg4_usu_46"), 0, STRING("me"))
        CASE_LESS(200)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_48"), 0, STRING("me"))
        CASE_LESS(219)
        CALL(evt_msg_print, 0, STRING("stg4_usu_49"), 0, STRING("me"))
        CASE_LESS(222)
        CALL(evt_msg_print, 0, STRING("stg4_usu_49_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(402)
        CALL(evt_msg_print, 0, STRING("stg4_usu_49_2"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("stg4_usu_49_2_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(crow_f_init)
    CALL(evt_npc_set_position, STRING("me"), 275, 83, 355)
    RETURN
EVT_END()

EVT_BEGIN(crow_g_init)
    CALL(evt_npc_set_position, STRING("me"), 250, 112, 355)
    RETURN
EVT_END()

EVT_BEGIN(crow_h_init)
    CALL(evt_npc_set_position, STRING("me"), -147, 110, 346)
    RETURN
EVT_END()

EVT_BEGIN(crow_i_init)
    CALL(evt_npc_set_position, STRING("me"), -175, 110, 348)
    RETURN
EVT_END()

EVT_BEGIN(crow_j_init)
    CALL(evt_npc_set_position, STRING("me"), -195, 100, -157)
    RETURN
EVT_END()

static EVT_BEGIN(musume_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(200)
        CALL(evt_npc_get_position, STRING("ブタ３人娘Ａ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_01"), 0, STRING("ブタ３人娘Ａ"))
        CALL(evt_npc_get_position, STRING("ブタ３人娘Ｂ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_02"), 0, STRING("ブタ３人娘Ｂ"))
        CALL(evt_npc_get_position, STRING("ブタ３人娘Ｃ"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_VOICE_PIG1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_03"), 0, STRING("ブタ３人娘Ｃ"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_04"), 0, STRING("３人娘Ａ"))
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_05"), 0, STRING("３人娘Ｂ"))
        CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_06"), 0, STRING("３人娘Ｃ"))
    END_SWITCH
    RETURN
EVT_END()

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

static EVT_BEGIN(musume_a_entry)
    CALL(evt_npc_entry, STRING("３人娘Ａ"), STRING("c_dance_r"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ａ"), STRING("踊り子"))
    SET_VALUE(LW(0), STRING("３人娘Ａ"))
    CALL_CHILD(PTR(musume_a_init))
    RETURN
EVT_END()

EVT_BEGIN(musume_a_pig_entry)
    CALL(evt_npc_entry, STRING("ブタ３人娘Ａ"), STRING("c_bubusan"))
    CALL(evt_npc_set_tribe, STRING("ブタ３人娘Ａ"), STRING("ブタ"))
    SET_VALUE(LW(0), STRING("ブタ３人娘Ａ"))
    CALL_CHILD(PTR(musume_a_init))
    RETURN
EVT_END()

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

static EVT_BEGIN(musume_b_entry)
    CALL(evt_npc_entry, STRING("３人娘Ｂ"), STRING("c_dance_p"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ｂ"), STRING("踊り子ピンク"))
    SET_VALUE(LW(0), STRING("３人娘Ｂ"))
    CALL_CHILD(PTR(musume_b_init))
    RETURN
EVT_END()

EVT_BEGIN(musume_b_pig_entry)
    CALL(evt_npc_entry, STRING("ブタ３人娘Ｂ"), STRING("c_bubusan"))
    CALL(evt_npc_set_tribe, STRING("ブタ３人娘Ｂ"), STRING("ブタ"))
    SET_VALUE(LW(0), STRING("ブタ３人娘Ｂ"))
    CALL_CHILD(PTR(musume_b_init))
    RETURN
EVT_END()

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

static EVT_BEGIN(musume_c_entry)
    CALL(evt_npc_entry, STRING("３人娘Ｃ"), STRING("c_dance_k"))
    CALL(evt_npc_set_tribe, STRING("３人娘Ｃ"), STRING("踊り子黄緑"))
    SET_VALUE(LW(0), STRING("３人娘Ｃ"))
    CALL_CHILD(PTR(musume_c_init))
    RETURN
EVT_END()

EVT_BEGIN(musume_c_pig_entry)
    CALL(evt_npc_entry, STRING("ブタ３人娘Ｃ"), STRING("c_bubusan"))
    CALL(evt_npc_set_tribe, STRING("ブタ３人娘Ｃ"), STRING("ブタ"))
    SET_VALUE(LW(0), STRING("ブタ３人娘Ｃ"))
    CALL_CHILD(PTR(musume_c_init))
    RETURN
EVT_END()

static EVT_BEGIN(urouro_move)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(50.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(urouro_move_semai)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(15.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"村人Ｆ"*/"\x91\xBA\x90\x6C\x82\x65", // name
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
        /*"村人Ｇ"*/"\x91\xBA\x90\x6C\x82\x66", // name
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
        /*"村人Ｈ"*/"\x91\xBA\x90\x6C\x82\x67", // name
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
        /*"村人Ｉ"*/"\x91\xBA\x90\x6C\x82\x68", // name
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
        /*"村人Ｊ"*/"\x91\xBA\x90\x6C\x82\x69", // name
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
        /*"店の主人"*/"\x93\x58\x82\xCC\x8E\xE5\x90\x6C", // name
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
        /*"お店の奥さん"*/"\x82\xA8\x93\x58\x82\xCC\x89\x9C\x82\xB3\x82\xF1", // name
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
        /*"キザ野郎"*/"\x83\x4C\x83\x55\x96\xEC\x98\x59", // name
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
        /*"おばさん"*/"\x82\xA8\x82\xCE\x82\xB3\x82\xF1", // name
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
        /*"子供Ａ"*/"\x8E\x71\x8B\x9F\x82\x60", // name
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
        /*"子供Ｂ"*/"\x8E\x71\x8B\x9F\x82\x61", // name
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
        /*"子供Ｃ"*/"\x8E\x71\x8B\x9F\x82\x62", // name
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
        /*"門番"*/"\x96\xE5\x94\xD4", // name
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
        /*"ブタキザ野郎"*/"\x83\x75\x83\x5E\x83\x4C\x83\x55\x96\xEC\x98\x59", // name
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
        /*"ブタおばさん"*/"\x83\x75\x83\x5E\x82\xA8\x82\xCE\x82\xB3\x82\xF1", // name
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
        /*"ブタ門番"*/"\x83\x75\x83\x5E\x96\xE5\x94\xD4", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        gatekeeper_pig_init, // initEvent
        NULL, // regularEvent
        gatekeeper_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブタ村人Ｉ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x68", // name
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
        /*"ブタ村人Ｊ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x69", // name
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
        /*"カラスＦ"*/"\x83\x4A\x83\x89\x83\x58\x82\x65", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_f_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＧ"*/"\x83\x4A\x83\x89\x83\x58\x82\x66", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_g_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＨ"*/"\x83\x4A\x83\x89\x83\x58\x82\x67", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_h_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＩ"*/"\x83\x4A\x83\x89\x83\x58\x82\x68", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        crow_i_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カラスＪ"*/"\x83\x4A\x83\x89\x83\x58\x82\x69", // name
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
    CALL(evt_mario_normalize)
    CALL(evt_snd_sfxon, STRING("SFX_STG4_TEMPLE_BELL1"), 0)
    WAIT_MS(500)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_hello_party, 0, 1)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
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
    CALL(evt_cam3d_evt_set, LW(0), LW(4), LW(5), LW(0), LW(1), LW(2), 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg4_usu_50"))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_mon_open)
    IF_INT_LESS(GSW_STORY_PROGRESS, 182)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_45_02"), 0, STRING("門番"))
        CALL(evt_mario_key_onoff, 1)
        ELSE
        CALL(evt_cam_letter_box_disable, 1)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_monato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_get_position, STRING("S_mon01"), LW(10), LW(11), LW(12))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_FENCE_OPEN1"), LW(10), LW(11), LW(12), LW(9))
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 900, 1000)
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
        SET_VALUE(GSW_STORY_PROGRESS, 183)
        CALL(evt_exit_case_evt)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(cam_up)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_get_pos, 4, LW(0), LW(1), LW(2))
        CALL(evt_cam_get_at, 4, LW(3), LW(4), LW(5))
        ADD(LW(1), 85)
        ADD(LW(4), 85)
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 500, 11)
    END_INLINE_EVENT
    WAIT_MS(600)
    RETURN
EVT_END()

static EVT_BEGIN(cam_down)
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    RETURN
EVT_END()

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
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＦ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_area_check, 1, LW(3), LW(5), 20, LW(0), LW(2), LW(6))
#ifdef VERSION_G8ME01
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
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        CALL(evt_mario_party_use_check, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            IF_BITS_SET(LW(0), 256)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(15), 0)
                CALL(evt_mario_key_onoff, 0)
                CALL(evt_party_cont_onoff, 0, 0)
                CALL(mainwindow_opendisable)
                CALL_CHILD(PTR(cam_up))
                CALL(evt_npc_get_position, STRING("カラスＦ"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
                CALL(evt_msg_print, 0, STRING("stg4_jin_153"), 0, STRING("カラスＦ"))
                CALL_CHILD(PTR(cam_down))
                CALL(evt_mario_key_onoff, 1)
                CALL(evt_party_cont_onoff, 0, 1)
                CALL(mainwindow_openenable)
            END_IF
        END_IF
    END_IF
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
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 206)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(mainwindow_opendisable)
            CALL_CHILD(PTR(cam_up))
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＦ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(210)
                CALL(evt_msg_print, 0, STRING("stg4_usu_137"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_138"), 0, STRING("カラスＧ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_139"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_140"), 0, STRING("カラスＧ"))
                CALL_CHILD(PTR(cam_down))
                IF_INT_EQUAL(GSWF(1928), 0)
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
                    SET_VALUE(GSWF(1928), 1)
                    CALL(evt_party_cont_onoff, 0, 0)
                    CALL(evt_party_run, 0)
                END_IF
                CASE_LESS(222)
                CALL(evt_msg_print, 0, STRING("stg4_usu_141"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_142"), 0, STRING("カラスＧ"))
                CALL_CHILD(PTR(cam_down))
                CASE_LESS_OR_EQUAL(402)
                CALL(evt_msg_print, 0, STRING("stg4_usu_142_1"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_142_2"), 0, STRING("カラスＧ"))
                CALL_CHILD(PTR(cam_down))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_142_2_1"), 0, STRING("カラスＦ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_142_2_2"), 0, STRING("カラスＧ"))
                CALL_CHILD(PTR(cam_down))
            END_SWITCH
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_mario_key_onoff, 1)
            CALL(mainwindow_openenable)
            SET_VALUE(GW(0), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(usu01_crowHI_talk_event)
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(1), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＨ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_area_check, 1, LW(3), LW(5), 80, LW(0), LW(2), LW(6))
#ifdef VERSION_G8ME01
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
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 206)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(mainwindow_opendisable)
            CALL_CHILD(PTR(cam_up))
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＨ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(210)
                CALL(evt_msg_print, 0, STRING("stg4_usu_143"), 0, STRING("カラスＨ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_144"), 0, STRING("カラスＩ"))
                CASE_LESS(222)
                CALL(evt_msg_print, 0, STRING("stg4_usu_145"), 0, STRING("カラスＨ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_146"), 0, STRING("カラスＩ"))
                CASE_LESS_OR_EQUAL(402)
                CALL(evt_msg_print, 0, STRING("stg4_usu_146_1"), 0, STRING("カラスＨ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_146_2"), 0, STRING("カラスＩ"))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_146_2_1"), 0, STRING("カラスＨ"))
                CALL(evt_msg_print, 0, STRING("stg4_usu_146_2_2"), 0, STRING("カラスＩ"))
            END_SWITCH
            CALL_CHILD(PTR(cam_down))
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(evt_mario_key_onoff, 1)
            CALL(mainwindow_openenable)
            SET_VALUE(GW(1), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(usu01_crowJ_talk_event)
    SET_VALUE(LW(15), 0)
    LABEL(0)
    IF_INT_NOT_EQUAL(GW(2), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("カラスＪ"), LW(3), LW(4), LW(5))
    CALL(evt_sub_area_check, 1, LW(3), LW(5), 20, LW(0), LW(2), LW(6))
#ifdef VERSION_G8ME01
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
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_mario_balloon_off)
        SET_VALUE(LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 206)
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_party_cont_onoff, 0, 0)
            CALL(mainwindow_opendisable)
            CALL_CHILD(PTR(cam_up))
            WAIT_MS(1000)
            CALL(evt_npc_get_position, STRING("カラスＪ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG4_CROW_HOWL1"), LW(0), LW(1), LW(2), 0)
            SWITCH(GSW_STORY_PROGRESS)
                CASE_LESS(210)
                CALL(evt_msg_print, 0, STRING("stg4_usu_147"), 0, STRING("カラスＪ"))
                CASE_LESS(222)
                CALL(evt_msg_print, 0, STRING("stg4_usu_148"), 0, STRING("カラスＪ"))
                CASE_LESS_OR_EQUAL(402)
                CALL(evt_msg_print, 0, STRING("stg4_usu_148_1"), 0, STRING("カラスＪ"))
                CASE_DEFAULT
                CALL(evt_msg_print, 0, STRING("stg4_usu_148_1_1"), 0, STRING("カラスＪ"))
            END_SWITCH
            CALL_CHILD(PTR(cam_down))
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_party_cont_onoff, 0, 1)
            CALL(mainwindow_openenable)
            SET_VALUE(GW(2), 1)
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

const char* minka_npc[] = {
    /*"おばさん"*/"\x82\xA8\x82\xCE\x82\xB3\x82\xF1",
    /*"ブタおばさん"*/"\x83\x75\x83\x5E\x82\xA8\x82\xCE\x82\xB3\x82\xF1",
    /*"子供Ａ"*/"\x8E\x71\x8B\x9F\x82\x60",
    /*"子供Ｂ"*/"\x8E\x71\x8B\x9F\x82\x61",
    /*"子供Ｃ"*/"\x8E\x71\x8B\x9F\x82\x62",
    NULL,
};

const char* minka_item[] = {
    "iri_20",
    NULL,
};

const char* shop_npc[] = {
    /*"お店の奥さん"*/"\x82\xA8\x93\x58\x82\xCC\x89\x9C\x82\xB3\x82\xF1",
    /*"キザ野郎"*/"\x83\x4C\x83\x55\x96\xEC\x98\x59",
    /*"ブタキザ野郎"*/"\x83\x75\x83\x5E\x83\x4C\x83\x55\x96\xEC\x98\x59",
    /*"店の主人"*/"\x93\x58\x82\xCC\x8E\xE5\x90\x6C",
    /*"ペラ魔人"*/"\x83\x79\x83\x89\x96\x82\x90\x6C",
    NULL,
};

const char* shop_mobj[] = {
    "usu_00_lock",
    "pera_box",
    "mobj_box00",
    NULL,
};

const char* shop_item[] = {
    "item_02",
    "item_03",
    "item_04",
    "item_05",
    NULL,
};

static EVT_BEGIN(evt_door_1_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie1_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_1_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie1_door"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_1)
    SET_FLOAT_VALUE(LW(1), LW(0))
    MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie1_kai1"), LW(1), 0, 0)
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie1_kai2"), LW(2), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(door_1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(door_1_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

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

EVT_BEGIN(evt_door_2_1_1)
    SET_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_2_1_2)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_2_1)
    SET_FLOAT_VALUE(LW(1), LW(0))
    MULTIPLY_FLOAT(LW(1), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_kai1"), 0, 0, LW(1))
    SET_FLOAT_VALUE(LW(2), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_kai2"), 0, 0, LW(2))
    RETURN
EVT_END()

EVT_BEGIN(loop_msg)
    IF_INT_LESS(GSW_STORY_PROGRESS, 200)
        RETURN
    END_IF
    IF_INT_GREATER(GSW_STORY_PROGRESS, 215)
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

EVT_BEGIN(mise_kiza_msg)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), STRING("キザ野郎"))
        SET_VALUE(LW(1), STRING("stg4_usu_61_3"))
        SET_RAW_VALUE(LW(2), LSWF(2))
        SET_VALUE(LW(3), 700)
        CALL_CHILD(PTR(loop_msg))
    END_SIBLING_EVENT
    SET_VALUE(LW(0), STRING("店の主人"))
    SET_VALUE(LW(1), STRING("stg4_usu_61_4"))
    SET_RAW_VALUE(LW(2), LSWF(3))
    SET_VALUE(LW(3), 300)
    CALL_CHILD(PTR(loop_msg))
    RETURN
EVT_END()

EVT_BEGIN(mawari_house_delay)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 180)
        IF_INT_EQUAL(GSWF(1925), 1)
            SPAWN_EVENT(PTR(evt_monban_buta2))
        END_IF
    END_IF
    DELETE_EVENT(GW(3))
    CALL(evt_msg_close, STRING("stg4_usu_61_3"))
    CALL(evt_msg_close, STRING("stg4_usu_61_4"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_usu01_ie3"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_usu01_ie3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

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

USER_FUNC(get_item_list) {
    EvtWord* args = event->args;
#ifdef VERSION_G8ME01
    s32 cnt = pouchGetHaveItemCnt();
    s32* list = _mapAlloc((cnt + 1) * sizeof(s32));
#else
    s32* list = _mapAlloc((pouchGetHaveItemCnt() + 1) * sizeof(s32));
#endif
    s32 n;
    s32 i;

    for (i = 0, n = 0; i < 20; i++) {
        if (pouchHaveItem(i)) {
            list[n] = pouchHaveItem(i);
            n++;
        }
    }
    list[n] = -1;
    evtSetValue(event, args[0], (EvtWord)list);
    return EVT_RETURN_DONE;
}

USER_FUNC(delete_item_list) {
#ifdef VERSION_G8ME01
    void* p = (void*)evtGetValue(event, event->args[0]);
    _mapFree(p);
#else
    _mapFree((void*)evtGetValue(event, event->args[0]));
#endif
    return EVT_RETURN_DONE;
}

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

USER_FUNC(shop_item_disp_off) {
    s32 i;

    for (i = 0; i < 6; i++) {
        mapObjFlagOn(obj_list[i].mobj, 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(shop_item_disp_on) {
    s32 i;

    for (i = 0; i < 6; i++) {
        mapObjFlagOff(obj_list[i].mobj, 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(ie2_npc_disp_off) {
    const char* show[] = {"\202\250\223X\202\314\211\234\202\263\202\361", "\203L\203U\226\354\230Y",
                          "\203u\203^\203L\203U\226\354\230Y", "\223X\202\314\216\345\220l", NULL};
    const char* hide[] = {"\203y\203\211\226\202\220l", NULL};
    s32 i;

    for (i = 0; show[i] != NULL; i++) {
        animPoseSetMaterialFlagOff(npcNameToPtr(show[i])->poseId, 0x1800);
    }
    for (i = 0; hide[i] != NULL; i++) {
        animPoseSetMaterialFlagOn(npcNameToPtr(hide[i])->poseId, 0x1800);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(ie2_npc_disp_on) {
    const char* hide[] = {"\202\250\223X\202\314\211\234\202\263\202\361", "\203L\203U\226\354\230Y",
                          "\203u\203^\203L\203U\226\354\230Y", "\223X\202\314\216\345\220l", NULL};
    const char* show[] = {"\203y\203\211\226\202\220l", NULL};
    s32 i;

    for (i = 0; hide[i] != NULL; i++) {
        animPoseSetMaterialFlagOn(npcNameToPtr(hide[i])->poseId, 0x1800);
    }
    for (i = 0; show[i] != NULL; i++) {
        animPoseSetMaterialFlagOff(npcNameToPtr(show[i])->poseId, 0x1800);
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(set_fook_evt) {
    EvtWord* args = event->args;
    s32* work = (s32*)evtGetValue(event, args[0]);

    work[1] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(booboo_main) {
    u32 i;

    for (i = 0; i < 8; i++) {
        if (npcNameToPtr_NoAssert(booboo_name[i].name) != NULL) {
            animPoseSetMaterialLightFlagOn(npcNameToPtr(booboo_name[i].name)->poseId, 0x10);
        }
    }
    return EVT_RETURN_DONE;
}

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

EVT_BEGIN(evt_ie2_door2_1)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_usu01_ie2_naka2"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_usu01_ie2_naka2"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_item_delete_check, STRING("item_05"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_05"), LW(0), LW(1), LW(2))
        CALL(evt_item_set_position, STRING("item_05"), LW(0), -1000, LW(2))
    END_IF
    CALL(evt_mario_mov_pos2, -213, -471, FLOAT(135.0f))
    CALL(evt_party_set_homing_dist, 0, FLOAT(20.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
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
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
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
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 185)
        CALL(shop_item_disp_off)
    END_IF
    CALL(ie2_npc_disp_off)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_WALL_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 300, 500)
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
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 300, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mapobj_trans, 1, STRING("S_usu01_ie2_mkabe"), 0, 0, LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 185)
        CALL(shop_item_disp_on)
    END_IF
    CALL(ie2_npc_disp_on)
    SPAWN_EVENT_GET_ID(PTR(mise_kiza_msg), GW(3))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 180, 500)
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
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
        CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie2_door3"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_usu01_ie2_naka2"), MAP_OBJ_FLAG_NO_MAP_BLEND)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_usu01_ie2_naka2"), MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_item_delete_check, STRING("item_05"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_item_get_position, STRING("item_05"), LW(0), LW(1), LW(2))
        CALL(evt_item_set_position, STRING("item_05"), LW(0), 0, LW(2))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_3_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie3_door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_3_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_usu01_ie3_door"), LW(0), 0, 0)
    RETURN
EVT_END()

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

const char* door_3_item[] = {
    "item01",
    NULL,
};

static EVT_BEGIN(door_3_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(door_3_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

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

static BeroEntry bero_entry_data[] = {
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
        "gra_00", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
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
        "usu_00", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(usu01_gusun)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 219)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2232), 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(3), 0)
        IF_INT_LESS_OR_EQUAL(LW(3), 100)
            IF_INT_LESS(GSW_STORY_PROGRESS, 200)
                CALL(evt_msg_toge, 2, 37, 30, -244)
                CALL(evt_msg_print, 0, STRING("stg4_usu_51"), 0, 0)
                ELSE
                CALL(evt_msg_toge, 2, 37, 30, -244)
                CALL(evt_msg_print, 0, STRING("stg4_usu_61_1"), 0, 0)
                CALL(evt_msg_toge, 2, 37, 30, -244)
                CALL(evt_msg_print, 0, STRING("stg4_usu_61_2"), 0, 0)
            END_IF
            RETURN
        END_IF
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(evt_monban_buta_main)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_set_position, STRING("ブタ門番"), 400, 0, -55)
    CALL(evt_npc_set_position, STRING("門番"), 0, -2000, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG4_TEMPLE_BELL1"), 0)
    WAIT_MS(500)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(GSW_STORY_PROGRESS, 181)
    RETURN
EVT_END()

EVT_BEGIN(evt_monban_buta)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 181)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_wait, 65536)
    CALL_CHILD(PTR(evt_monban_buta_main))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_monban_buta2)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 181)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1000)
    CALL_CHILD(PTR(evt_monban_buta_main))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_monban_look)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 182)
            RETURN
        END_IF
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 181)
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
                CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
                CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
                WAIT_MS(1000)
                CALL(evt_party_stop, 0)
                WAIT_MS(200)
                CALL(evt_msg_print_party, STRING("stg4_usu_47"))
                CALL(evt_party_run, 0)
                CALL(evt_mario_key_onoff, 1)
                SET_VALUE(GSW_STORY_PROGRESS, 182)
                RETURN
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static s32 lock_tbl[] = {
    ITEM_SHOP_KEY, -1,
};

static EVT_BEGIN(lock_item_check)
    CALL(evt_pouch_check_item, ITEM_SHOP_KEY, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(lock_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
        ELSE
        IF_INT_EQUAL(GSWF(1925), 1)
            CALL(evt_set_dir_to_target, STRING("お店の奥さん"), STRING("mario"))
            CALL(evt_msg_print, 0, STRING("stg4_usu_57"), 0, STRING("お店の奥さん"))
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(unlock)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_usu01_ie2_door3"), 0, PTR(evt_ie2_door2_1), 0)
    SET_VALUE(GSW_STORY_PROGRESS, 185)
    RETURN
EVT_END()

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
    {ITEM_SPITE_POUCH, 10},
    {ITEM_STOPWATCH, 30},
    {ITEM_MAPLE_SYRUP, 20},
    {ITEM_SUPER_SHROOM, 15},
    {ITEM_LIFE_SHROOM, 40},
};

static ShopDb trade_list[] = {
    {ITEM_THUNDER_RAGE, 10, 0},
    {ITEM_SPITE_POUCH, 5, 0},
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

static ShopperData35 shopper_data = {
    /*"お店の奥さん"*/"\x82\xA8\x93\x58\x82\xCC\x89\x9C\x82\xB3\x82\xF1", // name
    NULL, NULL, // fook_evt..buy_evt
    {"usugurashop_00", "usugurashop_01", "usugurashop_02", "usugurashop_03", "usugurashop_04", "usugurashop_05", "usugurashop_06", "usugurashop_07", "usugurashop_08", "usugurashop_09", "usugurashop_10", "usugurashop_11", "usugurashop_12", "usugurashop_13", "usugurashop_14", "usugurashop_15", "usugurashop_16", "usugurashop_17", "usugurashop_18", "usugurashop_19", "usugurashop_20", "usugurashop_21", "usugurashop_22", "usugurashop_23", "usugurashop_24", "usugurashop_25", "usugurashop_26", "usugurashop_27", "usugurashop_28", "usugurashop_29", "usugurashop_30", "usugurashop_31", "usugurashop_32", "usugurashop_33", "usugurashop_34"}, // msgs
};

EVT_BEGIN(shopper_talk1)
    CALL(evt_msg_print, 0, STRING("stg4_usu_59"), 0, STRING("me"))
    SET_VALUE(LW(0), -100)
    RETURN
EVT_END()

EVT_BEGIN(shopper_talk2)
    CALL(evt_msg_print, 0, STRING("stg4_usu_60"), 0, STRING("me"))
    SET_VALUE(LW(0), -100)
    RETURN
EVT_END()

static BoobooName booboo_name[] = {
    {/*"ブタキザ野郎"*/"\x83\x75\x83\x5E\x83\x4C\x83\x55\x96\xEC\x98\x59", 7, 0},
    {/*"ブタおばさん"*/"\x83\x75\x83\x5E\x82\xA8\x82\xCE\x82\xB3\x82\xF1", 1, 0},
    {/*"ブタ門番"*/"\x83\x75\x83\x5E\x96\xE5\x94\xD4", 3, 1},
    {/*"ブタ村人Ｉ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x68", 5, 1},
    {/*"ブタ村人Ｊ"*/"\x83\x75\x83\x5E\x91\xBA\x90\x6C\x82\x69", 4, 1},
    {/*"ブタ３人娘Ａ"*/"\x83\x75\x83\x5E\x82\x52\x90\x6C\x96\xBA\x82\x60", 4, 1},
    {/*"ブタ３人娘Ｂ"*/"\x83\x75\x83\x5E\x82\x52\x90\x6C\x96\xBA\x82\x61", 4, 1},
    {/*"ブタ３人娘Ｃ"*/"\x83\x75\x83\x5E\x82\x52\x90\x6C\x96\xBA\x82\x62", 4, 1},
};

EVT_BEGIN(evt_roll_get)
    RETURN
EVT_END()

static s32 key_tbl[] = {
    ITEM_BLACK_KEY_0x23, -1,
};

static EVT_BEGIN(evt_majin)
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
    IF_INT_EQUAL(GSWF(1932), 0)
        CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_01"), 0, 0)
        CALL(evt_mobj_get_position, STRING("pera_box"), LW(3), LW(4), LW(5))
        ADD(LW(4), 30)
        CALL(evt_snd_sfxon_3d, STRING("SFX_MOBJ_BLACKBOX_SWING1"), LW(3), LW(4), LW(5), LW(15))
        CALL(evt_mobj_set_anim, STRING("pera_box"), STRING("A_3"))
        CALL(evt_mobj_wait_animation_end, STRING("pera_box"))
        CALL(evt_snd_sfxoff, LW(15))
        WAIT_MS(300)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_02"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_G"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_03"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_04"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_05"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_06"), PTR(majin_callback), 0)
        CALL(evt_mario_set_pose, STRING("M_I_Z"))
        WAIT_MS(1600)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_07"), PTR(majin_callback), 0)
        CALL(evt_mobj_exec_cancel, STRING("pera_box"))
        SET_VALUE(GSWF(1932), 1)
        ELSE
        CALL(evt_pouch_check_item, ITEM_BLACK_KEY_0x23, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
            ADD(LW(1), 30)
            CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_08"), PTR(majin_callback), 0)
            CALL(evt_mobj_exec_cancel, STRING("pera_box"))
            ELSE
            CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
            ADD(LW(1), 30)
            CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
            CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_09"), PTR(majin_callback), 0)
            CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
            IF_INT_EQUAL(LW(0), -1)
                CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
                CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_10"), PTR(majin_callback), 0)
                CALL(evt_mobj_exec_cancel, STRING("pera_box"))
                ELSE
                CALL(evt_pouch_remove_item, ITEM_BLACK_KEY_0x23, LW(0))
                CALL(evt_mobj_get_position, STRING("pera_box"), LW(0), LW(1), LW(2))
                ADD(LW(1), 30)
                CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
                CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_11"), PTR(majin_callback), 0)
                CALL(evt_mario_set_pose, STRING("M_I_N"))
            END_IF
        END_IF
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_majin2)
    CALL(evt_mario_key_onoff, 0)
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
    CALL(evt_mario_set_camid, 7)
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
    CALL(evt_cam3d_evt_off, 500, 11)
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
    SET_VALUE(GSW_STORY_PROGRESS, 187)
    SET_VALUE(GSWF(1931), 1)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

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

USER_FUNC(box_r) {
    MapObjectEntry* mobj = mobjNameToPtr("pera_box");

    mobj->rotation.y = 135.0f;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}


static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

USER_FUNC(mobj_ry) {
    MapObjectEntry* mobj = mobjNameToPtr("mobj_box00");

    mobj->rotation.y = 135.0f;
    mobj->flags |= MOBJ_FLAG_UPDATE_MTX;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(tree_onoff)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mapobj_get_position, LW(10), LW(3), LW(4), LW(5))
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(9))
        IF_INT_GREATER(LW(9), LW(11))
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_mapobj_flag_onoff, 1, 1, LW(10), MAP_OBJ_FLAG_HIDDEN)
                SET_VALUE(LF(0), 0)
            END_IF
            ELSE
            IF_INT_EQUAL(LF(0), 0)
                CALL(evt_mapobj_flag_onoff, 1, 0, LW(10), MAP_OBJ_FLAG_HIDDEN)
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(evt_ki_1)
    CALL(evt_npc_set_anim, STRING("カラスＪ"), STRING("A_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カラスＪ"), STRING("S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_ki_2)
    CALL(evt_npc_set_anim, STRING("カラスＨ"), STRING("A_1"))
    CALL(evt_npc_set_anim, STRING("カラスＩ"), STRING("A_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カラスＨ"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("カラスＩ"), STRING("S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(evt_ki_3)
    CALL(evt_npc_set_anim, STRING("カラスＦ"), STRING("A_1"))
    CALL(evt_npc_set_anim, STRING("カラスＧ"), STRING("A_1"))
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, STRING("カラスＦ"), STRING("S_1"))
    CALL(evt_npc_set_anim, STRING("カラスＧ"), STRING("S_1"))
    RETURN
EVT_END()

static TreeAccess tree_tbl[] = {
    {2, "A_ki_1", "S_usu01_ki01", NULL, 0, evt_ki_1},
    {2, "A_ki_2", "S_usu01_ki02", NULL, 0, evt_ki_2},
    {2, "A_ki_3", "S_usu01_ki03", NULL, 0, evt_ki_3},
    {2, "A_ki_4", "S_usu01_ki05", NULL, 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(usu_01_tree_init)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    RETURN
EVT_END()

static EVT_BEGIN(mobj_box00)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("item00"), ITEM_DEFEND_PLUS, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 258)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_item, STRING("item00"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(usu_01_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_USU1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    CALL(evt_npc_setup, PTR(npcEnt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(178, 200)
        CALL_CHILD(PTR(musume_a_pig_entry))
        CALL_CHILD(PTR(musume_b_pig_entry))
        CALL_CHILD(PTR(musume_c_pig_entry))
        CASE_BETWEEN(200, 215)
        CALL_CHILD(PTR(musume_a_entry))
        CALL_CHILD(PTR(musume_b_entry))
        CALL_CHILD(PTR(musume_c_entry))
    END_SWITCH
    IF_INT_LESS(GSW_STORY_PROGRESS, 185)
        CALL(evt_mobj_lock, STRING("usu_00_lock"), 21, FLOAT(-263.5f), FLOAT(10.099609f), FLOAT(-432.0498f), 135, PTR(lock_item_check), PTR(unlock), GSWF(1926))
        ELSE
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_usu01_ie2_door3"), 0, PTR(evt_ie2_door2_1), 0)
    END_IF
    SET_VALUE(LW(0), PTR(&door_1))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_2_1))
    CALL_CHILD(PTR(evt_door_entry))
    SET_VALUE(LW(0), PTR(&door_3))
    CALL_CHILD(PTR(evt_door_entry))
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_usu01_ie2_door3_u"), 0, PTR(evt_ie2_door2_2), 0)
    SPAWN_EVENT(PTR(evt_monban_look))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 180)
        IF_INT_EQUAL(GSWF(1925), 1)
            SPAWN_EVENT(PTR(evt_monban_buta))
        END_IF
    END_IF
    SPAWN_EVENT_GET_ID(PTR(usu01_gusun), GW(4))
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 185)
        IF_INT_LESS(GSW_STORY_PROGRESS, 200)
            CALL(set_fook_evt, PTR(&shopper_data), PTR(shopper_talk1))
            ELSE
            IF_INT_LESS(GSW_STORY_PROGRESS, 213)
                CALL(set_fook_evt, PTR(&shopper_data), PTR(shopper_talk2))
            END_IF
        END_IF
        CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("mi02_S_items"), MAP_OBJ_FLAG_DISABLED)
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    SET_VALUE(GW(2), 0)
    SPAWN_EVENT(PTR(usu01_check_vivian))
    SPAWN_EVENT(PTR(usu01_crowFG_talk_event))
    SPAWN_EVENT(PTR(usu01_crowHI_talk_event))
    SPAWN_EVENT(PTR(usu01_crowJ_talk_event))
    IF_INT_LESS(GSW_STORY_PROGRESS, 183)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_mon"), 0, PTR(evt_mon_open), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_monato"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("A_mon02"), 1, 0)
        ELSE
        CALL(evt_mapobj_rotate, 1, STRING("S_mon01"), 0, 50, 0)
        CALL(evt_mapobj_rotate, 1, STRING("S_mon02"), 0, -85, 0)
        CALL(evt_hitobj_onoff, STRING("A_mon"), 1, 0)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(booboo_main)
    END_INLINE_EVENT
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ki1"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ki2"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ki3"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ki4"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("ki5"), MAP_OBJ_FLAG_CLIP)
    CALL_CHILD(PTR(usu_01_tree_init))
    SET_VALUE(LW(10), STRING("ki3"))
    SET_VALUE(LW(11), 450)
    SPAWN_EVENT(PTR(tree_onoff))
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, 210, 0, 265, ITEM_TYPE_STILL, GSWF(5630), 0)
    CALL(evt_item_entry, STRING("item_02"), ITEM_LIFE_SHROOM, -275, 0, -310, ITEM_TYPE_STILL, GSWF(1935), 0)
    CALL(evt_item_entry, STRING("item_03"), ITEM_BOOS_SHEET, -305, 0, -380, ITEM_TYPE_STILL, GSWF(1936), 0)
    CALL(evt_item_entry, STRING("item_04"), ITEM_JAMMIN_JELLY, -335, 0, -410, ITEM_TYPE_STILL, GSWF(1937), 0)
    CALL(evt_item_entry, STRING("item_05"), ITEM_INN_COUPON, -250, 0, -500, ITEM_TYPE_STILL, GSWF(1939), 0)
    CALL(evt_mobj_itembox, STRING("mobj_box00"), -255, 0, -288, MOBJ_ITEMBOX_NORMAL, 0, PTR(mobj_box00), GSWF(1934))
    CALL(mobj_ry)
    BEGIN_INLINE_EVENT
        CALL(npc_process)
    END_INLINE_EVENT
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 187)
        SET_VALUE(GSWF(1931), 1)
    END_IF
    CALL(evt_mobj_itembox, STRING("pera_box"), -289, 0, -341, MOBJ_ITEMBOX_BLACK, PTR(evt_majin), PTR(evt_majin2), GSWF(1931))
    CALL(evt_mobj_flag_onoff, 0, STRING("pera_box"), MOBJ_FLAG_GRAVITY)
    CALL(box_r)
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_TELL_THAT_PERSON), 1)
        CALL(evt_item_entry, STRING("iri_20"), ITEM_METEOR_MEAL, -345, 20, -132, ITEM_TYPE_STILL, GSWF(1938), 0)
    END_IF
    SPAWN_EVENT(PTR(evt_sub_mail_event))
    RETURN
EVT_END()


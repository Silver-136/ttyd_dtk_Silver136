#include "eff/eff_starstone.h"
#include "eff/eff_mahojin.h"
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_badgeshop.h"
#include "statuswindow.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "pmario_sound.h"
#include "eff/eff_fire.h"
#include "eff/n64/eff_kemuri12_n64.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_lecture.h"
#include "evt/evt_pouch.h"
#include "driver/mapdrv.h"
#include "eff/eff_kemuri.h"
#include "driver/casedrv.h"

#include "manager/evtmgr_cmd.h"
#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "mario/mario.h"
#include "system.h"
#include "data/item_data.h"
#include "dolphin/gx.h"
#include "string.h"
#include "evt/evt_cmd.h"
#include "dolphin/mtx.h"
#include "driver/animdrv.h"
#include "driver/mobjdrv.h"
#include "driver/effdrv.h"
#include "stdio.h"
#include "driver/camdrv.h"
#include "memory.h"
#include "mario/mario_pouch.h"
#include "driver/swdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_kinopio.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_case.h"
#include "evt/evt_sub.h"
#include "gor.h"
#include "evt/evt_shop.h"
#include "data_tables.h"
#include "driver/msgdrv.h"
#include "driver/seqdrv.h"
#ifdef VERSION_G8ME01
#endif

extern ItemData itemDataTable[];
typedef struct ShopSel {
    u8 field_0x0[8];
    s32* items;
    u8 field_0xC[0x24 - 0xC];
    s32 sel;
} ShopSel;
NpcEntry* fbatNpcTalkCheck(MarioWork* mario);
s32 search_evt_moji(s32 id);
void effStarStoneDraw(CameraId camera, void* param);
s32* iri_09_item_tbl;
s32 naniga_okorukana_cook(void);
s32 itemNameToID(const char* name);
static s32 item_tbl[21];
s32 chk_cook_eff_type(s32 item);

static USER_FUNC(anim_tevcallback);
static USER_FUNC(anim_tevcallback_mobj);
USER_FUNC(chk_cook_msg);
USER_FUNC(chk_marco_fuki);
USER_FUNC(chk_special_cook);
USER_FUNC(chusan1_alwaysencont);
USER_FUNC(cook_chk);
USER_FUNC(cook_chk2);
USER_FUNC(cook_effect);
USER_FUNC(cooking_flag);
static USER_FUNC(delete_npc_regl_evt);
static USER_FUNC(disp_off);
static USER_FUNC(evt_StatusClose);
static USER_FUNC(evt_StatusOpen);
USER_FUNC(evt_mario_dispflag_onoff);
static USER_FUNC(evt_osreport);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_pouch_get_havebadgecnt);
USER_FUNC(evt_pouch_get_haveitemcnt);
USER_FUNC(evt_pouch_party_join);
USER_FUNC(evt_pouch_set_coin);
USER_FUNC(evt_snd_env_lpf);
USER_FUNC(evt_snd_envoff_f);
USER_FUNC(evt_unitwin_alloc);
USER_FUNC(evt_unitwin_free);
USER_FUNC(evt_unitwin_selltable_setup);
USER_FUNC(evt_win_coin_off);
USER_FUNC(evt_win_coin_on);
USER_FUNC(evt_win_coin_wait);
USER_FUNC(evt_win_item_select);
USER_FUNC(evt_win_other_select);
USER_FUNC(get_item_name);
static USER_FUNC(get_value);
USER_FUNC(house03_f1_fade_func);
USER_FUNC(house03_f2_fade_func);
USER_FUNC(iri_09_item_tbl_alloc);
USER_FUNC(iri_09_item_tbl_free);
USER_FUNC(iri_09_item_tbl_make);
USER_FUNC(keijiban_data_alloc);
USER_FUNC(keijiban_data_free);
USER_FUNC(keijiban_joho_check);
USER_FUNC(keijiban_joho_makelabel);
USER_FUNC(kpa_chg_pose);
USER_FUNC(maffia_kemuri);
USER_FUNC(make_item_tbl);
static USER_FUNC(mario_keyoff_wait);
USER_FUNC(nancy_block_func);
USER_FUNC(nancy_block_init_func);
static USER_FUNC(set_fook_evt);
USER_FUNC(set_new_goods_list);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
USER_FUNC(unitwin_get_work_ptr);
USER_FUNC(unitwin_set_bought);
USER_FUNC(badgeshop_special_cnt);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
USER_FUNC(wait_mahojin);
static USER_FUNC(yami_view);
BOOL chkCookItem(s32 item);
static s32 npc_process(void);
extern s32 envSetYami[];

static EvtWord sell_evt[VERSION_PICK(135, 146)];
EvtWord zaiko_buy_evt[VERSION_PICK(174, 185)];
static EvtWord buy_evt[53];
EvtWord badgeshopper_talk_evt[38];
EvtWord badgemaster_init[36];
EvtWord badgemaster_talk[76];
EvtWord kinoji_init[2];
EvtWord kinoji_talk[76];
EvtWord nancy_init_check[608];
EvtWord nancy_init[58];
EvtWord nancy_door_anime[99];
EvtWord nancy_regl[35];
s32 item_tbl_nancy[2];
EvtWord nancy_talk[175];
static EvtWord marco_init[14];
static EvtWord marco_talk[519];
EvtWord master_init[12];
EvtWord iri_20_master_tubuyaki[37];
EvtWord master_talk[645];
EvtWord shoptender_init[27];
EvtWord shoptender_talk[227];
EvtWord yadotender_init[2];
EvtWord yadotender_talk[2];
EvtWord roten1_init[12];
EvtWord roten1_regl[25];
EvtWord roten1_talk[76];
EvtWord roten2_init[7];
EvtWord roten2_talk[76];
EvtWord kuribo1_init[12];
EvtWord kuribo1_regl[25];
EvtWord kuribo1_talk[101];
EvtWord kuribo2_init[7];
EvtWord kuribo2_regl[4];
EvtWord kuribo2_talk[76];
EvtWord kuribo3_init[2];
EvtWord kuribo3_regl[4];
EvtWord kuribo3_talk[144];
EvtWord chusan1_init[31];
EvtWord chusan1_regl[12];
EvtWord chusan1_dead[123];
EvtWord chusan1_naguru[79];
EvtWord chusan1_talk[180];
EvtWord chusan2_init[2];
EvtWord chusan2_talk[76];
static EvtWord bomhei_init[2];
EvtWord bomhei_talk[76];
EvtWord bottakuru_init[25];
EvtWord bottakuru_talk[VERSION_PICK(292, 314)];
static EvtWord luigi_init[43];
static EvtWord luigi_talk[4];
static EvtWord luigi_npcEnt[46];
EvtWord kick_init[12];
EvtWord kick_talk[8];
NpcSetupInfo kickEnt[2];
EvtWord torque_init[12];
EvtWord torque_talk[8];
NpcSetupInfo torqueEnt[2];
EvtWord kick2_init[25];
EvtWord kick2_talk[8];
NpcSetupInfo kick2Ent[2];
static EvtWord borodo2_init[51];
static EvtWord borodo2_talk[280];
EvtWord thief_init[13];
EvtWord thief_talk[163];
static NpcSetupInfo npcEnt[22];
EvtWord christine_nakama_before[18];
static BeroEntry bero_entry_data[4];
static EvtWord evt_door_01_00[12];
static EvtWord evt_door_01_01[12];
static EvtWord evt_wall_01[28];
static const char* door_data_01_npc[2];
const char* door_data_01_item[2];
static EvtWord evt_door_01_open[12];
static EvtWord evt_door_01_close[14];
static DoorParam door_data_01;
DoorParam door_data_01_01;
EvtWord evt_door_02_00_f1[12];
EvtWord evt_wall_02_f1[41];
static const char* door_data_02_npc[3];
const char* door_data_02_npc_01[2];
EvtWord evt_door_02f1_open[12];
EvtWord evt_door_02f1_close[14];
DoorParam door_data_02_f1;
DoorParam door_data_02_f1_01;
EvtWord evt_door_02_f2_junbityu[14];
EvtWord evt_door_02_00_f2[12];
EvtWord evt_wall_02_f2[41];
const char* badgemise_npc[3];
EvtWord evt_badge_dummy[VERSION_PICK(128, 139)];
UnitWinSell uwnobj_list[6];
EvtWord evt_open_02_01_f2[22];
EvtWord evt_close_sub[6];
EvtWord evt_close_02_01_f2[16];
DoorParam door_data_02_f2;
EvtWord house03_f1f2_fade[41];
EvtWord luigi_f1_fade_check[27];
EvtWord house03_f1_fade[54];
EvtWord luigi_f2_fade_check[27];
EvtWord house03_f2_fade[54];
EvtWord house03_suiteki[49];
EvtWord marco_hitorigoto[67];
EvtWord evt_door_03_00_f1[12];
EvtWord evt_door_03_01_f1[12];
EvtWord house_03_f1_open[64];
EvtWord house_03_f1_close[54];
EvtWord evt_wall_03_f1[47];
static const char* door_data_03_npc[10];
const char* door_data_03_item[2];
DoorParam door_data_03_f1;
EvtWord evt_door_03_00_f2[12];
EvtWord house_03_f2_open[48];
EvtWord house_03_f2_close[VERSION_PICK(46, 52)];
EvtWord evt_wall_03_f2[54];
DoorParam door_data_03_f2;
static EvtWord evt_door_04_00[12];
static EvtWord evt_door_04_01[12];
static EvtWord evt_wall_04[28];
static EvtWord evt_door_04_open[18];
static EvtWord evt_door_04_close[21];
static const char* door_data_04_npc[2];
static const char* door_data_04_item[2];
static DoorParam door_data_04;
static EvtWord koushukei_dai[63];
static ShopObj obj_list[6];
static ShopItem goods_list[6];
ShopItem goods_list2[6];
ShopItem goods_luigi1[6];
ShopItem goods_luigi2[6];
ShopItem goods_luigi3[6];
ShopItem goods_luigi4[6];
ShopItem goods_luigi5[6];
static ShopDb trade_list[19];
static ShopperData35 shopper_data;
EvtWord gor_01_shop_setup[156];
static KinopioData kino_dt;
EvtWord bar_door_anime[99];
EvtWord christine_nakama_init[58];
EvtWord christine_nakama[760];
EvtWord maffia_turn[84];
EvtWord maffia_assault[797];
EvtWord christine_nakama_after[82];
EvtWord box_copy[19];
EvtWord ultrahammer_check[222];
EvtWord ultrahammer_get[39];
EvtWord keijiban_omote_joho[93];
EvtWord keijiban_ura_joho[93];
EvtWord gate_disp[42];
static EvtWord gor_goki1_urouro[109];
static EvtWord gor_goki2_urouro[109];
static EvtWord koopa_npc_entry[38];
EvtWord gor_01_koopa_evt[584];
static EvtWord sekai_yami_npc[59];
static EvtWord sekai_yami_evt[185];
static EvtWord minnnanokoe_evt[397];
EvtWord iri_09_init[10];
EvtWord iri_09_tentyo_talk[172];
s32 iri_16_item_tbl[2];
static EvtWord iri_16_init[34];
EvtWord iri_16_tyutyu_talk[445];
EvtWord marco_jewel[22];
static EvtWord epilogue_npc_entry[48];
static EvtWord epilogue_evt_sub[160];
static EvtWord epilogue_evt[313];
#ifdef VERSION_G8ME01
EvtWord dai_disp[52];
#endif
EvtWord gor_01_init_evt[VERSION_PICK(546, 548)];
SimpleRecipe R_simple_recipe_table[26];
CookRecipe R_cookbook_recipe_table[VERSION_PICK(289, 292)];
s32 R_mystery_cook_list[20];
s32 special_cook_list[16];
EvtWord cooking_evt_sub[284];
s32 iri_18_item_tbl[2];
EvtWord cooking_evt[852];

USER_FUNC(badgeshop_special_cnt) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], U_badgeShop_SpecialCheck());
    return EVT_RETURN_DONE;
}

USER_FUNC(nancy_block_init_func) {
    NpcEntry* npc = evtNpcNameToPtr(event, "\203i\203\223\203V\201[");
    f32 z;

    z = npc->position.z;
    event->lwDataF[10] = z;
    event->lwDataF[11] = z;
    npc->flags |= NPC_FLAG_RESUME_AFTER_TALK;
    return EVT_RETURN_DONE;
}

USER_FUNC(nancy_block_func) {
    MarioWork* mario = marioGetPtr();
    NpcEntry* npc = evtNpcNameToPtr(event, "\203i\203\223\203V\201[");
    f32 z;
    f32 base;
    f32 limit;

    event->lwDataF[10] = npc->position.z;
    if (distABf(npc->position.x, npc->position.z, mario->position.x, mario->position.z) < 30.0f) {
        return EVT_RETURN_DONE;
    }
    if (distABf(npc->position.x, npc->position.z, mario->position.x, mario->position.z) > 130.0f) {
        return EVT_RETURN_DONE;
    }
    z = mario->position.z;
    base = event->lwDataF[11];
    limit = z;
    if (z > 40.0f + base) {
        limit = 40.0f + base;
    }
    if (z < base - 40.0f) {
        limit = base - 40.0f;
    }
    if (npc->position.z <= limit) {
        npc->position.z += 4.0f;
        if (npc->position.z > limit) {
            npc->position.z = limit;
        }
    }
    if (npc->position.z > limit) {
        npc->position.z -= 4.0f;
        if (npc->position.z <= limit) {
            npc->position.z = limit;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(chusan1_alwaysencont) {
    EvtWord* args = event->args;
    NpcWork* wp = npcGetWorkPtr();

    if (evtGetValue(event, args[0])) {
        wp->flags |= 1;
    } else {
        wp->flags &= ~1;
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_StatusOpen) {
    statusWinForceOpen();
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_StatusClose) {
    statusWinForceOff();
    return EVT_RETURN_DONE;
}

static USER_FUNC(disp_off) {
    *(u16*)evtGetValue(event, event->args[0]) &= ~1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(get_value) {
    EvtWord* args = event->args;
    ShopSel* wp = (ShopSel*)evtGetValue(event, args[0]);

#ifdef VERSION_G8ME01
    evtSetValue(event, args[1], (s32)msgSearch(itemDataTable[wp->items[wp->sel]].name));
#else
    evtSetValue(event, args[1], (EvtWord)itemDataTable[wp->items[wp->sel]].id);
#endif
    evtSetValue(event, args[2], wp->items[wp->sel]);
    evtSetValue(event, args[3], itemDataTable[wp->items[wp->sel]].discount_price);
    evtSetValue(event, args[4], itemDataTable[wp->items[wp->sel]].bp_cost);
    return EVT_RETURN_DONE;
}

static USER_FUNC(mario_keyoff_wait) {
    switch (!marioChkKey()) {
        default:
            return EVT_RETURN_BLOCK;
        case 0:
            return EVT_RETURN_YIELD;
    }
}

static EVT_BEGIN(sell_evt)
    CALL(evt_pouch_get_havebadgecnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_06"))
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("badgeshop_07"))
    CALL(evt_win_coin_on, 0, LW(8))
    LABEL(0)
    CALL(evt_win_other_select, 12)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(evt_sub_get_language, LW(0))
#ifdef VERSION_G8ME01
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_08"), 0, STRING("バッジ店員"), LW(2), LW(3))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("badgeshop_08"), LW(3))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
        CALL(evt_msg_print, 1, LW(14), 0, STRING("バッジ店員"))
    END_SWITCH
#else
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_08"), 0, STRING("バッジ店員"), LW(3), LW(2))
        ELSE
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_08"), 0, STRING("バッジ店員"), LW(2), LW(3))
    END_IF
#endif
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_09"))
        GOTO(0)
    END_IF
    CALL(N_evt_pouch_remove_item_index, LW(1), LW(4), LW(0))
    CALL(evt_badgeShop_throw_inc, LW(1))
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(8))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(8))
    CALL(evt_pouch_get_havebadgecnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_11"))
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("badgeshop_10"))
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_22"))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_win_coin_on, 0, LW(8))
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(zaiko_buy_evt)
    CALL(evt_badgeShop_throw_get_kind_cnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(badgeshop_special_cnt, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("badgeshop_37"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("badgeshop_40"))
        END_IF
        RETURN
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_win_coin_on, 0, LW(8))
    LABEL(0)
    CALL(evt_win_other_select, 11)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(evt_sub_get_language, LW(0))
#ifdef VERSION_G8ME01
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(2), LW(3), LW(4))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("badgeshop_00"), LW(3))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
        CALL(evt_msg_print_insert, 1, LW(14), 0, STRING("バッジ店員"), LW(4))
    END_SWITCH
#else
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(3), LW(2), LW(4))
        ELSE
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(2), LW(3), LW(4))
    END_IF
#endif
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_continue)
        GOTO(0)
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_msg_print_add, 0, STRING("badgeshop_01"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(evt_pouch_add_item, LW(1), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_02"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(evt_badgeShop_throw_dec, LW(1))
    MULTIPLY(LW(3), -1)
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(8))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(8))
    CALL(evt_badgeShop_throw_get_kind_cnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_11"))
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("badgeshop_38"))
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_22"))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_win_coin_on, 0, LW(8))
    GOTO(0)
    RETURN
EVT_END()

static EVT_BEGIN(buy_evt)
    CALL(evt_msg_print_add, 0, STRING("badgeshop_36"))
    CALL(evt_msg_select, 0, STRING("badgeshop_35"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(badgeshop_special_cnt, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_badgeShop_throw_get_kind_cnt, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print_add, 0, STRING("badgeshop_37"))
                ELSE
                CALL(evt_msg_print_add, 0, STRING("badgeshop_39"))
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("badgeshop_05"))
        END_IF
        CASE_EQUAL(1)
        CALL_CHILD(PTR(zaiko_buy_evt))
        CASE_EQUAL(2)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_22"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(badgeshopper_talk_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(unitwin_get_work_ptr, LW(10))
    CALL(evt_msg_print, 0, STRING("badgeshop_04"), 0, STRING("バッジ店員"))
    CALL(evt_msg_select, 0, STRING("badgeshop_23"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL_CHILD(PTR(buy_evt))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(sell_evt))
        CASE_EQUAL(2)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_22"))
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(badgemaster_init)
    IF_INT_EQUAL(GSW(1420), 16)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_IF
    IF_INT_EQUAL(GSWF(5390), 1)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 127)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, 148)
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(badgemaster_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_024_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_024_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_024_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_024_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_024_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_024_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_024_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_024_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_024_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kinoji_init)
    RETURN
EVT_END()

EVT_BEGIN(kinoji_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_019"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_019_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_019_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_019_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_019_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_019_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_019_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_019_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_019_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(nancy_init_check)
    CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        SET_VALUE(LW(3), 0)
        IF_INT_GREATER(LW(10), -580)
            IF_INT_LESS(LW(10), -400)
                IF_INT_GREATER(LW(11), -10)
                    IF_INT_LESS(LW(11), 5)
                        IF_INT_GREATER(LW(12), 0)
                            IF_INT_LESS(LW(12), 180)
                                SET_VALUE(LW(3), 1)
                            END_IF
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(3), 1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_npc_get_reglid, STRING("ナンシー"), LW(0))
    DELETE_EVENT(LW(0))
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mario_set_dir_npc, STRING("ナンシー"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_party_wait_landon, 0)
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_dir_npc, 0, STRING("ナンシー"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_party_wait_landon, 1)
        CALL(evt_party_stop, 1)
        CALL(evt_party_set_dir_npc, 1, STRING("ナンシー"))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("gor_01_000"), 0, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), LW(2))
    ADD(LW(1), 335)
    ADD(LW(2), -8)
    IF_INT_LESS_OR_EQUAL(LW(0), -520)
        SET_VALUE(LW(0), -520)
    END_IF
    CALL(evt_cam3d_evt_set, LW(0), 99, LW(1), LW(0), 39, LW(2), 500, 11)
    WAIT_MS(800)
    SET_VALUE(LW(3), 90)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(0), LW(10))
        ADD(LW(3), 180)
    END_IF
    CALL(evt_npc_set_ry, STRING("me"), LW(3))
    CALL(evt_msg_print, 0, STRING("gor_01_001"), 0, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(700)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("gor_01_002"), 0, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(700)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(700)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(700)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(800)
    CALL(evt_msg_print, 0, STRING("gor_01_003"), 0, STRING("me"))
    WAIT_MS(300)
    CALL(evt_npc_restart_regular_event, STRING("me"))
    CALL(evt_cam3d_evt_off, 800, 11)
    WAIT_MS(800)
    CALL(evt_mario_key_onoff, 1)
#ifdef VERSION_G8ME01
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
#endif
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(0), -400)
                BREAK_LOOP
            END_IF
            IF_INT_LESS_OR_EQUAL(LW(0), -550)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(0))
    LOOP(0)
        CALL(evt_mario_get_state, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(60)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_mario_wait_landon)
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), 5)
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_LENS_BREAK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
    CALL(evt_msg_print, 0, STRING("gor_01_004"), 0, 0)
    CALL(evt_npc_get_reglid, STRING("me"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), LW(2))
    ADD(LW(1), 250)
    ADD(LW(2), 30)
    IF_INT_LESS_OR_EQUAL(LW(0), -500)
        SET_VALUE(LW(0), -500)
    END_IF
    CALL(evt_cam3d_evt_set, LW(0), 80, LW(1), LW(0), 40, LW(2), 500, 11)
    WAIT_MS(400)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("gor_01_005"), 0, STRING("me"))
    CALL(evt_mario_set_dir_npc, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    IF_INT_LESS(LW(0), LW(10))
        IF_INT_LESS(LW(10), -550)
            GOTO(100)
        END_IF
        SUBTRACT(LW(10), 30)
        CALL(evt_npc_set_ry, STRING("me"), 90)
        ELSE
        LABEL(100)
        ADD(LW(10), 30)
        CALL(evt_npc_set_ry, STRING("me"), 270)
    END_IF
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_move_position, STRING("me"), LW(10), LW(12), 0, FLOAT(180.0f), 0)
    CALL(evt_mario_set_dir_npc, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    IF_INT_LESS(LW(0), LW(10))
        CALL(evt_npc_set_ry, STRING("me"), 90)
        ELSE
        CALL(evt_npc_set_ry, STRING("me"), 270)
    END_IF
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NO1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_B"))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("gor_01_006"), 0, STRING("me"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NO1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_C"))
    WAIT_MS(400)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(400)
    CALL(evt_npc_set_ry, STRING("me"), 270)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SET_VALUE(GF(0), 0)
    IF_INT_LESS_OR_EQUAL(LW(0), -540)
        SET_VALUE(GF(0), 1)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_mov_pos2, -530, LW(2), FLOAT(180.0f))
            SET_VALUE(GF(0), 0)
            CALL(evt_mario_set_dir, 270, 200, 0)
        END_INLINE_EVENT
    END_IF
    CALL(evt_npc_move_position, STRING("me"), -573, 56, 0, FLOAT(180.0f), 0)
    WAIT_WHILE(GF(0))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("gor_01_007"), 0, STRING("me"))
    CALL(evt_npc_restart_regular_event, STRING("me"))
    CALL(evt_npc_change_interrupt, STRING("me"), 6, PTR(nancy_talk))
    CALL(evt_cam3d_evt_off, 0, 11)
#ifndef VERSION_G8ME01
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
#endif
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSWF(1187), 1)
    RETURN
EVT_END()

EVT_BEGIN(nancy_init)
    SET_VALUE(LSWF(1), 0)
    IF_INT_EQUAL(GSWF(1188), 0)
        IF_INT_EQUAL(GSWF(1187), 0)
            CALL(evt_npc_set_ry, STRING("me"), 270)
            CALL(evt_npc_set_position, STRING("me"), -500, 0, 75)
            SPAWN_EVENT(PTR(nancy_init_check))
            ELSE
            CALL(evt_npc_set_ry, STRING("me"), 90)
            CALL(evt_npc_set_position, STRING("me"), -573, 0, 56)
        END_IF
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("w_bero"), 8388612)
        ELSE
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_set_position, STRING("me"), -405, 10, -100)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(nancy_door_anime)
    CALL(evt_mapobj_get_position, STRING("S_ie_01_naka"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_01_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 108, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 108, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_get_position, STRING("S_ie_01_naka"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_01_naka"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(GF(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(nancy_regl)
    IF_INT_EQUAL(GSWF(1187), 0)
        IF_INT_EQUAL(GSWF(1188), 0)
            CALL(urouro_init_func, STRING("me"), -500, 75, FLOAT(50.0f), FLOAT(45.0f), 0)
            CALL(urouro_main_func, STRING("me"))
        END_IF
        ELSE
        IF_INT_EQUAL(GSWF(1188), 0)
            CALL(nancy_block_init_func)
            LOOP(0)
                CALL(nancy_block_func)
                WAIT_FRAMES(1)
            END_LOOP
        END_IF
    END_IF
    RETURN
EVT_END()

s32 item_tbl_nancy[] = {
    ITEM_CONTACT_LENS, -1,
};

EVT_BEGIN(nancy_talk)
    IF_INT_EQUAL(GSWF(1188), 0)
        CALL(evt_pouch_check_item, ITEM_CONTACT_LENS, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(evt_msg_print, 0, STRING("gor_01_008"), 0, STRING("me"))
            CALL(evt_win_item_select, 0, PTR(item_tbl_nancy), LW(1), 0)
            IF_INT_EQUAL(LW(1), -1)
                SET_VALUE(LW(0), 0)
            END_IF
        END_IF
        IF_INT_LESS(LW(0), 1)
            CALL(evt_mario_set_pose, STRING("M_N_B"))
            WAIT_MS(800)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            WAIT_MS(200)
            CALL(evt_msg_print, 0, STRING("gor_01_009"), 0, STRING("me"))
            ELSE
            CALL(evt_npc_get_reglid, STRING("me"), LW(10))
            DELETE_EVENT(LW(10))
            WAIT_MS(500)
            CALL(evt_npc_reverse_ry, STRING("me"))
            WAIT_MS(700)
            CALL(evt_npc_reverse_ry, STRING("me"))
            WAIT_MS(1000)
            CALL(evt_pouch_remove_item, ITEM_CONTACT_LENS, LW(0))
            CALL(evt_msg_print, 0, STRING("gor_01_010"), 0, STRING("me"))
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_set_ry, STRING("me"), 90)
            CALL(evt_npc_move_position, STRING("me"), -380, 50, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_jump_position_nohit, STRING("me"), -380, 10, 20, 200, 30)
            CALL(evt_npc_set_ry, STRING("me"), 270)
            WAIT_MS(60)
            CALL(evt_msg_print, 0, STRING("gor_01_011"), 0, STRING("me"))
            WAIT_MS(500)
            SET_VALUE(GF(0), 1)
            SPAWN_EVENT(PTR(nancy_door_anime))
            WAIT_MS(500)
            CALL(evt_npc_move_position, STRING("me"), -370, -30, 0, FLOAT(80.0f), 0)
            WAIT_WHILE(GF(0))
#ifdef VERSION_G8ME01
            CALL(evt_npc_set_position, STRING("me"), -405, 10, -100)
#else
            CALL(evt_npc_set_position, STRING("me"), -455, 10, -170)
#endif
            CALL(evt_npc_set_ry, STRING("me"), 270)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            SET_VALUE(LW(0), STRING("w_bero"))
#ifdef VERSION_G8ME01
            CALL_CHILD(bero_case_switch_on)
#else
            CALL_CHILD(PTR(bero_case_switch_on))
#endif
            CALL(evt_hitobj_attr_onoff, 1, 1, STRING("w_bero"), 8388612)
            SET_VALUE(GSWF(1188), 1)
            CALL(evt_door_set_param, GW(6), 0, PTR(&door_data_01_npc))
        END_IF
        ELSE
        CALL_CHILD(PTR(cooking_evt))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(marco_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(223, 269)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(marco_talk)
    DELETE_EVENT(GW(0))
    CALL(evt_msg_close, STRING("mac_4_004"))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(222)
        CALL(evt_msg_print, 0, STRING("mac_4_005"), 0, STRING("me"))
        SET_VALUE(GSWF(1192), 1)
        SPAWN_EVENT_GET_ID(PTR(marco_hitorigoto), GW(0))
        CASE_EQUAL(222)
        CALL(evt_mario_dispflag_onoff, 1, 2)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        ADD(LW(0), LW(10))
        DIVIDE(LW(0), 2)
        CALL(evt_cam3d_evt_set, LW(0), 92, 128, LW(0), 40, -168, 500, 11)
        WAIT_MS(500)
        IF_INT_EQUAL(GSWF(1192), 0)
            CALL(evt_msg_print, 0, STRING("mac_4_007"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("mac_4_008"), 0, STRING("me"))
        END_IF
        CALL(evt_msg_select, 0, STRING("mac_4_007_select"))
        CALL(evt_mario_dispflag_onoff, 0, 2)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_add, 0, STRING("mac_4_009"))
            CASE_EQUAL(1)
            CALL(evt_msg_print_add, 0, STRING("mac_4_010"))
            CASE_EQUAL(2)
            CALL(evt_msg_print_add, 0, STRING("mac_4_011"))
            CASE_EQUAL(3)
            CALL(evt_msg_print_add, 0, STRING("mac_4_012"))
        END_SWITCH
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_npc_get_position, STRING("me"), LW(3), LW(1), LW(2))
        SET_VALUE(LW(11), LW(3))
        SET_VALUE(LW(12), LW(3))
        CALL(evt_npc_set_ry, STRING("me"), 270)
        SUBTRACT(LW(11), 70)
        SUBTRACT(LW(12), 50)
        CALL(evt_cam3d_evt_set, LW(11), 63, 152, LW(11), 45, -169, 1000, 11)
        BEGIN_INLINE_EVENT
            CALL(evt_party_move_pos2, 0, 215, -160, FLOAT(120.0f))
            CALL(evt_party_set_dir_npc, 0, STRING("me"))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_mario_mov_pos2, 250, -160, FLOAT(100.0f))
            CALL(evt_mario_set_dir_npc, STRING("me"))
        END_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("me"), LW(12), LW(2), 0, FLOAT(40.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(700)
        CALL(evt_msg_print, 0, STRING("mac_4_013"), 0, STRING("me"))
        WAIT_MS(500)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(700)
        CALL(evt_msg_print, 0, STRING("mac_4_014"), 0, STRING("me"))
        WAIT_MS(500)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("mac_4_015"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_npc_reverse_ry, STRING("me"))
        CALL(evt_msg_print, 0, STRING("mac_4_016"), 0, STRING("me"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_017"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_018"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        ADD(LW(2), 10)
        CALL(evt_item_entry, STRING("map00"), ITEM_MAGICAL_MAP_LARGE, LW(0), LW(1), LW(2), 16, -1, 0)
        CALL(evt_item_flag_on, STRING("map00"), 2)
        CALL(evt_item_set_scale, STRING("map00"), FLOAT(0.5498047f))
        WAIT_MS(500)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), -10)
        CALL(evt_npc_move_position, STRING("me"), LW(3), LW(2), 0, FLOAT(80.0f), 0)
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("mac_4_019"), 0, STRING("me"))
        CALL(evt_item_delete, STRING("map00"))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NO1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_B"))
        WAIT_MS(1000)
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_020"), 0, STRING("me"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_021"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_022"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_023"), 0, STRING("me"))
        CALL(evt_npc_set_ry, STRING("me"), 90)
        WAIT_MS(500)
        CALL(evt_del_case_evt, 0, GW(8))
        CALL(evt_item_delete, STRING("ドクロジュエル"))
        WAIT_MS(500)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        WAIT_MS(500)
        CALL(evt_cam3d_evt_off, 500, 11)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_move_position, STRING("me"), 260, -100, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_move_position, STRING("me"), 110, -100, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        END_INLINE_EVENT
        WAIT_MS(3000)
        SET_VALUE(GSW_STORY_PROGRESS, 223)
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_031_08_1"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_031_08_2"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_031_08_3"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_031_08_4"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(master_init)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 222)
        CALL(evt_npc_set_position, STRING("me"), 259, 0, -275)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_20_master_tubuyaki)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("マスター"), LW(3), LW(4), LW(5))
        CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(6))
        IF_INT_GREATER_OR_EQUAL(LW(6), 100)
            CALL(evt_msg_print, 0, STRING("irai_20_030"), 0, STRING("マスター"))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(master_talk)
    IF_INT_EQUAL(GSW(1420), 20)
        IF_INT_EQUAL(GSWF(5405), 0)
        ELSE
            IF_INT_EQUAL(GSWF(5406), 0)
                CALL(evt_msg_print, 0, STRING("irai_20_027"), 0, STRING("me"))
                CALL(evt_mario_set_pose, STRING("M_I_2"))
                WAIT_MS(1000)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_cam3d_evt_set, 320, 99, -12, 322, 43, -271, 1000, 11)
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("irai_20_028"), 0, STRING("me"))
                CALL(evt_mario_set_pose, STRING("M_N_5B"))
                WAIT_MS(500)
                CALL(evt_cam3d_evt_off, 1000, 11)
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("irai_20_029"), 0, STRING("me"))
                SPAWN_EVENT(iri_20_master_tubuyaki)
                SET_VALUE(GSWF(5406), 1)
                RETURN
            ELSE
                CALL(evt_msg_print, 0, STRING("irai_20_031"), 0, STRING("me"))
                RETURN
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSW(1420), 25)
        IF_INT_EQUAL(GSWF(5406), 1)
            IF_INT_EQUAL(GSWF(5408), 0)
                CALL(evt_msg_print, 0, STRING("irai_25_00_1"), 0, 0)
                SET_VALUE(GSWF(5407), 1)
                RETURN
            END_IF
        END_IF
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(223)
        CALL(evt_msg_print, 0, STRING("gor_01_016"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(224)
        CALL(evt_msg_print, 0, STRING("mac_4_047"), 0, STRING("me"))
        SET_VALUE(GSWF(1214), 1)
        CASE_EQUAL(225)
        SET_VALUE(GF(5), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(0), 320)
            SET_VALUE(GF(5), 1)
        END_IF
        IF_INT_EQUAL(GF(5), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_party_move_pos2, 0, 270, -275, FLOAT(150.0f))
                CALL(evt_party_set_dir, 0, 90, 0)
            END_INLINE_EVENT
            CALL(evt_mario_mov_pos2, 295, -270, FLOAT(150.0f))
            CALL(evt_mario_set_dir, 90, 0, 0)
        ELSE
            BEGIN_INLINE_EVENT
                CALL(evt_party_move_pos2, 0, 370, -275, FLOAT(150.0f))
                CALL(evt_party_set_dir, 0, 270, 0)
            END_INLINE_EVENT
            CALL(evt_mario_mov_pos2, 345, -270, FLOAT(150.0f))
            CALL(evt_mario_set_dir, 270, 0, 0)
        END_IF
        IF_INT_EQUAL(GSWF(1194), 1)
            CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
            CALL(evt_msg_print, 0, STRING("mac_4_071"), 0, STRING("me"))
            GOTO(100)
        END_IF
        IF_INT_EQUAL(GSWF(1193), 0)
            CALL(evt_msg_print, 0, STRING("mac_4_061"), 0, STRING("me"))
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_5"), LW(0), LW(1), LW(2), 0)
            CALL(evt_mario_set_pose, STRING("M_N_2"))
            WAIT_MS(800)
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(800)
            CALL(evt_msg_print, 0, STRING("mac_4_062"), 0, STRING("me"))
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            WAIT_MS(2000)
            CALL(evt_msg_print, 0, STRING("mac_4_063"), 0, STRING("me"))
            SET_VALUE(GSWF(1193), 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("mac_4_064"), 0, STRING("me"))
        END_IF
        CALL(evt_msg_select, 0, STRING("mac_4_063_yesno"))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_msg_print_add, 0, STRING("mac_4_065"))
            RETURN
        END_IF
        CALL(evt_msg_print_add, 0, STRING("mac_4_066"))
        CALL(evt_cam3d_evt_set, 300, 95, 27, 300, 43, -268, 1500, 11)
        CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
        WAIT_MS(1500)
        CALL(evt_msg_print, 0, STRING("mac_4_067"), 0, STRING("me"))
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("mac_4_068"), 0, STRING("me"))
        WAIT_MS(1000)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(1000)
        CALL(evt_msg_print_party, STRING("mac_4_069"))
        SET_VALUE(GSWF(1194), 1)
        CALL(evt_cam3d_evt_off, 0, 11)
        CALL(evt_msg_print, 0, STRING("mac_4_070"), 0, STRING("me"))
        LABEL(100)
        CALL(evt_msg_select, 0, STRING("mac_4_070_yesno"))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_msg_print_add, 0, STRING("mac_4_072"))
            CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 1000)
            CALL(evt_snd_bgmon_f, PSND_CONTINUE | PSND_FADEIN, 0, 1000)
            RETURN
        END_IF
        CALL(evt_msg_print_add, 0, STRING("mac_4_073"))
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        IF_INT_EQUAL(GF(5), 1)
            CALL(evt_npc_set_ry, STRING("me"), 90)
            CALL(evt_npc_move_position, STRING("me"), 345, -260, 0, FLOAT(40.0f), 0)
            CALL(evt_npc_sound_data_set, STRING("me"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, 0, 0)
            CALL(evt_npc_jump_position_nohit, STRING("me"), 345, -20, -260, 500, 0)
            WAIT_MS(1000)
            CALL(evt_npc_jump_position_nohit, STRING("me"), 345, 0, -260, 500, 0)
            CALL(evt_npc_sound_data_reset, STRING("me"))
            CALL(evt_npc_set_ry, STRING("me"), 270)
        ELSE
            CALL(evt_npc_set_ry, STRING("me"), 270)
            CALL(evt_npc_move_position, STRING("me"), 295, -260, 0, FLOAT(40.0f), 0)
            CALL(evt_npc_jump_position_nohit, STRING("me"), 295, -20, -260, 500, 0)
            WAIT_MS(1000)
            CALL(evt_npc_jump_position_nohit, STRING("me"), 295, 0, -260, 500, 0)
            WAIT_MS(500)
            CALL(evt_npc_set_ry, STRING("me"), 90)
        END_IF
        CALL(evt_npc_move_position, STRING("me"), 325, -275, 0, FLOAT(40.0f), 0)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_item_entry, STRING("letter"), ITEM_OLD_LETTER, LW(0), LW(1), LW(2), 16, -1, 0)
        CALL(evt_item_flag_on, STRING("letter"), 10)
        CALL(evt_item_get_item, STRING("letter"))
        CALL(evt_cam3d_evt_set, 300, 95, 27, 300, 43, -268, 500, 11)
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_075"), 0, STRING("me"))
        CALL(evt_cam3d_evt_off, 500, 11)
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_076"), 0, STRING("me"))
        CALL(evt_msg_print_party, STRING("mac_4_077"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(800)
        SET_VALUE(GSW_STORY_PROGRESS, 226)
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 1000)
        CALL(evt_snd_bgmon_f, PSND_CONTINUE | PSND_FADEIN, 0, 1000)
        CASE_EQUAL(226)
        CALL(evt_msg_print, 0, STRING("mac_4_076"), 0, STRING("me"))
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_016"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_016_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_016_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_016_03"), 0, STRING("me"))
        CASE_LESS(223)
        CALL(evt_msg_print, 0, STRING("gor_01_016_04"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_016_05"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_016_06"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_016_07"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_016_08"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_016_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(shoptender_init)
    IF_INT_LESS(GSW_STORY_PROGRESS, 223)
        IF_INT_EQUAL(GSWF(1191), 1)
            CALL(evt_npc_set_position, STRING("me"), -75, 0, -30)
            CALL(evt_npc_set_ry, STRING("me"), 90)
        END_IF
        ELSE
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(shoptender_talk)
    DEBUG_PUT_MSG(STRING("店員TALK"))
    IF_INT_EQUAL(GSWF(1188), 0)
        DEBUG_PUT_MSG(STRING("店員TALK２"))
        IF_INT_EQUAL(GSWF(1191), 1)
            CALL(evt_pouch_check_item, ITEM_CONTACT_LENS, LW(0))
            IF_INT_LESS(LW(0), 1)
                CALL(evt_msg_print, 0, STRING("gor_01_015"), 0, STRING("me"))
                ELSE
                CALL(evt_msg_print, 0, STRING("gor_01_015_01"), 0, STRING("me"))
            END_IF
            RETURN
        END_IF
        SET_VALUE(LW(0), GSWF(1189))
        ADD(LW(0), GSWF(1190))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("gor_01_014"), 0, STRING("me"))
            RETURN
        END_IF
        CALL(evt_msg_print, 0, STRING("gor_01_012"), 0, STRING("me"))
        IF_INT_EQUAL(GSWF(1187), 1)
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(700)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
            CALL(evt_msg_print, 0, STRING("gor_01_013"), 0, STRING("me"))
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_cam3d_evt_set, 358, 123, -301, -189, 75, -301, 2600, 11)
            CALL(evt_npc_set_ry, STRING("me"), 0)
            CALL(evt_npc_move_position, STRING("me"), LW(10), -330, 0, FLOAT(90.0f), 0)
            CALL(evt_npc_set_anim, STRING("me"), STRING("T_1"))
            WAIT_MS(1000)
            CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
            CALL(evt_npc_set_anim, STRING("店長"), STRING("T_1"))
            WAIT_MS(1000)
            CALL(evt_npc_set_anim, STRING("店長"), STRING("S_1"))
            CALL(evt_npc_set_anim, STRING("me"), STRING("T_1"))
            WAIT_MS(500)
            CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
            WAIT_MS(500)
            CALL(evt_cam3d_evt_off, 2600, 11)
            CALL(evt_npc_set_ry, STRING("me"), 180)
            CALL(evt_npc_move_position, STRING("me"), LW(10), LW(12), 0, FLOAT(90.0f), 0)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("gor_01_013_01"), 0, STRING("me"))
            IF_INT_LESS(GSW_STORY_PROGRESS, 56)
                SET_VALUE(GSWF(1189), 1)
                ELSE
                SET_VALUE(GSWF(1190), 1)
            END_IF
            CALL(evt_sub_save_playtime, GSW(108))
        END_IF
        ELSE
        DEBUG_PUT_MSG(STRING("店員TALK３"))
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(112)
            CALL(evt_msg_print, 0, STRING("gor_01_015_02"), 0, STRING("me"))
            CASE_LESS(164)
            CALL(evt_msg_print, 0, STRING("gor_01_015_03"), 0, STRING("me"))
            CASE_LESS(215)
            CALL(evt_msg_print, 0, STRING("gor_01_015_04"), 0, STRING("me"))
            CASE_LESS(261)
            CALL(evt_msg_print, 0, STRING("gor_01_015_05"), 0, STRING("me"))
        END_SWITCH
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(yadotender_init)
    RETURN
EVT_END()

EVT_BEGIN(yadotender_talk)
    RETURN
EVT_END()

EVT_BEGIN(roten1_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

EVT_BEGIN(roten1_regl)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("me"), 450, 150, 0, FLOAT(32.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING("me"), -350, 150, 0, FLOAT(32.0f), 0)
        WAIT_MS(1000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(roten1_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_025_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_025_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_025_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_025_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_025_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_025_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_025_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_025_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_025_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(roten2_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

EVT_BEGIN(roten2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_025_00_2"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_025_01_2"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_025_02_2"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_025_03_2"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_025_04_2"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_025_05_2"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_025_06_2"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_025_07_2"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_025_08_2"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kuribo1_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

EVT_BEGIN(kuribo1_regl)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("me"), 375, 390, 0, FLOAT(40.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING("me"), -375, 390, 0, FLOAT(40.0f), 0)
        WAIT_MS(1000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(kuribo1_talk)
    IF_INT_EQUAL(GSW(1420), 4)
        IF_INT_EQUAL(GSWF(5405), 0)
            CALL(evt_msg_print, 0, STRING("irai_04_001"), 0, STRING("me"))
            SET_VALUE(GSWF(5405), 1)
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_04_002"), 0, STRING("me"))
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_026_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_026_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_026_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_026_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_026_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_026_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_026_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_026_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_026_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kuribo2_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

EVT_BEGIN(kuribo2_regl)
    CALL_CHILD(PTR(gor_villager_urouro))
    RETURN
EVT_END()

EVT_BEGIN(kuribo2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_027_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_027_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_027_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_027_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_027_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_027_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_027_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_027_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_027_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kuribo3_init)
    RETURN
EVT_END()

EVT_BEGIN(kuribo3_regl)
    CALL_CHILD(PTR(gor_villager_urouro))
    RETURN
EVT_END()

EVT_BEGIN(kuribo3_talk)
    IF_INT_EQUAL(GSW(1420), 2)
        IF_INT_EQUAL(GSWF(5406), 0)
            IF_INT_EQUAL(GSWF(5405), 0)
                CALL(evt_msg_print, 0, STRING("irai_02_00"), 0, STRING("me"))
                CALL(evt_item_entry, STRING("irai_item"), ITEM_BOX, 0, -1000, 0, 16, -1, 0)
                CALL(evt_item_flag_on, STRING("irai_item"), 2)
                CALL(evt_item_get_item, STRING("irai_item"))
                SET_VALUE(GSWF(5405), 1)
                ELSE
                CALL(evt_msg_print, 0, STRING("irai_02_01"), 0, STRING("me"))
            END_IF
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_02_02"), 0, STRING("me"))
            CALL(evt_sub_get_coin, 20)
            SET_VALUE(GSW(1420), 0)
            SET_VALUE(GSWF(5376), 1)
            CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_028_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_028_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_028_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_028_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_028_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_028_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_028_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_028_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_028_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(chusan1_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    IF_INT_EQUAL(GSW(1420), 5)
        CALL(evt_npc_set_position, STRING("me"), -80, 0, 40)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        SET_VALUE(GF(9), 1)
        SPAWN_EVENT(PTR(chusan1_naguru))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(chusan1_regl)
    IF_INT_NOT_EQUAL(GSW(1420), 5)
        DEBUG_PUT_MSG(STRING("マルチュロ：普段のうろうろ"))
        CALL_CHILD(PTR(gor_villager_urouro))
    END_IF
    DEBUG_PUT_MSG(STRING("マルチュロ：依頼時は何もしない"))
    RETURN
EVT_END()

EVT_BEGIN(chusan1_dead)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_REQUEST_DAMAGED1"), LW(0), LW(1), LW(2), 0)
    IF_INT_GREATER_OR_EQUAL(GW(9), 11)
        SET_VALUE(GW(9), 0)
    END_IF
    ADD(GW(9), 1)
    BEGIN_INLINE_EVENT
        SWITCH(GW(9))
            CASE_EQUAL(1)
            CALL(evt_msg_print, 0, STRING("irai_05_004"), 0, STRING("me"))
            CASE_EQUAL(2)
            CALL(evt_msg_print, 0, STRING("irai_05_005"), 0, STRING("me"))
            CASE_EQUAL(3)
            CALL(evt_msg_print, 0, STRING("irai_05_006"), 0, STRING("me"))
            CASE_EQUAL(4)
            CALL(evt_msg_print, 0, STRING("irai_05_007"), 0, STRING("me"))
            CASE_EQUAL(5)
            CALL(evt_msg_print, 0, STRING("irai_05_008"), 0, STRING("me"))
            CASE_EQUAL(6)
            CALL(evt_msg_print, 0, STRING("irai_05_009"), 0, STRING("me"))
            CASE_EQUAL(7)
            CALL(evt_msg_print, 0, STRING("irai_05_010"), 0, STRING("me"))
            CASE_EQUAL(8)
            CALL(evt_msg_print, 0, STRING("irai_05_011"), 0, STRING("me"))
            CASE_EQUAL(9)
            CALL(evt_msg_print, 0, STRING("irai_05_012"), 0, STRING("me"))
            CASE_EQUAL(10)
            CALL(evt_msg_print, 0, STRING("irai_05_013"), 0, STRING("me"))
            CASE_EQUAL(11)
            CALL(evt_msg_print, 0, STRING("irai_05_014"), 0, STRING("me"))
        END_SWITCH
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(chusan1_naguru)
    WAIT_WHILE(GF(9))
    CALL(evt_mario_key_onoff, 0)
    CALL(chusan1_alwaysencont, 1)
    CALL(evt_npc_get_reglid, STRING("チューさん１"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_reaction_flag_onoff, 1, STRING("チューさん１"), 2)
    CALL(evt_npc_change_interrupt, STRING("チューさん１"), 8, PTR(chusan1_dead))
    CALL(evt_mario_set_enable_key, 512)
    WAIT_MS(15000)
    CALL(evt_mario_set_enable_key, 0)
    CALL(evt_npc_reaction_flag_onoff, 0, STRING("チューさん１"), 2)
    IF_INT_GREATER_OR_EQUAL(GW(9), 11)
        CALL(evt_msg_print, 0, STRING("irai_05_015"), 0, STRING("チューさん１"))
        SET_VALUE(GSW(1420), 0)
        SET_VALUE(GSWF(5379), 1)
        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        ELSE
        CALL(evt_msg_print, 0, STRING("irai_05_016"), 0, STRING("チューさん１"))
        SET_VALUE(GF(9), 1)
        SPAWN_EVENT(PTR(chusan1_naguru))
    END_IF
    CALL(chusan1_alwaysencont, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(chusan1_talk)
    IF_INT_EQUAL(GSW(1420), 5)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
        BEGIN_INLINE_EVENT
            ADD(LW(10), 60)
            CALL(evt_party_move_pos2, 0, LW(10), LW(12), FLOAT(120.0f))
            CALL(evt_party_set_dir, 0, 270, 200)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            ADD(LW(10), 90)
            CALL(evt_party_move_pos2, 1, LW(10), LW(12), FLOAT(120.0f))
            CALL(evt_party_set_dir, 1, 270, 200)
        END_INLINE_EVENT
        ADD(LW(10), 30)
        CALL(evt_mario_mov_pos2, LW(10), LW(12), FLOAT(120.0f))
        CALL(evt_mario_set_dir, 270, 200, 0)
        CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
        WAIT_MS(500)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        IF_INT_EQUAL(GSWF(5405), 0)
            CALL(evt_msg_print, 0, STRING("irai_05_001"), 0, STRING("me"))
            SET_VALUE(GSWF(5405), 0)
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_05_002"), 0, STRING("me"))
        END_IF
        CALL(evt_msg_print, 0, STRING("irai_05_003"), 0, STRING("me"))
        SET_VALUE(GW(9), 0)
        SET_VALUE(GF(9), 0)
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_029_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_029_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_029_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_029_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_029_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_029_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_029_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_029_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_029_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(chusan2_init)
    RETURN
EVT_END()

EVT_BEGIN(chusan2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_030_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_030_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_030_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_030_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_030_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_030_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_030_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_030_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_030_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(bomhei_init)
    RETURN
EVT_END()

EVT_BEGIN(bomhei_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("gor_01_031_00"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("gor_01_031_01"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("gor_01_031_02"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("gor_01_031_03"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("gor_01_031_04"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("gor_01_031_05"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("gor_01_031_06"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("gor_01_031_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_031_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(bottakuru_init)
    IF_INT_LESS(GSW_STORY_PROGRESS, 56)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        RETURN
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    SET_VALUE(GSWF(1206), 0)
    RETURN
EVT_END()

EVT_BEGIN(bottakuru_talk)
    CALL(U_evt_badgeShop_get_special_zaiko, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("bottakuru_16"), 0, STRING("me"))
        RETURN
        ELSE
        CALL(evt_badgeShop_bottakuru_get_kind_cnt, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("bottakuru_03"), 0, STRING("me"))
            RETURN
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1205), 0)
        CALL(evt_msg_print, 0, STRING("bottakuru_00"), 0, STRING("me"))
        SET_VALUE(GSWF(1205), 1)
        ELSE
        IF_INT_EQUAL(GSWF(1206), 0)
            CALL(evt_msg_print, 0, STRING("bottakuru_01"), 0, STRING("me"))
            ELSE
            CALL(evt_msg_print, 0, STRING("bottakuru_02"), 0, STRING("me"))
        END_IF
    END_IF
    CALL(evt_msg_select, 0, STRING("bottakuru_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("bottakuru_04"))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    LABEL(0)
    CALL(evt_win_coin_on, 0, LW(12))
    CALL(evt_win_other_select, 14)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print, 0, STRING("bottakuru_14"), 0, STRING("me"))
        RETURN
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print, 0, STRING("bottakuru_05"), 0, STRING("me"))
        GOTO(0)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(1), 240)
        CALL(evt_sub_get_language, LW(0))
#ifdef VERSION_G8ME01
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("bottakuru_07"), 0, STRING("me"), LW(2), LW(3), LW(4))
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("bottakuru_07"), LW(3))
            CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
            CALL(evt_msg_print_insert, 1, LW(14), 0, STRING("me"), LW(4))
        END_SWITCH
        ELSE
        CALL(evt_sub_get_language, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("bottakuru_06"), 0, STRING("me"), LW(2), LW(3))
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("bottakuru_06"), LW(3))
            CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
            CALL(evt_msg_print, 1, LW(14), 0, STRING("me"))
        END_SWITCH
#else
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_insert, 0, STRING("bottakuru_07"), 0, STRING("me"), LW(3), LW(2), LW(4))
            ELSE
            CALL(evt_msg_print_insert, 0, STRING("bottakuru_07"), 0, STRING("me"), LW(2), LW(3), LW(4))
        END_IF
        ELSE
        CALL(evt_sub_get_language, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_insert, 0, STRING("bottakuru_06"), 0, STRING("me"), LW(3), LW(2))
            ELSE
            CALL(evt_msg_print_insert, 0, STRING("bottakuru_06"), 0, STRING("me"), LW(2), LW(3))
        END_IF
#endif
    END_IF
    CALL(evt_msg_select, 0, STRING("bottakuru_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print_add, 0, STRING("bottakuru_08"))
        RETURN
    END_IF
    CALL(evt_pouch_add_item, LW(1), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_win_coin_off, LW(12))
        IF_INT_GREATER_OR_EQUAL(LW(1), 240)
            CALL(evt_msg_print_add, 0, STRING("bottakuru_12"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("bottakuru_13"))
        END_IF
        RETURN
    END_IF
    CALL(evt_badgeShop_bottakuru_dec, LW(1))
    MULTIPLY(LW(3), -1)
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(12))
    SET_VALUE(GSWF(1206), 1)
    CALL(U_evt_badgeShop_get_special_zaiko, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("bottakuru_15"))
        RETURN
        ELSE
        CALL(evt_badgeShop_bottakuru_get_kind_cnt, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("bottakuru_09"))
            RETURN
        END_IF
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("bottakuru_09"))
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("bottakuru_10"))
    CALL(evt_msg_select, 0, STRING("bottakuru_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("bottakuru_11"))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    GOTO(0)
    RETURN
EVT_END()

static EVT_BEGIN(luigi_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(112, 163)
        CALL(evt_npc_set_position, STRING("me"), 380, 0, 300)
        CALL(evt_npc_set_ry_lr, STRING("me"), 0)
        CASE_BETWEEN(215, 260)
        CALL(evt_npc_set_position, STRING("me"), 110, 0, -240)
        CALL(evt_npc_set_ry_lr, STRING("me"), 1)
        CASE_GREATER_OR_EQUAL(373)
        CALL(evt_npc_set_position, STRING("me"), 230, 154, -300)
        CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(luigi_talk)
    CALL_CHILD(PTR(luigi_gor_first_talk))
    RETURN
EVT_END()

static EVT_BEGIN(luigi_npcEnt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(112, 163)
        CALL(evt_npc_entry, STRING("キック"), STRING("c_lp_gesso"))
        CALL(evt_npc_set_tribe, STRING("キック"), STRING("キック"))
        CALL(evt_npc_setup, PTR(kickEnt))
        CASE_BETWEEN(215, 260)
        CALL(evt_npc_entry, STRING("トルク"), STRING("c_lp_met"))
        CALL(evt_npc_set_tribe, STRING("トルク"), STRING("トルク"))
        CALL(evt_npc_setup, PTR(torqueEnt))
        CASE_GREATER_OR_EQUAL(373)
        CALL(evt_npc_entry, STRING("キック２"), STRING("c_lp_gesso"))
        CALL(evt_npc_set_tribe, STRING("キック２"), STRING("キック２"))
        CALL(evt_npc_setup, PTR(kick2Ent))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(kick_init)
    CALL(evt_npc_set_position, STRING("me"), 420, 0, 300)
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    RETURN
EVT_END()

EVT_BEGIN(kick_talk)
    CALL(evt_msg_print, 0, STRING("luigi_npc_01"), 0, STRING("me"))
    RETURN
EVT_END()

NpcSetupInfo kickEnt[] = {
    {
        /*"キック"*/"\x83\x4C\x83\x62\x83\x4E", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        kick_init, // initEvent
        NULL, // regularEvent
        kick_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(torque_init)
    CALL(evt_npc_set_position, STRING("me"), 160, 0, -240)
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    RETURN
EVT_END()

EVT_BEGIN(torque_talk)
    CALL(evt_msg_print, 0, STRING("luigi_npc_03"), 0, STRING("me"))
    RETURN
EVT_END()

NpcSetupInfo torqueEnt[] = {
    {
        /*"トルク"*/"\x83\x67\x83\x8B\x83\x4E", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        torque_init, // initEvent
        NULL, // regularEvent
        torque_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(kick2_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(373, 400)
        CALL(evt_npc_set_position, STRING("me"), 270, 154, -300)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    RETURN
EVT_END()

EVT_BEGIN(kick2_talk)
    CALL(evt_msg_print, 0, STRING("luigi_npc_06"), 0, STRING("me"))
    RETURN
EVT_END()

NpcSetupInfo kick2Ent[] = {
    {
        /*"キック２"*/"\x83\x4C\x83\x62\x83\x4E\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        kick2_init, // initEvent
        NULL, // regularEvent
        kick2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(borodo2_init)
    IF_INT_EQUAL(GSW(1420), 4)
        IF_INT_EQUAL(GSWF(5405), 1)
            IF_INT_EQUAL(GSWF(5406), 1)
                IF_INT_EQUAL(GSWF(5407), 1)
                    IF_INT_EQUAL(GSWF(5408), 0)
                        CALL(evt_npc_entry, STRING("ボロドー２"), STRING("c_borodo_n"))
                        CALL(evt_npc_set_tribe, STRING("ボロドー２"), STRING("盗賊"))
                        CALL(evt_npc_flag_onoff, 1, STRING("ボロドー２"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
                        CALL(evt_npc_set_position, STRING("ボロドー２"), 402, 0, -302)
                        CALL(evt_npc_set_ry, STRING("ボロドー２"), 90)
                        CALL(evt_npc_change_interrupt, STRING("ボロドー２"), 6, PTR(borodo2_talk))
                        RETURN
                    END_IF
                END_IF
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(borodo2_talk)
    CALL(evt_npc_get_reglid, STRING("クリボー１"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_msg_print, 0, STRING("irai_04_005"), 0, STRING("me"))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("クリボー１"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("クリボー１"), 229, 0, -400)
    CALL(evt_npc_set_ry, STRING("クリボー１"), 90)
    CALL(evt_npc_move_position, STRING("クリボー１"), 420, -400, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_move_position, STRING("クリボー１"), 420, -350, 0, FLOAT(120.0f), 0)
    CALL(evt_set_dir_to_target, STRING("クリボー１"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("irai_04_006"), 0, STRING("クリボー１"))
    CALL(evt_sub_get_coin, 20)
    CALL(evt_npc_set_ry, STRING("クリボー１"), 270)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("irai_04_008"), 0, STRING("クリボー１"))
    CALL(evt_msg_print, 0, STRING("irai_04_009"), 0, STRING("me"))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("me"), 400, -400, 0, FLOAT(120.0f), 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("クリボー１"), 420, -400, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_ry, STRING("クリボー１"), 270)
    WAIT_MS(200)
    WAIT_WHILE(GF(0))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_npc_move_position, STRING("クリボー１"), 392, -400, 0, FLOAT(120.0f), 0)
        WAIT_MS(800)
        CALL(evt_npc_move_position, STRING("クリボー１"), 362, -400, 0, FLOAT(120.0f), 0)
        WAIT_MS(800)
        CALL(evt_npc_move_position, STRING("クリボー１"), 332, -400, 0, FLOAT(120.0f), 0)
        WAIT_MS(800)
        CALL(evt_npc_move_position, STRING("クリボー１"), 302, -400, 0, FLOAT(120.0f), 0)
        WAIT_MS(800)
        CALL(evt_npc_move_position, STRING("クリボー１"), 272, -400, 0, FLOAT(120.0f), 0)
        WAIT_MS(800)
        CALL(evt_npc_move_position, STRING("クリボー１"), 242, -400, 0, FLOAT(120.0f), 0)
        CALL(evt_npc_set_position, STRING("クリボー１"), -375, 0, 390)
        CALL(evt_npc_restart_regular_event, STRING("クリボー１"))
        CALL(evt_npc_flag_onoff, 0, STRING("クリボー１"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_ry, STRING("me"), 270)
    CALL(evt_npc_move_position, STRING("me"), 390, -395, 0, FLOAT(120.0f), 0)
    WAIT_MS(800)
    CALL(evt_npc_move_position, STRING("me"), 360, -395, 0, FLOAT(120.0f), 0)
    WAIT_MS(800)
    CALL(evt_npc_move_position, STRING("me"), 330, -395, 0, FLOAT(120.0f), 0)
    WAIT_MS(800)
    CALL(evt_npc_move_position, STRING("me"), 300, -395, 0, FLOAT(120.0f), 0)
    WAIT_MS(800)
    CALL(evt_npc_move_position, STRING("me"), 270, -395, 0, FLOAT(120.0f), 0)
    WAIT_MS(800)
    CALL(evt_npc_move_position, STRING("me"), 242, -395, 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    WAIT_WHILE(GF(0))
    SET_VALUE(GSWF(5408), 1)
    SET_VALUE(GSW(1420), 0)
    SET_VALUE(GSWF(5378), 1)
    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(thief_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(8)
        CALL(evt_npc_set_position, STRING("me"), 500, 0, -175)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(thief_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(8, 22)
        IF_INT_EQUAL(GSWF(1198), 0)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW(2), 0)
                IF_INT_EQUAL(GSW(3), 0)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                CALL(evt_msg_print, 0, STRING("mac_0_065_01"), 0, STRING("me"))
                BEGIN_INLINE_EVENT
                    CALL(evt_StatusOpen)
                    WAIT_MS(1000)
                    CALL(evt_pouch_get_coin, LW(0))
                    ADD(LW(0), GSW(2))
                    SET_VALUE(LW(1), GSW(3))
                    MULTIPLY(LW(1), 256)
                    ADD(LW(0), LW(1))
                    CALL(evt_pouch_set_coin, LW(0))
                    WAIT_MS(2000)
                    CALL(evt_StatusClose)
                END_INLINE_EVENT
                ELSE
                CALL(evt_msg_print, 0, STRING("mac_0_065_02"), 0, STRING("me"))
            END_IF
            SET_VALUE(GSWF(1198), 1)
            ELSE
            IF_INT_EQUAL(GSWF(1199), 0)
                CALL(evt_msg_print, 0, STRING("mac_0_065_03"), 0, STRING("me"))
                ELSE
                CALL(evt_msg_print, 0, STRING("mac_0_065_04"), 0, STRING("me"))
            END_IF
        END_IF
        CASE_LESS(56)
        CALL(evt_msg_print, 0, STRING("mac_0_065_04"), 0, STRING("me"))
        CASE_LESS(112)
        CALL(evt_msg_print, 0, STRING("mac_0_065_05"), 0, STRING("me"))
        CASE_LESS(164)
        CALL(evt_msg_print, 0, STRING("mac_0_065_06"), 0, STRING("me"))
        CASE_LESS(215)
        CALL(evt_msg_print, 0, STRING("mac_0_065_07"), 0, STRING("me"))
        CASE_LESS(261)
        CALL(evt_msg_print, 0, STRING("mac_0_065_08"), 0, STRING("me"))
        CASE_LESS(339)
        CALL(evt_msg_print, 0, STRING("mac_0_065_09"), 0, STRING("me"))
        CASE_LESS(373)
        CALL(evt_msg_print, 0, STRING("mac_0_065_10"), 0, STRING("me"))
        CASE_LESS(401)
        CALL(evt_msg_print, 0, STRING("mac_0_065_11"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("mac_0_065_12"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"バッジ店長"*/"\x83\x6F\x83\x62\x83\x57\x93\x58\x92\xB7", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        badgemaster_init, // initEvent
        NULL, // regularEvent
        badgemaster_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"バッジ店員"*/"\x83\x6F\x83\x62\x83\x57\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        npc_init_evt, // initEvent
        NULL, // regularEvent
        badgeshopper_talk_evt, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノじい"*/"\x83\x4C\x83\x6D\x82\xB6\x82\xA2", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        kinoji_init, // initEvent
        NULL, // regularEvent
        kinoji_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ナンシー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        nancy_init, // initEvent
        nancy_regl, // regularEvent
        nancy_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
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
        /*"マスター"*/"\x83\x7D\x83\x58\x83\x5E\x81\x5B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        master_init, // initEvent
        NULL, // regularEvent
        master_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"店長"*/"\x93\x58\x92\xB7", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"店員"*/"\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        shoptender_init, // initEvent
        NULL, // regularEvent
        shoptender_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        yadotender_init, // initEvent
        NULL, // regularEvent
        yadotender_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ロテン１"*/"\x83\x8D\x83\x65\x83\x93\x82\x50", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        roten1_init, // initEvent
        roten1_regl, // regularEvent
        roten1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ロテン２"*/"\x83\x8D\x83\x65\x83\x93\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        roten2_init, // initEvent
        NULL, // regularEvent
        roten2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クリボー１"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x50", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        kuribo1_init, // initEvent
        kuribo1_regl, // regularEvent
        kuribo1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クリボー２"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        kuribo2_init, // initEvent
        NULL, // regularEvent
        kuribo2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"クリボー３"*/"\x83\x4E\x83\x8A\x83\x7B\x81\x5B\x82\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        kuribo3_init, // initEvent
        kuribo3_regl, // regularEvent
        kuribo3_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"チューさん１"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x50", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        chusan1_init, // initEvent
        chusan1_regl, // regularEvent
        chusan1_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"チューさん２"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x51", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        chusan2_init, // initEvent
        NULL, // regularEvent
        chusan2_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        bomhei_init, // initEvent
        NULL, // regularEvent
        bomhei_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボッタクール"*/"\x83\x7B\x83\x62\x83\x5E\x83\x4E\x81\x5B\x83\x8B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        bottakuru_init, // initEvent
        NULL, // regularEvent
        bottakuru_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ルイージ"*/"\x83\x8B\x83\x43\x81\x5B\x83\x57", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        luigi_init, // initEvent
        NULL, // regularEvent
        luigi_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"盗賊"*/"\x93\x90\x91\xAF", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, // reactionFlags
        thief_init, // initEvent
        NULL, // regularEvent
        thief_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピコ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x52", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(christine_nakama_before)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, 6)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    SPAWN_EVENT(PTR(christine_nakama))
    SET_VALUE(LW(0), 1)
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
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gor_02", // nextMapName
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
        "gor_03", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        christine_nakama_before, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gor_00", // nextMapName
        "n_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(evt_door_01_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_01_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_01)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01uB"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01sB"), LW(3), 0, 0)
    RETURN
EVT_END()

static const char* door_data_01_npc[] = {
    /*"ナンシー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B",
    NULL,
};

const char* door_data_01_item[] = {
    "iri_18",
    NULL,
};

static EVT_BEGIN(evt_door_01_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_01_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

static DoorParam door_data_01 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_01_doa", // mapobj
    "A_ie_01_doa02", // hitIn
    "A_ie_01_doa01", // hitOut
    "S_ie_01_soto", // outside
    "A_ie_01_A", // outsideHit
    "S_ie_01_naka", // inside
    "A_ie_01_C", // insideHit
    evt_door_01_00, // evtIn
    evt_door_01_01, // evtOut
    evt_wall_01, // evtWall
    NULL, NULL, // npcs..mobjs
    door_data_01_item, // items
    evt_door_01_open, // evtOpen
    evt_door_01_close, // evtClose
};

DoorParam door_data_01_01 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_01_doa", // mapobj
    "A_ie_01_doa02", // hitIn
    "A_ie_01_doa01", // hitOut
    "S_ie_01_soto", // outside
    "A_ie_01_A", // outsideHit
    "S_ie_01_naka", // inside
    "A_ie_01_C", // insideHit
    evt_door_01_00, // evtIn
    evt_door_01_01, // evtOut
    evt_wall_01, // evtWall
    (const char**)door_data_01_npc, // npcs
    NULL, // mobjs
    door_data_01_item, // items
    evt_door_01_open, // evtOpen
    evt_door_01_close, // evtClose
};

EVT_BEGIN(evt_door_02_00_f1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f1_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_02_f1)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    SET_FLOAT_VALUE(LW(4), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.0f))
    MULTIPLY_FLOAT(LW(4), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f1B"), 0, 0, LW(2))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2uB"), 0, 0, LW(3))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2sB"), 0, 0, LW(4))
    RETURN
EVT_END()

static const char* door_data_02_npc[] = {
    /*"店長"*/"\x93\x58\x92\xB7",
    /*"店員"*/"\x93\x58\x88\xF5",
    NULL,
};

const char* door_data_02_npc_01[] = {
    /*"店長"*/"\x93\x58\x92\xB7",
    NULL,
};

EVT_BEGIN(evt_door_02f1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_02f1_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

DoorParam door_data_02_f1 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_02_f1_doa", // mapobj
    "A_ie_02_f1_doa02", // hitIn
    "A_ie_02_f1_doa01", // hitOut
    "S_ie_02_f1_soto", // outside
    NULL, // outsideHit
    "S_ie_02_f1_naka", // inside
    "A_ie_02_f1_C", // insideHit
    evt_door_02_00_f1, // evtIn
    NULL, // evtOut
    evt_wall_02_f1, // evtWall
    door_data_02_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_door_02f1_open, // evtOpen
    evt_door_02f1_close, // evtClose
};

DoorParam door_data_02_f1_01 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_02_f1_doa", // mapobj
    "A_ie_02_f1_doa02", // hitIn
    "A_ie_02_f1_doa01", // hitOut
    "S_ie_02_f1_soto", // outside
    NULL, // outsideHit
    "S_ie_02_f1_naka", // inside
    "A_ie_02_f1_C", // insideHit
    evt_door_02_00_f1, // evtIn
    NULL, // evtOut
    evt_wall_02_f1, // evtWall
    door_data_02_npc_01, // npcs
    NULL, NULL, // mobjs..items
    evt_door_02f1_open, // evtOpen
    evt_door_02f1_close, // evtClose
};

EVT_BEGIN(evt_door_02_f2_junbityu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("gor_01_024_00_00"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_02_00_f2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2_doaD"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_02_f2)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    SET_FLOAT_VALUE(LW(4), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.0f))
    MULTIPLY_FLOAT(LW(4), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f1B"), 0, 0, LW(2))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2uB"), 0, 0, LW(3))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2sB"), 0, 0, LW(4))
    RETURN
EVT_END()

const char* badgemise_npc[] = {
    /*"バッジ店長"*/"\x83\x6F\x83\x62\x83\x57\x93\x58\x92\xB7",
    /*"バッジ店員"*/"\x83\x6F\x83\x62\x83\x57\x93\x58\x88\xF5",
    NULL,
};

EVT_BEGIN(evt_badge_dummy)
    CALL(evt_mario_normalize)
    SET_VALUE(LW(10), LW(0))
    CALL(disp_off, LW(10))
    CALL(evt_set_dir_to_target, STRING("バッジ店員"), STRING("mario"))
    CALL(evt_mario_set_dir_npc, STRING("バッジ店員"))
    CALL(evt_win_coin_on, 0, LW(8))
    CALL(get_value, LW(10), LW(11), LW(12), LW(13), LW(14))
    CALL(evt_sub_get_language, LW(0))
#ifdef VERSION_G8ME01
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(11), LW(13), LW(14))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(15), STRING("badgeshop_00"), LW(13))
        CALL(evt_msg_fmt_str, 1, LW(15), LW(15), LW(11))
        CALL(evt_msg_print_insert, 1, LW(15), 0, STRING("バッジ店員"), LW(14))
    END_SWITCH
#else
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(13), LW(11), LW(14))
        ELSE
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(11), LW(13), LW(14))
    END_IF
#endif
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_22"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(13))
        CALL(evt_msg_print_add, 0, STRING("badgeshop_01"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(evt_pouch_add_item, LW(12), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_msg_print_add, 0, STRING("badgeshop_02"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(set_new_goods_list, LW(10), 0)
    CALL(evt_badgeShop_special_dec, LW(12))
    CALL(unitwin_set_bought, LW(10), LW(12))
    MULTIPLY(LW(13), -1)
    CALL(evt_pouch_add_coin, LW(13))
    CALL(evt_win_coin_wait, LW(8))
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(8))
    CALL(evt_msg_continue)
    RETURN
EVT_END()

UnitWinSell uwnobj_list[] = {
    {"S_item_07", "A_item_08", evt_badge_dummy},
    {"S_item_08", "A_item_09", evt_badge_dummy},
    {"S_item_09", "A_item_10", evt_badge_dummy},
    {"S_item_10", "A_item_11", evt_badge_dummy},
    {"S_item_11", "A_item_12", evt_badge_dummy},
    {0},
};

EVT_BEGIN(evt_open_02_01_f2)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_unitwin_alloc, 1)
    CALL(badgeShop_getBargainTable, LW(0))
    CALL(evt_unitwin_selltable_setup, PTR(uwnobj_list), LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_close_sub)
    CALL(mario_keyoff_wait)
    CALL(evt_unitwin_free)
    RETURN
EVT_END()

EVT_BEGIN(evt_close_02_01_f2)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    SPAWN_EVENT(PTR(evt_close_sub))
    RETURN
EVT_END()

DoorParam door_data_02_f2 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_02_f2_doaD", // mapobj
    "A_ie_02_f2_doa02", // hitIn
    "A_ie_02_f2_doa01", // hitOut
    "S_ie_02_f2_soto", // outside
    NULL, // outsideHit
    "S_ie_02_f2_naka", // inside
    "A_ie_02_f2_C", // insideHit
    evt_door_02_00_f2, // evtIn
    NULL, // evtOut
    evt_wall_02_f2, // evtWall
    (const char**)badgemise_npc, // npcs
    NULL, NULL, // mobjs..items
    evt_open_02_01_f2, // evtOpen
    evt_close_02_01_f2, // evtClose
};

USER_FUNC(house03_f1_fade_func) {
    s32 v = evtGetValue(event, event->args[0]);

    mapGrpSetColor("S_i03_f1_naka", (GXColor){v, v, v, 0xFF});
    return EVT_RETURN_DONE;
}

USER_FUNC(house03_f2_fade_func) {
    u8 v = evtGetValue(event, event->args[0]);

    mapGrpSetColor("S_i03_f2_naka", (GXColor){v, v, v, 0xFF});
    mapGrpSetColor("S_ie_03_f21_doa", (GXColor){v, v, v, 0xFF});
    return EVT_RETURN_DONE;
}

USER_FUNC(chk_marco_fuki) {
    EvtWord* args = event->args;
    NpcEntry* npc = fbatNpcTalkCheck(marioGetPtr());

    if (npc != NULL && strcmp(npc->description, "\203}\203\213\203R") == 0) {
        evtSetValue(event, args[0], 1);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[0], 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(maffia_kemuri) {
    EvtWord* args = event->args;
    f32 x = evtGetValue(event, args[0]);
    f32 y = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);

    effKemuriEntry(1, x, y, z, 0.0f);
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_osreport) {
    EvtWord* args = event->args;

    switch (evtGetValue(event, args[0])) {
        case 0:
            evtSetValue(event, args[1], search_evt_moji(evtGetValue(event, GSW_STORY_PROGRESS)));
            break;
    }
    return EVT_RETURN_DONE;
}

static s32 npc_process(void) {
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
    return 0;
}

static USER_FUNC(delete_npc_regl_evt) {
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));

    if (npc->regularEvtId != 0) {
        evtDeleteID(npc->regularEvtId);
        npc->moveEvent = NULL;
        npc->regularEvtId = 0;
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(house03_f1f2_fade)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(1), 100)
            SET_VALUE(GW(10), 2)
            ELSE
            SET_VALUE(GW(10), 1)
        END_IF
        IF_INT_EQUAL(GW(10), 1)
            SET_VALUE(GW(14), 1)
            SET_VALUE(GW(15), -1)
            ELSE
            SET_VALUE(GW(14), -1)
            SET_VALUE(GW(15), 1)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(luigi_f1_fade_check)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(112, 163)
        CASE_BETWEEN(215, 260)
        CALL(evt_npc_set_color, STRING("ルイージ"), LW(0), LW(0), LW(0), 255)
        CALL(evt_npc_set_color, STRING("トルク"), LW(0), LW(0), LW(0), 255)
        CASE_GREATER_OR_EQUAL(373)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(house03_f1_fade)
    LOOP(0)
        IF_INT_EQUAL(GW(14), 1)
            ADD(LW(0), 10)
            IF_INT_GREATER_OR_EQUAL(LW(0), 255)
                SET_VALUE(LW(0), 255)
            END_IF
        END_IF
        IF_INT_EQUAL(GW(14), -1)
            ADD(LW(0), -10)
            IF_INT_LESS_OR_EQUAL(LW(0), 0)
                SET_VALUE(LW(0), 0)
            END_IF
        END_IF
        CALL(house03_f1_fade_func, LW(0))
        CALL(evt_npc_set_color, STRING("水夫ボム兵"), LW(0), LW(0), LW(0), 255)
        CALL(evt_npc_set_color, STRING("チューさん２"), LW(0), LW(0), LW(0), 255)
        CALL_CHILD(PTR(luigi_f1_fade_check))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(luigi_f2_fade_check)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(112, 163)
        CASE_BETWEEN(215, 260)
        CASE_GREATER_OR_EQUAL(373)
        CALL(evt_npc_set_color, STRING("ルイージ"), LW(0), LW(0), LW(0), 255)
        CALL(evt_npc_set_color, STRING("キック２"), LW(0), LW(0), LW(0), 255)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(house03_f2_fade)
    LOOP(0)
        IF_INT_EQUAL(GW(15), 1)
            ADD(LW(0), 10)
            IF_INT_GREATER_OR_EQUAL(LW(0), 255)
                SET_VALUE(LW(0), 255)
            END_IF
        END_IF
        IF_INT_EQUAL(GW(15), -1)
            ADD(LW(0), -10)
            IF_INT_LESS_OR_EQUAL(LW(0), 0)
                SET_VALUE(LW(0), 0)
            END_IF
        END_IF
        CALL(house03_f2_fade_func, LW(0))
        CALL(evt_npc_set_color, STRING("宿店員"), LW(0), LW(0), LW(0), 255)
        CALL(evt_npc_set_color, STRING("キノじい"), LW(0), LW(0), LW(0), 255)
        CALL_CHILD(PTR(luigi_f2_fade_check))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(house03_suiteki)
    LOOP(0)
        CALL(evt_map_playanim, STRING("sizuku"), 0, 0)
        CALL(evt_map_checkanim, STRING("sizuku"), LW(0), LW(1))
        WAIT_MS(LW(1))
        CALL(evt_mapobj_get_position, STRING("S_i03_f2_mizu"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_AMB_WATERDROP2"), LW(0), 300, LW(2), LW(3))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER(LW(1), 100)
            CALL(evt_snd_sfx_vol, LW(3), 127)
            ELSE
            CALL(evt_snd_sfx_vol, LW(3), 30)
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(marco_hitorigoto)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 223)
        RETURN
    END_IF
    CALL(evt_mario_wait_movable)
    SET_VALUE(LSWF(2), 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(chk_marco_fuki, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                IF_INT_EQUAL(LSWF(2), 1)
                    CALL(evt_msg_close, STRING("mac_4_004"))
                END_IF
                SET_VALUE(LSWF(2), 0)
                ELSE
                SET_VALUE(LSWF(2), 1)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(LSWF(2), 1)
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LSWF(2), 1)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_msg_print, 0, STRING("mac_4_004"), 0, STRING("マルコ"))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_door_03_00_f1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_door_03_01_f1)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(house_03_f1_open)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_03_f21_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_f2_soto"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        CALL(evt_door_end_wait)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_i03_f2_soto"), MAP_OBJ_FLAG_HIDDEN)
    END_INLINE_EVENT
    SET_VALUE(GW(10), 1)
    SET_VALUE(GW(14), 0)
    SET_VALUE(GW(15), 0)
    SPAWN_EVENT_GET_ID(PTR(house03_f1f2_fade), GW(11))
    SET_VALUE(LW(0), 255)
    SPAWN_EVENT_GET_ID(PTR(house03_f1_fade), GW(12))
    SET_VALUE(LW(0), 0)
    SPAWN_EVENT_GET_ID(PTR(house03_f2_fade), GW(13))
    SPAWN_EVENT_GET_ID(PTR(marco_hitorigoto), GW(0))
    SPAWN_EVENT_GET_ID(PTR(house03_suiteki), GW(7))
    RETURN
EVT_END()

EVT_BEGIN(house_03_f1_close)
    BEGIN_INLINE_EVENT
        WAIT_MS(1600)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_f1_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_f21_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_color, 1, STRING("S_ie_03_f21_doa"), 255, 255, 255, 255)
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    SET_VALUE(GW(10), 0)
    DELETE_EVENT(GW(0))
    CALL(evt_msg_close, STRING("mac_4_004"))
    DELETE_EVENT(GW(7))
    DELETE_EVENT(GW(11))
    DELETE_EVENT(GW(12))
    DELETE_EVENT(GW(13))
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_03_f1)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    SET_FLOAT_VALUE(LW(4), LW(0))
    SET_FLOAT_VALUE(LW(5), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(0.5f))
    MULTIPLY_FLOAT(LW(4), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(5), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1B"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_i03_f2_sitaB"), LW(3), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_i03_f2_ueB"), LW(4), 0, 0)
    RETURN
EVT_END()

static const char* door_data_03_npc[] = {
    /*"マスター"*/"\x83\x7D\x83\x58\x83\x5E\x81\x5B",
    /*"キノじい"*/"\x83\x4C\x83\x6D\x82\xB6\x82\xA2",
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5",
    /*"チューさん２"*/"\x83\x60\x83\x85\x81\x5B\x82\xB3\x82\xF1\x82\x51",
    /*"水夫ボム兵"*/"\x90\x85\x95\x76\x83\x7B\x83\x80\x95\xBA",
    /*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52",
    /*"ルイージ"*/"\x83\x8B\x83\x43\x81\x5B\x83\x57",
    /*"トルク"*/"\x83\x67\x83\x8B\x83\x4E",
    /*"キック２"*/"\x83\x4C\x83\x62\x83\x4E\x82\x51",
    NULL,
};

const char* door_data_03_item[] = {
    /*"ドクロジュエル"*/"\x83\x68\x83\x4E\x83\x8D\x83\x57\x83\x85\x83\x47\x83\x8B",
    NULL,
};

DoorParam door_data_03_f1 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_03_f1_doa", // mapobj
    "A_ie_03_f1_doa02", // hitIn
    "A_ie_03_f1_doa01", // hitOut
    "S_ie_03_soto", // outside
    NULL, // outsideHit
    "S_ie_03_naka", // inside
    NULL, // insideHit
    evt_door_03_00_f1, // evtIn
    evt_door_03_01_f1, // evtOut
    evt_wall_03_f1, // evtWall
    door_data_03_npc, // npcs
    NULL, // mobjs
    door_data_03_item, // items
    house_03_f1_open, // evtOpen
    house_03_f1_close, // evtClose
};

EVT_BEGIN(evt_door_03_00_f2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f21_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(house_03_f2_open)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_03_f1_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    SET_VALUE(GW(10), 2)
    SET_VALUE(GW(14), 0)
    SET_VALUE(GW(15), 0)
    SPAWN_EVENT_GET_ID(PTR(house03_f1f2_fade), GW(11))
    SET_VALUE(LW(0), 0)
    SPAWN_EVENT_GET_ID(PTR(house03_f1_fade), GW(12))
    SET_VALUE(LW(0), 255)
    SPAWN_EVENT_GET_ID(PTR(house03_f2_fade), GW(13))
    SPAWN_EVENT_GET_ID(PTR(marco_hitorigoto), GW(0))
    SPAWN_EVENT_GET_ID(PTR(house03_suiteki), GW(7))
    RETURN
EVT_END()

EVT_BEGIN(house_03_f2_close)
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_i03_f1_naka"), MAP_OBJ_FLAG_HIDDEN)
#endif
    BEGIN_INLINE_EVENT
        WAIT_MS(1600)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_f1_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_f21_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    SET_VALUE(GW(10), 0)
    DELETE_EVENT(GW(0))
    CALL(evt_msg_close, STRING("mac_4_004"))
    DELETE_EVENT(GW(7))
    DELETE_EVENT(GW(11))
    DELETE_EVENT(GW(12))
    DELETE_EVENT(GW(13))
    RETURN
EVT_END()

EVT_BEGIN(evt_wall_03_f2)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    SET_FLOAT_VALUE(LW(4), LW(0))
    SET_FLOAT_VALUE(LW(5), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(0.5f))
    MULTIPLY_FLOAT(LW(4), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(5), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1B"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_i03_f2_sitaB"), LW(3), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_i03_f2_ueB"), LW(4), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1_doa"), LW(5), 0, 0)
    RETURN
EVT_END()

DoorParam door_data_03_f2 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_03_f21_doa", // mapobj
    "S_ie_03_f2_doa02", // hitIn
    "S_ie_03_f2_doa01", // hitOut
    "S_ie_03_soto", // outside
    NULL, // outsideHit
    "S_ie_03_naka", // inside
    NULL, // insideHit
    evt_door_03_00_f2, // evtIn
    NULL, // evtOut
    evt_wall_03_f2, // evtWall
    door_data_03_npc, // npcs
    NULL, // mobjs
    door_data_03_item, // items
    house_03_f2_open, // evtOpen
    house_03_f2_close, // evtClose
};

static EVT_BEGIN(evt_door_04_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_wall_04)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04uB"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04sB"), LW(3), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_goki_01"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    IF_INT_EQUAL(GSWF(1198), 1)
        SET_VALUE(GSWF(1199), 1)
    END_IF
    RETURN
EVT_END()

static const char* door_data_04_npc[] = {
    /*"盗賊"*/"\x93\x90\x91\xAF",
    NULL,
};

static const char* door_data_04_item[] = {
    "item_01",
    NULL,
};

static DoorParam door_data_04 = {
    0, // flags
    3, // type
    1, // se
    "S_ie_04_doa", // mapobj
    "A_ie_04_doa02", // hitIn
    "A_ie_04_doa01", // hitOut
    "S_ie_04_soto", // outside
    NULL, // outsideHit
    "S_ie_04_naka", // inside
    "A_ie_04_C", // insideHit
    evt_door_04_00, // evtIn
    evt_door_04_01, // evtOut
    evt_wall_04, // evtWall
    door_data_04_npc, // npcs
    NULL, // mobjs
    door_data_04_item, // items
    evt_door_04_open, // evtOpen
    evt_door_04_close, // evtClose
};

static EVT_BEGIN(koushukei_dai)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 10, -10, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_rop"), LW(0), 0, 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(1)
        CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -10, 10, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_rop"), LW(0), 0, 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static ShopObj obj_list[] = {
    {"S_item_01", "A_item_01"},
    {"S_item_02", "A_item_02"},
    {"S_item_03", "A_item_03"},
    {"S_item_04", "A_item_04"},
    {"S_item_05", "A_item_05"},
    {"S_item_06", "A_item_06"},
};

static ShopItem goods_list[] = {
    {ITEM_MUSHROOM, 5},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

ShopItem goods_list2[] = {
    {ITEM_CONTACT_LENS, 10},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

ShopItem goods_luigi1[] = {
    {ITEM_SUPER_LUIGI1, 64},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

ShopItem goods_luigi2[] = {
    {ITEM_SUPER_LUIGI2, 128},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

ShopItem goods_luigi3[] = {
    {ITEM_SUPER_LUIGI3, 128},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

ShopItem goods_luigi4[] = {
    {ITEM_SUPER_LUIGI4, 128},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

ShopItem goods_luigi5[] = {
    {ITEM_SUPER_LUIGI5, 256},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

static ShopDb trade_list[] = {
    {ITEM_MUSHROOM, 2, 0},
    {ITEM_HONEY_SYRUP, 2, 0},
    {ITEM_TASTY_TONIC, 1, 0},
    {ITEM_FIRE_FLOWER, 5, 0},
    {ITEM_SLEEPY_SHEEP, 4, 0},
    {ITEM_FRIGHT_MASK, 2, 0},
    {ITEM_WHACKA_BUMP, 55, 0},
    {ITEM_COCONUT, 2, 0},
    {ITEM_DRIED_BOUQUET, 12, 0},
    {ITEM_MYSTIC_EGG, 5, 0},
    {ITEM_GOLDEN_LEAF, 4, 0},
    {ITEM_KEEL_MANGO, 4, 0},
    {ITEM_FRESH_PASTA, 53, 0},
    {ITEM_CAKE_MIX, 8, 0},
    {ITEM_HOT_SAUCE, 11, 0},
    {ITEM_TURTLEY_LEAF, 2, 0},
    {ITEM_HORSETAIL, 2, 0},
    {ITEM_PEACHY_PEACH, 4, 0},
    {0},
};

static ShopperData35 shopper_data = {
    /*"店長"*/"\x93\x58\x92\xB7", // name
    NULL, NULL, // fook_evt..buy_evt
    {"gor_shop1_00", "gor_shop1_01", "gor_shop1_02", "gor_shop1_03", "gor_shop1_04", "gor_shop1_05", "gor_shop1_06", "gor_shop1_07", "gor_shop1_08", "gor_shop1_09", "gor_shop1_10", "gor_shop1_11", "gor_shop1_12", "gor_shop1_13", "gor_shop1_14", "gor_shop1_15", "gor_shop1_16", "gor_shop1_17", "gor_shop1_18", "gor_shop1_19", "gor_shop1_20", "gor_shop1_21", "gor_shop1_22", "gor_shop1_23", "gor_shop1_24", "gor_shop1_25", "gor_shop1_26", "gor_shop1_27", "gor_shop1_28", "gor_shop1_29", "gor_shop1_30", "gor_shop1_31", "gor_shop1_32", "gor_shop1_33", "gor_shop1_34"}, // msgs
};

EVT_BEGIN(gor_01_shop_setup)
    IF_INT_EQUAL(GSWF(1188), 0)
        IF_INT_EQUAL(GSWF(1191), 0)
            CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list), PTR(&shopper_data), PTR(trade_list))
            ELSE
            CALL(evt_shop_setup, PTR(obj_list), PTR(goods_list2), PTR(&shopper_data), PTR(trade_list))
        END_IF
        ELSE
        CALL(evt_pouch_check_item, ITEM_SUPER_LUIGI1, LW(1))
        CALL(evt_pouch_check_item, ITEM_SUPER_LUIGI2, LW(2))
        CALL(evt_pouch_check_item, ITEM_SUPER_LUIGI3, LW(3))
        CALL(evt_pouch_check_item, ITEM_SUPER_LUIGI4, LW(4))
        CALL(evt_pouch_check_item, ITEM_SUPER_LUIGI5, LW(5))
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(215)
            SET_VALUE(LW(11), 0)
            CASE_LESS(261)
            SET_VALUE(LW(11), 1)
            CASE_LESS(339)
            SET_VALUE(LW(11), 2)
            CASE_LESS(373)
            SET_VALUE(LW(11), 3)
            CASE_LESS(401)
            SET_VALUE(LW(11), 4)
            CASE_DEFAULT
            SET_VALUE(LW(11), 5)
        END_SWITCH
        SET_VALUE(LW(12), 0)
        IF_INT_NOT_EQUAL(LW(1), 0)
            ADD(LW(12), 1)
        END_IF
        IF_INT_NOT_EQUAL(LW(2), 0)
            ADD(LW(12), 1)
        END_IF
        IF_INT_NOT_EQUAL(LW(3), 0)
            ADD(LW(12), 1)
        END_IF
        IF_INT_NOT_EQUAL(LW(4), 0)
            ADD(LW(12), 1)
        END_IF
        IF_INT_NOT_EQUAL(LW(5), 0)
            ADD(LW(12), 1)
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(12), LW(11))
            SET_VALUE(LW(10), PTR(goods_list))
            ELSE
            SWITCH(LW(12))
                CASE_EQUAL(0)
                SET_VALUE(LW(10), PTR(goods_luigi1))
                CASE_EQUAL(1)
                SET_VALUE(LW(10), PTR(goods_luigi2))
                CASE_EQUAL(2)
                SET_VALUE(LW(10), PTR(goods_luigi3))
                CASE_EQUAL(3)
                SET_VALUE(LW(10), PTR(goods_luigi4))
                CASE_EQUAL(4)
                SET_VALUE(LW(10), PTR(goods_luigi5))
            END_SWITCH
        END_IF
        CALL(evt_shop_setup, PTR(obj_list), LW(10), PTR(&shopper_data), PTR(trade_list))
    END_IF
    RETURN
EVT_END()

static KinopioData kino_dt = {
    0, // breakfastItem
    {0}, // breakfastPos
    5, // ryoukin
    {368.0f, 170.0f, -295.0f}, // bed
    {330.0f, 154.0f, -250.0f}, // bedside
    {130.0f, 370.0f, -79.0f, 313.0f, 220.0f, -264.0f}, // cam
    -30.0f, // bedDeg
    {227.0f, 154.0f, -200.0f}, // npcTalk
    /*"宿店員"*/"\x8F\x68\x93\x58\x88\xF5", // npcName
    {"yado_00", "yado_01", "yado_02", "yado_03", "yado_04", "yado_05", "yado_06", "yado_07", "yado_08", "yado_09", "yado_10", "yado_11", "yado_12", "yado_13"}, // msg
    NULL, NULL, // toBedEvt..fromBedEvt
};

EVT_BEGIN(bar_door_anime)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_03_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_get_position, STRING("S_ie_03_f1_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 108, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 108, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_get_position, STRING("S_ie_03_f1_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ie_03_naka"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(GF(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(christine_nakama_init)
    CALL(evt_npc_entry, STRING("マフィア１"), STRING("c_m_buka"))
    CALL(evt_npc_set_tribe, STRING("マフィア１"), STRING("マフィア"))
    CALL(evt_npc_entry, STRING("マフィア２"), STRING("c_m_buka"))
    CALL(evt_npc_set_tribe, STRING("マフィア２"), STRING("マフィア"))
    CALL(evt_npc_entry, STRING("盗賊団１"), STRING("c_monban"))
    CALL(evt_npc_set_tribe, STRING("盗賊団１"), STRING("盗賊団"))
    CALL(evt_npc_entry, STRING("盗賊団２"), STRING("c_monban"))
    CALL(evt_npc_set_tribe, STRING("盗賊団２"), STRING("盗賊団"))
    CALL(evt_npc_set_position, STRING("マフィア１"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("マフィア２"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("盗賊団１"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("盗賊団２"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(christine_nakama)
    SET_VALUE(GF(10), 1)
    SPAWN_EVENT(PTR(maffia_assault))
    CALL(evt_mario_clear_party, 1)
    CALL(evt_cam3d_evt_set, 0, 134, 984, 0, 38, 442, 0, 11)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_pouch_party_join, 1)
        CALL(evt_mario_set_party_pos, 0, 1, 25, 0, 625)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_party_move_pos2, 0, 25, 500, FLOAT(150.0f))
        WAIT_FRAMES(1)
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_dispflag_onoff, 1, 4)
    CALL(evt_mario_set_pos, -25, 0, 625)
    CALL(evt_mario_mov_pos2, -25, 500, FLOAT(150.0f))
    CALL(evt_mario_adjust_dir)
    CALL(evt_mario_dispflag_onoff, 0, 4)
    CALL(evt_npc_set_position, STRING("キノじい"), 160, 0, 500)
    CALL(evt_npc_set_ry, STRING("キノじい"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("キノじい"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("キノじい"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    BEGIN_INLINE_EVENT
        WAIT_MS(60)
        CALL(evt_mario_set_dir, 90, 200, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_034"))
    CALL(evt_cam3d_evt_set, 0, 55, 800, 0, 45, 440, 400, 11)
    WAIT_MS(400)
    CALL(evt_msg_print_party, STRING("mac_0_034_00"))
    CALL(evt_party_move_pos2, 0, -8, 495, FLOAT(120.0f))
    WAIT_MS(100)
    CALL(evt_party_set_pose, 0, STRING("PKR_A_2A"))
    CALL(evt_snd_sfxon, STRING("SFX_STG0_KURI_KISS1"), 0)
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 15)
    ADD(LW(1), 35)
    ADD(LW(2), 10)
    CALL(evt_snd_sfxon, STRING("SFX_STG0_KURI_KISS2"), 0)
    CALL(evt_eff, 0, STRING("kiss"), 1, LW(0), LW(1), LW(2), 45, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_party_move_pos2, 0, 25, 500, FLOAT(120.0f))
    CALL(evt_party_set_dir, 0, 270, 200)
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("mac_0_035"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("mac_0_036"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("mac_0_037"))
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    SET_VALUE(GF(10), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_038"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("mac_0_039"))
    CALL(evt_mario_set_pose, STRING("M_C_3"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 20)
    ADD(LW(1), 20)
    ADD(LW(2), 10)
    CALL(evt_item_entry, STRING("item00"), ITEM_MAGICAL_MAP_LARGE, LW(0), LW(1), LW(2), 16, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_item_set_scale, STRING("item00"), FLOAT(0.5498047f))
    WAIT_MS(500)
    CALL(evt_party_move_pos2, 0, 0, 500, FLOAT(120.0f))
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, STRING("mac_0_040"))
    CALL(evt_item_delete, STRING("item00"))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("mac_0_041"))
    CALL(evt_mario_set_dir, 90, 200, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_party_set_dir, 0, 90, 200)
        WAIT_MS(500)
        CALL(evt_party_move_pos2, 0, 25, 500, FLOAT(120.0f))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_0_042"), 0, STRING("キノじい"))
    SET_VALUE(GF(10), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 25, 55, 800, 25, 45, 440, 800, 11)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("キノじい"), 75, 500, 0, FLOAT(55.0f), 0)
    CALL(evt_msg_print, 0, STRING("mac_0_043"), 0, STRING("キノじい"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_044"), 0, STRING("キノじい"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_045"), 0, STRING("キノじい"))
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 200, 1, -1000)
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_WHILE(GF(11))
    CALL(evt_msg_print, 0, STRING("mac_0_046"), 0, STRING("キノじい"))
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_047"), 0, STRING("キノじい"))
    CALL(evt_mario_set_pose, STRING("M_I_G"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_047_01"), 0, STRING("キノじい"))
    CALL(evt_npc_flag_onoff, 0, STRING("キノじい"), NPC_FLAG_TALK_POSE)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノじい"), 93, -46, 0, FLOAT(120.0f), 0)
        CALL(evt_npc_set_ry, STRING("キノじい"), 90)
        WAIT_MS(500)
        SPAWN_EVENT(PTR(bar_door_anime))
        CALL(evt_npc_move_position, STRING("キノじい"), 74, -25, 0, FLOAT(180.0f), 0)
        WAIT_MS(500)
        CALL(evt_npc_move_position, STRING("キノじい"), 105, -95, 0, FLOAT(180.0f), 0)
        WAIT_WHILE(GF(0))
        CALL(evt_npc_set_position, STRING("キノじい"), 320, 153, -200)
        CALL(evt_npc_set_ry, STRING("キノじい"), 270)
        CALL(evt_npc_flag_onoff, 0, STRING("キノじい"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_get_position, STRING("キノじい"), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_0_047_02"), 0, STRING("キノじい"))
        WAIT_MS(500)
        CALL(evt_npc_get_position, STRING("キノじい"), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(evt_msg_toge, 2, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_0_047_02"), 0, STRING("キノじい"))
        WAIT_MS(1000)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 55, 800, 0, 45, 440, 500, 11)
    WAIT_MS(1000)
    CALL(evt_party_set_dir, 0, 270, 200)
    WAIT_MS(200)
    CALL(evt_msg_print_party, STRING("mac_0_048"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("mac_0_049"))
    CALL(evt_mario_set_pose, STRING("M_I_Z"))
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("mac_0_050"))
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_print_party, STRING("mac_0_051"))
    CALL(evt_cam3d_evt_set, 27, 45, 800, 27, 35, 440, 800, 11)
    WAIT_MS(800)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 1)
    CALL_CHILD(PTR(all_party_lecture))
    CALL(evt_snd_bgmon_f, 768, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR1"))
    CALL(evt_msg_print_party, STRING("mac_0_053"))
    CALL(evt_cam3d_evt_off, 500, 11)
    CALL(evt_mario_dispflag_onoff, 0, 256)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, 7)
    SPAWN_EVENT(PTR(christine_nakama_after))
    RETURN
EVT_END()

EVT_BEGIN(maffia_turn)
    CALL(evt_snd_sfxon, STRING("SFX_STG0_THIEF_FIGHT1"), 0)
    CALL(evt_npc_get_position, STRING("盗賊団１"), LW(0), LW(1), LW(2))
    CALL(maffia_kemuri, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("盗賊団２"), LW(0), LW(1), LW(2))
    CALL(maffia_kemuri, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_STG0_THIEF_FIGHT2"), 0)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 720, 360)
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(10), 360)
            SUBTRACT(LW(10), 360)
        END_IF
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_rotate, STRING("盗賊団１"), 0, LW(10), 0)
        CALL(evt_npc_set_rotate, STRING("盗賊団２"), 0, LW(10), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_rotate, STRING("盗賊団１"), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING("盗賊団２"), 0, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(maffia_assault)
    SET_VALUE(GF(11), 1)
    CALL(evt_npc_set_position, STRING("マフィア１"), 75, 0, -400)
    CALL(evt_npc_set_ry, STRING("マフィア１"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア１"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア１"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("マフィア２"), 95, 0, -390)
    CALL(evt_npc_set_ry, STRING("マフィア２"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア２"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア２"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    WAIT_WHILE(GF(10))
    SET_VALUE(GF(10), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("マフィア２"), 45, -390, 0, FLOAT(300.0f), 0)
        WAIT_MS(1300)
        CALL(evt_npc_move_position, STRING("マフィア２"), 25, -390, 0, FLOAT(300.0f), 0)
        CALL(evt_npc_set_ry, STRING("マフィア２"), 90)
        CALL(evt_npc_move_position, STRING("マフィア２"), 25, -60, 0, FLOAT(300.0f), 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("マフィア１"), 25, -400, 0, FLOAT(300.0f), 0)
    WAIT_MS(1500)
    CALL(evt_npc_move_position, STRING("マフィア１"), 5, -390, 0, FLOAT(300.0f), 0)
    CALL(evt_npc_set_ry, STRING("マフィア１"), 90)
    CALL(evt_npc_move_position, STRING("マフィア１"), 5, -60, 0, FLOAT(300.0f), 0)
    WAIT_WHILE(GF(10))
    SET_VALUE(GF(10), 1)
    CALL(evt_npc_set_position, STRING("盗賊団１"), 225, 0, 10)
    CALL(evt_npc_set_ry, STRING("盗賊団１"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊団１"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊団１"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("盗賊団２"), 255, 0, 30)
    CALL(evt_npc_set_ry, STRING("盗賊団２"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊団２"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("盗賊団２"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    SET_VALUE(GF(12), 1)
    SET_VALUE(GF(13), 1)
    SET_VALUE(GF(14), 1)
    SET_VALUE(GF(15), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("盗賊団１"), 125, 10, 0, FLOAT(100.0f), 0)
        SET_VALUE(GF(12), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("盗賊団２"), 165, 30, 0, FLOAT(100.0f), 0)
        SET_VALUE(GF(13), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("マフィア１"), 5, 15, 0, FLOAT(360.0f), 0)
        CALL(evt_npc_move_position, STRING("マフィア１"), 90, 15, 0, FLOAT(360.0f), 0)
        SET_VALUE(GF(14), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("マフィア２"), 25, 30, 0, FLOAT(360.0f), 0)
        CALL(evt_npc_move_position, STRING("マフィア２"), 200, 35, 0, FLOAT(360.0f), 0)
        CALL(evt_npc_set_ry, STRING("マフィア２"), 270)
        SET_VALUE(GF(15), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(12))
    WAIT_WHILE(GF(13))
    WAIT_WHILE(GF(14))
    WAIT_WHILE(GF(15))
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア１"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 0, STRING("マフィア２"), NPC_FLAG_AUTO_MOVE_ANIM)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("マフィア２"), STRING("A_1"))
        CALL(evt_npc_move_position, STRING("マフィア２"), 180, 15, 0, FLOAT(360.0f), 0)
        CALL(evt_npc_move_position, STRING("マフィア２"), 200, 15, 0, FLOAT(360.0f), 0)
        WAIT_MS(500)
        CALL(evt_npc_move_position, STRING("マフィア２"), 180, 15, 0, FLOAT(360.0f), 0)
        CALL(evt_npc_move_position, STRING("マフィア２"), 200, 15, 0, FLOAT(360.0f), 0)
        WAIT_MS(500)
        CALL(evt_npc_move_position, STRING("マフィア２"), 180, 15, 0, FLOAT(360.0f), 0)
        CALL(evt_npc_move_position, STRING("マフィア２"), 200, 15, 0, FLOAT(360.0f), 0)
        CALL(evt_npc_set_anim, STRING("マフィア２"), STRING("S_1"))
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("マフィア１"), STRING("A_1"))
    CALL(evt_npc_move_position, STRING("マフィア１"), 110, 15, 0, FLOAT(360.0f), 0)
    CALL(evt_npc_set_anim, STRING("盗賊団１"), STRING("D_1"))
    CALL(evt_npc_set_anim, STRING("盗賊団２"), STRING("D_1"))
    SPAWN_EVENT(PTR(maffia_turn))
    CALL(evt_npc_move_position, STRING("マフィア１"), 90, 15, 0, FLOAT(360.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("マフィア１"), 110, 15, 0, FLOAT(360.0f), 0)
    SPAWN_EVENT(PTR(maffia_turn))
    CALL(evt_npc_move_position, STRING("マフィア１"), 90, 15, 0, FLOAT(360.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("マフィア１"), 110, 15, 0, FLOAT(360.0f), 0)
    SPAWN_EVENT(PTR(maffia_turn))
    CALL(evt_npc_move_position, STRING("マフィア１"), 90, 15, 0, FLOAT(360.0f), 0)
    CALL(evt_npc_set_anim, STRING("マフィア１"), STRING("S_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_054"), 0, STRING("盗賊団１"))
    CALL(evt_npc_set_anim, STRING("盗賊団１"), STRING("K_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("盗賊団１"), STRING("Y_1"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア１"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("マフィア２"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_rotate_offset, STRING("盗賊団１"), 0, 0, 0)
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 90, 150)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_rotate, STRING("盗賊団１"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxon, STRING("SFX_STG0_THIEF_DOWN1"), 0)
    SET_VALUE(LW(10), STRING("盗賊団２"))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(0))
    ADD(LW(3), -20)
    CALL(evt_npc_move_position, LW(10), LW(3), LW(2), 0, FLOAT(200.0f), 0)
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("盗賊団２"), STRING("D_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_055"), 0, STRING("盗賊団２"))
    CALL(evt_npc_set_anim, STRING("盗賊団２"), STRING("S_1"))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(10), STRING("マフィア１"))
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), -20)
        CALL(evt_npc_move_position, LW(10), LW(3), LW(2), 0, FLOAT(200.0f), 0)
    END_INLINE_EVENT
    SET_VALUE(LW(10), STRING("マフィア２"))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(0))
    ADD(LW(3), -95)
    CALL(evt_npc_set_ry, LW(10), 90)
    CALL(evt_npc_move_position, LW(10), LW(3), LW(2), 0, FLOAT(200.0f), 0)
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_056"), 0, STRING("マフィア１"))
    WAIT_MS(200)
    CALL(evt_msg_print, 0, STRING("mac_0_057"), 0, STRING("マフィア２"))
    SET_VALUE(GF(14), 1)
    SET_VALUE(GF(15), 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(10), STRING("マフィア１"))
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), -300)
        CALL(evt_npc_set_ry, LW(10), 270)
        CALL(evt_npc_move_position, LW(10), LW(3), LW(2), 0, FLOAT(360.0f), 0)
        CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
        SET_VALUE(GF(14), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(10), STRING("マフィア２"))
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), -300)
        CALL(evt_npc_set_ry, LW(10), 270)
        CALL(evt_npc_move_position, LW(10), LW(3), LW(2), 0, FLOAT(360.0f), 0)
        CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
        SET_VALUE(GF(15), 0)
    END_INLINE_EVENT
    WAIT_WHILE(GF(14))
    WAIT_WHILE(GF(15))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("盗賊団２"), STRING("D_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_058"), 0, STRING("盗賊団２"))
    CALL(evt_npc_set_anim, STRING("盗賊団２"), STRING("S_1"))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 90, 0, 150)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_rotate, STRING("盗賊団１"), LW(0), 0, 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_anim, STRING("盗賊団１"), STRING("K_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_059"), 0, STRING("盗賊団１"))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("盗賊団２"), STRING("D_1"))
    CALL(evt_msg_print, 0, STRING("mac_0_060"), 0, STRING("盗賊団２"))
    CALL(evt_npc_set_anim, STRING("盗賊団２"), STRING("S_1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("mac_0_061"), 0, STRING("盗賊団１"))
    WAIT_MS(200)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(10), STRING("盗賊団１"))
        CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 300)
        CALL(evt_npc_set_ry, LW(10), 90)
        CALL(evt_npc_move_position, LW(10), LW(3), LW(2), 0, FLOAT(120.0f), 0)
        CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
    END_INLINE_EVENT
    SET_VALUE(LW(10), STRING("盗賊団２"))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(0))
    ADD(LW(3), 300)
    CALL(evt_npc_set_ry, LW(10), 90)
    CALL(evt_npc_move_position, LW(10), LW(3), LW(2), 0, FLOAT(120.0f), 0)
    CALL(evt_npc_set_position, LW(10), 0, -1000, 0)
    SET_VALUE(GF(11), 0)
    RETURN
EVT_END()

EVT_BEGIN(christine_nakama_after)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 32)
        RETURN
    END_IF
    LOOP(0)
        IF_INT_EQUAL(GSWF(236), 1)
            RETURN
        END_IF
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(0), 440)
            IF_INT_GREATER_OR_EQUAL(LW(2), -10)
                IF_INT_LESS_OR_EQUAL(LW(2), 190)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_0_061_01"), 0, STRING("party"))
    SET_VALUE(GSWF(1200), 1)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(box_copy)
    LOOP(0)
        CALL(evt_npc_get_position, STRING("ダミー"), LW(0), LW(1), LW(2))
        CALL(evt_mobj_set_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(ultrahammer_check)
    CALL(evt_npc_entry, STRING("ダミー"), STRING("c_kinopio"))
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ダミー"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("ダミー"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_flag_onoff, 1, STRING("ダミー"), NPC_FLAG_NO_DRAW)
    LOOP(0)
        SET_VALUE(LW(1), 0)
        CALL(evt_mario_get_motion, LW(10))
        IF_INT_EQUAL(LW(10), 20)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(0), 315)
            IF_INT_LESS_OR_EQUAL(LW(0), 360)
                IF_INT_GREATER_OR_EQUAL(LW(1), 60)
                    IF_INT_LESS_OR_EQUAL(LW(1), 78)
                        IF_INT_GREATER_OR_EQUAL(LW(2), -40)
                            IF_INT_LESS_OR_EQUAL(LW(2), 0)
                                BREAK_LOOP
                            END_IF
                        END_IF
                    END_IF
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mobj_flag_onoff, 0, STRING("mobj_box00"), MOBJ_FLAG_GRAVITY)
    SPAWN_EVENT(PTR(box_copy))
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TBOX_FALL1"), LW(0), LW(1), LW(2), LW(10))
    CALL(evt_npc_jump_position_nohit, STRING("ダミー"), 275, 0, 75, 500, FLOAT(100.0f))
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    ADD(LW(0), -25)
    ADD(LW(1), -5)
    ADD(LW(2), 30)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    ADD(LW(0), 0)
    ADD(LW(1), -5)
    ADD(LW(2), 30)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    ADD(LW(0), 25)
    ADD(LW(1), -5)
    ADD(LW(2), 30)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 500)
    END_INLINE_EVENT
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TBOX_FALL2"), LW(0), LW(1), LW(2), 0)
    SET_VALUE(GSWF(1196), 1)
    RETURN
EVT_END()

EVT_BEGIN(ultrahammer_get)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(lect_juyoitem_rel_set, PTR(all_juyoitem_lecture_rel))
    CALL(evt_item_entry, STRING("hammer"), ITEM_ULTRA_HAMMER, LW(0), LW(1), LW(2), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("mobj_box00"))
    CALL(evt_item_get_important_item, STRING("hammer"))
    CALL(evt_item_get_item, STRING("hammer"))
    CALL(evt_item_get_item_end_wait, STRING("hammer"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(keijiban_omote_joho)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(keijiban_data_alloc, STRING("kj_omote"))
    CALL(keijiban_joho_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(keijiban_joho_makelabel, 0, LW(1))
        CALL(evt_msg_print, 0, LW(1), 0, 0)
        ELSE
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_osreport, 0, LW(1))
            CALL(evt_msg_print, 1, STRING("<plate>\nゆうこうな　データが\n見つかりませんでした\nメッセージを　とうろく　してください\n<key>\n<scrl_auto>\n<once_stop>\n"), 0, 0)
            CALL(evt_msg_print_add_insert, 1, STRING("げんざいの　イベントばんごうは\n『%s』\nです\n<key>"), LW(1))
            ELSE
            CALL(evt_osreport, 0, LW(1))
            CALL(evt_msg_print, 1, STRING("<plate>\nゆうこうな　データが\nじゅうふくして　みつかりました\nゆうこうな　メッセージは　１つにしてください\n<key>\n<scrl_auto>\n<once_stop>"), 0, 0)
            SET_VALUE(LW(2), 0)
            SET_VALUE(LW(3), LW(0))
            LOOP(LW(0))
                CALL(keijiban_joho_makelabel, LW(2), LW(1))
                CALL(evt_msg_print_add_insert, 1, STRING("『%s』\n<key>\n<scrl_auto>\n<once_stop>\n"), LW(1))
                ADD(LW(2), 1)
            END_LOOP
            CALL(evt_msg_print_add_insert, 1, STRING("いじょう　%dこが\nじゅうふくしている　メッセージです\n<key>"), LW(3))
        END_IF
    END_IF
    CALL(keijiban_data_free)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(keijiban_ura_joho)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(keijiban_data_alloc, STRING("kj_ura"))
    CALL(keijiban_joho_check, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(keijiban_joho_makelabel, 0, LW(1))
        CALL(evt_msg_print, 0, LW(1), 0, 0)
        ELSE
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_osreport, 0, LW(1))
            CALL(evt_msg_print, 1, STRING("<plate>\nゆうこうな　データが\n見つかりませんでした\nメッセージを　とうろく　してください\n<key>\n<scrl_auto>\n<once_stop>\n"), 0, 0)
            CALL(evt_msg_print_add_insert, 1, STRING("げんざいの　イベントばんごうは\n『%s』\nです\n<key>"), LW(1))
            ELSE
            CALL(evt_osreport, 0, LW(1))
            CALL(evt_msg_print, 1, STRING("<plate>\nゆうこうな　データが\nじゅうふくして　みつかりました\nゆうこうな　メッセージは　１つにしてください\n<key>\n<scrl_auto>\n<once_stop>"), 0, 0)
            SET_VALUE(LW(2), 0)
            SET_VALUE(LW(3), LW(0))
            LOOP(LW(0))
                CALL(keijiban_joho_makelabel, LW(2), LW(1))
                CALL(evt_msg_print_add_insert, 1, STRING("『%s』\n<key>\n<scrl_auto>\n<once_stop>\n"), LW(1))
                ADD(LW(2), 1)
            END_LOOP
            CALL(evt_msg_print_add_insert, 1, STRING("いじょう　%dこが\nじゅうふくしている　メッセージです\n<key>"), LW(3))
        END_IF
    END_IF
    CALL(keijiban_data_free)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(gate_disp)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_GREATER(LW(2), -100)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_gai_kabe_k"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_gai_kabe_k_01"), MAP_OBJ_FLAG_HIDDEN)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_gai_kabe_k"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_gai_kabe_k_01"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(gor_goki1_urouro)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    LOOP(0)
        SET_VALUE(LW(0), LW(4))
        SET_VALUE(LW(1), LW(5))
        CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
        CALL(evt_npc_add_dirdist, LW(0), LW(1), LW(3), FLOAT(50.0f))
        CALL(evt_gor_villager_urouro_check_func, LW(0), LW(1), 0, 0, FLOAT(70.0f))
        CALL(evt_sub_get_dir, LW(4), LW(5), LW(0), LW(1), LW(3))
        IF_INT_GREATER(LW(3), 180)
            SET_VALUE(LW(6), 0)
            ELSE
            SET_VALUE(LW(6), 180)
        END_IF
        CALL(evt_goki_sub_intpl_init, 13, LW(4), LW(0), LW(5), LW(1), 50)
        LOOP(50)
            CALL(evt_goki_sub_intpl_get_value)
            SET_VALUE(LW(2), LW(0))
            SET_VALUE(LW(3), LW(1))
            ADD(LW(2), 480)
            ADD(LW(3), -180)
            CALL(evt_mapobj_trans, 1, STRING("S_goki_01"), LW(2), 0, LW(3))
            CALL(evt_mapobj_rotate, 1, STRING("S_goki_01"), 0, LW(6), 0)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(4), LW(0))
        SET_VALUE(LW(5), LW(1))
        CALL(evt_sub_random, 3000, LW(2))
        ADD(LW(2), 500)
        WAIT_MS(LW(2))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(gor_goki2_urouro)
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    LOOP(0)
        SET_VALUE(LW(0), LW(4))
        SET_VALUE(LW(1), LW(5))
        CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
        CALL(evt_npc_add_dirdist, LW(0), LW(1), LW(3), FLOAT(50.0f))
        CALL(evt_gor_villager_urouro_check_func, LW(0), LW(1), 0, 0, FLOAT(65.0f))
        CALL(evt_sub_get_dir, LW(4), LW(5), LW(0), LW(1), LW(3))
        IF_INT_GREATER(LW(3), 180)
            SET_VALUE(LW(6), 0)
            ELSE
            SET_VALUE(LW(6), 180)
        END_IF
        CALL(evt_goki_sub_intpl_init, 13, LW(4), LW(0), LW(5), LW(1), 50)
        LOOP(50)
            CALL(evt_goki_sub_intpl_get_value)
            SET_VALUE(LW(2), LW(0))
            SET_VALUE(LW(3), LW(1))
            ADD(LW(2), -515)
            ADD(LW(3), -250)
            CALL(evt_mapobj_trans, 1, STRING("S_goki_02"), LW(2), 0, LW(3))
            CALL(evt_mapobj_rotate, 1, STRING("S_goki_02"), 0, LW(6), 0)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(4), LW(0))
        SET_VALUE(LW(5), LW(1))
        CALL(evt_sub_random, 3000, LW(2))
        ADD(LW(2), 500)
        WAIT_MS(LW(2))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(koopa_npc_entry)
    CALL(evt_npc_entry, STRING("ババ"), STRING("c_kamek_be"))
    CALL(evt_npc_set_tribe, STRING("ババ"), STRING("カメックババ"))
    CALL(evt_npc_set_position, STRING("ババ"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("ババ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ババ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("ババ"), 2)
    CALL(evt_npc_set_width, STRING("ババ"), 30)
    CALL(evt_npc_set_height, STRING("ババ"), 40)
    RETURN
EVT_END()

EVT_BEGIN(gor_01_koopa_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(delete_npc_regl_evt, STRING("ロテン１"))
    CALL(delete_npc_regl_evt, STRING("クリボー１"))
    CALL(delete_npc_regl_evt, STRING("クリボー２"))
    CALL(delete_npc_regl_evt, STRING("クリボー３"))
    CALL(delete_npc_regl_evt, STRING("チューさん１"))
    CALL(evt_mario_set_pos, 400, 0, 100)
    CALL(evt_npc_set_position, STRING("クリボー１"), 269, 0, 65)
    CALL(evt_npc_set_position, STRING("ロテン１"), 98, 0, 17)
    CALL(evt_npc_set_position, STRING("チューさん１"), 145, 0, 109)
    CALL(evt_npc_set_position, STRING("クリボー２"), 0, -1000, 0)
    CALL(evt_koopa_weary, 1)
    CALL(evt_npc_set_ry, STRING("クリボー１"), 90)
    CALL(evt_npc_set_ry, STRING("ロテン１"), 90)
    CALL(evt_npc_set_ry, STRING("チューさん１"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("クリボー１"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ロテン１"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("チューさん１"), NPC_FLAG_NO_HIT)
    CALL(evt_cam3d_evt_set, 200, 160, 616, 200, 65, 74, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_end_wait)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_msg_print, 0, STRING("kpa_stg6_000"), 0, STRING("クリボー１"))
        CALL(evt_npc_status_onoff, 1, STRING("クリボー１"), 2)
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(0))
        CALL(evt_npc_move_position, STRING("クリボー１"), 187, 65, 0, FLOAT(90.0f), 0)
        CALL(evt_sub_get_stopwatch, LW(0), LW(1))
        CALL(evt_npc_set_ry, STRING("クリボー１"), 90)
        WAIT_MS(300)
        CALL(evt_npc_move_position, STRING("クリボー１"), 142, 65, 0, FLOAT(90.0f), 0)
        CALL(evt_npc_set_ry, STRING("クリボー１"), 90)
        WAIT_MS(500)
        CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_PEAPLE_ESCAPE1"), 142, 0, 65, 0)
        CALL(evt_npc_move_position, STRING("クリボー１"), 142, 185, 0, FLOAT(150.0f), 0)
        CALL(evt_npc_move_position, STRING("クリボー１"), 380, 185, 0, FLOAT(150.0f), 0)
        CALL(evt_npc_flag_onoff, 1, STRING("クリボー１"), NPC_FLAG_NO_DRAW)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_msg_print, 0, STRING("kpa_stg6_001"), 0, STRING("ロテン１"))
        CALL(evt_npc_status_onoff, 1, STRING("ロテン１"), 2)
        CALL(evt_npc_move_position, STRING("ロテン１"), 27, -55, 0, FLOAT(120.0f), 0)
        CALL(evt_npc_move_position, STRING("ロテン１"), 27, -250, 0, FLOAT(120.0f), 0)
        CALL(evt_npc_flag_onoff, 1, STRING("ロテン１"), NPC_FLAG_NO_DRAW)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_msg_print, 0, STRING("kpa_stg6_002"), 0, STRING("チューさん１"))
        CALL(evt_npc_move_position, STRING("チューさん１"), 145, 340, 0, FLOAT(120.0f), 0)
        CALL(evt_npc_flag_onoff, 1, STRING("チューさん１"), NPC_FLAG_NO_DRAW)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1300)
        CALL(evt_npc_get_position, STRING("ロテン１"), LW(0), LW(1), LW(2))
        CALL(evt_npc_get_position, STRING("チューさん１"), LW(3), LW(4), LW(5))
        ADD(LW(0), LW(3))
        ADD(LW(1), LW(4))
        ADD(LW(2), LW(5))
        DIVIDE(LW(0), 2)
        DIVIDE(LW(1), 2)
        DIVIDE(LW(2), 2)
        CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_PEAPLE_ESCAPE1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SATIATED2_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_mov_pos2, 200, 100, FLOAT(60.0f))
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_2"), STRING("KPA_T_4"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_003"), 0, STRING("mario"))
    CALL(evt_npc_set_position, STRING("ババ"), 600, 280, 75)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_004"), 0, STRING("ババ"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_KOOPA_SURPRISED1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_koopa_weary, 0)
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    CALL(evt_mario_set_dir, 90, 500, 0)
    WAIT_MS(700)
    CALL(evt_cam3d_evt_set, -225, 120, 755, 416, 150, -12, 0, 11)
    WAIT_FRAMES(1)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_kamek_kemuri1, STRING("ババ"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_glide_position, STRING("ババ"), 302, 25, 75, 0, FLOAT(80.0f), FLOAT(40.0f), 11, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 255, 164, 587, 255, 120, 341, 2600, 11)
    WAIT_MS(3500)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_005"), 0, STRING("ババ"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("mario"), STRING("KPA_S_1"), STRING("KPA_T_1"))
    CALL(evt_msg_print, 0, STRING("kpa_stg6_006"), 0, STRING("mario"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_007"), 0, STRING("ババ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("kpa_stg6_008"), 0, STRING("mario"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_get_position, STRING("ババ"), LW(0), LW(1), LW(2))
            CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_KAMEBABA_FLY1"), LW(0), LW(1), LW(2), 0)
            WAIT_FRAMES(10)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("ババ"), 8224)
        CALL(evt_npc_kamek_move_position, STRING("ババ"), 600, 100, 10, 0, FLOAT(120.0f), 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, 500, 100, FLOAT(120.0f))
    WAIT_MS(2000)
    CALL(evt_npc_set_position, STRING("ルイージ"), 50, 0, 100)
    CALL(evt_npc_status_onoff, 1, STRING("ルイージ"), 2)
    CALL(evt_npc_move_position, STRING("ルイージ"), 200, 100, 0, 60, 0)
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("ルイージ"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("ルイージ"))
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, 1, STRING(""), STRING("ルイージ"), 1, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    CALL(evt_npc_move_position, STRING("ルイージ"), 50, 100, 0, 60, 0)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, 342)
    CALL(evt_bero_mapchange, STRING("kpa_05"), STRING(""))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

void envSetYamiView(Mtx mtx);

static USER_FUNC(yami_view) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){0.0f, 100.0f, 100.0f + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){0.0f, 0.0f, z});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, (flag & 0xF) << 24);
    return EVT_RETURN_DONE;
}

static USER_FUNC(anim_tevcallback_mobj) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjNameToPtrNoAssert(name);

    if (mobj == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(mobj->poseId, (flag & 0xF) << 24);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(sekai_yami_npc)
    LOOP(0)
        CALL(evt_npc_get_position, LW(0), LW(1), LW(2), LW(3))
        IF_INT_LESS_OR_EQUAL(GW(0), LW(3))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_get_reglid, LW(0), LW(1))
    DELETE_EVENT(LW(1))
    CALL(evt_eff_fukidashi, 1, STRING(""), LW(0), 0, 0, 0, 0, 0, 0, 0, 48)
    WAIT_MS(1000)
    LOOP(0)
        CALL(evt_sub_random, 400, LW(1))
        ADD(LW(1), 100)
        WAIT_MS(LW(1))
        CALL(evt_npc_reverse_ry, LW(0))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, LW(0))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("キノじい"), 0, 0, 403)
    CALL(evt_cam3d_evt_set, 0, 350, 1530, 0, 150, 44, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("キノじい"), 1)
    CALL(anim_tevcallback, STRING("ロテン１"), 1)
    CALL(anim_tevcallback, STRING("クリボー１"), 1)
    CALL(anim_tevcallback, STRING("クリボー２"), 1)
    CALL(anim_tevcallback, STRING("チューさん１"), 1)
    CALL(anim_tevcallback, STRING("ボッタクール"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_box00"), 1)
    SET_VALUE(GW(0), 700)
    SET_VALUE(LW(0), STRING("キノじい"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("ロテン１"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("クリボー１"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("チューさん１"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    SET_VALUE(LW(0), STRING("ボッタクール"))
    SPAWN_EVENT(PTR(sekai_yami_npc))
    CALL(evt_snd_sfxon, STRING("SFX_STG8_WORLD_DARK1"), 0)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 700, -850, 5000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(yami_view, LW(0))
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(1200)
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 255, 64, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_bg_set_color, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("nok_01"), STRING("sekai_yami1"))
    RETURN
EVT_END()

static EVT_BEGIN(minnnanokoe_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ロテン１"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("クリボー１"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("クリボー２"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("クリボー３"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("チューさん１"), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("ボッタクール"), 0, -1000, 0)
    CALL(evt_npc_check_delete, STRING("バッジ店長"))
    CALL(evt_npc_check_delete, STRING("バッジ店員"))
    CALL(evt_npc_check_delete, STRING("店長"))
    CALL(evt_npc_check_delete, STRING("店員"))
    CALL(evt_npc_check_delete, STRING("ナンシー"))
    CALL(evt_npc_check_delete, STRING("キノじい"))
    CALL(evt_npc_check_delete, STRING("マスター"))
    CALL(evt_npc_check_delete, STRING("マルコ"))
    CALL(evt_npc_check_delete, STRING("宿店員"))
    CALL(evt_npc_check_delete, STRING("ロテン２"))
    CALL(evt_npc_check_delete, STRING("チューさん２"))
    CALL(evt_npc_check_delete, STRING("水夫ボム兵"))
    CALL(evt_npc_check_delete, STRING("ルイージ"))
    CALL(evt_npc_check_delete, STRING("盗賊"))
    CALL(evt_npc_check_delete, STRING("キノピオ"))
    CALL(evt_npc_check_delete, STRING("キック２"))
    CALL(evt_cam3d_evt_set, 0, 645, 824, 0, 55, 283, 0, 11)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_box00"), 1)
    CALL(yami_view, -800)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 8, 0, 5, 250, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(wait_mahojin)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_STARSTONE_EFFECT1"), 0)
    WAIT_MS(7000)
    CALL(starstone_entry, 0, -50, 250)
    SET_FLOAT_VALUE(GW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(1), FLOAT(-50.0f))
    SET_FLOAT_VALUE(GW(2), FLOAT(250.0f))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, GW(0), GW(1), GW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(0)
            CALL(starstone_set_ry, LW(0))
            SUBTRACT_FLOAT(LW(0), FLOAT(3.0f))
            IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_VOICE1"))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, -50, 30, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 3000, 2500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(1), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 2500, 3000, 500)
            LABEL(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(1), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(0)
            END_IF
        END_LOOP
        SET_VALUE(GF(1), 1)
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_cam3d_evt_set, 0, 110, 1093, 0, 110, 293, 1000, 11)
    WAIT_MS(1000)
    SET_VALUE(GF(0), 1)
    LOOP(0)
        IF_INT_NOT_EQUAL(GF(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_cam3d_evt_set, 0, -75, 999, 0, 300, 293, 2000, 1)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 25, 800, 2000)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(GW(1), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("nok_01"), STRING("minnnanokoe"))
    RETURN
EVT_END()

EVT_BEGIN(iri_09_init)
    IF_INT_EQUAL(GSW(1420), 9)
        CALL(set_fook_evt, PTR(&shopper_data), PTR(iri_09_tentyo_talk))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_09_tentyo_talk)
    IF_INT_EQUAL(GSWF(5383), 1)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(5405), 0)
        CALL(evt_msg_print, 0, STRING("irai_09_001"), 0, STRING("店長"))
        SET_VALUE(GSWF(5405), 1)
        SET_VALUE(LW(0), -1)
        ELSE
        CALL(evt_msg_print, 0, STRING("irai_09_002"), 0, STRING("店長"))
        CALL(evt_pouch_check_item, ITEM_COURAGE_SHELL, LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL(iri_09_item_tbl_alloc, LW(0))
            LOOP(0)
                CALL(evt_msg_continue)
                CALL(iri_09_item_tbl_make, LW(0), LW(1))
                CALL(evt_win_item_select, 0, LW(1), LW(2), 0)
                IF_INT_EQUAL(LW(2), -1)
                    SET_VALUE(LF(0), 1)
                    BREAK_LOOP
                END_IF
                CALL(evt_msg_print, 0, STRING("irai_09_003"), 0, STRING("店長"))
                CALL(evt_pouch_remove_item, ITEM_COURAGE_SHELL, LW(1))
                ADD(GSW(1469), 1)
                IF_INT_EQUAL(GSW(1469), 5)
                    SET_VALUE(LF(0), 0)
                    BREAK_LOOP
                END_IF
                CALL(evt_pouch_check_item, ITEM_COURAGE_SHELL, LW(0))
                IF_INT_LESS_OR_EQUAL(LW(0), 0)
                    SET_VALUE(LF(0), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            CALL(iri_09_item_tbl_free)
            IF_INT_LESS(GSW(1469), 5)
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("irai_09_006_1"))
                    ELSE
                    CALL(evt_msg_print, 0, STRING("irai_09_006_1"), 0, STRING("店長"))
                END_IF
                ELSE
                CALL(evt_msg_print_add, 0, STRING("irai_09_004"))
                CALL(evt_item_entry, STRING("uk"), ITEM_ULTRA_SHROOM, 0, -1000, 0, 17, -1, 0)
                CALL(evt_item_get_item, STRING("uk"))
                CALL(evt_item_get_item_end_wait, STRING("uk"))
                CALL(evt_msg_print, 0, STRING("irai_09_006"), 0, STRING("店長"))
                SET_VALUE(GSW(1420), 0)
                SET_VALUE(GSWF(5383), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                SET_VALUE(LW(0), -1)
                RETURN
            END_IF
            ELSE
            CALL(evt_msg_print_add, 0, STRING("irai_09_008"))
        END_IF
        SET_VALUE(LW(0), -100)
    END_IF
    RETURN
EVT_END()

s32 iri_16_item_tbl[] = {
    ITEM_ATTACK_FX_B_KEY_ITEM, -1,
};

static EVT_BEGIN(iri_16_init)
    IF_INT_EQUAL(GSW(1420), 16)
        CALL(evt_npc_entry, STRING("チュチュリーナ"), STRING("c_tyutyu"))
        CALL(evt_npc_set_tribe, STRING("チュチュリーナ"), STRING("チュチュリーナ"))
        CALL(evt_npc_set_position, STRING("チュチュリーナ"), -516, 189, -110)
        CALL(evt_npc_set_ry, STRING("チュチュリーナ"), 90)
        CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_change_interrupt, STRING("チュチュリーナ"), 6, PTR(iri_16_tyutyu_talk))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_16_tyutyu_talk)
    IF_INT_EQUAL(GSW(1420), 16)
        CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_CYUCYURINA1"))
        IF_INT_EQUAL(GSWF(5405), 0)
            CALL(evt_msg_print, 0, STRING("irai_16_00"), 0, STRING("チュチュリーナ"))
            SET_VALUE(GSWF(5405), 1)
            ELSE
            IF_INT_EQUAL(GSWF(5407), 0)
                CALL(evt_msg_print, 0, STRING("irai_16_01"), 0, STRING("チュチュリーナ"))
                ELSE
                CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_NO_HIT)
                CALL(evt_party_stop, 0)
                BEGIN_INLINE_EVENT
                    CALL(evt_party_move_pos2, 0, -448, -110, 60)
                    CALL(evt_party_set_dir_npc, 0, STRING("チュチュリーナ"))
                END_INLINE_EVENT
                CALL(evt_mario_mov_pos2, -485, -110, 60)
                CALL(evt_mario_set_dir_npc, STRING("チュチュリーナ"))
                WAIT_MS(500)
                CALL(evt_set_dir_to_target, STRING("チュチュリーナ"), STRING("mario"))
                WAIT_FRAMES(1)
                CALL(evt_npc_wait_pera, STRING("チュチュリーナ"))
                CALL(evt_msg_print, 0, STRING("irai_16_02"), 0, STRING("チュチュリーナ"))
                CALL(evt_win_item_select, 0, PTR(iri_16_item_tbl), LW(0), 0)
                IF_INT_NOT_EQUAL(LW(0), -1)
                    CALL(evt_pouch_remove_item, ITEM_ATTACK_FX_B_KEY_ITEM, LW(0))
                    CALL(evt_msg_print, 0, STRING("irai_16_03"), 0, STRING("チュチュリーナ"))
                    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_C_1"))
                    WAIT_MS(1000)
                    CALL(evt_msg_print, 0, STRING("irai_16_04"), 0, STRING("チュチュリーナ"))
                    CALL(evt_mario_set_pose, STRING("M_N_5B"))
                    WAIT_MS(600)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_msg_print, 0, STRING("irai_16_05"), 0, STRING("チュチュリーナ"))
                    CALL(evt_pouch_get_havebadgecnt, LW(0))
                    IF_INT_LESS(LW(0), 200)
                        CALL(evt_msg_continue)
                        CALL(evt_item_entry, STRING("mb"), ITEM_ATTACK_FX_B, 0, -1000, 0, 17, -1, 0)
                        CALL(evt_item_get_item, STRING("mb"))
                        CALL(evt_item_get_item_end_wait, STRING("mb"))
                        SET_VALUE(GSWF(1201), 1)
                        ELSE
                        CALL(evt_msg_print_add, 0, STRING("irai_16_05_1"))
                        BEGIN_INLINE_EVENT
                            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                            LOOP(0)
                                CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(3), LW(4), LW(5))
                                IF_INT_GREATER(LW(3), LW(0))
                                    BREAK_LOOP
                                END_IF
                                WAIT_FRAMES(1)
                            END_LOOP
                            CALL(evt_mario_set_dir_npc, STRING("チュチュリーナ"))
                        END_INLINE_EVENT
                        CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
                        CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
                        CALL(evt_npc_set_ry, STRING("チュチュリーナ"), 90)
                        WAIT_FRAMES(1)
                        CALL(evt_npc_wait_pera, STRING("チュチュリーナ"))
                        CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), -150, 300, LW(2), 800, 100)
                        WAIT_MS(2000)
                        CALL(evt_npc_set_ry, STRING("チュチュリーナ"), 270)
                        WAIT_FRAMES(1)
                        CALL(evt_npc_wait_pera, STRING("チュチュリーナ"))
                        BEGIN_INLINE_EVENT
                            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                            LOOP(0)
                                CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(3), LW(4), LW(5))
                                IF_INT_LESS(LW(3), LW(0))
                                    BREAK_LOOP
                                END_IF
                                WAIT_FRAMES(1)
                            END_LOOP
                            CALL(evt_mario_set_dir_npc, STRING("チュチュリーナ"))
                        END_INLINE_EVENT
                        CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), LW(0), LW(1), LW(2), 800, 50)
                        CALL(evt_set_dir_to_target, STRING("チュチュリーナ"), STRING("mario"))
                        WAIT_FRAMES(1)
                        CALL(evt_npc_wait_pera, STRING("チュチュリーナ"))
                        CALL(evt_npc_flag_onoff, 0, STRING("チュチュリーナ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
                        CALL(evt_msg_print, 0, STRING("irai_16_05_2"), 0, STRING("チュチュリーナ"))
                        CALL(evt_item_entry, STRING("iri_16"), ITEM_ATTACK_FX_B, -150, 153, -120, 16, GSWF(1201), 0)
                    END_IF
                    CALL(evt_msg_print, 0, STRING("irai_16_06"), 0, STRING("チュチュリーナ"))
                    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_NO_HIT)
                    CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
                    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
                    SUBTRACT(LW(3), 15)
                    CALL(evt_npc_move_position, STRING("チュチュリーナ"), LW(3), LW(5), 0, 40, 0)
                    WAIT_FRAMES(1)
                    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_K_1"))
                    CALL(evt_mario_set_pose, STRING("M_I_O"))
                    BEGIN_INLINE_EVENT
                        CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
                        ADD(LW(1), 30)
                        CALL(evt_eff, 0, STRING("kiss"), 0, LW(0), LW(1), LW(2), -45, 0, 0, 0, 0, 0, 0, 0)
                        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_REQUEST_CHURINA_KISS1"), LW(0), LW(1), LW(2), 0)
                    END_INLINE_EVENT
                    WAIT_MS(400)
                    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
                    WAIT_MS(500)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_npc_move_position, STRING("チュチュリーナ"), LW(0), LW(2), 0, 40, 0)
                    WAIT_MS(1000)
#ifdef VERSION_G8ME01
                    CALL(evt_cam3d_evt_set, -516, 290, 230, -516, 225, -140, 800, 11)
#else
                    CALL(evt_cam3d_evt_set, -525, 290, 230, -525, 225, -140, 800, 11)
#endif
                    WAIT_MS(800)
                    SET_VALUE(LW(8), STRING("チュチュリーナ"))
                    SET_VALUE(LW(9), 7)
#ifdef VERSION_G8ME01
                    CALL_CHILD(all_party_lecture)
#else
                    CALL_CHILD(PTR(all_party_lecture))
#endif
                    SET_VALUE(GSW(1420), 0)
                    SET_VALUE(GSWF(5390), 1)
                    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                    CALL(evt_cam3d_evt_off, 500, 11)
                    CALL(evt_party_run, 0)
                END_IF
                CALL(evt_party_run, 0)
            END_IF
        END_IF
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        RETURN
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(marco_jewel)
    CALL(evt_mario_key_onoff, 0)
    DELETE_EVENT(GW(0))
    CALL(evt_msg_close, STRING("mac_4_004"))
    CALL(evt_msg_print, 0, STRING("mac_4_006"), 0, STRING("マルコ"))
    SPAWN_EVENT_GET_ID(PTR(marco_hitorigoto), GW(0))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_npc_entry)
    CALL(evt_npc_entry, STRING("クリスチーヌ"), STRING("c_pkr"))
    CALL(evt_npc_set_tribe, STRING("クリスチーヌ"), STRING("クリスチーヌ"))
    CALL(evt_npc_set_position, STRING("クリスチーヌ"), 55, 154, -110)
    CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("クリスチーヌ"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_entry, STRING("チュチュリーナ"), STRING("c_tyutyu"))
    CALL(evt_npc_set_tribe, STRING("チュチュリーナ"), STRING("チュチュリーナ"))
    CALL(evt_npc_set_position, STRING("チュチュリーナ"), 0, -1000, 0)
    CALL(evt_npc_set_ry, STRING("チュチュリーナ"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt_sub)
    SET_VALUE(LW(0), PTR(&door_data_03_f2))
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(PTR(evt_door_03_00_f2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("クリスチーヌ"), -50, -110, 0, 60, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_MS(1100)
    SET_VALUE(LW(0), PTR(&door_data_03_f2))
    SPAWN_EVENT(PTR(door_close_se))
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(PTR(evt_door_03_00_f2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_WHILE(GF(0))
    SET_VALUE(LW(0), PTR(&door_data_02_f2))
    SPAWN_EVENT(PTR(door_open_se))
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(PTR(evt_door_02_00_f2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_move_position, STRING("バッジ店長"), -114, -100, 0, 60, 0)
    CALL(evt_set_dir_to_target, STRING("バッジ店長"), STRING("クリスチーヌ"))
    SET_VALUE(LW(0), PTR(&door_data_02_f2))
    SPAWN_EVENT(PTR(door_close_se))
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(PTR(evt_door_02_00_f2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    LOOP(0)
        CALL(evt_npc_set_anim, STRING("バッジ店長"), STRING("CHU_T_1"))
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("バッジ店長"), STRING("CHU_S_1"))
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_T_1"))
        SET_VALUE(GF(15), 0)
        WAIT_MS(1500)
        CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(epilogue_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_CYUCYURINA1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_npc_set_position, STRING("バッジ店長"), -114, 154, -173)
    CALL(evt_npc_flag_onoff, 1, STRING("バッジ店長"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_02_f2_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_unitwin_alloc, 1)
    CALL(badgeShop_getBargainTable, LW(0))
    CALL(evt_unitwin_selltable_setup, PTR(uwnobj_list), LW(0))
    CALL(evt_cam3d_evt_set, -80, 225, 400, -80, 200, -100, 0, 11)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(GF(15), 1)
    SPAWN_EVENT_GET_ID(PTR(epilogue_evt_sub), LW(0))
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("ep_43"), 0, 0)
    WAIT_WHILE(GF(15))
    DELETE_EVENT(LW(0))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("バッジ店長"), STRING("CHU_S_1"))
    CALL(evt_npc_set_anim, STRING("クリスチーヌ"), STRING("PKR_S_1"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_get_position, STRING("バッジ店長"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_dir, STRING("バッジ店長"), LW(3))
    CALL(evt_npc_set_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("チュチュリーナ"), LW(3))
    CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_anim, STRING("バッジ店長"), STRING("CHU_Z_1"))
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_Z_1"))
    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(1), FLOAT(20.0f))
    SET_VALUE(LW(2), 0)
    LOOP(0)
        ADD_FLOAT(LW(0), LW(1))
        IF_INT_GREATER_OR_EQUAL(LW(0), FLOAT(360.0f))
            SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
            ADD(LW(2), 1)
            IF_INT_GREATER_OR_EQUAL(LW(2), 5)
                BREAK_LOOP
            END_IF
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(2), 4)
            SUBTRACT_FLOAT(LW(1), FLOAT(1.0f))
            IF_INT_LESS_OR_EQUAL(LW(1), FLOAT(5.0f))
                SET_FLOAT_VALUE(LW(1), FLOAT(5.0f))
            END_IF
        END_IF
        IF_INT_EQUAL(LW(2), 2)
            IF_INT_EQUAL(LW(0), FLOAT(180.0f))
                CALL(evt_npc_flag_onoff, 1, STRING("バッジ店長"), NPC_FLAG_NO_DRAW)
                CALL(evt_npc_flag_onoff, 0, STRING("チュチュリーナ"), NPC_FLAG_NO_DRAW)
            END_IF
        END_IF
        CALL(evt_npc_set_rotate, STRING("バッジ店長"), 0, LW(0), 0)
        CALL(evt_npc_set_rotate, STRING("チュチュリーナ"), 0, LW(0), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_rotate, STRING("チュチュリーナ"), 0, 0, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("チュチュリーナ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_anim, STRING("バッジ店長"), STRING("CHU_S_1"))
    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_S_1"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 275, 1397, 0, 200, -100, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("チュチュリーナ"), LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(0), -50)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_ry, STRING("クリスチーヌ"), 90)
    END_INLINE_EVENT
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), -84, 302, -167, 700, 130)
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), 140, 354, -216, 700, 70)
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), 290, 354, -216, 700, 70)
    CALL(evt_npc_jump_position_nohit, STRING("チュチュリーナ"), 490, 504, -216, 700, 120)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, 10, 900, 9, 900)
    CALL(evt_bero_mapchange, STRING("pik_04"), STRING(""))
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(dai_disp)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SET_VALUE(LF(0), 0)
        IF_INT_LESS_OR_EQUAL(LW(0), 60)
            IF_INT_GREATER_OR_EQUAL(LW(0), -60)
                IF_INT_LESS_OR_EQUAL(LW(2), -200)
                    IF_INT_GREATER_OR_EQUAL(LW(2), -300)
                        SET_VALUE(LF(0), 1)
                    END_IF
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(LF(0), 0)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_dai"), MAP_OBJ_FLAG_HIDDEN)
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_dai"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif
EVT_BEGIN(gor_01_init_evt)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 341)
        CALL(evt_mario_set_mode, 2)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            SET_VALUE(LF(1), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SET_VALUE(LF(1), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR1"))
    ELSE
        IF_INT_EQUAL(LF(0), 0)
            IF_INT_EQUAL(LF(1), 1)
                CALL(evt_bero_get_entername, LW(0))
                IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
                    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_WITCH"))
                    CALL(evt_snd_envoff, PSND_FADEOUT)
                END_IF
                IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
                    CALL(evt_snd_bgmoff, PSND_FADEOUT)
                    CALL(evt_snd_envoff, PSND_FADEOUT)
                    SET_VALUE(LF(1), 1)
                END_IF
            ELSE
                CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_GOR1"))
                CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR1"))
            END_IF
        ELSE
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_KUPPA1"))
            CALL(evt_snd_bgm_scope, 0, 1)
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR1"))
        END_IF
    END_IF
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 6)
        CALL_CHILD(christine_nakama_init)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 7)
        IF_INT_EQUAL(GSWF(1200), 0)
            SPAWN_EVENT(christine_nakama_after)
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1191), 0)
        IF_INT_EQUAL(GSWF(1189), 1)
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 56)
                SET_VALUE(GSWF(1191), 1)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1191), 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 56)
            SET_VALUE(LW(0), GSWF(1189))
            ADD(LW(0), GSWF(1190))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_sub_load_progresstime, GSW(108), LW(0))
                IF_INT_GREATER_OR_EQUAL(LW(0), 90000)
                    SET_VALUE(GSWF(1191), 1)
                END_IF
            END_IF
        END_IF
    END_IF
    CALL_CHILD(luigi_npcEnt)
    CALL(evt_npc_setup, PTR(npcEnt))
#ifdef VERSION_G8ME01
    CALL(evt_npc_sound_data_set, STRING("マルコ"), STRING("SFX_STG5_MARCO_MOVE1L"), STRING("SFX_STG5_MARCO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
#else
    CALL(evt_npc_sound_data_set, STRING(/*"マルコ"*/"\x83\x7D\x83\x8B\x83\x52"), STRING("SFX_STG5_MARCO_MOVE1L"), STRING("SFX_STG5_MARCO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
#endif
    IF_INT_EQUAL(LF(0), 1)
        CALL_CHILD(koopa_npc_entry)
    END_IF
    IF_INT_NOT_EQUAL(LF(0), 1)
        CALL(evt_mobj_save_blk, STRING("mobj_save"), 185, 60, -55, 0, 0)
    END_IF
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -455, 0, -260, STRING("S_ground"), 125, GSWF(5569))
    CALL(evt_mobj_kururing_floor, STRING("kururin2"), 0, 0, 450, STRING("S_ground"), 125, GSWF(5573))
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 490, 0, 245, 16, GSWF(5570), 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, 550, 0, -150, 16, GSWF(5571), 0)
    CALL(evt_item_entry, STRING("item_02"), ITEM_STAR_PIECE, -565, 162, -160, 16, GSWF(5572), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, 223)
#ifdef VERSION_G8ME01
        CALL(evt_item_entry, STRING("ドクロジュエル"), ITEM_SKULL_GEM, 315, 20, -125, 16, -1, 0)
        CALL(evt_item_flag_on, STRING("ドクロジュエル"), 2)
        CALL(evt_item_status_on, STRING("ドクロジュエル"), 33554432)
#else
        CALL(evt_item_entry, STRING(/*"ドクロジュエル"*/"\x83\x68\x83\x4E\x83\x8D\x83\x57\x83\x85\x83\x47\x83\x8B"), ITEM_SKULL_GEM, 315, 20, -125, 16, -1, 0)
        CALL(evt_item_flag_on, STRING(/*"ドクロジュエル"*/"\x83\x68\x83\x4E\x83\x8D\x83\x57\x83\x85\x83\x47\x83\x8B"), 2)
        CALL(evt_item_status_on, STRING(/*"ドクロジュエル"*/"\x83\x68\x83\x4E\x83\x8D\x83\x57\x83\x85\x83\x47\x83\x8B"), 33554432)
#endif
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_jewel"), 0, PTR(marco_jewel), GW(8))
    END_IF
    IF_INT_EQUAL(GSWF(1188), 0)
        SET_VALUE(LW(0), PTR(&door_data_01))
    ELSE
        SET_VALUE(LW(0), PTR(&door_data_01_01))
    END_IF
    CALL(evt_door_data_copy, LW(0), GW(6))
    SET_VALUE(LW(0), GW(6))
    CALL_CHILD(evt_door_entry)
    SET_VALUE(LW(0), PTR(&door_data_02_f1))
    IF_INT_LESS(GSW_STORY_PROGRESS, 223)
        IF_INT_EQUAL(GSWF(1191), 1)
            SET_VALUE(LW(0), PTR(&door_data_02_f1_01))
        END_IF
    END_IF
    CALL_CHILD(evt_door_entry)
    IF_INT_LESS(GSW_STORY_PROGRESS, 19)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie_02_f2_doa01"), 0, PTR(evt_door_02_f2_junbityu), 0)
    ELSE
        SET_VALUE(LW(0), PTR(&door_data_02_f2))
        CALL_CHILD(evt_door_entry)
    END_IF
    SET_VALUE(LW(0), PTR(&door_data_03_f1))
    CALL_CHILD(evt_door_entry)
    SET_VALUE(LW(0), PTR(&door_data_03_f2))
    CALL_CHILD(evt_door_entry)
    SET_VALUE(LW(0), PTR(&door_data_04))
    CALL_CHILD(evt_door_entry)
    CALL_CHILD(gor_01_shop_setup)
    CALL(evt_kinopio_setup, PTR(&kino_dt))
    CALL(evt_map_playanim, STRING("hae"), 1, 0)
    SPAWN_EVENT(koushukei_dai)
    SPAWN_EVENT(gate_disp)
    SPAWN_EVENT(gor_goki1_urouro)
    SPAWN_EVENT(gor_goki2_urouro)
    IF_INT_EQUAL(LF(1), 0)
        IF_INT_EQUAL(GSW(1420), 1)
            CALL(evt_item_entry, STRING("item_key00"), ITEM_HOUSE_KEY, -280, 153, -100, 0, GSWF(5405), 0)
        END_IF
        CALL_CHILD(borodo2_init)
        SPAWN_EVENT(iri_09_init)
        SPAWN_EVENT(iri_16_init)
        IF_INT_EQUAL(GSWF(5390), 1)
            CALL(evt_item_entry, STRING("iri_16"), ITEM_ATTACK_FX_B, -150, 153, -120, 16, GSWF(1201), 0)
        END_IF
        IF_INT_EQUAL(GSWF(5392), 1)
            CALL(evt_item_entry, STRING("iri_18"), ITEM_HONEY_SHROOM, -440, 30, -80, 16, GSWF(1202), 0)
        END_IF
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_keizi_mae"), 0, PTR(keijiban_omote_joho), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_keizi_ura"), 0, PTR(keijiban_ura_joho), 0)
    CALL(evt_pouch_check_item, ITEM_ULTRA_HAMMER, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        IF_INT_EQUAL(GSWF(1196), 0)
            CALL(evt_mobj_itembox, STRING("mobj_box00"), 340, 110, -20, 1, 0, PTR(ultrahammer_get), GSWF(1195))
            SPAWN_EVENT(ultrahammer_check)
        ELSE
            CALL(evt_mobj_itembox, STRING("mobj_box00"), 275, 0, 75, 1, 0, PTR(ultrahammer_get), GSWF(1195))
        END_IF
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_saku"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_dai"), MAP_OBJ_FLAG_CLIP)
#ifdef VERSION_G8ME01
    SPAWN_EVENT(dai_disp)
#endif
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ie_02_f1A"), MAP_OBJ_FLAG_CLIP)
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(gor_01_koopa_evt)
    END_IF
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            SPAWN_EVENT(sekai_yami_evt)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SPAWN_EVENT(minnnanokoe_evt)
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 402)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(epilogue_npc_entry)
        SPAWN_EVENT(epilogue_evt)
    END_IF
    SPAWN_EVENT(evt_sub_mail_event)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_roten"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_roten"), 1, 0)
    RETURN
EVT_END()

SimpleRecipe R_simple_recipe_table[] = {
    {/*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"あつあつスープ"*/"\x82\xA0\x82\xC2\x82\xA0\x82\xC2\x83\x58\x81\x5B\x83\x76", /*"あつあつスープ"*/"\x82\xA0\x82\xC2\x82\xA0\x82\xC2\x83\x58\x81\x5B\x83\x76"},
    {/*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF"},
    {/*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF"},
    {/*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF"},
    {/*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", "", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF"},
    {/*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", "", /*"あつあつスープ"*/"\x82\xA0\x82\xC2\x82\xA0\x82\xC2\x83\x58\x81\x5B\x83\x76"},
    {/*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"めだまやき"*/"\x82\xDF\x82\xBE\x82\xDC\x82\xE2\x82\xAB", /*"めだまやき"*/"\x82\xDF\x82\xBE\x82\xDC\x82\xE2\x82\xAB"},
    {/*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"ナンシーティー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x65\x83\x42\x81\x5B", /*"ナンシーティー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x65\x83\x42\x81\x5B"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"スパゲティ"*/"\x83\x58\x83\x70\x83\x51\x83\x65\x83\x42", /*"スパゲティ"*/"\x83\x58\x83\x70\x83\x51\x83\x65\x83\x42"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ムースケーキ"*/"\x83\x80\x81\x5B\x83\x58\x83\x50\x81\x5B\x83\x4C", /*"ムースケーキ"*/"\x83\x80\x81\x5B\x83\x58\x83\x50\x81\x5B\x83\x4C"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", "", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"カメカメティー"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x83\x65\x83\x42\x81\x5B", /*"カメカメティー"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x83\x65\x83\x42\x81\x5B"},
    {/*"つくしんぼ"*/"\x82\xC2\x82\xAD\x82\xB5\x82\xF1\x82\xDA", "", /*"あつあつスープ"*/"\x82\xA0\x82\xC2\x82\xA0\x82\xC2\x83\x58\x81\x5B\x83\x76"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E"},
    {/*"ゆきうさぎ"*/"\x82\xE4\x82\xAB\x82\xA4\x82\xB3\x82\xAC", "", /*"あつあつスープ"*/"\x82\xA0\x82\xC2\x82\xA0\x82\xC2\x83\x58\x81\x5B\x83\x76"},
    {/*"どくキノコ"*/"\x82\xC7\x82\xAD\x83\x4C\x83\x6D\x83\x52", "", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF"},
    {/*"たんこぶ"*/"\x82\xBD\x82\xF1\x82\xB1\x82\xD4", "", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
};

CookRecipe R_cookbook_recipe_table[] = {
#ifdef VERSION_G8ME01
    {"キノコ", "かみなりゴロゴロ", "びりびりキノコ"},
    {"キノコ", "かみなりドッカン", "びりびりキノコ"},
    {"キノコ", "ファイアフラワー", "キノコホイルやき"},
    {"キノコ", "びりびりキノコ", "キノコホイルやき"},
    {"キノコ", "ふにゃふにゃくん", "しなびたキノコ"},
    {"スーパーキノコ", "かみなりゴロゴロ", "びりびりキノコ"},
    {"スーパーキノコ", "かみなりドッカン", "びりびりキノコ"},
    {"スーパーキノコ", "ファイアフラワー", "ナンシーディナー"},
    {"スーパーキノコ", "びりびりキノコ", "キノコホイルやき"},
    {"スーパーキノコ", "ふにゃふにゃくん", "しなびたキノコ"},
    {"スーパーキノコ", "キノコ", "キノコホイルやき"},
    {"ウルトラキノコ", "かみなりゴロゴロ", "びりびりキノコ"},
    {"ウルトラキノコ", "かみなりドッカン", "びりびりキノコ"},
    {"ウルトラキノコ", "ファイアフラワー", "ナンシースペシャル"},
    {"ウルトラキノコ", "びりびりキノコ", "キノコステーキ"},
    {"ウルトラキノコ", "ふにゃふにゃくん", "しなびたキノコ"},
    {"ウルトラキノコ", "キノコ", "キノコステーキ"},
    {"ウルトラキノコ", "スーパーキノコ", "キノコステーキ"},
    {"きんきゅうキノコ", "かみなりゴロゴロ", "びりびりキノコ"},
    {"きんきゅうキノコ", "かみなりドッカン", "びりびりキノコ"},
    {"きんきゅうキノコ", "ファイアフラワー", "ナンシーディナー"},
    {"きんきゅうキノコ", "びりびりキノコ", "キノコホイルやき"},
    {"きんきゅうキノコ", "ふにゃふにゃくん", "しなびたキノコ"},
    {"きんきゅうキノコ", "キノコ", "キノコステーキ"},
    {"きんきゅうキノコ", "スーパーキノコ", "キノコステーキ"},
    {"きんきゅうキノコ", "ウルトラキノコ", "キノコステーキ"},
    {"しなびたキノコ", "ファイアフラワー", "キノコいため"},
    {"しなびたキノコ", "びりびりキノコ", "キノコいため"},
    {"しなびたキノコ", "デカデカドリンク", "キノコ"},
    {"しなびたキノコ", "キノコ", "キノコいため"},
    {"しなびたキノコ", "スーパーキノコ", "キノコホイルやき"},
    {"しなびたキノコ", "ウルトラキノコ", "キノコステーキ"},
    {"しなびたキノコ", "きんきゅうキノコ", "キノコステーキ"},
    {"ハニーシロップ", "こおりのいぶき", "アイスキャンディ"},
    {"ハニーシロップ", "びりびりキノコ", "ハニーキノコ"},
    {"ハニーシロップ", "キノコ", "ハニーキノコ"},
    {"ハニーシロップ", "スーパーキノコ", "ハニーキノコＳ"},
    {"ハニーシロップ", "ウルトラキノコ", "ハニーキノコＺ"},
    {"ハニーシロップ", "きんきゅうキノコ", "ハニーキノコＳ"},
    {"メイプルシロップ", "こおりのいぶき", "ナンシーフラッペ"},
    {"メイプルシロップ", "びりびりキノコ", "メイプルキノコ"},
    {"メイプルシロップ", "キノコ", "メイプルキノコ"},
    {"メイプルシロップ", "スーパーキノコ", "メイプルキノコＳ"},
    {"メイプルシロップ", "ウルトラキノコ", "メイプルキノコＺ"},
    {"メイプルシロップ", "きんきゅうキノコ", "メイプルキノコＳ"},
    {"メイプルシロップ", "ハニーシロップ", "フレッシュジュース"},
    {"ローヤルゼリー", "こおりのいぶき", "ナンシーフラッペ"},
    {"ローヤルゼリー", "びりびりキノコ", "ローヤルキノコ"},
    {"ローヤルゼリー", "キノコ", "ローヤルキノコ"},
    {"ローヤルゼリー", "スーパーキノコ", "ローヤルキノコＳ"},
    {"ローヤルゼリー", "ウルトラキノコ", "ローヤルキノコＺ"},
    {"ローヤルゼリー", "きんきゅうキノコ", "ローヤルキノコＳ"},
    {"ローヤルゼリー", "ハニーシロップ", "フレッシュジュース"},
    {"ローヤルゼリー", "メイプルシロップ", "ナンシーティー"},
    {"じわじわキノコ", "かみなりゴロゴロ", "びりびりキノコ"},
    {"じわじわキノコ", "かみなりドッカン", "びりびりキノコ"},
    {"じわじわキノコ", "キラキラおとし", "ナンシーディナー"},
    {"じわじわキノコ", "ファイアフラワー", "ナンシーディナー"},
    {"じわじわキノコ", "びりびりキノコ", "キノコホイルやき"},
    {"じわじわキノコ", "キノコ", "キノコホイルやき"},
    {"じわじわキノコ", "スーパーキノコ", "ナンシーディナー"},
    {"じわじわキノコ", "ウルトラキノコ", "ナンシースペシャル"},
    {"じわじわキノコ", "きんきゅうキノコ", "ナンシーディナー"},
    {"じわじわキノコ", "ハニーシロップ", "ハニーキノコ"},
    {"じわじわキノコ", "メイプルシロップ", "メイプルキノコ"},
    {"じわじわキノコ", "ローヤルゼリー", "ローヤルキノコＳ"},
    {"じわじわシロップ", "キノコ", "キノコホイルやき"},
    {"じわじわシロップ", "スーパーキノコ", "ナンシーディナー"},
    {"じわじわシロップ", "ウルトラキノコ", "ナンシースペシャル"},
    {"じわじわシロップ", "きんきゅうキノコ", "ナンシーディナー"},
    {"じわじわシロップ", "ハニーシロップ", "フレッシュジュース"},
    {"じわじわシロップ", "メイプルシロップ", "フレッシュジュース"},
    {"じわじわシロップ", "ローヤルゼリー", "フレッシュジュース"},
    {"じわじわシロップ", "じわじわキノコ", "じわじわキノコ"},
    {"イレカエール", "かみなりゴロゴロ", "ゆらゆらじしん"},
    {"イレカエール", "かみなりドッカン", "キラキラおとし"},
    {"イレカエール", "キラキラおとし", "かみなりドッカン"},
    {"イレカエール", "こおりのいぶき", "ファイアフラワー"},
    {"イレカエール", "ファイアフラワー", "こおりのいぶき"},
    {"イレカエール", "ゆらゆらじしん", "かみなりゴロゴロ"},
    {"イレカエール", "テレサのふく", "てきヨケール"},
    {"イレカエール", "びりびりキノコ", "スーパーキノコ"},
    {"イレカエール", "てきヨケール", "テレサのふく"},
    {"イレカエール", "まどわせのこな", "しかえしのこな"},
    {"イレカエール", "しかえしのこな", "まどわせのこな"},
    {"イレカエール", "ねむれよいこよ", "スッキリドリンク"},
    {"イレカエール", "デカデカドリンク", "ミニミニくん"},
    {"イレカエール", "カチカチコウラ", "ふにゃふにゃくん"},
    {"イレカエール", "ミニミニくん", "デカデカドリンク"},
    {"イレカエール", "ふにゃふにゃくん", "カチカチコウラ"},
    {"イレカエール", "キノコ", "ハニーシロップ"},
    {"イレカエール", "スーパーキノコ", "メイプルシロップ"},
    {"イレカエール", "ウルトラキノコ", "ローヤルゼリー"},
    {"イレカエール", "きんきゅうキノコ", "スーパーキノコ"},
    {"イレカエール", "しなびたキノコ", "キノコ"},
    {"イレカエール", "スッキリドリンク", "ねむれよいこよ"},
    {"イレカエール", "ハニーシロップ", "キノコ"},
    {"イレカエール", "メイプルシロップ", "スーパーキノコ"},
    {"イレカエール", "ローヤルゼリー", "ウルトラキノコ"},
    {"イレカエール", "じわじわキノコ", "どくキノコ"},
    {"イレカエール", "じわじわシロップ", "ハニーシロップ"},
    {"ドライフラワー", "ファイアフラワー", "あつあつスープ"},
    {"ドライフラワー", "びりびりキノコ", "かみなりドッカン"},
    {"ドライフラワー", "キノコ", "うちゅうしょく"},
    {"ドライフラワー", "スーパーキノコ", "うちゅうしょく"},
    {"ドライフラワー", "ウルトラキノコ", "うちゅうしょく"},
    {"ドライフラワー", "きんきゅうキノコ", "うちゅうしょく"},
    {"ドライフラワー", "しなびたキノコ", "うちゅうしょく"},
    {"ドライフラワー", "じわじわキノコ", "うちゅうしょく"},
    {"ドライフラワー", "ホットドッグ", "うちゅうしょく"},
    {"ふしぎなタマゴ", "ファイアフラワー", "バクハツタマゴ"},
    {"ふしぎなタマゴ", "キノコ", "キセツのオムレツ"},
    {"ふしぎなタマゴ", "スーパーキノコ", "キセツのオムレツ"},
    {"ふしぎなタマゴ", "ウルトラキノコ", "キセツのオムレツ"},
    {"ふしぎなタマゴ", "きんきゅうキノコ", "キセツのオムレツ"},
    {"ふしぎなタマゴ", "ドライフラワー", "うちゅうしょく"},
    {"おうごんはっぱ", "こおりのいぶき", "ゆきうさぎ"},
    {"おうごんはっぱ", "キノコ", "キノコいため"},
    {"おうごんはっぱ", "スーパーキノコ", "キノコホイルやき"},
    {"おうごんはっぱ", "ウルトラキノコ", "キノコステーキ"},
    {"おうごんはっぱ", "きんきゅうキノコ", "キノコステーキ"},
    {"おうごんはっぱ", "じわじわキノコ", "こうちゃキノコ"},
    {"おうごんはっぱ", "ドライフラワー", "うちゅうしょく"},
    {"トロピコマンゴー", "びりびりキノコ", "ナンシーディナー"},
    {"トロピコマンゴー", "キノコ", "ナンシーディナー"},
    {"トロピコマンゴー", "スーパーキノコ", "ナンシーディナー"},
    {"トロピコマンゴー", "ウルトラキノコ", "ナンシーディナー"},
    {"トロピコマンゴー", "ハニーシロップ", "フルーツパフェ"},
    {"トロピコマンゴー", "メイプルシロップ", "フルーツパフェ"},
    {"トロピコマンゴー", "ローヤルゼリー", "フルーツパフェ"},
    {"トロピコマンゴー", "じわじわシロップ", "フルーツパフェ"},
    {"トロピコマンゴー", "ドライフラワー", "うちゅうしょく"},
    {"生パスタ", "びりびりキノコ", "ナンシーディナー"},
    {"生パスタ", "キノコ", "ナンシースペシャル"},
    {"生パスタ", "スーパーキノコ", "ナンシースペシャル"},
    {"生パスタ", "ウルトラキノコ", "ナンシーデラックス"},
    {"生パスタ", "きんきゅうキノコ", "ナンシースペシャル"},
    {"生パスタ", "しなびたキノコ", "ナンシースペシャル"},
    {"生パスタ", "ドライフラワー", "うちゅうしょく"},
    {"生パスタ", "ふしぎなタマゴ", "ナンシーディナー"},
    {"おかしのもと", "ファイアフラワー", "ファイアキャンディ"},
    {"おかしのもと", "びりびりキノコ", "びりびりキャンディ"},
    {"おかしのもと", "まどわせのこな", "メロメロケーキ"},
    {"おかしのもと", "キノコ", "キノコケーキ"},
    {"おかしのもと", "スーパーキノコ", "キノコケーキ"},
    {"おかしのもと", "ウルトラキノコ", "キノコクレープ"},
    {"おかしのもと", "きんきゅうキノコ", "キノコケーキ"},
    {"おかしのもと", "ハニーシロップ", "ハニーあめ"},
    {"おかしのもと", "メイプルシロップ", "ナンシークッキー"},
    {"おかしのもと", "ローヤルゼリー", "ローヤルキャンディ"},
    {"おかしのもと", "じわじわキノコ", "キノコケーキ"},
    {"おかしのもと", "じわじわシロップ", "ナンシークッキー"},
    {"おかしのもと", "ドライフラワー", "うちゅうしょく"},
    {"おかしのもと", "ふしぎなタマゴ", "ナンシークッキー"},
    {"おかしのもと", "トロピコマンゴー", "マンゴープディング"},
    {"レッド・カラリン", "ファイアフラワー", "あつあつスープ"},
    {"レッド・カラリン", "しなびたキノコ", "キノコ"},
    {"レッド・カラリン", "スッキリドリンク", "すみぢる"},
    {"レッド・カラリン", "ハニーシロップ", "スッキリドリンク"},
    {"レッド・カラリン", "メイプルシロップ", "スッキリドリンク"},
    {"レッド・カラリン", "ローヤルゼリー", "スッキリドリンク"},
    {"レッド・カラリン", "生パスタ", "カラリーナパスタ"},
    {"レッド・カラリン", "おかしのもと", "ファイアキャンディ"},
    {"カメカメはっぱ", "キノコ", "キノコいため"},
    {"カメカメはっぱ", "スーパーキノコ", "キノコホイルやき"},
    {"カメカメはっぱ", "ウルトラキノコ", "キノコステーキ"},
    {"カメカメはっぱ", "きんきゅうキノコ", "キノコステーキ"},
    {"カメカメはっぱ", "ハニーシロップ", "フレッシュジュース"},
    {"カメカメはっぱ", "メイプルシロップ", "フレッシュジュース"},
    {"カメカメはっぱ", "ローヤルゼリー", "フレッシュジュース"},
    {"カメカメはっぱ", "じわじわキノコ", "こうちゃキノコ"},
    {"カメカメはっぱ", "じわじわシロップ", "フレッシュジュース"},
    {"カメカメはっぱ", "ドライフラワー", "うちゅうしょく"},
    {"カメカメはっぱ", "おうごんはっぱ", "ヘルスィーサラダ"},
    {"カメカメはっぱ", "トロピコマンゴー", "カメまん"},
    {"カメカメはっぱ", "生パスタ", "カメスパ"},
    {"カメカメはっぱ", "レッド・カラリン", "すみぢる"},
    {"つくしんぼ", "キノコ", "ナンシーディナー"},
    {"つくしんぼ", "スーパーキノコ", "ナンシーディナー"},
    {"つくしんぼ", "ウルトラキノコ", "ナンシースペシャル"},
    {"つくしんぼ", "きんきゅうキノコ", "ナンシーディナー"},
    {"つくしんぼ", "ドライフラワー", "うちゅうしょく"},
    {"つくしんぼ", "ふしぎなタマゴ", "キセツのオムレツ"},
    {"つくしんぼ", "カメカメはっぱ", "ヘルスィーサラダ"},
    {"ピチピーチ", "びりびりキノコ", "ナンシーディナー"},
    {"ピチピーチ", "まどわせのこな", "メロメロケーキ"},
    {"ピチピーチ", "キノコ", "ナンシーディナー"},
    {"ピチピーチ", "スーパーキノコ", "ナンシーディナー"},
    {"ピチピーチ", "ウルトラキノコ", "ナンシースペシャル"},
    {"ピチピーチ", "きんきゅうキノコ", "ナンシーディナー"},
    {"ピチピーチ", "しなびたキノコ", "キノコ"},
    {"ピチピーチ", "ハニーシロップ", "フルーツパフェ"},
    {"ピチピーチ", "メイプルシロップ", "フルーツパフェ"},
    {"ピチピーチ", "ローヤルゼリー", "フルーツパフェ"},
    {"ピチピーチ", "ドライフラワー", "うちゅうしょく"},
    {"ピチピーチ", "トロピコマンゴー", "フルーツパフェ"},
    {"ピチピーチ", "おかしのもと", "ピーチタルト"},
    {"ピチピーチ", "レッド・カラリン", "スッキリドリンク"},
    {"ヤシの実", "ファイアフラワー", "ココナッツボム"},
    {"ヤシの実", "ドライフラワー", "うちゅうしょく"},
    {"ヤシの実", "トロピコマンゴー", "フレッシュジュース"},
    {"ヤシの実", "生パスタ", "ナンシーディナー"},
    {"ヤシの実", "おかしのもと", "ココナツキャンディ"},
    {"ヤシの実", "カメカメはっぱ", "フレッシュジュース"},
    {"ヤシの実", "ピチピーチ", "フレッシュジュース"},
    {"キノコいため", "キラキラおとし", "ほしふるディナー"},
    {"キノコいため", "ドライフラワー", "うちゅうしょく"},
    {"キノコホイルやき", "キラキラおとし", "ほしふるディナー"},
    {"キノコホイルやき", "ドライフラワー", "うちゅうしょく"},
    {"キノコステーキ", "キラキラおとし", "ほしふるディナー"},
    {"キノコステーキ", "ドライフラワー", "うちゅうしょく"},
    {"しっぱいりょうり", "ドライフラワー", "うちゅうしょく"},
    {"ハニーキノコ", "ドライフラワー", "うちゅうしょく"},
    {"メイプルキノコ", "ドライフラワー", "うちゅうしょく"},
    {"ローヤルキノコ", "ドライフラワー", "うちゅうしょく"},
    {"ハニーキノコＳ", "ドライフラワー", "うちゅうしょく"},
    {"メイプルキノコＳ", "ドライフラワー", "うちゅうしょく"},
    {"ローヤルキノコＳ", "ドライフラワー", "うちゅうしょく"},
    {"ハニーキノコＺ", "ドライフラワー", "うちゅうしょく"},
    {"メイプルキノコＺ", "ドライフラワー", "うちゅうしょく"},
    {"ローヤルキノコＺ", "ドライフラワー", "うちゅうしょく"},
    {"ナンシーディナー", "カチカチコウラ", "カチカチりょうり"},
    {"ナンシーディナー", "ドライフラワー", "うちゅうしょく"},
    {"ナンシースペシャル", "カチカチコウラ", "カチカチりょうり"},
    {"ナンシースペシャル", "ドライフラワー", "うちゅうしょく"},
    {"ナンシーデラックス", "カチカチコウラ", "カチカチりょうり"},
    {"ナンシーデラックス", "ドライフラワー", "うちゅうしょく"},
    {"ナンシーダイナマイ", "ドライフラワー", "バクハツタマゴ"},
    {"ナンシーティー", "レッド・カラリン", "すみぢる"},
    {"ゆきうさぎ", "あつあつスープ", "いつまでもふたりで"},
    {"キノコケーキ", "ドライフラワー", "うちゅうしょく"},
    {"キノコクレープ", "ドライフラワー", "うちゅうしょく"},
    {"ムースケーキ", "ドライフラワー", "うちゅうしょく"},
    {"フルーツパフェ", "ドライフラワー", "うちゅうしょく"},
    {"バクハツタマゴ", "ドライフラワー", "うちゅうしょく"},
    {"イカスミパスタ", "ドライフラワー", "うちゅうしょく"},
    {"スパゲティ", "ドライフラワー", "うちゅうしょく"},
    {"スパゲティ", "レッド・カラリン", "カラリーナパスタ"},
    {"スパゲティ", "カメカメはっぱ", "カメスパ"},
    {"こうちゃキノコ", "レッド・カラリン", "すみぢる"},
    {"どくキノコ", "イレカエール", "キノコ"},
    {"どくキノコ", "ドライフラワー", "うちゅうしょく"},
    {"どくキノコ", "おうごんはっぱ", "こうちゃキノコ"},
    {"どくキノコ", "カメカメはっぱ", "こうちゃキノコ"},
    {"ショコラケーキ", "ドライフラワー", "うちゅうしょく"},
    {"マンゴープディング", "ドライフラワー", "うちゅうしょく"},
    {"マンゴープディング", "ふしぎなタマゴ", "はつこいプディング"},
    {"はつこいプディング", "ドライフラワー", "うちゅうしょく"},
    {"ほしふるディナー", "ドライフラワー", "うちゅうしょく"},
    {"ほしふるディナー", "フルーツパフェ", "ナンシーディナー"},
    {"しれんのナベ", "ドライフラワー", "どくキノコ"},
    {"いつまでもふたりで", "ドライフラワー", "うちゅうしょく"},
    {"いつまでもふたりで", "どくキノコ", "しれんのナベ"},
    {"すみぢる", "じわじわキノコ", "どくキノコ"},
    {"すみぢる", "生パスタ", "イカスミパスタ"},
    {"すみぢる", "おかしのもと", "ショコラケーキ"},
    {"すみぢる", "ムースケーキ", "ショコラケーキ"},
    {"すみぢる", "スパゲティ", "イカスミパスタ"},
    {"キセツのオムレツ", "ドライフラワー", "うちゅうしょく"},
    {"カメカメティー", "レッド・カラリン", "すみぢる"},
    {"カメスパ", "ドライフラワー", "うちゅうしょく"},
    {"カメスパ", "レッド・カラリン", "カラリーナパスタ"},
    {"カメスパ", "すみぢる", "イカスミパスタ"},
    {"カラリーナパスタ", "ドライフラワー", "うちゅうしょく"},
    {"カラリーナパスタ", "ヤシの実", "ナンシーディナー"},
    {"カラリーナパスタ", "すみぢる", "イカスミパスタ"},
    {"メロメロケーキ", "ドライフラワー", "うちゅうしょく"},
    {"ピーチタルト", "ドライフラワー", "うちゅうしょく"},
    {"びりびりキャンディ", "ドライフラワー", "うちゅうしょく"},
    {"ファイアキャンディ", "ドライフラワー", "うちゅうしょく"},
    {"ハニーあめ", "ドライフラワー", "うちゅうしょく"},
    {"ココナツキャンディ", "ドライフラワー", "うちゅうしょく"},
    {"ローヤルキャンディ", "ドライフラワー", "うちゅうしょく"},
    {"ナンシークッキー", "ドライフラワー", "うちゅうしょく"},
    {"ヘルスィーサラダ", "ドライフラワー", "うちゅうしょく"},
    {"ヘルスィーサラダ", "生パスタ", "ナンシーディナー"},
    {"ヘルスィーサラダ", "キノコいため", "ナンシーディナー"},
    {"ヘルスィーサラダ", "キノコホイルやき", "ナンシースペシャル"},
    {"ヘルスィーサラダ", "キノコステーキ", "ナンシーデラックス"},
    {"ヘルスィーサラダ", "イカスミパスタ", "ナンシースペシャル"},
    {"ヘルスィーサラダ", "スパゲティ", "ナンシーディナー"},
    {"ヘルスィーサラダ", "カメスパ", "ナンシーディナー"},
    {"ヘルスィーサラダ", "カラリーナパスタ", "ナンシースペシャル"},
    {"カメまん", "ドライフラワー", "うちゅうしょく"},
    {"フレッシュジュース", "レッド・カラリン", "すみぢる"},
    {"たんこぶ", "ドライフラワー", "うちゅうしょく"},
    {"たんこぶ", "おうごんはっぱ", "ナンシーデラックス"},
    {"きんかい", "イレカエール", "キノコ"},
    {"きんかい×３", "イレカエール", "きんかい"},
    {"ココナッツボム", "ドライフラワー", "ヤシの実"},
    {"ココナッツボム", "バクハツタマゴ", "ナンシーダイナマイ"},
    {"カチカチりょうり", "ドライフラワー", "カチカチコウラ"},
#else
    {/*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93", /*"かみなりゴロゴロ"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x53\x83\x8D\x83\x53\x83\x8D", /*"しれんのナベ"*/"\x82\xB5\x82\xEA\x82\xF1\x82\xCC\x83\x69\x83\x78"},
    {/*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"かみなりゴロゴロ"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x53\x83\x8D\x83\x53\x83\x8D", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"ふにゃふにゃくん"*/"\x82\xD3\x82\xC9\x82\xE1\x82\xD3\x82\xC9\x82\xE1\x82\xAD\x82\xF1", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52"},
    {/*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"かみなりゴロゴロ"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x53\x83\x8D\x83\x53\x83\x8D", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ふにゃふにゃくん"*/"\x82\xD3\x82\xC9\x82\xE1\x82\xD3\x82\xC9\x82\xE1\x82\xAD\x82\xF1", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52"},
    {/*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"かみなりゴロゴロ"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x53\x83\x8D\x83\x53\x83\x8D", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
    {/*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ふにゃふにゃくん"*/"\x82\xD3\x82\xC9\x82\xE1\x82\xD3\x82\xC9\x82\xE1\x82\xAD\x82\xF1", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52"},
    {/*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"かみなりゴロゴロ"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x53\x83\x8D\x83\x53\x83\x8D", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ふにゃふにゃくん"*/"\x82\xD3\x82\xC9\x82\xE1\x82\xD3\x82\xC9\x82\xE1\x82\xAD\x82\xF1", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF"},
    {/*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF"},
    {/*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"デカデカドリンク"*/"\x83\x66\x83\x4A\x83\x66\x83\x4A\x83\x68\x83\x8A\x83\x93\x83\x4E", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52"},
    {/*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF"},
    {/*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C"},
    {/*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"こおりのいぶき"*/"\x82\xB1\x82\xA8\x82\xE8\x82\xCC\x82\xA2\x82\xD4\x82\xAB", /*"アイスキャンディ"*/"\x83\x41\x83\x43\x83\x58\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42"},
    {/*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"ハニーキノコ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52"},
    {/*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"ハニーキノコ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52"},
    {/*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ハニーキノコＳ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52\x82\x72"},
    {/*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ハニーキノコＺ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52\x82\x79"},
    {/*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ハニーキノコＳ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52\x82\x72"},
    {/*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"こおりのいぶき"*/"\x82\xB1\x82\xA8\x82\xE8\x82\xCC\x82\xA2\x82\xD4\x82\xAB", /*"ナンシーフラッペ"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x74\x83\x89\x83\x62\x83\x79"},
    {/*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"メイプルキノコ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52"},
    {/*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"メイプルキノコ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52"},
    {/*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"メイプルキノコＳ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x72"},
    {/*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"メイプルキノコＺ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x79"},
    {/*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"メイプルキノコＳ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x72"},
    {/*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"こおりのいぶき"*/"\x82\xB1\x82\xA8\x82\xE8\x82\xCC\x82\xA2\x82\xD4\x82\xAB", /*"ナンシーフラッペ"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x74\x83\x89\x83\x62\x83\x79"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"ローヤルキノコ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x6D\x83\x52"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"ローヤルキノコ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x6D\x83\x52"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ローヤルキノコＳ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x72"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ローヤルキノコＺ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x79"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ローヤルキノコＳ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x72"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ナンシーティー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x65\x83\x42\x81\x5B"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"かみなりゴロゴロ"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x53\x83\x8D\x83\x53\x83\x8D", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"キラキラおとし"*/"\x83\x4C\x83\x89\x83\x4C\x83\x89\x82\xA8\x82\xC6\x82\xB5", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ハニーキノコ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"メイプルキノコ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52"},
    {/*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"メイプルキノコＳ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x72"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"かみなりゴロゴロ"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x53\x83\x8D\x83\x53\x83\x8D", /*"ゆらゆらじしん"*/"\x82\xE4\x82\xE7\x82\xE4\x82\xE7\x82\xB6\x82\xB5\x82\xF1"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93", /*"キラキラおとし"*/"\x83\x4C\x83\x89\x83\x4C\x83\x89\x82\xA8\x82\xC6\x82\xB5"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"キラキラおとし"*/"\x83\x4C\x83\x89\x83\x4C\x83\x89\x82\xA8\x82\xC6\x82\xB5", /*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"こおりのいぶき"*/"\x82\xB1\x82\xA8\x82\xE8\x82\xCC\x82\xA2\x82\xD4\x82\xAB", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"こおりのいぶき"*/"\x82\xB1\x82\xA8\x82\xE8\x82\xCC\x82\xA2\x82\xD4\x82\xAB"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ゆらゆらじしん"*/"\x82\xE4\x82\xE7\x82\xE4\x82\xE7\x82\xB6\x82\xB5\x82\xF1", /*"かみなりゴロゴロ"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x53\x83\x8D\x83\x53\x83\x8D"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"テレサのふく"*/"\x83\x65\x83\x8C\x83\x54\x82\xCC\x82\xD3\x82\xAD", /*"てきヨケール"*/"\x82\xC4\x82\xAB\x83\x88\x83\x50\x81\x5B\x83\x8B"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"てきヨケール"*/"\x82\xC4\x82\xAB\x83\x88\x83\x50\x81\x5B\x83\x8B", /*"テレサのふく"*/"\x83\x65\x83\x8C\x83\x54\x82\xCC\x82\xD3\x82\xAD"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"まどわせのこな"*/"\x82\xDC\x82\xC7\x82\xED\x82\xB9\x82\xCC\x82\xB1\x82\xC8", /*"しかえしのこな"*/"\x82\xB5\x82\xA9\x82\xA6\x82\xB5\x82\xCC\x82\xB1\x82\xC8"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"しかえしのこな"*/"\x82\xB5\x82\xA9\x82\xA6\x82\xB5\x82\xCC\x82\xB1\x82\xC8", /*"まどわせのこな"*/"\x82\xDC\x82\xC7\x82\xED\x82\xB9\x82\xCC\x82\xB1\x82\xC8"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ねむれよいこよ"*/"\x82\xCB\x82\xDE\x82\xEA\x82\xE6\x82\xA2\x82\xB1\x82\xE6", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"デカデカドリンク"*/"\x83\x66\x83\x4A\x83\x66\x83\x4A\x83\x68\x83\x8A\x83\x93\x83\x4E", /*"ミニミニくん"*/"\x83\x7E\x83\x6A\x83\x7E\x83\x6A\x82\xAD\x82\xF1"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"カチカチコウラ"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x83\x52\x83\x45\x83\x89", /*"ふにゃふにゃくん"*/"\x82\xD3\x82\xC9\x82\xE1\x82\xD3\x82\xC9\x82\xE1\x82\xAD\x82\xF1"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ミニミニくん"*/"\x83\x7E\x83\x6A\x83\x7E\x83\x6A\x82\xAD\x82\xF1", /*"デカデカドリンク"*/"\x83\x66\x83\x4A\x83\x66\x83\x4A\x83\x68\x83\x8A\x83\x93\x83\x4E"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ふにゃふにゃくん"*/"\x82\xD3\x82\xC9\x82\xE1\x82\xD3\x82\xC9\x82\xE1\x82\xAD\x82\xF1", /*"カチカチコウラ"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x83\x52\x83\x45\x83\x89"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E", /*"ねむれよいこよ"*/"\x82\xCB\x82\xDE\x82\xEA\x82\xE6\x82\xA2\x82\xB1\x82\xE6"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"どくキノコ"*/"\x82\xC7\x82\xAD\x83\x4C\x83\x6D\x83\x52"},
    {/*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"あつあつスープ"*/"\x82\xA0\x82\xC2\x82\xA0\x82\xC2\x83\x58\x81\x5B\x83\x76"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"かみなりドッカン"*/"\x82\xA9\x82\xDD\x82\xC8\x82\xE8\x83\x68\x83\x62\x83\x4A\x83\x93"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ホットドッグ"*/"\x83\x7A\x83\x62\x83\x67\x83\x68\x83\x62\x83\x4F", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"バクハツタマゴ"*/"\x83\x6F\x83\x4E\x83\x6E\x83\x63\x83\x5E\x83\x7D\x83\x53"},
    {/*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キセツのオムレツ"*/"\x83\x4C\x83\x5A\x83\x63\x82\xCC\x83\x49\x83\x80\x83\x8C\x83\x63"},
    {/*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"キセツのオムレツ"*/"\x83\x4C\x83\x5A\x83\x63\x82\xCC\x83\x49\x83\x80\x83\x8C\x83\x63"},
    {/*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"キセツのオムレツ"*/"\x83\x4C\x83\x5A\x83\x63\x82\xCC\x83\x49\x83\x80\x83\x8C\x83\x63"},
    {/*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"キセツのオムレツ"*/"\x83\x4C\x83\x5A\x83\x63\x82\xCC\x83\x49\x83\x80\x83\x8C\x83\x63"},
    {/*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"こおりのいぶき"*/"\x82\xB1\x82\xA8\x82\xE8\x82\xCC\x82\xA2\x82\xD4\x82\xAB", /*"ゆきうさぎ"*/"\x82\xE4\x82\xAB\x82\xA4\x82\xB3\x82\xAC"},
    {/*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52"},
    {/*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52"},
    {/*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52"},
    {/*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52"},
    {/*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"こうちゃキノコ"*/"\x82\xB1\x82\xA4\x82\xBF\x82\xE1\x83\x4C\x83\x6D\x83\x52"},
    {/*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46"},
    {/*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ファイアキャンディ"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"びりびりキャンディ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"まどわせのこな"*/"\x82\xDC\x82\xC7\x82\xED\x82\xB9\x82\xCC\x82\xB1\x82\xC8", /*"メロメロケーキ"*/"\x83\x81\x83\x8D\x83\x81\x83\x8D\x83\x50\x81\x5B\x83\x4C"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"キノコケーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x50\x81\x5B\x83\x4C"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"キノコケーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x50\x81\x5B\x83\x4C"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"キノコクレープ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x4E\x83\x8C\x81\x5B\x83\x76"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"キノコケーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x50\x81\x5B\x83\x4C"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ハニーあめ"*/"\x83\x6E\x83\x6A\x81\x5B\x82\xA0\x82\xDF"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ナンシークッキー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x4E\x83\x62\x83\x4C\x81\x5B"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"ローヤルキャンディ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"キノコケーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x50\x81\x5B\x83\x4C"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"ナンシークッキー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x4E\x83\x62\x83\x4C\x81\x5B"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"ナンシークッキー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x4E\x83\x62\x83\x4C\x81\x5B"},
    {/*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"マンゴープディング"*/"\x83\x7D\x83\x93\x83\x53\x81\x5B\x83\x76\x83\x66\x83\x42\x83\x93\x83\x4F"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"あつあつスープ"*/"\x82\xA0\x82\xC2\x82\xA0\x82\xC2\x83\x58\x81\x5B\x83\x76"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E", /*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"カラリーナパスタ"*/"\x83\x4A\x83\x89\x83\x8A\x81\x5B\x83\x69\x83\x70\x83\x58\x83\x5E"},
    {/*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ファイアキャンディ"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"こうちゃキノコ"*/"\x82\xB1\x82\xA4\x82\xBF\x82\xE1\x83\x4C\x83\x6D\x83\x52"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"じわじわシロップ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"カメまん"*/"\x83\x4A\x83\x81\x82\xDC\x82\xF1"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"カメスパ"*/"\x83\x4A\x83\x81\x83\x58\x83\x70"},
    {/*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9"},
    {/*"つくしんぼ"*/"\x82\xC2\x82\xAD\x82\xB5\x82\xF1\x82\xDA", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"つくしんぼ"*/"\x82\xC2\x82\xAD\x82\xB5\x82\xF1\x82\xDA", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"つくしんぼ"*/"\x82\xC2\x82\xAD\x82\xB5\x82\xF1\x82\xDA", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
    {/*"つくしんぼ"*/"\x82\xC2\x82\xAD\x82\xB5\x82\xF1\x82\xDA", /*"きんきゅうキノコ"*/"\x82\xAB\x82\xF1\x82\xAB\x82\xE3\x82\xA4\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"つくしんぼ"*/"\x82\xC2\x82\xAD\x82\xB5\x82\xF1\x82\xDA", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"つくしんぼ"*/"\x82\xC2\x82\xAD\x82\xB5\x82\xF1\x82\xDA", /*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"キセツのオムレツ"*/"\x83\x4C\x83\x5A\x83\x63\x82\xCC\x83\x49\x83\x80\x83\x8C\x83\x63"},
    {/*"つくしんぼ"*/"\x82\xC2\x82\xAD\x82\xB5\x82\xF1\x82\xDA", /*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"びりびりキノコ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"まどわせのこな"*/"\x82\xDC\x82\xC7\x82\xED\x82\xB9\x82\xCC\x82\xB1\x82\xC8", /*"メロメロケーキ"*/"\x83\x81\x83\x8D\x83\x81\x83\x8D\x83\x50\x81\x5B\x83\x4C"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"スーパーキノコ"*/"\x83\x58\x81\x5B\x83\x70\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"しなびたキノコ"*/"\x82\xB5\x82\xC8\x82\xD1\x82\xBD\x83\x4C\x83\x6D\x83\x52", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"ハニーシロップ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"メイプルシロップ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x56\x83\x8D\x83\x62\x83\x76", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"ローヤルゼリー"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x5B\x83\x8A\x81\x5B", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ピーチタルト"*/"\x83\x73\x81\x5B\x83\x60\x83\x5E\x83\x8B\x83\x67"},
    {/*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"スッキリドリンク"*/"\x83\x58\x83\x62\x83\x4C\x83\x8A\x83\x68\x83\x8A\x83\x93\x83\x4E"},
    {/*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"ファイアフラワー"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ココナッツボム"*/"\x83\x52\x83\x52\x83\x69\x83\x62\x83\x63\x83\x7B\x83\x80"},
    {/*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"トロピコマンゴー"*/"\x83\x67\x83\x8D\x83\x73\x83\x52\x83\x7D\x83\x93\x83\x53\x81\x5B", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ココナツキャンディ"*/"\x83\x52\x83\x52\x83\x69\x83\x63\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42"},
    {/*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"ピチピーチ"*/"\x83\x73\x83\x60\x83\x73\x81\x5B\x83\x60", /*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58"},
    {/*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF", /*"キラキラおとし"*/"\x83\x4C\x83\x89\x83\x4C\x83\x89\x82\xA8\x82\xC6\x82\xB5", /*"ほしふるディナー"*/"\x82\xD9\x82\xB5\x82\xD3\x82\xE9\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB", /*"キラキラおとし"*/"\x83\x4C\x83\x89\x83\x4C\x83\x89\x82\xA8\x82\xC6\x82\xB5", /*"ほしふるディナー"*/"\x82\xD9\x82\xB5\x82\xD3\x82\xE9\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C", /*"キラキラおとし"*/"\x83\x4C\x83\x89\x83\x4C\x83\x89\x82\xA8\x82\xC6\x82\xB5", /*"ほしふるディナー"*/"\x82\xD9\x82\xB5\x82\xD3\x82\xE9\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"しっぱいりょうり"*/"\x82\xB5\x82\xC1\x82\xCF\x82\xA2\x82\xE8\x82\xE5\x82\xA4\x82\xE8", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ハニーキノコ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"メイプルキノコ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ローヤルキノコ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x6D\x83\x52", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ハニーキノコＳ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52\x82\x72", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"メイプルキノコＳ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x72", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ローヤルキノコＳ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x72", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ハニーキノコＺ"*/"\x83\x6E\x83\x6A\x81\x5B\x83\x4C\x83\x6D\x83\x52\x82\x79", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"メイプルキノコＺ"*/"\x83\x81\x83\x43\x83\x76\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x79", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ローヤルキノコＺ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x6D\x83\x52\x82\x79", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B", /*"カチカチコウラ"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x83\x52\x83\x45\x83\x89", /*"カチカチりょうり"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x82\xE8\x82\xE5\x82\xA4\x82\xE8"},
    {/*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B", /*"カチカチコウラ"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x83\x52\x83\x45\x83\x89", /*"カチカチりょうり"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x82\xE8\x82\xE5\x82\xA4\x82\xE8"},
    {/*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ナンシーデラックス"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x89\x83\x62\x83\x4E\x83\x58", /*"カチカチコウラ"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x83\x52\x83\x45\x83\x89", /*"カチカチりょうり"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x82\xE8\x82\xE5\x82\xA4\x82\xE8"},
    {/*"ナンシーデラックス"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x89\x83\x62\x83\x4E\x83\x58", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ナンシーダイナマイ"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x5F\x83\x43\x83\x69\x83\x7D\x83\x43", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"バクハツタマゴ"*/"\x83\x6F\x83\x4E\x83\x6E\x83\x63\x83\x5E\x83\x7D\x83\x53"},
    {/*"ナンシーティー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x65\x83\x42\x81\x5B", /*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9"},
    {/*"ゆきうさぎ"*/"\x82\xE4\x82\xAB\x82\xA4\x82\xB3\x82\xAC", /*"あつあつスープ"*/"\x82\xA0\x82\xC2\x82\xA0\x82\xC2\x83\x58\x81\x5B\x83\x76", /*"いつまでもふたりで"*/"\x82\xA2\x82\xC2\x82\xDC\x82\xC5\x82\xE0\x82\xD3\x82\xBD\x82\xE8\x82\xC5"},
    {/*"キノコケーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x50\x81\x5B\x83\x4C", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"キノコクレープ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x4E\x83\x8C\x81\x5B\x83\x76", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ムースケーキ"*/"\x83\x80\x81\x5B\x83\x58\x83\x50\x81\x5B\x83\x4C", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"バクハツタマゴ"*/"\x83\x6F\x83\x4E\x83\x6E\x83\x63\x83\x5E\x83\x7D\x83\x53", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"イカスミパスタ"*/"\x83\x43\x83\x4A\x83\x58\x83\x7E\x83\x70\x83\x58\x83\x5E", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"スパゲティ"*/"\x83\x58\x83\x70\x83\x51\x83\x65\x83\x42", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"スパゲティ"*/"\x83\x58\x83\x70\x83\x51\x83\x65\x83\x42", /*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"カラリーナパスタ"*/"\x83\x4A\x83\x89\x83\x8A\x81\x5B\x83\x69\x83\x70\x83\x58\x83\x5E"},
    {/*"スパゲティ"*/"\x83\x58\x83\x70\x83\x51\x83\x65\x83\x42", /*"カメカメはっぱ"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x82\xCD\x82\xC1\x82\xCF", /*"カメスパ"*/"\x83\x4A\x83\x81\x83\x58\x83\x70"},
    {/*"こうちゃキノコ"*/"\x82\xB1\x82\xA4\x82\xBF\x82\xE1\x83\x4C\x83\x6D\x83\x52", /*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9"},
    {/*"どくキノコ"*/"\x82\xC7\x82\xAD\x83\x4C\x83\x6D\x83\x52", /*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"ウルトラキノコ"*/"\x83\x45\x83\x8B\x83\x67\x83\x89\x83\x4C\x83\x6D\x83\x52"},
    {/*"どくキノコ"*/"\x82\xC7\x82\xAD\x83\x4C\x83\x6D\x83\x52", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"どくキノコ"*/"\x82\xC7\x82\xAD\x83\x4C\x83\x6D\x83\x52", /*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"こうちゃキノコ"*/"\x82\xB1\x82\xA4\x82\xBF\x82\xE1\x83\x4C\x83\x6D\x83\x52"},
    {/*"ショコラケーキ"*/"\x83\x56\x83\x87\x83\x52\x83\x89\x83\x50\x81\x5B\x83\x4C", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"マンゴープディング"*/"\x83\x7D\x83\x93\x83\x53\x81\x5B\x83\x76\x83\x66\x83\x42\x83\x93\x83\x4F", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"マンゴープディング"*/"\x83\x7D\x83\x93\x83\x53\x81\x5B\x83\x76\x83\x66\x83\x42\x83\x93\x83\x4F", /*"ふしぎなタマゴ"*/"\x82\xD3\x82\xB5\x82\xAC\x82\xC8\x83\x5E\x83\x7D\x83\x53", /*"はつこいプディング"*/"\x82\xCD\x82\xC2\x82\xB1\x82\xA2\x83\x76\x83\x66\x83\x42\x83\x93\x83\x4F"},
    {/*"はつこいプディング"*/"\x82\xCD\x82\xC2\x82\xB1\x82\xA2\x83\x76\x83\x66\x83\x42\x83\x93\x83\x4F", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ほしふるディナー"*/"\x82\xD9\x82\xB5\x82\xD3\x82\xE9\x83\x66\x83\x42\x83\x69\x81\x5B", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ほしふるディナー"*/"\x82\xD9\x82\xB5\x82\xD3\x82\xE9\x83\x66\x83\x42\x83\x69\x81\x5B", /*"フルーツパフェ"*/"\x83\x74\x83\x8B\x81\x5B\x83\x63\x83\x70\x83\x74\x83\x46", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"しれんのナベ"*/"\x82\xB5\x82\xEA\x82\xF1\x82\xCC\x83\x69\x83\x78", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"どくキノコ"*/"\x82\xC7\x82\xAD\x83\x4C\x83\x6D\x83\x52"},
    {/*"いつまでもふたりで"*/"\x82\xA2\x82\xC2\x82\xDC\x82\xC5\x82\xE0\x82\xD3\x82\xBD\x82\xE8\x82\xC5", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"いつまでもふたりで"*/"\x82\xA2\x82\xC2\x82\xDC\x82\xC5\x82\xE0\x82\xD3\x82\xBD\x82\xE8\x82\xC5", /*"どくキノコ"*/"\x82\xC7\x82\xAD\x83\x4C\x83\x6D\x83\x52", /*"しれんのナベ"*/"\x82\xB5\x82\xEA\x82\xF1\x82\xCC\x83\x69\x83\x78"},
    {/*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9", /*"じわじわキノコ"*/"\x82\xB6\x82\xED\x82\xB6\x82\xED\x83\x4C\x83\x6D\x83\x52", /*"どくキノコ"*/"\x82\xC7\x82\xAD\x83\x4C\x83\x6D\x83\x52"},
    {/*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9", /*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"イカスミパスタ"*/"\x83\x43\x83\x4A\x83\x58\x83\x7E\x83\x70\x83\x58\x83\x5E"},
    {/*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9", /*"おかしのもと"*/"\x82\xA8\x82\xA9\x82\xB5\x82\xCC\x82\xE0\x82\xC6", /*"ショコラケーキ"*/"\x83\x56\x83\x87\x83\x52\x83\x89\x83\x50\x81\x5B\x83\x4C"},
    {/*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9", /*"ムースケーキ"*/"\x83\x80\x81\x5B\x83\x58\x83\x50\x81\x5B\x83\x4C", /*"ショコラケーキ"*/"\x83\x56\x83\x87\x83\x52\x83\x89\x83\x50\x81\x5B\x83\x4C"},
    {/*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9", /*"スパゲティ"*/"\x83\x58\x83\x70\x83\x51\x83\x65\x83\x42", /*"イカスミパスタ"*/"\x83\x43\x83\x4A\x83\x58\x83\x7E\x83\x70\x83\x58\x83\x5E"},
    {/*"キセツのオムレツ"*/"\x83\x4C\x83\x5A\x83\x63\x82\xCC\x83\x49\x83\x80\x83\x8C\x83\x63", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"カメカメティー"*/"\x83\x4A\x83\x81\x83\x4A\x83\x81\x83\x65\x83\x42\x81\x5B", /*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9"},
    {/*"カメスパ"*/"\x83\x4A\x83\x81\x83\x58\x83\x70", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"カメスパ"*/"\x83\x4A\x83\x81\x83\x58\x83\x70", /*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"カラリーナパスタ"*/"\x83\x4A\x83\x89\x83\x8A\x81\x5B\x83\x69\x83\x70\x83\x58\x83\x5E"},
    {/*"カメスパ"*/"\x83\x4A\x83\x81\x83\x58\x83\x70", /*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9", /*"イカスミパスタ"*/"\x83\x43\x83\x4A\x83\x58\x83\x7E\x83\x70\x83\x58\x83\x5E"},
    {/*"カラリーナパスタ"*/"\x83\x4A\x83\x89\x83\x8A\x81\x5B\x83\x69\x83\x70\x83\x58\x83\x5E", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"カラリーナパスタ"*/"\x83\x4A\x83\x89\x83\x8A\x81\x5B\x83\x69\x83\x70\x83\x58\x83\x5E", /*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"カラリーナパスタ"*/"\x83\x4A\x83\x89\x83\x8A\x81\x5B\x83\x69\x83\x70\x83\x58\x83\x5E", /*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9", /*"イカスミパスタ"*/"\x83\x43\x83\x4A\x83\x58\x83\x7E\x83\x70\x83\x58\x83\x5E"},
    {/*"メロメロケーキ"*/"\x83\x81\x83\x8D\x83\x81\x83\x8D\x83\x50\x81\x5B\x83\x4C", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ピーチタルト"*/"\x83\x73\x81\x5B\x83\x60\x83\x5E\x83\x8B\x83\x67", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"びりびりキャンディ"*/"\x82\xD1\x82\xE8\x82\xD1\x82\xE8\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ファイアキャンディ"*/"\x83\x74\x83\x40\x83\x43\x83\x41\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ハニーあめ"*/"\x83\x6E\x83\x6A\x81\x5B\x82\xA0\x82\xDF", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ココナツキャンディ"*/"\x83\x52\x83\x52\x83\x69\x83\x63\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ローヤルキャンディ"*/"\x83\x8D\x81\x5B\x83\x84\x83\x8B\x83\x4C\x83\x83\x83\x93\x83\x66\x83\x42", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ナンシークッキー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x4E\x83\x62\x83\x4C\x81\x5B", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"生パスタ"*/"\x90\xB6\x83\x70\x83\x58\x83\x5E", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"キノコいため"*/"\x83\x4C\x83\x6D\x83\x52\x82\xA2\x82\xBD\x82\xDF", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"キノコホイルやき"*/"\x83\x4C\x83\x6D\x83\x52\x83\x7A\x83\x43\x83\x8B\x82\xE2\x82\xAB", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"キノコステーキ"*/"\x83\x4C\x83\x6D\x83\x52\x83\x58\x83\x65\x81\x5B\x83\x4C", /*"ナンシーデラックス"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x89\x83\x62\x83\x4E\x83\x58"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"イカスミパスタ"*/"\x83\x43\x83\x4A\x83\x58\x83\x7E\x83\x70\x83\x58\x83\x5E", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"スパゲティ"*/"\x83\x58\x83\x70\x83\x51\x83\x65\x83\x42", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"カメスパ"*/"\x83\x4A\x83\x81\x83\x58\x83\x70", /*"ナンシーディナー"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x42\x83\x69\x81\x5B"},
    {/*"ヘルスィーサラダ"*/"\x83\x77\x83\x8B\x83\x58\x83\x42\x81\x5B\x83\x54\x83\x89\x83\x5F", /*"カラリーナパスタ"*/"\x83\x4A\x83\x89\x83\x8A\x81\x5B\x83\x69\x83\x70\x83\x58\x83\x5E", /*"ナンシースペシャル"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x58\x83\x79\x83\x56\x83\x83\x83\x8B"},
    {/*"カメまん"*/"\x83\x4A\x83\x81\x82\xDC\x82\xF1", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"フレッシュジュース"*/"\x83\x74\x83\x8C\x83\x62\x83\x56\x83\x85\x83\x57\x83\x85\x81\x5B\x83\x58", /*"レッド・カラリン"*/"\x83\x8C\x83\x62\x83\x68\x81\x45\x83\x4A\x83\x89\x83\x8A\x83\x93", /*"すみぢる"*/"\x82\xB7\x82\xDD\x82\xC0\x82\xE9"},
    {/*"たんこぶ"*/"\x82\xBD\x82\xF1\x82\xB1\x82\xD4", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"うちゅうしょく"*/"\x82\xA4\x82\xBF\x82\xE3\x82\xA4\x82\xB5\x82\xE5\x82\xAD"},
    {/*"たんこぶ"*/"\x82\xBD\x82\xF1\x82\xB1\x82\xD4", /*"おうごんはっぱ"*/"\x82\xA8\x82\xA4\x82\xB2\x82\xF1\x82\xCD\x82\xC1\x82\xCF", /*"ナンシーデラックス"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x66\x83\x89\x83\x62\x83\x4E\x83\x58"},
    {/*"きんかい"*/"\x82\xAB\x82\xF1\x82\xA9\x82\xA2", /*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"キノコ"*/"\x83\x4C\x83\x6D\x83\x52"},
    {/*"きんかい×３"*/"\x82\xAB\x82\xF1\x82\xA9\x82\xA2\x81\x7E\x82\x52", /*"イレカエール"*/"\x83\x43\x83\x8C\x83\x4A\x83\x47\x81\x5B\x83\x8B", /*"きんかい"*/"\x82\xAB\x82\xF1\x82\xA9\x82\xA2"},
    {/*"ココナッツボム"*/"\x83\x52\x83\x52\x83\x69\x83\x62\x83\x63\x83\x7B\x83\x80", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"ヤシの実"*/"\x83\x84\x83\x56\x82\xCC\x8E\xC0"},
    {/*"ココナッツボム"*/"\x83\x52\x83\x52\x83\x69\x83\x62\x83\x63\x83\x7B\x83\x80", /*"バクハツタマゴ"*/"\x83\x6F\x83\x4E\x83\x6E\x83\x63\x83\x5E\x83\x7D\x83\x53", /*"ナンシーダイナマイ"*/"\x83\x69\x83\x93\x83\x56\x81\x5B\x83\x5F\x83\x43\x83\x69\x83\x7D\x83\x43"},
    {/*"カチカチりょうり"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x82\xE8\x82\xE5\x82\xA4\x82\xE8", /*"ドライフラワー"*/"\x83\x68\x83\x89\x83\x43\x83\x74\x83\x89\x83\x8F\x81\x5B", /*"カチカチコウラ"*/"\x83\x4A\x83\x60\x83\x4A\x83\x60\x83\x52\x83\x45\x83\x89"},
#endif
};

s32 R_mystery_cook_list[] = {
    132, 153, 235, 138, 151, 141, 135, 179,
    223, 192, 222, 211, 207, 183, 193, 129,
    209, 176, 232, 197,
};

s32 special_cook_list[] = {
    194, 195, 196, 203, 210, 213, 214, 215,
    216, 217, 218, 219, 220, 221, 224, -1,
};

static USER_FUNC(starstone_entry) {
    char name[16];
    EvtWord* args = event->args;
    f32 x;
    f32 s;
    f32 z;
    f32 angle;
    f32 y;
    f32 c;
    EffectEntry* eff;
    s32 i;

    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    for (i = 0; i < 7; i++) {
        angle = 6.2831855f * i / 7.0f;
        s = sin(angle);
        c = cos(angle);
        sprintf(name, "starstone%d", i);
        eff = effStarStoneEntry(i, 140.0f * s + x, y, z - 140.0f * c, 1.0f);
        effSetName(eff, name);
        ((EffStarStoneWork*)eff->userdata)->flags &= ~4;
        ((EffStarStoneWork*)eff->userdata)->flags |= 0x20;
        ((EffMahojinWork*)effNameToPtr("mahojin")->userdata)[i + 2].arg = eff;
        ((EffMahojinWork*)effNameToPtr("mahojin")->userdata)[i + 2].func = effStarStoneDraw;
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_pos) {
    char name[16];
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    f32 s;
    f32 angle;
    f32 c;
    EffectEntry* eff;
    s32 i;

    for (i = 0; i < 7; i++) {
        angle = 6.2831855f * i / 7.0f;
        s = sin(angle);
        c = cos(angle);
        sprintf(name, "starstone%d", i);
        eff = effNameToPtr(name);
        ((EffStarStoneWork*)eff->userdata)->position.x = 140.0f * s + x;
        ((EffStarStoneWork*)eff->userdata)->position.y = y;
        ((EffStarStoneWork*)eff->userdata)->position.z = z - 140.0f * c;
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_ry) {
    char name[16];
    f32 ry = evtGetFloat(event, event->args[0]);
    s32 i;

    for (i = 0; i < 7; i++) {
        sprintf(name, "starstone%d", i);
        ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(wait_mahojin) {
    s32 anim = ((s32*)effNameToPtr("mahojin")->userdata)[6];

    if (anim != -1 && 0.0f != animPoseGetLoopTimes(anim)) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(iri_09_item_tbl_alloc) {
#ifdef VERSION_G8ME01
    s32 n = evtGetValue(event, event->args[0]);
    iri_09_item_tbl = _mapAlloc((n + 1) * sizeof(s32));
#else
    iri_09_item_tbl = _mapAlloc((evtGetValue(event, event->args[0]) + 1) * sizeof(s32));
#endif
    return EVT_RETURN_DONE;
}

USER_FUNC(iri_09_item_tbl_make) {
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);
    s32 i;

    for (i = 0; i < n; i++) {
        iri_09_item_tbl[i] = 143;
    }
    iri_09_item_tbl[i] = -1;
    evtSetValue(event, args[1], (EvtWord)iri_09_item_tbl);
    return EVT_RETURN_DONE;
}

USER_FUNC(iri_09_item_tbl_free) {
    _mapFree(iri_09_item_tbl);
    iri_09_item_tbl = NULL;
    return EVT_RETURN_DONE;
}

static USER_FUNC(set_fook_evt) {
    EvtWord* args = event->args;
    s32* wp = (s32*)evtGetValue(event, args[0]);

    wp[1] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

s32 naniga_okorukana_cook(void) {
    s32 r = rand() % 100;

    if (r < 50) {
        return R_mystery_cook_list[(s32)((u32)(r * 20) / 50.0f)];
    }
    return 182;
}

s32 itemNameToID(const char* name) {
    s32 i;

    for (i = 0; i < 339; i++) {
        if (strcmp(name, itemDataTable[i].id) == 0) {
            break;
        }
    }
    if (i >= 339) {
        return 0;
    }
    return i;
}

USER_FUNC(cook_chk) {
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);
    ItemData* data = &itemDataTable[item];
    u32 i;

    if (item == 163) {
        item = naniga_okorukana_cook();
        evtSetValue(event, args[1], item);
        evtSetValue(event, args[2], item);
        return EVT_RETURN_DONE;
    }
    for (i = 0; i < 26; i++) {
        if (strcmp(data->id, R_simple_recipe_table[i].ingredient) == 0) {
            evtSetValue(event, (volatile u32)args[1], itemNameToID(R_simple_recipe_table[i].result));
            evtSetValue(event, (volatile u32)args[2], itemNameToID(R_simple_recipe_table[i].result2));
            return EVT_RETURN_DONE;
        }
    }
    evtSetValue(event, args[1], 0);
    evtSetValue(event, args[2], 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(cook_chk2) {
    EvtWord* args = event->args;
    s32 item1 = evtGetValue(event, args[0]);
    s32 item2 = evtGetValue(event, args[1]);
    ItemData* data1 = &itemDataTable[item1];
    ItemData* data2 = &itemDataTable[item2];
    u32 i;

    if (item1 == 163 || item2 == 163) {
        item2 = naniga_okorukana_cook();
        evtSetValue(event, (volatile u32)args[2], item2);
        return EVT_RETURN_DONE;
    }
#ifdef VERSION_G8ME01
    for (i = 0; i < 292; i++) {
#else
    for (i = 0; i < 289; i++) {
#endif
        if ((strcmp(data1->id, R_cookbook_recipe_table[i].a) == 0 && strcmp(data2->id, R_cookbook_recipe_table[i].b) == 0) ||
            (strcmp(data2->id, R_cookbook_recipe_table[i].a) == 0 && strcmp(data1->id, R_cookbook_recipe_table[i].b) == 0)) {
            evtSetValue(event, (volatile u32)args[2], itemNameToID(R_cookbook_recipe_table[i].out));
            return EVT_RETURN_DONE;
        }
    }
    evtSetValue(event, args[2], 0);
    return EVT_RETURN_DONE;
}

BOOL chkCookItem(s32 item) {
    return item != 0;
}

USER_FUNC(make_item_tbl) {
    EvtWord* args = event->args;
    s32 skip = evtGetValue(event, args[0]);
    s32 cnt = pouchGetHaveItemCnt();
    s32 n = 0;
    s32 i;
    s32 item;

    for (i = 0; i < cnt; i++) {
        if (skip != i) {
            item = pouchHaveItem(i);
            if (item != 0) {
                item_tbl[n] = item;
                n++;
            }
        }
    }
    item_tbl[n] = -1;
    evtSetValue(event, args[1], n);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_item_name) {
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);

#ifdef VERSION_G8ME01
    ItemData* data = &itemDataTable[item];
    evtSetValue(event, args[1], (s32)msgSearch(data->name));
#else
    evtSetValue(event, args[1], (EvtWord)itemDataTable[item].id);
#endif
    return EVT_RETURN_DONE;
}

s32 chk_cook_eff_type(s32 item) {
    if (item < 148) {
        return 1;
    }
    if (item < 161) {
        return 0;
    }
    if (item < 179) {
        switch (item) {
            case 161:
            case 162:
            case 163:
            case 164:
            case 177:
            case 178:
                return 1;
            default:
                return 0;
        }
    }
    if (item < 236) {
        switch (item) {
            case 196:
            case 202:
            case 203:
            case 209:
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}

USER_FUNC(cook_effect) {
    switch (chk_cook_eff_type(evtGetValue(event, event->args[0]))) {
        case 0:
            psndSFXOn_3D("SFX_EVT_COOK_FIRE1", &(Vec){-380.0f, 25.0f, -170.0f});
            effFireEntry(4, 60, -380.0f, 25.0f, -170.0f, 1.5f);
            break;
        case 1:
        default:
            psndSFXOn_3D("SFX_EVT_COOK_EXPLOSION1", &(Vec){-380.0f, 60.0f, -170.0f});
            effKemuri12N64Entry(0, 60, -380.0f, 60.0f, -170.0f, 2.0f);
            break;
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(cooking_evt_sub)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_COOKING1"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_status_onoff, 1, STRING("ナンシー"), 2)
    CALL(evt_npc_move_position, STRING("ナンシー"), -485, -150, 0, 80, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("ナンシー"), 90)
    CALL(evt_npc_set_anim, STRING("ナンシー"), STRING("S_1R"))
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_INSTANT_TURN)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("gor_01_cook_016"), 0, STRING("ナンシー"))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("ナンシー"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_COOK_COOKING1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_npc_set_anim, STRING("ナンシー"), STRING("C_1"))
    WAIT_MS(2500)
    CALL(evt_snd_sfxoff, LW(3))
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_move_position, STRING("ナンシー"), -430, -150, 0, 80, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("ナンシー"), STRING("C_1"))
    CALL(evt_npc_get_position, STRING("ナンシー"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_COOK_COOKING2"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1500)
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_COOK_COOKING2"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(1500)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_move_position, STRING("ナンシー"), -380, -150, 0, 80, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("ナンシー"), STRING("S_1R"))
    WAIT_MS(200)
    CALL(evt_npc_get_position, STRING("ナンシー"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_COOK_COOKING3"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_npc_set_anim, STRING("ナンシー"), STRING("C_1"))
    WAIT_MS(2800)
    CALL(evt_snd_sfxoff, LW(3))
    WAIT_MS(200)
    CALL(cook_effect, LW(14))
    WAIT_MS(1300)
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(cook_effect, LW(15))
        WAIT_MS(1300)
    END_IF
    CALL(chk_special_cook, LW(13), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_COOK_LIGHT1"), -380, 40, -170, LW(0))
        CALL(evt_eff64, STRING("rad"), STRING("radiation_n64"), 3, -380, 40, -170, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1300)
        CALL(evt_eff_delete, STRING("rad"))
        CALL(evt_snd_sfxoff, LW(0))
        WAIT_MS(500)
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_move_position, STRING("ナンシー"), -405, -100, 0, 80, 0)
    CALL(evt_set_dir_to_target, STRING("ナンシー"), STRING("mario"))
    CALL(evt_npc_status_onoff, 0, STRING("ナンシー"), 2)
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_TALK_POSE)
    WAIT_MS(300)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    WAIT_MS(500)
    RETURN
EVT_END()

s32 iri_18_item_tbl[] = {
    ITEM_COOKBOOK, -1,
};

EVT_BEGIN(cooking_evt)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(11), -1)
    SET_VALUE(LW(12), -1)
    SET_VALUE(LW(14), 0)
    SET_VALUE(LW(15), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_NO_HIT)
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 1, -337, -80, 60)
        CALL(evt_party_set_dir_npc, 1, STRING("ナンシー"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -347, -100, 60)
        CALL(evt_party_set_dir_npc, 0, STRING("ナンシー"))
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_sub_get_dist, LW(0), LW(2), -377, -100, LW(3))
    IF_INT_LESS(LW(3), 10)
        CALL(evt_mario_dispflag_onoff, 1, 4)
    END_IF
    CALL(evt_mario_mov_pos2, -377, -100, 60)
    CALL(evt_mario_dispflag_onoff, 0, 4)
    CALL(evt_mario_set_dir_npc, STRING("ナンシー"))
    WAIT_MS(500)
    WAIT_WHILE(GF(0))
    CALL(evt_set_dir_to_target, STRING("ナンシー"), STRING("mario"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ナンシー"))
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_NO_HIT)
    IF_INT_EQUAL(GSW(1420), 18)
        IF_INT_EQUAL(GSWF(5405), 0)
            CALL(evt_msg_print, 0, STRING("irai_18_001"), 0, STRING("ナンシー"))
            SET_VALUE(GSWF(5405), 1)
            CALL(evt_msg_print_add, 0, STRING("irai_18_008_1"))
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_18_002"), 0, STRING("ナンシー"))
            SET_VALUE(LF(0), 0)
            CALL(evt_pouch_check_item, ITEM_COOKBOOK, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                CALL(evt_msg_continue)
                CALL(evt_win_item_select, 0, PTR(iri_18_item_tbl), LW(0), 0)
                ELSE
                SET_VALUE(LW(0), -1)
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_NOT_EQUAL(LW(0), -1)
                CALL(evt_pouch_remove_item, ITEM_COOKBOOK, LW(0))
                CALL(evt_msg_print, 0, STRING("irai_18_003"), 0, STRING("ナンシー"))
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_npc_get_position, STRING("ナンシー"), LW(3), LW(4), LW(5))
                IF_INT_LESS(LW(0), LW(3))
                    ADD(LW(0), 20)
                    ELSE
                    SUBTRACT(LW(0), 20)
                END_IF
                ADD(LW(1), 20)
                ADD(LW(2), 5)
                CALL(evt_item_entry, STRING("cb"), ITEM_COOKBOOK, LW(0), LW(1), LW(2), 17, -1, 0)
                CALL(evt_mario_set_pose, STRING("M_C_3"))
                WAIT_MS(800)
                CALL(evt_item_delete, STRING("cb"))
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                WAIT_MS(500)
                CALL(evt_msg_print, 0, STRING("irai_18_004"), 0, STRING("ナンシー"))
                CALL(evt_mario_set_pose, STRING("M_N_1"))
                SET_VALUE(LW(13), 183)
                SET_VALUE(LW(14), 148)
                SET_VALUE(LW(15), 154)
                CALL_CHILD(PTR(cooking_evt_sub))
                CALL(evt_msg_print, 0, STRING("irai_18_005"), 0, STRING("ナンシー"))
                SET_VALUE(LF(0), 0)
                CALL(evt_pouch_get_haveitemcnt, LW(0))
                CALL(evt_pouch_check_item, ITEM_STRANGE_SACK, LW(1))
                IF_INT_EQUAL(LW(1), 0)
                    IF_INT_GREATER_OR_EQUAL(LW(0), 10)
                        SET_VALUE(LF(0), 1)
                    END_IF
                    ELSE
                    IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                        SET_VALUE(LF(0), 1)
                    END_IF
                END_IF
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_msg_continue)
                    CALL(evt_item_entry, STRING("hk"), ITEM_HONEY_SHROOM, 0, -1000, 0, 17, -1, 0)
                    CALL(evt_item_get_item, STRING("hk"))
                    CALL(evt_item_get_item_end_wait, STRING("hk"))
                    SET_VALUE(GSWF(1202), 1)
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("irai_18_006_1"))
                    CALL(evt_npc_set_ry, STRING("ナンシー"), 270)
                    WAIT_FRAMES(1)
                    CALL(evt_npc_wait_pera, STRING("ナンシー"))
                    WAIT_MS(300)
                    CALL(evt_item_entry, STRING("iri_18"), ITEM_HONEY_SHROOM, -440, 30, -80, 16, GSWF(1202), 0)
                    WAIT_MS(300)
                    CALL(evt_npc_set_ry, STRING("ナンシー"), 90)
                    WAIT_FRAMES(1)
                    CALL(evt_npc_wait_pera, STRING("ナンシー"))
                END_IF
                CALL(evt_msg_print, 0, STRING("irai_18_007"), 0, STRING("ナンシー"))
                SET_VALUE(GSW(1420), 0)
                SET_VALUE(GSWF(5392), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                CALL(evt_mario_key_onoff, 1)
                RETURN
                ELSE
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_msg_print, 0, STRING("irai_18_008"), 0, STRING("ナンシー"))
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("irai_18_008"))
                END_IF
                CALL(evt_msg_print_add, 0, STRING("irai_18_008_1"))
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF(1197), 0)
        IF_INT_NOT_EQUAL(GSW(1420), 18)
            IF_INT_EQUAL(GSWF(5392), 0)
                CALL(evt_msg_print, 0, STRING("gor_01_cook_000"), 0, STRING("ナンシー"))
            END_IF
        END_IF
        CALL(evt_msg_print_add, 0, STRING("gor_01_cook_001"))
        SET_VALUE(GSWF(1197), 1)
        ELSE
        IF_INT_EQUAL(LSWF(1), 0)
            SET_VALUE(LF(0), 0)
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 215)
                IF_INT_NOT_EQUAL(GSW(1420), 18)
                    IF_INT_EQUAL(GSWF(5392), 0)
                        SET_VALUE(LF(0), 1)
                    END_IF
                END_IF
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                IF_INT_NOT_EQUAL(GSW(1420), 18)
                    CALL(evt_msg_print, 0, STRING("gor_01_cook_002"), 0, STRING("ナンシー"))
                    ELSE
                    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_002"))
                END_IF
                ELSE
                CALL(evt_msg_print, 0, STRING("gor_01_cook_003"), 0, STRING("ナンシー"))
            END_IF
            ELSE
            IF_INT_NOT_EQUAL(GSW(1420), 18)
                CALL(evt_msg_print, 0, STRING("gor_01_cook_004"), 0, STRING("ナンシー"))
                ELSE
                CALL(evt_msg_print_add, 0, STRING("gor_01_cook_004"))
            END_IF
        END_IF
    END_IF
    SET_VALUE(LSWF(1), 1)
    CALL(evt_msg_select, 0, STRING("gor_01_select"))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(0)
    END_IF
    CALL(make_item_tbl, LW(11), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("gor_01_cook_005"))
        GOTO(5)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_007"))
    LABEL(1)
    CALL(evt_win_item_select, 1, PTR(item_tbl), LW(0), LW(1))
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_msg_print, 0, STRING("gor_01_cook_008"), 0, STRING("ナンシー"))
        CALL(evt_msg_select, 0, STRING("gor_01_select"))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            GOTO(0)
            CASE_EQUAL(1)
            CALL(evt_msg_continue)
            GOTO(1)
        END_SWITCH
    END_IF
    SET_VALUE(LW(14), LW(0))
    SET_VALUE(LW(11), LW(1))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSWF(5392), 1)
        CALL(make_item_tbl, LW(11), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("gor_01_cook_009"), 0, STRING("ナンシー"))
            CALL(evt_msg_select, 0, STRING("gor_01_select"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_continue)
                LABEL(2)
                CALL(evt_win_item_select, 1, PTR(item_tbl), LW(0), LW(1))
                IF_INT_EQUAL(LW(0), -1)
                    CALL(evt_msg_print, 0, STRING("gor_01_cook_010"), 0, STRING("ナンシー"))
                    CALL(evt_msg_select, 0, STRING("gor_01_cook_select"))
                    SWITCH(LW(0))
                        CASE_EQUAL(0)
                        SET_VALUE(LF(0), 1)
                        GOTO(3)
                        CASE_EQUAL(1)
                        CALL(evt_msg_continue)
                        GOTO(2)
                        CASE_EQUAL(2)
                        GOTO(0)
                    END_SWITCH
                END_IF
                IF_INT_EQUAL(LW(14), LW(0))
                    CALL(evt_msg_print, 0, STRING("gor_01_cook_011"), 0, STRING("ナンシー"))
                    GOTO(2)
                END_IF
                SET_VALUE(LW(15), LW(0))
                SET_VALUE(LW(12), LW(1))
                ELSE
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
    END_IF
    LABEL(3)
    IF_INT_EQUAL(LW(15), 0)
        CALL(get_item_name, LW(14), LW(0))
        IF_INT_EQUAL(LF(0), 0)
            CALL(evt_msg_print_insert, 0, STRING("gor_01_cook_012"), 0, STRING("ナンシー"), LW(0))
            ELSE
            CALL(evt_msg_print_add_insert, 0, STRING("gor_01_cook_012"), LW(0))
        END_IF
        ELSE
        CALL(get_item_name, LW(14), LW(0))
        CALL(get_item_name, LW(15), LW(1))
        CALL(evt_msg_print_insert, 0, STRING("gor_01_cook_013"), 0, STRING("ナンシー"), LW(0), LW(1))
    END_IF
    CALL(evt_msg_select, 0, STRING("gor_01_select"))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(4)
    END_IF
    IF_INT_EQUAL(LW(15), 0)
        CALL(cook_chk, LW(14), LW(0), LW(1))
        CALL(N_evt_pouch_remove_item_index, LW(14), LW(11), LW(3))
        IF_INT_EQUAL(GSWF(5392), 1)
            SET_VALUE(LW(2), LW(1))
            ELSE
            SET_VALUE(LW(2), LW(0))
        END_IF
        ELSE
        CALL(cook_chk2, LW(14), LW(15), LW(2))
        CALL(N_evt_pouch_remove_item_index, LW(14), LW(11), LW(3))
        CALL(N_evt_pouch_remove_item_index, LW(15), LW(12), LW(3))
    END_IF
    IF_INT_EQUAL(LW(2), 0)
        SET_VALUE(LW(2), 182)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_015"))
    SET_VALUE(LW(13), LW(2))
    CALL_CHILD(PTR(cooking_evt_sub))
    SET_VALUE(LW(2), LW(13))
    CALL(chk_cook_msg, LW(2), LW(3))
    SWITCH(LW(3))
        CASE_EQUAL(0)
        CALL(evt_msg_print, 0, STRING("gor_01_cook_018"), 0, STRING("ナンシー"))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("gor_01_cook_017"), 0, STRING("ナンシー"))
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("gor_01_cook_020"), 0, STRING("ナンシー"))
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("gor_01_cook_019"), 0, STRING("ナンシー"))
        CASE_EQUAL(4)
        CALL(evt_msg_print, 0, STRING("gor_01_cook_021"), 0, STRING("ナンシー"))
    END_SWITCH
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_022"))
    CALL(evt_item_entry, STRING("cook"), LW(2), 0, -1000, 0, 17, -1, 0)
    CALL(evt_item_get_item, STRING("cook"))
    SET_VALUE(GSWF(64), 1)
    CALL(cooking_flag, LW(2))
    CALL(evt_msg_print, 0, STRING("gor_01_cook_023"), 0, STRING("ナンシー"))
    GOTO(5)
    LABEL(0)
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_006"))
    GOTO(5)
    LABEL(4)
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_014"))
    GOTO(5)
    LABEL(5)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

USER_FUNC(chk_cook_msg) {
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);
    s32 msg;

    if (item == 195 || item == 196) {
        msg = 0;
    } else if ((item >= 193 && item <= 197) || item == 232 || item == 200) {
        msg = 1;
    } else if (item == 182) {
        msg = 2;
    } else {
        msg = chk_cook_eff_type(item) + 3;
    }
    evtSetValue(event, args[1], msg);
    return EVT_RETURN_DONE;
}

USER_FUNC(cooking_flag) {
    swSet(evtGetValue(event, event->args[0]) - 114);
    return EVT_RETURN_DONE;
}

USER_FUNC(chk_special_cook) {
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);
    s32 found = FALSE;
    s32 i;
    s32 v;

    for (i = 0; (v = special_cook_list[i]) != -1; i++) {
        if (item == v) {
            found = TRUE;
            break;
        }
    }
    evtSetValue(event, args[1], found);
    return EVT_RETURN_DONE;
}


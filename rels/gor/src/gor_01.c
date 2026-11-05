// gor_01 (ゴロツキタウン, Rogueport): central plaza. Badge shop, Zess T. ("nancy" in the code) and her cooking,
// star-stone scenes. Hub between the harbor and the east and west sides.

#include "version.h"
#include "eff/eff_starstone.h"
#include "evt/evt_story.h"
#include "evt/evt_urouro.h"
#include "evt/evt_win.h"
#include "evt/evt_window.h"
#include "kpa_act.h"
#include "eff/eff_mahojin.h"
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
#include "driver/itemdrv.h"
#include "driver/envdrv.h"
#include "evt/evt_johoya.h"

#include "manager/evtmgr_cmd.h"
#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "mario/mario.h"
#include "system.h"
#include "data/item_data.h"
#include "dolphin/gx.h"
#include "string.h"
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

typedef struct ShopSel {
    u8 field_0x0[8];
    s32* items;
#if VERSION <= VERSION_DEMO_JP
    u8 field_0xC[0x18 - 0xC];
#else
    u8 field_0xC[0x24 - 0xC];
#endif
    s32 sel;
} ShopSel;
s32* iri_09_item_tbl;
static s32 item_tbl[21];

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
static USER_FUNC(evt_osreport);
USER_FUNC(get_item_name);
static USER_FUNC(get_value);
USER_FUNC(house03_f1_fade_func);
USER_FUNC(house03_f2_fade_func);
USER_FUNC(iri_09_item_tbl_alloc);
USER_FUNC(iri_09_item_tbl_free);
USER_FUNC(iri_09_item_tbl_make);
USER_FUNC(maffia_kemuri);
USER_FUNC(make_item_tbl);
static USER_FUNC(mario_keyoff_wait);
USER_FUNC(nancy_block_func);
USER_FUNC(nancy_block_init_func);
static USER_FUNC(set_fook_evt);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
USER_FUNC(badgeshop_special_cnt);
USER_FUNC(wait_mahojin);
static USER_FUNC(yami_view);
static s32 npc_process(void);

#if VERSION == VERSION_DEMO_JP
static EvtWord sell_evt[116];
EvtWord zaiko_buy_evt[151];
#else
static EvtWord sell_evt[VERSION_PICK(135, 146)];
EvtWord zaiko_buy_evt[VERSION_PICK(174, 185)];
#endif


#if VERSION <= VERSION_DEMO_JP
static EvtWord buy_evt[41];
extern EvtWord badgeshopper_talk_evt[];
#else
static EvtWord buy_evt[53];
EvtWord badgeshopper_talk_evt[38];
#endif

EvtWord badgemaster_init[36];
EvtWord badgemaster_talk[76];
EvtWord kinoji_init[2];
EvtWord kinoji_talk[76];

#if VERSION == VERSION_DEMO_JP
EvtWord nancy_init_check[585];
#else
EvtWord nancy_init_check[608];
#endif

EvtWord nancy_init[58];
EvtWord nancy_door_anime[99];
EvtWord nancy_regl[35];
s32 item_tbl_nancy[2];

#if VERSION == VERSION_DEMO_JP
EvtWord nancy_talk[173];
#else
EvtWord nancy_talk[175];
#endif

static EvtWord marco_init[14];
static EvtWord marco_talk[519];
EvtWord master_init[12];
EvtWord iri_20_master_tubuyaki[37];
EvtWord master_talk[];
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

#if VERSION <= VERSION_DEMO_JP
extern EvtWord bottakuru_init[];
#else
EvtWord bottakuru_init[25];
#endif


#if VERSION == VERSION_DEMO_JP
EvtWord bottakuru_talk[231];
#else
EvtWord bottakuru_talk[VERSION_PICK(292, 314)];
#endif

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

#if VERSION == VERSION_DEMO_JP
EvtWord evt_badge_dummy[139];
#else
EvtWord evt_badge_dummy[VERSION_PICK(128, 139)];
#endif

UnitWinSell uwnobj_list[6];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord evt_open_02_01_f2[];
#else
EvtWord evt_open_02_01_f2[22];
#endif

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

#if VERSION <= VERSION_DEMO_JP
extern EvtWord maffia_assault[];
#else
EvtWord maffia_assault[797];
#endif

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

#if VERSION <= VERSION_DEMO_JP
extern EvtWord iri_09_tentyo_talk[];
#else
EvtWord iri_09_tentyo_talk[172];
#endif

s32 iri_16_item_tbl[2];
static EvtWord iri_16_init[34];

#if VERSION == VERSION_DEMO_JP
EvtWord iri_16_tyutyu_talk[443];
#else
EvtWord iri_16_tyutyu_talk[445];
#endif

EvtWord marco_jewel[22];
static EvtWord epilogue_npc_entry[48];
static EvtWord epilogue_evt_sub[160];

#if VERSION <= VERSION_DEMO_JP
static EvtWord epilogue_evt[312];
#else
static EvtWord epilogue_evt[313];
#endif


#if VERSION >= VERSION_US
EvtWord dai_disp[52];
#endif


#if VERSION == VERSION_DEMO_JP
EvtWord gor_01_init_evt[542];
#else
EvtWord gor_01_init_evt[VERSION_PICK(546, 548)];
#endif


#if VERSION >= VERSION_JP
SimpleRecipe R_simple_recipe_table[26];
CookRecipe R_cookbook_recipe_table[VERSION_PICK(289, 292)];
#endif

s32 R_mystery_cook_list[20];
s32 special_cook_list[16];
EvtWord cooking_evt_sub[284];
s32 iri_18_item_tbl[2];
EvtWord cooking_evt[];

// Descriptive name (not in the symbol maps); placeholder unk_gor_00004db4, JP gor .text+0x4CE8. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/**
 * Returns in args[0] how many special badges the badge shop currently has in stock (badgeShop_SpecialCheck).
 * JP and later releases.
 */
USER_FUNC(badgeshop_special_cnt) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], badgeShop_SpecialCheck());
    return EVT_RETURN_DONE;
}
#endif

/**
 * Zess T.'s ("Nancy") position-blocking helper, setup half:
 * remembers her current z in LW float 10 and 11 as the line she stands on,
 * and (JP and later) makes her resume her regular event when Mario leaves her talk range.
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(nancy_block_init_func) {
#else
USER_FUNC(nancy_block_init_func) {
#endif

    NpcEntry* npc = evtNpcNameToPtr(event, "\203i\203\223\203V\201[");
    f32 z;

    z = npc->position.z;
    event->lwDataF[10] = z;
    event->lwDataF[11] = z;
#if VERSION >= VERSION_JP
    npc->flags |= NPC_FLAG_RESUME_AFTER_TALK;
#endif
    return EVT_RETURN_DONE;
}

/**
 * Per-frame half of Zess T.'s blocking: when Mario is between 30 and 130 units away,
 * she slides along z (4 units per frame, within 40 units of her post) to stay level with him
 * so he cannot slip past into the gap.
 */
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

/**
 * Sets or clears bit 0 of the NPC work flags (args[0] nonzero sets it);
 * Trouble Center job 5 turns it on so the target NPC keeps reacting to Mario hitting it.
 */
USER_FUNC(chusan1_alwaysencont) {
    EvtWord* args = event->args;
    NpcWork* wp = npcGetWorkPtr();

    if (evtGetValue(event, args[0])) {
        wp->flags |= NPC_WORK_ALWAYS_ENCOUNT;
    } else {
        wp->flags &= ~NPC_WORK_ALWAYS_ENCOUNT;
    }
    return EVT_RETURN_DONE;
}

/** Forces the status window (HP/FP/coins) open on screen. */
static USER_FUNC(evt_StatusOpen) {
    statusWinForceOpen();
    return EVT_RETURN_DONE;
}

/** Ends the forced status-window opening requested by evt_StatusOpen. */
static USER_FUNC(evt_StatusClose) {
    statusWinForceOff();
    return EVT_RETURN_DONE;
}

/** Clears the display bit of the 16-bit flags word at the address in args[0]; used to hide a shop menu window. */
static USER_FUNC(disp_off) {
    *(u16*)evtGetValue(event, event->args[0]) &= ~1;
    return EVT_RETURN_DONE;
}

/**
 * Reads the shop window selection at args[0] and returns, for the highlighted item:
 * its name (message or id) in args[1], its item id in args[2],
 * its discounted price in args[3] and its badge BP cost in args[4].
 */
static USER_FUNC(get_value) {
    EvtWord* args = event->args;
    ShopSel* wp = (ShopSel*)evtGetValue(event, args[0]);

#if VERSION >= VERSION_US
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

/** Badge shop sale menu: demos start new clerk messages; later releases also use message continuations. The coin window opens with each menu. */
static EVT_BEGIN(sell_evt)
    CALL(evt_pouch_get_havebadgecnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print, 0, STRING("badgeshop_06"), 0, STRING("バッジ店員"))
        RETURN
    END_IF
    LABEL(0)
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(12))
#else
        CALL(evt_msg_print_add, 0, STRING("badgeshop_06"))
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("badgeshop_07"))
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(8))
    LABEL(0)
#endif
    CALL(evt_win_other_select, SELECT_TYPE_BADGE_SELL)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_win_coin_off, LW(12))
        RETURN
    END_IF
    CALL(evt_msg_print_insert, 0, STRING("badgeshop_08"), 0, STRING("バッジ店員"), LW(2), LW(3))
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 1)
        GOTO(0)
    END_IF
    CALL(evt_pouch_remove_item, LW(1), LW(0))
    CALL(evt_badgeShop_throw_inc, LW(1))
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(12))
    CALL(evt_pouch_get_havebadgecnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("badgeshop_11"), 0, STRING("バッジ店員"))
        RETURN
    END_IF
    CALL(evt_msg_print, 0, STRING("badgeshop_10"), 0, STRING("バッジ店員"))
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
        RETURN
    END_IF
#else
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(evt_sub_get_language, LW(0))
#if VERSION >= VERSION_US
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_08"), 0, STRING("バッジ店員"), LW(2), LW(3))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("badgeshop_08"), LW(3))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
        CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("バッジ店員"))
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
    CALL(evt_pouch_remove_item_index, LW(1), LW(4), LW(0))
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
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(8))
#endif
    GOTO(0)
    RETURN
EVT_END()

/**
 * Badge shop purchase menu for the stock ("zaiko") of ordinary badges:
 * tells Mario when the shelves are empty, otherwise lets him pick a badge, checks his coins and badge room,
 * adds the badge and takes the price, and offers to buy more until he declines or the stock runs out.
 * The text formatting differs per release and language.
 */
EVT_BEGIN(zaiko_buy_evt)
    CALL(evt_badgeShop_throw_get_kind_cnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print, 0, STRING("badgeshop_37"), 0, STRING("バッジ店員"))
        RETURN
    END_IF
    LABEL(0)
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(12))
#else
        CALL(badgeshop_special_cnt, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("badgeshop_37"))
        ELSE
            CALL(evt_msg_print_add, 0, STRING("badgeshop_40"))
        END_IF
        RETURN
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(8))
    LABEL(0)
#endif
    CALL(evt_win_other_select, SELECT_TYPE_BADGE_BUY)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_win_coin_off, LW(12))
        RETURN
    END_IF
    CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(2), LW(3), LW(4))
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 1)
#else
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(evt_sub_get_language, LW(0))
#if VERSION >= VERSION_US
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(2), LW(3), LW(4))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), STRING("badgeshop_00"), LW(3))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
        CALL(evt_msg_print_insert, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("バッジ店員"), LW(4))
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
#endif
        GOTO(0)
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(3))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print, 0, STRING("badgeshop_01"), 0, STRING("バッジ店員"))
        CALL(evt_win_coin_off, LW(12))
#else
        CALL(evt_msg_print_add, 0, STRING("badgeshop_01"))
        CALL(evt_win_coin_off, LW(8))
#endif
        RETURN
    END_IF
    CALL(evt_pouch_add_item, LW(1), LW(0))
    IF_INT_EQUAL(LW(0), -1)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print, 0, STRING("badgeshop_02"), 0, STRING("バッジ店員"))
        CALL(evt_win_coin_off, LW(12))
#else
        CALL(evt_msg_print_add, 0, STRING("badgeshop_02"))
        CALL(evt_win_coin_off, LW(8))
#endif
        RETURN
    END_IF
    CALL(evt_badgeShop_throw_dec, LW(1))
    MULTIPLY(LW(3), -1)
    CALL(evt_pouch_add_coin, LW(3))
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(12))
    CALL(evt_badgeShop_throw_get_kind_cnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("badgeshop_11"), 0, STRING("バッジ店員"))
        RETURN
    END_IF
    CALL(evt_msg_print, 0, STRING("badgeshop_38"), 0, STRING("バッジ店員"))
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
        RETURN
    END_IF
#else
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
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(8))
#endif
    GOTO(0)
    RETURN
EVT_END()

/**
 * The badge shop's buy menu: the clerk greets (badgeshop_36) and Mario picks one of three choices.
 * Choice 0 says badgeshop_05 in demos; later releases choose badgeshop_05, _37 or _39 according to stock.
 * Choice 1 runs zaiko_buy_evt (the stock list) and choice 2 leaves with badgeshop_22.
 */
static EVT_BEGIN(buy_evt)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_msg_print, 0, STRING("badgeshop_36"), 0, STRING("バッジ店員"))
#else
    CALL(evt_msg_print_add, 0, STRING("badgeshop_36"))
#endif
    CALL(evt_msg_select, 0, STRING("badgeshop_35"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_continue)
        CALL(evt_msg_print, 0, STRING("badgeshop_05"), 0, STRING("バッジ店員"))
#else
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
#endif
        CASE_EQUAL(1)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_continue)
        CALL_CHILD(zaiko_buy_evt)
#else
        CALL_CHILD(zaiko_buy_evt)
#endif
        CASE_EQUAL(2)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_continue)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
#else
        CALL(evt_msg_print_add, 0, STRING("badgeshop_22"))
#endif
    END_SWITCH
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(first_shopping_evt)
    CALL(evt_msg_print, 0, STRING("badgeshop_30"), 0, STRING("バッジ店員"))
    RETURN
EVT_END()
#endif
EVT_BEGIN(badgeshopper_talk_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(unitwin_get_work_ptr, LW(10))
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF(2), 0)
        CALL_CHILD(first_shopping_evt)
        SET_VALUE(GSWF(2), 1)
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
#endif
    CALL(evt_msg_print, 0, STRING("badgeshop_04"), 0, STRING("バッジ店員"))
    CALL(evt_msg_select, 0, STRING("badgeshop_23"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_continue)
        CALL_CHILD(buy_evt)
#else
        CALL_CHILD(buy_evt)
#endif
        CASE_EQUAL(1)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_continue)
        CALL_CHILD(sell_evt)
#else
        CALL_CHILD(sell_evt)
#endif
        CASE_EQUAL(2)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_continue)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
#else
        CALL(evt_msg_print_add, 0, STRING("badgeshop_22"))
#endif
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Init event of the badge shop owner: hides him below the map while Trouble Center job 16 is active or solved,
 * and while Mario is in Glitzville (Chapter 3, up to Ms. Mowz).
 */
EVT_BEGIN(badgemaster_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ELUSIVE_BADGE)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_IF
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_ELUSIVE_BADGE), 1)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_00_WELCOME_TO_GLITZVILLE)
        IF_INT_LESS_OR_EQUAL(GSW_STORY_PROGRESS, STG3_20_08_MS_MOWZ)
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        END_IF
    END_IF
    RETURN
EVT_END()

/** Talk event of the badge shop owner: one line per story stage (gor_01_024_00 to gor_01_024_08). */
EVT_BEGIN(badgemaster_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_024_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_024_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_024_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_024_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_024_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_024_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_024_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_024_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_024_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the old Toad ("kinoji"): empty. */
EVT_BEGIN(kinoji_init)
    RETURN
EVT_END()

/** Talk event of the old Toad: one line per story stage (gor_01_019 to gor_01_019_08). */
EVT_BEGIN(kinoji_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_019"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_019_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_019_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_019_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_019_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_019_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_019_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_019_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_019_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Watches for Mario walking up to Zess T. ("Nancy") in the western alley before the contact lens quest:
 * when he enters the box near x -580 to -400 she stops pacing
 * and the first scene runs (gor_01_000 to gor_01_007), where she turns away, Mario crushes her lost
 * contact lens and she ends up blocking the way. Sets GSWF_GOR_ZESS_T_BLOCKING_WEST_GATE.
 */
EVT_BEGIN(nancy_init_check)
    CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)

    // Wait until Mario walks into the alley zone in front of Zess T.
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        SET_VALUE(LW(3), 0)
#if VERSION <= VERSION_DEMO_JP
        IF_INT_GREATER(LW(10), -575)
#else
        IF_INT_GREATER(LW(10), -580)
#endif
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

    // Mario's controls are taken; Zess T. stops pacing and everyone turns to face her.
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

    // She yells at them to freeze.
    CALL(evt_msg_print, 0, STRING("gor_01_000"), 0, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), LW(2))
    ADD(LW(1), 335)
    ADD(LW(2), -8)
    IF_INT_LESS_OR_EQUAL(LW(0), -520)
        SET_VALUE(LW(0), -520)
    END_IF
    CALL(evt_cam3d_evt_set, LW(0), 99, LW(1), LW(0), 39, LW(2), 500, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    SET_VALUE(LW(3), 90)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(0), LW(10))
        ADD(LW(3), 180)
    END_IF
    CALL(evt_npc_set_ry, STRING("me"), LW(3))

    // She orders them not to move: someone bumped into her and she lost a contact lens. She paces and keeps
    // hissing the order.
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
    CALL(evt_cam3d_evt_off, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)

    // Control returns to Mario, who is expected to stay put; the script waits until he leaves -550 < x < -400
    // or one second passes, then waits for a non-idle state before taking control again.
    CALL(evt_mario_key_onoff, 1)
#if VERSION >= VERSION_US
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

    // After the early demos, if Flurrie is current, wait until partner use ends before Mario resumes his normal pose.
#if VERSION >= VERSION_JP
    CALL(evt_mario_get_party, LW(0))
    IF_INT_EQUAL(LW(0), PARTY_MEMBER_FLURRIE)
        LOOP(0)
            CALL(evt_mario_party_use_check, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_IF
#endif
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))

    // Mario steps on the lens: it breaks with a squish.
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG0_LENS_BREAK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
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
    CALL(evt_cam3d_evt_set, LW(0), 80, LW(1), LW(0), 40, LW(2), 500, EASING_SINE_IN_OUT)
    WAIT_MS(400)
    CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))

    // Zess T. screams that Mario crushed her lens and storms toward him.
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
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
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

    // She blames Mario and demands he pay for a new lens while he cowers.
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

    // She walks back to the alley's west end to block the way.
    CALL(evt_npc_move_position, STRING("me"), -573, 56, 0, FLOAT(180.0f), 0)
    WAIT_WHILE(GF(0))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    WAIT_MS(300)

    // She announces she will block the west gate until Mario brings her a new contact lens.
    CALL(evt_msg_print, 0, STRING("gor_01_007"), 0, STRING("me"))
    CALL(evt_npc_restart_regular_event, STRING("me"))
    CALL(evt_npc_change_interrupt, STRING("me"), 6, PTR(nancy_talk))
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
#if VERSION < VERSION_US
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
#endif
    CALL(evt_mario_key_onoff, 1)

    // Story flag set: the gate to the west side is now blocked.
    SET_VALUE(GSWF_GOR_ZESS_T_BLOCKING_WEST_GATE, 1)
    RETURN
EVT_END()

/**
 * Init event of Zess T. ("Nancy"): before the lens is delivered she paces near (-500, 0, 75)
 * and the check event is started; once she blocks the gate she stands at (-573, 0, 56);
 * after the lens is delivered she stands at her door at (-405, 10, -100).
 * Also switches the west exit "w_bero" off until the lens is delivered.
 */
EVT_BEGIN(nancy_init)
    SET_VALUE(LSWF_GOR_01_ZESS_T_GREETED, 0)
    IF_INT_EQUAL(GSWF_GOR_ZESS_T_GOT_CONTACT_LENS, 0)
        IF_INT_EQUAL(GSWF_GOR_ZESS_T_BLOCKING_WEST_GATE, 0)
            CALL(evt_npc_set_ry, STRING("me"), 270)
            CALL(evt_npc_set_position, STRING("me"), -500, 0, 75)
            SPAWN_EVENT(nancy_init_check)
        ELSE
            CALL(evt_npc_set_ry, STRING("me"), 90)
            CALL(evt_npc_set_position, STRING("me"), -573, 0, 56)
        END_IF
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_hitobj_attr_onoff, 1, 0, STRING("w_bero"), 8388612)
    ELSE
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_set_position, STRING("me"), -405, 10, -100)
    END_IF
    RETURN
EVT_END()

/**
 * Animation of Zess T.'s front door ("S_ie_01_doa") opening to 108 degrees over 500 ms, pausing 500 ms,
 * then closing over 500 ms, with door sounds. Shows the interior during the animation, hides it afterwards,
 * and clears GF(0) when done.
 */
EVT_BEGIN(nancy_door_anime)
    CALL(evt_mapobj_get_position, STRING("S_ie_01_naka"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_01_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 108, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 108, 0, 500)
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
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_01_naka"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(GF(0), 0)
    RETURN
EVT_END()

/**
 * Regular event of Zess T.: before the story step she wanders around (-500, 75);
 * while she blocks the gate (lens not delivered) she keeps following Mario with nancy_block_func every frame.
 */
EVT_BEGIN(nancy_regl)
    IF_INT_EQUAL(GSWF_GOR_ZESS_T_BLOCKING_WEST_GATE, 0)
        IF_INT_EQUAL(GSWF_GOR_ZESS_T_GOT_CONTACT_LENS, 0)
            CALL(urouro_init_func, STRING("me"), -500, 75, FLOAT(50.0f), FLOAT(45.0f), 0)
            CALL(urouro_main_func, STRING("me"))
        END_IF
    ELSE
        IF_INT_EQUAL(GSWF_GOR_ZESS_T_GOT_CONTACT_LENS, 0)
            CALL(nancy_block_init_func)
            LOOP(0)
                CALL(nancy_block_func)
                WAIT_FRAMES(1)
            END_LOOP
        END_IF
    END_IF
    RETURN
EVT_END()

/** Items offered in the item menu when giving Zess T. something: only the Contact Lens. */
s32 item_tbl_nancy[] = {
    ITEM_CONTACT_LENS, -1,
};

/**
 * Talk event of Zess T. Before the contact lens is delivered,
 * Mario can hand over a Contact Lens (the item list item_tbl_nancy);
 * she then thanks him, walks into her house, the west exit opens and GSWF_GOR_ZESS_T_GOT_CONTACT_LENS is set.
 * After that talking to her runs cooking_evt.
 */
EVT_BEGIN(nancy_talk)
    IF_INT_EQUAL(GSWF_GOR_ZESS_T_GOT_CONTACT_LENS, 0)
        CALL(evt_pouch_check_item, ITEM_CONTACT_LENS, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(evt_msg_print, 0, STRING("gor_01_008"), 0, STRING("me"))
#if VERSION <= VERSION_DEMO_JP
            CALL(evt_win_item_select, PTR(item_tbl_nancy), LW(1))
#else
            CALL(evt_win_item_select, 0, PTR(item_tbl_nancy), LW(1), 0)
#endif
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
            SPAWN_EVENT(nancy_door_anime)
            WAIT_MS(500)
            CALL(evt_npc_move_position, STRING("me"), -370, -30, 0, FLOAT(80.0f), 0)
            WAIT_WHILE(GF(0))
#if VERSION >= VERSION_US
            CALL(evt_npc_set_position, STRING("me"), -405, 10, -100)
#else
            CALL(evt_npc_set_position, STRING("me"), -455, 10, -170)
#endif
            CALL(evt_npc_set_ry, STRING("me"), 270)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            SET_VALUE(LW(0), STRING("w_bero"))
#if VERSION >= VERSION_US
            CALL_CHILD(bero_case_switch_on)
#else
            CALL_CHILD(bero_case_switch_on)
#endif
            CALL(evt_hitobj_attr_onoff, 1, 1, STRING("w_bero"), 8388612)
            SET_VALUE(GSWF_GOR_ZESS_T_GOT_CONTACT_LENS, 1)
            CALL(evt_door_set_param, GW(6), 0, PTR(&door_data_01_npc))
        END_IF
    ELSE
        CALL_CHILD(cooking_evt)
    END_IF
    RETURN
EVT_END()

/**
 * Init event of Flavio (Marco) in the central plaza:
 * from STG4_32_CHATTING_WITH_FLAVIO to STG5_59_ROGUEPORT_DOCK he is not here (he is at the harbor)
 * and is moved below the map.
 */
static EVT_BEGIN(marco_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_32_CHATTING_WITH_FLAVIO, STG5_59_ROGUEPORT_DOCK)
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk event of Flavio (Marco) in the plaza.
 * Before Frankly's house in Chapter 4 he says mac_4_005
 * and goes back to muttering to himself (marco_hitorigoto).
 * At STG4_31_FRANKLYS_HOUSE_STG_4 Mario and Flavio have the long talk (mac_4_007 to mac_4_023):
 * the camera frames them, Mario answers a question,
 * Mario shows the Magical Map (ITEM_MAGICAL_MAP_LARGE) and Flavio talks about the Skull Gem,
 * and the story advances to STG4_32_CHATTING_WITH_FLAVIO. Later stages only give short lines.
 */
static EVT_BEGIN(marco_talk)
    DELETE_EVENT(GW(0))
    CALL(evt_msg_close, STRING("mac_4_004"))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG4_31_FRANKLYS_HOUSE_STG_4)

        // Before Frankly's house: Flavio is startled, introduces himself and goes back to muttering to himself.
        CALL(evt_msg_print, 0, STRING("mac_4_005"), 0, STRING("me"))
        SET_VALUE(GSWF_GOR_FLAVIO_FIRST_TALK_DONE, 1)
        SPAWN_EVENT_GET_ID(PTR(marco_hitorigoto), GW(0))
        CASE_EQUAL(STG4_31_FRANKLYS_HOUSE_STG_4)

        // Chapter 4 after Frankly's house: Mario and Flavio have the long talk. The camera frames them.
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        ADD(LW(0), LW(10))
        DIVIDE(LW(0), 2)
        CALL(evt_cam3d_evt_set, LW(0), 92, 128, LW(0), 40, -168, 500, EASING_SINE_IN_OUT)
        WAIT_MS(500)
        IF_INT_EQUAL(GSWF_GOR_FLAVIO_FIRST_TALK_DONE, 0)
            // Flavio introduces himself as a trader and asks Mario what Flavio's life lacks; the answer only
            // changes his reply.
            CALL(evt_msg_print, 0, STRING("mac_4_007"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("mac_4_008"), 0, STRING("me"))
        END_IF
        CALL(evt_msg_select, 0, STRING("mac_4_007_select"))
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
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

        // Flavio walks along the plaza, musing; the partner and Mario step up to him.
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_npc_get_position, STRING("me"), LW(3), LW(1), LW(2))
        SET_VALUE(LW(11), LW(3))
        SET_VALUE(LW(12), LW(3))
        CALL(evt_npc_set_ry, STRING("me"), 270)
        SUBTRACT(LW(11), 70)
        SUBTRACT(LW(12), 50)
        CALL(evt_cam3d_evt_set, LW(11), 63, 152, LW(11), 45, -169, 1000, EASING_SINE_IN_OUT)
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

        // Flavio is struck by the legend of Cortez's treasure on Keelhaul Key and decides to find it.
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

        // Flavio asks about Mario's own treasure hunt and first brushes it off.
        CALL(evt_msg_print, 0, STRING("mac_4_016"), 0, STRING("me"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_2"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_017"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)

        // Mario shows the treasure map; Flavio grabs it and recognises the way to Keelhaul Key.
        CALL(evt_msg_print, 0, STRING("mac_4_018"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_C_3"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 20)
        ADD(LW(1), 20)
        ADD(LW(2), 10)
        CALL(evt_item_entry, STRING("map00"), ITEM_MAGICAL_MAP_LARGE, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
        CALL(evt_item_flag_on, STRING("map00"), ITEM_FLAG_NO_TOUCH_GET)
        CALL(evt_item_set_scale, STRING("map00"), FLOAT(0.5498047f))
        WAIT_MS(500)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), -10)
        CALL(evt_npc_move_position, STRING("me"), LW(3), LW(2), 0, FLOAT(80.0f), 0)
        WAIT_MS(1000)

        // Flavio accuses Mario of stealing his treasure, then calms down when Mario explains the Crystal Stars.
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

        // Flavio hears there is no ship, laughs, then eagerly volunteers to lead the expedition.
        CALL(evt_msg_print, 0, STRING("mac_4_021"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_022"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_023"), 0, STRING("me"))
        CALL(evt_npc_set_ry, STRING("me"), 90)
        WAIT_MS(500)

        // The Skull Gem's field item is deleted and Flavio hurries off; he leaves the plaza.
        CALL(evt_del_case_evt, 0, GW(8))
        CALL(evt_item_delete, STRING("ドクロジュエル"))
        WAIT_MS(500)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        WAIT_MS(500)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_move_position, STRING("me"), 260, -100, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_move_position, STRING("me"), 110, -100, 0, FLOAT(120.0f), 0)
            CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        END_INLINE_EVENT
        WAIT_MS(3000)

        // Story advances: the expedition to Keelhaul Key is being arranged.
        SET_VALUE(GSW_STORY_PROGRESS, STG4_32_CHATTING_WITH_FLAVIO)
        CASE_LESS(STG6_54_WELCOME_BACK)

        // Later stages: Flavio only has a short line.
        CALL(evt_msg_print, 0, STRING("gor_01_031_08_1"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_031_08_2"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_031_08_3"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_031_08_4"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of the bar owner (Podley, "master"):
 * during Professor Frankly's house step (STG4_31) he stands at (259, 0, -275).
 */
EVT_BEGIN(master_init)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_31_FRANKLYS_HOUSE_STG_4)
        CALL(evt_npc_set_position, STRING("me"), 259, 0, -275)
    END_IF
    RETURN
EVT_END()

/**
 * Waits until Mario is 100 units or more away from the bar owner,
 * then makes him mutter irai_20_030 (Trouble Center job 20).
 */
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

/**
 * Talk event of the bar owner (Podley).
 * Handles Trouble Center jobs 20 and 25, then by story: through STG4_32 the same early line;
 * after Admiral Bobbery's refusal (STG4_34) he tells Mario the story of Bobbery and, if Mario wants,
 * gives him the Old Letter and advances to STG4_35_ADVICE_FROM_PODLEY; afterwards a line per stage.
 */
EVT_BEGIN(master_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_TELL_THAT_PERSON)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
        ELSE
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                CALL(evt_msg_print, 0, STRING("irai_20_027"), 0, STRING("me"))
                CALL(evt_mario_set_pose, STRING("M_I_2"))
                WAIT_MS(1000)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                CALL(evt_cam3d_evt_set, 320, 99, -12, 322, 43, -271, 1000, EASING_SINE_IN_OUT)
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("irai_20_028"), 0, STRING("me"))
                CALL(evt_mario_set_pose, STRING("M_N_5B"))
                WAIT_MS(500)
                CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
                WAIT_MS(1000)
                CALL(evt_msg_print, 0, STRING("irai_20_029"), 0, STRING("me"))
                SPAWN_EVENT(iri_20_master_tubuyaki)
                SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
                RETURN
            ELSE
                CALL(evt_msg_print, 0, STRING("irai_20_031"), 0, STRING("me"))
                RETURN
            END_IF
        END_IF
    END_IF

    // Trouble Center job 25: Podley says General White was just here.
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(3), 0)
                CALL(evt_msg_print, 0, STRING("irai_25_00_1"), 0, 0)
                SET_VALUE(GSWF_TROUBLE_WORK(2), 1)
                RETURN
            END_IF
        END_IF
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS_OR_EQUAL(STG4_32_CHATTING_WITH_FLAVIO)

        // Normal talk by story progress: the same line through STG4_32, then at STG4_33_MEET_BOBBERY he tells Mario where Bobbery's house is.
        CALL(evt_msg_print, 0, STRING("gor_01_016"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG4_33_MEET_BOBBERY)
        CALL(evt_msg_print, 0, STRING("mac_4_047"), 0, STRING("me"))
        SET_VALUE(GSWF_GOR_BOBBERY_HOME_LEARNED, 1)
        CASE_EQUAL(STG4_34_BOBBERY_DECLINES)

        // After Bobbery declines the voyage: Mario and the partner step up to Podley's bar.
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

        // If Podley already told the tale, he only asks again whether they want Bobbery back on the sea.
        IF_INT_EQUAL(GSWF_GOR_PODLEY_BOBBERY_STORY_TOLD, 1)
            CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
            CALL(evt_msg_print, 0, STRING("mac_4_071"), 0, STRING("me"))
            GOTO(100)
        END_IF
        IF_INT_EQUAL(GSWF_GOR_PODLEY_BOBBERY_TALK_DONE, 0)

            // Podley asks if they really want Bobbery on the water and offers to tell his sad tale.
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
            SET_VALUE(GSWF_GOR_PODLEY_BOBBERY_TALK_DONE, 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("mac_4_064"), 0, STRING("me"))
        END_IF
        CALL(evt_msg_select, 0, STRING("mac_4_063_yesno"))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_msg_print_add, 0, STRING("mac_4_065"))
            RETURN
        END_IF

        // Podley tells the tale of Bobbery and his wife Scarlette, with the camera pushed in.
        CALL(evt_msg_print_add, 0, STRING("mac_4_066"))
        CALL(evt_cam3d_evt_set, 300, 95, 27, 300, 43, -268, 1500, EASING_SINE_IN_OUT)
        CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
        WAIT_MS(1500)
        CALL(evt_msg_print, 0, STRING("mac_4_067"), 0, STRING("me"))
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, STRING("mac_4_068"), 0, STRING("me"))
        WAIT_MS(1000)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(1000)
#if VERSION >= VERSION_DEMO_EU
        CALL(evt_party_stop, 0)
        CALL(evt_msg_print_party, STRING("mac_4_069"))
        CALL(evt_party_run, 0)
#else
        CALL(evt_msg_print_party, STRING("mac_4_069"))
#endif
        SET_VALUE(GSWF_GOR_PODLEY_BOBBERY_STORY_TOLD, 1)
        CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)

        // Podley asks whether they still want Bobbery at sea.
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

        // Podley agrees and walks out from behind the bar, jumping the counter.
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

        // Mario receives the Old Letter.
        CALL(evt_item_entry, STRING("letter"), ITEM_OLD_LETTER, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
        CALL(evt_item_flag_on, STRING("letter"), ITEM_FLAG_BGM_SILENT | ITEM_FLAG_NO_TOUCH_GET)
        CALL(evt_item_get_item, STRING("letter"))
        CALL(evt_cam3d_evt_set, 300, 95, 27, 300, 43, -268, 500, EASING_SINE_IN_OUT)
        WAIT_MS(800)

        // Podley explains that it is Scarlette's last letter and asks Mario to take it to Bobbery.
        CALL(evt_msg_print, 0, STRING("mac_4_075"), 0, STRING("me"))
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        WAIT_MS(800)
        CALL(evt_msg_print, 0, STRING("mac_4_076"), 0, STRING("me"))
#if VERSION >= VERSION_DEMO_EU
        CALL(evt_party_stop, 0)
        CALL(evt_msg_print_party, STRING("mac_4_077"))
        CALL(evt_party_run, 0)
#else
        CALL(evt_msg_print_party, STRING("mac_4_077"))
#endif
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_2"))
        WAIT_MS(800)

        // Story advances: Podley's advice has been given.
        SET_VALUE(GSW_STORY_PROGRESS, STG4_35_ADVICE_FROM_PODLEY)
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 1000)
        CALL(evt_snd_bgmon_f, PSND_CONTINUE | PSND_FADEIN, 0, 1000)
        CASE_EQUAL(STG4_35_ADVICE_FROM_PODLEY)

        // Later stages: a short line, then a line per stage of the story.
        CALL(evt_msg_print, 0, STRING("mac_4_076"), 0, STRING("me"))
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_016"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_016_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_016_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_016_03"), 0, STRING("me"))
        CASE_LESS(STG4_32_CHATTING_WITH_FLAVIO)
        CALL(evt_msg_print, 0, STRING("gor_01_016_04"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_016_05"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_016_06"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_016_07"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_016_08"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_016_09"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(shoptender_init)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_32_CHATTING_WITH_FLAVIO)
        IF_INT_EQUAL(GSWF_GOR_CONTACT_LENS_ARRIVED, 1)
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
    IF_INT_EQUAL(GSWF_GOR_ZESS_T_GOT_CONTACT_LENS, 0)
        DEBUG_PUT_MSG(STRING("店員TALK２"))
        IF_INT_EQUAL(GSWF_GOR_CONTACT_LENS_ARRIVED, 1)
            CALL(evt_pouch_check_item, ITEM_CONTACT_LENS, LW(0))
            IF_INT_LESS(LW(0), 1)
                CALL(evt_msg_print, 0, STRING("gor_01_015"), 0, STRING("me"))
            ELSE
                CALL(evt_msg_print, 0, STRING("gor_01_015_01"), 0, STRING("me"))
            END_IF
            RETURN
        END_IF
        SET_VALUE(LW(0), GSWF_GOR_CONTACT_LENS_ORDERED_EARLY)
        ADD(LW(0), GSWF_GOR_CONTACT_LENS_ORDERED_LATE)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("gor_01_014"), 0, STRING("me"))
            RETURN
        END_IF
        CALL(evt_msg_print, 0, STRING("gor_01_012"), 0, STRING("me"))
        IF_INT_EQUAL(GSWF_GOR_ZESS_T_BLOCKING_WEST_GATE, 1)
            CALL(evt_mario_set_pose, STRING("M_I_2"))
            WAIT_MS(700)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
            CALL(evt_msg_print, 0, STRING("gor_01_013"), 0, STRING("me"))
            CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_cam3d_evt_set, 358, 123, -301, -189, 75, -301, 2600, EASING_SINE_IN_OUT)
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
            CALL(evt_cam3d_evt_off, 2600, EASING_SINE_IN_OUT)
            CALL(evt_npc_set_ry, STRING("me"), 180)
            CALL(evt_npc_move_position, STRING("me"), LW(10), LW(12), 0, FLOAT(90.0f), 0)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
            WAIT_MS(500)
            CALL(evt_msg_print, 0, STRING("gor_01_013_01"), 0, STRING("me"))
            IF_INT_LESS(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
                SET_VALUE(GSWF_GOR_CONTACT_LENS_ORDERED_EARLY, 1)
            ELSE
                SET_VALUE(GSWF_GOR_CONTACT_LENS_ORDERED_LATE, 1)
            END_IF
            CALL(evt_sub_save_playtime, GSW_GOR_CONTACT_LENS_ORDER_TIME)
        END_IF
    ELSE
        DEBUG_PUT_MSG(STRING("店員TALK３"))
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
            CALL(evt_msg_print, 0, STRING("gor_01_015_02"), 0, STRING("me"))
            CASE_LESS(STG3_32_BOSS_BATTLE)
            CALL(evt_msg_print, 0, STRING("gor_01_015_03"), 0, STRING("me"))
            CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
            CALL(evt_msg_print, 0, STRING("gor_01_015_04"), 0, STRING("me"))
            CASE_LESS(STG5_43_X_NAUTS_RETREAT)
            CALL(evt_msg_print, 0, STRING("gor_01_015_05"), 0, STRING("me"))
        END_SWITCH
    END_IF
    RETURN
EVT_END()

/** Init event of the inn clerk: empty. */
EVT_BEGIN(yadotender_init)
    RETURN
EVT_END()

/** Talk event of the inn clerk: empty. */
EVT_BEGIN(yadotender_talk)
    RETURN
EVT_END()

/** Init event of the first street-stall seller: marks his status and lets him walk through walls. */
EVT_BEGIN(roten1_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

/**
 * Regular event of the first street-stall seller:
 * loops forever walking between x=450 and x=-350 at z=150, pausing a second at each end.
 */
EVT_BEGIN(roten1_regl)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("me"), 450, 150, 0, FLOAT(32.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING("me"), -350, 150, 0, FLOAT(32.0f), 0)
        WAIT_MS(1000)
    END_LOOP
    RETURN
EVT_END()

/** Talk event of the first street-stall seller: one line per story stage (gor_01_025_00 to gor_01_025_08). */
EVT_BEGIN(roten1_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_025_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_025_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_025_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_025_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_025_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_025_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_025_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_025_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_025_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the second street-stall seller: lets him walk through walls. */
EVT_BEGIN(roten2_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

/** Talk event of the second street-stall seller: one line per story stage (gor_01_025_00_2 to gor_01_025_08_2). */
EVT_BEGIN(roten2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_025_00_2"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_025_01_2"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_025_02_2"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_025_03_2"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_025_04_2"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_025_05_2"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_025_06_2"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_025_07_2"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_025_08_2"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the first Goomba villager: marks his status and lets him walk through walls. */
EVT_BEGIN(kuribo1_init)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

/**
 * Regular event of the first Goomba villager: loops forever walking between x=375 and x=-375 at z=390,
 * pausing a second at each end.
 */
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
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_FIND_THIS_GUY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_04_001"), 0, STRING("me"))
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
        ELSE
            CALL(evt_msg_print, 0, STRING("irai_04_002"), 0, STRING("me"))
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_026_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_026_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_026_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_026_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_026_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_026_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_026_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_026_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_026_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the second Goomba villager: lets him walk through walls. */
EVT_BEGIN(kuribo2_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    RETURN
EVT_END()

/**
 * Regular event of the second Goomba villager: wanders with gor_villager_urouro.
 * npcEnt does not use it (his regularEvent is NULL).
 */
EVT_BEGIN(kuribo2_regl)
    CALL_CHILD(gor_villager_urouro)
    RETURN
EVT_END()

EVT_BEGIN(kuribo2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_027_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_027_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_027_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_027_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_027_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_027_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_027_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_027_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_027_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the third Goomba villager: empty. */
EVT_BEGIN(kuribo3_init)
    RETURN
EVT_END()

/** Regular event of the third Goomba villager: wanders with gor_villager_urouro. */
EVT_BEGIN(kuribo3_regl)
    CALL_CHILD(gor_villager_urouro)
    RETURN
EVT_END()

EVT_BEGIN(kuribo3_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_SAFE_DELIVERY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
                CALL(evt_msg_print, 0, STRING("irai_02_00"), 0, STRING("me"))
                CALL(evt_item_entry, STRING("irai_item"), ITEM_BOX, 0, -1000, 0, ITEM_TYPE_STILL, -1, 0)
                CALL(evt_item_flag_on, STRING("irai_item"), ITEM_FLAG_NO_TOUCH_GET)
                CALL(evt_item_get_item, STRING("irai_item"))
                SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            ELSE
                CALL(evt_msg_print, 0, STRING("irai_02_01"), 0, STRING("me"))
            END_IF
        ELSE
            CALL(evt_msg_print, 0, STRING("irai_02_02"), 0, STRING("me"))
            CALL(evt_sub_get_coin, 20)
            SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
            SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_SAFE_DELIVERY), 1)
            CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
        END_IF
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_028_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_028_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_028_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_028_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_028_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_028_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_028_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_028_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_028_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init event of "Chusan" (Mr. Chu, the first NPC);
 * during Trouble Center job 5 he stands at (-80, 0, 40) and chusan1_naguru is started.
 */
EVT_BEGIN(chusan1_init)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HIT_ME_PLEASE)
        CALL(evt_npc_set_position, STRING("me"), -80, 0, 40)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        SET_VALUE(GF(9), 1)
        SPAWN_EVENT(chusan1_naguru)
    END_IF
    RETURN
EVT_END()

/**
 * Regular event of Chusan: wanders with gor_villager_urouro, except during Trouble Center job 5,
 * when he stands still.
 */
EVT_BEGIN(chusan1_regl)
    IF_INT_NOT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HIT_ME_PLEASE)
        DEBUG_PUT_MSG(STRING("マルチュロ：普段のうろうろ"))
        CALL_CHILD(gor_villager_urouro)
    END_IF
    DEBUG_PUT_MSG(STRING("マルチュロ：依頼時は何もしない"))
    RETURN
EVT_END()

/**
 * Interrupt of Chusan during Trouble Center job 5, fired each time Mario hits him:
 * plays a hit sound and one line per hit count (irai_05_004 to irai_05_014; the first ten are the same text,
 * the 11th differs), then lets him recover.
 */
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
        SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
        SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_HIT_ME_PLEASE), 1)
        CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    ELSE
        CALL(evt_msg_print, 0, STRING("irai_05_016"), 0, STRING("チューさん１"))
        SET_VALUE(GF(9), 1)
        SPAWN_EVENT(chusan1_naguru)
    END_IF
    CALL(chusan1_alwaysencont, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(chusan1_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HIT_ME_PLEASE)
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
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_05_001"), 0, STRING("me"))
            // BUG: the flag is set to 0, not 1, so the ELSE line (irai_05_002) never prints.
            SET_VALUE(GSWF_TROUBLE_WORK(0), 0)
        ELSE
            CALL(evt_msg_print, 0, STRING("irai_05_002"), 0, STRING("me"))
        END_IF
        CALL(evt_msg_print, 0, STRING("irai_05_003"), 0, STRING("me"))
        SET_VALUE(GW(9), 0)
        SET_VALUE(GF(9), 0)
        RETURN
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_029_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_029_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_029_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_029_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_029_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_029_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_029_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_029_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_029_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the second Chusan: empty. */
EVT_BEGIN(chusan2_init)
    RETURN
EVT_END()

EVT_BEGIN(chusan2_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_030_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_030_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_030_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_030_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_030_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_030_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_030_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_030_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_030_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of the Bob-omb villager: does nothing. */
static EVT_BEGIN(bomhei_init)
    RETURN
EVT_END()

EVT_BEGIN(bomhei_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("gor_01_031_00"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_031_01"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("gor_01_031_02"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("gor_01_031_03"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("gor_01_031_04"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("gor_01_031_05"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("gor_01_031_06"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("gor_01_031_07"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("gor_01_031_08"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(bottakuru_init)
#if VERSION <= VERSION_DEMO_JP
    IF_INT_LESS(GSW_STORY_PROGRESS, STG2_25_AFTER_THE_BOSS_BATTLE)
#else
    IF_INT_LESS(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
#endif
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
        RETURN
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_WALL_COLLISION)
#if VERSION >= VERSION_JP
    CALL(evt_npc_set_ry, STRING("me"), 90)
#endif
    SET_VALUE(GSWF_GOR_CHARLIETON_BOUGHT_THIS_VISIT, 0)
    RETURN
EVT_END()

/**
 * Talk event of Charlieton, who sells special badges and items at high prices from the shop's special stock:
 * greets Mario, then loops through a buy menu (coins checked, item added to the pouch, stock decremented)
 * until Mario declines or the stock is empty. The US and PAL releases format prices with the message formatter.
 */
EVT_BEGIN(bottakuru_talk)
    // Charlieton is sold out, or has nothing Mario can still buy: he only thanks him.
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_badgeShop_bottakuru_get_kind_cnt, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("bottakuru_03"), 0, STRING("me"))
        RETURN
#else
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
#endif
    END_IF

    // Charlieton's sales pitch (bottakuru_00 on the first visit, bottakuru_01 later, bottakuru_02 after a purchase
    // this visit), then he asks if Mario wants to shop.
    IF_INT_EQUAL(GSWF_GOR_CHARLIETON_FIRST_TALK_DONE, 0)
        CALL(evt_msg_print, 0, STRING("bottakuru_00"), 0, STRING("me"))
        SET_VALUE(GSWF_GOR_CHARLIETON_FIRST_TALK_DONE, 1)
    ELSE
        IF_INT_EQUAL(GSWF_GOR_CHARLIETON_BOUGHT_THIS_VISIT, 0)
            CALL(evt_msg_print, 0, STRING("bottakuru_01"), 0, STRING("me"))
        ELSE
            CALL(evt_msg_print, 0, STRING("bottakuru_02"), 0, STRING("me"))
        END_IF
    END_IF

    // Asked whether to shop: if Mario says no, Charlieton says goodbye.
    CALL(evt_msg_select, 0, STRING("bottakuru_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("bottakuru_04"))
        RETURN
    END_IF
    CALL(evt_msg_continue)

    // Shopping loop: the stock window opens; backing out ends the visit, and too few coins sends Mario back to
    // the list.
    LABEL(0)
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(12))
    CALL(evt_win_other_select, SELECT_TYPE_BADGE_CHARLIETON)
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

    // Charlieton quotes the price of the chosen item; for a badge (id 240 and up) he also gives its BP cost.
    IF_INT_GREATER_OR_EQUAL(LW(1), BADGE_MIN)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print_insert, 0, STRING("bottakuru_07"), 0, STRING("me"), LW(2), LW(3), LW(4))
    ELSE
        CALL(evt_msg_print_insert, 0, STRING("bottakuru_06"), 0, STRING("me"), LW(2), LW(3))
#else
        CALL(evt_sub_get_language, LW(0))
#if VERSION >= VERSION_US
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("bottakuru_07"), 0, STRING("me"), LW(2), LW(3), LW(4))
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("bottakuru_07"), LW(3))
            CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
            CALL(evt_msg_print_insert, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("me"), LW(4))
        END_SWITCH
        ELSE
        CALL(evt_sub_get_language, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("bottakuru_06"), 0, STRING("me"), LW(2), LW(3))
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("bottakuru_06"), LW(3))
            CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("me"))
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
#endif
    END_IF

    // Mario confirms or declines the purchase.
    CALL(evt_msg_select, 0, STRING("bottakuru_yesno"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print_add, 0, STRING("bottakuru_08"))
        RETURN
    END_IF

    // The item goes into Mario's pouch; if the pouch is full, Charlieton says so and the sale is off.
    CALL(evt_pouch_add_item, LW(1), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_win_coin_off, LW(12))
        IF_INT_GREATER_OR_EQUAL(LW(1), BADGE_MIN)
            CALL(evt_msg_print_add, 0, STRING("bottakuru_12"))
        ELSE
            CALL(evt_msg_print_add, 0, STRING("bottakuru_13"))
        END_IF
        RETURN
    END_IF

    // The stock of that item drops by one, the coins are paid and the 'bought something' flag is set.
    CALL(evt_badgeShop_bottakuru_dec, LW(1))
    MULTIPLY(LW(3), -1)
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(500)
    CALL(evt_win_coin_off, LW(12))
    SET_VALUE(GSWF_GOR_CHARLIETON_BOUGHT_THIS_VISIT, 1)

    // Charlieton thanks Mario; if he is now sold out or Mario has no coins left the visit ends, otherwise he
    // offers more.
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_badgeShop_bottakuru_get_kind_cnt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("bottakuru_09"))
        RETURN
#else
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
#endif
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

/**
 * Init event of Luigi in the plaza: places him by story stage:
 * (380, 0, 300) from STG2_25 to STG3_31, (110, 0, -240) from STG4_15_05 to STG5_42,
 * and (230, 154, -300) from STG7_20_LORD_CRUMP_DEFEATED on.
 */
static EVT_BEGIN(luigi_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_25_AFTER_THE_BOSS_BATTLE, STG3_31_GRUBBAS_PLANS)
        CALL(evt_npc_set_position, STRING("me"), 380, 0, 300)
        CALL(evt_npc_set_ry_lr, STRING("me"), 0)
        CASE_BETWEEN(STG4_15_05_PEACH_X_NAUT_PHD_REPORT, STG5_42_PIRATES_VS_X_NAUTS)
        CALL(evt_npc_set_position, STRING("me"), 110, 0, -240)
        CALL(evt_npc_set_ry_lr, STRING("me"), 1)
        CASE_GREATER_OR_EQUAL(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_npc_set_position, STRING("me"), 230, 154, -300)
        CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    END_SWITCH
    RETURN
EVT_END()

/** Talk event of Luigi: runs the shared first-talk scene luigi_gor_first_talk. */
static EVT_BEGIN(luigi_talk)
    CALL_CHILD(luigi_gor_first_talk)
    RETURN
EVT_END()

/**
 * Enters the NPC that walks with Luigi by story stage:
 * Kick ("c_lp_gesso") from STG2_25 to STG3_31,
 * Torque ("c_lp_met") from STG4_15_05 to STG5_42,
 * and a second Kick from STG7_20_LORD_CRUMP_DEFEATED, each with its NPC setup table.
 */
static EVT_BEGIN(luigi_npcEnt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_25_AFTER_THE_BOSS_BATTLE, STG3_31_GRUBBAS_PLANS)
        CALL(evt_npc_entry, STRING("キック"), STRING("c_lp_gesso"))
        CALL(evt_npc_set_tribe, STRING("キック"), STRING("キック"))
        CALL(evt_npc_setup, PTR(kickEnt))
        CASE_BETWEEN(STG4_15_05_PEACH_X_NAUT_PHD_REPORT, STG5_42_PIRATES_VS_X_NAUTS)
        CALL(evt_npc_entry, STRING("トルク"), STRING("c_lp_met"))
        CALL(evt_npc_set_tribe, STRING("トルク"), STRING("トルク"))
        CALL(evt_npc_setup, PTR(torqueEnt))
        CASE_GREATER_OR_EQUAL(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_npc_entry, STRING("キック２"), STRING("c_lp_gesso"))
        CALL(evt_npc_set_tribe, STRING("キック２"), STRING("キック２"))
        CALL(evt_npc_setup, PTR(kick2Ent))
    END_SWITCH
    RETURN
EVT_END()

/** Init event of Kick, Luigi's friend in the Chapter 2 to 3 period: places him at (420, 0, 300). */
EVT_BEGIN(kick_init)
    CALL(evt_npc_set_position, STRING("me"), 420, 0, 300)
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    RETURN
EVT_END()

/** Talk event of Kick: one line (luigi_npc_01). */
EVT_BEGIN(kick_talk)
    CALL(evt_msg_print, 0, STRING("luigi_npc_01"), 0, STRING("me"))
    RETURN
EVT_END()

/** NPC setup of Kick, Luigi's friend in chapters 2 and 3. */
NpcSetupInfo kickEnt[] = {
    {
        "キック", // name
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

/** Init event of Torque, Luigi's friend in the Chapter 4 to 5 period: places him at (160, 0, -240). */
EVT_BEGIN(torque_init)
    CALL(evt_npc_set_position, STRING("me"), 160, 0, -240)
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    RETURN
EVT_END()

/** Talk event of Torque: one line (luigi_npc_03). */
EVT_BEGIN(torque_talk)
    CALL(evt_msg_print, 0, STRING("luigi_npc_03"), 0, STRING("me"))
    RETURN
EVT_END()

/** NPC setup of Torque, Luigi's friend in chapters 4 and 5. */
NpcSetupInfo torqueEnt[] = {
    {
        "トルク", // name
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

/**
 * Init event of the second Kick: standing at (270, 154, -300) on the rooftop between Lord Crump's defeat
 * and Grodus, hidden otherwise.
 */
EVT_BEGIN(kick2_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG7_20_LORD_CRUMP_DEFEATED, STG8_15_GRODUS)
        CALL(evt_npc_set_position, STRING("me"), 270, 154, -300)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    END_SWITCH
    CALL(evt_npc_set_ry_lr, STRING("me"), 0)
    RETURN
EVT_END()

/** Talk event of the second Kick: one line (luigi_npc_06). */
EVT_BEGIN(kick2_talk)
    CALL(evt_msg_print, 0, STRING("luigi_npc_06"), 0, STRING("me"))
    RETURN
EVT_END()

/** NPC setup of the second Kick, on the rooftop after Lord Crump's defeat. */
NpcSetupInfo kick2Ent[] = {
    {
        "キック２", // name
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
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_FIND_THIS_GUY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
                IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 1)
                    IF_INT_EQUAL(GSWF_TROUBLE_WORK(3), 0)
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

/**
 * Talk event of Borodo for job 4 (irai_04_005):
 * he stops the Goomba (Kuribo 1)'s regular event and sends it off,
 * moving it to (229, 0, -400) and then to (420, -350);
 * the Goomba speaks (irai_04_006), the scene adds 20 coins (evt_sub_get_coin) and goes on through irai_04_008
 * and irai_04_009, ending with GSWF_TROUBLE_WORK_3.
 */
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
    SET_VALUE(GSWF_TROUBLE_WORK(3), 1)
    SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
    SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_FIND_THIS_GUY), 1)
    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
    RETURN
EVT_END()

/** Init event of the thief who stole Mario's coins: from STG0_08_STOLEN_COINS on he stands at (500, 0, -175). */
EVT_BEGIN(thief_init)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG0_08_STOLEN_COINS)
        CALL(evt_npc_set_position, STRING("me"), 500, 0, -175)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(thief_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG0_08_STOLEN_COINS, STG0_END_TO_STAGE_1)
        IF_INT_EQUAL(GSWF_GOR_THIEF_FOUND_IN_HIDEOUT, 0)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_GOR_STOLEN_COINS_LOW, 0)
                IF_INT_EQUAL(GSW_GOR_STOLEN_COINS_HIGH, 0)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                CALL(evt_msg_print, 0, STRING("mac_0_065_01"), 0, STRING("me"))
                BEGIN_INLINE_EVENT
                    CALL(evt_StatusOpen)
                    WAIT_MS(1000)
                    CALL(evt_pouch_get_coin, LW(0))
                    ADD(LW(0), GSW_GOR_STOLEN_COINS_LOW)
                    SET_VALUE(LW(1), GSW_GOR_STOLEN_COINS_HIGH)
                    MULTIPLY(LW(1), 256)
                    ADD(LW(0), LW(1))
                    CALL(evt_pouch_set_coin, LW(0))
                    WAIT_MS(2000)
                    CALL(evt_StatusClose)
                END_INLINE_EVENT
            ELSE
                CALL(evt_msg_print, 0, STRING("mac_0_065_02"), 0, STRING("me"))
            END_IF
            SET_VALUE(GSWF_GOR_THIEF_FOUND_IN_HIDEOUT, 1)
        ELSE
            IF_INT_EQUAL(GSWF_GOR_THIEF_HIDEOUT_LEFT, 0)
                CALL(evt_msg_print, 0, STRING("mac_0_065_03"), 0, STRING("me"))
            ELSE
                CALL(evt_msg_print, 0, STRING("mac_0_065_04"), 0, STRING("me"))
            END_IF
        END_IF
        CASE_LESS(STG1_17_FATHER_SON_REUNITED)
        CALL(evt_msg_print, 0, STRING("mac_0_065_04"), 0, STRING("me"))
        CASE_LESS(STG2_25_AFTER_THE_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("mac_0_065_05"), 0, STRING("me"))
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CALL(evt_msg_print, 0, STRING("mac_0_065_06"), 0, STRING("me"))
        CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
        CALL(evt_msg_print, 0, STRING("mac_0_065_07"), 0, STRING("me"))
        CASE_LESS(STG5_43_X_NAUTS_RETREAT)
        CALL(evt_msg_print, 0, STRING("mac_0_065_08"), 0, STRING("me"))
        CASE_LESS(STG6_54_WELCOME_BACK)
        CALL(evt_msg_print, 0, STRING("mac_0_065_09"), 0, STRING("me"))
        CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_msg_print, 0, STRING("mac_0_065_10"), 0, STRING("me"))
        CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
        CALL(evt_msg_print, 0, STRING("mac_0_065_11"), 0, STRING("me"))
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING("mac_0_065_12"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * NPC setup of the central plaza: the badge shop owner and clerk, the old Toad, Zess T., Marco, the bar
 * owner, the shopkeepers, the inn clerk, the Goombas, the mice, the sailor Bob-omb, the rip-off merchant,
 * Luigi, the thief and "キノピコ" (no events).
 */
static NpcSetupInfo npcEnt[] = {
    {
        "バッジ店長", // name
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
        "バッジ店員", // name
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
        "キノじい", // name
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
        "ナンシー", // name
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
        "マルコ", // name
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
        "マスター", // name
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
        "店長", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        "店員", // name
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
        "宿店員", // name
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
        "ロテン１", // name
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
        "ロテン２", // name
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
        "クリボー１", // name
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
        "クリボー２", // name
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
        "クリボー３", // name
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
        "チューさん１", // name
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
        "チューさん２", // name
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
        "水夫ボム兵", // name
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
        "ボッタクール", // name
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
        "ルイージ", // name
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
        "盗賊", // name
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
        "キノピコ", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Enter event of the south exit "s_bero" (from the harbor):
 * when the story is at STG0_06_1000_X_NAUTS_AFTER it locks Mario's controls,
 * starts christine_nakama and returns 1 in LW(0); otherwise returns 0 and nothing happens.
 */
EVT_BEGIN(christine_nakama_before)
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG0_06_1000_X_NAUTS_AFTER)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    SPAWN_EVENT(christine_nakama)
    SET_VALUE(LW(0), 1)
    RETURN
EVT_END()

/** Exits (beros) of this room: e_bero to gor_02, w_bero to gor_03 and s_bero to gor_00. */
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
        "gor_02", // nextMapName
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
        "gor_03", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "s_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Door animation callback of the house 01 door (Zess T.'s house):
 * rotates the door "S_ie_01_doa" about Y by 0.6 times the value in LW(0).
 */
static EVT_BEGIN(evt_door_01_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Second door callback of house 01: rotates the door "S_ie_01_doa" about X by -0.5 times the value in LW(0). */
static EVT_BEGIN(evt_door_01_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Wall callback of house 01: rotates the wall pieces "S_ie_01uB"
 * and "S_ie_01sB" about X by LW(0) in opposite directions, folding the front wall flat as the door opens.
 */
static EVT_BEGIN(evt_wall_01)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(-1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01uB"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_01sB"), LW(3), 0, 0)
    RETURN
EVT_END()

/**
 * NPCs of house 01 kept lit (exempt from the darkening) while Mario is inside: Zess T. Swapped into the
 * door's copy with evt_door_set_param once the Contact Lens is delivered.
 */
static const char* door_data_01_npc[] = {
    "ナンシー",
    NULL,
};

/** Field items of house 01 kept lit while Mario is inside: the Honey Shroom of Trouble Center request 18. */
const char* door_data_01_item[] = {
    "iri_18",
    NULL,
};

/** Called when the house 01 door opens: dips the music and ambience to 50 percent and low-passes the ambience. */
static EVT_BEGIN(evt_door_01_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** Called when the house 01 door closes: brings the music and ambience back and removes the low-pass. */
static EVT_BEGIN(evt_door_01_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/**
 * Door of house 01 (Zess T.'s house) before the Contact Lens is delivered: the door model, hit areas, inside
 * and outside map groups and the scripts for opening and closing.
 */
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

/** Door of house 01 after the Contact Lens is delivered. */
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

/**
 * Door callback for the ground floor of house 02 (the item shop):
 * rotates the door "S_ie_02_f1_doa" by LW(0) scaled by about 0.6 as Mario opens it.
 */
EVT_BEGIN(evt_door_02_00_f1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f1_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Wall callback for house 02's ground floor: tilts the front wall pieces "S_ie_02_f1B",
 * "S_ie_02_f2uB" and "S_ie_02_f2sB" open by LW(0) degrees so the interior can be seen.
 */
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

/**
 * NPCs of the item shop (house 02's ground floor) kept lit while Mario is inside: the shopkeeper and the
 * clerk.
 */
static const char* door_data_02_npc[] = {
    "店長",
    "店員",
    NULL,
};

const char* door_data_02_npc_01[] = {
    "店長",
    NULL,
};

/**
 * Open callback for the item shop door: dips the music and ambience to half volume and low-passes the ambience
 * while Mario is inside.
 */
EVT_BEGIN(evt_door_02f1_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** Close callback for the item shop door: brings the music and ambience back to normal. */
EVT_BEGIN(evt_door_02f1_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/** Door of house 02's ground floor (the item shop) in its normal state. */
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

/** Door of house 02's ground floor before STG4_32_CHATTING_WITH_FLAVIO while the Contact Lens has arrived. */
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

/**
 * Message shown when the door of the badge shop upstairs is tried
 * while it is not open ("preparing", gor_01_024_00_00): locks the controls for the line, then unlocks them.
 */
EVT_BEGIN(evt_door_02_f2_junbityu)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("gor_01_024_00_00"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Door callback for the upstairs of house 02 (the badge shop):
 * rotates the door "S_ie_02_f2_doaD" by LW(0) scaled by about 0.6.
 */
EVT_BEGIN(evt_door_02_00_f2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_02_f2_doaD"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Wall callback for house 02's upstairs: tilts the wall pieces "S_ie_02_f1B",
 * "S_ie_02_f2uB" and "S_ie_02_f2sB" open by LW(0) degrees.
 */
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

/** NPCs of the badge shop upstairs kept lit while Mario is inside: the badge shop owner and clerk. */
const char* badgemise_npc[] = {
    "バッジ店長",
    "バッジ店員",
    NULL,
};

/**
 * Handler for the badge shop's display shelves ("S_item_07" to "S_item_11", see uwnobj_list):
 * when Mario picks a shelf item the clerk faces him, quotes the price,
 * and on a yes checks his coins and badge space, adds the badge, updates the shop stock and takes the coins.
 * The demo version is a simpler variant without the language switch.
 */
EVT_BEGIN(evt_badge_dummy)
#if VERSION >= VERSION_JP
    CALL(evt_mario_normalize)
#endif
    SET_VALUE(LW(10), LW(0))
    CALL(disp_off, LW(10))
    CALL(evt_set_dir_to_target, STRING("バッジ店員"), STRING("mario"))
    CALL(evt_mario_set_dir_npc, STRING("バッジ店員"))
#if VERSION <= VERSION_DEMO_JP
    IF_INT_EQUAL(GSWF(2), 0)
        CALL_CHILD(first_shopping_evt)
        SET_VALUE(GSWF(2), 1)
        RETURN
    END_IF
    CALL(unitwincoin_disp_onoff, 1)
    CALL(get_value, LW(10), LW(11), LW(12), LW(13), LW(14))
    CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(11), LW(13), LW(14))
    CALL(evt_msg_select, 0, STRING("badgeshop_24"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_continue)
        CALL(evt_msg_print, 0, STRING("badgeshop_22"), 0, STRING("バッジ店員"))
        CALL(unitwincoin_disp_onoff, 0)
#else
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(8))
    CALL(get_value, LW(10), LW(11), LW(12), LW(13), LW(14))
    CALL(evt_sub_get_language, LW(0))
#if VERSION >= VERSION_US
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, STRING("badgeshop_00"), 0, STRING("バッジ店員"), LW(11), LW(13), LW(14))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(15), STRING("badgeshop_00"), LW(13))
        CALL(evt_msg_fmt_str, 1, LW(15), LW(15), LW(11))
        CALL(evt_msg_print_insert, MSG_PRINT_DIRECT_TEXT, LW(15), 0, STRING("バッジ店員"), LW(14))
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
#endif
        RETURN
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(13))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_continue)
        CALL(evt_msg_print, 0, STRING("badgeshop_01"), 0, STRING("バッジ店員"))
        CALL(unitwincoin_disp_onoff, 0)
#else
        CALL(evt_msg_print_add, 0, STRING("badgeshop_01"))
        CALL(evt_win_coin_off, LW(8))
#endif
        RETURN
    END_IF
    CALL(evt_pouch_add_item, LW(12), LW(0))
    IF_INT_EQUAL(LW(0), -1)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_continue)
        CALL(evt_msg_print, 0, STRING("badgeshop_02"), 0, STRING("バッジ店員"))
        CALL(unitwincoin_disp_onoff, 0)
#else
        CALL(evt_msg_print_add, 0, STRING("badgeshop_02"))
        CALL(evt_win_coin_off, LW(8))
#endif
        RETURN
    END_IF
    CALL(set_new_goods_list, LW(10), 0)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(0), LW(10))
    CALL(evt_badgeShop_getBargainKind, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_badgeShop_special_dec, LW(12))
    ELSE
        CALL(evt_badgeShop_throw_dec, LW(12))
    END_IF
    MULTIPLY(LW(13), -1)
    CALL(evt_pouch_add_coin, LW(13))
    CALL(unitwincoin_wait)
    WAIT_MS(200)
    CALL(unitwincoin_disp_onoff, 0)
#else
    CALL(evt_badgeShop_special_dec, LW(12))
    CALL(unitwin_set_bought, LW(10), LW(12))
    MULTIPLY(LW(13), -1)
    CALL(evt_pouch_add_coin, LW(13))
    CALL(evt_win_coin_wait, LW(8))
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(8))
#endif
    CALL(evt_msg_continue)
    RETURN
EVT_END()

/**
 * Display pedestals of the badge shop upstairs in house 02: for each, the map object, its hit area and the
 * callback.
 */
UnitWinSell uwnobj_list[] = {
    {"S_item_07", "A_item_08", evt_badge_dummy},
    {"S_item_08", "A_item_09", evt_badge_dummy},
    {"S_item_09", "A_item_10", evt_badge_dummy},
    {"S_item_10", "A_item_11", evt_badge_dummy},
    {"S_item_11", "A_item_12", evt_badge_dummy},
    {0},
};

/**
 * Open callback for the badge shop (upstairs of house 02):
 * dips the music and ambience, then allocates the shop window
 * and fills it with the day's bargain badges (badgeShop_getBargainTable).
 */
EVT_BEGIN(evt_open_02_01_f2)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_unitwin_alloc)
#else
    CALL(evt_unitwin_alloc, 1)
#endif
    CALL(badgeShop_getBargainTable, LW(0))
    CALL(evt_unitwin_selltable_setup, PTR(uwnobj_list), LW(0))
    RETURN
EVT_END()

/** Waits until Mario's controls are free again and then frees the badge shop window; run from evt_close_02_01_f2. */
EVT_BEGIN(evt_close_sub)
    CALL(mario_keyoff_wait)
    CALL(evt_unitwin_free)
    RETURN
EVT_END()

/** Close callback for the badge shop: brings the music and ambience back and starts evt_close_sub. */
EVT_BEGIN(evt_close_02_01_f2)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    SPAWN_EVENT(evt_close_sub)
    RETURN
EVT_END()

/** Door of house 02's upstairs (the badge shop). */
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

/**
 * Sets the colour of the bar's ground floor interior "S_i03_f1_naka" to a gray of level args[0] (0 black to 255
 * normal), for fading it in and out.
 */
USER_FUNC(house03_f1_fade_func) {
    s32 intensity = evtGetValue(event, event->args[0]);

    mapGrpSetColor("S_i03_f1_naka", (GXColor){intensity, intensity, intensity, 0xFF});
    return EVT_RETURN_DONE;
}

/**
 * Sets the colour of the bar's upper floor interior "S_i03_f2_naka"
 * and its door "S_ie_03_f21_doa" to a gray of level args[0], for fading it in and out.
 */
USER_FUNC(house03_f2_fade_func) {
    u8 intensity = evtGetValue(event, event->args[0]);

    mapGrpSetColor("S_i03_f2_naka", (GXColor){intensity, intensity, intensity, 0xFF});
    mapGrpSetColor("S_ie_03_f21_doa", (GXColor){intensity, intensity, intensity, 0xFF});
    return EVT_RETURN_DONE;
}

/**
 * Returns 1 in args[0] when the NPC Mario is currently able to talk to is Marco (Flavio), else 0.
 * Used to cut Marco's muttering when Mario is next to him.
 */
USER_FUNC(chk_marco_fuki) {
    EvtWord* args = event->args;
    // FAKE: passes marioGetPtr() to fbatNpcTalkCheck, which takes no arguments, through a cast; the plain
    // fbatNpcTalkCheck() call drops the marioGetPtr call and changes the object.
    NpcEntry* npc = ((NpcEntry* (*)(MarioWork*))fbatNpcTalkCheck)(marioGetPtr());

    if (npc != NULL && strcmp(npc->description, "\203}\203\213\203R") == 0) {
        evtSetValue(event, args[0], 1);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[0], 0);
    return EVT_RETURN_DONE;
}

/**
 * Spawns a smoke puff effect at (args[0], args[1], args[2]);
 * used for the brawl cloud around the thieves in the Goombella scene.
 */
USER_FUNC(maffia_kemuri) {
    EvtWord* args = event->args;
    f32 x = evtGetValue(event, args[0]);
    f32 y = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);

    effKemuriEntry(1, x, y, z, 0.0f);
    return EVT_RETURN_DONE;
}

/**
 * Debug helper: with args[0] equal to 0, returns in args[1] a pointer to a string describing the current story
 * progress (search_evt_moji of GSW_STORY_PROGRESS).
 */
static USER_FUNC(evt_osreport) {
    EvtWord* args = event->args;

    switch (evtGetValue(event, args[0])) {
        case 0:
            evtSetValue(event, args[1], (EvtWord)search_evt_moji(evtGetValue(event, GSW_STORY_PROGRESS)));
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

/** If the NPC named in args[0] has a regular event, deletes it and clears its move event. */
static USER_FUNC(delete_npc_regl_evt) {
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));

    if (npc->regularEvtId != 0) {
        evtDeleteID(npc->regularEvtId);
        npc->moveEvent = NULL;
        npc->regularEvtId = 0;
    }
    return EVT_RETURN_DONE;
}

/**
 * Runs while Mario is in the bar (house 03): each frame decides whether he is on the ground floor (y below 100)
 * or upstairs and sets the fade direction GW(14) and GW(15),
 * so the floor he is on fades in and the other one out.
 */
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

/**
 * Applies the fade level in LW(0) to the colour of Luigi and his friend (Torque) on the bar's ground floor,
 * depending on the story stage they stand in the bar.
 */
EVT_BEGIN(luigi_f1_fade_check)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_25_AFTER_THE_BOSS_BATTLE, STG3_31_GRUBBAS_PLANS)
        CASE_BETWEEN(STG4_15_05_PEACH_X_NAUT_PHD_REPORT, STG5_42_PIRATES_VS_X_NAUTS)
        CALL(evt_npc_set_color, STRING("ルイージ"), LW(0), LW(0), LW(0), 255)
        CALL(evt_npc_set_color, STRING("トルク"), LW(0), LW(0), LW(0), 255)
        CASE_GREATER_OR_EQUAL(STG7_20_LORD_CRUMP_DEFEATED)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Loops forever fading the ground floor of the bar (interior, Bob-omb sailor, Chusan,
 * plus Luigi's friends) toward full colour or black by 10 levels per frame, following GW(14).
 */
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
        CALL_CHILD(luigi_f1_fade_check)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Applies the fade level in LW(0) to the colour of Luigi and the second Kick on the bar's upper floor,
 * depending on the story stage.
 */
EVT_BEGIN(luigi_f2_fade_check)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG2_25_AFTER_THE_BOSS_BATTLE, STG3_31_GRUBBAS_PLANS)
        CASE_BETWEEN(STG4_15_05_PEACH_X_NAUT_PHD_REPORT, STG5_42_PIRATES_VS_X_NAUTS)
        CASE_GREATER_OR_EQUAL(STG7_20_LORD_CRUMP_DEFEATED)
        CALL(evt_npc_set_color, STRING("ルイージ"), LW(0), LW(0), LW(0), 255)
        CALL(evt_npc_set_color, STRING("キック２"), LW(0), LW(0), LW(0), 255)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Loops forever fading the upper floor of the bar (interior, inn clerk, the old Toad,
 * plus Luigi's friends) toward full colour or black by 10 levels per frame, following GW(15).
 */
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
        CALL_CHILD(luigi_f2_fade_check)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Loops forever playing the dripping water animation "sizuku" in the bar and its sound at the sink,
 * loud upstairs and quiet downstairs.
 */
EVT_BEGIN(house03_suiteki)
    LOOP(0)
        CALL(evt_map_playanim, STRING("sizuku"), MAP_ANIM_PLAY_ONCE, 0)
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

/**
 * Marco's (Flavio's) muttering while Mario is in the bar before Chapter 4's Flavio talk:
 * shows mac_4_004 over and over, cancelling it when he becomes Mario's talk target. Does nothing from STG4_32 on.
 */
EVT_BEGIN(marco_hitorigoto)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_32_CHATTING_WITH_FLAVIO)
        RETURN
    END_IF
    CALL(evt_mario_wait_movable)
    SET_VALUE(LSWF_GOR_01_FLAVIO_SONG_BUBBLE_ON, 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(chk_marco_fuki, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                IF_INT_EQUAL(LSWF_GOR_01_FLAVIO_SONG_BUBBLE_ON, 1)
                    CALL(evt_msg_close, STRING("mac_4_004"))
                END_IF
                SET_VALUE(LSWF_GOR_01_FLAVIO_SONG_BUBBLE_ON, 0)
            ELSE
                SET_VALUE(LSWF_GOR_01_FLAVIO_SONG_BUBBLE_ON, 1)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    LOOP(0)
        LOOP(0)
            IF_INT_EQUAL(LSWF_GOR_01_FLAVIO_SONG_BUBBLE_ON, 1)
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LSWF_GOR_01_FLAVIO_SONG_BUBBLE_ON, 1)
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

/**
 * Door callback for house 03's ground floor (the bar):
 * rotates the door "S_ie_03_f1_doa" by LW(0) scaled by about 0.6.
 */
EVT_BEGIN(evt_door_03_00_f1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Second door callback for house 03's ground floor:
 * rotates the door "S_ie_03_f1_doa" about X by -0.5 times LW(0).
 */
EVT_BEGIN(evt_door_03_01_f1)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Open callback for the bar's ground floor door:
 * dips the music and ambience, hides the roof of the upper floor, and starts the floor fading,
 * Marco's muttering and the dripping sound.
 */
EVT_BEGIN(house_03_f1_open)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_03_f21_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_f2_soto"), MAP_OBJ_FLAG_HIDDEN)
    BEGIN_INLINE_EVENT
        CALL(evt_door_end_wait)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_i03_f2_soto"), MAP_OBJ_FLAG_HIDDEN)
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

/**
 * Close callback for the bar's ground floor door:
 * restores the doors, the music and ambience, and stops the fading, muttering and dripping events.
 */
EVT_BEGIN(house_03_f1_close)
    BEGIN_INLINE_EVENT
        WAIT_MS(1600)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_03_f1_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_03_f21_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
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

/**
 * Wall callback for the bar from the ground floor:
 * tilts the front wall pieces "S_ie_03_f1B", "S_i03_f2_sitaB" and "S_i03_f2_ueB" about X by LW(0), half LW(0), and -LW(0), respectively.
 */
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

/**
 * NPCs of house 03 kept lit while Mario is inside: the bar owner (Podley), the old Toad and the others
 * listed.
 */
static const char* door_data_03_npc[] = {
    "マスター",
    "キノじい",
    "宿店員",
    "チューさん２",
    "水夫ボム兵",
    "マルコ",
    "ルイージ",
    "トルク",
    "キック２",
    NULL,
};

/** Field items of house 03 kept lit while Mario is inside: the Skull Gem. */
const char* door_data_03_item[] = {
    "ドクロジュエル",
    NULL,
};

/** Door of house 03's ground floor (the bar). */
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

/** Door callback for the bar's upper floor: rotates the door "S_ie_03_f21_doa" by LW(0) scaled by about 0.6. */
EVT_BEGIN(evt_door_03_00_f2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f21_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Open callback for the bar's upper floor door:
 * dips the music and ambience and starts the floor fading, Marco's muttering and the dripping sound,
 * with the upper floor visible first.
 */
EVT_BEGIN(house_03_f2_open)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_03_f1_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
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

/**
 * Close callback for the bar's upper floor door:
 * restores the doors, the music and ambience, and stops the fading, muttering and dripping events.
 */
EVT_BEGIN(house_03_f2_close)
#if VERSION >= VERSION_US
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_i03_f1_naka"), MAP_OBJ_FLAG_HIDDEN)
#endif
    BEGIN_INLINE_EVENT
        WAIT_MS(1600)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_03_f1_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_03_f21_doa"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
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

/**
 * Wall callback for the bar from the upper floor:
 * tilts the front wall pieces by LW(0), half LW(0), and -LW(0), and the ground floor door by -half LW(0), all about X.
 */
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

/** Door of house 03's upstairs. */
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

/**
 * Door animation callback of the house 04 door (the thieves' house):
 * rotates the door "S_ie_04_doa" about Y by 0.6 times the value in LW(0).
 */
static EVT_BEGIN(evt_door_04_00)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5996094f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Second door callback of house 04: rotates the door "S_ie_04_doa" about X by -0.5 times the value in LW(0). */
static EVT_BEGIN(evt_door_04_01)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04_doa"), LW(0), 0, 0)
    RETURN
EVT_END()

/**
 * Wall callback of house 04: rotates the wall pieces "S_ie_04uB"
 * and "S_ie_04sB" about X by LW(0) in opposite directions.
 */
static EVT_BEGIN(evt_wall_04)
    SET_FLOAT_VALUE(LW(2), LW(0))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(2), FLOAT(1.0f))
    MULTIPLY_FLOAT(LW(3), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04uB"), LW(2), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("S_ie_04sB"), LW(3), 0, 0)
    RETURN
EVT_END()

/**
 * Called when the house 04 door opens: dips the music and ambience to 50 percent,
 * low-passes the ambience and turns off blending on the cockroach object S_goki_01.
 */
static EVT_BEGIN(evt_door_04_open)
    CALL(evt_snd_bgmoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_goki_01"), MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2)
    RETURN
EVT_END()

static EVT_BEGIN(evt_door_04_close)
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    IF_INT_EQUAL(GSWF_GOR_THIEF_FOUND_IN_HIDEOUT, 1)
        SET_VALUE(GSWF_GOR_THIEF_HIDEOUT_LEFT, 1)
    END_IF
    RETURN
EVT_END()

/** NPCs of house 04 kept lit while Mario is inside: the thief. */
static const char* door_data_04_npc[] = {
    "盗賊",
    NULL,
};

/** Field items of house 04 kept lit while Mario is inside: the Star Piece (item_01). */
static const char* door_data_04_item[] = {
    "item_01",
    NULL,
};

/** Door of house 04 (the thieves' house). */
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

/**
 * Loops forever, swinging the map object S_rop (the rope of the plaza's gallows: koushukei-dai, 絞首刑台) back and
 * forth between +10 and -10 degrees about X, one second each way.
 */
static EVT_BEGIN(koushukei_dai)
    LOOP(0)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 10, -10, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 0, STRING("S_rop"), LW(0), 0, 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_FRAMES(1)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -10, 10, 1000)
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

/** Item pedestals of the shop in house 02: for each, the map object and its hit area. */
static ShopObj obj_list[] = {
    {"S_item_01", "A_item_01"},
    {"S_item_02", "A_item_02"},
    {"S_item_03", "A_item_03"},
    {"S_item_04", "A_item_04"},
    {"S_item_05", "A_item_05"},
    {"S_item_06", "A_item_06"},
};

/**
 * Goods of the item shop in house 02, item and price in coins: Mushroom 5, Honey Syrup 5, Tasty Tonic 3,
 * Fire Flower 10, Sleepy Sheep 8, Fright Mask 5. Sold before the Contact Lens arrives, and after it is
 * delivered whenever Mario already has every Super Luigi book the story stage allows.
 */
static ShopItem goods_list[] = {
    {ITEM_MUSHROOM, 5},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

/**
 * Goods of the item shop once the Contact Lens has arrived and until it is delivered:
 * the Contact Lens (10 coins) takes the Mushroom's place.
 */
ShopItem goods_list2[] = {
    {ITEM_CONTACT_LENS, 10},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

/**
 * Goods of the item shop after the lens is delivered while Mario has no Super Luigi book and the story allows
 * one: the first book, Super Luigi (64 coins), takes the Mushroom's place.
 */
ShopItem goods_luigi1[] = {
    {ITEM_SUPER_LUIGI1, 64},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

/**
 * Goods of the item shop while Mario has one Super Luigi book: Super Luigi 2 (128 coins) replaces the
 * Mushroom.
 */
ShopItem goods_luigi2[] = {
    {ITEM_SUPER_LUIGI2, 128},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

/**
 * Goods of the item shop while Mario has two Super Luigi books: Super Luigi 3 (128 coins) replaces the
 * Mushroom.
 */
ShopItem goods_luigi3[] = {
    {ITEM_SUPER_LUIGI3, 128},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

/**
 * Goods of the item shop while Mario has three Super Luigi books: Super Luigi 4 (128 coins) replaces the
 * Mushroom.
 */
ShopItem goods_luigi4[] = {
    {ITEM_SUPER_LUIGI4, 128},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

/**
 * Goods of the item shop while Mario has four Super Luigi books: Super Luigi 5 (256 coins) replaces the
 * Mushroom.
 */
ShopItem goods_luigi5[] = {
    {ITEM_SUPER_LUIGI5, 256},
    {ITEM_HONEY_SYRUP, 5},
    {ITEM_TASTY_TONIC, 3},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_SLEEPY_SHEEP, 8},
    {ITEM_FRIGHT_MASK, 5},
};

/**
 * Coins the item shop in house 02 pays when Mario sells these items (its stock plus a few others, such as the
 * Whacka Bump for 55); they replace the item table's sell prices only while the sell menu runs.
 */
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

/** Shopkeeper of the item shop in house 02: name and the shop's message keys. */
static ShopperData35 shopper_data = {
    "店長", // name
    NULL, NULL, // fook_evt..buy_evt
    {"gor_shop1_00", "gor_shop1_01", "gor_shop1_02", "gor_shop1_03", "gor_shop1_04", "gor_shop1_05", "gor_shop1_06", "gor_shop1_07", "gor_shop1_08", "gor_shop1_09", "gor_shop1_10", "gor_shop1_11", "gor_shop1_12", "gor_shop1_13", "gor_shop1_14", "gor_shop1_15", "gor_shop1_16", "gor_shop1_17", "gor_shop1_18", "gor_shop1_19", "gor_shop1_20", "gor_shop1_21", "gor_shop1_22", "gor_shop1_23", "gor_shop1_24", "gor_shop1_25", "gor_shop1_26", "gor_shop1_27", "gor_shop1_28", "gor_shop1_29", "gor_shop1_30", "gor_shop1_31", "gor_shop1_32", "gor_shop1_33", "gor_shop1_34"}, // msgs
};

/**
 * Sets up the item shop in house 02.
 * Before the lens is delivered it sells goods_list (goods_list2 with the Contact Lens once it has arrived);
 * afterwards it sells goods_luigi1 to goods_luigi5 in turn, adding the next Super Luigi book each time,
 * until Mario owns as many as the story stage allows.
 */
EVT_BEGIN(gor_01_shop_setup)
    IF_INT_EQUAL(GSWF_GOR_ZESS_T_GOT_CONTACT_LENS, 0)
        IF_INT_EQUAL(GSWF_GOR_CONTACT_LENS_ARRIVED, 0)
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
            CASE_LESS(STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
            SET_VALUE(LW(11), 0)
            CASE_LESS(STG5_43_X_NAUTS_RETREAT)
            SET_VALUE(LW(11), 1)
            CASE_LESS(STG6_54_WELCOME_BACK)
            SET_VALUE(LW(11), 2)
            CASE_LESS(STG7_20_LORD_CRUMP_DEFEATED)
            SET_VALUE(LW(11), 3)
            CASE_LESS(STG8_16_THE_RETURN_OF_THE_QUEEN)
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

/**
 * Inn setup (evt_kinopio_setup): the price of a night (5 coins), the bed, the camera and the clerk's position
 * and the inn's message keys.
 */
static KinopioData kino_dt = {
    0, // breakfastItem
    {0}, // breakfastPos
    5, // ryoukin
    {368.0f, 170.0f, -295.0f}, // bed
    {330.0f, 154.0f, -250.0f}, // bedside
    {130.0f, 370.0f, -79.0f, 313.0f, 220.0f, -264.0f}, // cam
    -30.0f, // bedDeg
    {227.0f, 154.0f, -200.0f}, // npcTalk
    "宿店員", // npcName
    {"yado_00", "yado_01", "yado_02", "yado_03", "yado_04", "yado_05", "yado_06", "yado_07", "yado_08", "yado_09", "yado_10", "yado_11", "yado_12", "yado_13"}, // msg
    NULL, NULL, // toBedEvt..fromBedEvt
};

/**
 * Animation of the bar's front door "S_ie_03_f1_doa" opening to 108 degrees over 500 ms, pausing 500 ms,
 * then closing over 500 ms, with door sounds. Shows the interior during the animation, hides it afterwards,
 * and clears GF(0) when done.
 */
EVT_BEGIN(bar_door_anime)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_03_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_get_position, STRING("S_ie_03_f1_doa"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 108, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mapobj_rotate, 1, STRING("S_ie_03_f1_doa"), 0, LW(0), 0)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 108, 0, 500)
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
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_03_naka"), MAP_OBJ_FLAG_HIDDEN)
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
    SPAWN_EVENT(maffia_assault)
    CALL(evt_mario_clear_party, 1)
    CALL(evt_cam3d_evt_set, 0, 134, 984, 0, 38, 442, 0, EASING_SINE_IN_OUT)
    CALL(evt_party_stop, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_pouch_party_join, PARTY_MEMBER_GOOMBELLA)
        CALL(evt_mario_set_party_pos, 0, PARTY_MEMBER_GOOMBELLA, 25, 0, 625)
        CALL(evt_party_set_dir, 0, 270, 0)
        CALL(evt_party_move_pos2, 0, 25, 500, FLOAT(150.0f))
        WAIT_FRAMES(1)
        CALL(evt_party_set_dir, 0, 270, 200)
    END_INLINE_EVENT
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(evt_mario_set_pos, -25, 0, 625)
    CALL(evt_mario_mov_pos2, -25, 500, FLOAT(150.0f))
    CALL(evt_mario_adjust_dir)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
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
    CALL(evt_cam3d_evt_set, 0, 55, 800, 0, 45, 440, 400, EASING_SINE_IN_OUT)
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
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
    CALL(evt_item_entry, STRING("item00"), ITEM_MAGICAL_MAP_LARGE, LW(0), LW(1), LW(2), ITEM_TYPE_STILL, -1, 0)
    CALL(evt_item_flag_on, STRING("item00"), 2)
    CALL(evt_item_set_scale, STRING("item00"), FLOAT(0.5498047f))
    WAIT_MS(500)
    CALL(evt_party_move_pos2, 0, 0, 500, FLOAT(120.0f))
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
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
        CALL(evt_cam3d_evt_set, 25, 55, 800, 25, 45, 440, 800, EASING_SINE_IN_OUT)
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
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 200, 1, -1000)
    WAIT_MS(500)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
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
        SPAWN_EVENT(bar_door_anime)
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
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_0_047_02"), 0, STRING("キノじい"))
        WAIT_MS(500)
        CALL(evt_npc_get_position, STRING("キノじい"), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, LW(0), LW(1), LW(2))
        CALL(evt_msg_print, 0, STRING("mac_0_047_02"), 0, STRING("キノじい"))
        WAIT_MS(1000)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_cam3d_evt_set, 0, 55, 800, 0, 45, 440, 500, EASING_SINE_IN_OUT)
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
    CALL(evt_cam3d_evt_set, 27, 45, 800, 27, 35, 440, 800, EASING_SINE_IN_OUT)
    WAIT_MS(800)
    SET_VALUE(LW(8), 0)
    SET_VALUE(LW(9), 1)
    CALL_CHILD(all_party_lecture)
    CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_STG0_GOR1"), 2000)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR1"))
    CALL(evt_msg_print_party, STRING("mac_0_053"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_MAKE_DISP_DIR)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    SET_VALUE(GSW_STORY_PROGRESS, STG0_07_GOOMBELLA_JOINS)
    SPAWN_EVENT(christine_nakama_after)
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
#if VERSION >= VERSION_JP
    SET_VALUE(GF(11), 1)
#endif
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
    SPAWN_EVENT(maffia_turn)
    CALL(evt_npc_move_position, STRING("マフィア１"), 90, 15, 0, FLOAT(360.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("マフィア１"), 110, 15, 0, FLOAT(360.0f), 0)
    SPAWN_EVENT(maffia_turn)
    CALL(evt_npc_move_position, STRING("マフィア１"), 90, 15, 0, FLOAT(360.0f), 0)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("マフィア１"), 110, 15, 0, FLOAT(360.0f), 0)
    SPAWN_EVENT(maffia_turn)
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
#if VERSION >= VERSION_JP
    SET_VALUE(GF(11), 0)
#endif
    RETURN
EVT_END()

EVT_BEGIN(christine_nakama_after)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_NO_SAVE_BLOCK)
        RETURN
    END_IF
    LOOP(0)
        IF_INT_EQUAL(GSWF_MAIN_SAVED_SINCE_GOOMBELLA_JOINED, 1)
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, STRING("mac_0_061_01"), 0, STRING("party"))
    SET_VALUE(GSWF_GOR_01_SAVE_REMINDER_DONE, 1)
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
    SPAWN_EVENT(box_copy)
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
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.01953125f), 500)
    END_INLINE_EVENT
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_TBOX_FALL2"), LW(0), LW(1), LW(2), 0)
    SET_VALUE(GSWF_GOR_01_ULTRA_HAMMER_CHEST_FELL, 1)
    RETURN
EVT_END()

EVT_BEGIN(ultrahammer_get)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("mobj_box00"), LW(0), LW(1), LW(2))
    CALL(lect_juyoitem_rel_set, PTR(all_juyoitem_lecture_rel))
    CALL(evt_item_entry, STRING("hammer"), ITEM_ULTRA_HAMMER, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
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
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, STRING("<plate>\nゆうこうな　データが\n見つかりませんでした\nメッセージを　とうろく　してください\n<key>\n<scrl_auto>\n<once_stop>\n"), 0, 0)
            CALL(evt_msg_print_add_insert, MSG_PRINT_DIRECT_TEXT, STRING("げんざいの　イベントばんごうは\n『%s』\nです\n<key>"), LW(1))
        ELSE
            CALL(evt_osreport, 0, LW(1))
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, STRING("<plate>\nゆうこうな　データが\nじゅうふくして　みつかりました\nゆうこうな　メッセージは　１つにしてください\n<key>\n<scrl_auto>\n<once_stop>"), 0, 0)
            SET_VALUE(LW(2), 0)
            SET_VALUE(LW(3), LW(0))
            LOOP(LW(0))
                CALL(keijiban_joho_makelabel, LW(2), LW(1))
                CALL(evt_msg_print_add_insert, MSG_PRINT_DIRECT_TEXT, STRING("『%s』\n<key>\n<scrl_auto>\n<once_stop>\n"), LW(1))
                ADD(LW(2), 1)
            END_LOOP
            CALL(evt_msg_print_add_insert, MSG_PRINT_DIRECT_TEXT, STRING("いじょう　%dこが\nじゅうふくしている　メッセージです\n<key>"), LW(3))
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
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, STRING("<plate>\nゆうこうな　データが\n見つかりませんでした\nメッセージを　とうろく　してください\n<key>\n<scrl_auto>\n<once_stop>\n"), 0, 0)
            CALL(evt_msg_print_add_insert, MSG_PRINT_DIRECT_TEXT, STRING("げんざいの　イベントばんごうは\n『%s』\nです\n<key>"), LW(1))
        ELSE
            CALL(evt_osreport, 0, LW(1))
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, STRING("<plate>\nゆうこうな　データが\nじゅうふくして　みつかりました\nゆうこうな　メッセージは　１つにしてください\n<key>\n<scrl_auto>\n<once_stop>"), 0, 0)
            SET_VALUE(LW(2), 0)
            SET_VALUE(LW(3), LW(0))
            LOOP(LW(0))
                CALL(keijiban_joho_makelabel, LW(2), LW(1))
                CALL(evt_msg_print_add_insert, MSG_PRINT_DIRECT_TEXT, STRING("『%s』\n<key>\n<scrl_auto>\n<once_stop>\n"), LW(1))
                ADD(LW(2), 1)
            END_LOOP
            CALL(evt_msg_print_add_insert, MSG_PRINT_DIRECT_TEXT, STRING("いじょう　%dこが\nじゅうふくしている　メッセージです\n<key>"), LW(3))
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
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_gai_kabe_k"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_gai_kabe_k_01"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_gai_kabe_k"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_gai_kabe_k_01"), MAP_OBJ_FLAG_HIDDEN)
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
    CALL(evt_npc_status_onoff, 1, STRING("ババ"), NPC_STATUS_FACE_MOVE_DIR)
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
    CALL(evt_cam3d_evt_set, 200, 160, 616, 200, 65, 74, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_fade_end_wait)
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_msg_print, 0, STRING("kpa_stg6_000"), 0, STRING("クリボー１"))
        CALL(evt_npc_status_onoff, 1, STRING("クリボー１"), NPC_STATUS_FACE_MOVE_DIR)
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
        CALL(evt_npc_status_onoff, 1, STRING("ロテン１"), NPC_STATUS_FACE_MOVE_DIR)
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_koopa_weary, 0)
    CALL(kpa_chg_pose, STRING("KPA_S_1"))
    CALL(evt_mario_set_dir, 90, 500, 0)
    WAIT_MS(700)
    CALL(evt_cam3d_evt_set, -225, 120, 755, 416, 150, -12, 0, EASING_SINE_IN_OUT)
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
    CALL(evt_cam3d_evt_set, 255, 164, 587, 255, 120, 341, 2600, EASING_SINE_IN_OUT)
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
    CALL(evt_npc_status_onoff, 1, STRING("ルイージ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("ルイージ"), 200, 100, 0, 60, 0)
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("ルイージ"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("ルイージ"))
    WAIT_MS(500)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), STRING("ルイージ"), 1, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    CALL(evt_npc_move_position, STRING("ルイージ"), 50, 100, 0, 60, 0)
    WAIT_MS(1000)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_55_02_BOWSER_ROGUEPORT)
    CALL(evt_bero_mapchange, STRING("kpa_05"), STRING(""))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()


/** Sets the darkness projection view from (0, 100, 100 + z) toward (0, 0, z), with z from args[0]. */
static USER_FUNC(yami_view) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){0.0f, 100.0f, 100.0f + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){0.0f, 0.0f, z});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

/**
 * Turns on TEV callback number args[1] (low 4 bits) for the animation pose of the NPC named in args[0],
 * if it exists; used to draw it in the darkness.
 */
static USER_FUNC(anim_tevcallback) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, MAP_OBJ_TEV_CALLBACK(flag & 0xF));
    return EVT_RETURN_DONE;
}

/** Same as anim_tevcallback for a map object named in args[0] (the save block or the chest). */
static USER_FUNC(anim_tevcallback_mobj) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    MapObjectEntry* mobj = mobjNameToPtrNoAssert(name);

    if (mobj == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(mobj->poseId, MAP_OBJ_TEV_CALLBACK(flag & 0xF));
    return EVT_RETURN_DONE;
}

/**
 * Script run for each townsperson in the darkness scene (name in LW(0)):
 * waits until the NPC's Z position reaches the marker in GW(0), stops its regular event,
 * shows a speech bubble and then turns around at random intervals of 0.1 to 0.5 seconds.
 */
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
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(""), LW(0), 0, 0, 0, 0, 0, 0, 0, 48)
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

/**
 * Scene of the world darkening (Chapter 8 story step):
 * Mario and his partner are hidden, Toadsworth is placed at the plaza,
 * the map and NPCs get a darkness colour callback (envSetYami), the townspeople run sekai_yami_npc,
 * SFX_STG8_WORLD_DARK1 plays and the game changes to the nok_01 scene "sekai_yami1".
 */
static EVT_BEGIN(sekai_yami_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_npc_set_position, STRING("キノじい"), 0, 0, 403)
    CALL(evt_cam3d_evt_set, 0, 350, 1530, 0, 150, 44, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
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
    SPAWN_EVENT(sekai_yami_npc)
    SET_VALUE(LW(0), STRING("ロテン１"))
    SPAWN_EVENT(sekai_yami_npc)
    SET_VALUE(LW(0), STRING("クリボー１"))
    SPAWN_EVENT(sekai_yami_npc)
    SET_VALUE(LW(0), STRING("チューさん１"))
    SPAWN_EVENT(sekai_yami_npc)
    SET_VALUE(LW(0), STRING("ボッタクール"))
    SPAWN_EVENT(sekai_yami_npc)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_WORLD_DARK1"), 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 700, -850, 5000)
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
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 64, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_bg_set_color, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("nok_01"), STRING("sekai_yami1"))
    RETURN
EVT_END()

/**
 * Scene of everyone's voices (Chapter 8 story step):
 * Mario and his partner and the townspeople are moved out of the way, the shop NPCs are removed,
 * a magic circle (mahojin) appears with SFX_STG8_STARSTONE_EFFECT1
 * and the game changes to the nok_01 scene "minnnanokoe" with BGM_EVT_STG8_VOICE1.
 */
static EVT_BEGIN(minnnanokoe_evt)

    // Controls are locked and Mario and his partner are moved off the map.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)

    // The shop, inn and street NPCs are put out of sight or removed so the plaza is empty.
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

    // The camera looks down on the darkened plaza (Shadow Queen's darkness).
    CALL(evt_cam3d_evt_set, 0, 645, 824, 0, 55, 283, 0, EASING_SINE_IN_OUT)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback_mobj, STRING("mobj_save"), 1)
    CALL(anim_tevcallback_mobj, STRING("mobj_box00"), 1)
    CALL(yami_view, -800)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(evt_seq_wait, SEQ_GAME)

    // A magic circle appears over the plaza.
    CALL(evt_eff, STRING("mahojin"), STRING("mahojin"), 8, 0, 5, 250, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(wait_mahojin)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_STARSTONE_EFFECT1"), 0)
    WAIT_MS(7000)

    // A ring of seven star stones rises out of the circle and spins as the voices theme starts.
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
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_VOICE1"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -50, 30, 3000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP

    // The stones pulse in height.
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 3000, 2500, 500)
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
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 2500, 3000, 500)
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

    // The camera pulls back, then drops and tilts up as the stones climb into the sky.
    CALL(evt_cam3d_evt_set, 0, 110, 1093, 0, 110, 293, 1000, EASING_SINE_IN_OUT)
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
        CALL(evt_cam3d_evt_set, 0, -75, 999, 0, 300, 293, 2000, EASING_QUADRATIC_IN)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 25, 800, 2000)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(GW(1), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)

    // Fade out and change to the scene where everyone's voices are heard.
    CALL(evt_bero_mapchange, STRING("nok_01"), STRING("minnnanokoe"))
    RETURN
EVT_END()

/**
 * Init hook for Trouble Center job 9 (the Courage Shell collecting request):
 * while the job is active it routes the item shop manager's talk through iri_09_tentyo_talk.
 */
EVT_BEGIN(iri_09_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ORDER_ME_AN_ITEM)
        CALL(set_fook_evt, PTR(&shopper_data), PTR(iri_09_tentyo_talk))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_09_tentyo_talk)
    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_ORDER_ME_AN_ITEM), 1)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
        CALL(evt_msg_print, 0, STRING("irai_09_001"), 0, STRING("店長"))
        SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
        SET_VALUE(LW(0), -1)
    ELSE
        CALL(evt_msg_print, 0, STRING("irai_09_002"), 0, STRING("店長"))
        CALL(evt_pouch_check_item, ITEM_COURAGE_SHELL, LW(0))
        IF_INT_GREATER(LW(0), 0)
            CALL(iri_09_item_tbl_alloc, LW(0))
            LOOP(0)
                CALL(evt_msg_continue)
                CALL(iri_09_item_tbl_make, LW(0), LW(1))
#if VERSION <= VERSION_DEMO_JP
                CALL(evt_win_item_select, LW(1), LW(2))
#else
                CALL(evt_win_item_select, 0, LW(1), LW(2), 0)
#endif
                IF_INT_EQUAL(LW(2), -1)
                    SET_VALUE(LF(0), 1)
                    BREAK_LOOP
                END_IF
                CALL(evt_msg_print, 0, STRING("irai_09_003"), 0, STRING("店長"))
                CALL(evt_pouch_remove_item, ITEM_COURAGE_SHELL, LW(1))
                ADD(GSW_TROUBLE_WORK(0), 1)
                IF_INT_EQUAL(GSW_TROUBLE_WORK(0), 5)
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
            IF_INT_LESS(GSW_TROUBLE_WORK(0), 5)
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_msg_print_add, 0, STRING("irai_09_006_1"))
                ELSE
                    CALL(evt_msg_print, 0, STRING("irai_09_006_1"), 0, STRING("店長"))
                END_IF
            ELSE
                CALL(evt_msg_print_add, 0, STRING("irai_09_004"))
                CALL(evt_item_entry, STRING("uk"), ITEM_ULTRA_SHROOM, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_item_get_item, STRING("uk"))
                CALL(evt_item_get_item_end_wait, STRING("uk"))
                CALL(evt_msg_print, 0, STRING("irai_09_006"), 0, STRING("店長"))
                SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_ORDER_ME_AN_ITEM), 1)
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

/** Items offered in the item menu for Trouble Center job 16: only Attack FX B. */
s32 iri_16_item_tbl[] = {
    ITEM_ATTACK_FX_B_KEY_ITEM, -1,
};

/**
 * Map-init part of Trouble Center request 16: while it is active,
 * enters Ms. Mowz ("c_tyutyu") at (-516, 189, -110) facing 90 with iri_16_tyutyu_talk as her talk event.
 */
static EVT_BEGIN(iri_16_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ELUSIVE_BADGE)
        CALL(evt_npc_entry, STRING("チュチュリーナ"), STRING("c_tyutyu"))
        CALL(evt_npc_set_tribe, STRING("チュチュリーナ"), STRING("チュチュリーナ"))
        CALL(evt_npc_set_position, STRING("チュチュリーナ"), -516, 189, -110)
        CALL(evt_npc_set_ry, STRING("チュチュリーナ"), 90)
        CALL(evt_npc_flag_onoff, 1, STRING("チュチュリーナ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_change_interrupt, STRING("チュチュリーナ"), 6, PTR(iri_16_tyutyu_talk))
    END_IF
    RETURN
EVT_END()

/**
 * Trouble Center job 16: Ms. Mowz asks Mario for an item (the Mowz request key item),
 * kisses him when he hands it over and rewards him with the Attack FX B badge,
 * which she leaves in front of the badge shop if his badge list is already full (200 badges).
 * She then joins the party as a partner (all_party_lecture for partner 7).
 * The "Ms. Mowz" theme plays during the talk.
 */
EVT_BEGIN(iri_16_tyutyu_talk)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ELUSIVE_BADGE)

        // Ms. Mowz's own theme replaces the plaza music for the whole talk.
        CALL(evt_snd_bgmoff_f, PSND_PAUSE, 2000)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_CYUCYURINA1"))

        // First talk: she explains what she is looking for.
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)
            CALL(evt_msg_print, 0, STRING("irai_16_00"), 0, STRING("チュチュリーナ"))
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
        ELSE
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)

                // Mario has not found it yet: she repeats her request.
                CALL(evt_msg_print, 0, STRING("irai_16_01"), 0, STRING("チュチュリーナ"))
            ELSE

                // Mario has the item: he and the partner walk up to her.
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

                // She asks for the item; Mario picks it from the item menu.
                CALL(evt_msg_print, 0, STRING("irai_16_02"), 0, STRING("チュチュリーナ"))
#if VERSION <= VERSION_DEMO_JP
                CALL(evt_win_item_select, PTR(iri_16_item_tbl), LW(0))
#else
                CALL(evt_win_item_select, 0, PTR(iri_16_item_tbl), LW(0), 0)
#endif
                IF_INT_NOT_EQUAL(LW(0), -1)

                    // Mario hands it over and Ms. Mowz is overjoyed.
                    CALL(evt_pouch_remove_item, ITEM_ATTACK_FX_B_KEY_ITEM, LW(0))
                    CALL(evt_msg_print, 0, STRING("irai_16_03"), 0, STRING("チュチュリーナ"))
                    CALL(evt_npc_set_anim, STRING("チュチュリーナ"), STRING("PCH_C_1"))
                    WAIT_MS(1000)
                    CALL(evt_msg_print, 0, STRING("irai_16_04"), 0, STRING("チュチュリーナ"))
                    CALL(evt_mario_set_pose, STRING("M_N_5B"))
                    WAIT_MS(600)
                    CALL(evt_mario_set_pose, STRING("M_S_1"))
                    CALL(evt_msg_print, 0, STRING("irai_16_05"), 0, STRING("チュチュリーナ"))

                    // Her reward is the Attack FX B badge, handed over only if Mario has room for another
                    // badge.
                    CALL(evt_pouch_get_havebadgecnt, LW(0))
                    IF_INT_LESS(LW(0), MAX_BADGES)
                        CALL(evt_msg_continue)
                        CALL(evt_item_entry, STRING("mb"), ITEM_ATTACK_FX_B, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                        CALL(evt_item_get_item, STRING("mb"))
                        CALL(evt_item_get_item_end_wait, STRING("mb"))
                        SET_VALUE(GSWF_GOR_01_MS_MOWZ_ATTACK_FX_B, 1)
                    ELSE
                        // Badge list full: she hops over the rooftops and leaves the badge in front of the badge
                        // shop for Mario to pick up later.
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
                        CALL(evt_item_entry, STRING("iri_16"), ITEM_ATTACK_FX_B, -150, 153, -120, ITEM_TYPE_STILL, GSWF_GOR_01_MS_MOWZ_ATTACK_FX_B, 0)
                    END_IF

                    // She thanks Mario with a kiss.
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
#if VERSION >= VERSION_US
                    CALL(evt_cam3d_evt_set, -516, 290, 230, -516, 225, -140, 800, EASING_SINE_IN_OUT)
#else
                    CALL(evt_cam3d_evt_set, -525, 290, 230, -525, 225, -140, 800, EASING_SINE_IN_OUT)
#endif
                    WAIT_MS(800)

                    // Ms. Mowz joins the party and the partner lecture plays.
                    SET_VALUE(LW(8), STRING("チュチュリーナ"))
                    SET_VALUE(LW(9), PARTY_MEMBER_MS_MOWZ)
#if VERSION >= VERSION_US
                    CALL_CHILD(all_party_lecture)
#else
                    CALL_CHILD(all_party_lecture)
#endif
                    SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)

                    // The request is marked solved and the Trouble Center completion message appears.
                    SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_ELUSIVE_BADGE), 1)
                    CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
                    CALL(evt_party_run, 0)
                END_IF
                CALL(evt_party_run, 0)
            END_IF
        END_IF

        // Ms. Mowz's theme fades out and the plaza music returns.
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        RETURN
    END_IF
    RETURN
EVT_END()

/**
 * Case event for the Skull Gem item (A_jewel): shows Marco's (Flavio's) line mac_4_006 about it
 * and restarts his muttering.
 */
EVT_BEGIN(marco_jewel)
    CALL(evt_mario_key_onoff, 0)
    DELETE_EVENT(GW(0))
    CALL(evt_msg_close, STRING("mac_4_004"))
    CALL(evt_msg_print, 0, STRING("mac_4_006"), 0, STRING("マルコ"))
    SPAWN_EVENT_GET_ID(PTR(marco_hitorigoto), GW(0))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Enters Goombella ("c_pkr") at (55, 154, -110) facing 270
 * and Ms. Mowz ("c_tyutyu", hidden below the map) for the ending scene in the plaza,
 * both intangible and with move animation.
 */
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

/**
 * Background part of the badge shop ending scene:
 * opens the bar's upstairs door and moves Goombella to x -50, then opens the badge shop door
 * (door_data_03_f2, then door_data_02_f2) and moves the owner out to face her, then loops the badge shop owner's and Goombella's talk animations.
 */
static EVT_BEGIN(epilogue_evt_sub)
    SET_VALUE(LW(0), PTR(&door_data_03_f2))
    SPAWN_EVENT(door_open_se)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(evt_door_03_00_f2)
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
    SPAWN_EVENT(door_close_se)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(evt_door_03_00_f2)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_WHILE(GF(0))
    SET_VALUE(LW(0), PTR(&door_data_02_f2))
    SPAWN_EVENT(door_open_se)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 0, 180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(evt_door_02_00_f2)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_move_position, STRING("バッジ店長"), -114, -100, 0, 60, 0)
    CALL(evt_set_dir_to_target, STRING("バッジ店長"), STRING("クリスチーヌ"))
    SET_VALUE(LW(0), PTR(&door_data_02_f2))
    SPAWN_EVENT(door_close_se)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        SPAWN_EVENT(evt_door_02_00_f2)
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

/**
 * The ending scene at the badge shop: the owner and Goombella meet (ep_43) with the shop camera,
 * then the owner and Ms. Mowz spin around together and swap places, hiding the owner and showing Ms. Mowz,
 * who jumps away across the rooftops while Goombella turns to follow; the game then changes to pik_04.
 */
static EVT_BEGIN(epilogue_evt)

    // The epilogue: Ms. Mowz's theme starts and the badge shop's rooms are set up for the scene.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_CYUCYURINA1"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_npc_set_position, STRING("バッジ店長"), -114, 154, -173)
    CALL(evt_npc_flag_onoff, 1, STRING("バッジ店長"), NPC_FLAG_NO_HIT | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_02_f2_naka"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_unitwin_alloc)
#else
    CALL(evt_unitwin_alloc, 1)
#endif
    CALL(badgeShop_getBargainTable, LW(0))
    CALL(evt_unitwin_selltable_setup, PTR(uwnobj_list), LW(0))
    CALL(evt_cam3d_evt_set, -80, 225, 400, -80, 200, -100, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    SET_VALUE(GF(15), 1)
    SPAWN_EVENT_GET_ID(PTR(epilogue_evt_sub), LW(0))
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)

    // The narrator tells what became of Ms. Mowz and the badge shop.
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

    // The shop owner and Ms. Mowz spin together and trade places, revealing the owner as Ms. Mowz.
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

    // The camera pulls back as Ms. Mowz leaps away across the rooftops.
    CALL(evt_cam3d_evt_set, 0, 275, 1397, 0, 200, -100, 0, EASING_SINE_IN_OUT)
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

    // Music fades, then a fade to black and on to the next scene.
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
    CALL(evt_snd_envoff_f, PSND_FADEOUT, 3000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 900, FADE_IN_BLACK, 900)
    CALL(evt_bero_mapchange, STRING("pik_04"), STRING(""))
    RETURN
EVT_END()

#if VERSION >= VERSION_US
/**
 * Loops forever: hides the stage platform pieces "S_dai" while Mario stands near x -60 to 60,
 * z -300 to -200 (the area near the fountain); shows them otherwise. US and later releases.
 */
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
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_dai"), MAP_OBJ_FLAG_HIDDEN)
        ELSE
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_dai"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * Map init for gor_01 (Rogueport plaza).
 * Chooses the music by story stage, sets up the exits (east to gor_02, west to gor_03, south to the harbor),
 * creates the NPCs, buildings' doors, shops, the save block, the star pieces and chests,
 * and starts whichever story scene belongs to the current stage:
 * Goombella joining, the Skull Gem and Ultra Hammer events, Bowser's visit,
 * the Shadow Queen's darkness scenes and the epilogue.
 */
EVT_BEGIN(gor_01_init_evt)

    // Work out which special scene this visit belongs to: Bowser's visit in Chapter 6 or the darkness scenes in
    // Chapter 8.
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_55_01_PEACH_TEC_DELETED)
        CALL(evt_mario_set_mode, 2)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            SET_VALUE(LF(1), 1)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SET_VALUE(LF(1), 1)
        END_IF
    END_IF

    // Choose the music and ambience: darkness themes, Bowser's theme, or the plaza theme.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR1"))
    ELSE
        IF_INT_EQUAL(LF(0), 0)
            IF_INT_EQUAL(LF(1), 1)
                CALL(evt_bero_get_entername, LW(0))
                IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
                    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_WITCH"))
                    CALL(evt_snd_envoff, PSND_FADEOUT)
                END_IF
                IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
                    CALL(evt_snd_bgmoff, PSND_FADEOUT)
                    CALL(evt_snd_envoff, PSND_FADEOUT)
                    SET_VALUE(LF(1), 1)
                END_IF
            ELSE
                CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_GOR1"))
                CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
                CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR1"))
            END_IF
        ELSE
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_KUPPA1"))
            CALL(evt_snd_bgm_scope, 0, 1)
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_GOR1"))
        END_IF
    END_IF

    // Register the exits and, if Goombella is about to join, prepare her scene's NPCs.
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG0_06_1000_X_NAUTS_AFTER)
        CALL_CHILD(christine_nakama_init)
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG0_07_GOOMBELLA_JOINS)
        IF_INT_EQUAL(GSWF_GOR_01_SAVE_REMINDER_DONE, 0)
            SPAWN_EVENT(christine_nakama_after)
        END_IF
    END_IF

    // The ordered contact lens arrives once the story has moved on or enough time has passed.
    IF_INT_EQUAL(GSWF_GOR_CONTACT_LENS_ARRIVED, 0)
        IF_INT_EQUAL(GSWF_GOR_CONTACT_LENS_ORDERED_EARLY, 1)
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
                SET_VALUE(GSWF_GOR_CONTACT_LENS_ARRIVED, 1)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF_GOR_CONTACT_LENS_ARRIVED, 0)
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG1_17_FATHER_SON_REUNITED)
            SET_VALUE(LW(0), GSWF_GOR_CONTACT_LENS_ORDERED_EARLY)
            ADD(LW(0), GSWF_GOR_CONTACT_LENS_ORDERED_LATE)
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(evt_sub_load_progresstime, GSW_GOR_CONTACT_LENS_ORDER_TIME, LW(0))
                IF_INT_GREATER_OR_EQUAL(LW(0), 90000)
                    SET_VALUE(GSWF_GOR_CONTACT_LENS_ARRIVED, 1)
                END_IF
            END_IF
        END_IF
    END_IF

    // Create the plaza NPCs; Flavio gets his footstep sounds.
    CALL_CHILD(luigi_npcEnt)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL(evt_npc_sound_data_set, STRING("マルコ"), STRING("SFX_STG5_MARCO_MOVE1L"), STRING("SFX_STG5_MARCO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    IF_INT_EQUAL(LF(0), 1)
        CALL_CHILD(koopa_npc_entry)
    END_IF

    // The Save Block stands in the plaza except during Bowser's visit.
    IF_INT_NOT_EQUAL(LF(0), 1)
        CALL(evt_mobj_save_blk, STRING("mobj_save"), 185, 60, -55, 0, 0)
    END_IF

    // Star Piece panels and the hidden Star Pieces.
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -455, 0, -260, STRING("S_ground"), 125, GSWF_STARPIECE_02)
    CALL(evt_mobj_kururing_floor, STRING("kururin2"), 0, 0, 450, STRING("S_ground"), 125, GSWF_STARPIECE_06)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, 490, 0, 245, ITEM_TYPE_STILL, GSWF_STARPIECE_03, 0)
    CALL(evt_item_entry, STRING("item_01"), ITEM_STAR_PIECE, 550, 0, -150, ITEM_TYPE_STILL, GSWF_STARPIECE_04, 0)
    CALL(evt_item_entry, STRING("item_02"), ITEM_STAR_PIECE, -565, 162, -160, ITEM_TYPE_STILL, GSWF_STARPIECE_05, 0)

    // Until Flavio's talk the Skull Gem lies in the bar.
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_32_CHATTING_WITH_FLAVIO)
        CALL(evt_item_entry, STRING("ドクロジュエル"), ITEM_SKULL_GEM, 315, 20, -125, ITEM_TYPE_STILL, -1, 0)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_item_flag_on, STRING("ドクロジュエル"), 2)
#else
        CALL(evt_item_flag_on, STRING("ドクロジュエル"), ITEM_FLAG_NO_TOUCH_GET)
        CALL(evt_item_status_on, STRING("ドクロジュエル"), ITEM_FLAG2_NO_PARTNER)
#endif
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_jewel"), 0, PTR(marco_jewel), GW(8))
    END_IF

    // Set up the building doors, depending on story progress and whether the lens was delivered.
    IF_INT_EQUAL(GSWF_GOR_ZESS_T_GOT_CONTACT_LENS, 0)
        SET_VALUE(LW(0), PTR(&door_data_01))
    ELSE
        SET_VALUE(LW(0), PTR(&door_data_01_01))
    END_IF
    CALL(evt_door_data_copy, LW(0), GW(6))
    SET_VALUE(LW(0), GW(6))
    CALL_CHILD(evt_door_entry)
    SET_VALUE(LW(0), PTR(&door_data_02_f1))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG4_32_CHATTING_WITH_FLAVIO)
        IF_INT_EQUAL(GSWF_GOR_CONTACT_LENS_ARRIVED, 1)
            SET_VALUE(LW(0), PTR(&door_data_02_f1_01))
        END_IF
    END_IF
    CALL_CHILD(evt_door_entry)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG0_16_FRANKLYS_HOUSE)
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

    // Shops, townsfolk and ambient animations.
    CALL_CHILD(gor_01_shop_setup)
    CALL(evt_kinopio_setup, PTR(&kino_dt))
    CALL(evt_map_playanim, STRING("hae"), MAP_ANIM_PLAY_LOOP, 0)
    SPAWN_EVENT(koushukei_dai)
    SPAWN_EVENT(gate_disp)
    SPAWN_EVENT(gor_goki1_urouro)
    SPAWN_EVENT(gor_goki2_urouro)

    // Trouble Center request items and NPCs, except during the darkness scenes.
    IF_INT_EQUAL(LF(1), 0)
        IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_NEED_A_KEY)
            CALL(evt_item_entry, STRING("item_key00"), ITEM_HOUSE_KEY, -280, 153, -100, ITEM_TYPE_GROUND, GSWF_TROUBLE_WORK(0), 0)
        END_IF
        CALL_CHILD(borodo2_init)
        SPAWN_EVENT(iri_09_init)
        SPAWN_EVENT(iri_16_init)
        IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_ELUSIVE_BADGE), 1)
            CALL(evt_item_entry, STRING("iri_16"), ITEM_ATTACK_FX_B, -150, 153, -120, ITEM_TYPE_STILL, GSWF_GOR_01_MS_MOWZ_ATTACK_FX_B, 0)
        END_IF
        IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_SEEKING_LEGENDARY_BOOK), 1)
            CALL(evt_item_entry, STRING("iri_18"), ITEM_HONEY_SHROOM, -440, 30, -80, ITEM_TYPE_STILL, GSWF_GOR_01_ZESS_T_HONEY_SHROOM, 0)
        END_IF
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_keizi_mae"), 0, PTR(keijiban_omote_joho), 0)
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_keizi_ura"), 0, PTR(keijiban_ura_joho), 0)

    // The Ultra Hammer chest: on the roof until it has fallen, then on the ground.
    CALL(evt_pouch_check_item, ITEM_ULTRA_HAMMER, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        IF_INT_EQUAL(GSWF_GOR_01_ULTRA_HAMMER_CHEST_FELL, 0)
#if VERSION <= VERSION_DEMO_JP
            CALL(evt_mobj_itembox, STRING("mobj_box00"), 340, 146, -20, MOBJ_ITEMBOX_BIG, 0, PTR(ultrahammer_get), GSWF_GOR_01_ULTRA_HAMMER_CHEST)
#else
            CALL(evt_mobj_itembox, STRING("mobj_box00"), 340, 110, -20, MOBJ_ITEMBOX_BIG, 0, PTR(ultrahammer_get), GSWF_GOR_01_ULTRA_HAMMER_CHEST)
#endif
            SPAWN_EVENT(ultrahammer_check)
        ELSE
            CALL(evt_mobj_itembox, STRING("mobj_box00"), 275, 0, 75, MOBJ_ITEMBOX_BIG, 0, PTR(ultrahammer_get), GSWF_GOR_01_ULTRA_HAMMER_CHEST)
        END_IF
    END_IF

    // Map object culling for the fence, stage and houses.
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_saku"), MAP_OBJ_FLAG_CLIP)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_dai"), MAP_OBJ_FLAG_CLIP)
#if VERSION >= VERSION_US
    SPAWN_EVENT(dai_disp)
#endif
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_02_f1A"), MAP_OBJ_FLAG_CLIP)

    // Chapter 6: Bowser arrives and the Bowser interlude starts.
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mario_kill_party, 0)
        SPAWN_EVENT(gor_01_koopa_evt)
    END_IF

    // Chapter 8: start the matching darkness scene.
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("sekai_yami1"))
            SPAWN_EVENT(sekai_yami_evt)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SPAWN_EVENT(minnnanokoe_evt)
        END_IF
    END_IF

    // After the final battle: the epilogue scene starts.
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_END_AFTER_THE_BATTLE)
        CALL(evt_mario_kill_party, 0)
        CALL_CHILD(epilogue_npc_entry)
        SPAWN_EVENT(epilogue_evt)
    END_IF

    // Plaza mail delivery event.
    SPAWN_EVENT(evt_sub_mail_event)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_roten"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_roten"), 1, 0)
    RETURN
EVT_END()

/**
 * Zess T.'s one-ingredient recipes: for each ingredient, the dish made from it by itself before and after the
 * cookbook is solved.
 */
#if VERSION >= VERSION_JP
SimpleRecipe R_simple_recipe_table[] = {
    {"ファイアフラワー", "あつあつスープ", "あつあつスープ"}, // ITEM_FIRE_FLOWER -> ITEM_SPICY_SOUP
    {"びりびりキノコ", "キノコいため", "キノコいため"}, // ITEM_VOLT_SHROOM -> ITEM_SHROOM_FRY
    {"キノコ", "キノコいため", "キノコいため"}, // ITEM_MUSHROOM -> ITEM_SHROOM_FRY
    {"スーパーキノコ", "キノコいため", "キノコいため"}, // ITEM_SUPER_SHROOM -> ITEM_SHROOM_FRY
    {"ウルトラキノコ", "キノコステーキ", "キノコステーキ"}, // ITEM_ULTRA_SHROOM -> ITEM_SHROOM_STEAK
    {"きんきゅうキノコ", "キノコホイルやき", "キノコホイルやき"}, // ITEM_LIFE_SHROOM -> ITEM_SHROOM_ROAST
    {"しなびたキノコ", "", "キノコいため"}, // ITEM_DRIED_SHROOM -> ITEM_MISTAKE without the Cookbook (no result), ITEM_SHROOM_FRY with it
    {"ハニーシロップ", "フレッシュジュース", "フレッシュジュース"}, // ITEM_HONEY_SYRUP -> ITEM_FRESH_JUICE
    {"メイプルシロップ", "フレッシュジュース", "フレッシュジュース"}, // ITEM_MAPLE_SYRUP -> ITEM_FRESH_JUICE
    {"ローヤルゼリー", "フレッシュジュース", "フレッシュジュース"}, // ITEM_JAMMIN_JELLY -> ITEM_FRESH_JUICE
    {"じわじわキノコ", "キノコホイルやき", "キノコホイルやき"}, // ITEM_SLOW_SHROOM -> ITEM_SHROOM_ROAST
    {"じわじわシロップ", "フレッシュジュース", "フレッシュジュース"}, // ITEM_GRADUAL_SYRUP -> ITEM_FRESH_JUICE
    {"ドライフラワー", "", "あつあつスープ"}, // ITEM_DRIED_BOUQUET -> ITEM_MISTAKE without the Cookbook (no result), ITEM_SPICY_SOUP with it
    {"ふしぎなタマゴ", "めだまやき", "めだまやき"}, // ITEM_MYSTIC_EGG -> ITEM_FRIED_EGG
    {"おうごんはっぱ", "ナンシーティー", "ナンシーティー"}, // ITEM_GOLDEN_LEAF -> ITEM_ZESS_TEA
    {"トロピコマンゴー", "フレッシュジュース", "フレッシュジュース"}, // ITEM_KEEL_MANGO -> ITEM_FRESH_JUICE
    {"生パスタ", "スパゲティ", "スパゲティ"}, // ITEM_FRESH_PASTA -> ITEM_SPAGHETTI
    {"おかしのもと", "ムースケーキ", "ムースケーキ"}, // ITEM_CAKE_MIX -> ITEM_MOUSSE_CAKE
    {"レッド・カラリン", "", "スッキリドリンク"}, // ITEM_HOT_SAUCE -> ITEM_MISTAKE without the Cookbook (no result), ITEM_TASTY_TONIC with it
    {"カメカメはっぱ", "カメカメティー", "カメカメティー"}, // ITEM_TURTLEY_LEAF -> ITEM_KOOPA_TEA
    {"つくしんぼ", "", "あつあつスープ"}, // ITEM_HORSETAIL -> ITEM_MISTAKE without the Cookbook (no result), ITEM_SPICY_SOUP with it
    {"ピチピーチ", "フレッシュジュース", "フレッシュジュース"}, // ITEM_PEACHY_PEACH -> ITEM_FRESH_JUICE
    {"ヤシの実", "スッキリドリンク", "スッキリドリンク"}, // ITEM_COCONUT -> ITEM_TASTY_TONIC
    {"ゆきうさぎ", "", "あつあつスープ"}, // ITEM_SNOW_BUNNY -> ITEM_MISTAKE without the Cookbook (no result), ITEM_SPICY_SOUP with it
    {"どくキノコ", "", "キノコいため"}, // ITEM_POISON_SHROOM -> ITEM_MISTAKE without the Cookbook (no result), ITEM_SHROOM_FRY with it
    {"たんこぶ", "", "ナンシースペシャル"}, // ITEM_WHACKA_BUMP -> ITEM_MISTAKE without the Cookbook (no result), ITEM_ZESS_SPECIAL with it
};
#endif

/**
 * Zess T.'s two-ingredient recipes: pairs of ingredients and the dish they make in JP and every later version (the early demos read cooking.xml instead).
 */
#if VERSION >= VERSION_JP
CookRecipe R_cookbook_recipe_table[] = {
#if VERSION < VERSION_US
    {"かみなりドッカン", "かみなりゴロゴロ", "しれんのナベ"}, // ITEM_THUNDER_RAGE + ITEM_THUNDER_BOLT -> ITEM_TRIAL_STEW
#endif
    {"キノコ", "かみなりゴロゴロ", "びりびりキノコ"}, // ITEM_MUSHROOM + ITEM_THUNDER_BOLT -> ITEM_VOLT_SHROOM
    {"キノコ", "かみなりドッカン", "びりびりキノコ"}, // ITEM_MUSHROOM + ITEM_THUNDER_RAGE -> ITEM_VOLT_SHROOM
    {"キノコ", "ファイアフラワー", "キノコホイルやき"}, // ITEM_MUSHROOM + ITEM_FIRE_FLOWER -> ITEM_SHROOM_ROAST
    {"キノコ", "びりびりキノコ", "キノコホイルやき"}, // ITEM_MUSHROOM + ITEM_VOLT_SHROOM -> ITEM_SHROOM_ROAST
    {"キノコ", "ふにゃふにゃくん", "しなびたキノコ"}, // ITEM_MUSHROOM + ITEM_MR_SOFTENER -> ITEM_DRIED_SHROOM
    {"スーパーキノコ", "かみなりゴロゴロ", "びりびりキノコ"}, // ITEM_SUPER_SHROOM + ITEM_THUNDER_BOLT -> ITEM_VOLT_SHROOM
    {"スーパーキノコ", "かみなりドッカン", "びりびりキノコ"}, // ITEM_SUPER_SHROOM + ITEM_THUNDER_RAGE -> ITEM_VOLT_SHROOM
    {"スーパーキノコ", "ファイアフラワー", "ナンシーディナー"}, // ITEM_SUPER_SHROOM + ITEM_FIRE_FLOWER -> ITEM_ZESS_DINNER
    {"スーパーキノコ", "びりびりキノコ", "キノコホイルやき"}, // ITEM_SUPER_SHROOM + ITEM_VOLT_SHROOM -> ITEM_SHROOM_ROAST
    {"スーパーキノコ", "ふにゃふにゃくん", "しなびたキノコ"}, // ITEM_SUPER_SHROOM + ITEM_MR_SOFTENER -> ITEM_DRIED_SHROOM
    {"スーパーキノコ", "キノコ", "キノコホイルやき"}, // ITEM_SUPER_SHROOM + ITEM_MUSHROOM -> ITEM_SHROOM_ROAST
    {"ウルトラキノコ", "かみなりゴロゴロ", "びりびりキノコ"}, // ITEM_ULTRA_SHROOM + ITEM_THUNDER_BOLT -> ITEM_VOLT_SHROOM
    {"ウルトラキノコ", "かみなりドッカン", "びりびりキノコ"}, // ITEM_ULTRA_SHROOM + ITEM_THUNDER_RAGE -> ITEM_VOLT_SHROOM
    {"ウルトラキノコ", "ファイアフラワー", "ナンシースペシャル"}, // ITEM_ULTRA_SHROOM + ITEM_FIRE_FLOWER -> ITEM_ZESS_SPECIAL
    {"ウルトラキノコ", "びりびりキノコ", "キノコステーキ"}, // ITEM_ULTRA_SHROOM + ITEM_VOLT_SHROOM -> ITEM_SHROOM_STEAK
    {"ウルトラキノコ", "ふにゃふにゃくん", "しなびたキノコ"}, // ITEM_ULTRA_SHROOM + ITEM_MR_SOFTENER -> ITEM_DRIED_SHROOM
    {"ウルトラキノコ", "キノコ", "キノコステーキ"}, // ITEM_ULTRA_SHROOM + ITEM_MUSHROOM -> ITEM_SHROOM_STEAK
    {"ウルトラキノコ", "スーパーキノコ", "キノコステーキ"}, // ITEM_ULTRA_SHROOM + ITEM_SUPER_SHROOM -> ITEM_SHROOM_STEAK
    {"きんきゅうキノコ", "かみなりゴロゴロ", "びりびりキノコ"}, // ITEM_LIFE_SHROOM + ITEM_THUNDER_BOLT -> ITEM_VOLT_SHROOM
    {"きんきゅうキノコ", "かみなりドッカン", "びりびりキノコ"}, // ITEM_LIFE_SHROOM + ITEM_THUNDER_RAGE -> ITEM_VOLT_SHROOM
    {"きんきゅうキノコ", "ファイアフラワー", "ナンシーディナー"}, // ITEM_LIFE_SHROOM + ITEM_FIRE_FLOWER -> ITEM_ZESS_DINNER
    {"きんきゅうキノコ", "びりびりキノコ", "キノコホイルやき"}, // ITEM_LIFE_SHROOM + ITEM_VOLT_SHROOM -> ITEM_SHROOM_ROAST
    {"きんきゅうキノコ", "ふにゃふにゃくん", "しなびたキノコ"}, // ITEM_LIFE_SHROOM + ITEM_MR_SOFTENER -> ITEM_DRIED_SHROOM
    {"きんきゅうキノコ", "キノコ", "キノコステーキ"}, // ITEM_LIFE_SHROOM + ITEM_MUSHROOM -> ITEM_SHROOM_STEAK
    {"きんきゅうキノコ", "スーパーキノコ", "キノコステーキ"}, // ITEM_LIFE_SHROOM + ITEM_SUPER_SHROOM -> ITEM_SHROOM_STEAK
    {"きんきゅうキノコ", "ウルトラキノコ", "キノコステーキ"}, // ITEM_LIFE_SHROOM + ITEM_ULTRA_SHROOM -> ITEM_SHROOM_STEAK
    {"しなびたキノコ", "ファイアフラワー", "キノコいため"}, // ITEM_DRIED_SHROOM + ITEM_FIRE_FLOWER -> ITEM_SHROOM_FRY
    {"しなびたキノコ", "びりびりキノコ", "キノコいため"}, // ITEM_DRIED_SHROOM + ITEM_VOLT_SHROOM -> ITEM_SHROOM_FRY
    {"しなびたキノコ", "デカデカドリンク", "キノコ"}, // ITEM_DRIED_SHROOM + ITEM_POWER_PUNCH -> ITEM_MUSHROOM
    {"しなびたキノコ", "キノコ", "キノコいため"}, // ITEM_DRIED_SHROOM + ITEM_MUSHROOM -> ITEM_SHROOM_FRY
    {"しなびたキノコ", "スーパーキノコ", "キノコホイルやき"}, // ITEM_DRIED_SHROOM + ITEM_SUPER_SHROOM -> ITEM_SHROOM_ROAST
    {"しなびたキノコ", "ウルトラキノコ", "キノコステーキ"}, // ITEM_DRIED_SHROOM + ITEM_ULTRA_SHROOM -> ITEM_SHROOM_STEAK
    {"しなびたキノコ", "きんきゅうキノコ", "キノコステーキ"}, // ITEM_DRIED_SHROOM + ITEM_LIFE_SHROOM -> ITEM_SHROOM_STEAK
    {"ハニーシロップ", "こおりのいぶき", "アイスキャンディ"}, // ITEM_HONEY_SYRUP + ITEM_ICE_STORM -> ITEM_ICICLE_POP
    {"ハニーシロップ", "びりびりキノコ", "ハニーキノコ"}, // ITEM_HONEY_SYRUP + ITEM_VOLT_SHROOM -> ITEM_HONEY_SHROOM
    {"ハニーシロップ", "キノコ", "ハニーキノコ"}, // ITEM_HONEY_SYRUP + ITEM_MUSHROOM -> ITEM_HONEY_SHROOM
    {"ハニーシロップ", "スーパーキノコ", "ハニーキノコＳ"}, // ITEM_HONEY_SYRUP + ITEM_SUPER_SHROOM -> ITEM_HONEY_SUPER
    {"ハニーシロップ", "ウルトラキノコ", "ハニーキノコＺ"}, // ITEM_HONEY_SYRUP + ITEM_ULTRA_SHROOM -> ITEM_HONEY_ULTRA
    {"ハニーシロップ", "きんきゅうキノコ", "ハニーキノコＳ"}, // ITEM_HONEY_SYRUP + ITEM_LIFE_SHROOM -> ITEM_HONEY_SUPER
    {"メイプルシロップ", "こおりのいぶき", "ナンシーフラッペ"}, // ITEM_MAPLE_SYRUP + ITEM_ICE_STORM -> ITEM_ZESS_FRAPPE
    {"メイプルシロップ", "びりびりキノコ", "メイプルキノコ"}, // ITEM_MAPLE_SYRUP + ITEM_VOLT_SHROOM -> ITEM_MAPLE_SHROOM
    {"メイプルシロップ", "キノコ", "メイプルキノコ"}, // ITEM_MAPLE_SYRUP + ITEM_MUSHROOM -> ITEM_MAPLE_SHROOM
    {"メイプルシロップ", "スーパーキノコ", "メイプルキノコＳ"}, // ITEM_MAPLE_SYRUP + ITEM_SUPER_SHROOM -> ITEM_MAPLE_SUPER
    {"メイプルシロップ", "ウルトラキノコ", "メイプルキノコＺ"}, // ITEM_MAPLE_SYRUP + ITEM_ULTRA_SHROOM -> ITEM_MAPLE_ULTRA
    {"メイプルシロップ", "きんきゅうキノコ", "メイプルキノコＳ"}, // ITEM_MAPLE_SYRUP + ITEM_LIFE_SHROOM -> ITEM_MAPLE_SUPER
    {"メイプルシロップ", "ハニーシロップ", "フレッシュジュース"}, // ITEM_MAPLE_SYRUP + ITEM_HONEY_SYRUP -> ITEM_FRESH_JUICE
    {"ローヤルゼリー", "こおりのいぶき", "ナンシーフラッペ"}, // ITEM_JAMMIN_JELLY + ITEM_ICE_STORM -> ITEM_ZESS_FRAPPE
    {"ローヤルゼリー", "びりびりキノコ", "ローヤルキノコ"}, // ITEM_JAMMIN_JELLY + ITEM_VOLT_SHROOM -> ITEM_JELLY_SHROOM
    {"ローヤルゼリー", "キノコ", "ローヤルキノコ"}, // ITEM_JAMMIN_JELLY + ITEM_MUSHROOM -> ITEM_JELLY_SHROOM
    {"ローヤルゼリー", "スーパーキノコ", "ローヤルキノコＳ"}, // ITEM_JAMMIN_JELLY + ITEM_SUPER_SHROOM -> ITEM_JELLY_SUPER
    {"ローヤルゼリー", "ウルトラキノコ", "ローヤルキノコＺ"}, // ITEM_JAMMIN_JELLY + ITEM_ULTRA_SHROOM -> ITEM_JELLY_ULTRA
    {"ローヤルゼリー", "きんきゅうキノコ", "ローヤルキノコＳ"}, // ITEM_JAMMIN_JELLY + ITEM_LIFE_SHROOM -> ITEM_JELLY_SUPER
    {"ローヤルゼリー", "ハニーシロップ", "フレッシュジュース"}, // ITEM_JAMMIN_JELLY + ITEM_HONEY_SYRUP -> ITEM_FRESH_JUICE
    {"ローヤルゼリー", "メイプルシロップ", "ナンシーティー"}, // ITEM_JAMMIN_JELLY + ITEM_MAPLE_SYRUP -> ITEM_ZESS_TEA
    {"じわじわキノコ", "かみなりゴロゴロ", "びりびりキノコ"}, // ITEM_SLOW_SHROOM + ITEM_THUNDER_BOLT -> ITEM_VOLT_SHROOM
    {"じわじわキノコ", "かみなりドッカン", "びりびりキノコ"}, // ITEM_SLOW_SHROOM + ITEM_THUNDER_RAGE -> ITEM_VOLT_SHROOM
    {"じわじわキノコ", "キラキラおとし", "ナンシーディナー"}, // ITEM_SLOW_SHROOM + ITEM_SHOOTING_STAR -> ITEM_ZESS_DINNER
    {"じわじわキノコ", "ファイアフラワー", "ナンシーディナー"}, // ITEM_SLOW_SHROOM + ITEM_FIRE_FLOWER -> ITEM_ZESS_DINNER
    {"じわじわキノコ", "びりびりキノコ", "キノコホイルやき"}, // ITEM_SLOW_SHROOM + ITEM_VOLT_SHROOM -> ITEM_SHROOM_ROAST
    {"じわじわキノコ", "キノコ", "キノコホイルやき"}, // ITEM_SLOW_SHROOM + ITEM_MUSHROOM -> ITEM_SHROOM_ROAST
    {"じわじわキノコ", "スーパーキノコ", "ナンシーディナー"}, // ITEM_SLOW_SHROOM + ITEM_SUPER_SHROOM -> ITEM_ZESS_DINNER
    {"じわじわキノコ", "ウルトラキノコ", "ナンシースペシャル"}, // ITEM_SLOW_SHROOM + ITEM_ULTRA_SHROOM -> ITEM_ZESS_SPECIAL
    {"じわじわキノコ", "きんきゅうキノコ", "ナンシーディナー"}, // ITEM_SLOW_SHROOM + ITEM_LIFE_SHROOM -> ITEM_ZESS_DINNER
    {"じわじわキノコ", "ハニーシロップ", "ハニーキノコ"}, // ITEM_SLOW_SHROOM + ITEM_HONEY_SYRUP -> ITEM_HONEY_SHROOM
    {"じわじわキノコ", "メイプルシロップ", "メイプルキノコ"}, // ITEM_SLOW_SHROOM + ITEM_MAPLE_SYRUP -> ITEM_MAPLE_SHROOM
#if VERSION >= VERSION_US
    {"じわじわキノコ", "ローヤルゼリー", "ローヤルキノコＳ"}, // ITEM_SLOW_SHROOM + ITEM_JAMMIN_JELLY -> ITEM_JELLY_SUPER
#else
    {"じわじわキノコ", "ローヤルゼリー", "メイプルキノコＳ"}, // ITEM_SLOW_SHROOM + ITEM_JAMMIN_JELLY -> ITEM_MAPLE_SUPER
#endif
    {"じわじわシロップ", "キノコ", "キノコホイルやき"}, // ITEM_GRADUAL_SYRUP + ITEM_MUSHROOM -> ITEM_SHROOM_ROAST
    {"じわじわシロップ", "スーパーキノコ", "ナンシーディナー"}, // ITEM_GRADUAL_SYRUP + ITEM_SUPER_SHROOM -> ITEM_ZESS_DINNER
    {"じわじわシロップ", "ウルトラキノコ", "ナンシースペシャル"}, // ITEM_GRADUAL_SYRUP + ITEM_ULTRA_SHROOM -> ITEM_ZESS_SPECIAL
    {"じわじわシロップ", "きんきゅうキノコ", "ナンシーディナー"}, // ITEM_GRADUAL_SYRUP + ITEM_LIFE_SHROOM -> ITEM_ZESS_DINNER
    {"じわじわシロップ", "ハニーシロップ", "フレッシュジュース"}, // ITEM_GRADUAL_SYRUP + ITEM_HONEY_SYRUP -> ITEM_FRESH_JUICE
    {"じわじわシロップ", "メイプルシロップ", "フレッシュジュース"}, // ITEM_GRADUAL_SYRUP + ITEM_MAPLE_SYRUP -> ITEM_FRESH_JUICE
    {"じわじわシロップ", "ローヤルゼリー", "フレッシュジュース"}, // ITEM_GRADUAL_SYRUP + ITEM_JAMMIN_JELLY -> ITEM_FRESH_JUICE
    {"じわじわシロップ", "じわじわキノコ", "じわじわキノコ"}, // ITEM_GRADUAL_SYRUP + ITEM_SLOW_SHROOM -> ITEM_SLOW_SHROOM
    {"イレカエール", "かみなりゴロゴロ", "ゆらゆらじしん"}, // ITEM_POINT_SWAP + ITEM_THUNDER_BOLT -> ITEM_EARTH_QUAKE
    {"イレカエール", "かみなりドッカン", "キラキラおとし"}, // ITEM_POINT_SWAP + ITEM_THUNDER_RAGE -> ITEM_SHOOTING_STAR
    {"イレカエール", "キラキラおとし", "かみなりドッカン"}, // ITEM_POINT_SWAP + ITEM_SHOOTING_STAR -> ITEM_THUNDER_RAGE
    {"イレカエール", "こおりのいぶき", "ファイアフラワー"}, // ITEM_POINT_SWAP + ITEM_ICE_STORM -> ITEM_FIRE_FLOWER
    {"イレカエール", "ファイアフラワー", "こおりのいぶき"}, // ITEM_POINT_SWAP + ITEM_FIRE_FLOWER -> ITEM_ICE_STORM
    {"イレカエール", "ゆらゆらじしん", "かみなりゴロゴロ"}, // ITEM_POINT_SWAP + ITEM_EARTH_QUAKE -> ITEM_THUNDER_BOLT
    {"イレカエール", "テレサのふく", "てきヨケール"}, // ITEM_POINT_SWAP + ITEM_BOOS_SHEET -> ITEM_REPEL_CAPE
    {"イレカエール", "びりびりキノコ", "スーパーキノコ"}, // ITEM_POINT_SWAP + ITEM_VOLT_SHROOM -> ITEM_SUPER_SHROOM
    {"イレカエール", "てきヨケール", "テレサのふく"}, // ITEM_POINT_SWAP + ITEM_REPEL_CAPE -> ITEM_BOOS_SHEET
    {"イレカエール", "まどわせのこな", "しかえしのこな"}, // ITEM_POINT_SWAP + ITEM_RUIN_POWDER -> ITEM_SPITE_POUCH
    {"イレカエール", "しかえしのこな", "まどわせのこな"}, // ITEM_POINT_SWAP + ITEM_SPITE_POUCH -> ITEM_RUIN_POWDER
    {"イレカエール", "ねむれよいこよ", "スッキリドリンク"}, // ITEM_POINT_SWAP + ITEM_SLEEPY_SHEEP -> ITEM_TASTY_TONIC
    {"イレカエール", "デカデカドリンク", "ミニミニくん"}, // ITEM_POINT_SWAP + ITEM_POWER_PUNCH -> ITEM_MINI_MR_MINI
    {"イレカエール", "カチカチコウラ", "ふにゃふにゃくん"}, // ITEM_POINT_SWAP + ITEM_COURAGE_SHELL -> ITEM_MR_SOFTENER
    {"イレカエール", "ミニミニくん", "デカデカドリンク"}, // ITEM_POINT_SWAP + ITEM_MINI_MR_MINI -> ITEM_POWER_PUNCH
    {"イレカエール", "ふにゃふにゃくん", "カチカチコウラ"}, // ITEM_POINT_SWAP + ITEM_MR_SOFTENER -> ITEM_COURAGE_SHELL
    {"イレカエール", "キノコ", "ハニーシロップ"}, // ITEM_POINT_SWAP + ITEM_MUSHROOM -> ITEM_HONEY_SYRUP
    {"イレカエール", "スーパーキノコ", "メイプルシロップ"}, // ITEM_POINT_SWAP + ITEM_SUPER_SHROOM -> ITEM_MAPLE_SYRUP
    {"イレカエール", "ウルトラキノコ", "ローヤルゼリー"}, // ITEM_POINT_SWAP + ITEM_ULTRA_SHROOM -> ITEM_JAMMIN_JELLY
    {"イレカエール", "きんきゅうキノコ", "スーパーキノコ"}, // ITEM_POINT_SWAP + ITEM_LIFE_SHROOM -> ITEM_SUPER_SHROOM
    {"イレカエール", "しなびたキノコ", "キノコ"}, // ITEM_POINT_SWAP + ITEM_DRIED_SHROOM -> ITEM_MUSHROOM
    {"イレカエール", "スッキリドリンク", "ねむれよいこよ"}, // ITEM_POINT_SWAP + ITEM_TASTY_TONIC -> ITEM_SLEEPY_SHEEP
    {"イレカエール", "ハニーシロップ", "キノコ"}, // ITEM_POINT_SWAP + ITEM_HONEY_SYRUP -> ITEM_MUSHROOM
    {"イレカエール", "メイプルシロップ", "スーパーキノコ"}, // ITEM_POINT_SWAP + ITEM_MAPLE_SYRUP -> ITEM_SUPER_SHROOM
    {"イレカエール", "ローヤルゼリー", "ウルトラキノコ"}, // ITEM_POINT_SWAP + ITEM_JAMMIN_JELLY -> ITEM_ULTRA_SHROOM
    {"イレカエール", "じわじわキノコ", "どくキノコ"}, // ITEM_POINT_SWAP + ITEM_SLOW_SHROOM -> ITEM_POISON_SHROOM
    {"イレカエール", "じわじわシロップ", "ハニーシロップ"}, // ITEM_POINT_SWAP + ITEM_GRADUAL_SYRUP -> ITEM_HONEY_SYRUP
    {"ドライフラワー", "ファイアフラワー", "あつあつスープ"}, // ITEM_DRIED_BOUQUET + ITEM_FIRE_FLOWER -> ITEM_SPICY_SOUP
    {"ドライフラワー", "びりびりキノコ", "かみなりドッカン"}, // ITEM_DRIED_BOUQUET + ITEM_VOLT_SHROOM -> ITEM_THUNDER_RAGE
    {"ドライフラワー", "キノコ", "うちゅうしょく"}, // ITEM_DRIED_BOUQUET + ITEM_MUSHROOM -> ITEM_SPACE_FOOD
    {"ドライフラワー", "スーパーキノコ", "うちゅうしょく"}, // ITEM_DRIED_BOUQUET + ITEM_SUPER_SHROOM -> ITEM_SPACE_FOOD
    {"ドライフラワー", "ウルトラキノコ", "うちゅうしょく"}, // ITEM_DRIED_BOUQUET + ITEM_ULTRA_SHROOM -> ITEM_SPACE_FOOD
    {"ドライフラワー", "きんきゅうキノコ", "うちゅうしょく"}, // ITEM_DRIED_BOUQUET + ITEM_LIFE_SHROOM -> ITEM_SPACE_FOOD
    {"ドライフラワー", "しなびたキノコ", "うちゅうしょく"}, // ITEM_DRIED_BOUQUET + ITEM_DRIED_SHROOM -> ITEM_SPACE_FOOD
    {"ドライフラワー", "じわじわキノコ", "うちゅうしょく"}, // ITEM_DRIED_BOUQUET + ITEM_SLOW_SHROOM -> ITEM_SPACE_FOOD
    {"ドライフラワー", "ホットドッグ", "うちゅうしょく"}, // ITEM_DRIED_BOUQUET + ITEM_HOT_DOG -> ITEM_SPACE_FOOD
    {"ふしぎなタマゴ", "ファイアフラワー", "バクハツタマゴ"}, // ITEM_MYSTIC_EGG + ITEM_FIRE_FLOWER -> ITEM_EGG_BOMB
    {"ふしぎなタマゴ", "キノコ", "キセツのオムレツ"}, // ITEM_MYSTIC_EGG + ITEM_MUSHROOM -> ITEM_OMELETTE_MEAL
    {"ふしぎなタマゴ", "スーパーキノコ", "キセツのオムレツ"}, // ITEM_MYSTIC_EGG + ITEM_SUPER_SHROOM -> ITEM_OMELETTE_MEAL
    {"ふしぎなタマゴ", "ウルトラキノコ", "キセツのオムレツ"}, // ITEM_MYSTIC_EGG + ITEM_ULTRA_SHROOM -> ITEM_OMELETTE_MEAL
    {"ふしぎなタマゴ", "きんきゅうキノコ", "キセツのオムレツ"}, // ITEM_MYSTIC_EGG + ITEM_LIFE_SHROOM -> ITEM_OMELETTE_MEAL
    {"ふしぎなタマゴ", "ドライフラワー", "うちゅうしょく"}, // ITEM_MYSTIC_EGG + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"おうごんはっぱ", "こおりのいぶき", "ゆきうさぎ"}, // ITEM_GOLDEN_LEAF + ITEM_ICE_STORM -> ITEM_SNOW_BUNNY
#if VERSION >= VERSION_US
    {"おうごんはっぱ", "キノコ", "キノコいため"}, // ITEM_GOLDEN_LEAF + ITEM_MUSHROOM -> ITEM_SHROOM_FRY
    {"おうごんはっぱ", "スーパーキノコ", "キノコホイルやき"}, // ITEM_GOLDEN_LEAF + ITEM_SUPER_SHROOM -> ITEM_SHROOM_ROAST
    {"おうごんはっぱ", "ウルトラキノコ", "キノコステーキ"}, // ITEM_GOLDEN_LEAF + ITEM_ULTRA_SHROOM -> ITEM_SHROOM_STEAK
    {"おうごんはっぱ", "きんきゅうキノコ", "キノコステーキ"}, // ITEM_GOLDEN_LEAF + ITEM_LIFE_SHROOM -> ITEM_SHROOM_STEAK
#else
    {"おうごんはっぱ", "キノコ", "きんきゅうキノコ"}, // ITEM_GOLDEN_LEAF + ITEM_MUSHROOM -> ITEM_LIFE_SHROOM
    {"おうごんはっぱ", "スーパーキノコ", "きんきゅうキノコ"}, // ITEM_GOLDEN_LEAF + ITEM_SUPER_SHROOM -> ITEM_LIFE_SHROOM
    {"おうごんはっぱ", "ウルトラキノコ", "きんきゅうキノコ"}, // ITEM_GOLDEN_LEAF + ITEM_ULTRA_SHROOM -> ITEM_LIFE_SHROOM
    {"おうごんはっぱ", "きんきゅうキノコ", "きんきゅうキノコ"}, // ITEM_GOLDEN_LEAF + ITEM_LIFE_SHROOM -> ITEM_LIFE_SHROOM
#endif
    {"おうごんはっぱ", "じわじわキノコ", "こうちゃキノコ"}, // ITEM_GOLDEN_LEAF + ITEM_SLOW_SHROOM -> ITEM_SHROOM_BROTH
    {"おうごんはっぱ", "ドライフラワー", "うちゅうしょく"}, // ITEM_GOLDEN_LEAF + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"トロピコマンゴー", "びりびりキノコ", "ナンシーディナー"}, // ITEM_KEEL_MANGO + ITEM_VOLT_SHROOM -> ITEM_ZESS_DINNER
#if VERSION >= VERSION_US
    {"トロピコマンゴー", "キノコ", "ナンシーディナー"}, // ITEM_KEEL_MANGO + ITEM_MUSHROOM -> ITEM_ZESS_DINNER
#endif
    {"トロピコマンゴー", "スーパーキノコ", "ナンシーディナー"}, // ITEM_KEEL_MANGO + ITEM_SUPER_SHROOM -> ITEM_ZESS_DINNER
    {"トロピコマンゴー", "ウルトラキノコ", "ナンシーディナー"}, // ITEM_KEEL_MANGO + ITEM_ULTRA_SHROOM -> ITEM_ZESS_DINNER
    {"トロピコマンゴー", "ハニーシロップ", "フルーツパフェ"}, // ITEM_KEEL_MANGO + ITEM_HONEY_SYRUP -> ITEM_FRUIT_PARFAIT
    {"トロピコマンゴー", "メイプルシロップ", "フルーツパフェ"}, // ITEM_KEEL_MANGO + ITEM_MAPLE_SYRUP -> ITEM_FRUIT_PARFAIT
    {"トロピコマンゴー", "ローヤルゼリー", "フルーツパフェ"}, // ITEM_KEEL_MANGO + ITEM_JAMMIN_JELLY -> ITEM_FRUIT_PARFAIT
    {"トロピコマンゴー", "じわじわシロップ", "フルーツパフェ"}, // ITEM_KEEL_MANGO + ITEM_GRADUAL_SYRUP -> ITEM_FRUIT_PARFAIT
    {"トロピコマンゴー", "ドライフラワー", "うちゅうしょく"}, // ITEM_KEEL_MANGO + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"生パスタ", "びりびりキノコ", "ナンシーディナー"}, // ITEM_FRESH_PASTA + ITEM_VOLT_SHROOM -> ITEM_ZESS_DINNER
#if VERSION >= VERSION_US
    {"生パスタ", "キノコ", "ナンシースペシャル"}, // ITEM_FRESH_PASTA + ITEM_MUSHROOM -> ITEM_ZESS_SPECIAL
    {"生パスタ", "スーパーキノコ", "ナンシースペシャル"}, // ITEM_FRESH_PASTA + ITEM_SUPER_SHROOM -> ITEM_ZESS_SPECIAL
    {"生パスタ", "ウルトラキノコ", "ナンシーデラックス"}, // ITEM_FRESH_PASTA + ITEM_ULTRA_SHROOM -> ITEM_ZESS_DELUXE
    {"生パスタ", "きんきゅうキノコ", "ナンシースペシャル"}, // ITEM_FRESH_PASTA + ITEM_LIFE_SHROOM -> ITEM_ZESS_SPECIAL
    {"生パスタ", "しなびたキノコ", "ナンシースペシャル"}, // ITEM_FRESH_PASTA + ITEM_DRIED_SHROOM -> ITEM_ZESS_SPECIAL
#else
    {"生パスタ", "キノコ", "ナンシーディナー"}, // ITEM_FRESH_PASTA + ITEM_MUSHROOM -> ITEM_ZESS_DINNER
    {"生パスタ", "スーパーキノコ", "ナンシーディナー"}, // ITEM_FRESH_PASTA + ITEM_SUPER_SHROOM -> ITEM_ZESS_DINNER
    {"生パスタ", "ウルトラキノコ", "ナンシースペシャル"}, // ITEM_FRESH_PASTA + ITEM_ULTRA_SHROOM -> ITEM_ZESS_SPECIAL
    {"生パスタ", "きんきゅうキノコ", "ナンシーディナー"}, // ITEM_FRESH_PASTA + ITEM_LIFE_SHROOM -> ITEM_ZESS_DINNER
    {"生パスタ", "しなびたキノコ", "ナンシーディナー"}, // ITEM_FRESH_PASTA + ITEM_DRIED_SHROOM -> ITEM_ZESS_DINNER
#endif
    {"生パスタ", "ドライフラワー", "うちゅうしょく"}, // ITEM_FRESH_PASTA + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"生パスタ", "ふしぎなタマゴ", "ナンシーディナー"}, // ITEM_FRESH_PASTA + ITEM_MYSTIC_EGG -> ITEM_ZESS_DINNER
    {"おかしのもと", "ファイアフラワー", "ファイアキャンディ"}, // ITEM_CAKE_MIX + ITEM_FIRE_FLOWER -> ITEM_FIRE_POP
    {"おかしのもと", "びりびりキノコ", "びりびりキャンディ"}, // ITEM_CAKE_MIX + ITEM_VOLT_SHROOM -> ITEM_ELECTRO_POP
    {"おかしのもと", "まどわせのこな", "メロメロケーキ"}, // ITEM_CAKE_MIX + ITEM_RUIN_POWDER -> ITEM_HEARTFUL_CAKE
    {"おかしのもと", "キノコ", "キノコケーキ"}, // ITEM_CAKE_MIX + ITEM_MUSHROOM -> ITEM_SHROOM_CAKE
    {"おかしのもと", "スーパーキノコ", "キノコケーキ"}, // ITEM_CAKE_MIX + ITEM_SUPER_SHROOM -> ITEM_SHROOM_CAKE
    {"おかしのもと", "ウルトラキノコ", "キノコクレープ"}, // ITEM_CAKE_MIX + ITEM_ULTRA_SHROOM -> ITEM_SHROOM_CREPE
    {"おかしのもと", "きんきゅうキノコ", "キノコケーキ"}, // ITEM_CAKE_MIX + ITEM_LIFE_SHROOM -> ITEM_SHROOM_CAKE
    {"おかしのもと", "ハニーシロップ", "ハニーあめ"}, // ITEM_CAKE_MIX + ITEM_HONEY_SYRUP -> ITEM_HONEY_CANDY
    {"おかしのもと", "メイプルシロップ", "ナンシークッキー"}, // ITEM_CAKE_MIX + ITEM_MAPLE_SYRUP -> ITEM_ZESS_COOKIE
    {"おかしのもと", "ローヤルゼリー", "ローヤルキャンディ"}, // ITEM_CAKE_MIX + ITEM_JAMMIN_JELLY -> ITEM_JELLY_CANDY
    {"おかしのもと", "じわじわキノコ", "キノコケーキ"}, // ITEM_CAKE_MIX + ITEM_SLOW_SHROOM -> ITEM_SHROOM_CAKE
    {"おかしのもと", "じわじわシロップ", "ナンシークッキー"}, // ITEM_CAKE_MIX + ITEM_GRADUAL_SYRUP -> ITEM_ZESS_COOKIE
    {"おかしのもと", "ドライフラワー", "うちゅうしょく"}, // ITEM_CAKE_MIX + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"おかしのもと", "ふしぎなタマゴ", "ナンシークッキー"}, // ITEM_CAKE_MIX + ITEM_MYSTIC_EGG -> ITEM_ZESS_COOKIE
    {"おかしのもと", "トロピコマンゴー", "マンゴープディング"}, // ITEM_CAKE_MIX + ITEM_KEEL_MANGO -> ITEM_MANGO_DELIGHT
    {"レッド・カラリン", "ファイアフラワー", "あつあつスープ"}, // ITEM_HOT_SAUCE + ITEM_FIRE_FLOWER -> ITEM_SPICY_SOUP
    {"レッド・カラリン", "しなびたキノコ", "キノコ"}, // ITEM_HOT_SAUCE + ITEM_DRIED_SHROOM -> ITEM_MUSHROOM
    {"レッド・カラリン", "スッキリドリンク", "すみぢる"}, // ITEM_HOT_SAUCE + ITEM_TASTY_TONIC -> ITEM_INKY_SAUCE
    {"レッド・カラリン", "ハニーシロップ", "スッキリドリンク"}, // ITEM_HOT_SAUCE + ITEM_HONEY_SYRUP -> ITEM_TASTY_TONIC
    {"レッド・カラリン", "メイプルシロップ", "スッキリドリンク"}, // ITEM_HOT_SAUCE + ITEM_MAPLE_SYRUP -> ITEM_TASTY_TONIC
    {"レッド・カラリン", "ローヤルゼリー", "スッキリドリンク"}, // ITEM_HOT_SAUCE + ITEM_JAMMIN_JELLY -> ITEM_TASTY_TONIC
    {"レッド・カラリン", "生パスタ", "カラリーナパスタ"}, // ITEM_HOT_SAUCE + ITEM_FRESH_PASTA -> ITEM_SPICY_PASTA
    {"レッド・カラリン", "おかしのもと", "ファイアキャンディ"}, // ITEM_HOT_SAUCE + ITEM_CAKE_MIX -> ITEM_FIRE_POP
#if VERSION >= VERSION_US
    {"カメカメはっぱ", "キノコ", "キノコいため"}, // ITEM_TURTLEY_LEAF + ITEM_MUSHROOM -> ITEM_SHROOM_FRY
    {"カメカメはっぱ", "スーパーキノコ", "キノコホイルやき"}, // ITEM_TURTLEY_LEAF + ITEM_SUPER_SHROOM -> ITEM_SHROOM_ROAST
    {"カメカメはっぱ", "ウルトラキノコ", "キノコステーキ"}, // ITEM_TURTLEY_LEAF + ITEM_ULTRA_SHROOM -> ITEM_SHROOM_STEAK
    {"カメカメはっぱ", "きんきゅうキノコ", "キノコステーキ"}, // ITEM_TURTLEY_LEAF + ITEM_LIFE_SHROOM -> ITEM_SHROOM_STEAK
#else
    {"カメカメはっぱ", "キノコ", "きんきゅうキノコ"}, // ITEM_TURTLEY_LEAF + ITEM_MUSHROOM -> ITEM_LIFE_SHROOM
    {"カメカメはっぱ", "スーパーキノコ", "きんきゅうキノコ"}, // ITEM_TURTLEY_LEAF + ITEM_SUPER_SHROOM -> ITEM_LIFE_SHROOM
    {"カメカメはっぱ", "ウルトラキノコ", "きんきゅうキノコ"}, // ITEM_TURTLEY_LEAF + ITEM_ULTRA_SHROOM -> ITEM_LIFE_SHROOM
    {"カメカメはっぱ", "きんきゅうキノコ", "きんきゅうキノコ"}, // ITEM_TURTLEY_LEAF + ITEM_LIFE_SHROOM -> ITEM_LIFE_SHROOM
#endif
    {"カメカメはっぱ", "ハニーシロップ", "フレッシュジュース"}, // ITEM_TURTLEY_LEAF + ITEM_HONEY_SYRUP -> ITEM_FRESH_JUICE
    {"カメカメはっぱ", "メイプルシロップ", "フレッシュジュース"}, // ITEM_TURTLEY_LEAF + ITEM_MAPLE_SYRUP -> ITEM_FRESH_JUICE
    {"カメカメはっぱ", "ローヤルゼリー", "フレッシュジュース"}, // ITEM_TURTLEY_LEAF + ITEM_JAMMIN_JELLY -> ITEM_FRESH_JUICE
    {"カメカメはっぱ", "じわじわキノコ", "こうちゃキノコ"}, // ITEM_TURTLEY_LEAF + ITEM_SLOW_SHROOM -> ITEM_SHROOM_BROTH
    {"カメカメはっぱ", "じわじわシロップ", "フレッシュジュース"}, // ITEM_TURTLEY_LEAF + ITEM_GRADUAL_SYRUP -> ITEM_FRESH_JUICE
    {"カメカメはっぱ", "ドライフラワー", "うちゅうしょく"}, // ITEM_TURTLEY_LEAF + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"カメカメはっぱ", "おうごんはっぱ", "ヘルスィーサラダ"}, // ITEM_TURTLEY_LEAF + ITEM_GOLDEN_LEAF -> ITEM_HEALTHY_SALAD
    {"カメカメはっぱ", "トロピコマンゴー", "カメまん"}, // ITEM_TURTLEY_LEAF + ITEM_KEEL_MANGO -> ITEM_KOOPA_BUN
    {"カメカメはっぱ", "生パスタ", "カメスパ"}, // ITEM_TURTLEY_LEAF + ITEM_FRESH_PASTA -> ITEM_KOOPASTA
    {"カメカメはっぱ", "レッド・カラリン", "すみぢる"}, // ITEM_TURTLEY_LEAF + ITEM_HOT_SAUCE -> ITEM_INKY_SAUCE
    {"つくしんぼ", "キノコ", "ナンシーディナー"}, // ITEM_HORSETAIL + ITEM_MUSHROOM -> ITEM_ZESS_DINNER
    {"つくしんぼ", "スーパーキノコ", "ナンシーディナー"}, // ITEM_HORSETAIL + ITEM_SUPER_SHROOM -> ITEM_ZESS_DINNER
    {"つくしんぼ", "ウルトラキノコ", "ナンシースペシャル"}, // ITEM_HORSETAIL + ITEM_ULTRA_SHROOM -> ITEM_ZESS_SPECIAL
    {"つくしんぼ", "きんきゅうキノコ", "ナンシーディナー"}, // ITEM_HORSETAIL + ITEM_LIFE_SHROOM -> ITEM_ZESS_DINNER
    {"つくしんぼ", "ドライフラワー", "うちゅうしょく"}, // ITEM_HORSETAIL + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"つくしんぼ", "ふしぎなタマゴ", "キセツのオムレツ"}, // ITEM_HORSETAIL + ITEM_MYSTIC_EGG -> ITEM_OMELETTE_MEAL
    {"つくしんぼ", "カメカメはっぱ", "ヘルスィーサラダ"}, // ITEM_HORSETAIL + ITEM_TURTLEY_LEAF -> ITEM_HEALTHY_SALAD
    {"ピチピーチ", "びりびりキノコ", "ナンシーディナー"}, // ITEM_PEACHY_PEACH + ITEM_VOLT_SHROOM -> ITEM_ZESS_DINNER
    {"ピチピーチ", "まどわせのこな", "メロメロケーキ"}, // ITEM_PEACHY_PEACH + ITEM_RUIN_POWDER -> ITEM_HEARTFUL_CAKE
#if VERSION >= VERSION_US
    {"ピチピーチ", "キノコ", "ナンシーディナー"}, // ITEM_PEACHY_PEACH + ITEM_MUSHROOM -> ITEM_ZESS_DINNER
    {"ピチピーチ", "スーパーキノコ", "ナンシーディナー"}, // ITEM_PEACHY_PEACH + ITEM_SUPER_SHROOM -> ITEM_ZESS_DINNER
    {"ピチピーチ", "ウルトラキノコ", "ナンシースペシャル"}, // ITEM_PEACHY_PEACH + ITEM_ULTRA_SHROOM -> ITEM_ZESS_SPECIAL
    {"ピチピーチ", "きんきゅうキノコ", "ナンシーディナー"}, // ITEM_PEACHY_PEACH + ITEM_LIFE_SHROOM -> ITEM_ZESS_DINNER
#else
    {"ピチピーチ", "スーパーキノコ", "ナンシーディナー"}, // ITEM_PEACHY_PEACH + ITEM_SUPER_SHROOM -> ITEM_ZESS_DINNER
    {"ピチピーチ", "ウルトラキノコ", "ナンシーディナー"}, // ITEM_PEACHY_PEACH + ITEM_ULTRA_SHROOM -> ITEM_ZESS_DINNER
#endif
    {"ピチピーチ", "しなびたキノコ", "キノコ"}, // ITEM_PEACHY_PEACH + ITEM_DRIED_SHROOM -> ITEM_MUSHROOM
    {"ピチピーチ", "ハニーシロップ", "フルーツパフェ"}, // ITEM_PEACHY_PEACH + ITEM_HONEY_SYRUP -> ITEM_FRUIT_PARFAIT
    {"ピチピーチ", "メイプルシロップ", "フルーツパフェ"}, // ITEM_PEACHY_PEACH + ITEM_MAPLE_SYRUP -> ITEM_FRUIT_PARFAIT
    {"ピチピーチ", "ローヤルゼリー", "フルーツパフェ"}, // ITEM_PEACHY_PEACH + ITEM_JAMMIN_JELLY -> ITEM_FRUIT_PARFAIT
    {"ピチピーチ", "ドライフラワー", "うちゅうしょく"}, // ITEM_PEACHY_PEACH + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ピチピーチ", "トロピコマンゴー", "フルーツパフェ"}, // ITEM_PEACHY_PEACH + ITEM_KEEL_MANGO -> ITEM_FRUIT_PARFAIT
    {"ピチピーチ", "おかしのもと", "ピーチタルト"}, // ITEM_PEACHY_PEACH + ITEM_CAKE_MIX -> ITEM_PEACH_TART
    {"ピチピーチ", "レッド・カラリン", "スッキリドリンク"}, // ITEM_PEACHY_PEACH + ITEM_HOT_SAUCE -> ITEM_TASTY_TONIC
    {"ヤシの実", "ファイアフラワー", "ココナッツボム"}, // ITEM_COCONUT + ITEM_FIRE_FLOWER -> ITEM_COCONUT_BOMB
    {"ヤシの実", "ドライフラワー", "うちゅうしょく"}, // ITEM_COCONUT + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ヤシの実", "トロピコマンゴー", "フレッシュジュース"}, // ITEM_COCONUT + ITEM_KEEL_MANGO -> ITEM_FRESH_JUICE
    {"ヤシの実", "生パスタ", "ナンシーディナー"}, // ITEM_COCONUT + ITEM_FRESH_PASTA -> ITEM_ZESS_DINNER
    {"ヤシの実", "おかしのもと", "ココナツキャンディ"}, // ITEM_COCONUT + ITEM_CAKE_MIX -> ITEM_COCO_CANDY
    {"ヤシの実", "カメカメはっぱ", "フレッシュジュース"}, // ITEM_COCONUT + ITEM_TURTLEY_LEAF -> ITEM_FRESH_JUICE
    {"ヤシの実", "ピチピーチ", "フレッシュジュース"}, // ITEM_COCONUT + ITEM_PEACHY_PEACH -> ITEM_FRESH_JUICE
    {"キノコいため", "キラキラおとし", "ほしふるディナー"}, // ITEM_SHROOM_FRY + ITEM_SHOOTING_STAR -> ITEM_METEOR_MEAL
    {"キノコいため", "ドライフラワー", "うちゅうしょく"}, // ITEM_SHROOM_FRY + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"キノコホイルやき", "キラキラおとし", "ほしふるディナー"}, // ITEM_SHROOM_ROAST + ITEM_SHOOTING_STAR -> ITEM_METEOR_MEAL
    {"キノコホイルやき", "ドライフラワー", "うちゅうしょく"}, // ITEM_SHROOM_ROAST + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"キノコステーキ", "キラキラおとし", "ほしふるディナー"}, // ITEM_SHROOM_STEAK + ITEM_SHOOTING_STAR -> ITEM_METEOR_MEAL
    {"キノコステーキ", "ドライフラワー", "うちゅうしょく"}, // ITEM_SHROOM_STEAK + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"しっぱいりょうり", "ドライフラワー", "うちゅうしょく"}, // ITEM_MISTAKE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ハニーキノコ", "ドライフラワー", "うちゅうしょく"}, // ITEM_HONEY_SHROOM + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"メイプルキノコ", "ドライフラワー", "うちゅうしょく"}, // ITEM_MAPLE_SHROOM + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ローヤルキノコ", "ドライフラワー", "うちゅうしょく"}, // ITEM_JELLY_SHROOM + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ハニーキノコＳ", "ドライフラワー", "うちゅうしょく"}, // ITEM_HONEY_SUPER + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"メイプルキノコＳ", "ドライフラワー", "うちゅうしょく"}, // ITEM_MAPLE_SUPER + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ローヤルキノコＳ", "ドライフラワー", "うちゅうしょく"}, // ITEM_JELLY_SUPER + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ハニーキノコＺ", "ドライフラワー", "うちゅうしょく"}, // ITEM_HONEY_ULTRA + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"メイプルキノコＺ", "ドライフラワー", "うちゅうしょく"}, // ITEM_MAPLE_ULTRA + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ローヤルキノコＺ", "ドライフラワー", "うちゅうしょく"}, // ITEM_JELLY_ULTRA + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ナンシーディナー", "カチカチコウラ", "カチカチりょうり"}, // ITEM_ZESS_DINNER + ITEM_COURAGE_SHELL -> ITEM_COURAGE_MEAL
    {"ナンシーディナー", "ドライフラワー", "うちゅうしょく"}, // ITEM_ZESS_DINNER + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ナンシースペシャル", "カチカチコウラ", "カチカチりょうり"}, // ITEM_ZESS_SPECIAL + ITEM_COURAGE_SHELL -> ITEM_COURAGE_MEAL
    {"ナンシースペシャル", "ドライフラワー", "うちゅうしょく"}, // ITEM_ZESS_SPECIAL + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ナンシーデラックス", "カチカチコウラ", "カチカチりょうり"}, // ITEM_ZESS_DELUXE + ITEM_COURAGE_SHELL -> ITEM_COURAGE_MEAL
    {"ナンシーデラックス", "ドライフラワー", "うちゅうしょく"}, // ITEM_ZESS_DELUXE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ナンシーダイナマイ", "ドライフラワー", "バクハツタマゴ"}, // ITEM_ZESS_DYNAMITE + ITEM_DRIED_BOUQUET -> ITEM_EGG_BOMB
    {"ナンシーティー", "レッド・カラリン", "すみぢる"}, // ITEM_ZESS_TEA + ITEM_HOT_SAUCE -> ITEM_INKY_SAUCE
    {"ゆきうさぎ", "あつあつスープ", "いつまでもふたりで"}, // ITEM_SNOW_BUNNY + ITEM_SPICY_SOUP -> ITEM_COUPLES_CAKE
    {"キノコケーキ", "ドライフラワー", "うちゅうしょく"}, // ITEM_SHROOM_CAKE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"キノコクレープ", "ドライフラワー", "うちゅうしょく"}, // ITEM_SHROOM_CREPE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ムースケーキ", "ドライフラワー", "うちゅうしょく"}, // ITEM_MOUSSE_CAKE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"フルーツパフェ", "ドライフラワー", "うちゅうしょく"}, // ITEM_FRUIT_PARFAIT + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"バクハツタマゴ", "ドライフラワー", "うちゅうしょく"}, // ITEM_EGG_BOMB + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"イカスミパスタ", "ドライフラワー", "うちゅうしょく"}, // ITEM_INK_PASTA + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"スパゲティ", "ドライフラワー", "うちゅうしょく"}, // ITEM_SPAGHETTI + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"スパゲティ", "レッド・カラリン", "カラリーナパスタ"}, // ITEM_SPAGHETTI + ITEM_HOT_SAUCE -> ITEM_SPICY_PASTA
    {"スパゲティ", "カメカメはっぱ", "カメスパ"}, // ITEM_SPAGHETTI + ITEM_TURTLEY_LEAF -> ITEM_KOOPASTA
    {"こうちゃキノコ", "レッド・カラリン", "すみぢる"}, // ITEM_SHROOM_BROTH + ITEM_HOT_SAUCE -> ITEM_INKY_SAUCE
#if VERSION >= VERSION_US
    {"どくキノコ", "イレカエール", "キノコ"}, // ITEM_POISON_SHROOM + ITEM_POINT_SWAP -> ITEM_MUSHROOM
    {"どくキノコ", "ドライフラワー", "うちゅうしょく"}, // ITEM_POISON_SHROOM + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"どくキノコ", "おうごんはっぱ", "こうちゃキノコ"}, // ITEM_POISON_SHROOM + ITEM_GOLDEN_LEAF -> ITEM_SHROOM_BROTH
    {"どくキノコ", "カメカメはっぱ", "こうちゃキノコ"}, // ITEM_POISON_SHROOM + ITEM_TURTLEY_LEAF -> ITEM_SHROOM_BROTH
#else
    {"どくキノコ", "イレカエール", "ウルトラキノコ"}, // ITEM_POISON_SHROOM + ITEM_POINT_SWAP -> ITEM_ULTRA_SHROOM
    {"どくキノコ", "ドライフラワー", "うちゅうしょく"}, // ITEM_POISON_SHROOM + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"どくキノコ", "おうごんはっぱ", "こうちゃキノコ"}, // ITEM_POISON_SHROOM + ITEM_GOLDEN_LEAF -> ITEM_SHROOM_BROTH
#endif
    {"ショコラケーキ", "ドライフラワー", "うちゅうしょく"}, // ITEM_CHOCO_CAKE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"マンゴープディング", "ドライフラワー", "うちゅうしょく"}, // ITEM_MANGO_DELIGHT + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"マンゴープディング", "ふしぎなタマゴ", "はつこいプディング"}, // ITEM_MANGO_DELIGHT + ITEM_MYSTIC_EGG -> ITEM_LOVE_PUDDING
    {"はつこいプディング", "ドライフラワー", "うちゅうしょく"}, // ITEM_LOVE_PUDDING + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ほしふるディナー", "ドライフラワー", "うちゅうしょく"}, // ITEM_METEOR_MEAL + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ほしふるディナー", "フルーツパフェ", "ナンシーディナー"}, // ITEM_METEOR_MEAL + ITEM_FRUIT_PARFAIT -> ITEM_ZESS_DINNER
    {"しれんのナベ", "ドライフラワー", "どくキノコ"}, // ITEM_TRIAL_STEW + ITEM_DRIED_BOUQUET -> ITEM_POISON_SHROOM
    {"いつまでもふたりで", "ドライフラワー", "うちゅうしょく"}, // ITEM_COUPLES_CAKE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"いつまでもふたりで", "どくキノコ", "しれんのナベ"}, // ITEM_COUPLES_CAKE + ITEM_POISON_SHROOM -> ITEM_TRIAL_STEW
    {"すみぢる", "じわじわキノコ", "どくキノコ"}, // ITEM_INKY_SAUCE + ITEM_SLOW_SHROOM -> ITEM_POISON_SHROOM
    {"すみぢる", "生パスタ", "イカスミパスタ"}, // ITEM_INKY_SAUCE + ITEM_FRESH_PASTA -> ITEM_INK_PASTA
    {"すみぢる", "おかしのもと", "ショコラケーキ"}, // ITEM_INKY_SAUCE + ITEM_CAKE_MIX -> ITEM_CHOCO_CAKE
    {"すみぢる", "ムースケーキ", "ショコラケーキ"}, // ITEM_INKY_SAUCE + ITEM_MOUSSE_CAKE -> ITEM_CHOCO_CAKE
    {"すみぢる", "スパゲティ", "イカスミパスタ"}, // ITEM_INKY_SAUCE + ITEM_SPAGHETTI -> ITEM_INK_PASTA
    {"キセツのオムレツ", "ドライフラワー", "うちゅうしょく"}, // ITEM_OMELETTE_MEAL + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"カメカメティー", "レッド・カラリン", "すみぢる"}, // ITEM_KOOPA_TEA + ITEM_HOT_SAUCE -> ITEM_INKY_SAUCE
    {"カメスパ", "ドライフラワー", "うちゅうしょく"}, // ITEM_KOOPASTA + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"カメスパ", "レッド・カラリン", "カラリーナパスタ"}, // ITEM_KOOPASTA + ITEM_HOT_SAUCE -> ITEM_SPICY_PASTA
    {"カメスパ", "すみぢる", "イカスミパスタ"}, // ITEM_KOOPASTA + ITEM_INKY_SAUCE -> ITEM_INK_PASTA
    {"カラリーナパスタ", "ドライフラワー", "うちゅうしょく"}, // ITEM_SPICY_PASTA + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"カラリーナパスタ", "ヤシの実", "ナンシーディナー"}, // ITEM_SPICY_PASTA + ITEM_COCONUT -> ITEM_ZESS_DINNER
    {"カラリーナパスタ", "すみぢる", "イカスミパスタ"}, // ITEM_SPICY_PASTA + ITEM_INKY_SAUCE -> ITEM_INK_PASTA
    {"メロメロケーキ", "ドライフラワー", "うちゅうしょく"}, // ITEM_HEARTFUL_CAKE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ピーチタルト", "ドライフラワー", "うちゅうしょく"}, // ITEM_PEACH_TART + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"びりびりキャンディ", "ドライフラワー", "うちゅうしょく"}, // ITEM_ELECTRO_POP + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ファイアキャンディ", "ドライフラワー", "うちゅうしょく"}, // ITEM_FIRE_POP + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ハニーあめ", "ドライフラワー", "うちゅうしょく"}, // ITEM_HONEY_CANDY + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ココナツキャンディ", "ドライフラワー", "うちゅうしょく"}, // ITEM_COCO_CANDY + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ローヤルキャンディ", "ドライフラワー", "うちゅうしょく"}, // ITEM_JELLY_CANDY + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ナンシークッキー", "ドライフラワー", "うちゅうしょく"}, // ITEM_ZESS_COOKIE + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ヘルスィーサラダ", "ドライフラワー", "うちゅうしょく"}, // ITEM_HEALTHY_SALAD + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"ヘルスィーサラダ", "生パスタ", "ナンシーディナー"}, // ITEM_HEALTHY_SALAD + ITEM_FRESH_PASTA -> ITEM_ZESS_DINNER
    {"ヘルスィーサラダ", "キノコいため", "ナンシーディナー"}, // ITEM_HEALTHY_SALAD + ITEM_SHROOM_FRY -> ITEM_ZESS_DINNER
    {"ヘルスィーサラダ", "キノコホイルやき", "ナンシースペシャル"}, // ITEM_HEALTHY_SALAD + ITEM_SHROOM_ROAST -> ITEM_ZESS_SPECIAL
    {"ヘルスィーサラダ", "キノコステーキ", "ナンシーデラックス"}, // ITEM_HEALTHY_SALAD + ITEM_SHROOM_STEAK -> ITEM_ZESS_DELUXE
    {"ヘルスィーサラダ", "イカスミパスタ", "ナンシースペシャル"}, // ITEM_HEALTHY_SALAD + ITEM_INK_PASTA -> ITEM_ZESS_SPECIAL
    {"ヘルスィーサラダ", "スパゲティ", "ナンシーディナー"}, // ITEM_HEALTHY_SALAD + ITEM_SPAGHETTI -> ITEM_ZESS_DINNER
    {"ヘルスィーサラダ", "カメスパ", "ナンシーディナー"}, // ITEM_HEALTHY_SALAD + ITEM_KOOPASTA -> ITEM_ZESS_DINNER
    {"ヘルスィーサラダ", "カラリーナパスタ", "ナンシースペシャル"}, // ITEM_HEALTHY_SALAD + ITEM_SPICY_PASTA -> ITEM_ZESS_SPECIAL
    {"カメまん", "ドライフラワー", "うちゅうしょく"}, // ITEM_KOOPA_BUN + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"フレッシュジュース", "レッド・カラリン", "すみぢる"}, // ITEM_FRESH_JUICE + ITEM_HOT_SAUCE -> ITEM_INKY_SAUCE
    {"たんこぶ", "ドライフラワー", "うちゅうしょく"}, // ITEM_WHACKA_BUMP + ITEM_DRIED_BOUQUET -> ITEM_SPACE_FOOD
    {"たんこぶ", "おうごんはっぱ", "ナンシーデラックス"}, // ITEM_WHACKA_BUMP + ITEM_GOLDEN_LEAF -> ITEM_ZESS_DELUXE
    {"きんかい", "イレカエール", "キノコ"}, // ITEM_GOLD_BAR + ITEM_POINT_SWAP -> ITEM_MUSHROOM
    {"きんかい×３", "イレカエール", "きんかい"}, // ITEM_GOLD_BAR_X3 + ITEM_POINT_SWAP -> ITEM_GOLD_BAR
    {"ココナッツボム", "ドライフラワー", "ヤシの実"}, // ITEM_COCONUT_BOMB + ITEM_DRIED_BOUQUET -> ITEM_COCONUT
    {"ココナッツボム", "バクハツタマゴ", "ナンシーダイナマイ"}, // ITEM_COCONUT_BOMB + ITEM_EGG_BOMB -> ITEM_ZESS_DYNAMITE
    {"カチカチりょうり", "ドライフラワー", "カチカチコウラ"}, // ITEM_COURAGE_MEAL + ITEM_DRIED_BOUQUET -> ITEM_COURAGE_SHELL
};
#endif

s32 R_mystery_cook_list[] = {
    ITEM_FIRE_FLOWER,
    ITEM_TASTY_TONIC,
    ITEM_FRESH_JUICE,
    ITEM_SLEEPY_SHEEP,
    ITEM_LIFE_SHROOM,
    ITEM_DIZZY_DIAL,
    ITEM_VOLT_SHROOM,
    ITEM_SHROOM_FRY,
    ITEM_KOOPASTA,
    ITEM_SPICY_SOUP,
    ITEM_KOOPA_TEA,
    ITEM_SPAGHETTI,
    ITEM_FRIED_EGG,
    ITEM_HONEY_SHROOM,
    ITEM_ZESS_DINNER,
    ITEM_THUNDER_RAGE,
    ITEM_EGG_BOMB,
    ITEM_PEACHY_PEACH,
    ITEM_ZESS_COOKIE,
    ITEM_ZESS_TEA,
};

/** Dishes that get the special radiating light when Zess T. cooks them, ending with -1. */
s32 special_cook_list[] = {
    ITEM_ZESS_SPECIAL,
    ITEM_ZESS_DELUXE,
    ITEM_ZESS_DYNAMITE,
    ITEM_COURAGE_MEAL,
    ITEM_INK_PASTA,
    ITEM_POISON_SHROOM,
    ITEM_CHOCO_CAKE,
    ITEM_MANGO_DELIGHT,
    ITEM_LOVE_PUDDING,
    ITEM_METEOR_MEAL,
    ITEM_TRIAL_STEW,
    ITEM_COUPLES_CAKE,
    ITEM_INKY_SAUCE,
    ITEM_OMELETTE_MEAL,
    ITEM_SPICY_PASTA,
    -1, // end of list
};

/**
 * Creates the seven Star Stone effects of the Chapter 8 "everyone's voices" scene in a ring of radius 140 around
 * (args[0], args[1], args[2]), named "starstone0" to "starstone6", without the shine sound and drawn by
 * their owner (STAR_STONE_FLAG_EXTERNAL_DRAW), and hooks each one into the "mahojin" magic circle effect's draw list.
 */
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
        ((EffMahojinWork*)effNameToPtr("mahojin")->userdata)[i + 2].func = (DispCallback)effStarStoneDraw;
    }
    return EVT_RETURN_DONE;
}

/** Moves the ring of seven Star Stone effects so that its center is at (args[0], args[1], args[2]). */
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

/** Sets the Y rotation (args[0], degrees) of all seven Star Stone effects. */
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

/** Blocks until the "mahojin" magic circle effect's animation has played through once. */
USER_FUNC(wait_mahojin) {
    s32 anim = ((s32*)effNameToPtr("mahojin")->userdata)[6];

    if (anim != -1 && 0.0f != animPoseGetLoopTimes(anim)) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Allocates the item list used by Trouble Center job 9; args[0] is the number of Courage Shells Mario carries. */
USER_FUNC(iri_09_item_tbl_alloc) {
#if VERSION >= VERSION_US
    s32 n = evtGetValue(event, event->args[0]);
    iri_09_item_tbl = _mapAlloc((n + 1) * sizeof(s32));
#else
    iri_09_item_tbl = _mapAlloc((evtGetValue(event, event->args[0]) + 1) * sizeof(s32));
#endif
    return EVT_RETURN_DONE;
}

/**
 * Fills the job 9 item list with args[0] copies of item 143 (Courage Shell) ended by -1
 * and returns its pointer in args[1], so the selection window offers that many shells.
 */
USER_FUNC(iri_09_item_tbl_make) {
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);
    s32 i;

    for (i = 0; i < n; i++) {
        iri_09_item_tbl[i] = ITEM_COURAGE_SHELL;
    }
    iri_09_item_tbl[i] = -1;
    evtSetValue(event, args[1], (EvtWord)iri_09_item_tbl);
    return EVT_RETURN_DONE;
}

/** Frees the item list allocated by iri_09_item_tbl_alloc. */
USER_FUNC(iri_09_item_tbl_free) {
    _mapFree(iri_09_item_tbl);
    iri_09_item_tbl = NULL;
    return EVT_RETURN_DONE;
}

/**
 * Stores the event pointer args[1] as the shop manager's talk hook (second word) in the shopper data at args[0];
 * Trouble Center job 9 uses it to take over his conversation.
 */
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
    return ITEM_MISTAKE;
}

/**
 * Looks up an item by its internal id string in itemDataTable and returns its index, or 0 if none matches.
 * Used to turn the recipe table's names into items.
 */
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

#if VERSION >= VERSION_JP
/**
 * Looks up one ingredient in R_simple_recipe_table and returns its before- and after-Cookbook results
 * in args[1] and args[2], or 0 when no recipe matches. Mystery gives the same random dish in both outputs.
 */
USER_FUNC(cook_chk) {
    // FAKE: `(volatile u32)args[n]` index casts on the evtSetValue calls below (plain args[n] changes
    // the object in both versions)
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);
    ItemData* data = &itemDataTable[item];
    u32 i;

    if (item == ITEM_MYSTERY) {
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
#endif

/**
 * Zess T.'s two-ingredient recipe lookup: given items args[0] and args[1] (in either order),
 * returns the dish in args[2] or 0 if the pair has no recipe.
 * Anything with the Mystery item gives a random result.
 * The early demos read cooking.xml from the disc, later releases search R_cookbook_recipe_table.
 */
#if VERSION >= VERSION_JP
USER_FUNC(cook_chk2) {
    // FAKE: `(volatile u32)args[n]` index casts on the evtSetValue calls below (plain args[n] changes
    // the object in both versions)
    EvtWord* args = event->args;
    s32 item1 = evtGetValue(event, args[0]);
    s32 item2 = evtGetValue(event, args[1]);
    ItemData* data1 = &itemDataTable[item1];
    ItemData* data2 = &itemDataTable[item2];
    u32 i;

    if (item1 == ITEM_MYSTERY || item2 == ITEM_MYSTERY) {
        item2 = naniga_okorukana_cook();
        evtSetValue(event, (volatile u32)args[2], item2);
        return EVT_RETURN_DONE;
    }
#if VERSION >= VERSION_US
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
#endif

static inline BOOL fileAsyncMissing(FileEntry* file) {
    return (s32)file == -1;
}

#if VERSION <= VERSION_DEMO_JP
/**
 * The demo reads the recipes from cooking.xml on the disc:
 * <cooking_x1> blocks for one ingredient, <cooking_x2> blocks for two.
 * Each call loads the file (blocking until it is in), scans for the matching block and frees it.
 */
static USER_FUNC(cook_chk) {
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);
    ItemData* data = &itemDataTable[item];
    FileEntry* file;
    char ingredient[128];
    char result[128];
    char result2[128];

    if (item == 163) {
        item = naniga_okorukana_cook();
        evtSetValue(event, args[1], item);
        evtSetValue(event, args[2], item);
        return EVT_RETURN_DONE;
    }
    file = (FileEntry*)fileAsyncf(0, NULL, "%s/cooking.xml", getMarioStDvdRoot());
    if (file == NULL) {
        return EVT_RETURN_BLOCK;
    }
    if (fileAsyncMissing(file)) {
        evtSetValue(event, args[1], 0);
        evtSetValue(event, args[2], 0);
        return EVT_RETURN_DONE;
    }
    strcpy(ingredient, "");
    strcpy(result, "");
    strcpy(result2, "");
    file = fileAllocf(0, "%s/cooking.xml", getMarioStDvdRoot());
    parseInit(file->data[0], (u32)file->data[1]);
    while (parsePush("<cooking_x1>")) {
        parseTagGet1("<item>", 0, ingredient);
        parseTagGet1("<cook_lv1>", 0, result);
        parseTagGet1("<cook_lv2>", 0, result2);
        parsePopNext();
        if (strcmp(ingredient, data->id) == 0) {
            break;
        }
        strcpy(ingredient, "");
        strcpy(result, "");
        strcpy(result2, "");
    }
    parsePop();
    fileFree(file);
    // FAKE: volatile casts, see the retail cook_chk.
    evtSetValue(event, (volatile u32)args[1], itemNameToID(result));
    evtSetValue(event, (volatile u32)args[2], itemNameToID(result2));
    return EVT_RETURN_DONE;
}
#endif

/**
 * Zess T.'s two-ingredient recipe lookup: given items args[0] and args[1] (in either order),
 * returns the dish in args[2] or 0 if the pair has no recipe.
 * Anything with the Mystery item gives a random result.
 * The early demos read cooking.xml from the disc, later releases search R_cookbook_recipe_table.
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(cook_chk2) {
    EvtWord* args = event->args;
    s32 item1 = evtGetValue(event, args[0]);
    s32 item2 = evtGetValue(event, args[1]);
    ItemData* data1 = &itemDataTable[item1];
    ItemData* data2 = &itemDataTable[item2];
    FileEntry* file;
    char a[128];
    char b[128];
    char out[128];

    if (item1 == 163 || item2 == 163) {
        item2 = naniga_okorukana_cook();
        // FAKE: volatile cast, see the retail cook_chk.
        evtSetValue(event, (volatile u32)args[2], item2);
        return EVT_RETURN_DONE;
    }
    file = (FileEntry*)fileAsyncf(0, NULL, "%s/cooking.xml", getMarioStDvdRoot());
    if (file == NULL) {
        return EVT_RETURN_BLOCK;
    }
    if (fileAsyncMissing(file)) {
        evtSetValue(event, args[2], 0);
        return EVT_RETURN_DONE;
    }
    strcpy(a, "");
    strcpy(b, "");
    strcpy(out, "");
    file = fileAllocf(0, "%s/cooking.xml", getMarioStDvdRoot());
    parseInit(file->data[0], (u32)file->data[1]);
    while (parsePush("<cooking_x2>")) {
        parseTagGet1("<item1>", 0, a);
        parseTagGet1("<item2>", 0, b);
        parseTagGet1("<cook>", 0, out);
        parsePopNext();
        if ((strcmp(a, data1->id) == 0 && strcmp(b, data2->id) == 0) ||
            (strcmp(a, data2->id) == 0 && strcmp(b, data1->id) == 0)) {
            break;
        }
        strcpy(a, "");
        strcpy(b, "");
        strcpy(out, "");
    }
    parsePop();
    fileFree(file);
    // FAKE: volatile cast, see the retail cook_chk.
    evtSetValue(event, (volatile u32)args[2], itemNameToID(out));
    return EVT_RETURN_DONE;
}
#endif

/** Returns TRUE when item is not 0, i.e. when there is a dish to cook. */
BOOL chkCookItem(s32 item) {
    return item != 0;
}

/**
 * Builds item_tbl, the list of items Mario carries for the cooking selection (without the item he already
 * picked), ends it with -1 and returns the count in args[1].
 * args[0] is the pouch slot to leave out (the item id in the early demos).
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(make_item_tbl) {
#else
USER_FUNC(make_item_tbl) {
#endif
    EvtWord* args = event->args;
    s32 skip = evtGetValue(event, args[0]);
    s32 cnt = pouchGetHaveItemCnt();
    s32 n = 0;
    s32 i;
    s32 item;

    for (i = 0; i < cnt; i++) {
#if VERSION >= VERSION_JP
        if (skip != i) {
            item = pouchHaveItem(i);
            if (item != 0) {
#else
        item = pouchHaveItem(i);
        if (item != 0) {
            if (skip == item) {
                skip = 0;
            } else {
#endif
                item_tbl[n] = item;
                n++;
            }
        }
    }
    item_tbl[n] = -1;
    evtSetValue(event, args[1], n);
    return EVT_RETURN_DONE;
}

/**
 * Returns in args[1] the display name of item args[0] (a message string in the US and later,
 * the id string in earlier releases).
 */
USER_FUNC(get_item_name) {
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);

#if VERSION >= VERSION_US
    ItemData* data = &itemDataTable[item];
    evtSetValue(event, args[1], (s32)msgSearch(data->name));
#else
    evtSetValue(event, args[1], (EvtWord)itemDataTable[item].id);
#endif
    return EVT_RETURN_DONE;
}

/**
 * Effect type of an item, used by cook_effect (for each ingredient) and chk_cook_msg (for the dish):
 * 1 for items below the Mushroom's id, a few non-food items (Point Swap, Fright Mask, Mystery, Inn Coupon,
 * Spite Pouch, Koopa Curse) and explosive dishes (smoke and explosion), 0 for the rest, the Mistake included (fire).
 */

// The cooking animation for an item: 0 = flames, 1 = explosion puff (cook_effect).
s32 chk_cook_eff_type(s32 item) {
    if (item < ITEM_MUSHROOM) {
        return 1;
    }
    if (item < ITEM_POINT_SWAP) {
        return 0;
    }
    if (item < ITEM_SHROOM_FRY) {
        switch (item) {
            case ITEM_POINT_SWAP:
            case ITEM_FRIGHT_MASK:
            case ITEM_MYSTERY:
            case ITEM_INN_COUPON:
            case ITEM_SPITE_POUCH:
            case ITEM_KOOPA_CURSE:
                return 1;
            default:
                return 0;
        }
    }
    if (item < ITEM_AUDIENCE_CAN) {
        switch (item) {
            case ITEM_ZESS_DYNAMITE:
            case ITEM_COCONUT_BOMB:
            case ITEM_COURAGE_MEAL:
            case ITEM_EGG_BOMB:
                return 1;
            default:
                return 0;
        }
    }
    return 0;
}

/**
 * Plays the cooking effect at Zess T.'s stove (-380, 25..60, -170) for item args[0]:
 * fire and a fire sound for type 0 items, an explosion and smoke for type 1 items.
 */
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

/**
 * Zess T. cooking animation: she walks to the stove, stirs and chops with cooking sounds,
 * the pot reacts (cook_effect for the ingredients in LW(14) and LW(15)),
 * a radiating light plays for special dishes (chk_special_cook on LW(13)),
 * and she walks back to the counter facing Mario.
 */
EVT_BEGIN(cooking_evt_sub)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_COOKING1"))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_status_onoff, 1, STRING("ナンシー"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("ナンシー"), -485, -150, 0, 80, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("ナンシー"), 90)
    CALL(evt_npc_set_anim, STRING("ナンシー"), STRING("S_1R"))
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_INSTANT_TURN)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
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
    CALL(evt_npc_status_onoff, 0, STRING("ナンシー"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_TALK_POSE)
    WAIT_MS(300)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    WAIT_MS(500)
    RETURN
EVT_END()

/** Items offered in the item menu for Trouble Center job 18: only the Cookbook. */
s32 iri_18_item_tbl[] = {
    ITEM_COOKBOOK, -1,
};

/**
 * Zess T.'s talk after the contact lens: cooking service.
 * Handles the Trouble Center job 18 cookbook request first when it is active,
 * then the introduction on the first visit, and then asks which one or two items Mario wants to cook,
 * shows what she makes, removes the ingredients from the pouch, runs cooking_evt_sub and gives Mario the dish,
 * which also unlocks the recipe log.
 */
EVT_BEGIN(cooking_evt)

    // Controls are locked while Mario and his partners walk up to Zess T.'s stove.
    CALL(evt_mario_key_onoff, 0)
#if VERSION >= VERSION_JP
    SET_VALUE(LW(11), -1)
    SET_VALUE(LW(12), -1)
#endif
    SET_VALUE(LW(14), 0)
    SET_VALUE(LW(15), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ナンシー"), NPC_FLAG_NO_HIT)

    // The partners and Mario line up in front of her and face her.
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
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_NO_REAR_POSE)
    END_IF
    CALL(evt_mario_mov_pos2, -377, -100, 60)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_NO_REAR_POSE)
    CALL(evt_mario_set_dir_npc, STRING("ナンシー"))
    WAIT_MS(500)
    WAIT_WHILE(GF(0))
    CALL(evt_set_dir_to_target, STRING("ナンシー"), STRING("mario"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ナンシー"))
    CALL(evt_npc_flag_onoff, 0, STRING("ナンシー"), NPC_FLAG_NO_HIT)

    // Trouble Center job 18: Zess T. has asked Mario to find a Cookbook.
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_SEEKING_LEGENDARY_BOOK)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 0)

            // First time: she explains the request and the book she wants.
            CALL(evt_msg_print, 0, STRING("irai_18_001"), 0, STRING("ナンシー"))
            SET_VALUE(GSWF_TROUBLE_WORK(0), 1)
            CALL(evt_msg_print_add, 0, STRING("irai_18_008_1"))
        ELSE

            // Later: she asks whether he found the book, and Mario can show it in the item menu.
            CALL(evt_msg_print, 0, STRING("irai_18_002"), 0, STRING("ナンシー"))
            SET_VALUE(LF(0), 0)
            CALL(evt_pouch_check_item, ITEM_COOKBOOK, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                CALL(evt_msg_continue)
#if VERSION <= VERSION_DEMO_JP
                CALL(evt_win_item_select, PTR(iri_18_item_tbl), LW(0))
#else
                CALL(evt_win_item_select, 0, PTR(iri_18_item_tbl), LW(0), 0)
#endif
                ELSE
                SET_VALUE(LW(0), -1)
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_NOT_EQUAL(LW(0), -1)

                // Mario hands over the Cookbook; she is thrilled and studies it.
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
                CALL(evt_item_entry, STRING("cb"), ITEM_COOKBOOK, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                CALL(evt_mario_set_pose, STRING("M_C_3"))
                WAIT_MS(800)
                CALL(evt_item_delete, STRING("cb"))
                CALL(evt_mario_set_pose, STRING("M_S_1"))
                WAIT_MS(500)
                CALL(evt_msg_print, 0, STRING("irai_18_004"), 0, STRING("ナンシー"))
                CALL(evt_mario_set_pose, STRING("M_N_1"))
                SET_VALUE(LW(13), ITEM_HONEY_SHROOM)
                SET_VALUE(LW(14), ITEM_MUSHROOM)
                SET_VALUE(LW(15), ITEM_HONEY_SYRUP)

                // She tries out the new recipe in the cooking cutscene and announces the Honey Shroom.
                CALL_CHILD(cooking_evt_sub)
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

                    // Her reward, a Honey Shroom, goes to Mario if he has room in his pouch.
                    CALL(evt_item_entry, STRING("hk"), ITEM_HONEY_SHROOM, 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
                    CALL(evt_item_get_item, STRING("hk"))
                    CALL(evt_item_get_item_end_wait, STRING("hk"))
                    SET_VALUE(GSWF_GOR_01_ZESS_T_HONEY_SHROOM, 1)
                ELSE

                    // Pouch full: she leaves the Honey Shroom at (-440, 30, -80) for him to pick up.
                    CALL(evt_msg_print_add, 0, STRING("irai_18_006_1"))
                    CALL(evt_npc_set_ry, STRING("ナンシー"), 270)
                    WAIT_FRAMES(1)
                    CALL(evt_npc_wait_pera, STRING("ナンシー"))
                    WAIT_MS(300)
                    CALL(evt_item_entry, STRING("iri_18"), ITEM_HONEY_SHROOM, -440, 30, -80, ITEM_TYPE_STILL, GSWF_GOR_01_ZESS_T_HONEY_SHROOM, 0)
                    WAIT_MS(300)
                    CALL(evt_npc_set_ry, STRING("ナンシー"), 90)
                    WAIT_FRAMES(1)
                    CALL(evt_npc_wait_pera, STRING("ナンシー"))
                END_IF

                // Request complete: she can now cook with two ingredients, and the Trouble Center marks it
                // done.
                CALL(evt_msg_print, 0, STRING("irai_18_007"), 0, STRING("ナンシー"))
                SET_VALUE(GSW_TROUBLE_ACCEPTED, TROUBLE_NONE)
                SET_VALUE(GSWF_TROUBLE_DONE(TROUBLE_SEEKING_LEGENDARY_BOOK), 1)
                CALL(evt_msg_print, 0, STRING("sys_request_finish"), 0, 0)
                CALL(evt_mario_key_onoff, 1)
                RETURN
            ELSE

                // Mario does not hand over the book: she tells him where to look before the cooking conversation continues.
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_msg_print, 0, STRING("irai_18_008"), 0, STRING("ナンシー"))
                ELSE
                    CALL(evt_msg_print_add, 0, STRING("irai_18_008"))
                END_IF
                CALL(evt_msg_print_add, 0, STRING("irai_18_008_1"))
            END_IF
        END_IF
    END_IF

    // First visit after the lens: Zess T. introduces herself and her cooking service.
    IF_INT_EQUAL(GSWF_GOR_ZESS_T_COOKING_INTRO_DONE, 0)
        IF_INT_NOT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_SEEKING_LEGENDARY_BOOK)
            IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_SEEKING_LEGENDARY_BOOK), 0)
                CALL(evt_msg_print, 0, STRING("gor_01_cook_000"), 0, STRING("ナンシー"))
            END_IF
        END_IF
        CALL(evt_msg_print_add, 0, STRING("gor_01_cook_001"))
        SET_VALUE(GSWF_GOR_ZESS_T_COOKING_INTRO_DONE, 1)
    ELSE

        // Later visits: she asks if Mario wants her to cook, with a hint about the Cookbook once the story has
        // reached Chapter 4.
        IF_INT_EQUAL(LSWF_GOR_01_ZESS_T_GREETED, 0)
            SET_VALUE(LF(0), 0)
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_15_05_PEACH_X_NAUT_PHD_REPORT)
                IF_INT_NOT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_SEEKING_LEGENDARY_BOOK)
                    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_SEEKING_LEGENDARY_BOOK), 0)
                        SET_VALUE(LF(0), 1)
                    END_IF
                END_IF
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                IF_INT_NOT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_SEEKING_LEGENDARY_BOOK)
                    CALL(evt_msg_print, 0, STRING("gor_01_cook_002"), 0, STRING("ナンシー"))
                ELSE
                    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_002"))
                END_IF
            ELSE
                CALL(evt_msg_print, 0, STRING("gor_01_cook_003"), 0, STRING("ナンシー"))
            END_IF
        ELSE
            IF_INT_NOT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_SEEKING_LEGENDARY_BOOK)
                CALL(evt_msg_print, 0, STRING("gor_01_cook_004"), 0, STRING("ナンシー"))
            ELSE
                CALL(evt_msg_print_add, 0, STRING("gor_01_cook_004"))
            END_IF
        END_IF
    END_IF
    SET_VALUE(LSWF_GOR_01_ZESS_T_GREETED, 1)

    // Mario answers whether he wants her to cook something.
    CALL(evt_msg_select, 0, STRING("gor_01_select"))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(0)
    END_IF
#if VERSION <= VERSION_DEMO_JP
    CALL(make_item_tbl, LW(14), LW(0))
#else
    CALL(make_item_tbl, LW(11), LW(0))
#endif
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("gor_01_cook_005"))
        GOTO(5)
    END_IF

    // She asks for an ingredient; Mario picks it from his pouch (backing out asks again).
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_007"))
    LABEL(1)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_win_item_select, PTR(item_tbl), LW(0))
#else
    CALL(evt_win_item_select, 1, PTR(item_tbl), LW(0), LW(1))
#endif
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

    // With the Cookbook recipes unlocked, Mario may add a second ingredient.
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LF(0), 0)

    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_SEEKING_LEGENDARY_BOOK), 1)
        CALL(make_item_tbl, LW(14), LW(0))
#else
    SET_VALUE(LW(11), LW(1))
    SET_VALUE(LF(0), 0)

    IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_SEEKING_LEGENDARY_BOOK), 1)
        CALL(make_item_tbl, LW(11), LW(0))
#endif
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING("gor_01_cook_009"), 0, STRING("ナンシー"))
            CALL(evt_msg_select, 0, STRING("gor_01_select"))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_continue)
                LABEL(2)
#if VERSION <= VERSION_DEMO_JP
                CALL(evt_win_item_select, PTR(item_tbl), LW(0))
#else
                CALL(evt_win_item_select, 1, PTR(item_tbl), LW(0), LW(1))
#endif
                IF_INT_EQUAL(LW(0), -1)
                    CALL(evt_msg_print, 0, STRING("gor_01_cook_010"), 0, STRING("ナンシー"))
                    CALL(evt_msg_select, 0, STRING("gor_01_cook_select"))
                    SWITCH(LW(0))
                        CASE_EQUAL(0)
                        SET_VALUE(LF(0), 1)
                        GOTO(3)
                        CASE_EQUAL(1)
#if VERSION >= VERSION_JP
                        CALL(evt_msg_continue)
#endif
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
#if VERSION >= VERSION_JP
                SET_VALUE(LW(12), LW(1))
#endif
                ELSE
                SET_VALUE(LF(0), 1)
            END_IF
        END_IF
    END_IF

    // She repeats what she will cook, and Mario confirms or cancels.
    LABEL(3)
    IF_INT_EQUAL(LW(15), 0)
        CALL(get_item_name, LW(14), LW(0))

        IF_INT_EQUAL(LF(0), 0)
#if VERSION >= VERSION_DEMO_EU
            CALL(evt_msg_fmt_str, 0, LW(2), STRING("gor_01_cook_012"), LW(0))
            CALL(evt_msg_print, 1, LW(2), 0, STRING("ナンシー"))
        ELSE
            CALL(evt_msg_fmt_str, 0, LW(2), STRING("gor_01_cook_012"), LW(0))
            CALL(evt_msg_print_add, 1, LW(2))
#else
            CALL(evt_msg_print_insert, 0, STRING("gor_01_cook_012"), 0, STRING("ナンシー"), LW(0))
            ELSE
            CALL(evt_msg_print_add_insert, 0, STRING("gor_01_cook_012"), LW(0))
#endif
        END_IF
        ELSE
        CALL(get_item_name, LW(14), LW(0))
        CALL(get_item_name, LW(15), LW(1))
#if VERSION >= VERSION_DEMO_EU
        CALL(evt_msg_fmt_str, 0, LW(2), STRING("gor_01_cook_013"), LW(0))
        CALL(evt_msg_fmt_str, 1, LW(2), LW(2), LW(1))
        CALL(evt_msg_print, 1, LW(2), 0, STRING("ナンシー"))
#else
        CALL(evt_msg_print_insert, 0, STRING("gor_01_cook_013"), 0, STRING("ナンシー"), LW(0), LW(1))
#endif
    END_IF

    // Mario answers whether he wants her to cook something.
    CALL(evt_msg_select, 0, STRING("gor_01_select"))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(4)
    END_IF

    // The ingredients leave the pouch and the dish is worked out from the recipe.
    IF_INT_EQUAL(LW(15), 0)
        CALL(cook_chk, LW(14), LW(0), LW(1))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_pouch_remove_item, LW(14), LW(3))
#else
        CALL(evt_pouch_remove_item_index, LW(14), LW(11), LW(3))
#endif
        IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_SEEKING_LEGENDARY_BOOK), 1)
            SET_VALUE(LW(2), LW(1))
        ELSE
            SET_VALUE(LW(2), LW(0))
        END_IF
        ELSE
        CALL(cook_chk2, LW(14), LW(15), LW(2))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_pouch_remove_item, LW(14), LW(3))
        CALL(evt_pouch_remove_item, LW(15), LW(3))
#else
        CALL(evt_pouch_remove_item_index, LW(14), LW(11), LW(3))
        CALL(evt_pouch_remove_item_index, LW(15), LW(12), LW(3))
#endif
    END_IF
    IF_INT_EQUAL(LW(2), 0)
        SET_VALUE(LW(2), ITEM_MISTAKE)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_015"))

    // The cooking cutscene plays.
    SET_VALUE(LW(13), LW(2))
    CALL_CHILD(cooking_evt_sub)
    SET_VALUE(LW(2), LW(13))
    CALL(chk_cook_msg, LW(2), LW(3))

    // Her reaction depends on how the dish turned out.
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

    // She hands over the dish; the recipe log is unlocked and she invites Mario to come back.
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_022"))
    CALL(evt_item_entry, STRING("cook"), LW(2), 0, -1000, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_item_get_item, STRING("cook"))
    SET_VALUE(GSWF_GOR_ZESS_T_DISH_RECEIVED, 1)
    CALL(cooking_flag, LW(2))
    CALL(evt_msg_print, 0, STRING("gor_01_cook_023"), 0, STRING("ナンシー"))
    GOTO(5)

    // Mario declined to have anything cooked: she is let down.
    LABEL(0)
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_006"))
    GOTO(5)

    // Mario cancelled at the last moment: she tells him to come back whenever.
    LABEL(4)
    CALL(evt_msg_print_add, 0, STRING("gor_01_cook_014"))
    GOTO(5)

    // The visit ends and Mario's controls return.
    LABEL(5)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Picks which reaction Zess T. has to a finished dish, returned in args[1]:
 * 0 and 1 for the best dishes, 2 for a failure (Mistake), 3 or 4 for the rest by cooking effect type.
 */
USER_FUNC(chk_cook_msg) {
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);
    s32 msg;

    if (item == ITEM_ZESS_DELUXE || item == ITEM_ZESS_DYNAMITE) {
        msg = 0;
    } else if ((item >= ITEM_ZESS_DINNER && item <= ITEM_ZESS_TEA) || item == ITEM_ZESS_COOKIE || item == ITEM_ZESS_FRAPPE) {
        msg = 1;
    } else if (item == ITEM_MISTAKE) {
        msg = 2;
    } else {
        msg = chk_cook_eff_type(item) + 3;
    }
    evtSetValue(event, args[1], msg);
    return EVT_RETURN_DONE;
}

/** Marks dish args[0] as cooked in the recipe log (swSet with the item number minus 114). */
USER_FUNC(cooking_flag) {
    swSet(evtGetValue(event, event->args[0]) - 114);
    return EVT_RETURN_DONE;
}

/**
 * Returns in args[1] whether item args[0] is in special_cook_list,
 * the dishes that get the special radiating light when cooked.
 */
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


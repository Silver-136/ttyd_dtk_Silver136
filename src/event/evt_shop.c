// Scripts and commands for the item shops: the shop list window, buying, selling, shop points.

#include "version.h"
#include "evt/evt_window.h"
#include "evt/evt_shop.h"
#include "evt/evt_cmd.h"
#include "evt/evt_case.h"
#include "driver/casedrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/winmgr.h"
#include "manager/fontmgr.h"
#include "driver/msgdrv.h"
#include "driver/icondrv.h"
#include "driver/mapdrv.h"
#include "driver/npcdrv.h"
#include "driver/hitdrv.h"
#include "driver/camdrv.h"
#include "driver/swdrv.h"
#include "mario/mario.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "data/item_data.h"
#include "party.h"
#include "system.h"
#include "memory.h"
#include "pmario_sound.h"
#include <dolphin/mtx.h>
#include <string.h>
#include <stdio.h>
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "win/win_mario.h"
#include "driver/itemdrv.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_sub.h"
#include "win/win_main.h"
#include "evt/evt_pouch.h"
#include "icon_ids.h"


void FontDrawString(f32 x, f32 y, const char* str);

//.sbss
static ShopWork* _wp;
char mapname[16];

void list_disp(WinMgrEntry* entry);
void title_disp(WinMgrEntry* entry);
void point_disp(WinMgrEntry* entry);
void help_disp(WinMgrEntry* entry);
void help_main(WinMgrEntry* entry);
void name_disp(WinMgrEntry* entry);

USER_FUNC(disp_off);
USER_FUNC(get_ptr);
USER_FUNC(get_value);

/**
 * Text colours of the shop windows: white for the item name, price, point count, list title and the list's
 * "Pts." and "Prizes" headings; in the Shop Prizes list, prizes already earned are red and the rest black.
 */
static const GXColor shop_name_color = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor shop_price_color = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor shop_point_num_color = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor shop_title_color = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor shop_list_head5_color = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor shop_list_head6_color = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor shop_list_pts_got_color = {0xF0, 0x00, 0x00, 0xFF};
static const GXColor shop_list_pts_color = {0x00, 0x00, 0x00, 0xFF};
static const GXColor shop_list_unit_got_color = {0xF0, 0x00, 0x00, 0xFF};
static const GXColor shop_list_unit_color = {0x00, 0x00, 0x00, 0xFF};
static const GXColor shop_list_item_got_color = {0xF0, 0x00, 0x00, 0xFF};
static const GXColor shop_list_item_color = {0x00, 0x00, 0x00, 0xFF};

static WinMgrDesc window_desc[3];
ShopPoint shopPointList[11];
WinMgrDesc list_desc[3];
static EvtWord buy_evt[VERSION_PICK(326, 366)];
EvtWord shopper_talk_evt[];
EvtWord first_shopping_evt[86];
#if VERSION <= VERSION_DEMO_US
static EvtWord sell_evt[193];
#else
static EvtWord sell_evt[VERSION_PICK(196, 207)];
#endif
EvtWord keep_evt[VERSION_PICK(148, 167)];
EvtWord unkeep_evt[147];
EvtWord point_evt[VERSION_PICK(15, 35)];
EvtWord evt_shoplist[10];
static EvtWord main_evt[4];

/**
 * Windows shown while browsing a shop shelf: the item name and price,
 * the item description and the shop point count.
 */
static WinMgrDesc window_desc[] = {
    {0, 0, 8, -150, 120, 300, 50, {0x2B, 0xA4, 0xFF, 0xFF}, NULL, name_disp},
    {0, 0, 8, -240, -130, 500, 80, {0xF0, 0xF0, 0xF0, 0xFF}, help_main, help_disp},
    {2, 0, 8, -250, -130, 150, 45, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, point_disp},
};

/**
 * Shop point prizes: 10 Mushroom, 20 Ice Storm, 30 Super Shroom, 50 Thunder Rage, 70 Shooting Star,
 * 100 Gold Bar, 150 Life Shroom, 200 Ultra Shroom, 250 Jammin' Jelly
 * and 300 Gold Bar x3 (the point count then resets). Ends with -1.
 * Read by the prize checks and the Shop Prizes list.
 */
ShopPoint shopPointList[] = {
    {10, ITEM_MUSHROOM},
    {20, ITEM_ICE_STORM},
    {30, ITEM_SUPER_SHROOM},
    {50, ITEM_THUNDER_RAGE},
    {70, ITEM_SHOOTING_STAR},
    {100, ITEM_GOLD_BAR},
    {150, ITEM_LIFE_SHROOM},
    {200, ITEM_ULTRA_SHROOM},
    {250, ITEM_JAMMIN_JELLY},
    {300, ITEM_GOLD_BAR_X3},
    {-1, -1},
};

/**
 * Windows of the Shop Prizes list ("Just checking points..."): the title,
 * the prize list and the shop point count.
 */
WinMgrDesc list_desc[] = {
    {0, 0, 8, -280, 160, 210, 80, {0x2B, 0xA4, 0xFF, 0xFF}, NULL, title_disp},
    {0, 3, 8, -60, 160, 340, 300, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, list_disp},
    {2, 0, 8, -250, -130, 150, 45, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, point_disp},
};

/**
 * Runs when Mario presses A on a shelf item (started by evt_shop_main_func with LW(0) = the ShopWork):
 * the shopkeeper names the item and price, and on yes takes the coins, gives the item,
 * adds a shop point (doubled during the timed sales at the Great Tree (mri_07),
 * Twilight Town (usu_01) and the Excess Express (rsh_03)), hands out any point prizes reached,
 * then runs the shop's own buy_evt with LW(0) = the item id.
 */
#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(evt_shoplist)
    CALL(evt_mario_key_onoff, 0)
    CALL(disp_list)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
static EVT_BEGIN(buy_evt)
    // Mario stops and the item's highlight goes away;
    // a first-time shopper hears the shopping explanation instead.
    CALL(evt_mario_normalize)
    SET_VALUE(LW(10), LW(0))
    CALL(disp_off, LW(10))
    IF_INT_EQUAL(GSWF_SHOP_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_SHOP_TUTORIAL_SEEN, 1)
        CALL_CHILD(PTR(first_shopping_evt))
        RETURN
    END_IF
    // The shopkeeper turns to Mario, the coin and point windows open, and he names the item and its price.
    CALL(shopper_name, LW(9))
    CALL(evt_set_dir_to_target, LW(9), STRING("mario"))
    CALL(evt_win_coin_on, 0, LW(8))
    CALL(point_disp_onoff, 1)
    CALL(get_value, LW(10), LW(11), LW(12), LW(13))
    CALL(get_msg, LW(10), 0, LW(14))
    CALL(evt_sub_get_language, LW(0))
IF_INT_NOT_EQUAL(LW(0), LANGUAGE_JP)
        CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(13), LW(11))
        ELSE
        CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(11), LW(13))
    END_IF
CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        CALL(evt_win_coin_off, LW(8))
        CALL(point_disp_onoff, 0)
        RETURN
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(13))
        CALL(get_msg, LW(10), 1, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        CALL(evt_win_coin_off, LW(8))
        CALL(point_disp_onoff, 0)
        RETURN
    END_IF
    CALL(evt_pouch_add_item, LW(12), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(get_msg, LW(10), 2, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        CALL(evt_win_coin_off, LW(8))
        CALL(point_disp_onoff, 0)
        RETURN
    END_IF
    // Bought: a shop point is added (two during a timed double-points sale) and the coins are paid.
    CALL(set_buy_item_id, LW(12))
    CALL(evt_sub_load_progresstime, GSW_SHOP_MRI_07_SALE_START_TIME, LW(0))
    CALL(evt_sub_load_progresstime, GSW_SHOP_USU_01_SALE_START_TIME, LW(1))
    CALL(evt_sub_load_progresstime, GSW_SHOP_RSH_03_SALE_START_TIME, LW(2))
    CALL(add_shop_point, 1, LW(0), LW(1), LW(2))
    MULTIPLY(LW(13), -1)
    CALL(evt_pouch_add_coin, LW(13))
    CALL(evt_win_coin_wait, LW(8))
    CALL(point_wait)
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(8))
    CALL(point_disp_onoff, 0)
CALL(evt_msg_continue)
CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_CLEAR(LW(0), GW_FLAG_E3)
        IF_INT_EQUAL(GSWF_ITEM_TUTORIAL_SEEN, 0)
            SET_VALUE(GSWF_ITEM_TUTORIAL_SEEN, 1)
            CALL(evt_msg_print, 0, STRING("item_first_hit"), 0, 0)
        END_IF
    END_IF
    // Hand out every shop point prize now reached; at 300 points the shopkeeper announces the maximum and the
    // count resets.
    LOOP(0)
        CALL(chk_shop_point, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LW(0), -1)
            GOTO(123)
            RETURN
        END_IF
        SET_VALUE(LF(0), 0)
        CALL(get_shop_point, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 300)
            CALL(get_msg, LW(10), 26, LW(14))
CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(2))
ELSE
            CALL(get_msg, LW(10), 34, LW(14))
            CALL(evt_msg_print, 0, LW(14), 0, LW(9))
            CALL(evt_win_coin_on, 0, LW(8))
            CALL(point_disp_onoff, 1)
            CALL(reset_shop_point)
            CALL(evt_win_coin_wait, LW(8))
            CALL(point_wait)
            WAIT_MS(500)
            CALL(evt_win_coin_off, LW(8))
            CALL(point_disp_onoff, 0)
            SET_VALUE(LF(0), 1)
        END_IF
        // With a full item bag the prize is kept for later (the 300-point prize is remembered by a flag).
        CALL(evt_pouch_add_item, LW(1), LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(get_msg, LW(10), 27, LW(14))
            CALL(evt_msg_print, 0, LW(14), 0, LW(9))
            IF_INT_EQUAL(LF(0), 1)
                SET_VALUE(GSWF_SHOP_POINT_PRIZE_PENDING, 1)
            END_IF
            GOTO(123)
            RETURN
        END_IF
        CALL(exchange_shop_point)
    END_LOOP
    // Finally the shop's own after-purchase script runs, if it has one.
    LABEL(123)
    CALL(get_buy_evt, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SET_VALUE(LW(0), LW(12))
        CALL_CHILD(LW(1))
    END_IF
    RETURN
EVT_END()
#else
static EVT_BEGIN(buy_evt)
    // Mario stops and the item's highlight goes away;
    // a first-time shopper hears the shopping explanation instead.
    CALL(evt_mario_normalize)
    SET_VALUE(LW(10), LW(0))
    CALL(disp_off, LW(10))
    IF_INT_EQUAL(GSWF_SHOP_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_SHOP_TUTORIAL_SEEN, 1)
        CALL_CHILD(PTR(first_shopping_evt))
        RETURN
    END_IF
    // The shopkeeper turns to Mario, the coin and point windows open, and he names the item and its price.
    CALL(shopper_name, LW(9))
    CALL(evt_set_dir_to_target, LW(9), STRING("mario"))
    CALL(evt_win_coin_on, 0, LW(8))
    CALL(point_disp_onoff, 1)
    CALL(get_value, LW(10), LW(11), LW(12), LW(13))
    CALL(get_msg, LW(10), 0, LW(14))
    CALL(evt_sub_get_language, LW(0))
#if VERSION >= VERSION_US
    SWITCH(LW(0))
        CASE_EQUAL(LANGUAGE_JP)
        CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(11), LW(13))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), LW(14), LW(13))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(11))
        CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, LW(9))
    END_SWITCH
#else
    IF_INT_NOT_EQUAL(LW(0), LANGUAGE_JP)
        CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(13), LW(11))
        ELSE
        CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(11), LW(13))
    END_IF
#endif
    // Mario decides: "no", too few coins or a full item bag each end the purchase with the shopkeeper's line.
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        CALL(evt_win_coin_off, LW(8))
        CALL(point_disp_onoff, 0)
        RETURN
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(13))
        CALL(get_msg, LW(10), 1, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        CALL(evt_win_coin_off, LW(8))
        CALL(point_disp_onoff, 0)
        RETURN
    END_IF
    CALL(evt_pouch_add_item, LW(12), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(get_msg, LW(10), 2, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        CALL(evt_win_coin_off, LW(8))
        CALL(point_disp_onoff, 0)
        RETURN
    END_IF
    // Bought: a shop point is added (two during a timed double-points sale) and the coins are paid.
    CALL(set_buy_item_id, LW(12))
    CALL(evt_sub_load_progresstime, GSW_SHOP_MRI_07_SALE_START_TIME, LW(0))
    CALL(evt_sub_load_progresstime, GSW_SHOP_USU_01_SALE_START_TIME, LW(1))
    CALL(evt_sub_load_progresstime, GSW_SHOP_RSH_03_SALE_START_TIME, LW(2))
    CALL(add_shop_point, 1, LW(0), LW(1), LW(2))
    MULTIPLY(LW(13), -1)
    CALL(evt_pouch_add_coin, LW(13))
    CALL(evt_win_coin_wait, LW(8))
    CALL(point_wait)
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(8))
    CALL(point_disp_onoff, 0)
#if VERSION >= VERSION_US
    CALL(get_msg, LW(10), 11, LW(14))
    CALL(evt_msg_print_add, 0, LW(14))
#else
    CALL(evt_msg_continue)
#endif
    // The first item ever bought brings up the item tutorial, except on kiosk units.
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_CLEAR(LW(0), GW_FLAG_E3)
        IF_INT_EQUAL(GSWF_ITEM_TUTORIAL_SEEN, 0)
            SET_VALUE(GSWF_ITEM_TUTORIAL_SEEN, 1)
            CALL(evt_msg_print, 0, STRING("item_first_hit"), 0, 0)
        END_IF
    END_IF
    // Hand out every shop point prize now reached; at 300 points the shopkeeper announces the maximum and the
    // count resets.
    LOOP(0)
        CALL(chk_shop_point, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LW(0), -1)
            GOTO(123)
            RETURN
        END_IF
        SET_VALUE(LF(0), 0)
        CALL(get_shop_point, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 300)
            CALL(get_msg, LW(10), 26, LW(14))
#if VERSION >= VERSION_US
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(LANGUAGE_JP)
                CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(2))
                CASE_DEFAULT
                CALL(evt_msg_fmt_str, 0, LW(14), LW(14), LW(2))
                CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, LW(9))
                CASE_END
            END_SWITCH
#else
            CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(2))
#endif
            ELSE
            CALL(get_msg, LW(10), 34, LW(14))
            CALL(evt_msg_print, 0, LW(14), 0, LW(9))
            CALL(evt_win_coin_on, 0, LW(8))
            CALL(point_disp_onoff, 1)
            CALL(reset_shop_point)
            CALL(evt_win_coin_wait, LW(8))
            CALL(point_wait)
            WAIT_MS(500)
            CALL(evt_win_coin_off, LW(8))
            CALL(point_disp_onoff, 0)
            SET_VALUE(LF(0), 1)
        END_IF
        // With a full item bag the prize is kept for later (the 300-point prize is remembered by a flag).
        CALL(evt_pouch_add_item, LW(1), LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(get_msg, LW(10), 27, LW(14))
            CALL(evt_msg_print, 0, LW(14), 0, LW(9))
            IF_INT_EQUAL(LF(0), 1)
                SET_VALUE(GSWF_SHOP_POINT_PRIZE_PENDING, 1)
            END_IF
            GOTO(123)
            RETURN
        END_IF
        CALL(exchange_shop_point)
    END_LOOP
    // Finally the shop's own after-purchase script runs, if it has one.
    LABEL(123)
    CALL(get_buy_evt, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SET_VALUE(LW(0), LW(12))
        CALL_CHILD(LW(1))
    END_IF
    RETURN
EVT_END()
#endif

/**
 * Talk script the shop setup gives the shopkeeper NPC:
 * runs the shop's fook (hook) script first, introduces the shop on the first visit,
 * hands out unclaimed shop point prizes, then offers the counter menu (sell, store items, take stored items out,
 * check points).
 * LW(10) holds the ShopWork; the hook returning -1 in LW(0) ends the talk,
 * -100 continues the hook's message box.
 */
EVT_BEGIN(shopper_talk_evt)
    CALL(evt_mario_key_onoff, 0)
    // The shop's hook script runs first and may end the talk or keep its message box open.
    CALL(get_ptr, LW(10))
    SET_VALUE(LF(10), 0)
    CALL(get_fook_evt, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mario_key_onoff, 1)
            RETURN
        END_IF
        IF_INT_EQUAL(LW(0), -100)
            SET_VALUE(LF(10), 1)
        END_IF
    END_IF
    // A first-time shopper hears the shopping explanation.
    IF_INT_EQUAL(GSWF_SHOP_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_SHOP_TUTORIAL_SEEN, 1)
        CALL_CHILD(PTR(first_shopping_evt))
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SET_VALUE(LF(0), 0)
    // Hand out shop point prizes Mario earned but could not carry, one at a time.
    LOOP(0)
        CALL(chk_shop_point, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(GSWF_SHOP_POINT_PRIZE_PENDING, 1)
            SET_VALUE(LW(0), 0)
            CALL(shop_point_item, 300, LW(1), LW(2))
        END_IF
        IF_INT_EQUAL(LW(0), -1)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_mario_key_onoff, 1)
                RETURN
            END_IF
            // Nothing (left) to hand out: greet Mario, with the double-points greeting during a timed sale at
            // the Great Tree, Twilight Town or the Excess Express shop.
            SET_VALUE(LF(11), 0)
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("mri_07"))
                IF_INT_EQUAL(GSWF_SHOP_MRI_07_DOUBLE_POINTS_SALE, 1)
                    CALL(evt_sub_load_progresstime, GSW_SHOP_MRI_07_SALE_START_TIME, LW(0))
                    IF_INT_LESS(LW(0), 900000)
                        SET_VALUE(LW(14), STRING("stg2_shop_04_1"))
                        SET_VALUE(LF(11), 1)
                    END_IF
                END_IF
            END_IF
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("usu_01"))
                IF_INT_EQUAL(GSWF_SHOP_USU_01_DOUBLE_POINTS_SALE, 1)
                    CALL(evt_sub_load_progresstime, GSW_SHOP_USU_01_SALE_START_TIME, LW(0))
                    IF_INT_LESS(LW(0), 900000)
                        SET_VALUE(LW(14), STRING("usugurashop_04_1"))
                        SET_VALUE(LF(11), 1)
                    END_IF
                END_IF
            END_IF
            SET_VALUE(LF(12), 0)
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("rsh_03_a"))
                SET_VALUE(LF(12), 1)
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_03_b"))
                SET_VALUE(LF(12), 1)
            END_IF
            IF_STR_EQUAL(LW(0), STRING("rsh_03_c"))
                SET_VALUE(LF(12), 1)
            END_IF
            IF_INT_EQUAL(LF(12), 1)
                IF_INT_EQUAL(GSWF_SHOP_RSH_03_DOUBLE_POINTS_SALE, 1)
                    CALL(evt_sub_load_progresstime, GSW_SHOP_RSH_03_SALE_START_TIME, LW(0))
                    IF_INT_LESS(LW(0), 900000)
                        SET_VALUE(LW(14), STRING("stg6_shop_04_1"))
                        SET_VALUE(LF(11), 1)
                    END_IF
                END_IF
            END_IF
            IF_INT_EQUAL(LF(11), 0)
                CALL(get_msg, LW(10), 4, LW(14))
            END_IF
            IF_INT_EQUAL(LF(10), 0)
                CALL(evt_msg_print, 0, LW(14), 0, 0)
                ELSE
                SET_VALUE(LF(10), 0)
                CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
                CALL(evt_msg_print_add, 0, LW(14))
            END_IF
            BREAK_LOOP
        END_IF
        // A prize that still does not fit is kept for later.
        CALL(evt_pouch_add_item, LW(1), LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(get_msg, LW(10), 29, LW(14))
            IF_INT_EQUAL(LF(10), 0)
                CALL(evt_msg_print, 0, LW(14), 0, 0)
                ELSE
                SET_VALUE(LF(10), 0)
                CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
                CALL(evt_msg_print_add, 0, LW(14))
            END_IF
            BREAK_LOOP
        END_IF
        // "Here's your prize": the prize is marked as given.
        CALL(get_msg, LW(10), 28, LW(14))
        IF_INT_EQUAL(LF(10), 0)
#if VERSION >= VERSION_US
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(LANGUAGE_JP)
                CALL(evt_msg_print_insert, 0, LW(14), 0, 0, LW(2))
                CASE_DEFAULT
                CALL(evt_msg_fmt_str, 0, LW(14), LW(14), LW(2))
                CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, 0)
            END_SWITCH
#else
            CALL(evt_msg_print_insert, 0, LW(14), 0, 0, LW(2))
#endif
            ELSE
            SET_VALUE(LF(10), 0)
            CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
#if VERSION >= VERSION_US
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(LANGUAGE_JP)
#if VERSION == VERSION_EU
                CALL(evt_msg_print_add_insert, 0, LW(14), LW(2))
#else
                CALL(evt_msg_print_insert, 0, LW(14), 0, 0, LW(2))
#endif
                CASE_DEFAULT
                CALL(evt_msg_fmt_str, 0, LW(14), LW(14), LW(2))
#if VERSION == VERSION_EU
                CALL(evt_msg_print_add, 1, LW(14))
#else
                CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, 0)
#endif
            END_SWITCH
#else
            CALL(evt_msg_print_add_insert, 0, LW(14), LW(2))
#endif
        END_IF
        IF_INT_EQUAL(GSWF_SHOP_POINT_PRIZE_PENDING, 0)
            CALL(exchange_shop_point)
        END_IF
        SET_VALUE(LF(0), 1)
        IF_INT_EQUAL(GSWF_SHOP_POINT_PRIZE_PENDING, 1)
            SET_VALUE(GSWF_SHOP_POINT_PRIZE_PENDING, 0)
        END_IF
    END_LOOP
    // The counter menu: shop, sell, store, withdraw or check shop points.
    CALL(shop_flag_onoff, 1, SHOP_FLAG_TALKING)
    CALL(get_msg, LW(10), 23, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    CALL(shop_flag_onoff, 0, SHOP_FLAG_TALKING)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(get_msg, LW(10), 5, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(sell_evt))
        CASE_EQUAL(2)
        CALL_CHILD(PTR(keep_evt))
        CASE_EQUAL(3)
        CALL_CHILD(PTR(unkeep_evt))
        CASE_EQUAL(4)
        CALL_CHILD(PTR(point_evt))
        CASE_EQUAL(5)
        CALL(evt_msg_continue)
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
EVT_BEGIN(first_shopping_evt)
    CALL(shopper_name, LW(9))
    CALL(evt_set_dir_to_target, LW(9), STRING("mario"))
    CALL(evt_mario_set_dir_npc, LW(9))
    CALL(get_msg, LW(10), 30, LW(14))
    IF_INT_EQUAL(LF(10), 0)
        CALL(evt_msg_print, 0, LW(14), 0, LW(9))
        ELSE
        SET_VALUE(LF(10), 0)
        CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
        CALL(evt_msg_print_add, 0, LW(14))
    END_IF
    LOOP(0)
        CALL(get_msg, LW(10), 31, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, LW(9))
        CALL(get_msg, LW(10), 24, LW(14))
        CALL(evt_msg_select, 0, LW(14))
        IF_INT_EQUAL(LW(0), 0)
            CALL(get_msg, LW(10), 32, LW(14))
            CALL(evt_msg_print_add, 0, LW(14))
            ELSE
            CALL(get_msg, LW(10), 33, LW(14))
            CALL(evt_msg_print_add, 0, LW(14))
            RETURN
        END_IF
    END_LOOP
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
                        static EVT_BEGIN(sell_evt)
    CALL(sell_pouchcheck_func)
    IF_INT_EQUAL(LW(0), 0)
        CALL(get_msg, LW(10), 6, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 7, LW(14))
    CALL(evt_msg_print_add, 0, LW(14))
    CALL(evt_win_coin_on, 0, LW(8))
    LABEL(0)
    CALL(item_data_db_arrange)
    CALL(evt_win_item_select, 3, LW(1))
    CALL(item_data_db_restore)
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(item_data_db_arrange)
    CALL(name_price, LW(1), LW(2), LW(3))
    CALL(item_data_db_restore)
    CALL(get_msg, LW(10), 8, LW(14))
    CALL(evt_sub_get_language, LW(0))
    IF_INT_NOT_EQUAL(LW(0), LANGUAGE_JP)
        CALL(evt_msg_print_insert, 0, LW(14), 0, STRING("me"), LW(3), LW(2))
    ELSE
        CALL(evt_msg_print_insert, 0, LW(14), 0, STRING("me"), LW(2), LW(3))
    END_IF
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 9, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        GOTO(0)
    END_IF
    CALL(evt_pouch_remove_item, LW(1), LW(0))
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(8))
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(8))
    CALL(sell_pouchcheck_func)
    IF_INT_EQUAL(LW(0), 0)
        CALL(get_msg, LW(10), 11, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 10, LW(14))
    CALL(evt_msg_print_add, 0, LW(14))
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_win_coin_on, 0, LW(8))
    GOTO(0)
    RETURN
EVT_END()
#else
static EVT_BEGIN(sell_evt)
    CALL(sell_pouchcheck_func)
    IF_INT_EQUAL(LW(0), 0)
        CALL(get_msg, LW(10), 6, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 7, LW(14))
    CALL(evt_msg_print_add, 0, LW(14))
    CALL(evt_win_coin_on, 0, LW(8))
    LABEL(0)
    CALL(item_data_db_arrange)
    CALL(evt_win_item_select, 1, 3, LW(1), LW(4))
    CALL(item_data_db_restore)
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        CALL(evt_win_coin_off, LW(8))
        RETURN
    END_IF
    CALL(item_data_db_arrange)
    CALL(name_price, LW(1), LW(2), LW(3))
    CALL(item_data_db_restore)
    CALL(get_msg, LW(10), 8, LW(14))
    CALL(evt_sub_get_language, LW(0))
#if VERSION >= VERSION_US
    SWITCH(LW(0))
        CASE_EQUAL(LANGUAGE_JP)
        CALL(evt_msg_print_insert, 0, LW(14), 0, STRING("me"), LW(2), LW(3))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), LW(14), LW(3))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
        CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("me"))
    END_SWITCH
#else
    IF_INT_NOT_EQUAL(LW(0), LANGUAGE_JP)
        CALL(evt_msg_print_insert, 0, LW(14), 0, STRING("me"), LW(3), LW(2))
        ELSE
        CALL(evt_msg_print_insert, 0, LW(14), 0, STRING("me"), LW(2), LW(3))
    END_IF
#endif
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 9, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        GOTO(0)
    END_IF
    CALL(evt_pouch_remove_item_index, LW(1), LW(4), LW(0))
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(8))
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(8))
    CALL(sell_pouchcheck_func)
    IF_INT_EQUAL(LW(0), 0)
        CALL(get_msg, LW(10), 11, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 10, LW(14))
    CALL(evt_msg_print_add, 0, LW(14))
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    CALL(evt_win_coin_on, 0, LW(8))
    GOTO(0)
    RETURN
EVT_END()
#endif

#if VERSION <= VERSION_DEMO_US
                        EVT_BEGIN(keep_evt)
    CALL(keep_pouchcheck_func)
    IF_INT_EQUAL(LW(0), 0)
        CALL(get_msg, LW(10), 12, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    IF_INT_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 13, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 14, LW(14))
    CALL(evt_msg_print_add_insert, 0, LW(14), LW(1))
    LABEL(0)
    CALL(evt_win_item_select, 4, LW(1))
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
    CALL(evt_pouch_remove_item, LW(1), LW(0))
    CALL(evt_pouch_add_keepitem, LW(1), LW(0))
    CALL(keep_pouchcheck_func)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(1), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(get_msg, LW(10), 15, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 16, LW(14))
    CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    GOTO(0)
    RETURN
EVT_END()
#else
EVT_BEGIN(keep_evt)
    CALL(keep_pouchcheck_func)
    IF_INT_EQUAL(LW(0), 0)
        CALL(get_msg, LW(10), 12, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    IF_INT_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 13, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 14, LW(14))
#if VERSION >= VERSION_US
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(LANGUAGE_JP)
        CALL(evt_msg_print_add_insert, 0, LW(14), LW(1))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), LW(14), LW(1))
        CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, LW(14))
    END_SWITCH
#else
    CALL(evt_msg_print_add_insert, 0, LW(14), LW(1))
#endif
    LABEL(0)
    CALL(evt_win_item_select, 1, 4, LW(1), LW(4))
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
    CALL(evt_pouch_remove_item_index, LW(1), LW(4), LW(0))
    CALL(evt_pouch_add_keepitem, LW(1), LW(0))
    CALL(keep_pouchcheck_func)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(1), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(get_msg, LW(10), 15, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 16, LW(14))
    CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    GOTO(0)
    RETURN
EVT_END()
#endif

/**
 * Counter menu option for taking stored items back:
 * picks from the stored-item list while Mario's pouch has room and puts each back in the pouch.
 */
EVT_BEGIN(unkeep_evt)
    CALL(unkeep_pouchcheck_func)
    IF_INT_EQUAL(LW(0), 0)
        CALL(get_msg, LW(10), 17, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    IF_INT_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 18, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 19, LW(14))
    CALL(evt_msg_print_add, 0, LW(14))
    LABEL(0)
#if VERSION <= VERSION_DEMO_US
CALL(evt_win_item_select, 5, LW(1))
#else
    CALL(evt_win_item_select, 1, 5, LW(1), LW(4))
#endif
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
#if VERSION <= VERSION_DEMO_US
CALL(evt_pouch_remove_keepitem, LW(1), LW(0))
#else
    CALL(evt_pouch_remove_keepitem, LW(1), LW(4), LW(0))
#endif
    CALL(evt_pouch_add_item, LW(1), LW(0))
    CALL(unkeep_pouchcheck_func)
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(1), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(get_msg, LW(10), 20, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
    CALL(get_msg, LW(10), 21, LW(14))
    CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    GOTO(0)
    RETURN
EVT_END()

#if VERSION >= VERSION_US
/** Counter menu option that tells Mario how many shop points he has. */
EVT_BEGIN(point_evt)
    CALL(get_shop_point, LW(1))
    CALL(get_msg, LW(10), 25, LW(14))
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(LANGUAGE_JP)
            CALL(evt_msg_print_add_insert, 0, LW(14), LW(1))
        CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), LW(14), LW(1))
            CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, LW(14))
    END_SWITCH
    RETURN
    OPCODE_END_SCRIPT,
    0 // US: the object is 0x8C bytes (trailing zero word)
};
#else
/** Counter menu option that tells Mario how many shop points he has. */
EVT_BEGIN(point_evt)
    CALL(get_shop_point, LW(0))
    CALL(get_msg, LW(10), 25, LW(14))
    CALL(evt_msg_print_add_insert, 0, LW(14), LW(0))
    RETURN
EVT_END()
#endif

#if VERSION >= VERSION_US
// US: point window icon per language (first object of evt_shop .sdata)
u16 point_disp_icons[] = {0x226, 0x227, 0x228, 0x227, 0x229, 0x22A};
#endif

#if VERSION > VERSION_DEMO_US
EVT_BEGIN(evt_shoplist)
    CALL(evt_mario_key_onoff, 0)
    CALL(disp_list)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
#endif

/** Per-shop driver script; evt_shop_main_func does all the work every frame. */
static EVT_BEGIN(main_evt)
    CALL(evt_shop_main_func)
    RETURN
EVT_END()

void name_disp(WinMgrEntry* entry) {
    ShopWork* wp = _wp;
    const char* name;
    s32 i;
    Vec pos;
    Mtx m;
    char buf[64];

    name = msgSearch(itemDataTable[wp->items[wp->cur].id].name);
    if (!winMgrAction(wp->win[0])) {
        FontDrawStart();
        FontDrawEdge();
        FontDrawColor(shop_name_color);
        FontDrawString(-FontGetMessageWidth(name) / 2, 108.0f, name);
        FontDrawEdgeOff();
    }
    for (i = 0; i < wp->count; i++) {
        if (wp->itemFlags[i] & SHOP_ITEM_FLAG_SOLD_OUT) {
            continue;
        }
        mapObjGetPos(wp->objs[i].mobj, &pos);
        pos.y += 30.0f;
        PSMTXMultVec(camGetPtr(CAMERA_3D)->view, &pos, &pos);
        PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &pos, &pos);
        pos.x *= gp->fbWidth / 2.0f;
        pos.y *= gp->efbHeight / 2.0f;
        sprintf(buf, "%d", wp->items[i].price);
        PSMTXTrans(m, 12.0f * (strlen(buf) - 1) + pos.x, pos.y, pos.z);
        iconNumberDispGx(m, wp->items[i].price, FALSE, shop_price_color);
    }
}

/** Updates the item description window (winMgrHelpMain). */
void help_main(WinMgrEntry* entry) {
    winMgrHelpMain(entry);
}

/** Draws the description of the shelf item Mario stands in front of in the help window. */
void help_disp(WinMgrEntry* entry) {
    ShopWork* wp = _wp;
    ShopItem* item = &wp->items[wp->cur];

    if (!winMgrAction(wp->win[1])) {
        entry->msg = msgSearch(itemDataTable[item->id].description);
        winMgrHelpDraw(entry);
    }
}

/**
 * Draws the shop points window: the "points" label icon (per language)
 * and a counter that rolls a tenth of the way toward the pouch's shop points each frame,
 * so gains count up visibly.
 */
void point_disp(WinMgrEntry* entry) {
    ShopWork* wp = _wp;
    PouchData* pouch = pouchGetPtr();
    s32 diff;
    Mtx m;
    Mtx s;

    if (entry->flags & WINMGR_FLAG_ANIMATING) {
        return;
    }
    PSMTXTrans(m, 10.0f + (30.0f + entry->x), entry->y - 36.0f, 0.0f);
#if VERSION >= VERSION_US
    PSMTXScale(s, (u32)gp->language != LANGUAGE_JP ? 0.8f : 1.1f, 1.1f, 1.1f);
    PSMTXConcat(m, s, m);
    ((void (*)(Mtx, s16, u16))iconDispGx2)(m, ICON_FLAG_2D, point_disp_icons[gp->language]);
#else
    if ((u32)gp->language == LANGUAGE_JP) {
        PSMTXScale(s, 1.1f, 1.1f, 1.1f);
        PSMTXConcat(m, s, m);
        iconDispGx2(m, ICON_FLAG_2D, ICON_1F1);
    } else {
        PSMTXScale(s, 0.8f, 1.1f, 1.1f);
        PSMTXConcat(m, s, m);
        iconDispGx2(m, ICON_FLAG_2D, ICON_20A);
    }
#endif
    PSMTXTrans(m, 124.0f + entry->x, entry->y - 38.0f, 0.0f);
    iconNumberDispGx(m, wp->points, FALSE, shop_point_num_color);
    diff = pouch->shopPoints - wp->points;
    if (diff) {
        diff /= 10;
        if (!diff) {
            diff = pouch->shopPoints > wp->points ? 1 : -1;
        }
        wp->points += diff;
    }
}

/** Draws the title of the shop point prize list window (msg_shop_point_list4), centred; retail squeezes it to fit. */
void title_disp(WinMgrEntry* entry) {
    const char* msg;
#if VERSION >= VERSION_US
    f32 w;
    Mtx m;
    Mtx s;
#endif

    if (entry->flags & WINMGR_FLAG_ANIMATING) {
        return;
    }
    msg = msgSearch("msg_shop_point_list4");
#if VERSION >= VERSION_US
    w = FontGetMessageWidth(msg);
#endif
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_title_color);
#if VERSION >= VERSION_US
    if (w > entry->width - 20) {
        PSMTXTrans(m, entry->x + 10, entry->y - 12, 0.0f);
        PSMTXScale(s, (entry->width - 20) / w, 1.0f, 1.0f);
    } else {
        PSMTXTrans(m, entry->x + (entry->width - w) / 2, entry->y - 12, 0.0f);
        PSMTXScale(s, 1.0f, 1.0f, 1.0f);
    }
    PSMTXConcat(m, s, m);
    FontDrawMessageMtx(m, msg);
#else
    FontDrawMessage(entry->x + (entry->width - FontGetMessageWidth(msg)) / 2, entry->y - 12, msg);
#endif
    FontDrawEdgeOff();
}

/**
 * Draws the shop point prize list: a column of point thresholds and the prize item names,
 * prizes already claimed in red.
 * This is the retail version, which scales long headings and names to fit for the translated releases.
 */
void list_disp(WinMgrEntry* entry) {
    PouchData* pouch;
    const char* msg;
#if VERSION >= VERSION_US
    s32 i;
    ItemData* item;
    f32 w;
#else
    u32 mask;
    s32 i;
    ItemData* item;
#endif
    char buf[256];
    Mtx m;
    Mtx s;

    if (entry->flags & WINMGR_FLAG_ANIMATING) {
        return;
    }
    pouch = pouchGetPtr();
    i = 0;
    msg = msgSearch("msg_shop_point_list5");
#if VERSION >= VERSION_US
    w = FontGetMessageWidth(msg);
#endif
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_list_head5_color);
#if VERSION >= VERSION_US
    if (w > 80.0f) {
        PSMTXTrans(m, 10.0f + (entry->x + 10), entry->y + 13, 0.0f);
        PSMTXScale(s, 80.0f / w, 1.0f, 1.0f);
    } else {
        PSMTXTrans(m, (entry->x + 10) + (100.0f - w) / 2, entry->y + 13, 0.0f);
        PSMTXScale(s, 1.0f, 1.0f, 1.0f);
    }
    PSMTXConcat(m, s, m);
    FontDrawMessageMtx(m, msg);
    FontDrawEdgeOff();
    msg = msgSearch("msg_shop_point_list6");
    w = FontGetMessageWidth(msg);
#else
    FontDrawMessage(entry->x + 10 + (100 - FontGetMessageWidth(msg)) / 2, entry->y + 13, msg);
    FontDrawEdgeOff();
    msg = msgSearch("msg_shop_point_list6");
#endif
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_list_head6_color);
#if VERSION >= VERSION_US
    if (w > 80.0f) {
        PSMTXTrans(m, 10.0f + (entry->x + entry->width / 2 - 10), entry->y + 13, 0.0f);
        PSMTXScale(s, 80.0f / w, 1.0f, 1.0f);
    } else {
        PSMTXTrans(m, (entry->x + entry->width / 2 - 10) + (100.0f - w) / 2, entry->y + 13, 0.0f);
        PSMTXScale(s, 1.0f, 1.0f, 1.0f);
    }
    PSMTXConcat(m, s, m);
    FontDrawMessageMtx(m, msg);
#else
    FontDrawMessage(entry->x + entry->width / 2 - 10 + (100 - FontGetMessageWidth(msg)) / 2, entry->y + 13, msg);
#endif
    FontDrawEdgeOff();
    FontDrawStart();
    for (; shopPointList[i].pts != -1; i++) {
        item = &itemDataTable[shopPointList[i].item];
        FontDrawStart();
#if VERSION >= VERSION_US
        if (pouch->shopPointRewardFlags & (1 << i)) {
#else
        if (pouch->shopPointRewardFlags & (mask = 1 << i)) {
#endif
            FontDrawColor(shop_list_pts_got_color);
        } else {
            FontDrawColor(shop_list_pts_color);
        }
        sprintf(buf, "%s", winZenkakuStr(shopPointList[i].pts));
        FontDrawMessage(entry->x + (entry->width / 2 - (s32)FontGetMessageWidth(buf)) - 90, entry->y - 20 - i * 26, buf);
#if VERSION >= VERSION_US
        msg = msgSearch("msg_shop_point_list5");
        w = 0.64f * FontGetMessageWidth(msg);
        FontDrawStart();
        if (pouch->shopPointRewardFlags & (1 << i)) {
#else
        FontDrawStart();
        if (pouch->shopPointRewardFlags & mask) {
#endif
            FontDrawColor(shop_list_unit_got_color);
        } else {
            FontDrawColor(shop_list_unit_color);
        }
#if VERSION >= VERSION_US
        if (w > 60.0f) {
            PSMTXTrans(m, entry->x + entry->width / 2 - 88, entry->y - 28 - i * 26, 0.0f);
            PSMTXScale(s, 0.64f * 60.0f / w, 0.64f, 0.64f);
        } else {
            PSMTXTrans(m, entry->x + entry->width / 2 - 88, entry->y - 28 - i * 26, 0.0f);
            PSMTXScale(s, 0.64f, 0.64f, 0.64f);
        }
        PSMTXConcat(m, s, m);
        FontDrawMessageMtx(m, msg);
        msg = msgSearch(item->name);
        FontDrawStart();
        if (pouch->shopPointRewardFlags & (1 << i)) {
#else
        sprintf(buf, "%s", msgSearch("msg_shop_point_list5"));
        PSMTXTrans(m, entry->x + (entry->width / 2 - 0.6f * FontGetMessageWidth(buf)) - 40.0f, entry->y - 28 - i * 26, 0.0f);
        PSMTXScale(s, 0.64f, 0.64f, 0.64f);
        PSMTXConcat(m, s, m);
        FontDrawMessageMtx(m, buf);
        msg = msgSearch(item->name);
        FontDrawStart();
        if (pouch->shopPointRewardFlags & mask) {
#endif
            FontDrawColor(shop_list_item_got_color);
        } else {
            FontDrawColor(shop_list_item_color);
        }
#if VERSION >= VERSION_US
        PSMTXTrans(m, entry->x + entry->width / 2 - 10, entry->y - 20 - i * 26, 0.0f);
        if (FontGetMessageWidth(msg) > entry->width / 2 - 10) {
            PSMTXScale(s, (f32)(entry->width / 2 - 10) / FontGetMessageWidth(msg), 1.0f, 1.0f);
        } else {
            PSMTXScale(s, 1.0f, 1.0f, 1.0f);
        }
        PSMTXConcat(m, s, m);
        FontDrawMessageMtx(m, msg);
#else
        FontDrawMessage(entry->x + entry->width / 2 - 10, entry->y - 20 - i * 26, msg);
#endif
    }
}

/**
 * Opens the three prize list windows (title, list, points), then blocks until B closes them;
 * plays the open and close sounds.
 */
USER_FUNC(disp_list) {
    if (isFirstCall) {
        event->userdata[0] = winMgrEntry(&list_desc[0]);
        event->userdata[1] = winMgrEntry(&list_desc[1]);
        event->userdata[2] = winMgrEntry(&list_desc[2]);
        winMgrOpen(event->userdata[0]);
        winMgrOpen(event->userdata[1]);
        winMgrOpen(event->userdata[2]);
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_OPEN1));
    }
    if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
        winMgrCloseAutoDelete(event->userdata[0]);
        winMgrCloseAutoDelete(event->userdata[1]);
        winMgrCloseAutoDelete(event->userdata[2]);
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_STATUS_WINDOW_CLOSE1));
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Sets up a shop on map load.
 * args: the six shelf objects (map object and hit names), their items and prices,
 * the shopkeeper data (NPC name, hook and after-buy scripts, message ids)
 * and an optional sell-price override table.
 * Puts an item icon over each shelf, hides one-off items Mario already has (Contact Lens, the Super Luigi
 * books), gives the shopkeeper its talk script and starts the shop driver.
 */
#if VERSION == VERSION_DEMO_JP
USER_FUNC(evt_shop_setup) {
    s32 i;
    ShopWork* wp;
    EvtWord* args = event->args;
    ShopObj* objs = (ShopObj*)evtGetValue(event, args[0]);
    ShopItem* items = (ShopItem*)evtGetValue(event, args[1]);
    ShopperData* shopper = (ShopperData*)evtGetValue(event, args[2]);
    ShopDb* db = (ShopDb*)evtGetValue(event, args[3]);
    Vec pos;
    char buf[32];

    wp = _mapAlloc(sizeof(ShopWork));
    memset(wp, 0, sizeof(ShopWork));
    _wp = wp;
    strncpy(mapname, gp->currentMapName, sizeof(gp->currentMapName) - 1);
    wp->objs = objs;
    wp->items = items;
    wp->shopper = shopper;
    wp->db = db;
    wp->count = 6;
    for (i = 0; i < wp->count; i++) {
        mapObjGetPos(wp->objs[i].mobj, &pos);
        mapObjFlagOn(wp->objs[i].mobj, MAP_OBJ_FLAG_DISABLED);
        sprintf(buf, "shp%d", i);
        iconEntry(buf, itemDataTable[wp->items[i].id].icon_id);
        iconSetPos(buf, pos.x, pos.y + 6.0f, pos.z);
        iconFlagOn(buf, ICON_FLAG_NO_MAP_BLEND);
        wp->itemFlags[i] = 0;
        if (wp->items[i].id == ITEM_CONTACT_LENS && pouchCheckItem(ITEM_CONTACT_LENS)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI1 && pouchCheckItem(ITEM_SUPER_LUIGI1)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI2 && pouchCheckItem(ITEM_SUPER_LUIGI2)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI3 && pouchCheckItem(ITEM_SUPER_LUIGI3)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI4 && pouchCheckItem(ITEM_SUPER_LUIGI4)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI5 && pouchCheckItem(ITEM_SUPER_LUIGI5)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
    }
    // FAKE: block-local NPC pointer; assigning through npcNameToPtr(shopper->name) directly changes the object
    {
        NpcEntry* npc = npcNameToPtr(shopper->name);
        npc->talkEvt = shopper_talk_evt;
    }
    wp->mainEvt = evtEntryType(main_evt, 0, EVENT_START_IMMEDIATE, 0);
    wp->mainEvt->lwData[0] = (EvtWord)wp;
    for (i = 0; i < 3; i++) {
        wp->win[i] = winMgrEntry(&window_desc[i]);
    }
    wp->sel = -1;
    evtRunCaseEntry(CASE_FRONT_A, TRUE, "a_shop_keihin", 0, evt_shoplist, 0);
    return EVT_RETURN_DONE;
}
#elif VERSION == VERSION_DEMO_US
USER_FUNC(evt_shop_setup) {
    s32 i;
    ShopWork* wp;
    EvtWord* args = event->args;
    ShopObj* objs = (ShopObj*)evtGetValue(event, args[0]);
    ShopItem* items = (ShopItem*)evtGetValue(event, args[1]);
    ShopperData* shopper = (ShopperData*)evtGetValue(event, args[2]);
    ShopDb* db = (ShopDb*)evtGetValue(event, args[3]);
    Vec pos;
    char buf[32];

    wp = _mapAlloc(sizeof(ShopWork));
    memset(wp, 0, sizeof(ShopWork));
    _wp = wp;
    strncpy(mapname, gp->currentMapName, sizeof(gp->currentMapName) - 1);
    wp->objs = objs;
    wp->items = items;
    wp->shopper = shopper;
    wp->db = db;
    wp->count = 6;
    for (i = 0; i < wp->count; i++) {
        mapObjGetPos(wp->objs[i].mobj, &pos);
        mapObjFlagOn(wp->objs[i].mobj, MAP_OBJ_FLAG_DISABLED);
        sprintf(buf, "shp%d", i);
        iconEntry(buf, itemDataTable[wp->items[i].id].icon_id);
        iconSetPos(buf, pos.x, pos.y + 6.0f, pos.z);
        iconFlagOn(buf, ICON_FLAG_NO_MAP_BLEND);
        wp->itemFlags[i] = 0;
        if (wp->items[i].id == ITEM_CONTACT_LENS && pouchCheckItem(ITEM_CONTACT_LENS)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI1 && pouchCheckItem(ITEM_SUPER_LUIGI1)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI2 && pouchCheckItem(ITEM_SUPER_LUIGI2)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI3 && pouchCheckItem(ITEM_SUPER_LUIGI3)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI4 && pouchCheckItem(ITEM_SUPER_LUIGI4)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI5 && pouchCheckItem(ITEM_SUPER_LUIGI5)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
    }
    {
        NpcEntry* npc = npcNameToPtr(shopper->name);
        npc->talkEvt = shopper_talk_evt;
    }
    wp->mainEvt = evtEntryType(main_evt, 0, EVENT_START_IMMEDIATE, 0);
    wp->mainEvt->lwData[0] = (EvtWord)wp;
    for (i = 0; i < 3; i++) {
        wp->win[i] = winMgrEntry(&window_desc[i]);
    }
    wp->sel = -1;
return EVT_RETURN_DONE;
}
#else
USER_FUNC(evt_shop_setup) {
    s32 i;
    ShopWork* wp;
    EvtWord* args = event->args;
    ShopObj* objs = (ShopObj*)evtGetValue(event, args[0]);
    ShopItem* items = (ShopItem*)evtGetValue(event, args[1]);
    ShopperData* shopper = (ShopperData*)evtGetValue(event, args[2]);
    ShopDb* db = (ShopDb*)evtGetValue(event, args[3]);
    Vec pos;
    char buf[32];

    wp = _mapAlloc(sizeof(ShopWork));
    memset(wp, 0, sizeof(ShopWork));
    _wp = wp;
    strncpy(mapname, gp->currentMapName, sizeof(gp->currentMapName) - 1);
    wp->objs = objs;
    wp->items = items;
    wp->shopper = shopper;
    wp->db = db;
    wp->count = 6;
    for (i = 0; i < wp->count; i++) {
        mapObjGetPos(wp->objs[i].mobj, &pos);
        mapObjFlagOn(wp->objs[i].mobj, MAP_OBJ_FLAG_DISABLED);
        sprintf(buf, "shp%d", i);
        iconEntry(buf, itemDataTable[wp->items[i].id].icon_id);
        iconSetPos(buf, pos.x, pos.y + 6.0f, pos.z);
        iconFlagOn(buf, ICON_FLAG_NO_MAP_BLEND);
        wp->itemFlags[i] = 0;
        if (wp->items[i].id == ITEM_CONTACT_LENS && pouchCheckItem(ITEM_CONTACT_LENS)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI1 && pouchCheckItem(ITEM_SUPER_LUIGI1)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI2 && pouchCheckItem(ITEM_SUPER_LUIGI2)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI3 && pouchCheckItem(ITEM_SUPER_LUIGI3)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI4 && pouchCheckItem(ITEM_SUPER_LUIGI4)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
        if (wp->items[i].id == ITEM_SUPER_LUIGI5 && pouchCheckItem(ITEM_SUPER_LUIGI5)) {
            wp->itemFlags[i] |= SHOP_ITEM_FLAG_SOLD_OUT;
        }
    }
    // FAKE: block-local NPC pointer; assigning through npcNameToPtr(shopper->name) directly changes the object
    {
        NpcEntry* npc = npcNameToPtr(shopper->name);
        npc->talkEvt = shopper_talk_evt;
    }
    wp->mainEvt = evtEntryType(main_evt, 0, EVENT_START_IMMEDIATE, 0);
    wp->mainEvt->lwData[0] = (EvtWord)wp;
    for (i = 0; i < 3; i++) {
        wp->win[i] = winMgrEntry(&window_desc[i]);
    }
    wp->sel = -1;
    evtRunCaseEntry(CASE_FRONT_A, TRUE, "a_shop_keihin", 0, evt_shoplist, 0);
    return EVT_RETURN_DONE;
}
#endif

/** Opens (args[0] nonzero) or closes the shop points window, starting its counter from the current points. */
USER_FUNC(point_disp_onoff) {
    s32 on = evtGetValue(event, event->args[0]);
    PouchData* pouch = pouchGetPtr();
    ShopWork* wp = _wp;
    wp->points = pouch->shopPoints;
    if (on) {
        winMgrOpen(wp->win[2]);
    } else {
        winMgrClose(wp->win[2]);
    }
    return EVT_RETURN_DONE;
}

/**
 * Blocks until the shop points window has counted up to Mario's points;
 * retail plays a tick sound each frame meanwhile.
 */
#if VERSION <= VERSION_DEMO_US
static USER_FUNC(point_wait) {
#else
USER_FUNC(point_wait) {
#endif
    ShopWork* wp = _wp;
    PouchData* pouch = pouchGetPtr();
#if VERSION <= VERSION_DEMO_US
return wp->points != SHOP_PTS(pouch) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
#else
    if (wp->points != pouch->shopPoints) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_EVT_GAME_MONTE_COUNT1));
        return EVT_RETURN_BLOCK;
#endif
    }
#if VERSION > VERSION_DEMO_US
    return EVT_RETURN_DONE;
}
#endif

/** Sets or clears ShopWork flag bits; args: on/off, mask. The talk script sets SHOP_FLAG_TALKING while the counter menu is up. */
USER_FUNC(shop_flag_onoff) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 mask = evtGetValue(event, args[1]);
    ShopWork* wp = _wp;
    if (on) {
        wp->flags |= mask;
    } else {
        wp->flags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

/** Writes Mario's shop points (pouch, 0-300) to args[0]. */
USER_FUNC(get_shop_point) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    evtSetValue(event, args[0], pouch->shopPoints);
    return EVT_RETURN_DONE;
}

/**
 * Adds args[0] shop points, doubled on mri_07, usu_01 or rsh_03 when that shop's sale flag is on
 * and the play-time values in args[1..3] are under 900000 (the sale's time limit); caps the total at 300.
 */
USER_FUNC(add_shop_point) {
    EvtWord* args = event->args;
    s32 add = evtGetValue(event, args[0]);
    s32 mriSaleMs = evtGetValue(event, args[1]);
    s32 usuSaleMs = evtGetValue(event, args[2]);
    s32 rshSaleMs = evtGetValue(event, args[3]);
    PouchData* pouch = pouchGetPtr();

    if (!strcmp(gp->currentMapName, "mri_07") && swGet(0xFA) && mriSaleMs < 900000) { // GSWF_SHOP_MRI_07_DOUBLE_POINTS_SALE
        add *= 2;
    }
    if (!strcmp(gp->currentMapName, "usu_01") && swGet(0xFB) && usuSaleMs < 900000) { // GSWF_SHOP_USU_01_DOUBLE_POINTS_SALE
        add *= 2;
    }
    if (!strncmp(gp->currentMapName, "rsh_03", 6) && swGet(0xFC) && rshSaleMs < 900000) { // GSWF_SHOP_RSH_03_DOUBLE_POINTS_SALE
        add *= 2;
    }
    pouch->shopPoints += add;
    if (pouch->shopPoints > 300) {
        pouch->shopPoints = 300;
    }
    return EVT_RETURN_DONE;
}

/** Clears Mario's shop points and the prizes claimed, after the 300-point prize. */
USER_FUNC(reset_shop_point) {
    PouchData* pouch = pouchGetPtr();
    pouch->shopPoints = 0;
    pouch->shopPointRewardFlags = 0;
    return EVT_RETURN_DONE;
}

/**
 * Finds the next prize Mario has earned but not claimed:
 * writes 0, its item id and its name to args[0..2], or -1 to all three when there is none.
 * At 300 points it always offers the 300-point prize.
 */
USER_FUNC(chk_shop_point) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    s32 i = 0;
    s32 j;
    ShopPoint* p;
    ShopPoint* q;
    s32 v;

    for (p = shopPointList; (v = p->pts) != -1; i++, p++) {
        if (v <= pouch->shopPoints && !(pouch->shopPointRewardFlags & (1 << i))) {
            if (pouch->shopPoints != 300) {
                evtSetValue(event, args[0], 0);
                evtSetValue(event, args[1], p->item);
                evtSetValue(event, args[2], (EvtWord)msgSearch(itemDataTable[p->item].name));
            } else {
                for (j = 0; shopPointList[j].pts != 300; j++) {
                }
                evtSetValue(event, args[0], 0);
                evtSetValue(event, args[1], shopPointList[j].item);
                evtSetValue(event, args[2], (EvtWord)msgSearch(itemDataTable[shopPointList[j].item].name));
            }
            return EVT_RETURN_DONE;
        }
    }
    evtSetValue(event, args[0], -1);
    evtSetValue(event, args[1], -1);
    evtSetValue(event, args[2], -1);
    return EVT_RETURN_DONE;
}

/** Marks the lowest earned but unclaimed shop point prize as claimed. */
USER_FUNC(exchange_shop_point) {
    PouchData* pouch = pouchGetPtr();
    ShopPoint* p;
    s32 i = 0;
    s32 v;

    for (p = shopPointList; (v = p->pts) != -1; i++, p++) {
        if (v <= pouch->shopPoints && !(pouch->shopPointRewardFlags & (1 << i))) {
            pouch->shopPointRewardFlags |= (1 << i);
            return EVT_RETURN_DONE;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Looks up the prize for the point threshold in args[0] and writes its item id and name to args[1]
 * and args[2] (-1 when there is no such threshold).
 */
USER_FUNC(shop_point_item) {
    EvtWord* args = event->args;
    s32 pts = evtGetValue(event, args[0]);
    ShopPoint* p;
    s32 v;

    pouchGetPtr();
    for (p = shopPointList; (v = p->pts) != -1; p++) {
        if (v == pts) {
            evtSetValue(event, args[1], p->item);
            evtSetValue(event, args[2], (EvtWord)msgSearch(itemDataTable[p->item].name));
            return EVT_RETURN_DONE;
        }
    }
    evtSetValue(event, args[1], -1);
    evtSetValue(event, args[2], -1);
    return EVT_RETURN_DONE;
}

/** Writes the shopkeeper NPC's name to args[0]. */
USER_FUNC(shopper_name) {
    evtSetValue(event, event->args[0], (EvtWord)_wp->shopper->name);
    return EVT_RETURN_DONE;
}

static USER_FUNC(disp_off) {
    ShopWork* wp = (ShopWork*)evtGetValue(event, event->args[0]);
    wp->flags &= ~1;
    return EVT_RETURN_DONE;
}

/** For the shelf item Mario picked (ShopWork in args[0]): writes its name, item id and price to args[1..3]. */
static USER_FUNC(get_value) {
    EvtWord* args = event->args;
    ShopWork* wp = (ShopWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (EvtWord)msgSearch(itemDataTable[wp->items[wp->cur].id].name));
    evtSetValue(event, args[2], wp->items[wp->cur].id);
    evtSetValue(event, args[3], wp->items[wp->cur].price);
    return EVT_RETURN_DONE;
}

/** Writes the shop's message id number args[1] (from ShopperData.msgs) to args[2]; args[0] is the ShopWork. */
USER_FUNC(get_msg) {
    EvtWord* args = event->args;
    ShopWork* wp = (ShopWork*)evtGetValue(event, args[0]);
    s32 i = evtGetValue(event, args[1]);
    evtSetValue(event, args[2], (EvtWord)wp->shopper->msgs[i]);
    return EVT_RETURN_DONE;
}

/** Writes the shop's after-buy script (ShopperData.buy_evt, may be NULL) to args[0]. */
USER_FUNC(get_buy_evt) {
    evtSetValue(event, event->args[0], (EvtWord)_wp->shopper->buy_evt);
    return EVT_RETURN_DONE;
}

/**
 * Records the item id just bought (args[0]) so the shop driver can hide one-off items once the purchase script
 * ends.
 */
USER_FUNC(set_buy_item_id) {
    _wp->buyItem = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/** Writes the shop's ShopWork pointer to args[0]. */
static USER_FUNC(get_ptr) {
    evtSetValue(event, event->args[0], (EvtWord)_wp);
    return EVT_RETURN_DONE;
}

/** Writes the shop's talk hook script (ShopperData.fook_evt, may be NULL) to args[0]. */
USER_FUNC(get_fook_evt) {
    evtSetValue(event, event->args[0], (EvtWord)_wp->shopper->fook_evt);
    return EVT_RETURN_DONE;
}

/** Writes the name and sell price of item args[0] to args[1] and args[2]. */
USER_FUNC(name_price) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (EvtWord)msgSearch(itemDataTable[id].name));
    evtSetValue(event, args[2], itemDataTable[id].sell_price);
    return EVT_RETURN_DONE;
}

/** Sets LW(0) to the number of items Mario carries, to check he has something to sell. */
USER_FUNC(sell_pouchcheck_func) {
    pouchGetPtr();
    event->lwData[0] = pouchGetHaveItemCnt();
    return EVT_RETURN_DONE;
}

/**
 * Applies the shop's sell-price overrides (ShopDb) to the item table, saving the original prices,
 * around the sell menu.
 */
USER_FUNC(item_data_db_arrange) {
    ShopDb* db = _wp->db;
    if (db) {
        for (; db->id; db++) {
            db->save = itemDataTable[db->id].sell_price;
            itemDataTable[db->id].sell_price = db->price;
        }
    }
    return EVT_RETURN_DONE;
}

/** Puts back the sell prices item_data_db_arrange replaced. */
USER_FUNC(item_data_db_restore) {
    ShopDb* db = _wp->db;
    if (db) {
        for (; db->id; db++) {
            itemDataTable[db->id].sell_price = db->save;
        }
    }
    return EVT_RETURN_DONE;
}

/** Sets LW(0) to the number of items Mario carries and LW(1) to the free stored-item slots. */
USER_FUNC(keep_pouchcheck_func) {
    pouchGetPtr();
    event->lwData[0] = pouchGetHaveItemCnt();
    event->lwData[1] = pouchGetEmptyKeepItemCnt();
    return EVT_RETURN_DONE;
}

/** Sets LW(0) to the number of stored items and LW(1) to the free slots in Mario's item pouch. */
USER_FUNC(unkeep_pouchcheck_func) {
    pouchGetPtr();
    event->lwData[0] = pouchGetKeepItemCnt();
    event->lwData[1] = pouchGetEmptyHaveItemCnt();
    return EVT_RETURN_DONE;
}

/**
 * Shop driver, every frame: when Mario stands on a shelf's hit object,
 * opens the name and description windows for that item; A starts buy_evt.
 * Waits while a partner ability, the Ghost Diary or (retail) a field item pickup is active,
 * and on rsh_03 on day 3 until the shopkeeper is woken.
 * Hides the icons of one-off items once bought and of shelves whose map object is hidden.
 */
#if VERSION <= VERSION_DEMO_US
static USER_FUNC(evt_shop_main_func) {
#else
USER_FUNC(evt_shop_main_func) {
#endif
    ShopWork* wp;
    MarioWork* mario;
    PartyEntry* party;
    BOOL found;
    HitEntry* hit;
    u32 trg;
    s32 i;
    MapObject* mobj;
    char buf[32];

    found = FALSE;
    wp = (ShopWork*)event->lwData[0];
    mario = marioGetPtr();
    party = partyGetPtr(0);
    hit = mario->hitobjStandOn;
    wp->flags &= ~1;
    trg = keyGetButtonTrg(0);
#if VERSION <= VERSION_DEMO_US
    if ((marioChkCtrl() ? FALSE : TRUE) || (marioChkKey() ? FALSE : TRUE)) {
#else
    // FAKE: the masks and BOOL casts do nothing (!x is 0 or 1); !marioChkCtrl() || !marioChkKey() changes the object
    if ((BOOL)(!marioChkCtrl() & 0x7ffffff) || (BOOL)(!marioChkKey() & 0x7ffffff)) {
#endif
        trg = 0;
    }
    if (!strncmp(gp->currentMapName, "rsh_03", strlen("rsh_03")) &&
#if VERSION <= VERSION_DEMO_US
        evtGetValue(event, GSW(GSW_STORY_PROGRESS)) >= STG6_45_DAY_3_STARTS && evtGetValue(event, GSW(GSW_STORY_PROGRESS)) < STG6_48_SMORG_MIASMA &&
        !evtGetValue(event, GSWF(GSWF_RSH03_SHOP_CLERK_WOKEN))) {
#else
        evtGetValue(event, GSW_STORY_PROGRESS) >= STG6_45_DAY_3_STARTS && evtGetValue(event, GSW_STORY_PROGRESS) < STG6_48_SMORG_MIASMA &&
        !evtGetValue(event, GSWF_RSH_03_SHOP_CLERK_REVIVED)) {
#endif
        return EVT_RETURN_BLOCK;
    }
    if (party && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        if (wp->sel != -1) {
            wp->sel = -1;
            winMgrClose(wp->win[0]);
            winMgrClose(wp->win[1]);
        }
        return EVT_RETURN_BLOCK;
    }
#if VERSION <= VERSION_DEMO_US
if (winGhostDiaryChk()) {
#else
    if (winGhostDiaryChk() || itemPickUpFromFieldCheck()) {
#endif
        if (wp->sel != -1) {
            wp->sel = -1;
            winMgrClose(wp->win[0]);
            winMgrClose(wp->win[1]);
        }
        return EVT_RETURN_BLOCK;
    }
    if (!(wp->flags & 2)) {
        if (hit) {
            for (i = 0; i < wp->count; i++) {
                if (!strcmp(wp->objs[i].hit, hitGetName(hit))) {
                    break;
                }
            }
#if VERSION > VERSION_DEMO_US
            // itemFlags[i] is read before the i < wp->count test; a full six-item shop with no matching hit reads one past itemFlags.
#endif
            if (!(wp->itemFlags[i] & 1) && i < wp->count) {
                if (wp->sel != i) {
                    if (wp->sel == -1) {
                        winMgrOpen(wp->win[0]);
                        winMgrOpen(wp->win[1]);
                    }
                    wp->sel = i;
                    wp->cur = i;
                    winMgrHelpInit(winMgrGetPtr(wp->win[1]));
                }
                found = TRUE;
            }
        }
        if (!found && wp->sel != -1) {
            wp->sel = -1;
            winMgrClose(wp->win[0]);
            winMgrClose(wp->win[1]);
        }
        if (wp->sel != -1) {
            wp->flags |= 1;
            if (trg & PAD_BUTTON_A) {
                marioKeyOff();
                wp->buyEvt = evtEntry(buy_evt, 0, 0);
                wp->buyEvt->lwData[0] = (EvtWord)wp;
                wp->flags |= 2;
                winMgrClose(wp->win[0]);
                winMgrClose(wp->win[1]);
                wp->sel = -1;
                wp->buyItem = 0;
            }
        }
    } else if (!evtCheckID(wp->buyEvt->eventId)) {
        wp->flags &= ~2;
        marioKeyOn();
        if (wp->buyItem == ITEM_CONTACT_LENS) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == ITEM_SUPER_LUIGI1) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == ITEM_SUPER_LUIGI2) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == ITEM_SUPER_LUIGI3) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == ITEM_SUPER_LUIGI4) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == ITEM_SUPER_LUIGI5) {
            wp->itemFlags[wp->cur] |= 1;
        }
    }
    for (i = 0; i < wp->count; i++) {
        mobj = mapGetMapObj(wp->objs[i].mobj);
        if (mobj) {
            sprintf(buf, "shp%d", i);
            if (wp->itemFlags[i] & 1) {
                iconFlagOn(buf, ICON_FLAG_HIDDEN);
            } else if (mobj->flags & MAP_OBJ_FLAG_HIDDEN) {
                iconFlagOn(buf, ICON_FLAG_HIDDEN);
            } else {
                iconFlagOff(buf, ICON_FLAG_HIDDEN);
            }
        }
    }
    return EVT_RETURN_BLOCK;
}

/** TRUE while the shop set up on the current map has its item display up or a purchase running (any ShopWork flag). */
BOOL evtShopIsActive(void) {
    if (!strcmp(gp->currentMapName, mapname) && _wp->flags) {
        return TRUE;
    }
    return FALSE;
}

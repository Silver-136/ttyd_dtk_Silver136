#include "version.h"
#include "evt/evt_shop.h"
#include "evt/evt_cmd.h"
#include "evt/evt_case.h"
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
#ifdef VERSION_G8ME01
USER_FUNC(evt_msg_fmt_int);
USER_FUNC(evt_msg_fmt_str);
#endif

#define SHOP_PTS(p) ((p)->mShopPoints)

extern ItemData itemDataTable[];
void FontDrawString(f32 x, f32 y, const char* str);
void mapObjGetPos(const char* name, Vec* pos);

//.sbss
static ShopWork* _wp;
char mapname[16];

void list_disp(WinMgrEntry* entry);
void title_disp(WinMgrEntry* entry);
void point_disp(WinMgrEntry* entry);
void help_disp(WinMgrEntry* entry);
void help_main(WinMgrEntry* entry);
void name_disp(WinMgrEntry* entry);

USER_FUNC(N_evt_pouch_remove_item_index);
USER_FUNC(add_shop_point);
USER_FUNC(chk_shop_point);
USER_FUNC(disp_list);
USER_FUNC(disp_off);
USER_FUNC(evt_mario_set_dir_npc);
USER_FUNC(evt_msg_print_add_insert);
USER_FUNC(evt_msg_print_insert);
USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_add_item);
USER_FUNC(evt_pouch_add_keepitem);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_pouch_remove_keepitem);
USER_FUNC(evt_shop_main_func);
USER_FUNC(evt_sub_get_language);
USER_FUNC(evt_sub_get_mapname);
USER_FUNC(evt_win_coin_off);
USER_FUNC(evt_win_coin_on);
USER_FUNC(evt_win_coin_wait);
USER_FUNC(evt_win_item_select);
USER_FUNC(exchange_shop_point);
USER_FUNC(get_buy_evt);
USER_FUNC(get_fook_evt);
USER_FUNC(get_msg);
USER_FUNC(get_ptr);
USER_FUNC(get_shop_point);
USER_FUNC(get_value);
USER_FUNC(item_data_db_arrange);
USER_FUNC(item_data_db_restore);
USER_FUNC(keep_pouchcheck_func);
USER_FUNC(name_price);
USER_FUNC(point_disp_onoff);
USER_FUNC(point_wait);
USER_FUNC(reset_shop_point);
USER_FUNC(sell_pouchcheck_func);
USER_FUNC(set_buy_item_id);
USER_FUNC(shop_flag_onoff);
USER_FUNC(shop_point_item);
USER_FUNC(shopper_name);
USER_FUNC(unkeep_pouchcheck_func);

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
EvtWord shopper_talk_evt[VERSION_PICK(352, 396)];
EvtWord first_shopping_evt[86];
static EvtWord sell_evt[VERSION_PICK(196, 207)];
EvtWord keep_evt[VERSION_PICK(148, 167)];
EvtWord unkeep_evt[147];
EvtWord point_evt[VERSION_PICK(15, 35)];
EvtWord evt_shoplist[10];
static EvtWord main_evt[4];

static WinMgrDesc window_desc[] = {
    {0, 0, 8, -150, 120, 300, 50, {0x2B, 0xA4, 0xFF, 0xFF}, NULL, name_disp},
    {0, 0, 8, -240, -130, 500, 80, {0xF0, 0xF0, 0xF0, 0xFF}, help_main, help_disp},
    {2, 0, 8, -250, -130, 150, 45, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, point_disp},
};

ShopPoint shopPointList[] = {
    10, 148, 20, 131, 30, 149, 50, 129,
    70, 130, 100, 126, 150, 151, 200, 150,
    250, 156, 300, 127, -1, -1,
};

WinMgrDesc list_desc[] = {
    {0, 0, 8, -280, 160, 210, 80, {0x2B, 0xA4, 0xFF, 0xFF}, NULL, title_disp},
    {0, 3, 8, -60, 160, 340, 300, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, list_disp},
    {2, 0, 8, -250, -130, 150, 45, {0xF0, 0xF0, 0xF0, 0xFF}, NULL, point_disp},
};

#ifdef VERSION_G8ME01
static EVT_BEGIN(buy_evt)
    CALL(evt_mario_normalize)
    SET_VALUE(LW(10), LW(0))
    CALL(disp_off, LW(10))
    IF_INT_EQUAL(GSWF_SHOP_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_SHOP_TUTORIAL_SEEN, 1)
        CALL_CHILD(PTR(first_shopping_evt))
        RETURN
    END_IF
    CALL(shopper_name, LW(9))
    CALL(evt_set_dir_to_target, LW(9), STRING("mario"))
    CALL(evt_win_coin_on, 0, LW(8))
    CALL(point_disp_onoff, 1)
    CALL(get_value, LW(10), LW(11), LW(12), LW(13))
    CALL(get_msg, LW(10), 0, LW(14))
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(11), LW(13))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), LW(14), LW(13))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(11))
        CALL(evt_msg_print, 1, LW(14), 0, LW(9))
    END_SWITCH
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
    CALL(set_buy_item_id, LW(12))
    CALL(evt_sub_load_progresstime, GSW(5), LW(0))
    CALL(evt_sub_load_progresstime, GSW(14), LW(1))
    CALL(evt_sub_load_progresstime, GSW(23), LW(2))
    CALL(add_shop_point, 1, LW(0), LW(1), LW(2))
    MULTIPLY(LW(13), -1)
    CALL(evt_pouch_add_coin, LW(13))
    CALL(evt_win_coin_wait, LW(8))
    CALL(point_wait)
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(8))
    CALL(point_disp_onoff, 0)
    CALL(get_msg, LW(10), 11, LW(14))
    CALL(evt_msg_print_add, 0, LW(14))
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_CLEAR(LW(0), 4096)
        IF_INT_EQUAL(GSWF_ITEM_TUTORIAL_SEEN, 0)
            SET_VALUE(GSWF_ITEM_TUTORIAL_SEEN, 1)
            CALL(evt_msg_print, 0, STRING("item_first_hit"), 0, 0)
        END_IF
    END_IF
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
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print_insert, 0, LW(14), 0, LW(9), LW(2))
                CASE_DEFAULT
                CALL(evt_msg_fmt_str, 0, LW(14), LW(14), LW(2))
                CALL(evt_msg_print, 1, LW(14), 0, LW(9))
                CASE_END
            END_SWITCH
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
        CALL(evt_pouch_add_item, LW(1), LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(get_msg, LW(10), 27, LW(14))
            CALL(evt_msg_print, 0, LW(14), 0, LW(9))
            IF_INT_EQUAL(LF(0), 1)
                SET_VALUE(GSWF(1), 1)
            END_IF
            GOTO(123)
            RETURN
        END_IF
        CALL(exchange_shop_point)
    END_LOOP
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
    CALL(evt_mario_normalize)
    SET_VALUE(LW(10), LW(0))
    CALL(disp_off, LW(10))
    IF_INT_EQUAL(GSWF_SHOP_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_SHOP_TUTORIAL_SEEN, 1)
        CALL_CHILD(PTR(first_shopping_evt))
        RETURN
    END_IF
    CALL(shopper_name, LW(9))
    CALL(evt_set_dir_to_target, LW(9), STRING("mario"))
    CALL(evt_win_coin_on, 0, LW(8))
    CALL(point_disp_onoff, 1)
    CALL(get_value, LW(10), LW(11), LW(12), LW(13))
    CALL(get_msg, LW(10), 0, LW(14))
    CALL(evt_sub_get_language, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
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
    CALL(set_buy_item_id, LW(12))
    CALL(evt_sub_load_progresstime, GSW(5), LW(0))
    CALL(evt_sub_load_progresstime, GSW(14), LW(1))
    CALL(evt_sub_load_progresstime, GSW(23), LW(2))
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
    IF_BITS_CLEAR(LW(0), 4096)
        IF_INT_EQUAL(GSWF_ITEM_TUTORIAL_SEEN, 0)
            SET_VALUE(GSWF_ITEM_TUTORIAL_SEEN, 1)
            CALL(evt_msg_print, 0, STRING("item_first_hit"), 0, 0)
        END_IF
    END_IF
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
        CALL(evt_pouch_add_item, LW(1), LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(get_msg, LW(10), 27, LW(14))
            CALL(evt_msg_print, 0, LW(14), 0, LW(9))
            IF_INT_EQUAL(LF(0), 1)
                SET_VALUE(GSWF(1), 1)
            END_IF
            GOTO(123)
            RETURN
        END_IF
        CALL(exchange_shop_point)
    END_LOOP
    LABEL(123)
    CALL(get_buy_evt, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        SET_VALUE(LW(0), LW(12))
        CALL_CHILD(LW(1))
    END_IF
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
EVT_BEGIN(shopper_talk_evt)
    CALL(evt_mario_key_onoff, 0)
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
    IF_INT_EQUAL(GSWF_SHOP_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_SHOP_TUTORIAL_SEEN, 1)
        CALL_CHILD(PTR(first_shopping_evt))
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SET_VALUE(LF(0), 0)
    LOOP(0)
        CALL(chk_shop_point, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(GSWF(1), 1)
            SET_VALUE(LW(0), 0)
            CALL(shop_point_item, 300, LW(1), LW(2))
        END_IF
        IF_INT_EQUAL(LW(0), -1)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_mario_key_onoff, 1)
                RETURN
            END_IF
            SET_VALUE(LF(11), 0)
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("mri_07"))
                IF_INT_EQUAL(GSWF(250), 1)
                    CALL(evt_sub_load_progresstime, GSW(5), LW(0))
                    IF_INT_LESS(LW(0), 900000)
                        SET_VALUE(LW(14), STRING("stg2_shop_04_1"))
                        SET_VALUE(LF(11), 1)
                    END_IF
                END_IF
            END_IF
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("usu_01"))
                IF_INT_EQUAL(GSWF(251), 1)
                    CALL(evt_sub_load_progresstime, GSW(14), LW(0))
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
                IF_INT_EQUAL(GSWF(252), 1)
                    CALL(evt_sub_load_progresstime, GSW(23), LW(0))
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
                CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
                CALL(evt_msg_print_add, 0, LW(14))
            END_IF
            BREAK_LOOP
        END_IF
        CALL(evt_pouch_add_item, LW(1), LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(get_msg, LW(10), 29, LW(14))
            IF_INT_EQUAL(LF(10), 0)
                CALL(evt_msg_print, 0, LW(14), 0, 0)
                ELSE
                SET_VALUE(LF(10), 0)
                CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
                CALL(evt_msg_print_add, 0, LW(14))
            END_IF
            BREAK_LOOP
        END_IF
        CALL(get_msg, LW(10), 28, LW(14))
        IF_INT_EQUAL(LF(10), 0)
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print_insert, 0, LW(14), 0, 0, LW(2))
                CASE_DEFAULT
                CALL(evt_msg_fmt_str, 0, LW(14), LW(14), LW(2))
                CALL(evt_msg_print, 1, LW(14), 0, 0)
            END_SWITCH
            ELSE
            SET_VALUE(LF(10), 0)
            CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
            CALL(evt_sub_get_language, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print_insert, 0, LW(14), 0, 0, LW(2))
                CASE_DEFAULT
                CALL(evt_msg_fmt_str, 0, LW(14), LW(14), LW(2))
                CALL(evt_msg_print, 1, LW(14), 0, 0)
            END_SWITCH
        END_IF
        IF_INT_EQUAL(GSWF(1), 0)
            CALL(exchange_shop_point)
        END_IF
        SET_VALUE(LF(0), 1)
        IF_INT_EQUAL(GSWF(1), 1)
            SET_VALUE(GSWF(1), 0)
        END_IF
    END_LOOP
    CALL(shop_flag_onoff, 1, 16384)
    CALL(get_msg, LW(10), 23, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    CALL(shop_flag_onoff, 0, 16384)
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
#else
EVT_BEGIN(shopper_talk_evt)
    CALL(evt_mario_key_onoff, 0)
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
    IF_INT_EQUAL(GSWF_SHOP_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_SHOP_TUTORIAL_SEEN, 1)
        CALL_CHILD(PTR(first_shopping_evt))
        CALL(evt_mario_key_onoff, 1)
        RETURN
    END_IF
    SET_VALUE(LF(0), 0)
    LOOP(0)
        CALL(chk_shop_point, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(GSWF(1), 1)
            SET_VALUE(LW(0), 0)
            CALL(shop_point_item, 300, LW(1), LW(2))
        END_IF
        IF_INT_EQUAL(LW(0), -1)
            IF_INT_EQUAL(LF(0), 1)
                CALL(evt_mario_key_onoff, 1)
                RETURN
            END_IF
            SET_VALUE(LF(11), 0)
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("mri_07"))
                IF_INT_EQUAL(GSWF(250), 1)
                    CALL(evt_sub_load_progresstime, GSW(5), LW(0))
                    IF_INT_LESS(LW(0), 900000)
                        SET_VALUE(LW(14), STRING("stg2_shop_04_1"))
                        SET_VALUE(LF(11), 1)
                    END_IF
                END_IF
            END_IF
            CALL(evt_sub_get_mapname, LW(0))
            IF_STR_EQUAL(LW(0), STRING("usu_01"))
                IF_INT_EQUAL(GSWF(251), 1)
                    CALL(evt_sub_load_progresstime, GSW(14), LW(0))
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
                IF_INT_EQUAL(GSWF(252), 1)
                    CALL(evt_sub_load_progresstime, GSW(23), LW(0))
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
                CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
                CALL(evt_msg_print_add, 0, LW(14))
            END_IF
            BREAK_LOOP
        END_IF
        CALL(evt_pouch_add_item, LW(1), LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(get_msg, LW(10), 29, LW(14))
            IF_INT_EQUAL(LF(10), 0)
                CALL(evt_msg_print, 0, LW(14), 0, 0)
                ELSE
                SET_VALUE(LF(10), 0)
                CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
                CALL(evt_msg_print_add, 0, LW(14))
            END_IF
            BREAK_LOOP
        END_IF
        CALL(get_msg, LW(10), 28, LW(14))
        IF_INT_EQUAL(LF(10), 0)
            CALL(evt_msg_print_insert, 0, LW(14), 0, 0, LW(2))
            ELSE
            SET_VALUE(LF(10), 0)
            CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
            CALL(evt_msg_print_add_insert, 0, LW(14), LW(2))
        END_IF
        IF_INT_EQUAL(GSWF(1), 0)
            CALL(exchange_shop_point)
        END_IF
        SET_VALUE(LF(0), 1)
        IF_INT_EQUAL(GSWF(1), 1)
            SET_VALUE(GSWF(1), 0)
        END_IF
    END_LOOP
    CALL(shop_flag_onoff, 1, 16384)
    CALL(get_msg, LW(10), 23, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    CALL(shop_flag_onoff, 0, 16384)
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
#endif
EVT_BEGIN(first_shopping_evt)
    CALL(shopper_name, LW(9))
    CALL(evt_set_dir_to_target, LW(9), STRING("mario"))
    CALL(evt_mario_set_dir_npc, LW(9))
    CALL(get_msg, LW(10), 30, LW(14))
    IF_INT_EQUAL(LF(10), 0)
        CALL(evt_msg_print, 0, LW(14), 0, LW(9))
        ELSE
        SET_VALUE(LF(10), 0)
        CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
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

#ifdef VERSION_G8ME01
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
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_insert, 0, LW(14), 0, STRING("me"), LW(2), LW(3))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), LW(14), LW(3))
        CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
        CALL(evt_msg_print, 1, LW(14), 0, STRING("me"))
    END_SWITCH
    CALL(get_msg, LW(10), 24, LW(14))
    CALL(evt_msg_select, 0, LW(14))
    IF_INT_EQUAL(LW(0), 1)
        CALL(get_msg, LW(10), 9, LW(14))
        CALL(evt_msg_print_add, 0, LW(14))
        GOTO(0)
    END_IF
    CALL(N_evt_pouch_remove_item_index, LW(1), LW(4), LW(0))
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
    IF_INT_NOT_EQUAL(LW(0), 0)
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
    CALL(N_evt_pouch_remove_item_index, LW(1), LW(4), LW(0))
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

#ifdef VERSION_G8ME01
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
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_msg_print_add_insert, 0, LW(14), LW(1))
        CASE_DEFAULT
        CALL(evt_msg_fmt_int, 0, LW(14), LW(14), LW(1))
        CALL(evt_msg_print_add, 1, LW(14))
    END_SWITCH
    LABEL(0)
    CALL(evt_win_item_select, 1, 4, LW(1), LW(4))
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
    CALL(N_evt_pouch_remove_item_index, LW(1), LW(4), LW(0))
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
    CALL(evt_msg_print_add_insert, 0, LW(14), LW(1))
    LABEL(0)
    CALL(evt_win_item_select, 1, 4, LW(1), LW(4))
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
    CALL(N_evt_pouch_remove_item_index, LW(1), LW(4), LW(0))
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
    CALL(evt_win_item_select, 1, 5, LW(1), LW(4))
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(get_msg, LW(10), 22, LW(14))
        CALL(evt_msg_print, 0, LW(14), 0, STRING("me"))
        RETURN
    END_IF
    CALL(evt_pouch_remove_keepitem, LW(1), LW(4), LW(0))
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

#ifdef VERSION_G8ME01
EVT_BEGIN(point_evt)
    CALL(get_shop_point, LW(1))
    CALL(get_msg, LW(10), 25, LW(14))
    CALL(evt_sub_get_language, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
            CALL(evt_msg_print_add_insert, 0, LW(14), LW(1))
        CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), LW(14), LW(1))
            CALL(evt_msg_print_add, 1, LW(14))
    END_SWITCH
    RETURN
    OPCODE_END_SCRIPT,
    0 // US: the object is 0x8C bytes (trailing zero word)
};
#else
EVT_BEGIN(point_evt)
    CALL(get_shop_point, LW(0))
    CALL(get_msg, LW(10), 25, LW(14))
    CALL(evt_msg_print_add_insert, 0, LW(14), LW(0))
    RETURN
EVT_END()
#endif

#ifdef VERSION_G8ME01
// US: point window icon per language (first object of evt_shop .sdata)
u16 point_disp_icons[] = {0x226, 0x227, 0x228, 0x227, 0x229, 0x22A};
#endif

EVT_BEGIN(evt_shoplist)
    CALL(evt_mario_key_onoff, 0)
    CALL(disp_list)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

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
        if (wp->itemFlags[i] & 1) {
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

void help_main(WinMgrEntry* entry) {
    winMgrHelpMain(entry);
}

void help_disp(WinMgrEntry* entry) {
    ShopWork* wp = _wp;
    s32 off = wp->cur * (s32)sizeof(ShopItem);
    ShopItem* items = wp->items;

    if (!winMgrAction(wp->win[1])) {
        entry->msg = msgSearch(itemDataTable[((ShopItem*)((u8*)items + off))->id].description);
        winMgrHelpDraw(entry);
    }
}

#ifdef VERSION_G8ME01
// US: "points" label icon per language (lives in another unit's .sdata)
extern u16 point_disp_icons[];
#endif

void point_disp(WinMgrEntry* entry) {
    ShopWork* wp = _wp;
    PouchData* pouch = pouchGetPtr();
    s32 diff;
    Mtx m;
    Mtx s;

    if (entry->flags & 4) {
        return;
    }
    PSMTXTrans(m, 10.0f + (30.0f + entry->x), entry->y - 36.0f, 0.0f);
#ifdef VERSION_G8ME01
    PSMTXScale(s, gp->language != 0U ? 0.8f : 1.1f, 1.1f, 1.1f);
    PSMTXConcat(m, s, m);
    ((void (*)(Mtx, s16, u16))iconDispGx2)(m, 0x10, point_disp_icons[gp->language]);
#else
    if (gp->language == 0U) {
        PSMTXScale(s, 1.1f, 1.1f, 1.1f);
        PSMTXConcat(m, s, m);
        iconDispGx2(m, 0x10, 0x1F1);
    }
    else {
        PSMTXScale(s, 0.8f, 1.1f, 1.1f);
        PSMTXConcat(m, s, m);
        iconDispGx2(m, 0x10, 0x20A);
    }
#endif
    PSMTXTrans(m, 124.0f + entry->x, entry->y - 38.0f, 0.0f);
    iconNumberDispGx(m, wp->points, FALSE, shop_point_num_color);
    diff = SHOP_PTS(pouch) - wp->points;
    if (diff) {
        diff /= 10;
        if (!diff) {
            diff = SHOP_PTS(pouch) > wp->points ? 1 : -1;
        }
        wp->points += diff;
    }
}

void title_disp(WinMgrEntry* entry) {
    const char* msg;
#ifdef VERSION_G8ME01
    f32 w;
    Mtx m;
    Mtx s;
#endif

    if (entry->flags & 4) {
        return;
    }
#ifdef VERSION_G8ME01
    msg = msgSearch("msg_shop_point_list4");
    w = FontGetMessageWidth(msg);
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_title_color);
    if (w > entry->width - 20) {
        PSMTXTrans(m, entry->x + 10, entry->y - 12, 0.0f);
        PSMTXScale(s, (entry->width - 20) / w, 1.0f, 1.0f);
    } else {
        PSMTXTrans(m, entry->x + (entry->width - w) / 2, entry->y - 12, 0.0f);
        PSMTXScale(s, 1.0f, 1.0f, 1.0f);
    }
    PSMTXConcat(m, s, m);
    FontDrawMessageMtx(m, msg);
    FontDrawEdgeOff();
#else
    msg = msgSearch("msg_shop_point_list4");
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_title_color);
    FontDrawMessage(entry->x + (entry->width - FontGetMessageWidth(msg)) / 2, entry->y - 12, msg);
    FontDrawEdgeOff();
#endif
}

#ifdef VERSION_G8ME01
void list_disp(WinMgrEntry* entry) {
    PouchData* pouch;
    const char* msg;
    s32 i;
    ItemData* item;
    f32 w;
    char buf[256];
    Mtx m;
    Mtx s;

    if (entry->flags & 4) {
        return;
    }
    pouch = pouchGetPtr();
    i = 0;
    msg = msgSearch("msg_shop_point_list5");
    w = FontGetMessageWidth(msg);
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_list_head5_color);
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
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_list_head6_color);
    if (w > 80.0f) {
        PSMTXTrans(m, 10.0f + (entry->x + entry->width / 2 - 10), entry->y + 13, 0.0f);
        PSMTXScale(s, 80.0f / w, 1.0f, 1.0f);
    } else {
        PSMTXTrans(m, (entry->x + entry->width / 2 - 10) + (100.0f - w) / 2, entry->y + 13, 0.0f);
        PSMTXScale(s, 1.0f, 1.0f, 1.0f);
    }
    PSMTXConcat(m, s, m);
    FontDrawMessageMtx(m, msg);
    FontDrawEdgeOff();
    FontDrawStart();
    for (; shopPointList[i].pts != -1; i++) {
        item = &itemDataTable[shopPointList[i].item];
        FontDrawStart();
        if (pouch->shopPointRewardFlags & (1 << i)) {
            FontDrawColor(shop_list_pts_got_color);
        }
        else {
            FontDrawColor(shop_list_pts_color);
        }
        sprintf(buf, "%s", winZenkakuStr(shopPointList[i].pts));
        FontDrawMessage(entry->x + (entry->width / 2 - (s32)FontGetMessageWidth(buf)) - 90, entry->y - 20 - i * 26, buf);
        msg = msgSearch("msg_shop_point_list5");
        w = 0.64f * FontGetMessageWidth(msg);
        FontDrawStart();
        if (pouch->shopPointRewardFlags & (1 << i)) {
            FontDrawColor(shop_list_unit_got_color);
        }
        else {
            FontDrawColor(shop_list_unit_color);
        }
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
            FontDrawColor(shop_list_item_got_color);
        }
        else {
            FontDrawColor(shop_list_item_color);
        }
        PSMTXTrans(m, entry->x + entry->width / 2 - 10, entry->y - 20 - i * 26, 0.0f);
        if (FontGetMessageWidth(msg) > entry->width / 2 - 10) {
            PSMTXScale(s, (f32)(entry->width / 2 - 10) / FontGetMessageWidth(msg), 1.0f, 1.0f);
        } else {
            PSMTXScale(s, 1.0f, 1.0f, 1.0f);
        }
        PSMTXConcat(m, s, m);
        FontDrawMessageMtx(m, msg);
    }
}
#else
void list_disp(WinMgrEntry* entry) {
    PouchData* pouch;
    const char* msg;
    u32 mask;
    s32 i;
    ItemData* item;
    char buf[256];
    Mtx m;
    Mtx s;

    if (entry->flags & 4) {
        return;
    }
    pouch = pouchGetPtr();
    i = 0;
    msg = msgSearch("msg_shop_point_list5");
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_list_head5_color);
    FontDrawMessage(entry->x + 10 + (100 - FontGetMessageWidth(msg)) / 2, entry->y + 13, msg);
    FontDrawEdgeOff();
    msg = msgSearch("msg_shop_point_list6");
    FontDrawStart();
    FontDrawEdge();
    FontDrawColor(shop_list_head6_color);
    FontDrawMessage(entry->x + entry->width / 2 - 10 + (100 - FontGetMessageWidth(msg)) / 2, entry->y + 13, msg);
    FontDrawEdgeOff();
    FontDrawStart();
    for (; shopPointList[i].pts != -1; i++) {
        item = &itemDataTable[shopPointList[i].item];
        FontDrawStart();
        if (pouch->shopPointRewardFlags & (mask = 1 << i)) {
            FontDrawColor(shop_list_pts_got_color);
        }
        else {
            FontDrawColor(shop_list_pts_color);
        }
        sprintf(buf, "%s", winZenkakuStr(shopPointList[i].pts));
        FontDrawMessage(entry->x + (entry->width / 2 - (s32)FontGetMessageWidth(buf)) - 90, entry->y - 20 - i * 26, buf);
        FontDrawStart();
        if (pouch->shopPointRewardFlags & mask) {
            FontDrawColor(shop_list_unit_got_color);
        }
        else {
            FontDrawColor(shop_list_unit_color);
        }
        sprintf(buf, "%s", msgSearch("msg_shop_point_list5"));
        PSMTXTrans(m, entry->x + (entry->width / 2 - 0.6f * FontGetMessageWidth(buf)) - 40.0f, entry->y - 28 - i * 26, 0.0f);
        PSMTXScale(s, 0.64f, 0.64f, 0.64f);
        PSMTXConcat(m, s, m);
        FontDrawMessageMtx(m, buf);
        msg = msgSearch(item->name);
        FontDrawStart();
        if (pouch->shopPointRewardFlags & mask) {
            FontDrawColor(shop_list_item_got_color);
        }
        else {
            FontDrawColor(shop_list_item_color);
        }
        FontDrawMessage(entry->x + entry->width / 2 - 10, entry->y - 20 - i * 26, msg);
    }
}
#endif

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
        mapObjFlagOn(wp->objs[i].mobj, 0x4000);
        sprintf(buf, "shp%d", i);
        ((void (*)(const char*, u16))iconEntry)(buf, itemDataTable[wp->items[i].id].icon_id);
        iconSetPos(buf, pos.x, pos.y + 6.0f, pos.z);
        iconFlagOn(buf, 0x40);
        wp->itemFlags[i] = 0;
        if (wp->items[i].id == 0x33 && pouchCheckItem(ITEM_CONTACT_LENS)) {
            wp->itemFlags[i] |= 1;
        }
        if (wp->items[i].id == 0x37 && pouchCheckItem(ITEM_SUPER_LUIGI1)) {
            wp->itemFlags[i] |= 1;
        }
        if (wp->items[i].id == 0x38 && pouchCheckItem(ITEM_SUPER_LUIGI2)) {
            wp->itemFlags[i] |= 1;
        }
        if (wp->items[i].id == 0x39 && pouchCheckItem(ITEM_SUPER_LUIGI3)) {
            wp->itemFlags[i] |= 1;
        }
        if (wp->items[i].id == 0x3A && pouchCheckItem(ITEM_SUPER_LUIGI4)) {
            wp->itemFlags[i] |= 1;
        }
        if (wp->items[i].id == 0x3B && pouchCheckItem(ITEM_SUPER_LUIGI5)) {
            wp->itemFlags[i] |= 1;
        }
    }
    { NpcEntry* npc = npcNameToPtr(shopper->name); npc->talkEvt = shopper_talk_evt; }
    wp->mainEvt = evtEntryType(main_evt, 0, 0x20, 0);
    wp->mainEvt->lwData[0] = (EvtWord)wp;
    for (i = 0; i < 3; i++) {
        wp->win[i] = winMgrEntry(&window_desc[i]);
    }
    wp->sel = -1;
    evtRunCaseEntry(9, TRUE, "a_shop_keihin", 0, evt_shoplist, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(point_disp_onoff) {
    s32 on = evtGetValue(event, event->args[0]);
    PouchData* pouch = pouchGetPtr();
    ShopWork* wp = _wp;
    wp->points = SHOP_PTS(pouch);
    if (on) {
        winMgrOpen(wp->win[2]);
    }
    else {
        winMgrClose(wp->win[2]);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(point_wait) {
    ShopWork* wp = _wp;
    PouchData* pouch = pouchGetPtr();
    if (wp->points != SHOP_PTS(pouch)) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_EVT_GAME_MONTE_COUNT1));
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(shop_flag_onoff) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 mask = evtGetValue(event, args[1]);
    ShopWork* wp = _wp;
    if (on) {
        wp->flags |= mask;
    }
    else {
        wp->flags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(get_shop_point) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    evtSetValue(event, args[0], SHOP_PTS(pouch));
    return EVT_RETURN_DONE;
}

USER_FUNC(add_shop_point) {
    EvtWord* args = event->args;
    s32 add = evtGetValue(event, args[0]);
    s32 t1 = evtGetValue(event, args[1]);
    s32 t2 = evtGetValue(event, args[2]);
    s32 t3 = evtGetValue(event, args[3]);
    PouchData* pouch = pouchGetPtr();

    if (!strcmp(gp->currentMapName, "mri_07") && swGet(0xFA) && t1 < 900000) {
        add *= 2;
    }
    if (!strcmp(gp->currentMapName, "usu_01") && swGet(0xFB) && t2 < 900000) {
        add *= 2;
    }
    if (!strncmp(gp->currentMapName, "rsh_03", 6) && swGet(0xFC) && t3 < 900000) {
        add *= 2;
    }
    SHOP_PTS(pouch) += add;
    if (SHOP_PTS(pouch) > 300) {
        SHOP_PTS(pouch) = 300;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(reset_shop_point) {
    PouchData* pouch = pouchGetPtr();
    SHOP_PTS(pouch) = 0;
    pouch->shopPointRewardFlags = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(chk_shop_point) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    s32 i = 0;
    s32 j;
    ShopPoint* p;
    ShopPoint* q;
    s32 v;

    for (p = shopPointList; (v = p->pts) != -1; i++, p++) {
        if (v <= SHOP_PTS(pouch) && !(pouch->shopPointRewardFlags & (1 << i))) {
            if (SHOP_PTS(pouch) != 300) {
                evtSetValue(event, args[0], 0);
                evtSetValue(event, args[1], p->item);
                evtSetValue(event, args[2], (EvtWord)msgSearch(itemDataTable[p->item].name));
            }
            else {
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

USER_FUNC(exchange_shop_point) {
    PouchData* pouch = pouchGetPtr();
    ShopPoint* p;
    s32 i = 0;
    s32 v;

    for (p = shopPointList; (v = p->pts) != -1; i++, p++) {
        if (v <= SHOP_PTS(pouch) && !(pouch->shopPointRewardFlags & (1 << i))) {
            pouch->shopPointRewardFlags |= (1 << i);
            return EVT_RETURN_DONE;
        }
    }
    return EVT_RETURN_DONE;
}

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

USER_FUNC(shopper_name) {
    evtSetValue(event, event->args[0], (EvtWord)_wp->shopper->name);
    return EVT_RETURN_DONE;
}

static USER_FUNC(disp_off) {
    ShopWork* wp = (ShopWork*)evtGetValue(event, event->args[0]);
    wp->flags &= ~1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(get_value) {
    EvtWord* args = event->args;
    ShopWork* wp = (ShopWork*)evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (EvtWord)msgSearch(itemDataTable[wp->items[wp->cur].id].name));
    evtSetValue(event, args[2], wp->items[wp->cur].id);
    evtSetValue(event, args[3], wp->items[wp->cur].price);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_msg) {
    EvtWord* args = event->args;
    ShopWork* wp = (ShopWork*)evtGetValue(event, args[0]);
    s32 i = evtGetValue(event, args[1]);
    evtSetValue(event, args[2], (EvtWord)wp->shopper->msgs[i]);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_buy_evt) {
    evtSetValue(event, event->args[0], (EvtWord)_wp->shopper->buy_evt);
    return EVT_RETURN_DONE;
}

USER_FUNC(set_buy_item_id) {
    _wp->buyItem = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(get_ptr) {
    evtSetValue(event, event->args[0], (EvtWord)_wp);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_fook_evt) {
    evtSetValue(event, event->args[0], (EvtWord)_wp->shopper->fook_evt);
    return EVT_RETURN_DONE;
}

USER_FUNC(name_price) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (EvtWord)msgSearch(itemDataTable[id].name));
    evtSetValue(event, args[2], itemDataTable[id].sell_price);
    return EVT_RETURN_DONE;
}

USER_FUNC(sell_pouchcheck_func) {
    pouchGetPtr();
    event->lwData[0] = pouchGetHaveItemCnt();
    return EVT_RETURN_DONE;
}

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

USER_FUNC(item_data_db_restore) {
    ShopDb* db = _wp->db;
    if (db) {
        for (; db->id; db++) {
            itemDataTable[db->id].sell_price = db->save;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(keep_pouchcheck_func) {
    pouchGetPtr();
    event->lwData[0] = pouchGetHaveItemCnt();
    event->lwData[1] = pouchGetEmptyKeepItemCnt();
    return EVT_RETURN_DONE;
}

USER_FUNC(unkeep_pouchcheck_func) {
    pouchGetPtr();
    event->lwData[0] = pouchGetKeepItemCnt();
    event->lwData[1] = pouchGetEmptyHaveItemCnt();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_shop_main_func) {
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
    if ((BOOL)(!marioChkCtrl() & 0x7ffffff) || (BOOL)(!marioChkKey() & 0x7ffffff)) {
        trg = 0;
    }
    if (!strncmp(gp->currentMapName, "rsh_03", strlen("rsh_03")) &&
        evtGetValue(event, GSW_STORY_PROGRESS) >= 0x148 && evtGetValue(event, GSW_STORY_PROGRESS) < 0x14C &&
        !evtGetValue(event, GSWF(3442))) {
        return EVT_RETURN_BLOCK;
    }
    if (party && (party->flags & 0x100)) {
        if (wp->sel != -1) {
            wp->sel = -1;
            winMgrClose(wp->win[0]);
            winMgrClose(wp->win[1]);
        }
        return EVT_RETURN_BLOCK;
    }
    if (winGhostDiaryChk() || N_itemPickUpFromFieldCheck()) {
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
            if (trg & 0x100) {
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
    }
    else if (!evtCheckID(wp->buyEvt->eventId)) {
        wp->flags &= ~2;
        marioKeyOn();
        if (wp->buyItem == 0x33) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == 0x37) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == 0x38) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == 0x39) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == 0x3A) {
            wp->itemFlags[wp->cur] |= 1;
        }
        if (wp->buyItem == 0x3B) {
            wp->itemFlags[wp->cur] |= 1;
        }
    }
    for (i = 0; i < wp->count; i++) {
        mobj = mapGetMapObj(wp->objs[i].mobj);
        if (mobj) {
            sprintf(buf, "shp%d", i);
            if (wp->itemFlags[i] & 1) {
                iconFlagOn(buf, 2);
            }
            else if (mobj->flags & 1) {
                iconFlagOn(buf, 2);
            }
            else {
                iconFlagOff(buf, 2);
            }
        }
    }
    return EVT_RETURN_BLOCK;
}

BOOL evtShopIsActive(void) {
    if (!strcmp(gp->currentMapName, mapname) && _wp->flags) {
        return TRUE;
    }
    return FALSE;
}

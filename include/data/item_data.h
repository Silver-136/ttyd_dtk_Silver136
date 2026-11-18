// item_data.c: the item table itemDataTable (ItemData: names, prices, icon, HP/FP restored, battle weapon).

#pragma once

#include <dolphin/types.h>
#include "battle/battle_database_common.h"

#define ITEM_USABLE_MENU 4 // usable_locations: usable from the pause menu (win_item)

typedef struct ItemData {
    const char* id; // 0x0, japanese name of the item
    const char* name; // 0x4, msg tag of the item name (msgSearch)
    const char* description; // 0x8, msg tag of the help text (battle menus, item pickup window)
    const char* menu_description; // 0xC, msg tag of the pause-menu text (win_badge, win_log)
    u16 usable_locations; // 0x10, bit flags: 1 leaves the pouch when used in battle, 2 listed in the battle item menu, 4 usable from the pause item menu
    s16 type_sort_order; // 0x12, type-order sort key (pouch, badge and journal lists); -1 = not in the journal
    u16 buy_price; // 0x14, coins in shops
    u16 discount_price; // 0x16, shop price shown for badges (evt_win; other items use buy_price)
    u16 star_piece_price; // 0x18, Star Piece price (starmaniac_table)
    u16 sell_price; // 0x1A, coins when sold (also Refund's base)
    s8 bp_cost; // 0x1C, badge BP cost
    u8 hp_restored; // 0x1D, HP an item restores
    u8 fp_restored; // 0x1E, FP an item restores
    u8 sp_restored; // 0x1F, Star Power an item restores
    u16 icon_id; // 0x20, IconType drawn for the item
    s16 pad_22; // 0x22, always 0 (alignment before battle_params)
    BattleWeapon* battle_params; // 0x24, BattleWeapon used when the item is used in battle
} ItemData;

extern ItemData itemDataTable[];

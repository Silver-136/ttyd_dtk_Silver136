#pragma once

#include <dolphin/types.h>
#include "battle/battle_database_common.h"

typedef struct ItemData {
    const char* id; //0x0, japanese name of the item
    const char* name; //0x4
    const char* description; //0x8
    const char* menu_description; //0xC
    u16 usable_locations; //0x10, bit flags (1 battle, 2 field?, 4 menu)
    s16 type_sort_order; //0x12
    u16 buy_price; //0x14
    u16 discount_price; //0x16
    u16 star_piece_price; //0x18
    u16 sell_price; //0x1A
    s8 bp_cost; //0x1C
    u8 hp_restored; //0x1D
    u8 fp_restored; //0x1E
    u8 sp_restored; //0x1F
    u16 icon_id; //0x20
    s16 pad_22; //0x22, always 0 (alignment before battle_params)
    BattleWeapon* battle_params; //0x24
} ItemData;
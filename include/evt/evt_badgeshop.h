#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"

typedef struct BadgeShopWork {
    u8 special_table[0x19]; //0x0, Lovely Howz of Badges
    u8 bargain_table[0x19]; //0x19, Lovely Howz of Badges Discount
    u8 starmaniac_table[0x19]; //0x32, Dazzle's Badges
    u8 bottakuru_table[0x55]; //0x4B, Charlieton's Items
    u8 bteresa_table[0x55]; //0xA0, Pianta Parlor
    u8 align[3];
    s32 bargainBadges[5]; //0xF8
    s32 bargainNum; //0x10C
    s32 reserved_110; //0x110, cleared and added to bargainNum in the restock loop, never set nonzero
    u16 restockFlags; //0x114, bit0: regenerate bargain/bottakuru stock on next entry to gor_01
    u16 pad_116; //0x116, never accessed
    u32 pad_118; //0x118, never accessed
    u32 pad_11C; //0x11C, never accessed
    u32 pad_120; //0x120, never accessed
} BadgeShopWork;

s32 getBadgeBottakuru100TableMaxCount(void); //Charlieton Pit of 100 Trials
s32 getBadgeStarmaniacTableMaxCount(void); //Dazzle
s32 getBadgeBteresaTableMaxCount(void); //Pianta Parlor
void badgeShop_init(void);
void badgeShop_bottakuruGeneration(void);
void badgeShop_bteresaGeneration(void);
void badgeShop_bargainGeneration(void);
s16 badgeShop_set(u8* table, s16 id, s16 val);
s32 badgeShop_get(u8* table, s16 id);
s32 badgeShop_add(u8* table, s16 id, s32 val);
USER_FUNC(badgeShop_getBargainTable);
USER_FUNC(evt_badgeShop_throw_inc);
USER_FUNC(evt_badgeShop_throw_dec);
USER_FUNC(evt_badgeShop_special_dec);
USER_FUNC(evt_badgeShop_starmaniac_dec);
USER_FUNC(evt_badgeShop_bottakuru_dec);
USER_FUNC(evt_badgeShop_throw_get_kind_cnt);
USER_FUNC(evt_badgeShop_starmaniac_get_kind_cnt);
USER_FUNC(evt_badgeShop_bottakuru_get_kind_cnt);
USER_FUNC(U_evt_badgeShop_get_special_zaiko);
USER_FUNC(evt_badgeShop_bteresa_get_kind_cnt);
s32 badgeShop_ThrowCheck(s32 itemId);
s32 badgeShop_SpecialCheck(void);

extern s32 badge_bottakuru100_table[];
extern s32 badge_bottakuru_table[];
extern BadgeShopWork* bdsw;

#include "event/evt_badgeshop.h"

s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void badgeShop_add(void* shop, s16 item, s32 count);
extern void* bdsw;
extern s32 badge_bottakuru_table[];

s32 U_badgeShop_SpecialCheck(void* unused, void* shop) {
    s32 count;
    shop = bdsw;
    count = 0;
    if (*(s32*)((s32)shop + 0xF8) != 0) count++;
    if (*(s32*)((s32)shop + 0xFC) != 0) count++;
    if (*(s32*)((s32)shop + 0x100) != 0) count++;
    if (*(s32*)((s32)shop + 0x104) != 0) count++;
    if (*(s32*)((s32)shop + 0x108) != 0) count++;
    return count;
}

s32 badgeShop_ThrowCheck(s32 value) {
    void* shop = bdsw;
    s32 i;
    s32 count = 0;
    for (i = 0; i < *(s32*)((s32)shop + 0x10C); i++) {
        if (value == *(s32*)((s32)shop + ((i + *(s32*)((s32)shop + 0x10C)) * 4) + 0xF8)) {
            count++;
        }
    }
    return count;
}

s32 bottakuruComp(void* a, void* b) {
    s32 lhs = *(s32*)((s32)a + 4);
    s32 rhs = *(s32*)((s32)b + 4);
    if (lhs < rhs) {
        return -1;
    }
    return lhs > rhs;
}

USER_FUNC(evt_badgeShop_starmaniac_dec) {
    badgeShop_add((void*)((s32)bdsw + 0x32), (s16)evtGetValue(event, event->args[0]), -1);
    return 2;
}

USER_FUNC(evt_badgeShop_special_dec) {
    s32 item = evtGetValue(event, event->args[0]);
    void* shop = bdsw;
    badgeShop_add(shop, (s16)item, -1);
    return 2;
}

USER_FUNC(badgeShop_getBargainTable) {
    evtSetValue(event, event->args[0], (s32)bdsw + 0xF8);
    return 2;
}

s32 getBadgeBteresaTableMaxCount(void) {
    return 0x14;
}

s32 getBadgeStarmaniacTableMaxCount(void) {
    return 0xF;
}

s32 getBadgeBottakuru100TableMaxCount(void) {
    return 6;
}


void badgeShop_bargainGeneration(void) {
}


void badgeShop_bottakuruGeneration(void) {
}


u8 badgeShop_bteresaGeneration(void) {
    return 0;
}


void badgeShop_init(void) {
}


s32 evt_badgeShop_starmaniac_get_kind_cnt(int param_1, s32 param_2, s32 param_3, u32 param_4, u32 param_5) {
    return 0;
}


s32 evt_badgeShop_throw_get_kind_cnt(int param_1, s32 param_2, s32 param_3, u32 param_4, u32 param_5) {
    return 0;
}


void badgeShop_add(void* shop, s16 item, s32 count) {
}


s32 U_evt_badgeShop_get_special_zaiko(int param_1, s32 param_2, s32 param_3, u32 param_4, u32 param_5) {
    return 0;
}


s32 evt_badgeShop_bteresa_get_kind_cnt(int param_1, s32 param_2, s32 param_3, u32 param_4, u32 param_5) {
    return 0;
}


s32 badgeShop_set(int param_1, short param_2, s32 param_3, s32 param_4, u8 param_5, int param_6) {
    return 0;
}


int badgeShop_get(int param_1, short param_2) {
    return 0;
}


u8 evt_badgeShop_bottakuru_dec(void) {
    return 0;
}


s32 evt_badgeShop_bottakuru_get_kind_cnt(int param_1) {
    s32* table = badge_bottakuru_table;
    s32* args;
    s32 count = 0;
    s32 i = 0;

    args = *(s32**)((s32)param_1 + 0x18);
    for (; i < 4; i++) {
        if (table[evtGetValue((EventEntry*)param_1, GSW(i + 0x76))] != 0) {
            count++;
        }
    }

    evtSetValue((EventEntry*)param_1, args[0], count);
    return 2;
}

u8 evt_badgeShop_throw_dec(void) {
    return 0;
}


u8 evt_badgeShop_throw_inc(void) {
    return 0;
}

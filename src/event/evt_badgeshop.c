#include "event/evt_badgeshop.h"

s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
s32 badgeShop_add(void* shop, s16 item, s32 count);
s32 badgeShop_get(void* shop, s16 item);
s32 badgeShop_set(void* shop, s16 item, s32 count);
extern void* bdsw;
extern u8 badgeshopwork[];
extern s32 badge_special_table[];
extern s32 badge_starmaniac_table[];
extern s32 badge_bottakuru_table[];
extern s32 badge_bteresa_table[];

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
    ;
}


void badgeShop_bottakuruGeneration(void) {
    typedef struct BottakuruEntry {
        s32 item;
        s32 sort;
    } BottakuruEntry;
    extern s32 irand(s32 max);
    extern void qqsort(void* base, s32 count, s32 size, void* compare);
    BottakuruEntry entries[17] = {
        { 0, 200 }, { 0, 200 }, { 0, 200 }, { 0, 200 },
        { 0, 200 }, { 0, 200 }, { 0, 200 }, { 0, 200 },
        { 0, 200 }, { 0, 200 }, { 0, 200 }, { 0, 200 },
        { 0, 200 }, { 0, 200 }, { 0, 200 }, { 0, 200 },
        { 0, 200 }
    };
    s32 item;
    s32 count = 0;
    s32 stock;
    s32 i;
    s32 j;
    s32* table;

    for (item = 0; item < 0x152 && count < 0x10; item++) {
        stock = badgeShop_get((void*)((s32)bdsw + 0x4B), (s16)(item + 1));
        for (j = 0; j < stock; j++) {
            entries[count].sort = irand(100);
            entries[count].item = item + 1;
            count++;
            if (count >= 0x10) {
                break;
            }
        }
    }

    qqsort(entries, 0x11, 8, bottakuruComp);

    table = badge_bottakuru_table;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 0x11; j++) {
            if (entries[i].item == table[j]) {
                evtSetValue(NULL, GSW(i + 0x76), j);
            }
        }
    }
}

u8 badgeShop_bteresaGeneration(void) {
    extern s32 pouchCheckItem(s32 item);
    extern s32 badge_bteresa_table_card_special[];
    extern s32 badge_bteresa_table_card_silver[];
    extern s32 badge_bteresa_table_card_gold[];
    extern s32 badge_bteresa_table_card_platinum[];
    s32* table;

    if (pouchCheckItem(0x5B) != 0 && evtGetValue(NULL, GSW(0x76)) == 0) {
        evtSetValue(NULL, GSW(0x76), 1);
        table = badge_bteresa_table_card_special;
        while (*table != 0) {
            badgeShop_add((void*)((s32)bdsw + 0xA0), (s16)*table, 1);
            table++;
        }
    }

    if (pouchCheckItem(0x5E) != 0 && evtGetValue(NULL, GSW(0x77)) == 0) {
        evtSetValue(NULL, GSW(0x77), 1);
        table = badge_bteresa_table_card_silver;
        while (*table != 0) {
            badgeShop_add((void*)((s32)bdsw + 0xA0), (s16)*table, 1);
            table++;
        }
    }

    if (pouchCheckItem(0x5D) != 0 && evtGetValue(NULL, GSW(0x78)) == 0) {
        evtSetValue(NULL, GSW(0x78), 1);
        table = badge_bteresa_table_card_gold;
        while (*table != 0) {
            badgeShop_add((void*)((s32)bdsw + 0xA0), (s16)*table, 1);
            table++;
        }
    }

    if (pouchCheckItem(0x5C) != 0 && evtGetValue(NULL, GSW(0x79)) == 0) {
        evtSetValue(NULL, GSW(0x79), 1);
        table = badge_bteresa_table_card_platinum;
        while (*table != 0) {
            badgeShop_add((void*)((s32)bdsw + 0xA0), (s16)*table, 1);
            table++;
        }
    }
}

void badgeShop_init(void) {
    s32* table;

    bdsw = badgeshopwork;
    memset(bdsw, 0, 0x19);
    memset((void*)((s32)bdsw + 0x19), 0, 0x19);
    memset((void*)((s32)bdsw + 0x32), 0, 0x19);
    memset((void*)((s32)bdsw + 0x4B), 0, 0x55);
    memset((void*)((s32)bdsw + 0xA0), 0, 0x55);
    *(s16*)((s32)bdsw + 0x114) = 1;

    table = badge_special_table;
    while (*table != 0) {
        badgeShop_add(bdsw, (s16)*table, 1);
        table++;
    }

    table = badge_starmaniac_table;
    while (*table != 0) {
        badgeShop_add((void*)((s32)bdsw + 0x32), (s16)*table, 1);
        table++;
    }

    table = badge_bottakuru_table;
    while (*table != 0) {
        badgeShop_add((void*)((s32)bdsw + 0x4B), (s16)*table, 1);
        table++;
    }

    badgeShop_bottakuruGeneration();

    table = badge_bteresa_table;
    while (*table != 0) {
        badgeShop_add((void*)((s32)bdsw + 0xA0), (s16)*table, 1);
        table++;
    }
}


s32 evt_badgeShop_starmaniac_get_kind_cnt(EventEntry* event) {
    s32* args = event->args;
    s32 count = 0;
    s32 i;

    for (i = 0; i < 0x63; i++) {
        if (badgeShop_get((void*)((s32)bdsw + 0x32), (s16)(i + 0xF0)) > 0) {
            count++;
        }
    }

    evtSetValue(event, args[0], count);
    return 2;
}


s32 evt_badgeShop_throw_get_kind_cnt(EventEntry* event) {
    s32* args = event->args;
    s32 count = 0;
    s32 i;

    for (i = 0; i < 0x63; i++) {
        if (badgeShop_get((void*)((s32)bdsw + 0x19), (s16)(i + 0xF0)) > 0) {
            count++;
        }
    }

    evtSetValue(event, args[0], count);
    return 2;
}


s32 badgeShop_add(void* shop, s16 item, s32 count) {
    s32 base;
    s32 index;
    s32 mask;
    s32 shift;
    s32 value;
    s32 byteIndex;
    u8 old;

    if (shop == (void*)((s32)bdsw + 0x4B) || shop == (void*)((s32)bdsw + 0xA0)) {
        base = 1;
    } else {
        base = 0xF0;
    }

    if (item < base || item >= 0x153) {
        return 0;
    }

    index = (s16)(item - base);
    switch (index & 3) {
        case 0:
            mask = 3;
            shift = 0;
            break;
        case 1:
            mask = 0xC;
            shift = 2;
            break;
        case 2:
            mask = 0x30;
            shift = 4;
            break;
        case 3:
            mask = 0xC0;
            shift = 6;
            break;
    }

    byteIndex = index / 4;
    mask = (u8)mask;
    old = *(u8*)((s32)shop + byteIndex);
    value = (s16)((old & mask) >> shift);
    value += count;
    value = (s16)value;
    if (value < 0) {
        value = 0;
    } else if (value > 3) {
        value = 3;
    }

    *(u8*)((s32)shop + byteIndex) = (old & ~mask) + (u8)((s16)value << shift);
    return value;
}


s32 U_evt_badgeShop_get_special_zaiko(EventEntry* event) {
    s32* args = event->args;
    s32 count = 0;
    s32 i;

    for (i = 0; i < 0x152; i++) {
        if (badgeShop_get((void*)((s32)bdsw + 0x4B), (s16)(i + 1)) != 0) {
            count++;
        }
    }

    evtSetValue(event, args[0], count);
    return 2;
}


s32 evt_badgeShop_bteresa_get_kind_cnt(EventEntry* event) {
    s32* args = event->args;
    s32 count = 0;
    s32 i;

    for (i = 0; i < 0x152; i++) {
        if (badgeShop_get((void*)((s32)bdsw + 0xA0), (s16)(i + 1)) > 0) {
            count++;
        }
    }

    evtSetValue(event, args[0], count);
    return 2;
}


s32 badgeShop_set(void* shop, s16 item, s32 count) {
    s32 base;
    s32 index;
    s32 mask;
    s32 shift;
    s32 byteIndex;
    u8 old;

    if (shop == (void*)((s32)bdsw + 0x4B) || shop == (void*)((s32)bdsw + 0xA0)) {
        base = 1;
    } else {
        base = 0xF0;
    }

    if (item < base || item >= 0x153) {
        return 0;
    }

    index = (s16)(item - base);
    switch (index & 3) {
        case 0:
            mask = 0xFC;
            shift = 0;
            break;
        case 1:
            mask = 0xF3;
            shift = 2;
            break;
        case 2:
            mask = 0xCF;
            shift = 4;
            break;
        case 3:
            mask = 0x3F;
            shift = 6;
            break;
    }

    byteIndex = index / 4;
    if ((s16)count < 0) {
        count = 0;
    } else if ((s16)count > 3) {
        count = 3;
    }

    old = *(u8*)((s32)shop + byteIndex);
    old = mask & old;
    *(u8*)((s32)shop + byteIndex) = (u8)old + (u8)((s16)count << shift);
    return count;
}

s32 badgeShop_get(void* shop, s16 item) {
    s32 base;
    s32 index;
    s32 mask;
    s32 shift;

    if (shop == (void*)((s32)bdsw + 0x4B) || shop == (void*)((s32)bdsw + 0xA0)) {
        base = 1;
    } else {
        base = 0xF0;
    }

    if (item < base || item >= 0x153) {
        return 0;
    }

    index = (s16)(item - base);
    switch (index & 3) {
        case 0:
            mask = 3;
            shift = 0;
            break;
        case 1:
            mask = 0xC;
            shift = 2;
            break;
        case 2:
            mask = 0x30;
            shift = 4;
            break;
        case 3:
            mask = 0xC0;
            shift = 6;
            break;
    }

    return (s16)((*(u8*)((s32)shop + index / 4) & (u8)mask) >> shift);
}


USER_FUNC(evt_badgeShop_bottakuru_dec) {
    s32 item = evtGetValue(event, event->args[0]);
    s32* table = badge_bottakuru_table;
    s32 i;

    badgeShop_add((void*)((s32)bdsw + 0x4B), (s16)item, -1);
    for (i = 0; i < 4; i++) {
        if (item == table[evtGetValue(event, GSW(i + 0x76))]) {
            evtSetValue(event, GSW(i + 0x76), 0x10);
        }
    }
    return 2;
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_badgeShop_throw_dec) {
    s32 item = evtGetValue(event, event->args[0]);

    badgeShop_add((void*)((s32)bdsw + 0x19), (s16)item, -1);
    badgeShop_add(bdsw, (s16)item, -1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(evt_badgeShop_throw_inc) {
    s32 item = evtGetValue(event, event->args[0]);

    badgeShop_add((void*)((s32)bdsw + 0x19), (s16)item, 1);
    badgeShop_add(bdsw, (s16)item, 1);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


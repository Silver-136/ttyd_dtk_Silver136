#include "event/evt_badgeshop.h"

s32 evtGetValue(EventEntry* event, s32 value);
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void badgeShop_add(void* shop, s16 item, s32 count);
extern void* bdsw;

s32 U_badgeShop_SpecialCheck(void* unused, void* shop) {
    s32 count = 0;
    if (*(s32*)((s32)shop + 0xF8) != 0) count++;
    if (*(s32*)((s32)shop + 0xFC) != 0) count++;
    if (*(s32*)((s32)shop + 0x100) != 0) count++;
    if (*(s32*)((s32)shop + 0x104) != 0) count++;
    if (*(s32*)((s32)shop + 0x108) != 0) count++;
    return count;
}

s32 badgeShop_ThrowCheck(s32 value) {
    void* shop = bdsw;
    s32 count = 0;
    s32 i;
    for (i = 0; i < *(s32*)((s32)shop + 0x10C); i++) {
        if (value == *(s32*)((s32)shop + 0xF8 + (*(s32*)((s32)shop + 0x10C) + i) * 4)) {
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
    badgeShop_add(bdsw, (s16)evtGetValue(event, event->args[0]), -1);
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

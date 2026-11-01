/* "evt_badgeshop" - Badge Shop Script Functions
 * Part of the backend for the shops (badge shop functions for event scripts).
 */
#include "version.h"
#if VERSION <= VERSION_DEMO_US
#define DTRIGF_PLAIN_DATA
#include <dtrigf.h>
#include "evt/evt_badgeshop.h"
#else
#include "evt/evt_badgeshop.h"
#endif
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include <string.h>

//.data
#if VERSION > VERSION_DEMO_US
s32 badge_starmaniac_table[] = { //Dazzle
    ITEM_ATTACK_FX_Y, ITEM_CHILL_OUT, ITEM_PRETTY_LUCKY, ITEM_HAPPY_HEART,
    ITEM_HAPPY_FLOWER, ITEM_HAPPY_HEART_P, ITEM_ITEM_HOG, ITEM_HEART_FINDER,
    ITEM_FLOWER_FINDER, ITEM_PEEKABOO, ITEM_QUICK_CHANGE, ITEM_FLOWER_SAVER,
    ITEM_FLOWER_SAVER_P, ITEM_POWER_PLUS, ITEM_POWER_PLUS_P, ITEM_NULL
};
#endif

/**
 * Charlieton's Rogueport stock: the badges
 * and items he can offer; the bargain slots store indexes into it.
 */
#if VERSION > VERSION_DEMO_US
s32 badge_bottakuru_table[] = { //Charlieton Rogueport
    ITEM_SHOOTING_STAR, ITEM_EARTH_QUAKE, ITEM_BOOS_SHEET, ITEM_REPEL_CAPE,
    ITEM_STOPWATCH, ITEM_ULTRA_SHROOM, ITEM_JAMMIN_JELLY, ITEM_DRIED_SHROOM,
    ITEM_DOUBLE_PAIN, ITEM_JUMPMAN, ITEM_HAMMERMAN, ITEM_ATTACK_FX_G,
    ITEM_SLOW_GO, ITEM_MEGA_RUSH, ITEM_TIMING_TUTOR, ITEM_W_EMBLEM, ITEM_NULL
};
#endif

//.sdata
s32 badge_special_table[] = { //Lovely Howz of Badges
    ITEM_PIERCING_BLOW, ITEM_POWER_JUMP, ITEM_SIMPLIFIER, ITEM_UNSIMPLIFIER,
    ITEM_FP_DRAIN, ITEM_LAST_STAND, ITEM_LAST_STAND_P, ITEM_HAPPY_FLOWER,
    ITEM_SUPER_APPEAL, ITEM_NULL
};

/** Badges Lovely Howz of Badges adds to its stock once Hooktail Castle (chapter 1) is cleared. */
s32 badge_special_table_stage_1_clear[] = { //Hooktail Castle
    ITEM_CLOSE_CALL, ITEM_CLOSE_CALL_P, ITEM_SLEEPY_STOMP, ITEM_NULL
};

/** Badges added to the Lovely Howz of Badges stock after the Boggly Tree (chapter 2). */
s32 badge_special_table_stage_2_clear[] = { //Boggly Tree
    ITEM_FIRST_ATTACK, ITEM_PRETTY_LUCKY_P, ITEM_POWER_RUSH, ITEM_NULL
};

/** Badges added to the Lovely Howz of Badges stock after the Glitz Pit (chapter 3). */
s32 badge_special_table_stage_3_clear[] = { //Glitz Pit
    ITEM_SHRINK_STOMP, ITEM_ICE_SMASH, ITEM_POWER_RUSH_P, ITEM_NULL
};

/** Badges added to the Lovely Howz of Badges stock after the Creepy Steeple (chapter 4). */
s32 badge_special_table_stage_4_clear[] = { //Creepy Steeple
    ITEM_SOFT_STOMP, ITEM_HEAD_RATTLE, ITEM_DAMAGE_DODGE, ITEM_NULL
};

/** Badges added to the Lovely Howz of Badges stock after Keelhaul Key (chapter 5). */
s32 badge_special_table_stage_5_clear[] = { //Keelhaul Key
    ITEM_SUPER_APPEAL_P, ITEM_DAMAGE_DODGE_P, ITEM_FIRE_DRIVE, ITEM_NULL
};

/** Badges added to the Lovely Howz of Badges stock after Poshley Heights (chapter 6). */
s32 badge_special_table_stage_6_clear[] = { //Poshley Heights
    ITEM_SIMPLIFIER, ITEM_UNSIMPLIFIER, ITEM_ATTACK_FX_P, ITEM_NULL
};

#if VERSION <= VERSION_DEMO_US
s32 badge_starmaniac_table[] = { //Dazzle
    ITEM_ATTACK_FX_Y, ITEM_CHILL_OUT, ITEM_PRETTY_LUCKY, ITEM_HAPPY_HEART,
    ITEM_HAPPY_FLOWER, ITEM_HAPPY_HEART_P, ITEM_ITEM_HOG, ITEM_HEART_FINDER,
    ITEM_FLOWER_FINDER, ITEM_PEEKABOO, ITEM_QUICK_CHANGE, ITEM_FLOWER_SAVER,
    ITEM_FLOWER_SAVER_P, ITEM_POWER_PLUS, ITEM_POWER_PLUS_P, ITEM_NULL
};
s32 badge_bottakuru_table[] = { //Charlieton Rogueport
    ITEM_SHOOTING_STAR, ITEM_EARTH_QUAKE, ITEM_BOOS_SHEET, ITEM_REPEL_CAPE,
    ITEM_STOPWATCH, ITEM_ULTRA_SHROOM, ITEM_JAMMIN_JELLY, ITEM_DRIED_SHROOM,
    ITEM_DOUBLE_PAIN, ITEM_JUMPMAN, ITEM_HAMMERMAN, ITEM_ATTACK_FX_G,
    ITEM_SLOW_GO, ITEM_MEGA_RUSH, ITEM_TIMING_TUTOR, ITEM_W_EMBLEM, ITEM_NULL
};
#endif
/** Charlieton's stock in the Pit of 100 Trials: basic recovery and attack items. */
s32 badge_bottakuru100_table[] = { //Charlieton Pit of 100 Trials
    ITEM_MUSHROOM, ITEM_SUPER_SHROOM, ITEM_HONEY_SYRUP, ITEM_MAPLE_SYRUP,
    ITEM_FIRE_FLOWER, ITEM_THUNDER_RAGE, ITEM_NULL
};

/** Prizes at the Pianta Parlor counter that are always on offer. */
s32 badge_bteresa_table[] = { //Pianta Parlor
    ITEM_SUPER_SHROOM, ITEM_CAKE_MIX, ITEM_SUPER_APPEAL, ITEM_REFUND, ITEM_NULL
};

/** Extra Pianta Parlor prizes unlocked by the Special Card. */
s32 badge_bteresa_table_card_special[] = { //Pianta Parlor Special Card
    ITEM_MAPLE_SYRUP, ITEM_POWER_JUMP, ITEM_HP_PLUS, ITEM_FP_PLUS, ITEM_NULL
};

// US moves Multibounce from the Gold Card list to the Silver Card list.
s32 badge_bteresa_table_card_silver[] = { // Pianta Parlor Silver Card
    ITEM_GOLD_BAR_X3, ITEM_POWER_SMASH, ITEM_HP_PLUS_P,
#if VERSION >= VERSION_US
    ITEM_MULTIBOUNCE,
#endif
    ITEM_NULL
};

/** Extra Pianta Parlor prizes unlocked by the Gold Card. */
s32 badge_bteresa_table_card_gold[] = { // Pianta Parlor Gold Card
    ITEM_HAMMER_THROW, ITEM_POWER_RUSH, ITEM_TORNADO_JUMP,
#if VERSION < VERSION_US
    ITEM_MULTIBOUNCE,
#endif
    ITEM_POWER_RUSH_P, ITEM_NULL
};

/** Extra Pianta Parlor prizes unlocked by the Platinum Card. */
s32 badge_bteresa_table_card_platinum[] = { //Pianta Parlor Platinum Card
    ITEM_QUAKE_HAMMER, ITEM_MONEY_MONEY, ITEM_ULTRA_SHROOM, ITEM_JAMMIN_JELLY, ITEM_NULL
};

//.bss
#if VERSION <= VERSION_DEMO_US
s32 badge_bteresa_table_nedan[] = {
    400, 200, 100, 50,
    800, 600, 400, 200,
    1600, 900, 500, 300,
    9999, 999, 599, 399,
    0
};
#endif
BadgeShopWork badgeshopwork;

//.sbss
BadgeShopWork* bdsw;

/** Number of items in Charlieton's Pit of 100 Trials list (without the ITEM_NULL end); winmgr sizes that menu by it. */
s32 getBadgeBottakuru100TableMaxCount(void) {
    return (sizeof(badge_bottakuru100_table) / sizeof(s32)) - 1;
}

/** Number of badges in Dazzle's list (without the ITEM_NULL end); winmgr sizes the Star Piece exchange menu by it. */
s32 getBadgeStarmaniacTableMaxCount(void) {
    return (sizeof(badge_starmaniac_table) / sizeof(s32)) - 1;
}

/**
 * Number of Pianta Parlor prizes across the base list and all four card lists (Special, Silver, Gold, Platinum);
 * winmgr sizes the prize menu by it.
 */
s32 getBadgeBteresaTableMaxCount(void) {
    return ((sizeof(badge_bteresa_table) / sizeof(s32)) - 1) +
        ((sizeof(badge_bteresa_table_card_special) / sizeof(s32)) - 1) +
        ((sizeof(badge_bteresa_table_card_silver) / sizeof(s32)) - 1) +
        ((sizeof(badge_bteresa_table_card_gold) / sizeof(s32)) - 1) +
        ((sizeof(badge_bteresa_table_card_platinum) / sizeof(s32)) - 1);
}

/**
 * Resets all shop stock: one of each Lovely Howz special badge, Dazzle badge and Charlieton item,
 * the base Pianta Parlor prizes, and Charlieton's first offer. Sets restockFlags bit 0.
 * On boot (mariost.c) and new game (stg0_00).
 */
void badgeShop_init(void) {
    s32* ptr;
    s32* p2;

    bdsw = &badgeshopwork;
    memset(bdsw->special_table, 0, sizeof(bdsw->special_table));
    memset(bdsw->bargain_table, 0, sizeof(bdsw->bargain_table));
    memset(bdsw->starmaniac_table, 0, sizeof(bdsw->starmaniac_table));
    memset(bdsw->bottakuru_table, 0, sizeof(bdsw->bottakuru_table));
    memset(bdsw->bteresa_table, 0, sizeof(bdsw->bteresa_table));
    bdsw->restockFlags = BADGESHOP_RESTOCK_PENDING;

    for (ptr = badge_special_table; *ptr; ++ptr) {
        badgeShop_add(bdsw->special_table, (s16)*ptr, 1);
    }
    for (ptr = badge_starmaniac_table; *ptr; ++ptr) {
        badgeShop_add(bdsw->starmaniac_table, (s16)*ptr, 1);
    }
    for (ptr = badge_bottakuru_table; *ptr; ++ptr) {
        badgeShop_add(bdsw->bottakuru_table, (s16)*ptr, 1);
    }

    badgeShop_bottakuruGeneration();

    for (p2 = badge_bteresa_table; *p2; ++p2) {
        badgeShop_add(bdsw->bteresa_table, (s16)*p2, 1);
    }
}

/** qqsort comparator for badgeShop_bottakuruGeneration: orders (item, key) pairs by the key in word 1, ascending. */
s32 bottakuruComp(const s32* sortA, const s32* sortB) {
    s32 a, b;

    a = sortA[1];
    b = sortB[1];
    if (a < b) {
        return -1;
    }
    return a > b;
}

void badgeShop_bottakuruGeneration(void) {
    s32 table[0x11][2];
    s32 i;
    s32 k;
    s32 m;
    s32 j;
    s16 count;

    for (i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
        table[i][1] = 200;
        table[i][0] = 0;
    }

    k = 0;
    for (i = 0; i < BADGE_MAX - 1; i++) {
        count = badgeShop_get(bdsw->bottakuru_table, (s16)(i + 1));
        // FAKE: dead int-guarded re-definition (COLOURING M): count keeps a second def, so it stays a named web and takes the target r21
        j = 0;
        if (j != 0) {
            count = count;
        }
        for (j = 0; j < count; j++) {
            table[k][1] = irand(100);
            table[k++][0] = i + 1;
            if (k >= 0x10u) {
                break;
            }
        }
        if (k >= 0x10u) {
            break;
        }
    }

    qqsort(table, 0x11, sizeof(table[0]), (QSortCompare)bottakuruComp);

    for (i = 0; i < 4; i++) {
        for (m = 0; m < sizeof(badge_bottakuru_table) / sizeof(badge_bottakuru_table[0]); m++) {
            if (table[i][0] == badge_bottakuru_table[m]) {
                evtSetValue(NULL, i + GSW_BADGESHOP_CHARLIETON_STOCK, m);
            }
        }
    }
}

void badgeShop_bteresaGeneration(void) {
    s32* ptr;
    s32* p2;
    s32 has;

    has = pouchCheckItem(ITEM_SPECIAL_CARD);
    if (has) {
        if (!evtGetValue(NULL, GSWF_PARLOR_SPECIAL_CARD_PRIZES_ADDED)) {
            evtSetValue(NULL, GSWF_PARLOR_SPECIAL_CARD_PRIZES_ADDED, 1);
            for (ptr = badge_bteresa_table_card_special; *ptr; ptr++) {
                badgeShop_add(bdsw->bteresa_table, (s16)*ptr, 1);
            }
        }
    }

    has = pouchCheckItem(ITEM_SILVER_CARD);
    if (has) {
        if (!evtGetValue(NULL, GSWF_PARLOR_SILVER_CARD_PRIZES_ADDED)) {
            evtSetValue(NULL, GSWF_PARLOR_SILVER_CARD_PRIZES_ADDED, 1);
            for (ptr = badge_bteresa_table_card_silver; *ptr; ptr++) {
                badgeShop_add(bdsw->bteresa_table, (s16)*ptr, 1);
            }
        }
    }

    has = pouchCheckItem(ITEM_GOLD_CARD);
    if (has) {
        if (!evtGetValue(NULL, GSWF_PARLOR_GOLD_CARD_PRIZES_ADDED)) {
            evtSetValue(NULL, GSWF_PARLOR_GOLD_CARD_PRIZES_ADDED, 1);
            for (ptr = badge_bteresa_table_card_gold; *ptr; ptr++) {
                badgeShop_add(bdsw->bteresa_table, (s16)*ptr, 1);
            }
        }
    }

    has = pouchCheckItem(ITEM_PLATINUM_CARD);
    if (has) {
        if (!evtGetValue(NULL, GSWF_PARLOR_PLATINUM_CARD_PRIZES_ADDED)) {
            evtSetValue(NULL, GSWF_PARLOR_PLATINUM_CARD_PRIZES_ADDED, 1);
            for (p2 = badge_bteresa_table_card_platinum; *p2; p2++) {
                badgeShop_add(bdsw->bteresa_table, (s16)*p2, 1);
            }
        }
    }
}

void badgeShop_bargainGeneration(void) {
    s16 pickIndex;
    u8 table[0x19];
    s16 has;
    s32 badge;
    s16 value;
    int i;
    s32 item;
    s32 pickedCount;
    s32 candidateCount;
    s32 targetCount;
    s16 seenCount;
    s32* ptr;
    s32 stockedCount;
    s32 flag2Count;

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG1_16_HOOKTAIL && !evtGetValue(NULL, GSWF_BADGESHOP_STAGE_1_STOCK_ADDED)) {
        evtSetValue(NULL, GSWF_BADGESHOP_STAGE_1_STOCK_ADDED, 1);
        for (ptr = badge_special_table_stage_1_clear; *ptr; ptr++) {
            badgeShop_add(bdsw->special_table, (s16)*ptr, 1);
        }
    }

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG2_25_AFTER_THE_BOSS_BATTLE && !evtGetValue(NULL, GSWF_BADGESHOP_STAGE_2_STOCK_ADDED)) {
        evtSetValue(NULL, GSWF_BADGESHOP_STAGE_2_STOCK_ADDED, 1);
        for (ptr = badge_special_table_stage_2_clear; *ptr; ptr++) {
            badgeShop_add(bdsw->special_table, (s16)*ptr, 1);
        }
    }

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG3_32_BOSS_BATTLE && !evtGetValue(NULL, GSWF_BADGESHOP_STAGE_3_STOCK_ADDED)) {
        evtSetValue(NULL, GSWF_BADGESHOP_STAGE_3_STOCK_ADDED, 1);
        for (ptr = badge_special_table_stage_3_clear; *ptr; ptr++) {
            badgeShop_add(bdsw->special_table, (s16)*ptr, 1);
        }
    }

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG4_15_02_BACK_TO_NORMAL && !evtGetValue(NULL, GSWF_BADGESHOP_STAGE_4_STOCK_ADDED)) {
        evtSetValue(NULL, GSWF_BADGESHOP_STAGE_4_STOCK_ADDED, 1);
        for (ptr = badge_special_table_stage_4_clear; *ptr; ptr++) {
            badgeShop_add(bdsw->special_table, (s16)*ptr, 1);
        }
    }

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG5_33_CORTEZ_AND_THE_TREASURE && !evtGetValue(NULL, GSWF_BADGESHOP_STAGE_5_STOCK_ADDED)) {
        evtSetValue(NULL, GSWF_BADGESHOP_STAGE_5_STOCK_ADDED, 1);
        for (ptr = badge_special_table_stage_5_clear; *ptr; ptr++) {
            badgeShop_add(bdsw->special_table, (s16)*ptr, 1);
        }
    }

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG6_53_THE_REAL_GARNET_STAR && !evtGetValue(NULL, GSWF_BADGESHOP_STAGE_6_STOCK_ADDED)) {
        evtSetValue(NULL, GSWF_BADGESHOP_STAGE_6_STOCK_ADDED, 1);
        for (ptr = badge_special_table_stage_6_clear; *ptr; ptr++) {
            badgeShop_add(bdsw->special_table, (s16)*ptr, 1);
        }
    }

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG7_20_LORD_CRUMP_DEFEATED && !evtGetValue(NULL, GSWF_BADGESHOP_STAGE_7_STOCK_CHECK_DONE)) {
        evtSetValue(NULL, GSWF_BADGESHOP_STAGE_7_STOCK_CHECK_DONE, 1);
    }

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) >= STG8_16_THE_RETURN_OF_THE_QUEEN && !evtGetValue(NULL, GSWF_BADGESHOP_STAGE_8_STOCK_CHECK_DONE)) {
        evtSetValue(NULL, GSWF_BADGESHOP_STAGE_8_STOCK_CHECK_DONE, 1);
    }

    memset(table, 0, sizeof(table));
    candidateCount = stockedCount = flag2Count = i = 0;
    bdsw->bargainBadges[0] = stockedCount;
    bdsw->bargainBadges[1] = stockedCount;
    bdsw->bargainBadges[2] = stockedCount;
    bdsw->bargainBadges[3] = stockedCount;
    bdsw->bargainBadges[4] = stockedCount;
    bdsw->bargainNum = stockedCount;
    bdsw->reserved_110 = stockedCount;

    for (; i < BADGE_COUNT; i++) {
        value = 0;
        has = badgeShop_get(bdsw->special_table, (s16)(i + BADGE_MIN));
        if (has) {
            stockedCount++;
            value |= 1;
        }
        if (value) {
            badgeShop_set(table, (s16)(i + BADGE_MIN), value);
            candidateCount++;
        }
    }
    if (candidateCount > 5) {
        targetCount = 5;
    } else {
        targetCount = candidateCount;
    }
    irand(0x100); // result unused
    while (bdsw->bargainNum + bdsw->reserved_110 < targetCount) {
        pickedCount = bdsw->bargainNum;
        if (pickedCount < 3 || !flag2Count) {
            pickIndex = irand(stockedCount);
            seenCount = 0;
            for (i = 0; i < BADGE_COUNT; i++) {
                badge = badgeShop_get(table, (s16)(i + BADGE_MIN));
                if (badge & 1) {
                    if (seenCount == pickIndex) {
                        break;
                    }
                    seenCount++;
                }
            }
            if (i < BADGE_COUNT) {
                bdsw->bargainBadges[bdsw->bargainNum] = i + BADGE_MIN;
                bdsw->bargainNum++;
                badgeShop_set(table, (s16)(i + BADGE_MIN), 0);
                if ((s16)badge & 1) {
                    stockedCount--;
                }
                if ((s16)badge & 2) {
                    flag2Count--;
                }
            }
        }
    }
    for (i = 0; i < sizeof(bdsw->special_table); i++) {
        bdsw->bargain_table[i] = bdsw->special_table[i];
    }
    for (i = 0; i < 5; i++) {
        item = bdsw->bargainBadges[i];
        if (item) {
            badgeShop_add(bdsw->bargain_table, (s16)item, -1);
        }
    }
}

#pragma dont_inline on
/**
 * Writes a 2-bit stock count (clamped to 0-3) for item id into a shop table and returns the stored count.
 * Tables hold four items per byte from BADGE_MIN, or from KEY_ITEM_MIN for Charlieton's and the Parlor's;
 * other ids give 0.
 */
s16 badgeShop_set(u8* table, s16 id, s16 val) {
    s32 bound;
    s16 index;
    u8 mask;
    s32 shift;
    s32 pos;

    if (table == bdsw->bottakuru_table || table == bdsw->bteresa_table) {
        bound = KEY_ITEM_MIN;
    } else {
        bound = BADGE_MIN;
    }
    if (id < bound || id >= BADGE_MAX) {
        return 0;
    }
    index = (s16)(id - bound);
    switch (index & 3) {
        case 0:
            mask = (u8)~(3 << 0);
            shift = 0;
            break;
        case 1:
            mask = (u8)~(3 << 2);
            shift = 2;
            break;
        case 2:
            mask = (u8)~(3 << 4);
            shift = 4;
            break;
        case 3:
            mask = (u8)~(3 << 6);
            shift = 6;
            break;
    }
    pos = index / 4;
    if (val < 0) {
        val = 0;
    } else if (val > 3) {
        val = 3;
    }
    table[pos] = (u8)((u8)(mask & table[pos]) + (u8)(val << shift));
    return val;
}

#pragma dont_inline reset
/**
 * Reads the 2-bit stock count (0-3) of item id from a shop table;
 * same layout as badgeShop_set, 0 when out of range.
 */
s32 badgeShop_get(u8* table, s16 id) {
    s32 bound;
    s16 index;
    u8 mask;
    s32 shift;

    if (table == bdsw->bottakuru_table || table == bdsw->bteresa_table) {
        bound = KEY_ITEM_MIN;
    } else {
        bound = BADGE_MIN;
    }
    if (id < bound || id >= BADGE_MAX) {
        return 0;
    }
    index = (s16)(id - bound);
    switch (index & 3) {
        case 0:
            mask = 3 << 0;
            shift = 0;
            break;
        case 1:
            mask = 3 << 2;
            shift = 2;
            break;
        case 2:
            mask = 3 << 4;
            shift = 4;
            break;
        case 3:
            mask = 3 << 6;
            shift = 6;
            break;
    }
    return (s16)((mask & table[index / 4]) >> shift);
}

/**
 * Adds value (may be negative) to the 2-bit stock count of item id, clamped to 0-3, and returns the new count;
 * same table layout as badgeShop_set.
 */
s32 badgeShop_add(u8* table, s16 id, s32 value) {
    s32 temp;
    s32 mask;
    s32 shift;
    s32 bound;
    int curr;
    int data;

    if (table == bdsw->bottakuru_table || table == bdsw->bteresa_table) {
        bound = KEY_ITEM_MIN;
    } else {
        bound = BADGE_MIN;
    }
    temp = (s16)id;
    if (temp < bound || temp >= BADGE_MAX) {
        return 0;
    }

    id = (s16)(temp - bound);
    switch (id & 3) {
        case 0:
            mask = 3 << 0;
            shift = 0;
            break;
        case 1:
            mask = 3 << 2;
            shift = 2;
            break;
        case 2:
            mask = 3 << 4;
            shift = 4;
            break;
        case 3:
            mask = 3 << 6;
            shift = 6;
            break;
    }
    data = value;
    mask &= 0xFF;
    curr = (mask & table[id / 4]) >> shift;
    data = (s16)((s16)curr + data);
    if (data < 0) {
        data = 0;
    } else if (data > 3) {
        data = 3;
    }
    table[id / 4] = (u8)(table[id / 4] & ~mask) + (u8)((s16)data << shift);
    return data;
}

/** Stores a pointer to the five bargain badge ids (bdsw->bargainBadges) in args[0] for the shop script. */
USER_FUNC(badgeShop_getBargainTable) {
    evtSetValue(event, *event->args, (EvtWord)&bdsw->bargainBadges);
    return EVT_RETURN_DONE;
}

/**
 * Adds one of the badge in args[0] to the Lovely Howz stock (both the special and bargain tables).
 * gor_01's sell_evt calls it after Mario sells the badge, so the shop puts it back on sale.
 */
USER_FUNC(evt_badgeShop_throw_inc) {
    s32 value;

    value = evtGetValue(event, *event->args);
    badgeShop_add(bdsw->bargain_table, (s16)value, 1);
    badgeShop_add(bdsw->special_table, (s16)value, 1);
    return EVT_RETURN_DONE;
}

/** Removes one of the badge in args[0] from the Lovely Howz stock (both the special and bargain tables). */
USER_FUNC(evt_badgeShop_throw_dec) {
    s32 value;

    value = evtGetValue(event, *event->args);
    badgeShop_add(bdsw->bargain_table, (s16)value, -1);
    badgeShop_add(bdsw->special_table, (s16)value, -1);
    return EVT_RETURN_DONE;
}

/** Removes one of the badge in args[0] from the Lovely Howz special stock only. */
USER_FUNC(evt_badgeShop_special_dec) {
    s32 value;

    value = evtGetValue(event, *event->args);
    badgeShop_add(bdsw->special_table, (s16)value, -1);
    return EVT_RETURN_DONE;
}

/** Removes one of the badge in args[0] from Dazzle's stock (bought with Star Pieces). */
USER_FUNC(evt_badgeShop_starmaniac_dec) {
    s32 value;

    value = (s16)evtGetValue(event, *event->args);
    badgeShop_add(bdsw->starmaniac_table, (s16)value, -1);
    return EVT_RETURN_DONE;
}

#pragma opt_common_subs off
USER_FUNC(evt_badgeShop_bottakuru_dec) {
    s32 value;
    int i;

    value = evtGetValue(event, *event->args);
    badgeShop_add(bdsw->bottakuru_table, (s16)value, -1);
    for (i = 0; i < 4; i++) {
        if (value == badge_bottakuru_table[evtGetValue(event, i + GSW_BADGESHOP_CHARLIETON_STOCK)]) {
            evtSetValue(event, i + GSW_BADGESHOP_CHARLIETON_STOCK, (sizeof(badge_bottakuru_table) / sizeof(s32)) - 1); // index of the ITEM_NULL terminator
        }
    }
    return EVT_RETURN_DONE;
}

#pragma opt_common_subs reset
#if VERSION <= VERSION_DEMO_US
/** the demo counts the thrown badges through an inline copy of badgeShop_ThrowCheck (with its bug: always 0) */
static inline s32 badgeShop_ThrowCount(s32 itemId) {
    int i;
    s32 count;

    for (count = 0, i = 0; i < bdsw->bargainNum; i++) {
        if (itemId == bdsw->bargainBadges[i + bdsw->bargainNum]) {
            count++;
        }
    }
    return count;
}
#endif
/**
 * Counts how many different badges are left in the Lovely Howz bargain_table and writes it to args[0].
 * The demo also subtracts badgeShop_ThrowCount from each badge's stock first, which is always 0.
 */
USER_FUNC(evt_badgeShop_throw_get_kind_cnt) {
    EvtWord* args = event->args;
    s16 badge;
    s32 count;
    long i; // FAKE: `long` (same size): the int form allocates registers differently in the demos

    for (count = 0, i = 0; i < BADGE_COUNT; i++) {
#if VERSION <= VERSION_DEMO_US
        if ((badge = badgeShop_get(bdsw->bargain_table, (s16)(i + BADGE_MIN))) - badgeShop_ThrowCount(i + BADGE_MIN) > 0) {
#else
        badge = badgeShop_get(bdsw->bargain_table, (s16)(i + BADGE_MIN));
        if (badge > 0) {
#endif
            count++;
        }
    }
    evtSetValue(event, *args, count);
    return EVT_RETURN_DONE;
}

/** Counts how many different badges Dazzle still has in stock and writes it to args[0]. */
USER_FUNC(evt_badgeShop_starmaniac_get_kind_cnt) {
    EvtWord* args = event->args;
    s16 badge;
    s32 count;
    int i;

    for (count = 0, i = 0; i < BADGE_COUNT; i++) {
        badge = badgeShop_get(bdsw->starmaniac_table, (s16)(i + BADGE_MIN));
        if (badge > 0) {
            count++;
        }
    }
    evtSetValue(event, *args, count);
    return EVT_RETURN_DONE;
}

/** Counts how many of Charlieton's four offer slots hold an item (not the empty entry) and writes it to args[0]. */
USER_FUNC(evt_badgeShop_bottakuru_get_kind_cnt) {
    EvtWord* args = event->args;
    s32 count;
    int i;

    for (count = 0, i = 0; i < 4; i++) {
        if (badge_bottakuru_table[evtGetValue(event, i + GSW_BADGESHOP_CHARLIETON_STOCK)]) {
            count++;
        }
    }
    evtSetValue(event, *args, count);
    return EVT_RETURN_DONE;
}

#if VERSION > VERSION_DEMO_US
USER_FUNC(U_evt_badgeShop_get_special_zaiko) {
    EvtWord* args = event->args;
    s16 badge;
    s32 count;
    int i;

    for (count = 0, i = 0; i < BADGE_MAX - 1; i++) {
        badge = badgeShop_get(bdsw->bottakuru_table, (s16)(i + KEY_ITEM_MIN));
        if (!(badge <= 0)) {
            count++;
        }
    }
    evtSetValue(event, *args, count);
    return EVT_RETURN_DONE;
}
#endif

/** Counts how many different prizes the Pianta Parlor has in stock and writes it to args[0]. */
USER_FUNC(evt_badgeShop_bteresa_get_kind_cnt) {
    EvtWord* args = event->args;
    s16 badge;
    s32 count;
    int i;

    for (count = 0, i = 0; i < BADGE_MAX - 1; i++) {
        badge = badgeShop_get(bdsw->bteresa_table, (s16)(i + KEY_ITEM_MIN));
        if (badge > 0) {
            count++;
        }
    }
    evtSetValue(event, *args, count);
    return EVT_RETURN_DONE;
}

/**
 * Counts itemId in bargainBadges[bargainNum..2*bargainNum-1];
 * the callers (winmgr.c, evt_win.c) subtract the count from the bargain stock when they build the badge lists.
 * Those slots lie past the five-entry array (bargainNum, reserved_110, restockFlags and padding),
 * which never hold a badge id, so it always returns 0.
 * Guess: it was meant to count copies already set aside.
 */
#if VERSION <= VERSION_DEMO_US
/** demo only: TRUE once word 6 of the block in LW(0) reaches the bargain count (no caller in the demo dol) */
USER_FUNC(evt_badgeShop_getBargainKind) {
    evtSetValue(event, event->args[0], !(((s32*)event->lwData[0])[6] < bdsw->bargainNum));
    return EVT_RETURN_DONE;
}
#endif
/**
 * Counts itemId in bargainBadges[bargainNum..2*bargainNum-1];
 * the callers (winmgr.c, evt_win.c) subtract the count from the bargain stock when they build the badge lists.
 * Those slots lie past the five-entry array (bargainNum, reserved_110, restockFlags and padding),
 * which never hold a badge id, so it always returns 0.
 * Guess: it was meant to count copies already set aside.
 */
s32 badgeShop_ThrowCheck(s32 itemId) {
    int i;
    s32 count;

    for (count = 0, i = 0; i < bdsw->bargainNum; i++) {
        // BUG: reads bargainBadges[bargainNum + i], not the bargainNum entries stored from index 0 (and past the
        // five-entry array, into the following BadgeShopWork fields, once bargainNum > 2)
        if (itemId == bdsw->bargainBadges[i + bdsw->bargainNum]) {
            count++;
        }
    }
    return count;
}

#if VERSION > VERSION_DEMO_US
s32 badgeShop_SpecialCheck(void) {
    s32 count;
    int i;

    for (count = 0, i = 0; i < 5; i++) {
        if (bdsw->bargainBadges[i]) {
            count++;
        }
    }
    return count;
}
#endif


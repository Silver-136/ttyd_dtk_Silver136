// Mario's pouch (save data): items, badges, coins, HP/FP, Star Points and level, partners, Star Pieces and
// Shine Sprites, the item and badge sorts.

#include "version.h"
#include "system.h"
#include "mario/mario_pouch.h"
#include "driver/msgdrv.h"
#include "mario/mario_party.h"
#include "evt/evt_badgeshop.h"
#include "evt/evt_yuugijou.h"
#include "data/item_data.h"
#include "memory.h"
#include <string.h>
#if VERSION >= VERSION_US
#include "mario/mariost.h"
#endif
#include "driver/swdrv.h"



PouchData* mpp_gap;
static PouchData* mpp;

/**
 * Max HP of each partner by partner id (row) and HP level
 * (column: levels 0 to 2, then 200 for the cap).
 */
#if VERSION == VERSION_DEMO_JP
s16 _party_max_hp_table[8][4] = {
    {0, 0, 0, 0},
    {10, 20, 30, 200},
    {10, 15, 25, 200},
    {20, 30, 40, 200},
    {10, 20, 30, 200},
    {15, 25, 35, 200},
    {15, 20, 30, 200},
    {15, 20, 25, 200}
};
#elif VERSION == VERSION_DEMO_US
s16 _party_max_hp_table[9][4] = {
{0, 0, 0, 0},
    {10, 20, 30, 200},
    {10, 15, 25, 200},
    {20, 30, 40, 200},
    {10, 20, 30, 200},
    {15, 25, 35, 200},
    {15, 20, 30, 200},
    {15, 20, 25, 200},
{0, 0, 0, 0},
};
#else
s16 _party_max_hp_table[8][4] = {
    {0, 0, 0, 0},
    {10, 20, 30, 200},
    {10, 15, 25, 200},
    {20, 30, 40, 200},
    {10, 20, 30, 200},
    {15, 25, 35, 200},
    {15, 20, 30, 200},
    {15, 20, 25, 200}
};
#endif

/** The pouch (Mario's save data: items, badges, stats, partners, mail), allocated by pouchInit. */
PouchData* pouchGetPtr(void) {
    return mpp;
}

/**
 * Allocates and clears the pouch for a new game:
 * empty lists, 10 HP, 5 FP, 3 BP, level 1, partners at 10 HP, and Yoshi's default name.
 */
void pouchInit(void) {
    int i;

    // BUG: allocates a new PouchData without checking for or freeing the old one. pouchInit runs at boot
    // (marioInit) and again when a new file starts (stg0_00_init), so the first block is leaked.
    mpp = __memAlloc(HEAP_DEFAULT, sizeof(PouchData));
    memset(mpp, 0, sizeof(PouchData));
    for (i = 0; i < MAX_KEY_ITEMS; i++) {
        mpp->keyItems[i] = ITEM_NULL;
    }
    for (i = 0; i < MAX_HELD_ITEMS; i++) {
        mpp->heldItems[i] = ITEM_NULL;
    }
    for (i = 0; i < MAX_STORED_ITEMS; i++) {
        mpp->storedItems[i] = ITEM_NULL;
    }
    for (i = 0; i < MAX_BADGES; i++) {
        mpp->badges[i] = ITEM_NULL;
    }
    for (i = 0; i < MAX_BADGES; i++) {
        mpp->equippedBadges[i] = ITEM_NULL;
    }
    mpp->maximumHP = 10;
    mpp->baseMaxHP = 10;
    mpp->maximumFP = 5;
    mpp->baseMaxFP = 5;
    mpp->availableBP = 3;
    mpp->maximumBP = 3;
    mpp->currentHP = mpp->maximumHP;
    mpp->currentFP = mpp->maximumFP;
    mpp->currentSP = 0;
    mpp->reserved_7E = 0;
    mpp->maximumSP = 0;
    mpp->reserved_80 = 0;
    mpp->audienceCount = 0.0f;
    mpp->rank = 0;
    mpp->level = 1;
    mpp->jumpLevel = 0;
    mpp->hammerLevel = 0;
    for (i = 0; i < PARTNER_COUNT; i++) {
        PouchPartyData* party = &mpp->partyData[i];
        if (i != PARTNER_NONE) {
            party->flags = 0;
            party->baseMaxHP = 10;
            party->maximumHP = party->baseMaxHP;
            party->currentHP = party->baseMaxHP;
            party->attackLevel = 0;
            party->techLevel = 0;
        }
    }
#if VERSION == VERSION_EU
    strcpy(mpp->yoshiName, "Yoshi");
#elif VERSION >= VERSION_US
    strcpy(mpp->yoshiName, msgSearch("name_party3"));
#else
    strcpy(mpp->yoshiName, "チビヨッシー");
#endif
}

/** Key item in key item list slot id (0 if empty). */
ItemType pouchKeyItem(s32 id) {
    return mpp->keyItems[id];
}

/** Item in Mario's item list slot id (0 if empty). */
ItemType pouchHaveItem(s32 id) {
    return mpp->heldItems[id];
}

/** Item in storage slot id (0 if empty). */
ItemType pouchKeepItem(s32 id) {
    return mpp->storedItems[id];
}

/** Badge in badge list slot id (0 if empty). */
ItemType pouchHaveBadge(s32 id) {
    return mpp->badges[id];
}
#if VERSION >= VERSION_US
ItemType pouchEquipBadge(s32 id) {
    return mpp->equippedBadges[id];
}

#endif

/** Number of items Mario carries (out of 20 slots). */
s32 pouchGetHaveItemCnt(void) {
    s32 i, count;

    for (count = 0, i = 0; i < MAX_HELD_ITEMS; i++) {
        if (mpp->heldItems[i] != ITEM_NULL) {
            count++;
        }
    }
    return count;
}

/** Number of items in storage. */
s32 pouchGetKeepItemCnt(void) {
    s32 i, count;

    for (count = 0, i = 0; i < MAX_STORED_ITEMS; i++) {
        if (mpp->storedItems[i] != ITEM_NULL) {
            count++;
        }
    }
    return count;
}

/** Number of badges Mario owns. */
s32 pouchGetHaveBadgeCnt(void) {
    s32 i, count;

    for (count = 0, i = 0; i < MAX_BADGES; i++) {
        if (mpp->badges[i] != ITEM_NULL) {
            count++;
        }
    }
    return count;
}

/** Number of badges Mario is wearing. */
s32 pouchGetEquipBadgeCnt(void) {
    s32 i, count;

    for (count = 0, i = 0; i < MAX_BADGES; i++) {
        if (mpp->equippedBadges[i] != ITEM_NULL) {
            count++;
        }
    }
    return count;
}

/** Free slots in Mario's item list: 20 with the Strange Sack, else 10. */
s32 pouchGetEmptyHaveItemCnt(void) {
    s32 i, count, max;

    max = pouchCheckItem(ITEM_STRANGE_SACK) ? MAX_HELD_ITEMS : HELD_ITEM_COUNT_BASE;
    for (count = 0, i = 0; i < max; i++) {
        if (mpp->heldItems[i] == ITEM_NULL) {
            count++;
        }
    }
    return count;
}

/** Number of free item storage slots. */
s32 pouchGetEmptyKeepItemCnt(void) {
    s32 i, count;

    for (count = 0, i = 0; i < MAX_STORED_ITEMS; i++) {
        if (mpp->storedItems[i] == ITEM_NULL) {
            count++;
        }
    }
    return count;
}

/** we have obtained an item, update the appropriate data */
// Puts type at the front of list t1 if it has an empty slot: the entries above the last empty slot move down
// one place (t2, if given, moves in step and gets 0 at the front). Returns 1, or 0 when the list is full.
static inline BOOL pouchAdd(s16 *t1, s16 *t2, s32 size, ItemType type) {
    s32 j;
    for (size--; size >= 0; size--) {
        if (t1[size] == 0) {
            for (j = size; j > 0; j--) {
                t1[j] = t1[j - 1];
                if (t2) {
                    t2[j] = t2[j - 1];
                }
            }
            t1[0] = type;
            if (t2) {
                t2[0] = 0;
            }
            return 1;
        }
    }
    return 0;
}

/**
 * Gives Mario an item (pickups, scripts, battle audience gifts).
 * Coins (max 999), Pianta tokens (max 99999; the demo has no cap),
 * hearts and flowers (1 HP / FP to Mario and the current partner),
 * Star Pieces and Shine Sprites only bump counters.
 * Boots and hammers set the jump/hammer level, badges set their found flag (GSWF id - 0x70),
 * and both join a list. Returns FALSE when the list is full or the id is not a pouch item.
 */
BOOL pouchGetItem(ItemType type) {
    s16 *itemLists[2];
    s32 size;
    BOOL ok;
    if (type == ITEM_COIN) {
        if (mpp->coins < 999) {
            mpp->coins++;
        }
        return 1;
    }
    if (type == ITEM_PIANTA_TOKEN) {
#if VERSION <= VERSION_DEMO_US
        if (yuwp->tokens < 0x7FFFFFFF)
        {
#else
        if (yuwp->tokens < 99999) {
#endif
            yuwp->tokens++;
        }
        return 1;
    }
    if (type == ITEM_HEART) {
        size = marioGetParty();
        if (mpp->currentHP < mpp->maximumHP) {
            mpp->currentHP++;
        }
        if (size != 0) {
            if (mpp->partyData[size].currentHP < mpp->partyData[size].maximumHP) {
                mpp->partyData[size].currentHP++;
            }
        }
        return 1;
    }
    if (type == ITEM_FLOWER) {
        if (mpp->currentFP < mpp->maximumFP) {
            mpp->currentFP++;
            // FAKE: dead store (function returns right after); steers register allocation
            itemLists[1] = NULL;
        }
        return 1;
    }
    if (type == ITEM_STAR_PIECE) {
        mpp->starPieceCount++;
        return 1;
    }
    if (type == ITEM_SHINE_SPRITE) {
        mpp->shineSpriteCount++;
        return 1;
    }
    if (type == ITEM_BOOTS || type == ITEM_SUPER_BOOTS || type == ITEM_ULTRA_BOOTS) {
        switch (type) {
            case ITEM_BOOTS:
                mpp->jumpLevel = 1;
                break;
            case ITEM_SUPER_BOOTS:
                mpp->jumpLevel = 2;
                break;
            case ITEM_ULTRA_BOOTS:
                mpp->jumpLevel = 3;
                break;
        }
    } else if (type == ITEM_HAMMER || type == ITEM_SUPER_HAMMER || type == ITEM_ULTRA_HAMMER) {
        switch (type) {
            case ITEM_HAMMER:
                mpp->hammerLevel = 1;
                break;
            case ITEM_SUPER_HAMMER:
                mpp->hammerLevel = 2;
                break;
            case ITEM_ULTRA_HAMMER:
                mpp->hammerLevel = 3;
                break;
        }
    } else if (type == ITEM_BOWSER_BROS_MEAT) {
        return 1;
    } else if (type >= BADGE_MIN && type < BADGE_MAX) {
        swSet(type - 0x70);
    }
    size = 0;
    itemLists[0] = NULL;
    itemLists[1] = NULL;
    if (type >= ITEM_MIN && type < ITEM_MAX) {
        itemLists[0] = mpp->heldItems;
        size = pouchCheckItem(ITEM_STRANGE_SACK) ? MAX_HELD_ITEMS : HELD_ITEM_COUNT_BASE;
        ok = 1;
    } else if (type >= KEY_ITEM_MIN && type < KEY_ITEM_MAX) {
        itemLists[1] = NULL;
        size = MAX_KEY_ITEMS;
        ok = 1;
        itemLists[0] = mpp->keyItems;
    } else if (type >= BADGE_MIN && type < BADGE_MAX) {
        size = MAX_BADGES;
        ok = 1;
        itemLists[0] = mpp->badges;
        itemLists[1] = mpp->equippedBadges;
    } else {
        ok = 0;
    }
    if (!ok) {
        return 0;
    }
    return pouchAdd(itemLists[0], itemLists[1], size, type);
}


/** How many copies of an item, key item or badge Mario carries (stored items do not count); 0 for other ids. */
s32 pouchCheckItem(u32 itemId) {
    s32 id = itemId;
    s32 i;
    u32 itemCnt;

    if (id >= ITEM_MIN && id < ITEM_MAX) {
        itemCnt = 0;
        for (i = 0; i < MAX_HELD_ITEMS; i++) {
            if (id == mpp->heldItems[i]) {
                itemCnt++;
            }
        }
        return itemCnt;
    }
    if (id >= KEY_ITEM_MIN && id < KEY_ITEM_MAX) {
        itemCnt = 0;
        for (i = 0; i < MAX_KEY_ITEMS; i++) {
            if (id == mpp->keyItems[i]) {
                itemCnt++;
            }
        }
        return itemCnt;
    }
    if (id >= BADGE_MIN && id < BADGE_MAX) {
        itemCnt = 0;
        for (i = 0; i < MAX_BADGES; i++) {
            if (id == mpp->badges[i]) {
                itemCnt++;
            }
        }
        return itemCnt;
    }
    return 0;
}

/** Clears slot i of a list (and of its parallel worn-badge list, if any) and moves the later slots up one. */
static inline void pouchShift(s16 *table1, s16 *table2, int i, s32 maximum) {
    table1[i] = ITEM_NULL;
    if (table2 != NULL) {
        table2[i] = ITEM_NULL;
    }
    for (; i < maximum - 1; i++) {
        table1[i] = table1[i + 1];
        table1[i + 1] = ITEM_NULL;
        if (table2 != NULL) {
            table2[i] = table2[i + 1];
            table2[i + 1] = ITEM_NULL;
        }
    }
}

/** Removes the first copy of an item, key item or badge from its list and closes the gap; FALSE if Mario lacks it. */
BOOL pouchRemoveItem(ItemType type) {
    s16 *table1;
    s16 *table2;
    BOOL valid;
    s32 maximum;
    int i;
    maximum = 0;
    table1 = NULL;
    table2 = NULL;
    if (type >= ITEM_MIN && type < ITEM_MAX) {
        table1 = mpp->heldItems;
        table2 = NULL;
        maximum = pouchCheckItem(ITEM_STRANGE_SACK) ? MAX_HELD_ITEMS : HELD_ITEM_COUNT_BASE;
        valid = 1;
    } else if (type >= KEY_ITEM_MIN && type < KEY_ITEM_MAX) {
        table1 = mpp->keyItems;
        table2 = NULL;
        maximum = MAX_KEY_ITEMS;
        valid = 1;
    } else if (type >= BADGE_MIN && type < BADGE_MAX) {
        table1 = mpp->badges;
        table2 = mpp->equippedBadges;
        maximum = MAX_BADGES;
        valid = 1;
    } else {
        valid = 0;
    }
    if (!valid) {
        return 0;
    }
    for (i = 0; i < maximum; i++) {
        if (type == table1[i]) {
            pouchShift(table1, table2, i, maximum);
            return 1;
        }
    }

    return 0;
}

/**
 * Removes the item id from list slot index (the item used or thrown away, from the item menu,
 * battle and evt_pouch) and closes the gap; FALSE if the slot holds another item.
 * Badges drop their worn state too.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * The demo's pouchRemoveItemIndex shifts the second table wrongly:
 * it clears the slot it has just filled instead of the one it moved from (the demo's own bug;
 * the later versions call pouchShift).
 */
static inline void pouchShiftDemo(s16* table1, s16* table2, int i, s32 maximum) {
    table1[i] = ITEM_NULL;
    if (table2 != 0)
        table2[i] = ITEM_NULL;
    for (; i < maximum - 1; i++) {
        table1[i] = table1[i + 1];
        table1[i + 1] = ITEM_NULL;
        if (table2 != 0) {
            table2[i] = table2[i + 1];
            table2[i] = ITEM_NULL;
        }
    }
}
#endif
/**
 * Removes the item id from list slot index (the item used or thrown away, from the item menu,
 * battle and evt_pouch) and closes the gap; FALSE if the slot holds another item.
 * Badges drop their worn state too.
 */
BOOL pouchRemoveItemIndex(ItemType type, s32 index) {
    s16* table1, * table2, * p;
    BOOL valid;
    s32 maximum;
    int i;

    maximum = 0;
    table1 = NULL;
    table2 = NULL;
    if (type >= ITEM_MIN && type < ITEM_MAX) {
        table1 = mpp->heldItems;
        table2 = NULL;
        maximum = pouchCheckItem(ITEM_STRANGE_SACK) ? MAX_HELD_ITEMS : HELD_ITEM_COUNT_BASE;
        valid = TRUE;
    } else if (type >= KEY_ITEM_MIN && type < KEY_ITEM_MAX) {
        table1 = mpp->keyItems;
        table2 = NULL;
        maximum = MAX_KEY_ITEMS;
        valid = TRUE;
    } else if (type >= BADGE_MIN && type < BADGE_MAX) {
        table1 = mpp->badges;
        table2 = mpp->equippedBadges;
        maximum = MAX_BADGES;
        valid = TRUE;
    } else {
        valid = FALSE;
    }
    if (!valid) {
        return FALSE;
    }
    p = &table1[index];
    if (type != *p) {
        return FALSE;
    }
    for (i = index; i < maximum; p++, i++) {
        if (type == *p) {
#if VERSION <= VERSION_DEMO_US
            pouchShiftDemo(table1, table2, i, maximum);
#else
            pouchShift(table1, table2, i, maximum);
#endif
            return TRUE;
        }
    }
    return FALSE;
}

#if VERSION == VERSION_EU
char* strupr(char* str);
#endif

// Descriptive name (not in the symbol maps); placeholder unk_800d06b4, JP 0x800D06B4. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
void pouchHiraToKata(const char* src, char* dst) {
    u16 c;
    s32 len = strlen(src) / 2;
    u16* s = (u16*)src;
    u16* d = (u16*)dst;
    s32 i;
#if VERSION >= VERSION_US
    if ((u32)gp->language == LANGUAGE_JP) {
        s = (u16*)src; /* FAKE: s and d re-assigned (both already hold these values) inside the language branch */
        d = (u16*)dst;
        for (i = 0; i < len; i++) {
            c = *s;
            if (c >= 0x829F && c < 0x82DE) {
                c += 0xA1;
            } else if (c >= 0x82DE && c <= 0x82F1) {
                c += 0xA2;
            }
            *d = c;
            s++;
            d++;
        }
        dst[i * 2] = 0;
    } else {
#if VERSION == VERSION_EU
        // PAL: sort key of a Latin name: drops <...> message tags, upper-cases, folds accented vowels to
        // A E I O U, spells the OE ligature out and drops C-cedilla
        char buf[0x400];
        char* key = dst;
        BOOL inTag = FALSE;
        s32 n;

        for (; *(const s8*)src; src++) {
            switch (*(const s8*)src) {
                case '<':
                    inTag = TRUE;
                    break;
                case '>':
                    inTag = FALSE;
                    break;
                default:
                    if (!inTag) {
                        *dst = *(const s8*)src;
                        *++dst = 0;
                    }
                    break;
            }
        }
        strupr(key);
        for (i = 0; i < strlen(key); i++) {
            u8 ch = key[i];
            if ((ch >= 0xC0 && ch <= 0xC6) || (ch >= 0xE0 && ch <= 0xE6)) {
                ch = 'A';
            } else if ((ch >= 0xC8 && ch <= 0xCB) || (ch >= 0xE8 && ch <= 0xEB)) {
                ch = 'E';
            } else if ((ch >= 0xCC && ch <= 0xCF) || (ch >= 0xEC && ch <= 0xEF)) {
                ch = 'I';
            } else if ((ch >= 0xD2 && ch <= 0xD6) || (ch >= 0xF2 && ch <= 0xF6)) {
                ch = 'O';
            } else if ((ch >= 0xD9 && ch <= 0xDC) || (ch >= 0xF9 && ch <= 0xFC)) {
                ch = 'U';
            }
            key[i] = ch;
        }
        n = 0;
        for (i = 0; i < strlen(key); i++) {
            u8 ch = key[i];
            if (ch == 0x8C || ch == 0x9C) {
                buf[n++] = 'O';
                buf[n++] = 'E';
            } else if (ch != 0xC7 && ch != 0xE7) {
                buf[n++] = ch;
            }
            buf[n] = 0;
        }
        strcpy(key, buf);
#else
        strcpy(dst, src);
#endif
    }
}
#else
    for (i = 0; i < len; i++) {
        c = *s;
        if (c >= 0x829F && c < 0x82DE) {
            c += 0xA1;
        } else if (c >= 0x82DE && c <= 0x82F1) {
            c += 0xA2;
        }
        *d = c;
        s++;
        d++;
    }
    dst[i * 2] = 0;
}
#endif
#endif


/**
 * Sort key of an item name for the alphabetical sorts.
 * JP and US inline their own copy of the kana conversion here; PAL calls pouchHiraToKata.
 */
// PAL calls pouchHiraToKata; JP and US inline their own copy of the conversion
static inline void pouchAiueoConv(const char* src, char* dst) {
#if VERSION == VERSION_EU
    pouchHiraToKata(src, dst);
}
#else
#if VERSION >= VERSION_US
    u16* s;
    u16* d;
    s32 len = strlen(src) / 2;
#else
    u16* s = (u16*)src;
    s32 len = strlen(src) / 2;
    u16* d = (u16*)dst;
#endif
    u16 c;
    s32 i;
#if VERSION >= VERSION_US
    if ((u32)gp->language == LANGUAGE_JP) {
        s = (u16*)src; /* FAKE: s and d set inside the language branch, the same spelling as in pouchHiraToKata */
        d = (u16*)dst;
        for (i = 0; i < len; i++) {
            c = *s;
            if (c >= 0x829F && c < 0x82DE) {
                c += 0xA1;
            } else if (c >= 0x82DE && c <= 0x82F1) {
                c += 0xA2;
            }
            *d = c;
            s++;
            d++;
        }
        dst[i * 2] = 0;
    } else {
        strcpy(dst, src);
    }
}
#else
    for (i = 0; i < len; i++) {
        c = *s;
        if (c >= 0x829F && c < 0x82DE) {
            c += 0xA1;
        } else if (c >= 0x82DE && c <= 0x82F1) {
            c += 0xA2;
        }
        *d = c;
        s++;
        d++;
    }
    dst[i * 2] = 0;
}
#endif
#endif

#if VERSION <= VERSION_DEMO_US
/** The demo compares the item names as they are, without the kana conversion. */
static s32 comp_aiueo(const void* a, const void* b) {
#else
/** The demo compares the item names as they are, without the kana conversion. */
s32 comp_aiueo(const void* a, const void* b) {
    char bufA[0x80];
    char bufB[0x80];
#endif
    s16 ia = *(s16*)a;
    s16 ib = *(s16*)b;
#if VERSION <= VERSION_DEMO_US
    s32 r = strcmp(msgSearch(itemDataTable[ia].name), msgSearch(itemDataTable[ib].name));
    if (r > 0)
        return 1;
    return -(r < 0);
#else

    pouchAiueoConv(msgSearch(itemDataTable[ia].name), bufA);
    pouchAiueoConv(msgSearch(itemDataTable[ib].name), bufB);
    return strcmp(bufA, bufB);
#endif
}

/** qqsort comparator: items by type_sort_order from the item table, ascending (pouchSortItem modes 1 and 3). */
s32 comp_kind(s16* a, s16* b) {
    s16 x = itemDataTable[*a].type_sort_order;
    s16 y = itemDataTable[*b].type_sort_order;
    if (x > y) {
        return 1;
    }
    return -(x < y);
}

/** qqsort comparator: items by name, descending (pouchSortItem modes 7 and 9). */
#if VERSION <= VERSION_DEMO_US
/** qqsort comparator: items by name, descending (pouchSortItem modes 7 and 9). */
static s32 comp_aiueo_r(const void* a, const void* b) {
#else
/** qqsort comparator: items by name, descending (pouchSortItem modes 7 and 9). */
s32 comp_aiueo_r(const void* a, const void* b) {
    char bufA[0x80];
    char bufB[0x80];
#endif
    s16 ia = *(s16*)a;
    s16 ib = *(s16*)b;
#if VERSION <= VERSION_DEMO_US
    s32 r = strcmp(msgSearch(itemDataTable[ia].name), msgSearch(itemDataTable[ib].name));
    if (r < 0)
        return 1;
    return -(r > 0);
#else

    pouchAiueoConv(msgSearch(itemDataTable[ia].name), bufA);
    pouchAiueoConv(msgSearch(itemDataTable[ib].name), bufB);
    return strcmp(bufB, bufA);
#endif
}

/** qqsort comparator: items by type_sort_order, descending (pouchSortItem modes 8 and 10). */
s32 comp_kind_r(s16* a, s16* b) {
    s16 x = itemDataTable[*a].type_sort_order;
    s16 y = itemDataTable[*b].type_sort_order;
    if (x < y) {
        return 1;
    }
    return -(x > y);
}

/**
 * Sorts a pouch list for the item and badge menus (win_root.c).
 * Mode: 0/7 items by name ascending/descending, 1/8 items by kind, 2/9 and 3/10 key items by name and kind,
 * 4/11 badges by name, 5/12 badges by kind, 6/13 badges by BP cost then kind. Badges keep their worn state.
 * JP compares internal ids for badge names.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Sorts a pouch list for the item and badge menus (win_root.c).
 * Mode: 0/7 items by name ascending/descending, 1/8 items by kind, 2/9 and 3/10 key items by name and kind,
 * 4/11 badges by name, 5/12 badges by kind, 6/13 badges by BP cost then kind. Badges keep their worn state.
 * JP compares internal ids for badge names.
 */
void pouchSortItem(s32 mode) {
    s32 k;
    s16 items[MAX_HELD_ITEMS];
    s16 keys[MAX_KEY_ITEMS];
    s16 badges[MAX_BADGES];
    s16 equipped[MAX_BADGES];
    s32 n, i, j;
    BOOL swap;
    s16 t2, t1;
    PouchData* pp[1];
    pp[0] = mpp;
    if ((u32)mode <= 1 || mode == 7 || mode == 8) {
        n = 0;
        for (j = 0; j < MAX_HELD_ITEMS; j++) {
            if (pp[0]->heldItems[j] != 0) {
                items[n++] = pp[0]->heldItems[j];
            }
        }
        for (k = n; k < MAX_HELD_ITEMS; k++) {
            items[k] = 0;
        }
        if (mode == 0)
            qqsort(items, n, 2, comp_aiueo);
        else if (mode == 1)
            qqsort(items, n, 2, (QSortCompare)comp_kind);
        else if (mode == 7)
            qqsort(items, n, 2, comp_aiueo_r);
        else if (mode == 8)
            qqsort(items, n, 2, (QSortCompare)comp_kind_r);
        memcpy(pp[0]->heldItems, items, sizeof(items));
    }
    if ((u32)(mode - 2) <= 1 || mode == 9 || mode == 10) {
        n = 0;
        for (j = 0; j < MAX_KEY_ITEMS; j++) {
            if (pp[0]->keyItems[j] != 0) {
                keys[n++] = pp[0]->keyItems[j];
            }
        }
        for (k = n; k < MAX_KEY_ITEMS; k++) {
            keys[k] = 0;
        }
        if (mode == 2)
            qqsort(keys, n, 2, comp_aiueo);
        else if (mode == 3)
            qqsort(keys, n, 2, (QSortCompare)comp_kind);
        else if (mode == 9)
            qqsort(keys, n, 2, comp_aiueo_r);
        else if (mode == 10)
            qqsort(keys, n, 2, (QSortCompare)comp_kind_r);
        memcpy(pp[0]->keyItems, keys, sizeof(keys));
    }
    if ((u32)(mode - 4) <= 2 || (u32)(mode - 11) <= 1 || mode == 13) {
            i = 0;
            for (n = 0; n < MAX_BADGES; n++) {
                if (pp[0]->badges[n] != 0) {
                    badges[i] = pp[0]->badges[n];
                    i++;
                    equipped[i - 1] = pp[0]->equippedBadges[n];
                }
            }
            for (n = i; n < MAX_BADGES; n++) {
                badges[n] = 0;
                equipped[n] = 0;
            }
            for (n = 0; n < i - 1; n++) {
                for (j = n + 1; j < i; j++) {
                    ItemData* a = &itemDataTable[badges[n]];
                    ItemData* b = &itemDataTable[badges[j]];
                    s32 ca = a->bp_cost;
                    s32 cb = b->bp_cost;
                    const char* ida = a->id;
                    const char* idb = b->id;
                    swap = FALSE;
                    if (mode == 4) {
                        if (strcmp(ida, idb) > 0)
                            swap = TRUE;
                    } else if (mode == 11) {
                        if (strcmp(ida, idb) < 0)
                            swap = TRUE;
                    } else if (mode == 5) {
                        if (a->type_sort_order > b->type_sort_order)
                            swap = TRUE;
                    } else if (mode == 12) {
                        if (a->type_sort_order < b->type_sort_order)
                            swap = TRUE;
} else if (mode == 6) {
                        if (ca > cb)
                            swap = TRUE;
                    } else if (mode == 13) {
                        if (ca < cb)
                            swap = TRUE;
                    }
                    if (swap) {
                        t1 = badges[n];
                        t2 = equipped[n];
                        badges[n] = badges[j];
                        equipped[n] = equipped[j];
                        badges[j] = t1;
                        equipped[j] = t2;
                    }
                }
                memcpy(pp[0]->badges, badges, sizeof(badges));
                memcpy(pp[0]->equippedBadges, equipped, sizeof(equipped));
            }
}
}
#else
/**
 * Sorts a pouch list for the item and badge menus (win_root.c).
 * Mode: 0/7 items by name ascending/descending, 1/8 items by kind, 2/9 and 3/10 key items by name and kind,
 * 4/11 badges by name, 5/12 badges by kind, 6/13 badges by BP cost then kind. Badges keep their worn state.
 * JP compares internal ids for badge names.
 */
void pouchSortItem(s32 mode) {
    s32 k;
    s16 items[MAX_HELD_ITEMS];
    s16 keys[MAX_KEY_ITEMS];
    s16 badges[MAX_BADGES];
    s16 equipped[MAX_BADGES];
    s32 n, i, j;
    BOOL swap;
    s16 t2, t1;
    PouchData* pp[1];
#if VERSION == VERSION_EU
    char keyA[0x80];
    s16 bj;
    s32 l;
    s32 count;
    const char* ida;
    const char* idb;
    s32 ca;
    s32 cb;
    s16 bn;
    char keyB[0x80];
#elif VERSION >= VERSION_US
    s16 bn;
    s16 bj;
    const char* idb;
    s32 ca;
    s32 cb;
#endif

    pp[0] = mpp;
    if ((u32)mode <= 1 || mode == 7 || mode == 8) {
        n = 0;
        for (j = 0; j < MAX_HELD_ITEMS; j++) {
            if (pp[0]->heldItems[j] != 0) {
                items[n++] = pp[0]->heldItems[j];
            }
        }
        for (k = n; k < MAX_HELD_ITEMS; k++) {
            items[k] = 0;
        }
        if (mode == 0) {
            qqsort(items, n, sizeof(s16), comp_aiueo);
        } else if (mode == 1) {
            qqsort(items, n, sizeof(s16), (QSortCompare)comp_kind);
        } else if (mode == 7) {
            qqsort(items, n, sizeof(s16), comp_aiueo_r);
        } else if (mode == 8) {
            qqsort(items, n, sizeof(s16), (QSortCompare)comp_kind_r);
        }
        memcpy(pp[0]->heldItems, items, sizeof(items));
    }
    if ((u32)(mode - 2) <= 1 || mode == 9 || mode == 10) {
        n = 0;
        for (j = 0; j < MAX_KEY_ITEMS; j++) {
            if (pp[0]->keyItems[j] != 0) {
                keys[n++] = pp[0]->keyItems[j];
            }
        }
        for (k = n; k < MAX_KEY_ITEMS; k++) {
            keys[k] = 0;
        }
        if (mode == 2) {
            qqsort(keys, n, sizeof(s16), comp_aiueo);
        } else if (mode == 3) {
            qqsort(keys, n, sizeof(s16), (QSortCompare)comp_kind);
        } else if (mode == 9) {
            qqsort(keys, n, sizeof(s16), comp_aiueo_r);
        } else if (mode == 10) {
            qqsort(keys, n, sizeof(s16), (QSortCompare)comp_kind_r);
        }
        memcpy(pp[0]->keyItems, keys, sizeof(keys));
    }
#if VERSION == VERSION_EU
    // PAL sorts badge names by the pouchHiraToKata key
    if ((u32)(mode - 4) <= 2 || (u32)(mode - 11) <= 1 || mode == 13) {
        count = 0;
        for (n = 0; n < MAX_BADGES; n++) {
            if (pp[0]->badges[n] != 0) {
                badges[count] = pp[0]->badges[n];
                count++;
                equipped[count - 1] = pp[0]->equippedBadges[n];
            }
        }
        for (n = count; n < MAX_BADGES; n++) {
            badges[n] = 0;
            equipped[n] = 0;
        }
        for (j = 0; j < count - 1; j++) {
            for (l = j + 1; l < count; l++) {
                BOOL swap;
                bn = badges[j];
                bj = badges[l];
                ida = msgSearch(itemDataTable[bn].name);
                idb = msgSearch(itemDataTable[bj].name);
                ca = itemDataTable[bn].bp_cost;
                cb = itemDataTable[bj].bp_cost;
                swap = FALSE;
                pouchHiraToKata(ida, keyA);
                pouchHiraToKata(idb, keyB);
                ida = keyA;
                idb = keyB;
                if (mode == 4) {
                    if (strcmp(ida, idb) > 0) {
                        swap = TRUE;
                    }
                } else if (mode == 11) {
                    if (strcmp(ida, idb) < 0) {
                        swap = TRUE;
                    }
                } else if (mode == 5) {
                    if (itemDataTable[bn].type_sort_order > itemDataTable[bj].type_sort_order) {
                        swap = TRUE;
                    }
                } else if (mode == 12) {
                    if (itemDataTable[bn].type_sort_order < itemDataTable[bj].type_sort_order) {
                        swap = TRUE;
                    }
                } else if (mode == 6) {
                    if (ca > cb) {
                        swap = TRUE;
                    } else if (ca == cb && itemDataTable[bn].type_sort_order > itemDataTable[bj].type_sort_order) {
                        swap = TRUE;
                    }
                } else if (mode == 13) {
                    if (ca < cb) {
                        swap = TRUE;
                    } else if (ca == cb && itemDataTable[bn].type_sort_order < itemDataTable[bj].type_sort_order) {
                        swap = TRUE;
                    }
                }
                if (swap) {
                    t1 = badges[j];
                    t2 = equipped[j];
                    badges[j] = badges[l];
                    equipped[j] = equipped[l];
                    badges[l] = t1;
                    equipped[l] = t2;
#else
    if ((u32)(mode - 4) <= 2 || (u32)(mode - 11) <= 1 || mode == 13) {
#if VERSION >= VERSION_US
        /* FAKE: block-scoped (shadowing j/i) locals and separate sort counter m steer register colouring */
        ItemData* a;
        ItemData* b;
        s32 m;
        s32 j;
        s32 i;
        const char* ida;

#endif
        i = 0;
        for (n = 0; n < MAX_BADGES; n++) {
            if (pp[0]->badges[n] != 0) {
                badges[i] = pp[0]->badges[n];
                i++;
                equipped[i - 1] = pp[0]->equippedBadges[n];
            }
        }
        for (n = i; n < MAX_BADGES; n++) {
            badges[n] = 0;
            equipped[n] = 0;
        }
#if VERSION >= VERSION_US
        for (m = 0; m < i - 1; m++) {
            for (j = m + 1; j < i; j++) {
                BOOL swap;
                bn = badges[m];
                bj = badges[j];
                a = &itemDataTable[bn];
                ida = msgSearch(a->name);
                b = &itemDataTable[bj];
                idb = msgSearch(b->name);
                ca = a->bp_cost;
                cb = b->bp_cost;
#else
        for (n = 0; n < i - 1; n++) {
            for (j = n + 1; j < i; j++) {
                ItemData* a = &itemDataTable[badges[n]];
                ItemData* b = &itemDataTable[badges[j]];
                s32 ca = a->bp_cost;
                s32 cb = b->bp_cost;
                const char* ida = a->id;
                const char* idb = b->id;
#endif
                swap = FALSE;
                if (mode == 4) {
                    if (strcmp(ida, idb) > 0) {
                        swap = TRUE;
                    }
                } else if (mode == 11) {
                    if (strcmp(ida, idb) < 0) {
                        swap = TRUE;
                    }
                } else if (mode == 5) {
                    if (a->type_sort_order > b->type_sort_order) {
                        swap = TRUE;
                    }
                } else if (mode == 12) {
                    if (a->type_sort_order < b->type_sort_order) {
                        swap = TRUE;
                    }
                } else if (mode == 6) {
                    if (ca > cb) {
                        swap = TRUE;
                    } else if (ca == cb && a->type_sort_order > b->type_sort_order) {
                        swap = TRUE;
                    }
                } else if (mode == 13) {
                    if (ca < cb) {
                        swap = TRUE;
                    } else if (ca == cb && a->type_sort_order < b->type_sort_order) {
                        swap = TRUE;
                    }
                }
                if (swap) {
#if VERSION >= VERSION_US
                    t1 = badges[m];
                    t2 = equipped[m];
                    badges[m] = badges[j];
                    equipped[m] = equipped[j];
#else
                    t1 = badges[n];
                    t2 = equipped[n];
                    badges[n] = badges[j];
                    equipped[n] = equipped[j];
#endif
                    badges[j] = t1;
                    equipped[j] = t2;
#endif
                }
            }
        }
        memcpy(pp[0]->badges, badges, sizeof(badges));
        memcpy(pp[0]->equippedBadges, equipped, sizeof(equipped));
    }
}
#endif

/** Mario's coins. */
s32 pouchGetCoin(void) {
    return mpp->coins;
}

/** Adds coins (negative to pay), clamped to 0..999; returns the new count. */
s32 pouchAddCoin(s32 coins) {
    mpp->coins += coins;
    if (mpp->coins < 0) {
        mpp->coins = 0;
    }
    if (mpp->coins > 999) {
        mpp->coins = 999;
    }
    return mpp->coins;
}

/** Sets Mario's coins, clamped to 0..999; returns the new count. */
s32 pouchSetCoin(s32 coins) {
    mpp->coins = coins;
    if (mpp->coins < 0) {
        mpp->coins = 0;
    }
    if (mpp->coins > 999) {
        mpp->coins = 999;
    }
    return mpp->coins;
}

/** Number of Shine Sprites Mario carries (super coin: the Japanese name). */
s32 pouchGetSuperCoin(void) {
    return mpp->shineSpriteCount;
}

/** Sets the Shine Sprite count (super coin: the Japanese name) and returns it. */
s32 pouchSetSuperCoin(s32 count) {
    mpp->shineSpriteCount = count;
    return mpp->shineSpriteCount;
}

/** Number of Star Pieces Mario carries. */
s32 pouchGetStarPiece(void) {
    return mpp->starPieceCount;
}

/** Adds Star Pieces (negative to spend) without a clamp; returns the new count. */
s32 pouchAddStarPiece(s32 count) {
    mpp->starPieceCount += count;
    return mpp->starPieceCount;
}

/** Adds HP to Mario (negative for damage), clamped to 0..maximumHP; returns the new HP. */
s16 pouchAddHP(s16 points) {
    mpp->currentHP += (s16)points;
    if (mpp->currentHP < 0) {
        mpp->currentHP = 0;
    }
    if (mpp->currentHP > mpp->maximumHP) {
        mpp->currentHP = mpp->maximumHP;
    }
    return mpp->currentHP;
}

/** Mario's current HP. */
s32 pouchGetHP(void) {
    return mpp->currentHP;
}

/** Max HP including HP Plus badges. */
s32 pouchGetMaxHP(void) {
    return mpp->maximumHP;
}

/** Sets current HP, clamped to the maximum (not to 0). */
void pouchSetHP(s32 points) {
    mpp->currentHP = points;
    if (mpp->currentHP > mpp->maximumHP) {
        mpp->currentHP = mpp->maximumHP;
    }
}

/** Sets max HP and the badge-free base max HP. */
void pouchSetMaxHP(s16 points) {
    mpp->maximumHP = points;
    mpp->baseMaxHP = mpp->maximumHP;
}

/** A partner's current HP (partyData is indexed by PartyId). */
s32 pouchGetPartyHP(PartyId partnerId) {
    return mpp->partyData[partnerId].currentHP;
}

/** Sets a partner's current HP, clamped to its maximum (not to 0). */
void pouchSetPartyHP(s32 partnerId, s32 points) {
    PouchPartyData* party;

    mpp->partyData[partnerId].currentHP = points;
    party = &mpp->partyData[partnerId];
    if (party->currentHP > party->maximumHP) {
        party->currentHP = party->maximumHP;
    }
}

/** Mario's current FP. */
s32 pouchGetFP(void) {
    return mpp->currentFP;
}

/** Max FP including FP Plus badges. */
s32 pouchGetMaxFP(void) {
    return mpp->maximumFP;
}

/** Sets current FP, clamped to the maximum (not to 0). */
void pouchSetFP(s32 points) {
    mpp->currentFP = points;
    if (mpp->currentFP > mpp->maximumFP) {
        mpp->currentFP = mpp->maximumFP;
    }
}

/** Sets max FP and the badge-free base max FP. */
void pouchSetMaxFP(s16 points) {
    mpp->maximumFP = points;
    mpp->baseMaxFP = mpp->maximumFP;
}

/** Current Star Power in hundredths of a gauge unit (AP: appeal points, from the audience). */
s32 pouchGetAP(void) {
    return mpp->currentSP;
}

/** Adds Star Power (negative to spend: a special move costs baseSpCost * 100), clamped to 0..maximumSP. */
void pouchAddAP(s32 points) {
    mpp->currentSP += points;
    if (mpp->currentSP < 0) {
        mpp->currentSP = 0;
    }
    if (mpp->currentSP > mpp->maximumSP) {
        mpp->currentSP = mpp->maximumSP;
    }
}

/** Sets Star Power, clamped to 0..maximumSP. */
void pouchSetAP(s32 points) {
    mpp->currentSP = points;
    if (mpp->currentSP < 0) {
        mpp->currentSP = 0;
    }
    if (mpp->currentSP > mpp->maximumSP) {
        mpp->currentSP = mpp->maximumSP;
    }
}

/** Star Power maximum in hundredths of a gauge unit (100 per Crystal Star). */
s32 pouchGetMaxAP(void) {
    return mpp->maximumSP;
}

/** Audience size carried between battles; the next battle's starting crowd is drawn around it. */
f32 pouchGetAudienceNum(void) {
    return mpp->audienceCount;
}

/** Saves the battle audience size (battle_audience.c) so the next battle starts with about as many. */
void pouchSetAudienceNum(f32 num) {
    mpp->audienceCount = num;
}

/**
 * Jump level from the boots key items (0 none, 1 Boots, 2 Super Boots, 3 Ultra Boots);
 * also caches it in jumpLevel. Field objects check it (evt_mobj.c).
 */
s32 pouchGetJumpLv(void) {
    s32 level = 0;

    if (pouchCheckItem(ITEM_ULTRA_BOOTS)) {
        level = 3;
    } else if (pouchCheckItem(ITEM_SUPER_BOOTS)) {
        level = 2;
    } else if (pouchCheckItem(ITEM_BOOTS)) {
        level = 1;
    }
    mpp->jumpLevel = level;
    return mpp->jumpLevel;
}

/**
 * Hammer level from the hammer key items (0 none, 1 Hammer, 2 Super Hammer, 3 Ultra Hammer);
 * also caches it in hammerLevel.
 */
s32 pouchGetHammerLv(void) {
    s32 level = 0;

    if (pouchCheckItem(ITEM_ULTRA_HAMMER)) {
        level = 3;
    } else if (pouchCheckItem(ITEM_SUPER_HAMMER)) {
        level = 2;
    } else if (pouchCheckItem(ITEM_HAMMER)) {
        level = 1;
    }
    mpp->hammerLevel = level;
    return mpp->hammerLevel;
}

/**
 * Partner's attack level (number of rank-ups); the party menu shows attackLevel + 2 moves
 * and battle uses it to pick move power.
 */
s32 pouchGetPartyAttackLv(PartyId partyId) {
    // FAKE: &mpp->partyData[partyId] is formed by hand (same address) so the scaled index gets the field offset before the base is added (addi + lhax), as in the target
    return ((PouchPartyData*)((u8*)mpp + partyId * sizeof(PouchPartyData)))->attackLevel;
}

/** The demo stores into the first empty slot and removes by id alone, without shifting the list. */
BOOL pouchAddKeepItem(s32 id) {
    s32 i, j;

#if VERSION <= VERSION_DEMO_US
    for (i = 0; i < MAX_STORED_ITEMS; i++) {
#else
    for (i = MAX_STORED_ITEMS - 1; i >= 0; i--) {
#endif
        if (mpp->storedItems[i] == 0) {
#if VERSION <= VERSION_DEMO_US
            break;
#else
            for (j = i; j > 0; j--) {
                mpp->storedItems[j] = mpp->storedItems[j - 1];
#endif
            }
#if VERSION > VERSION_DEMO_US
            mpp->storedItems[0] = id;
            return TRUE;
#endif
        }
#if VERSION <= VERSION_DEMO_US
    if (i < MAX_STORED_ITEMS) {
        mpp->storedItems[i] = id;
        return TRUE;
#endif
    }
    return FALSE;
}

/** Takes the first copy of id out of storage, leaving an empty slot; FALSE if it is not stored. */
#if VERSION <= VERSION_DEMO_US
/** Takes the first copy of id out of storage, leaving an empty slot; FALSE if it is not stored. */
BOOL pouchRemoveKeepItem(s32 id) {
    s32 i;
#else
/** Takes the first copy of id out of storage, leaving an empty slot; FALSE if it is not stored. */
BOOL pouchRemoveKeepItem(s32 id, s32 index) {
    s32 i, j;
#endif

#if VERSION <= VERSION_DEMO_US
    for (i = 0; i < MAX_STORED_ITEMS; i++) {
#else
    if (mpp->storedItems[index] != id) {
        return FALSE;
    }
    for (i = index; i < MAX_STORED_ITEMS; i++) {
#endif
        if (mpp->storedItems[i] == id) {
#if VERSION <= VERSION_DEMO_US
            break;
#else
            mpp->storedItems[i] = 0;
            for (j = i; j < MAX_STORED_ITEMS - 1; j++) {
                mpp->storedItems[j] = mpp->storedItems[j + 1];
                mpp->storedItems[j + 1] = 0;
#endif
            }
#if VERSION > VERSION_DEMO_US
            return TRUE;
#endif
        }
#if VERSION <= VERSION_DEMO_US
    if (i < MAX_STORED_ITEMS) {
        mpp->storedItems[i] = 0;
        return TRUE;
#endif
    }
    return FALSE;
}

/**
 * Recomputes Mario's stats from his badges: max HP and FP are the base values plus 5 per HP Plus / FP Plus worn,
 * current HP and FP are clamped, and available BP is max BP minus the cost of every worn badge.
 * Runs after battle and after badge or item changes (badge menu, evt_pouch).
 */
void pouchReviseMarioParam(void) {
    int i;

    mpp->maximumHP = mpp->baseMaxHP;
    mpp->maximumFP = mpp->baseMaxFP;

    for (i = 0; i < MAX_BADGES; i++) {
        if (!mpp->equippedBadges[i]) {
            continue;
        }
        switch (mpp->equippedBadges[i]) {
            case ITEM_HP_PLUS:
                mpp->maximumHP += 5;
                break;
            case ITEM_FP_PLUS:
                mpp->maximumFP += 5;
                break;
        }
    }
    if (mpp->currentHP > mpp->maximumHP) {
        mpp->currentHP = mpp->maximumHP;
    }
    if (mpp->currentFP > mpp->maximumFP) {
        mpp->currentFP = mpp->maximumFP;
    }
    mpp->availableBP = mpp->maximumBP;

    for (i = 0; i < MAX_BADGES; i++) {
        mpp->availableBP -= itemDataTable[mpp->equippedBadges[i]].bp_cost;
    }
}

/**
 * Recomputes every partner's max HP: the _party_max_hp_table entry for its HP level plus 5 per HP Plus P worn,
 * clamping current HP down to it.
 */
void pouchRevisePartyParam(void) {
    PouchPartyData* party;
    int count, i;

    count = pouchEquipCheckBadge(ITEM_HP_PLUS_P);

    for (i = 0; i < PARTNER_COUNT; i++) {
        if (i != PARTNER_NONE) {
            party = &mpp->partyData[i];
            party->baseMaxHP = _party_max_hp_table[i][party->HPLevel];
            party->maximumHP = (s16)(party->baseMaxHP + (count * 5));
            if (party->currentHP > party->maximumHP) {
                party->currentHP = party->maximumHP;
            }
        }
    }
}

/** Mario's Star Points toward the next level, for the status window. */
s16 pouchGetStarPoint(void) {
    return mpp->starPoints;
}

/** Wears the badge in list slot index by copying it into the same slot of equippedBadges; FALSE for an empty slot. */
BOOL pouchEquipBadgeIndex(s32 index) {
    if (mpp->badges[index] == 0) {
        return FALSE;
    }
    mpp->equippedBadges[index] = mpp->badges[index];
    return TRUE;
}

/**
 * Takes off the badge in list slot index (badge menu); FALSE if that slot was not worn.
 * BP is recomputed later by pouchReviseMarioParam.
 */
BOOL pouchUnEquipBadgeIndex(s32 index) {
    if (mpp->equippedBadges[index] == 0) {
        return FALSE;
    }
    mpp->equippedBadges[index] = 0;
    return TRUE;
}

/** How many copies of a badge Mario is wearing (for stacking badges such as HP Plus P). */
s32 pouchEquipCheckBadge(s32 badgeId) {
    int i, count;

    count = 0;
    for (i = 0; i < MAX_BADGES; i++) {
        if (badgeId == mpp->equippedBadges[i]) {
            count++;
        }
    }
    return count;
}

/** TRUE if the badge in list slot index is worn (equippedBadges mirrors the badges list slot for slot). */
BOOL pouchEquipCheckBadgeIndex(s32 index) {
    return mpp->equippedBadges[index] != 0;
}

/**
 * Equips the first copy of a badge in the badge list; FALSE if Mario does not have it.
 * Used by the story-progress setup in event.c (Attack FX R) and the E3 demo sequence. Does not recompute BP.
 */
BOOL pouchEquipBadgeID(s32 badgeId) {
    int i;

    for (i = 0; i < MAX_BADGES; i++) {
        if (mpp->badges[i] == badgeId) {
            if (mpp->badges[i]) {
                mpp->equippedBadges[i] = mpp->badges[i];
            }
            return TRUE;
        }
    }
    return FALSE;
}

// Descriptive name (not in the symbol maps).
#if VERSION > VERSION_DEMO_US
/**
 * Unequips the first worn copy of a badge; FALSE if none is worn.
 * The Game Over sequence takes off Slow Go with it. Not in the US demo.
 */
BOOL pouchUnEquipBadgeID(s32 badgeId) {
    int i;

    for (i = 0; i < MAX_BADGES; i++) {
        if (mpp->equippedBadges[i] == badgeId) {
            if (mpp->equippedBadges[i]) {
                mpp->equippedBadges[i] = 0;
            }
            return TRUE;
        }
    }
    return FALSE;
}
#endif

/**
 * Puts a badge on sale at the badge shop (Lovely Howz)
 * when a dropped badge item carrying ITEM_FLAG_BADGE_RETURN goes away uncollected (itemdrv.c itemCheckBadge),
 * so it is not lost for good.
 * In retail those are the badges field enemies drop when beaten (enemy_common_dead_event).
 * Adds it to the special table outside battle; the demo adds it to the bargain table, in or out of battle.
 */
s32 pouchArriveBadge(s32 id) {
#if VERSION <= VERSION_DEMO_US
    return badgeShop_add(bdsw->bargain_table, (s16)id, 1);
#else
    return badgeShop_add(bdsw->special_table, (s16)id, 1);
#endif
}

/**
 * Script command evt_pouch_majinai_set: gives Merlee curseCount charm uses unless more are already left,
 * and restarts her countdown (majinai: charm).
 */
void pouchMajinaiInit(s32 curseCount) {
    if (mpp->merleeCurseUsesRemaining < curseCount) {
        mpp->merleeCurseUsesRemaining = (u8)curseCount;
        mpp->turnsUntilMerleeActivation = -1;
        mpp->merleeNextCurseType = 0;
    }
}

/** Coin counter of Bowser's side-scrolling stages (0-99). */
s32 pouchGetKpaCoin(void) {
    return mpp->superBowserCoins;
}

/**
 * Adds coins collected in Bowser's side-scrolling stages (kpa_act.c).
 * Every 100 coins wraps the counter and returns TRUE; the caller then adds a 1000-point bonus.
 */
BOOL pouchAddKpaCoin(s32 coins) {
    BOOL ret = FALSE;
    mpp->superBowserCoins += coins;
    if (mpp->superBowserCoins >= 100) {
        mpp->superBowserCoins -= 100;
        ret = TRUE;
    }
    return ret;
}

/** Score of Bowser's side-scrolling stages, shown by the kpa rel. */
u32 pouchGetKpaScore(void) {
    return mpp->superBowserScore;
}

/** Adds to the score of Bowser's side-scrolling stages (kpa_act.c), capped at 999999. */
void pouchAddKpaScore(u32 score) {
    mpp->superBowserScore += score;
    if (mpp->superBowserScore > 999999) {
        mpp->superBowserScore = 999999;
    }
}

/**
 * Gives Mario Crystal Star id (0-based; called by the story-progress setup in event.c):
 * sets its bit in starPowersObtained and raises the Star Power maximum to 100 per star (capped at 800).
 * The first star also sets the saved audience size to 2.
 */
void pouchGetStarStone(u32 id) {
    if (!mpp->starPowersObtained) {
        mpp->audienceCount = 2.0f;
    }
    mpp->starPowersObtained |= 1 << id;
    mpp->maximumSP = (s16)(100 * (id + 1));
    if (mpp->maximumSP < 0) {
        mpp->maximumSP = 0;
    }
    if (mpp->maximumSP > 800) {
        mpp->maximumSP = 800;
    }
}

/** Number of mails received so far (bits set among the first 99 of emailReceivedFlags). */
s32 pouchReceiveMailCount(void) {
    s32 i, count;

    for (count = 0, i = 0; i < 99; i++) {
        if (mpp->emailReceivedFlags[i / 32] & (1 << (i % 32))) {
            count++;
        }
    }
    return count;
}

/** Delivers a mail: sets its received bit and appends its id to emailIds in arrival order (the mail menu's list). */
void pouchReceiveMail(s32 mailId) {
    s32 count;
    s32 index, shift;

    index = mailId / 32;
    shift = mailId % 32;
    count = pouchReceiveMailCount();
    mpp->emailReceivedFlags[index] |= (1 << shift);
    mpp->emailIds[count] = (u8)mailId;
}

/** Marks a mail as read in emailReadFlags; the story-progress setup in event.c opens mails it pre-delivers. */
void pouchOpenMail(s32 mailId) {
    mpp->emailReadFlags[mailId / 32] |= (1 << (mailId % 32));
}

/** Mail state for the mail menu: 2 when read, 1 when received but unread, 0 when not received. */
u32 pouchCheckMail(s32 mailId) {
    s32 mask = (1 << (mailId % 32));
    if (mpp->emailReadFlags[mailId / 32] & mask) {
        return 2;
    } else {
        return (mpp->emailReceivedFlags[mailId / 32] & mask) != 0;
    }
}

/**
 * Partner colour from the top three bits of its flags;
 * Yoshi's colour picks his sprite pose table and his window and battle colours.
 */
s32 pouchGetPartyColor(PartyId partnerId) {
    return mpp->partyData[partnerId].flags >> 13;
}

/**
 * Stores a partner's colour in the top three bits of its flags;
 * only Yoshi uses it (set when the egg hatches, tou_10 in Glitzville, and by party.c).
 */
void pouchSetPartyColor(PartyId partnerId, s32 color) {
    mpp->partyData[partnerId].flags = (mpp->partyData[partnerId].flags & 0x1FFF) | (u16)(color << 13);
}

/**
 * Stores the name the player gave Yoshi when the egg hatched (evt_pouch script command);
 * the story-progress setup in event.c resets it to the default name.
 */
void pouchSetYoshiName(const char* name) {
    strcpy(mpp->yoshiName, name);
}

/** Yoshi's name for messages: the stored name, or the default from message name_party3 when it is empty. */
char* pouchGetYoshiName(void) {
    if (strlen(mpp->yoshiName) < 1) {
        return (char*)msgSearch("name_party3");
    }
    return mpp->yoshiName;
}

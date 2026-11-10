// Mario's pouch (save data): items, badges, coins, HP/FP, Star Points and level, partners, Star Pieces and
// Shine Sprites, the item and badge sorts.

#include "evt/evt_story.h"
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

// Unused 4-byte object, read by nothing; the original had it too (its storage is in the retail .sbss). Name invented.
PouchData* mpp_gap;
static PouchData* mpp;

/**
 * Max HP of each partner by partner id (row) and HP level
 * (column: levels 0 to 2, then 200 for the cap).
 */
#if VERSION <= VERSION_DEMO_JP
#if VERSION == VERSION_DEMO_JP
static s16 _party_max_hp_table[8][4] = {
#else
static s16 _party_max_hp_table[9][4] = {
#endif
#else
static s16 _party_max_hp_table[8][4] = {
#endif

    {0, 0, 0, 0},
    {10, 20, 30, 200},
    {10, 15, 25, 200},
    {20, 30, 40, 200},
    {10, 20, 30, 200},
    {15, 25, 35, 200},
    {15, 20, 30, 200},
#if VERSION <= VERSION_DEMO_JP
#if VERSION == VERSION_DEMO_JP
    {15, 20, 25, 200}
#else
    {15, 20, 25, 200},
    {0, 0, 0, 0},
#endif
#else
    {15, 20, 25, 200}
#endif
};

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
        if (i != PARTY_NONE) {
            party->flags = 0;
            party->baseMaxHP = 10;
            party->maximumHP = party->baseMaxHP;
            party->currentHP = party->baseMaxHP;
            party->attackLevel = 0;
            party->techLevel = 0;
        }
    }
#if VERSION >= VERSION_DEMO_EU
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

// JP and the demos call nothing here and strip it (JP demo map: UNUSED, 0x14, the same size as in US).
ItemType pouchEquipBadge(s32 id) {
    return mpp->equippedBadges[id];
}

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

/**
 * The list an item, key item or badge id belongs to: *table1 (Mario's items, key items or badges), *table2 (the
 * worn-badge list for badges, else NULL) and *maximum (its slot count); FALSE for other ids.
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
BOOL pouchGetMaxAndList(ItemType type, s32* maximum, s16** table1, s16** table2) {
    if (type >= ITEM_MIN && type < ITEM_MAX) {
        *table1 = mpp->heldItems;
        *table2 = NULL;
        *maximum = pouchCheckItem(ITEM_STRANGE_SACK) ? MAX_HELD_ITEMS : HELD_ITEM_COUNT_BASE;
        return TRUE;
    }
    if (type >= KEY_ITEM_MIN && type < KEY_ITEM_MAX) {
        *table1 = mpp->keyItems;
        *table2 = NULL;
        *maximum = MAX_KEY_ITEMS;
        return TRUE;
    }
    if (type >= BADGE_MIN && type < BADGE_MAX) {
        *table1 = mpp->badges;
        *table2 = mpp->equippedBadges;
        *maximum = MAX_BADGES;
        return TRUE;
    }
    return FALSE;
}

// Puts type at the front of its list (pouchGetMaxAndList) if that has an empty slot: the entries above the last
// empty slot move down one place (the worn-badge list, if any, moves in step and gets 0 at the front).
// Returns 1, or 0 when the list is full or type is not a pouch item.
// Original name and place from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
// Stand-alone it compiles to the map's 0x298 bytes.
BOOL addHead(ItemType type) {
    s16* t1;
    s16* t2;
    s32 size;
    s32 j;

    size = 0;
    t1 = NULL;
    t2 = NULL;
    if (!pouchGetMaxAndList(type, &size, &t1, &t2)) {
        return 0;
    }
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

/** Whether type's list (pouchGetMaxAndList) has an empty slot; 0 for other ids. */
// Original name and size from the JP demo's link map (UNUSED there, 0xfc bytes): only pouchGetItemCheck
// calls it, and nothing calls that, so the linker strips both. Body from NWPlayer123's reconstruction; with it
// both compile to their map sizes (a counting loop also gives 0xfc but makes pouchGetItemCheck 0x15c).
BOOL addHeadCheck(ItemType type) {
    s16* t1;
    s16* t2;
    s32 size;
    s32 i;

    size = 0;
    t1 = NULL;
    t2 = NULL;
    if (!pouchGetMaxAndList(type, &size, &t1, &t2)) {
        return 0;
    }
    for (i = size - 1; i >= 0; i--) {
        if (t1[i] == 0) {
            return 1;
        }
    }
    return 0;
}

/** Puts type in the first empty slot of its list (worn-badge entry 0); 0 when full or not a pouch item. */
// Original name and size from the JP demo's link map (UNUSED there, 0x138 bytes): nothing calls it, so the
// linker strips it and the body cannot be checked beyond its size (this one compiles to it). Body from
// NWPlayer123's reconstruction.
BOOL addTail(ItemType type) {
    s16* t1;
    s16* t2;
    s32 size;
    s32 i;

    size = 0;
    t1 = NULL;
    t2 = NULL;
    if (!pouchGetMaxAndList(type, &size, &t1, &t2)) {
        return 0;
    }
    for (i = 0; i < size; i++) {
        if (t1[i] == 0) {
            t1[i] = type;
            if (t2) {
                t2[i] = 0;
            }
            return 1;
        }
    }
    return 0;
}

/**
 * Gives Mario an item (pickups, scripts, battle audience gifts).
 * Coins (max 999), Pianta tokens (max 99999; the early demos cap at 0x7FFFFFFF),
 * hearts (1 HP to Mario and the current partner) and flowers (1 FP to Mario),
 * Star Pieces and Shine Sprites only bump counters.
 * Boots and hammers set the jump/hammer level, badges set their found flag (GSWF id - 0x70),
 * and both join a list. Returns FALSE when the list is full or the id is not a pouch item.
 */
BOOL pouchGetItem(ItemType type) {
    s32 party;

    if (type == ITEM_COIN) {
        if (mpp->coins < 999) {
            mpp->coins++;
        }
        return 1;
    }
    if (type == ITEM_PIANTA_TOKEN) {
#if VERSION <= VERSION_DEMO_JP
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
        party = marioGetParty();
        if (mpp->currentHP < mpp->maximumHP) {
            mpp->currentHP++;
        }
        if (party != 0) {
            if (mpp->partyData[party].currentHP < mpp->partyData[party].maximumHP) {
                mpp->partyData[party].currentHP++;
            }
        }
        return 1;
    }
    if (type == ITEM_FLOWER) {
        if (mpp->currentFP < mpp->maximumFP) {
            mpp->currentFP++;
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
        swSet(type - BADGE_MIN + GSWF_BADGE_FOUND_BASE);
    }
    return addHead(type);
}

/** Whether pouchGetItem would accept type: counter items always, list items when their list has room. */
// Original name, place and size from the JP demo's link map (UNUSED there, 0x158 bytes): nothing calls it, so
// the linker strips it; this body (NWPlayer123's) compiles to that size.
BOOL pouchGetItemCheck(ItemType type) {
    if (type == ITEM_COIN) {
        return 1;
    }
    if (type == ITEM_PIANTA_TOKEN) {
        return 1;
    }
    if (type == ITEM_HEART) {
        return 1;
    }
    if (type == ITEM_FLOWER) {
        return 1;
    }
    if (type == ITEM_STAR_PIECE) {
        return 1;
    }
    if (type == ITEM_SHINE_SPRITE) {
        return 1;
    }
    return addHeadCheck(type);
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
    s32 maximum;
    int i;

    maximum = 0;
    table1 = NULL;
    table2 = NULL;
    if (!pouchGetMaxAndList(type, &maximum, &table1, &table2)) {
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

#if VERSION <= VERSION_DEMO_JP

/**
 * The early demos' pouchRemoveItemIndex shifts the second table wrongly:
 * it clears the slot it has just filled instead of the one it moved from (the early demos' own bug;
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
    s32 maximum;
    int i;

    maximum = 0;
    table1 = NULL;
    table2 = NULL;
    if (!pouchGetMaxAndList(type, &maximum, &table1, &table2)) {
        return FALSE;
    }
    p = &table1[index];
    if (type != *p) {
        return FALSE;
    }
    for (i = index; i < maximum; p++, i++) {
        if (type == *p) {
#if VERSION <= VERSION_DEMO_JP
            pouchShiftDemo(table1, table2, i, maximum);
#else
            pouchShift(table1, table2, i, maximum);
#endif
            return TRUE;
        }
    }
    return FALSE;
}

#if VERSION >= VERSION_DEMO_EU
char* strupr(char* str);
#endif

// Descriptive name (not in the symbol maps); placeholder unk_800d06b4, JP 0x800D06B4. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
void pouchHiraToKata(const char* src, char* dst) {
    u16 c;
    s32 len = strlen(src) / 2;
    u16* s = (u16*)src;
    u16* d = (u16*)dst;
    s32 i;
#if VERSION >= VERSION_US
    if (gp->language == LANGUAGE_JP) {
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
#if VERSION >= VERSION_DEMO_EU
        // PAL and the PAL demo: sort key of a Latin name: drops <...> message tags, upper-cases, folds accented vowels to
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
#endif
}
#endif

/**
 * Sort key of an item name for the alphabetical sorts.
 * the early demos, JP, US and the US September demo inline their own copy of the kana conversion here; PAL and the PAL demo
 * call pouchHiraToKata.
 */
// PAL and the PAL demo call pouchHiraToKata; the early demos, JP, US and the US September demo inline their own copy of the conversion
static inline void pouchAiueoConv(const char* src, char* dst) {
#if VERSION >= VERSION_DEMO_EU
    pouchHiraToKata(src, dst);
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
    if (gp->language == LANGUAGE_JP) {
        s = (u16*)src;
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
#endif
#endif
}

#if VERSION <= VERSION_DEMO_JP

/** The early demos compare the item names as they are, without the kana conversion. */
static s32 comp_aiueo(const void* a, const void* b) {
#else
static s32 comp_aiueo(const void* a, const void* b) {
    char bufA[0x80];
    char bufB[0x80];
#endif
    s16 ia = *(s16*)a;
    s16 ib = *(s16*)b;
#if VERSION <= VERSION_DEMO_JP
    s32 r = strcmp(msgSearch(itemDataTable[ia].name), msgSearch(itemDataTable[ib].name));
    if (r > 0) {
        return 1;
    }
    if (r < 0) {
        return -1;
    }
    return 0;
#else
    pouchAiueoConv(msgSearch(itemDataTable[ia].name), bufA);
    pouchAiueoConv(msgSearch(itemDataTable[ib].name), bufB);
    return strcmp(bufA, bufB);
#endif
}

/** qqsort comparator: items by type_sort_order from the item table, ascending (pouchSortItem modes 1 and 3). */
static s32 comp_kind(const void* paramA, const void* paramB) {
    const s16* a = paramA;
    const s16* b = paramB;
    s16 x = itemDataTable[*a].type_sort_order;
    s16 y = itemDataTable[*b].type_sort_order;
    if (x > y) {
        return 1;
    }
    if (x < y) {
        return -1;
    }
    return 0;
}

/** qqsort comparator: items by name, descending (pouchSortItem modes 7 and 9). */
#if VERSION <= VERSION_DEMO_JP
static s32 comp_aiueo_r(const void* a, const void* b) {
#else
static s32 comp_aiueo_r(const void* a, const void* b) {
    char bufA[0x80];
    char bufB[0x80];
#endif
    s16 ia = *(s16*)a;
    s16 ib = *(s16*)b;
#if VERSION <= VERSION_DEMO_JP
    s32 r = strcmp(msgSearch(itemDataTable[ia].name), msgSearch(itemDataTable[ib].name));
    if (r < 0) {
        return 1;
    }
    if (r > 0) {
        return -1;
    }
    return 0;
#else
    pouchAiueoConv(msgSearch(itemDataTable[ia].name), bufA);
    pouchAiueoConv(msgSearch(itemDataTable[ib].name), bufB);
    return strcmp(bufB, bufA);
#endif
}

/** qqsort comparator: items by type_sort_order, descending (pouchSortItem modes 8 and 10). */
static s32 comp_kind_r(const void* paramA, const void* paramB) {
    const s16* a = paramA;
    const s16* b = paramB;
    s16 x = itemDataTable[*a].type_sort_order;
    s16 y = itemDataTable[*b].type_sort_order;
    if (x < y) {
        return 1;
    }
    if (x > y) {
        return -1;
    }
    return 0;
}

/**
 * Sorts a pouch list for the item and badge menus (win_root.c).
 * Mode: 0/7 items by name ascending/descending, 1/8 items by kind, 2/9 and 3/10 key items by name and kind,
 * 4/11 badges by name, 5/12 badges by kind, 6/13 badges by BP cost (JP and later break ties by kind). Badges keep their worn state.
 * JP and the early demos compare internal ids for badge names.
 */
void pouchSortItem(s32 mode) {
    PouchData* pouch = pouchGetPtr();

    if (mode == 0 || mode == 1 || mode == 7 || mode == 8) {
        s16 items[MAX_HELD_ITEMS];
        s32 i, n = 0;

        for (i = 0; i < MAX_HELD_ITEMS; i++) {
            if (pouch->heldItems[i] != 0) {
                items[n++] = pouch->heldItems[i];
            }
        }
        for (i = n; i < MAX_HELD_ITEMS; i++) {
            items[i] = 0;
        }
        if (mode == 0) {
            qqsort(items, n, sizeof(s16), comp_aiueo);
        } else if (mode == 1) {
            qqsort(items, n, sizeof(s16), comp_kind);
        } else if (mode == 7) {
            qqsort(items, n, sizeof(s16), comp_aiueo_r);
        } else if (mode == 8) {
            qqsort(items, n, sizeof(s16), comp_kind_r);
        }
        memcpy(pouch->heldItems, items, sizeof(items));
    }

    if (mode == 2 || mode == 3 || mode == 9 || mode == 10) {
        s16 keys[MAX_KEY_ITEMS];
        s32 i, n = 0;

        for (i = 0; i < MAX_KEY_ITEMS; i++) {
            if (pouch->keyItems[i] != 0) {
                keys[n++] = pouch->keyItems[i];
            }
        }
        for (i = n; i < MAX_KEY_ITEMS; i++) {
            keys[i] = 0;
        }
        if (mode == 2) {
            qqsort(keys, n, sizeof(s16), comp_aiueo);
        } else if (mode == 3) {
            qqsort(keys, n, sizeof(s16), comp_kind);
        } else if (mode == 9) {
            qqsort(keys, n, sizeof(s16), comp_aiueo_r);
        } else if (mode == 10) {
            qqsort(keys, n, sizeof(s16), comp_kind_r);
        }
        memcpy(pouch->keyItems, keys, sizeof(keys));
    }

    // PAL and the PAL demo sort badge names by the pouchHiraToKata key
    if (mode == 4 || mode == 5 || mode == 6 || mode == 11 || mode == 12 || mode == 13) {
        s16 badges[MAX_BADGES];
        s16 equipped[MAX_BADGES];
#if VERSION >= VERSION_US && VERSION < VERSION_DEMO_EU
        ItemData* a;
        ItemData* b;
#endif
#if VERSION >= VERSION_DEMO_EU
        s32 i, j, n = 0;
        char keyA[0x80];
        char keyB[0x80];

        for (i = 0; i < MAX_BADGES; i++) {
            if (pouch->badges[i] != 0) {
                badges[n] = pouch->badges[i];
                equipped[n] = pouch->equippedBadges[i];
                n++;
            }
        }
        for (i = n; i < MAX_BADGES; i++) {
            badges[i] = 0;
            equipped[i] = 0;
        }
#else
        s32 i, j, n = 0, k;
#if VERSION >= VERSION_US
        const char* ida;
#endif

        for (k = 0; k < MAX_BADGES; k++) {
            if (pouch->badges[k] != 0) {
                badges[n] = pouch->badges[k];
                equipped[n] = pouch->equippedBadges[k];
                n++;
            }
        }
        for (k = n; k < MAX_BADGES; k++) {
            badges[k] = 0;
            equipped[k] = 0;
        }
#endif

        for (i = 0; i < n - 1; i++) {
            for (j = i + 1; j < n; j++) {
#if VERSION >= VERSION_DEMO_EU
                s16 bn = badges[i];
                s16 bj = badges[j];
                const char* ida = msgSearch(itemDataTable[bn].name);
                const char* idb = msgSearch(itemDataTable[bj].name);
                s32 ca = itemDataTable[bn].bp_cost;
                s32 cb = itemDataTable[bj].bp_cost;
                BOOL swap = FALSE;

                pouchHiraToKata(ida, keyA);
                pouchHiraToKata(idb, keyB);
                ida = keyA;
                idb = keyB;
#elif VERSION >= VERSION_US
                s16 bn, bj;
                const char* idb;
                s32 ca, cb;
                BOOL swap;

                bn = badges[i];
                bj = badges[j];
                a = &itemDataTable[bn];
                ida = msgSearch(a->name);
                b = &itemDataTable[bj];
                idb = msgSearch(b->name);
                ca = a->bp_cost;
                cb = b->bp_cost;
                swap = FALSE;
#else
                ItemData* a = &itemDataTable[badges[i]];
                ItemData* b = &itemDataTable[badges[j]];
                const char* ida = a->id;
                const char* idb = b->id;
                s32 ca = a->bp_cost;
                s32 cb = b->bp_cost;
                BOOL swap = FALSE;
#endif

                if (mode == 4) {
                    if (strcmp(ida, idb) > 0) {
                        swap = TRUE;
                    }
                } else if (mode == 11) {
                    if (strcmp(ida, idb) < 0) {
                        swap = TRUE;
                    }
                } else if (mode == 5) {
#if VERSION >= VERSION_DEMO_EU
                    if (itemDataTable[bn].type_sort_order > itemDataTable[bj].type_sort_order) {
#else
                    if (a->type_sort_order > b->type_sort_order) {
#endif
                        swap = TRUE;
                    }
                } else if (mode == 12) {
#if VERSION >= VERSION_DEMO_EU
                    if (itemDataTable[bn].type_sort_order < itemDataTable[bj].type_sort_order) {
#else
                    if (a->type_sort_order < b->type_sort_order) {
#endif
                        swap = TRUE;
                    }
                } else if (mode == 6) {
                    if (ca > cb) {
                        swap = TRUE;
#if VERSION >= VERSION_JP
#if VERSION >= VERSION_DEMO_EU
                    } else if (ca == cb && itemDataTable[bn].type_sort_order > itemDataTable[bj].type_sort_order) {
#else
                    } else if (ca == cb && a->type_sort_order > b->type_sort_order) {
#endif
                        swap = TRUE;
#endif
                    }
                } else if (mode == 13) {
                    if (ca < cb) {
                        swap = TRUE;
#if VERSION >= VERSION_JP
#if VERSION >= VERSION_DEMO_EU
                    } else if (ca == cb && itemDataTable[bn].type_sort_order < itemDataTable[bj].type_sort_order) {
#else
                    } else if (ca == cb && a->type_sort_order < b->type_sort_order) {
#endif
                        swap = TRUE;
#endif
                    }
                }

                if (swap) {
                    s16 t2, t1;
                    t1 = badges[i];
                    t2 = equipped[i];
                    badges[i] = badges[j];
                    equipped[i] = equipped[j];
                    badges[j] = t1;
                    equipped[j] = t2;
                }
            }
#if VERSION <= VERSION_DEMO_JP
            memcpy(pouch->badges, badges, sizeof(badges));
            memcpy(pouch->equippedBadges, equipped, sizeof(equipped));
#endif
        }
#if VERSION >= VERSION_JP
        memcpy(pouch->badges, badges, sizeof(badges));
        memcpy(pouch->equippedBadges, equipped, sizeof(equipped));
#endif
    }
}

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

/** A partner's current HP (partyData is indexed by s32). */
s32 pouchGetPartyHP(s32 partnerId) {
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
s32 pouchGetPartyAttackLv(s32 partyId) {
    PouchPartyData* pd = &mpp->partyData[partyId];

    return pd->attackLevel;
}

/**
 * Adds a stored item: the early demos fill the first empty slot; JP and later shift entries down and insert at
 * the front.
 * Returns FALSE if storage is full. Removal differs by version in pouchRemoveKeepItem below.
 */
BOOL pouchAddKeepItem(s32 id) {
    s32 i, j;

#if VERSION <= VERSION_DEMO_JP
    for (i = 0; i < MAX_STORED_ITEMS; i++) {
        if (mpp->storedItems[i] == 0) {
            break;
        }
    }
    if (i < MAX_STORED_ITEMS) {
        mpp->storedItems[i] = id;
        return TRUE;
#else
    for (i = MAX_STORED_ITEMS - 1; i >= 0; i--) {
        if (mpp->storedItems[i] == 0) {
            for (j = i; j > 0; j--) {
                mpp->storedItems[j] = mpp->storedItems[j - 1];
            }
            mpp->storedItems[0] = id;
            return TRUE;
        }
#endif
    }
    return FALSE;
}

/** Removes a stored item: the early demos clear the first matching id; JP and later require id at index and close the gap. FALSE if not found. */
#if VERSION <= VERSION_DEMO_JP
BOOL pouchRemoveKeepItem(s32 id) {
    s32 i;

    for (i = 0; i < MAX_STORED_ITEMS; i++) {
        if (mpp->storedItems[i] == id) {
            break;
        }
    }
    if (i < MAX_STORED_ITEMS) {
        mpp->storedItems[i] = 0;
        return TRUE;
#else
BOOL pouchRemoveKeepItem(s32 id, s32 index) {
    s32 i, j;

    if (mpp->storedItems[index] != id) {
        return FALSE;
    }
    for (i = index; i < MAX_STORED_ITEMS; i++) {
        if (mpp->storedItems[i] == id) {
            mpp->storedItems[i] = 0;
            for (j = i; j < MAX_STORED_ITEMS - 1; j++) {
                mpp->storedItems[j] = mpp->storedItems[j + 1];
                mpp->storedItems[j + 1] = 0;
            }
            return TRUE;
        }
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
        if (i != PARTY_NONE) {
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
#if VERSION >= VERSION_JP

/**
 * Unequips the first worn copy of a badge; FALSE if none is worn.
 * The Game Over sequence takes off Slow Go with it. Not in the early demos.
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
 * when a dropped badge item carrying ITEM_FLAG_LOST_BADGE_TO_SHOP goes away uncollected (itemdrv.c itemCheckBadge),
 * so it is not lost for good.
 * In JP and later those are the badges field enemies drop when beaten (enemy_common_dead_event).
 * JP and later add it to the special table, outside battle only; the early demos add it to the bargain table,
 * in or out of battle, for badges without that flag.
 */
s32 pouchArriveBadge(s32 id) {
#if VERSION <= VERSION_DEMO_JP
    return badgeShop_add(bdsw->bargain_table, (s16)id, 1);
#else
    return badgeShop_add(bdsw->special_table, (s16)id, 1);
#endif
}

/**
 * Script command evt_pouch_majinai_set: when fewer than curseCount charm uses remain,
 * sets that many uses and restarts her countdown (majinai: charm).
 */
void pouchMajinaiInit(s32 curseCount) {
    if (mpp->merleeCurseUsesRemaining < curseCount) {
        mpp->merleeCurseUsesRemaining = (u8)curseCount;
        mpp->turnsUntilMerleeActivation = -1;
        mpp->merleeNextCurseType = MAJINAI_NONE;
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
s32 pouchCheckMail(s32 mailId) {
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
s32 pouchGetPartyColor(s32 partnerId) {
    return mpp->partyData[partnerId].flags >> 13;
}

/**
 * Stores a partner's colour in the top three bits of its flags;
 * only Yoshi uses it (set when the egg hatches, tou_10 in Glitzville, and by party.c).
 */
void pouchSetPartyColor(s32 partnerId, s32 color) {
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

// winmgr.c: the window manager (WinMgrDesc, WinMgrEntry, winMgrEntry/Main) and the winMgrSelect* lists.

#pragma once

#include "driver/dispdrv.h"
#include "evt/evt_word.h"
#include <dolphin/gx.h>
#include <dolphin/types.h>

typedef struct WinMgrDesc WinMgrDesc;
typedef struct WinMgrEntry WinMgrEntry;

struct WinMgrDesc {
    s32 type; //0x0
    s32 dispType; //0x4
    s32 camId; //0x8, CameraId
    s32 x; //0xC, entry->x
    s32 y; //0x10, entry->y
    s32 width; //0x14, entry->width
    s32 height; //0x18, entry->height
    GXColor color; //0x1C
    void (*main)(WinMgrEntry* entry); //0x20
    void (*disp)(WinMgrEntry* entry); //0x24
};

// WinMgrEntry.flags (winMgrEntry, winMgrMain, winMgrSeq)
typedef enum WinMgrFlags {
    WINMGR_FLAG_ACTIVE = (1 << 0),      // 0x1, slot in use
    WINMGR_FLAG_VISIBLE = (1 << 1),     // 0x2, drawn
    WINMGR_FLAG_ANIMATING = (1 << 2),   // 0x4, opening or closing animation running
    WINMGR_FLAG_AUTO_DELETE = (1 << 3), // 0x8, winMgrCloseAutoDelete: freed once closed
} WinMgrFlags;

struct WinMgrEntry {
    s32 flags; //0x0, WinMgrFlags
    s32 state; //0x4
    s32 timer; //0x8
    s32 alpha; //0xC
    f32 scale; //0x10
    f32 rotation; //0x14
    s32 x; //0x18
    s32 y; //0x1C
    s32 width; //0x20
    s32 height; //0x24
    WinMgrDesc* description; //0x28
    void* param; //0x2C
    s32 priority; //0x30, gets cast to float for dispEntry
    const char* msg; //0x34
    s32 scroll; //0x38
    s32 lines; //0x3C
    u8 pad_40[0x44 - 0x40]; //0x40
};

typedef struct WinMgrWork {
    s32 numEntries; //0x0
    WinMgrEntry* entries; //0x4
} WinMgrWork;

// WinMgrSelectItem.flags
typedef enum WinMgrSelectItemFlags {
    SELECT_ITEM_GRAY = (1 << 0),      // 0x1, drawn grayed out
    SELECT_ITEM_DISABLED = (1 << 1),  // 0x2, cannot be chosen (error sound and help)
    SELECT_ITEM_OVER_MAX = (1 << 2),  // 0x4, level up: the stat would pass its maximum ("helperr2")
    SELECT_ITEM_EQUIPPED = (1 << 3),  // 0x8, equipped badge (lamp icon, US and later)
} WinMgrSelectItemFlags;

// WinMgrSelect.flags
typedef enum WinMgrSelectFlags {
    SELECT_CANCEL_OK = (1 << 8),      // 0x100, B cancels the selection
    SELECT_DONE = (1 << 12),          // 0x1000, the windows are closed: winMgrSelect returns the result
    SELECT_CANCELLED = (1 << 13),     // 0x2000, cancelled with B (result -1)
} WinMgrSelectFlags;

typedef struct WinMgrSelectItem {
    u16 flags; //0x0, WinMgrSelectItemFlags
    u16 id; //0x2
} WinMgrSelectItem;

// WinMgrSelect.type: which list winMgrSelectEntry fills, what winMgrSelectOther returns.
// A negative value is not one of these: it is a pointer to a -1-terminated item id list.
typedef enum WinMgrSelectType {
    SELECT_TYPE_KEY_ITEM = 0,              // pouchKeyItem without menu_skip_list
    SELECT_TYPE_ITEM = 1,                  // pouchHaveItem
    SELECT_TYPE_BADGE = 2,                 // pouchHaveBadge
    SELECT_TYPE_ITEM_SELL = 3,             // pouchHaveItem, sell_price column (evt_shop)
    SELECT_TYPE_ITEM_STORE = 4,            // pouchHaveItem to put away (evt_shop)
    SELECT_TYPE_ITEM_RETRIEVE = 5,         // pouchKeepItem (evt_shop)
    SELECT_TYPE_PARTY_RANK_UP = 6,         // partners; rank limit by GSWF_GOR_UP_ARROW_USED (gor_02)
    SELECT_TYPE_PARTY_SWAP_RANK_UP = 7,    // the same list, first step of the tik_11 swap
    SELECT_TYPE_PARTY_SWAP_RANK_DOWN = 8,  // partners other than GW(1), rank 0 refused (tik_11)
    SELECT_TYPE_STAT_SWAP_UP = 9,          // HP/FP/BP to raise (tik_11)
    SELECT_TYPE_STAT_SWAP_DOWN = 10,       // the stats other than GW(1), to lower (tik_11)
    SELECT_TYPE_BADGE_BUY = 11,            // bargain_table, buy_price (gor_01)
    SELECT_TYPE_BADGE_SELL = 12,           // pouchHaveBadge, sell_price (gor_01)
    SELECT_TYPE_BADGE_STAR_PIECE = 13,     // starmaniac_table, star_piece_price (tik_00)
    SELECT_TYPE_BADGE_CHARLIETON = 14,     // badge_bottakuru_table, buy_price * 12 / 10 (gor_01)
    SELECT_TYPE_BADGE_CHARLIETON_PIT = 15, // badge_bottakuru100_table, price by GSW_PIT_FLOOR (jon)
    SELECT_TYPE_BADGE_PIANTA_TOKEN = 16,   // bteresa_table, Pianta token icon (gor_03)
    SELECT_TYPE_INFORMANT_PERFECT = 17,    // jperfect_title entries, perfectRead (gor_02)
    SELECT_TYPE_INFORMANT_LUIGI = 18,      // Luigi's entries, luigiRead (gor)
} WinMgrSelectType;

typedef struct WinMgrSelect {
    u16 flags; //0x0, WinMgrSelectFlags
    u16 pad_2; //0x2
    s32 type; //0x4, WinMgrSelectType or an item list pointer
    s32 state; //0x8
    s32 cursor; //0xC
    s32 scrollTop; //0x10
    f32 cursorX; //0x14, hand cursor (icon 0x1F0) x, eased toward the selected row
    f32 cursorY; //0x18, hand cursor y
    f32 scrollY; //0x1C, list scroll offset, eased toward 24 * scrollTop
    s32 numDescriptors; //0x20
    s32 descriptorId[3]; //0x24
    WinMgrSelectItem* items; //0x30
    s32 numItems; //0x34
    s32 newItem; //0x38, item being picked up when the bag is full (row 0 above a divider; picking it returns -1)
} WinMgrSelect;

void winMgrInit(void);
void winMgrReInit(void);
void winMgrMain(void);
s32 winMgrEntry(WinMgrDesc* desc);
void winMgrSetParam(s32 id, void* param);
void winMgrOpen(s32 id);
void winMgrClose(s32 id);
void winMgrCloseAutoDelete(s32 id);
void winMgrDelete(s32 id);
BOOL winMgrAction(s32 id);
void winMgrSetSize(s32 id, s32 x, s32 y, s32 width, s32 height);
void winMgrSetPriority(s32 id, s32 priority);
WinMgrEntry* winMgrGetPtr(s32 id);
void winMgrHelpInit(WinMgrEntry* entry);
void winMgrHelpMain(WinMgrEntry* entry);
void winMgrHelpDraw(WinMgrEntry* entry);
WinMgrSelect* winMgrSelectEntry(EvtWord type, s32 a2, BOOL a3);
s32 winMgrSelect(WinMgrSelect* select);
s32 winMgrSelectOther(WinMgrSelect* select, struct EventEntry* evt);
void winMgrSelectDelete(WinMgrSelect* select);
s32 winMgrSelectGetCursor(WinMgrSelect* sel);

#include "evt/evt_cmd.h"

USER_FUNC(evt_win_party_lankdown_chk);
USER_FUNC(evt_win_party_lankup_chk);

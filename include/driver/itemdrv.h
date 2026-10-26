#pragma once

#include "driver/effdrv.h"
#include "evt/evt_word.h"
#include <dolphin/mtx.h>
#include <dolphin/os.h>

// ItemEntry.flags, named from itemdrv (itemMain, itemHitCheck, itemseq_*, the get sequence). 0x80 (tested, never
// set) is not named.
typedef enum ItemEntry_Flags {
    ITEM_FLAG_ACTIVE = (1 << 0),
    ITEM_FLAG_NO_TOUCH_GET = (1 << 1),        // 0x2, itemHitCheck ignores it (no pickup by touch)
    ITEM_FLAG_QUICK_GET = (1 << 2),           // 0x4, straight into the pouch without the get window
    ITEM_FLAG_BGM_SILENT = (1 << 3),          // 0x8, the get sequence dipped the BGM
    ITEM_FLAG_CUSTOM_BOUND = (1 << 4),        // 0x10, itemseq_Bound keeps the caller's bounce parameters
    ITEM_FLAG_COLLECT_GSWF = (1 << 5),        // 0x20, collectFlag is a GSWF, set when the item is collected
    ITEM_FLAG_COLLECT_LSWF = (1 << 6),        // 0x40, collectFlag is an LSWF, set when the item is collected
    ITEM_FLAG_LOST_BADGE_TO_SHOP = (1 << 8),  // 0x100, a badge that vanishes goes back to the shop (pouchArriveBadge)
    ITEM_FLAG_BOUND_NEXT = (1 << 9),          // 0x200, first bounce uses evt_item_set_bound_next_dir/_speed
    ITEM_FLAG_REPLACEABLE = (1 << 10),        // 0x400, may be deleted when all entries are in use
    ITEM_FLAG_NO_CEILING_HIT = (1 << 11),     // 0x800
    ITEM_FLAG_FORCE_GET = (1 << 12),          // 0x1000, itemForceGet
    ITEM_FLAG_GET_BGM_ENDED = (1 << 13),      // 0x2000, the get fanfare finished
    ITEM_FLAG_NOKO_GET = (1 << 14),           // 0x4000, itemNokoForceGet (fetched by Koops' shell)
    ITEM_FLAG_NO_GET_BGM = (1 << 15),         // 0x8000, no get fanfare
} ItemEntry_Flags;

// ItemEntry.flags2. 0x2 (tested in evt_item_get_item, never set) and 0x10000 are not named.
typedef enum ItemEntry_Flags2 {
    ITEM_FLAG2_IN_POUCH = (1 << 0),             // 0x1, the item went into the pouch
    ITEM_FLAG2_NO_PICKUP_RISING = (1 << 2),     // 0x4, not collectible while rising
    ITEM_FLAG2_NO_PICKUP_UNTIL_BOUND = (1 << 3), // 0x8, not collectible before the first bounce
    ITEM_FLAG2_SKIP_BOUND_INIT = (1 << 4),      // 0x10, one-shot: the next bounce keeps its state
    ITEM_FLAG2_POP_VANISH = (1 << 5),           // 0x20, pops up once and disappears
    ITEM_FLAG2_SPREAD = (1 << 6),               // 0x40, scattered like lost coins
    ITEM_FLAG2_HIDDEN = (1 << 8),               // 0x100, not drawn (blinks in the last 3000 ms)
    ITEM_FLAG2_CARRIED = (1 << 12),             // 0x1000, carried by the partner
    ITEM_FLAG2_UPGRADE_GET = (1 << 13),         // 0x2000, boots/hammer upgrade get sequence
    ITEM_FLAG2_TIMER_WAIT_LAND = (1 << 14),     // 0x4000, the despawn timer starts once it has landed
    ITEM_FLAG2_NO_Z_MOVE = (1 << 15),           // 0x8000
    ITEM_FLAG2_RELEASED_UNUSED = (1 << 16),     // 0x10000, thrown out of a full pouch (msg_window_item_release), cleared when taken; nothing reads it
    ITEM_FLAG2_SUNK = (1 << 20),                // 0x100000
    ITEM_FLAG2_CTRL_OFF = (1 << 24),            // 0x1000000
    ITEM_FLAG2_NO_PARTNER = (1 << 25),          // 0x2000000
    ITEM_FLAG2_SAVED_LOST_BADGE = (1 << 26),    // 0x4000000, its badge was already sent back to the shop
} ItemEntry_Flags2;

// itemEntry's type argument (ItemEntry.type): an ItemEntryMode plus a despawn timer (ms) and extra flags
typedef enum ItemEntryType {
    ITEM_TYPE_GROUND = 0,
    ITEM_TYPE_GROUND_NO_TOUCH = 1,          // + ITEM_FLAG_NO_TOUCH_GET
    ITEM_TYPE_SCATTER = 2,
    ITEM_TYPE_SCATTER_TIMED = 3,            // vanishes after 5000 ms
    ITEM_TYPE_SCATTER_NO_PICKUP = 4,
    ITEM_TYPE_SCATTER_NO_PICKUP_TIMED = 5,  // vanishes after 5000 ms
    ITEM_TYPE_SPREAD_TIMED = 6,             // + ITEM_FLAG2_SPREAD, 5000 ms
    ITEM_TYPE_SCATTER_NO_PICKUP_TIMED_LAND = 7, // 5000 ms counted from landing
    ITEM_TYPE_DROP = 8,                     // enemy and coin drops: 8000 ms from landing
    ITEM_TYPE_DROP_LOST_BADGE = 9,          // + ITEM_FLAG_LOST_BADGE_TO_SHOP
    ITEM_TYPE_DROP_REPLACEABLE = 10,        // + ITEM_FLAG_REPLACEABLE
    ITEM_TYPE_DROP_LONG = 11,               // recovered items: 20000 ms from landing
    ITEM_TYPE_DROP_LONG_REPLACEABLE = 12,
    ITEM_TYPE_FALL = 13,
    ITEM_TYPE_FALL_NO_PICKUP = 14,
    ITEM_TYPE_POP = 15,
    ITEM_TYPE_STILL = 16,
    ITEM_TYPE_STILL_NO_TOUCH = 17,          // + ITEM_FLAG_NO_TOUCH_GET
} ItemEntryType;

// ItemEntry.mode (itemModeChange); descriptive names from itemMain / itemseq_Bound
typedef enum ItemEntryMode {
    ITEM_MODE_GROUND = 1, // follows the ground, collectible
    ITEM_MODE_GET = 2, // being collected (itemseq_GetItem)
    ITEM_MODE_FALL = 3, // falls and bounces, collectible
    ITEM_MODE_FALL_NO_PICKUP = 4, // as FALL, not collectible before the first bounce
    ITEM_MODE_SCATTER = 5, // thrown up and scattered, collectible
    ITEM_MODE_SCATTER_NO_PICKUP = 6, // as SCATTER, not collectible while rising
    ITEM_MODE_STILL = 7, // stays in place, collectible
    ITEM_MODE_CARRIED = 8, // carried by the partner
    ITEM_MODE_DELETE = 9, // deleted on the next update
    ITEM_MODE_POP = 10, // pops up once and disappears
} ItemEntryMode;

typedef struct ItemEntry {
    u16 flags; //0x0
    u16 type; //0x2, ItemEntryType
    s32 itemId; //0x4
    s32 collectFlag; //0x8, saved flag set when the item is collected (-1: none)
    char name[0x10]; //0xC
    EffectEntry* effect; //0x1C
    EvtWord* evtCode; //0x20, event script run on pickup (evtEntry)
    u16 mode; //0x24, ItemEntryMode
    u16 subState; //0x26, per-sequence state
    s32 winId[3]; //0x28
    u8 pad_34[0x38 - 0x34]; //0x34, never accessed
    u32 flags2; //0x38, ItemEntry_Flags2
    Vec position; //0x3C
    f32 scale; //0x48
    u8 alpha; //0x4C
    u8 pad_4D[0x50 - 0x4D]; //0x4D, never accessed
    f32 speed; //0x50
    s32 direction; //0x54
    f32 velocityY; //0x58
    u8 pad_5C[0x60 - 0x5C]; //0x5C, never accessed
    s64 airTime; //0x60
    OSTime baseTime; //0x68, timestamp elapsed time is measured from
    OSTime curTime; //0x70
    s64 timer; //0x78
    f32 bounceFactor; //0x80
    f32 gravityScale; //0x84
    s32 maxBounces; //0x88
    s32 bounceCount; //0x8C
    s32 savedDirection; //0x90
    f32 savedSpeed; //0x94
} ItemEntry;

typedef struct ItemWork {
    s32 count; //0x0
    ItemEntry* entries; //0x4
    u32 currId; //0x8, next available itemId
    EffectEntry* effect; //0xC
    struct WinMgrSelect* select; //0x10
    struct EventEntry* lectureEvt; //0x14
    s32 msgWindowId; //0x18
} ItemWork;

void itemInit(void);
void itemReInit(void);
void itemMain(void);
ItemEntry* itemNameToPtr(char* name);

void itemNokoForceGet(ItemEntry* entry);
void itemPickUp(ItemEntry* entry);

void itemDelete(const char* name);
void itemFlagOn(ItemEntry* entry, u16 flags);
void itemModeChange(ItemEntry* entry, s32 mode);
BOOL N_itemPickUpFromFieldCheck(void);
u16 itemFlag(ItemEntry* entry, u16 flags);
void itemFlagOff(ItemEntry* entry, u16 flags);
void itemSetPosition(ItemEntry* entry, f32 x, f32 y, f32 z);
u32 itemStatus(ItemEntry* entry, u32 flags);
void itemForceGet(ItemEntry* entry);
ItemEntry* itemHitCheck(f32 x, f32 y, f32 z, f32 range);
ItemEntry* itemNearDistCheck(f32 x, f32 y, f32 z, f32 range);
void itemStatusOn(ItemEntry* entry, u32 flags);
void itemCoinDrop(Vec* position);

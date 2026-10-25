#pragma once

#include "driver/effdrv.h"
#include "evt/evt_word.h"
#include <dolphin/mtx.h>
#include <dolphin/os.h>

typedef enum ItemEntry_Flags {
    ITEM_FLAG_ACTIVE = (1 << 0),
    ITEM_FLAG_COLLECT_GSWF = (1 << 5), // collectFlag is a GSWF, set when the item is collected
    ITEM_FLAG_COLLECT_LSWF = (1 << 6), // collectFlag is an LSWF, set when the item is collected
} ItemEntry_Flags;

typedef struct ItemEntry {
    u16 flags; //0x0
    u16 type; //0x2, entry type (itemEntry arg)
    s32 itemId; //0x4
    s32 collectFlag; //0x8, saved flag set when the item is collected (-1: none)
    char name[0x10]; //0xC
    EffectEntry* effect; //0x1C
    EvtWord evtCode; //0x20, event script run on pickup (evtEntry)
    u16 mode; //0x24
    u16 subState; //0x26, per-sequence state
    s32 winId[3]; //0x28
    u8 pad_34[0x38 - 0x34]; //0x34, never accessed
    u32 flags2; //0x38
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

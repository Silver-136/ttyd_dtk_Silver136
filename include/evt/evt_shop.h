#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"

typedef struct ShopObj {
    const char* mobj; //0x0
    const char* hit; //0x4
} ShopObj;

typedef struct ShopItem {
    s32 id; //0x0
    s32 price; //0x4
} ShopItem;

typedef struct ShopperData {
    const char* name; //0x0
    void* fook_evt; //0x4
    void* buy_evt; //0x8
    const char* msgs[1]; //0xC
} ShopperData;

typedef struct ShopPoint {
    s32 pts; //0x0
    s32 item; //0x4
} ShopPoint;

typedef struct ShopDb {
    s32 id; //0x0
    s16 price; //0x4
    s16 save; //0x6
} ShopDb;

typedef struct ShopWork {
    u16 flags; //0x0
    ShopObj* objs; //0x4
    ShopItem* items; //0x8
    ShopperData* shopper; //0xC
    ShopDb* db; //0x10
    u16 itemFlags[6]; //0x14
    s32 count; //0x20
    EventEntry* mainEvt; //0x24
    EventEntry* buyEvt; //0x28
    s32 cur; //0x2C
    u8 pad_30[0x38 - 0x30]; //0x30, never accessed
    s32 points; //0x38
    u8 pad_3C[0x50 - 0x3C]; //0x3C, never accessed
    s32 win[3]; //0x50
    u8 pad_5C[0xD0 - 0x5C]; //0x5C, never accessed
    s32 sel; //0xD0
    s32 buyItem; //0xD4
} ShopWork;

USER_FUNC(evt_shop_setup);

BOOL evtShopIsActive(void);

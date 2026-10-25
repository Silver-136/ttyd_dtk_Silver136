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

struct WinMgrEntry {
    s32 flags; //0x0
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
    s32 mNumEntries; //0x0
    WinMgrEntry* mEntries; //0x4
} WinMgrWork;

typedef struct WinMgrSelectItem {
    u16 flags; //0x0
    u16 id; //0x2
} WinMgrSelectItem;

typedef struct WinMgrSelect {
    u16 flags; //0x0
    u16 pad_2; //0x2
    s32 type; //0x4
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

#pragma once

#include <dolphin/ar.h>
#include <dolphin/arq.h>

typedef struct ARAMEntry {
    vu16 flags; //0x0
    u8 pad_0x2[2]; //0x2
    ARQRequest request; //0x4
    u32 size; //0x24
    u32 aramAddr; //0x28
    void* mramAddr; //0x2C
    void* dest; //0x30
    struct ARAMEntry* next; //0x34
} ARAMEntry;

typedef struct ARAMWork {
    u32 arStack[8]; //0x0
    u32 nextAddr; //0x20
    u32 baseAddr; //0x24
    u32 size; //0x28
    ARAMEntry* entries; //0x2C
    s32 count; //0x30
    ARAMEntry* head; //0x34
    ARAMEntry* tail; //0x38
    ARQRequest request; //0x3C
    volatile s32 busy; //0x5C
    void* buffer; //0x60
    u32 bufferSize; //0x64
} ARAMWork;

void aramMgrInit(void);
void aramMgrGarbage(void);
ARAMEntry* aramMgrToAramAsync(void* src, u32 size);
ARAMEntry* aramMgrToAram(void* src, u32 size);
void aramMgrToMram(ARAMEntry* entry, void* dest);

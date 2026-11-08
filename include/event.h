// event.c: the story event tables (EventData, EventStage): per-chapter progress points.

#pragma once

#include "version.h"
#include <dolphin/types.h>

typedef struct EventData {
    u8 motionType;           // 0x0
    u8 partners[2];          // 0x1
    u16 id;                  // 0x4
    const char* textId;      // 0x8
    const char* nameJp;      // 0xC
#if VERSION >= VERSION_US
    const char* nameEn;      // 0x10
#endif
    const char* mapName;     // 0x10 (0x14 from US on)
    const char* beroName;    // 0x14 (0x18 from US on)
    void (*initEvent)(void); // 0x18 (0x1C from US on)
} EventData;

typedef struct EventStage {
    const char* name; // 0x0
#if VERSION >= VERSION_US
    const char* nameEn; // 0x4
#endif
    EventData* data;  // 0x4 (0x8 from US on)
    s32 count;        // 0x8 (0xC from US on)
} EventStage;

void stg0_00_init(void);

void stg9_02_init(void);

EventStage* eventStgDtPtr(s32 stage);
s32 eventStgNum(void);

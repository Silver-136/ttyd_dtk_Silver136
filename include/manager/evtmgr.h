#pragma once

#include <dolphin/types.h>

typedef struct EventEntry {
    u8 unk0[0x18 - 0x0]; //0x0
    s32* args; //0x18
    u8 unk1C[0x1B0 - 0x1C]; //0x1C
} EventEntry;
STATIC_ASSERT_SIZEOF(EventEntry, 0x1B0);

void evtmgrReInit(void);
void evtDeleteID(s32 eventId);
BOOL evtCheckID(s32 eventId);
void* evtEntry(void* script, s32 priority, s32 flags);

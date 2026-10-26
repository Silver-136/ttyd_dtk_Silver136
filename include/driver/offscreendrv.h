#pragma once

#include <dolphin/types.h>
#include <dolphin/gx.h>
#include "memory.h"

// OffscreenEntry.flags
typedef enum OffscreenFlags {
    OFFSCREEN_FLAG_ACTIVE = (1 << 0),  // 0x1, slot in use
    OFFSCREEN_FLAG_BOX = (1 << 1),     // 0x2, a bounding box was added this frame
    OFFSCREEN_FLAG_COPIED = (1 << 2),  // 0x4, the texture was copied this frame
} OffscreenFlags;

typedef struct OffscreenEntry {
    s32 flags; //0x0, OffscreenFlags
    char name[16]; //0x4
    u16 currLeft; //0x14
    u16 currTop; //0x16
    u16 currRight; //0x18
    u16 currBottom; //0x1A
    GXTexObj texObj; //0x1C
    s32 copyFmt; //0x3C, GXSetTexCopyDst format (default 5)
    s32 texFmt; //0x40, GXInitTexObj format (default 5)
    smartEntry* texBuf; //0x44
    u16 prevLeft; //0x48
    u16 prevTop; //0x4A
    u16 prevRight; //0x4C
    u16 prevBottom; //0x4E
} OffscreenEntry;

typedef struct OffscreenWork {
    s32 count; //0x0
    OffscreenEntry* entries; //0x4
} OffscreenWork;

void offscreenInit(void);
void offscreenReset(BOOL inBattle);
s32 offscreenEntry(const char* name);
s32 offscreenNameToId(const char* name);
OffscreenEntry* offscreenNameToPtr(const char* name);
void offscreenDelete(const char* name);
BOOL offscreenGetBoundingBox_BeforeFrame(s32 id, u16* left, u16* top, u16* right, u16* bottom);
void offscreenGetTexObj(s32 id, GXTexObj** texObj, smartEntry** texBuf);
BOOL offscreenGetBoundingBox(s32 id, u16* left, u16* top, u16* right, u16* bottom);
void offscreenMain(void);
void offscreenAddBoundingBox(s32 id, u16 left, u16 top, u16 right, u16 bottom);

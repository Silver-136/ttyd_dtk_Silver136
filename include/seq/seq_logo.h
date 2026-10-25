#pragma once

#include "driver/seqdrv.h"
#include "manager/filemgr.h"
#include <dolphin/os.h>

#ifdef VERSION_G8ME01
typedef struct LogoWork {
    FileEntry* texture; //0x0
    s32 page; //0x4
    OSTime lastInput; //0x8
    BOOL skipStory; //0x10
    s32 progState; //0x14
    BOOL progressive; //0x18
    s32 toggle; //0x1C
} LogoWork;
#else
typedef struct LogoWork {
    BOOL progressive; //0x0
    s32 promptStep; //0x4, progressive-scan prompt step
    s32 toggle; //0x8, 0 = yes, 1 = no
    FileEntry* texture; //0xC
    s32 logoId; //0x10, logo shown: 0, 1, 2
    u8 pad_14[0x18 - 0x14]; //0x14, never accessed
    OSTime lastInputTime; //0x18, OSGetTime of the last pad input (prompt times out after 10 s)
    BOOL skipStory; //0x20
    u8 pad_24[0x28 - 0x24]; //0x24, never accessed
} LogoWork;
#endif

void seq_logoInit(SequenceWork* work);
void seq_logoMain(SequenceWork* work);
void seq_logoExit(SequenceWork* work);

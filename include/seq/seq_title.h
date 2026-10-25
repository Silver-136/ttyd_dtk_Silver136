#pragma once

#include "driver/seqdrv.h"
#include "manager/filemgr.h"
#include <dolphin/types.h>

typedef struct TitleWork {
    FileEntry* texture; //0x0
    u8 pad_04[0x8 - 0x4]; //0x4, never accessed
    s32 state; //0x8, title sequence step (switch in titleMain)
    f32 topPosX; //0xC, texture 5 position (slides down from y 1000 to 120)
    f32 topPosY; //0x10
    f32 bottomPosX; //0x14, texture 6 position (slides up from y -1000 to -170)
    f32 bottomPosY; //0x18
    f32 fadeAlpha; //0x1C, 0..1, +/-0.1 per frame
    s32 counter; //0x20
    s32 timer; //0x24
    s32 idle; //0x28
    s32 frame; //0x2C
    s32 debug; //0x30
} TitleWork;

const char* DbgBtlSel_GetMsgDataPtr(void);
s32 getDebugMode(void);
s32 N_getDebugMode(void);
void seq_titleInit(SequenceWork* work);
void seq_titleMain(SequenceWork* work);
void seq_titleExit(SequenceWork* work);

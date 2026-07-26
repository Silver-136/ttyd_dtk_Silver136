#pragma once

#include <dolphin/types.h>

typedef struct BackgroundWork {
    u8 unk0[0x20 - 0x0]; //0x0
} BackgroundWork;

void bgInit(void);
void bgReInit(void);
void bgMain(void);
void bgEntry(char* name);
void bgDispOn(void);
void bgDispOff(void);
void bgSetScrlOffset(f32 x, f32 y);
void bgAutoScrollOff(void);
void bgAutoScrollOn(void);
void bgSetColor(void* color);

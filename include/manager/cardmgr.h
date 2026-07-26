#pragma once

#include <dolphin/types.h>

void* cardGetFilePtr(void);
void cardBufReset(void);
u8 cardInit(void);
s32 cardIsExec(void);
void* cardGetCode(void);
u8 cardMain(void);
void cardCopy2Main(s32 fileNo);
void cardErase(s32 fileNo);
void cardCopy(s32 fileNo, s32 dataNo);
void cardWrite(s32 fileNo);
void cardWriteHeader(void);
void cardReadAll(void);
void cardCreate(void);
void cardFormat(void);

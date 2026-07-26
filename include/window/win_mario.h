#pragma once

#include <dolphin/types.h>

u8 winMarioInit(void* pWin);
void winMarioInit2(void* pWin);
s32 winMarioMain(void* pWin);
void winMarioMain2(void);
char* winZenkakuStr(s32 value);
void winMarioDisp(s32 cameraId, void* pWin, s32 index);

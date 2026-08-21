#pragma once

#include <dolphin/types.h>

u8 winMarioInit(void* pWin);
void winMarioInit2(void* pWin);
void winMarioExit(void* pWin);
s32 winMarioMain(void* pWin);
void winMarioMain2(void);
char* winZenkakuStr(s32 value);
char* unk_801703e8(s32 value, s32 width);
void fukidashi(double x, double y, void* menu, s32 type);
void winMarioDisp(s32 cameraId, void* pWin, s32 index);

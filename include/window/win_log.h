#pragma once

#include <dolphin/types.h>

typedef struct MonosiriWork MonosiriWork;

void winLogInit(void* pWin);
void winLogInit2(void* pWin);
s32 winLogMain(void* pWin);
void winLogMain2(void* pWin);
void winLogDisp(s32 cameraId, void* pWin, s32 index);
void monosiri_disp(s32 cameraId, void* work);
MonosiriWork* monosiriInit(s32 param, f32 x, f32 y);
u8 monosiriMain(void* param_1);
void monosiriClose(void* work);
s32 monosiriCloseWait(void* work);
void monosiriExit(void* work);

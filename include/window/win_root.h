#pragma once

#include <dolphin/types.h>

s32 winRootMain(void* pWin);
void winRootDisp(s32 cameraId, void* pWin);
void winMsgEntry(void* pWin, s32 param_2, char* param_3, s32 param_4);
void winKirinukiGX(double x, double y, double w, double h, void* pWin, s32 style);
void winNameGX(double x, double y, double width, double unusedHeight, void* pWin, s32 type);
void winBookGX(double x, double y, void* pWin, s32 page);
void winHalfBookGX(double x, double y, void* file, s32 side);
void winHakoGX(double x, double y, void* pWin, s32 type);
void winWazaGX(double x, double y, double w, double h, void* pWin);
s32 compare_func5_2(void* param_1, void* param_2);
s32 compare_func6_2(void* param_1, void* param_2);
void winSortEntry(void* win, s32 type, f32 a, f32 b);
s32 winSortWait(void* win);

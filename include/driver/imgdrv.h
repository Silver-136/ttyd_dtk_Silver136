#pragma once

#include <dolphin/types.h>

void imgInit(void);
void imgAutoRelease(s32 value);
s32 imgEntry(s32 name, s32 flag);
void imgMain(void);
void* imgNameToPtr(s32 name, s32 flag);
void imgFreeCapture(void* img, s32 index);
void imgSetVirtualPoint(void* img, void* point);
void imgClearVirtualPoint(void* img);
void imgRelease(void* image);
void imgSetShadow(void* img, s32 shadow);

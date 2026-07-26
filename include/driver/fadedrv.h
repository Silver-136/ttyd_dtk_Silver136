#pragma once

#include <dolphin/types.h>

void fadeInit(void);
void fadeTexSetup(void);
void fadeEntry(s32 type, s32 time, void* data);
void fadeMain(void);
void fadeTecOn(void);
void fadeTecOff(void);
void fadeTecSoftOn(void);
void fadeTecSoftOff(void);
void fadeSoftFocusOn(void);
void fadeSoftFocusOff(void);
void fadeSetSpotPos(f32 x, f32 y);
void fadeSetAnimOfsPos(f32 x, f32 y);
void fadeSetAnimVirtualPos(f32 x, f32 y, f32 z);
void fadeSetOffscreenCallback(void* callback, void* param);
void fadeSetNarrowFast(void);
s32 fadeIsFinish(void);
void fadeReset(s32 type);
void* fadeGetTpl(void);

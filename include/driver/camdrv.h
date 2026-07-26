#pragma once

#include <dolphin/types.h>

void camInit(void);
void camMain(void);
void camDraw(void);
void camLoadRoad(s32 cameraId, const char* name);
void camUnLoadRoad(s32 cameraId);
void* camGetPtr(s32 cameraId);
void* camGetCurPtr(void);
s32 camGetCurNo(void);
void camSetCurNo(s32 cameraId);
void camSetMode(s32 cameraId, s16 mode);
void L_camDispOn(s32 cameraId);
void L_camDispOff(s32 cameraId);
void camCtrlOn(s32 cameraId);
void camCtrlOff(s32 cameraId);
void camSetTypePersp(s32 cameraId);
void camSetTypeOrtho(s32 cameraId);
void getScreenPoint(void* position, f32* output);

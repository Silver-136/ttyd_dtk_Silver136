#pragma once

#include <dolphin/mtx.h>

void fontmgrInit(void);
void fontmgrTexSetup(void);
void FontDrawStart(void);
void FontDrawStart_alpha(u8 alpha);
void FontDrawEdge(void);
void FontDrawEdgeOff(void);
void FontDrawRainbowColor(void);
void FontDrawRainbowColorOff(void);
void FontDrawNoise(void);
void FontDrawNoiseOff(void);
void FontDrawColorIDX(s32 idx);
void FontDrawColor(void* colorPtr);
void FontDrawColor_(void);
u32 FontGetDrawColor(void);
void FontDrawScale(f32 scale);
void FontDrawScaleVec(Vec* scale);
void FontDrawCode(f64 x, f64 y, u16 code);
void FontDrawCodeMtx(void* mtx, u16 code);
void FontDrawString(f32 x, f32 y, s32 str);
void FontDrawStringMtx(f32* mtx, void* string);
void FontDrawStringCenterMtx(void* mtx, char* msg);
void FontDrawStringShake(double xIn, double yIn, char* str);
void FontDrawMessage(s32 x0, s32 y0, char* msg);
void FontDrawMessageMtx(f32 mtx[3][4], char* msg);
f32 FontGetMessageWidthLine(const char* msg, s32 line);
f32 FontGetMessageWidth(const char* msg);
u32 HSV2RGB(u8* hsv);

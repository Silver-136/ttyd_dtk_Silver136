#pragma once

#include <dolphin/types.h>

void iconInit(void);
void iconTexSetup(void);
void iconReInit(void);
void iconMain(void);
void iconEntry(char* name, u16 iconId);
void iconEntry2D(char* name, u16 iconId);
void iconDelete(const char* name);
void iconChange(const char* name, s32 iconId);
void iconDispGxAlpha(f32 scale, void* pos, s32 flags, s32 iconId, s32 alpha);
void iconDispGx(f32 scale, void* pos, s32 flags, s32 iconId);
void iconDispGx2(s32 mtx, s16 flags, u16 iconId);
void iconDispGxCol(s32 mtx, s16 flags, u16 iconId, void* color);
void iconGetTexObj(u32* texObj, u16 iconId);
void iconGetWidthHight(u16* width, u16* height, s32 iconId);
void* iconNameToPtr(const char* name);
void iconSetPos(const char* name, f32 x, f32 y, f32 z);
void iconFlagOn(const char* name, u32 flag);
void iconFlagOff(const char* name, u32 flag);
void iconSetScale(const char* name, f32 scale);
void iconSetAlpha(const char* name, s32 alpha);
u32 iconNumberDispGx(void* mtx, s32 number, s32 small, u32* color);
u32 iconNumberDispGx3D(void* mtx, s32 number, s32 small, u32* color);

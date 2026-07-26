#pragma once

#include <dolphin/types.h>

void offscreenInit(void);
void offscreenReset(s32 value);
s32 offscreenEntry(char* name);
void offscreenMain(void);
void offscreenAddBoundingBox(s32 id, u16 x0, u16 y0, u16 x1, u16 y1);
s32 offscreenGetBoundingBox(int id, void* x0, void* y0, void* x1, void* y1);
void offscreenGetTexObj(s32 id, void** texObj, u32* size);
s32 offscreenNameToId(char* name);
void* offscreenNameToPtr(char* name);
void offscreenDelete(char* name);
s32 offscreenGetBoundingBox_BeforeFrame(int id, void* x0, void* y0, void* x1, void* y1);

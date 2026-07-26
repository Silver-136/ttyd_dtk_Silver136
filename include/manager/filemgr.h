#pragma once

#include <dolphin/types.h>

u8 fileInit(void);
void* fileAllocf(s32 size, const char* fmt, ...);
void fileAlloc(void* name, unsigned long size);
void fileFree(void* file);
s32 fileAsyncf(s32 id, s32 flags, const char* fmt, ...);
char* fileAsync(char* name, s32 kind, s32 callback);
void fileSetCurrentArchiveType(s32 type);

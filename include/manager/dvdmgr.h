#pragma once

#include <dolphin/types.h>

typedef struct DVDEntry DVDEntry;

u8 DVDMgrInit(void);
void DVDMgrDelete(void);
void* DVDMgrOpen(const char* path, s32 mode, s32 unk);
s32 DVDMgrRead(void* handle, void* dst, u32 size, s32 offset);
void DVDMgrReadAsync(DVDEntry* entry, void* dst, u32 size, u32 offset, void* callback);
void DVDMgrClose(DVDEntry* entry);
s32 DVDMgrGetLength(DVDEntry* entry);
void DVDMgrSetupCallback(void* callback);

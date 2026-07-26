#pragma once

#include <dolphin/types.h>

void memInit(void);
void memClear(s32 heap);
void* __memAlloc(s32 heap, u32 size);
void __memFree(s32 heap, void* ptr);
void N_battleMapAlloc(void);
void N_battleMapFree(void);
void* _mapAlloc(void* base, u32 size);
void* _mapAllocTail(void* base, u32 size);
void _mapFree(void* base, void* ptr);
void smartInit(void);
void L_smartReInit(void);
void smartAutoFree(s32 kind);
void smartFree(void* entry);
void* smartAlloc(u32 param_1, u8 param_2);
void* smartTexObj(void* texObj, void** data);

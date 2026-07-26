#pragma once

#include <dolphin/types.h>

void aramMgrInit(void);
void* aramMgrToAram(u32 mramAddr, int size);
void aramMgrToMram(void* entry, s32 dest);

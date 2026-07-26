#pragma once

#include <dolphin/types.h>

u8 effInit64(void);
void effTexSetupN64(void);
u8 effGetTexObjN64(int id, u32* texObj);
void* effGetSetN64(char* name);
int effTblRandN64(int max, int seed);
void effSetVtxDescN64(void* data);
void tri1(s16 v0, s16 v1, s16 v2);
u8 tri2(s16 v0, s16 v1, s16 v2, s32 unused, s16 v3, s16 v4, s16 v5);

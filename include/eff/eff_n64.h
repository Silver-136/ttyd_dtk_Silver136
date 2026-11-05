// eff_n64.c: shared Paper Mario 64 effect support: init, texture lookup, effect set table, vertex/triangle helpers.

#pragma once

#include "version.h"
#include <dolphin/types.h>
#include "sdk/texPalette.h"
#include "eff/eff_n64_vtx.h"

typedef struct eff_n64_work {
    TEXPalette* effTexture; // 0x0
    BOOL effTextureLoaded;  // 0x4
} eff_n64_work;

typedef struct eff_set {
    s16 id;     // 0x0
    char* name; // 0x4
#if VERSION <= VERSION_DEMO_JP
    const char* description; // 0x8
    const char** users;
#endif
} eff_set;

void effInit64(void);
void effTexSetupN64(void);
void effGetTexObjN64(u32 id, GXTexObj* obj);
eff_set* effGetSetN64(const char* name);
u32 effTblRandN64(u32 rand, s32 index);
void effSetVtxDescN64(effN64Vtx* vtx);
void tri1(u16 x, u16 y, u16 z, u16 flag);
void tri2(s32 x1, s32 y1, s32 z1, s32 flag1, s32 x2, s32 y2, s32 z2, s32 flag2);

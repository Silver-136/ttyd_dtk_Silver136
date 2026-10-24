#pragma once

#include <dolphin/types.h>
#include "sdk/texPalette.h"

typedef struct eff_n64_work {
    TEXPalette* effTexture; // 0x0
    BOOL effTextureLoaded;  // 0x4
} eff_n64_work;

typedef struct eff_set {
    s16 id;     // 0x0
    char* name; // 0x4
} eff_set;

// N64-style vertex, stride 0xE
typedef struct effN64Vtx {
    s16 x, y, z; // 0x0
    s16 s, t;    // 0x6
    u8 r, g, b, a; // 0xA
} effN64Vtx;

void effInit64(void);
void effTexSetupN64(void);
void effGetTexObjN64(u32 id, GXTexObj* obj);
eff_set* effGetSetN64(const char* name);
u32 effTblRandN64(u32 rand, s32 index);
void effSetVtxDescN64(effN64Vtx* vtx);
void tri1(u16 x, u16 y, u16 z, u16 flag);
void tri2(s32 x1, s32 y1, s32 z1, s32 flag1, s32 x2, s32 y2, s32 z2, s32 flag2);

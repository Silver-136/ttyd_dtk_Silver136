#pragma once

#include <dolphin/types.h>

// N64-style vertex, stride 0xE (eff_n64.h and the N64 effects).
typedef struct effN64Vtx {
    s16 x, y, z;   // 0x0
    s16 s, t;      // 0x6
    u8 r, g, b, a; // 0xA
} effN64Vtx;

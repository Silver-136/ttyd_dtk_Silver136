// eff_kameki_tornade_n64.c: Magikoopa tornado effect entry (effKamekiTornadeN64Entry) and its work struct.

#pragma once

#include <dolphin/mtx.h>

#include "driver/effdrv.h"
#include "dolphin/types.h"

typedef struct EffKamekiTornadeN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 life;      // 0x10
    s32 frame;     // 0x14
    s32 c0r;       // 0x18
    s32 c0g;       // 0x1C
    s32 c0b;       // 0x20
    s32 alpha;     // 0x24
    s32 c1r;       // 0x28
    s32 c1g;       // 0x2C
    s32 c1b;       // 0x30
    f32 scale;     // 0x34
    f32 px[8];     // 0x38
    f32 py[8];     // 0x58
    f32 pz[8];     // 0x78
    f32 sc[8];     // 0x98
    f32 ang[8];    // 0xB8
    f32 angVel[8]; // 0xD8
    f32 radius[8]; // 0xF8
    f32 rot[8];    // 0x118
    f32 twist;     // 0x138
    f32 scrollS;   // 0x13C
    f32 scrollT;   // 0x140
    f32 rotZ;      // 0x144, degrees
    f32 alphaScale; // 0x148, multiplies the TEV alpha (tornade_alpha_func)
} EffKamekiTornadeN64Work;

EffectEntry* effKamekiTornadeN64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale);

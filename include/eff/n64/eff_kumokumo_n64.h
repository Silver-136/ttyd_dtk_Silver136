#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

/* effKumokumoN64Entry userdata (EffectEntry.userdata), one per particle; used by party_cloud.c
   (and writable by unit_party_clauda.c) */
typedef struct EffKumokumoN64Particle {
    s32 type;       // 0x0
    f32 x;          // 0x4
    f32 y;          // 0x8
    f32 z;          // 0xC
    s32 delay;      // 0x10
    s32 frame;      // 0x14
    s32 r1;         // 0x18
    s32 g1;         // 0x1C
    s32 b1;         // 0x20
    s32 alpha;      // 0x24
    s32 r2;         // 0x28
    s32 g2;         // 0x2C
    s32 b2;         // 0x30
    s32 a2;         // 0x34
    f32 s1;         // 0x38
    f32 ds1;        // 0x3C
    f32 t1;         // 0x40
    f32 dt1;        // 0x44
    f32 s2;         // 0x48
    f32 ds2;        // 0x4C
    f32 t2;         // 0x50
    f32 dt2;        // 0x54
    f32 rot;        // 0x58
    f32 scale;      // 0x5C
    s32 camera;     // 0x60
} EffKumokumoN64Particle;

EffectEntry* effKumokumoN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 rot, f32 unused, f32 scale);

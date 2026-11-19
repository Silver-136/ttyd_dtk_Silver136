// eff_kumokumo_n64.c: cloud (kumokumo) effect entry (effKumokumoN64Entry).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

/* effKumokumoN64Entry userdata (EffectEntry.userdata), one per particle; used by party_cloud.c
   (and writable by unit_party_clauda.c) */
typedef struct EffKumokumoN64Particle {
    // Record 0 is the cloud model, records 1 and up its puffs; the effect reads type and camera from record 0 only.
    s32 type;       // 0x0, effect type 0-4
    f32 x;          // 0x4, record 0: cloud position; puffs: offset in the cloud's model space
    f32 y;          // 0x8
    f32 z;          // 0xC
    s32 delay;      // 0x10, record 0: frames left (1000: until deleted; a delete request sets 16, the cloud fades
                    // out over the last 16); puffs: frames until the puff shows and moves
    s32 frame;      // 0x14, frames since the start (record 0 fades in over the first 16)
    s32 r1;         // 0x18, record 0: first TEV colour, blended with the second by the first texture's alpha
    s32 g1;         // 0x1C
    s32 b1;         // 0x20
    s32 alpha;      // 0x24, record 0: cloud alpha; puffs: their own alpha, multiplied by the cloud's
    s32 r2;         // 0x28, record 0: second TEV colour
    s32 g2;         // 0x2C
    s32 b2;         // 0x30
    s32 a2;         // 0x34, alpha of the second colour (255)
    f32 s1;         // 0x38, record 0: first texture's scroll offset; puffs of types 0 and 4: outward speed
                    // (8, easing to 2)
    f32 ds1;        // 0x3C, record 0: s1 step per frame
    f32 t1;         // 0x40, record 0: first texture's scroll offset
    f32 dt1;        // 0x44, record 0: t1 step per frame
    f32 s2;         // 0x48, record 0: second texture's scroll offset
    f32 ds2;        // 0x4C, record 0: s2 step per frame
    f32 t2;         // 0x50, record 0: second texture's scroll offset
    f32 dt2;        // 0x54, record 0: t2 step per frame
    f32 rot;        // 0x58, z rotation in degrees; record 0 at 90 or more draws flipped upside down (types 0-2
                    // and 4); type 3 spins it by -2 a frame
    f32 scale;      // 0x5C, draw scale
    s32 camera;     // 0x60, record 0: camera the effect draws with (CAMERA_3D; party_cloud sets the partner's)
} EffKumokumoN64Particle;

EffectEntry* effKumokumoN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 rot, f32 unused, f32 scale);

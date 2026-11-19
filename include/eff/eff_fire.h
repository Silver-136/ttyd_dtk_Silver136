// eff_fire.c: flame effect: effFireEntry and its work data (EffFireWork).

#pragma once

#include "version.h"
#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct EffFireWork {
    s32 type;               // 0x0, 0-2: plain flames (effFireDisp), 3/4/8: effFireDisp2, 5-7: effFireDisp3
    Vec position;           // 0x4
    u8 alpha;               // 0x10, 0xCC for types 3, 4, 8, else 0xFF; duration * 16 over the last 16 frames
    s32 duration;           // 0x14, frames left (1000 or more: endless); 16 on a soft delete
    s32 delay;              // 0x18, animation counter 0 .. smokeInt (random start); types 0-2 show frame delay / 2
    s32 sparkTimer;         // 0x1C, counts up to range, then spawns a spark
    s32 range;              // 0x20, sparkTimer limit, 30 to 59 at random after each spark or puff
    f32 scale;              // 0x24, overall size
    s32 smokePhase0;        // 0x28, types 3-8: strip frame smokeLife * delay / smokeInt (texture matrix 0)
    s32 smokePhase1;        // 0x2C, the following strip frame (texture matrix 1)
    s32 smokeInt;           // 0x30, frames per animation cycle (smokeLife * 2)
    s32 smokeLife;          // 0x34, frames in the texture strip (12; type 1: 4, type 2: 10)
    s32 glowAlpha;          // 0x38, TEVREG2 level
    s32 keyIdx;             // 0x3C, current key of the FireScaleKey table (types 3, 4, 8)
    s32 keyFrame;           // 0x40, frame counter through that table (back to 0 with keyIdx)
    f32 pulseScale;         // 0x44, keyframed scale (FireScaleKey table)
    f32 pad48[1];
    f32 reserved_4C[6];     // 0x4C, only set to 0
    s32 bboxValid;          // 0x64, set once the screen bbox is computed
    s32 scrMinX;            // 0x68, screen-space bbox
    s32 scrMaxX;            // 0x6C
    s32 scrMaxY;            // 0x70
    s32 scrMinY;            // 0x74
    f32 scaleX;             // 0x78, extra x stretch from the caller (1 by default)
    f32 scaleY;             // 0x7C, extra y stretch from the caller (1 by default)
    s32 reserved_80;        // 0x80, only set to 0
    s32 reserved_84;        // 0x84, only set to 0
#if VERSION >= VERSION_JP
    s32 useMapBlend;        // 0x88, adds the map blend-colour TEV stage
#endif
} EffFireWork;

EffectEntry* effFireEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);

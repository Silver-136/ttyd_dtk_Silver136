// eff_fire.c: flame effect: effFireEntry and its work data (EffFireWork).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct EffFireWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    u8 alpha;      // 0x10
    u8 pad11[3];
    s32 duration;  // 0x14
    s32 delay;     // 0x18
    s32 sparkTimer;     // 0x1C, counts up to range, then spawns a spark
    s32 range;     // 0x20
    f32 scale;     // 0x24
    s32 smokePhase0;     // 0x28
    s32 smokePhase1;     // 0x2C
    s32 smokeInt;  // 0x30
    s32 smokeLife; // 0x34
    s32 glowAlpha;     // 0x38, TEVREG2 level
    s32 keyIdx;    // 0x3C
    s32 keyFrame;  // 0x40
    f32 pulseScale;     // 0x44, keyframed scale (FireScaleKey table)
    f32 pad48[1];
    f32 f4c[6];    // 0x4C
    s32 bboxValid;     // 0x64, set once the screen bbox is computed
    s32 scrMinX;     // 0x68, screen-space bbox
    s32 scrMaxX;     // 0x6C
    s32 scrMaxY;     // 0x70
    s32 scrMinY;     // 0x74
    f32 scaleX;     // 0x78
    f32 scaleY;     // 0x7C
    s32 reserved_80;     // 0x80, only set to 0
    s32 reserved_84;     // 0x84, only set to 0
#include "version.h"
#if VERSION > VERSION_DEMO_US
    s32 useMapBlend;     // 0x88, adds the map blend-colour TEV stage
#endif
} EffFireWork;

EffectEntry* effFireEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);

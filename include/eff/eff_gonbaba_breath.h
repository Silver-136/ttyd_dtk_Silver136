// eff_gonbaba_breath.c: fire breath (ファイアブレス) of Hooktail, Bonetail and Bowser: type % 10 picks the colour set (normal, poison, mega, cold, confuse, sleep, mini, Bowser), types 10-17 are the companion layer. effGonbabaBreathEntry and EffGonbabaBreathWork.

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>

typedef struct EffGonbabaBreathWork {
    s32 type;           // 0x0
    Vec position;       // 0x4
    f32 velX;           // 0x10
    f32 velY;           // 0x14
    f32 pad_18;          // 0x18, never accessed
    s32 life;           // 0x1C
    s32 timer;          // 0x20
    s32 from[3];        // 0x24
    s32 to[3];          // 0x30
    s32 alpha;          // 0x3C
    f32 scale;          // 0x40
    s32 delay;          // 0x44
    f32 yaw;            // 0x48
    EffectEntry* child; // 0x4C
} EffGonbabaBreathWork;

EffectEntry* effGonbabaBreathEntry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 yaw);

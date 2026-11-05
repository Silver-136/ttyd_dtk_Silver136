// eff_majinai.c: majinai (spell) effect entry (effMajinaiEntry), its work struct and main/draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>
#include <dolphin/mtx.h>

typedef struct EffMajinaiWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    s32 alpha;    // 0x14
    s32 state;    // 0x18
    s32 poseId;   // 0x1C
} EffMajinaiWork;

EffectEntry* effMajinaiEntry(s32 type, f32 x, f32 y, f32 z);

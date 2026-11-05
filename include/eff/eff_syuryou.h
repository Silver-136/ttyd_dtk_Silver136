// eff_syuryou.c: Grodus effects (syuryou, 首領 "leader"): type 0 his staff animation (EFF_batug_tue), type 1 the barrier (MOBJ_EFF_baria, also used by the Yux); effSyuryouEntry and EffSyuryouWork.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/mtx.h>
#include <dolphin/types.h>

typedef struct EffSyuryouWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 scale;    // 0x10
    s32 poseId;   // 0x14
    s32 alpha;    // 0x18
    s32 timer;    // 0x1C, counts down while fading out
    s32 field_0x20; // 0x20
} EffSyuryouWork;

EffectEntry* effSyuryouEntry(s32 type, s32 duration, f32 x, f32 y, f32 z);

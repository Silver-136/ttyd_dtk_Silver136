#pragma once
#include "driver/effdrv.h"
#include <dolphin/mtx.h>
#include <dolphin/types.h>

typedef struct ArmWork {
    Vec prevPos;
    Vec pos;
    f32 rot;
    f32 targetRot;
    EffectEntry* eff;
} ArmWork;


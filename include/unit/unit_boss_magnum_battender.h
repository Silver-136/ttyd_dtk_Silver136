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

// userdata of the effects both Magnus von Grapple units move by hand (eff_tor, eff_jet)
typedef struct BattenEffWork {
    s32 type;
    Vec pos;
    u8 field_0x10[0x28 - 0x10];
    Mtx mtx;
} BattenEffWork;


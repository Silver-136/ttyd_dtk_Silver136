// eff_guruguru_n64.c: Paper Mario 64 Guruguru effect entry (effGuruguruN64Entry) and its work data (EffGuruguruN64Work).

#pragma once

#include <dolphin/mtx.h>

#include "driver/effdrv.h"
#include "dolphin/types.h"

typedef struct EffGuruguruN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 radius;   // 0x10
    f32 targetRadius; // 0x14
    f32 rotZ;     // 0x18
    f32 rotZSpeed; // 0x1C
    f32 rotX;     // 0x20
    s32 angle;    // 0x24
    s32 active;   // 0x28
    f32 scale;    // 0x2C
} EffGuruguruN64Work;

EffectEntry* effGuruguruN64Entry(s32 type, s32 num, f32 x, f32 y, f32 z, f32 radius);

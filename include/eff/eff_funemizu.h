#pragma once

#include "dolphin/gx/GXStruct.h"
#include <dolphin/mtx.h>
#include <dolphin/gx.h>
#include "driver/camdrv.h"
#include "driver/effdrv.h"

typedef struct EffFunemizuWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 f10;       // 0x10
    f32 f14;       // 0x14
    f32 f18;       // 0x18
    f32 f1c;       // 0x1C
    f32 f20;       // 0x20
    f32 f24;       // 0x24
    f32 f28;       // 0x28
    f32 f2c;       // 0x2C
    s32 timer;     // 0x30
    f32 f34;       // 0x34
    f32 f38;       // 0x38
    s32 delay;     // 0x3C
    s32 alpha;     // 0x40
    f32 scale;     // 0x44
    CameraId cam;  // 0x48
} EffFunemizuWork;

GXColor effFunemizuGetMapColor(void);
EffectEntry* effFunemizuEntry(s32 type, f32 x, f32 y, f32 z, f32 size, f32 yaw);

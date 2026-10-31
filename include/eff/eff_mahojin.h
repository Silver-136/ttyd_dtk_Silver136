// eff_mahojin.c: magic-circle effect entry (effMahojinEntry) and its work struct (EffMahojinWork).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include "driver/camdrv.h"

typedef struct EffMahojinWork {
    s32 type;                      // 0x0
    Vec position;                  // 0x4
    s32 timer;                     // 0x10
    s32 field_0x14;                // 0x14
    s32 poseId[2];                 // 0x18
    s32 alpha;                     // 0x20
    s32 state;                     // 0x24
    void* arg;                     // 0x28
    void (*func)(CameraId, void*); // 0x2C
} EffMahojinWork;

EffectEntry* effMahojinEntry(s32 type, f32 x, f32 y, f32 z);

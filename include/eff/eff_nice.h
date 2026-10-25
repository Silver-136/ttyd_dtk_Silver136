#pragma once

#include "driver/effdrv.h"
#include <dolphin/mtx.h>

typedef struct NiceEffectWork {
    s32 type; //0x0
    Vec position; //0x4
    f32 scaleX; //0x10, squash-and-stretch of the "NICE" letters
    f32 scaleY; //0x14
    f32 scaleZ; //0x18
    f32 overallScale; //0x1C, overall scale (battle_event_cmd sets 0.75 / scale)
    s32 timer; //0x20, frames in the current state
    u8 pad_24[0x28 - 0x24]; //0x24, never accessed
    s32 state; //0x28
    u8 pad_2C[0x30 - 0x2C]; //0x2C, never accessed
    s32 poseId; //0x30
    u8 pad_34[0x64 - 0x34]; //0x34, never accessed
} NiceEffectWork;

BOOL effNiceAsync(s32 group);
EffectEntry* effNiceEntry(s32 type, f32 x, f32 y, f32 z);

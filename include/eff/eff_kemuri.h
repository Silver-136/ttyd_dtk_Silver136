#pragma once

#include "driver/effdrv.h"
#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <dolphin/gx.h>

typedef struct effKemuri2Data {
    s32 pad_00; //0x0, never accessed
    f32 x; //0x4
    f32 y; //0x8
    f32 z; //0xC
    f32 speed; //0x10
    u8 pad_14[0x20 - 0x14]; //0x14, never accessed
    f32 vy; //0x20
    u8 pad_24[0x30 - 0x24]; //0x24, never accessed
    f32 dirX; //0x30
    f32 dirZ; //0x34
    u8 pad_38[0x3C - 0x38]; //0x38, never accessed
    s32 timer; //0x3C
    u8 pad_40[0x64 - 0x40]; //0x40, never accessed
    f32 rotVel; //0x64
    f32 rot; //0x68
    s32 alpha; //0x6C
    f32 scale; //0x70
    f32 scale2; //0x74
    f32 angle; //0x78
    u8 pad_7C[0x8C - 0x7C]; //0x7C, never accessed
} effKemuri2Data;

typedef struct effKemuriData {
    s32 type; //0x0, type
    f32 x; //0x4
    f32 y; //0x8
    f32 z; //0xC
    u8 pad_10[0x40 - 0x10]; //0x10, never accessed
    s32 frame; //0x40, frames since start
    f32 scaleX; //0x44, touchdown_scale[frame][0] / 128
    f32 scaleY; //0x48, touchdown_scale[frame][1] / 128
    f32 velX; //0x4C, sideways drift (+-3, decays by 0.85)
    u8 pad_50[0x58 - 0x50]; //0x50, never accessed
    f32 offsetX; //0x58, accumulated drift
    f32 reserved_5C; //0x5C, only set to 0
    u8 pad_60[0x74 - 0x60]; //0x60, never accessed
    f32 scale; //0x74, uniform scale
    u8 pad_78[0x80 - 0x78]; //0x78, never accessed
    s32 texFrame0; //0x80, current texture frame (0..15)
    s32 texFrame1; //0x84, next texture frame, clamped to 15
    s32 lifetime; //0x88, frames (32 or 64)
} effKemuriData;

EffectEntry* effKemuriEntry(s32 int2, f32 float3, f32 float4, f32 float5);
void effKemuriMain(EffectEntry* effect);

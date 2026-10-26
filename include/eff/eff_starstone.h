#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"
#include <dolphin/mtx.h>
#include <dolphin/gx/GXStruct.h>

// EffStarStoneWork.flags
typedef enum StarStoneFlags {
    STAR_STONE_FLAG_FALLING = (1 << 0),       // 0x1, set when the fall starts
    STAR_STONE_FLAG_PLAIN = (1 << 1),         // 0x2, types 7 and up: no dust, sparkles or shadow
    STAR_STONE_FLAG_SHINE_SFX = (1 << 2),     // 0x4, plays the shine sound
    STAR_STONE_FLAG_SHINE_SFX_ON = (1 << 3),  // 0x8, the shine sound is playing
    STAR_STONE_FLAG_NO_JUMP_SFX = (1 << 4),   // 0x10, no bounce sound
    STAR_STONE_FLAG_EXTERNAL_DRAW = (1 << 5), // 0x20, drawn by its owner through effStarStoneDraw
} StarStoneFlags;

typedef struct EffStarStoneWork {
    s32 type;        // 0x0
    u16 flags;       // 0x4, StarStoneFlags
    Vec position;    // 0x8
    f32 scale;       // 0x14
    Vec rot;         // 0x18, degrees; drawn turned about y by rot.y, about z by rot.z, then about y by rot.x
                     // (rot.y is stone_ry)
    f32 f24;         // 0x24
    f32 f28;         // 0x28
    s32 timer;       // 0x2C
    GXColor color;   // 0x30
    EffectEntry* dust; // 0x34
    s32 cam;         // 0x38
    s32 state;       // 0x3C
    f32 f40;         // 0x40
    f32 f44;         // 0x44
    f32 f48;         // 0x48
    f32 f4c;         // 0x4C
    f32 f50;         // 0x50
    f32 f54;         // 0x54
    f32 f58;         // 0x58
    u8 r, g, b, a;   // 0x5C
    s32 sfx;         // 0x60
} EffStarStoneWork;

EffectEntry* effStarStoneEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);

// Crystal Star effect: entry (effStarStoneEntry), draw function (effStarStoneDraw), flags (StarStoneFlags)
// and per-effect work data (EffStarStoneWork).

#pragma once

#include "driver/camdrv.h"
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
    s32 type;        // 0x0, Crystal Star 0-6 (the entry's type % 7)
    u16 flags;       // 0x4, StarStoneFlags
    Vec position;    // 0x8
    f32 scale;       // 0x14
    Vec rot;         // 0x18, degrees; drawn turned about y by rot.y, about z by rot.z, then about y by rot.x
                     // (rot.y is stone_ry)
    f32 texScrollX;  // 0x24, x translation of the texture matrix (DISP_MID); scrolled by 0.01 per frame in state 0 (type 6 backwards), wraps at 1
    f32 texScrollY;  // 0x28, y translation of the texture matrix; set to 0 at entry, never changed
    s32 timer;       // 0x2C, frame counter (mod 60); in state 0 a stardust puff every 10 frames
    GXColor color;   // 0x30, material colour from edge_color by type
    EffectEntry* dust; // 0x34, effSnowDustN64 trail (none for types 7 and up); moved out of sight once it falls
    s32 cam;         // 0x38, camera it draws with (CAMERA_3D; some maps set an effect camera)
    s32 state;       // 0x3C, 0 floats and spins; 1 (set by kpa_07 and pik_02) starts the fall; from 2 each bounce
                     // adds 1 and 6 stops it; the halves part from 3
    f32 gravity;     // 0x40, set to 0.4 when the fall starts; subtracted from speedY each frame
    f32 speedY;      // 0x44, vertical speed: added to position.y; on landing reversed and halved (bounce); its size sets the ground-check distance
    f32 half0OffsetX; // 0x48, x offset of the first stone half (dl_0); drifts apart from the other half after the first bounce
    f32 half0OffsetZ; // 0x4C, z offset of the first stone half (dl_0)
    f32 half1OffsetX; // 0x50, x offset of the second stone half (dl_1), moves opposite to half0OffsetX
    f32 half1OffsetZ; // 0x54, z offset of the second stone half (dl_1)
    f32 reserved_58; // 0x58, only set to 90.0f, never read
    u8 r, g, b, a;   // 0x5C, TEV colour (255 each); JP and later also give a to the shadow (shadowSetMode)
    s32 sfx;         // 0x60, SFX_EVT_STARSTONE_SHINE1 while STAR_STONE_FLAG_SHINE_SFX_ON
} EffStarStoneWork;

EffectEntry* effStarStoneEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
void effStarStoneDraw(CameraId cameraId, EffectEntry* effect);

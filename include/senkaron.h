#pragma once

// Dry Bones swarm (senkaron): shared by the map RELs that run it (gon_03, las_05).

#include <dolphin/types.h>

// One swarm member.
typedef struct Senkaron {
    s8 state;
    u16 area;   // floor cell index (used by las_05 only)
    s32 timer;
    f32 vx;
    f32 vy;
    f32 vz;
    u32 flags;  // SENKARON_FLAG_*
} Senkaron;

// Senkaron.flags. The code clears TURNED with `&= 0xFFFE`, which also clears the (unused) upper 16 bits.
enum {
    SENKARON_FLAG_TURNED = 1,       // faces the other way while pushed (1 in 50 when a push starts)
    SENKARON_FLAG_CROWDED = 1 << 1, // las_05: shares a floor cell with a bone nearer Mario; moves backwards
};

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
    u32 flags;
} Senkaron;

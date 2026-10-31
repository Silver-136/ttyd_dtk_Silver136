// eff_fire_ring_n64.c: Paper Mario 64 fire ring: one 90-vertex ring mesh with two scrolling textures that grows, spawns fire dust and fades over its last 10 frames (effFireRingN64Entry).

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effFireRingN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);

// eff_mugi.c: the bits kicked up by Mario's footsteps (むぎばたけ); effMugiKemuriEntry, effMugiKemuri2Entry.

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effMugiKemuri2Entry(s32 type, f32 x, f32 y, f32 z, f32 angle, f32 dir);
EffectEntry* effMugiKemuriEntry(f32 x, f32 y, f32 z, f32 angle);

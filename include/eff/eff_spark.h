// eff_spark.c: spark effect entry (effSparkEntry) with its main and draw callbacks.

#pragma once

#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include <dolphin/types.h>

EffectEntry* effSparkEntry(s32 type, s32 unused1, f32 x, f32 y, f32 z, f32 unused2); // the two unused: never read

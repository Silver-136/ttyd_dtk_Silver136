#pragma once

#include <dolphin/types.h>

void* effGonbabaBreathEntry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 timer);
void effGonbabaBreathMain(void* effect);
void effGonbabaBreathDisp(s32 cameraId, void* effect);

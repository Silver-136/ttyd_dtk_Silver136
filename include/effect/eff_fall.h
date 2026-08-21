#pragma once

#include <dolphin/types.h>

void* effFallEntry(f32 x, s32 type, f32 y, s32 count, f32 z, s32 param,
                   f32 rot, f32 arg5, f32 arg6);
void effFallMain(void* effect);
void effFallDisp(s32 cameraId, void* effect);
void effFallDispInseki(s32 cameraId, void* effect);
void effFallDispWater(s32 cameraId, void* effect);
s32 effFallCheckHit(void* effect);

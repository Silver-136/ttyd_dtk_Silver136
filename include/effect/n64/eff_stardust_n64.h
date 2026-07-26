#pragma once

#include <dolphin/types.h>

void* effStardustN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type);
void effStardustN64SetDrawCam(void* effect, s32 camId);
void effStardustN64SetScale(void* effect, f32 scale);

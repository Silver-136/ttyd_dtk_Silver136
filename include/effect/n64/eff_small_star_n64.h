#pragma once

#include <dolphin/types.h>

void* effSmallStarN64Entry(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ,
                           s32 type, s32 count);
void effSmallStarN64CamChg(void* effect, s8 camId);

#pragma once

#include <dolphin/types.h>

void effIndirectMain(void* entry);
void* effIndirectEntry(s32 type, s32 timer, f32 x, f32 y, f32 z);

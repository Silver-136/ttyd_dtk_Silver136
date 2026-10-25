#pragma once

#include "dolphin/types.h"

extern u8 enemy_monoshiri_sort_table[];
void winZClear(void);
void winHalfBookGX(TEXPalette* tpl, s32 a, f32 x, f32 y);
void winRootDisp(s32 camId, WindowWork* wp);
s32 winRootMain(WindowWork* w);

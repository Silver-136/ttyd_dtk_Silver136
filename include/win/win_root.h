// win_root.c: the pause menu root (tabs, help text, sort menu, shared frames) and the list sort comparers.

#pragma once

#include "dolphin/types.h"

extern u8 enemy_monoshiri_sort_table[];
void winZClear(void);
void winHalfBookGX(TEXPalette* tpl, s32 a, f32 x, f32 y);
void winRootDisp(s32 camId, WindowWork* wp);
s32 winRootMain(WindowWork* w);
s32 compare_func5_2(const void* a, const void* b);
s32 compare_func6_2(const void* a, const void* b);
void winTabMain(WindowWork* w);

// win_item.c: the pause menu item window (items, important items, mail, Koopa book): winItemInit/Main/Disp.

#pragma once

#include "dolphin/types.h"

struct WindowWork;

extern const char** luigi_book_list[];
extern u16 menu_skip_list[];
void winItemDisp(s32 camId, void* param, s32 n);
void winItemExit(struct WindowWork* w);
void winItemInit(struct WindowWork* w);
void winItemInit2(struct WindowWork* w);
s32 winItemMain(struct WindowWork* w);
void winItemMain2(struct WindowWork* w);
void winMakeSkipList(struct WindowWork* w);

#pragma once

#include "win/win_main.h"

extern WindowPartyDt winPartyDt[];
void winPartyDisp(s32 camId, void* param, s32 idx);
void winPartyExit(WindowWork* wp);
void winPartyInit(WindowWork* wp);
void winPartyInit2(WindowWork* wp);
s32 winPartyMain(WindowWork* wp);
void winPartyMain2(WindowWork* wp);

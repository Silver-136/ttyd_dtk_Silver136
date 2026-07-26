#pragma once

#include <dolphin/types.h>

void BattleSequenceManager(void);

void battleSortPhaseMoveTable(void* battleWork, s32 arg);
s32 BattleWaitAllActiveEvtEnd(void* battleWork);
int BattleWaitAllActiveEvtEnd_NoBgSetEndWait(void* battleWork);

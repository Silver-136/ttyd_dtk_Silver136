#pragma once

#include "battle/battle.h"

void BattleSequenceManager(void);

BOOL BattleWaitAllActiveEvtEnd(BattleWork* wp);
BOOL BattleCheckConcluded(BattleWork* wp);
BOOL BattleWaitAllActiveEvtEnd_NoBgSetEndWait(BattleWork* wp);
void battleSortPhaseMoveTable(BattleWork* wp, s32 count);

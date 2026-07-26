#pragma once

#include <dolphin/types.h>

s32 BattleCheckEndUnitInitEvent(void* battleWork);
s32 BattleRunHitEventDirect(void* unit, s32 flags, void* script);
s32 BattleRunHitEvent(void* unit, u32 flags);
s32 BattleRunPhaseEvent(void* unit, int unisonPhase);
void BattlePhaseEventStartDeclare(void* unit);
s32 BattleRunWaitEvent(void* unit);

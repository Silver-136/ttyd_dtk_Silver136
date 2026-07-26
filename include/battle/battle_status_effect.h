#pragma once

#include <dolphin/types.h>

#include "battle/battle_unit.h"

void BattleStatusChangeMsgWorkInit(void);
void BattleStatusEffectInit(BattleWorkUnit* unit);
void BattleStatusChangeInfoWorkInit(BattleWorkUnit* unit);
void BattleStatusEffectMain(BattleWorkUnit* unit);
void BSE_TurnFirstProcessEffectEntry(BattleWorkUnit* unit, s32 value);
s32 BSE_TurnFirstProcessEffectMain(void* unit);
s32 BattleStatusChangeAnnouceMain(void* battleWork);
void BattleStatusChangeMsgAdjust(void* battleWork);
int BattleStatusChangeMsgMain(void* battleWork);

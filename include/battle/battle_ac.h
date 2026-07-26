#pragma once

#include <dolphin/types.h>

#include "battle/battle.h"

void BattleActionCommandManagerInit(BattleWork* wp);
void BattleActionCommandManager(BattleWork* wp);
void BattleActionCommandSetup(BattleWork* battleWork, s32 param, BattleWorkUnit* unit, s32 rawArg, s32 value);
void BattleActionCommandStart(BattleWork* battleWork);
void BattleActionCommandStop(BattleWork* battleWork);
void BattleActionCommandCheckDefence(BattleWorkUnit* unit, s32 value);
s32 BattleActionCommandGetDefenceResult(void);
s32 BattleActionCommandGetDifficulty(BattleWork* battleWork);
void BattleActionCommandSetDifficulty(BattleWork* battleWork, BattleWorkUnit* unit, s32 difficulty);
s32 BattleActionCommandGetPrizeLv(BattleWork* battleWork, BattleWorkUnit* unit, s32 value);

s32 BattleActionCommandResult(void* work);
void BattleActionCommandDeclareACResult(void* battleWork, void* unit, s32 result);
void BattleActionCommandResetDefenceResult(void);
s32 BattleACGetButtonIcon(int button, int pressed);

void BattleAcGaugeSeInit(void);
void BattleAcGaugeSeUpdate(f32 value);
void BattleAcGaugeSeDelete(void);

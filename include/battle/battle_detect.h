#pragma once

#include <dolphin/types.h>
s32 BattleSamplingEnemy(void* targetWork, void* weapon, int attackerIdx,
                        int enemyBelong, u32 targetClassFlags,
                        u32 targetPropertyFlags, u8 param7);
void BattleSamplingEnemyUpdate(void);
void BattleChoiceSamplingEnemy(void* targetWork, u32 weighting, int* unitIdx,
                               int* partIdx);
void BattleGetFirstAttackUnit(void* battleWork, void** part, void** unit);

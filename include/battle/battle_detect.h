#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"
#include "battle/battle_database_common.h"

BOOL BattleSamplingEnemy(BattleWorkWeaponTargets* d, BattleWeapon* weapon, s32 unitId, s8 belong, u32 classFlags,
                         u32 propFlags, s8 direction);
void BattleSamplingEnemyUpdate(BattleWorkWeaponTargets* d);
void BattleChoiceSamplingEnemy(BattleWorkWeaponTargets* d, u32 flags, s32* unitOut, s32* partOut);
void BattleGetFirstAttackUnit(BattleWork* wp, BattleUnit** attacker, BattleUnit** target);

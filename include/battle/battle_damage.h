#pragma once

#include <dolphin/types.h>

struct BattleWork;
struct BattleWorkUnit;
struct BattleWorkUnitPart;

u32 BattleSetStatusDamageFromWeapon(struct BattleWorkUnit* unit1, struct BattleWorkUnit* unit2, struct BattleWorkUnitPart* part, void* weapon, u32 flags);
s32 BattlePreCheckDamage(void* unit1, void* unit2, void* part, void* weapon, u32 flags);
void BattleAttackDeclare(int* unitIdx, int* partIdx);
void BattleAttackDeclareAll(struct BattleWork* battleWork);
void BattleInitCounterPreCheckWork(void* work);
s32 BattleCheckCounter(void* work, struct BattleWorkUnit* attacker, struct BattleWorkUnit* target, struct BattleWorkUnitPart* part, void* weapon, s32* result);

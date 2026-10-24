#pragma once

#include <dolphin/types.h>
#include "battle/battle_unit.h"
#include "driver/npcdrv.h"

void BattleInformationSetMode(FieldBattleInfo* info, u32 mode);
void BattleInformationSetParty(FieldBattleInfo* info, BattleUnitType party);
void BattleInformationSetFirstAttack(FieldBattleInfo* info, u32 partyId);
void BattleInformationInit(FieldBattleInfo* info);
void BattleInfomationSetBattleSetupInfo(FieldBattleInfo* info, NpcBattleInfo* setup);
void BattleInformationSetResult(FieldBattleInfo* info, u32 result);
s32 BattleInformationGetResult(FieldBattleInfo* info);

void BattleInformationSetDropMaterial(FieldBattleInfo* info);

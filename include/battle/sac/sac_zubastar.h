#include "battle/battle_unit.h"
#include "battle/battle_database_common.h"
#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"

extern EvtWord marioAttackEvent_ZubaStar[];
u32 weaponGetPower_ZubaStar(BattleUnit* a, BattleWeapon* weapon, BattleUnit* target, BattleUnitPart* part);

#include "battle/battle_unit.h"
#include "battle/battle_database_common.h"
#pragma once

#include "dolphin/types.h"
#include "battle/battle_unit.h"
#include "evt/evt_word.h"

extern EvtWord marioAttackEvent_Scissor[];
u32 weaponGetPower_Scissor(BattleUnit* a, BattleWeapon* b, BattleUnit* unit, BattleUnitPart* part);

// Declares the Earth Tremor (sac_deka) special move event.

#include "battle/battle_unit.h"
#include "battle/battle_database_common.h"
#pragma once

#include "dolphin/types.h"
#include "evt/evt_word.h"

extern EvtWord marioAttackEvent_Deka[];
u32 weaponGetPower_Deka(BattleUnit* attacker, BattleWeapon* weapon, BattleUnit* target, BattleUnitPart* part);

extern EvtWord dmg_evt[];

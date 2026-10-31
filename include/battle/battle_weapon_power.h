// Declares the weapon power calculation functions.

#pragma once

#include <dolphin/types.h>
#include "battle/battle_unit.h"
#include "battle/battle_database_common.h"

u32 weaponGetACOutputParam(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetFPPowerDefault(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetFPPowerFPHalf(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerDefault(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerFromGulliblePartyAttackLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerFromMarioHammerLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerFromMarioJumpLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerFromPartyAttackLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerHPHalf(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerHPHalf2(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerOverlapHammer1(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerOverlapJump1(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);
u32 weaponGetPowerTatsumaki(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part);

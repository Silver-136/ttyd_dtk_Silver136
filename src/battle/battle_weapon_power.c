#include "battle/battle_weapon_power.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_unit.h"
#include "mario/mario_pouch.h"

u32 weaponGetPowerDefault(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    return weapon->hpDamageWork[0];
}

u32 weaponGetFPPowerDefault(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    return weapon->fpDamageWork[0];
}

u32 weaponGetPowerFromMarioJumpLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    s32 acSuccess;
    s32 slot;
    u32 power;

    acSuccess = (_battleWorkPointer->actionCommands.result >> 1) & 1;
    slot = acSuccess + 2 * (pouchGetPtr()->jumpLevel - 1);
    if (slot >= 6) {
        slot = 5;
    }
    power = weapon->hpDamageWork[slot];
    if (unit1->badgesEquipped.Jumpman) {
        power += unit1->badgesEquipped.Jumpman;
    }
    return power;
}

u32 weaponGetPowerFromMarioHammerLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    s32 acSuccess;
    s32 slot;
    u32 power;

    acSuccess = (_battleWorkPointer->actionCommands.result >> 1) & 1;
    slot = acSuccess + 2 * (pouchGetPtr()->hammerLevel - 1);
    if (slot >= 6) {
        slot = 5;
    }
    power = weapon->hpDamageWork[slot];
    if (unit1->badgesEquipped.Hammerman) {
        power += unit1->badgesEquipped.Hammerman;
    }
    return power;
}

u32 weaponGetPowerOverlapJump1(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    u32 jumpPower;
    u32 bonusPerBadge;

    jumpPower = weaponGetPowerFromMarioJumpLv(unit1, weapon, unit2, part);
    bonusPerBadge = weapon->hpDamageWork[7];
    return jumpPower + bonusPerBadge * pouchEquipCheckBadge(weapon->hpDamageWork[6]);
}

u32 weaponGetPowerOverlapHammer1(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    u32 hammerPower;
    u32 bonusPerBadge;

    hammerPower = weaponGetPowerFromMarioHammerLv(unit1, weapon, unit2, part);
    bonusPerBadge = weapon->hpDamageWork[7];
    return hammerPower + bonusPerBadge * pouchEquipCheckBadge(weapon->hpDamageWork[6]);
}

u32 weaponGetPowerFromPartyAttackLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    BattleWork* wp = _battleWorkPointer;
    PartyId partyId;
    s32 acSuccess;
    s32 slot;

    partyId = BattleTransPartyId(unit1->currentKind);
    acSuccess = (wp->actionCommands.result >> 1) & 1;
    slot = acSuccess + 2 * pouchGetPartyAttackLv(partyId);
    if (slot >= 6) {
        slot = 5;
    }
    return weapon->hpDamageWork[slot];
}

u32 weaponGetPowerFromGulliblePartyAttackLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2,
                                            BattleUnitPart* part) {
    PartyId partyId;
    s32 partyAttackLv;

    partyId = BattleTransPartyId(unit1->currentKind);
    partyAttackLv = pouchGetPartyAttackLv(partyId);
    if (partyAttackLv >= 6) {
        partyAttackLv = 5;
    }
    return weapon->hpDamageWork[partyAttackLv];
}

u32 weaponGetACOutputParam(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    s32 result;

    result = _battleWorkPointer->actionCommands.outputParams[weapon->hpDamageWork[0]] + weapon->hpDamageWork[1];
    if (result < 0) {
        result = 0;
    }
    return result;
}

u32 weaponGetPowerHPHalf(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    s32 hp = unit2->currentHp;

    if ((hp & 1) == 1) {
        return hp / 2 + 1;
    }
    return hp / 2;
}

u32 weaponGetPowerHPHalf2(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    return unit2->currentHp / 2;
}

u32 weaponGetFPPowerFPHalf(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    s32 fp = unit2->currentFp;

    if ((fp & 1) == 1) {
        return fp / 2 + 1;
    }
    return fp / 2;
}

u32 weaponGetPowerTatsumaki(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    u32 power;

    BattleGetMarioPtr(_battleWorkPointer);
    power = weapon->hpDamageWork[0];
    power += weapon->hpDamageWork[1] * pouchEquipCheckBadge(ITEM_TORNADO_JUMP);
    return power;
}

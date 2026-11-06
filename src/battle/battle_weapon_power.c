// Weapon power calculation: how much damage each kind of attack deals from levels, HP and badges.

#include "battle/battle_weapon_power.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_unit.h"
#include "mario/mario_pouch.h"

/** Default hpDamageFn: a fixed HP damage taken from hpDamageWork[0]. */
u32 weaponGetPowerDefault(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    return weapon->hpDamageWork[0];
}

/** Default fpDamageFn: a fixed FP damage taken from fpDamageWork[0]. */
u32 weaponGetFPPowerDefault(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    return weapon->fpDamageWork[0];
}

/**
 * Damage of Mario's jumps: hpDamageWork holds a (failed, succeeded) action-command pair per boot level (Normal,
 * Super, Ultra Boots), indexed by the action command's success bit; Jumpman adds its equipped count.
 */
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

/**
 * Damage of Mario's hammer attacks: hpDamageWork holds a (failed, succeeded) action-command pair per hammer
 * level, indexed by the action command's success bit; Hammerman adds its equipped count.
 */
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

/**
 * Damage of jumps that stack with copies of a badge (Power Jump, Multibounce and the like):
 * the boot-level damage plus hpDamageWork[7] for every equipped copy of the badge whose item id is in
 * hpDamageWork[6].
 */
u32 weaponGetPowerOverlapJump1(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    u32 jumpPower;
    u32 bonusPerBadge;

    jumpPower = weaponGetPowerFromMarioJumpLv(unit1, weapon, unit2, part);
    bonusPerBadge = weapon->hpDamageWork[7];
    return jumpPower + bonusPerBadge * pouchEquipCheckBadge(weapon->hpDamageWork[6]);
}

/**
 * Hammer counterpart of weaponGetPowerOverlapJump1:
 * hammer-level damage plus hpDamageWork[7] per equipped copy of the badge in hpDamageWork[6].
 */
u32 weaponGetPowerOverlapHammer1(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    u32 hammerPower;
    u32 bonusPerBadge;

    hammerPower = weaponGetPowerFromMarioHammerLv(unit1, weapon, unit2, part);
    bonusPerBadge = weapon->hpDamageWork[7];
    return hammerPower + bonusPerBadge * pouchEquipCheckBadge(weapon->hpDamageWork[6]);
}

/**
 * Damage of partner moves: hpDamageWork holds a (failed, succeeded) action-command pair per partner rank
 * (Normal, Super, Ultra), chosen by the attacker's rank and the action command's success bit.
 */
u32 weaponGetPowerFromPartyAttackLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    BattleWork* wp = _battleWorkPointer;
    s32 partyId;
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

/**
 * Partner-rank damage without an action command: hpDamageWork[rank] (capped at slot 5), with the rank of the
 * real partner the attacker's kind maps to. Used by the moves of Doopliss's partner copies (the gullible
 * partner units in jin and Doopliss's partner forms in unit_boss_rampell_las.c).
 */
u32 weaponGetPowerFromGulliblePartyAttackLv(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2,
                                            BattleUnitPart* part) {
    s32 partyId;
    s32 partyAttackLv;

    partyId = BattleTransPartyId(unit1->currentKind);
    partyAttackLv = pouchGetPartyAttackLv(partyId);
    if (partyAttackLv >= 6) {
        partyAttackLv = 5;
    }
    return weapon->hpDamageWork[partyAttackLv];
}

/**
 * Damage read from the action command's result:
 * outputParams[hpDamageWork[0]] plus hpDamageWork[1], never below 0.
 * Used by partner moves whose damage the minigame decides (Flurrie, Koops, Bobbery, Ms. Mowz and others).
 */
u32 weaponGetACOutputParam(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    s32 result;

    result = _battleWorkPointer->actionCommands.outputParams[weapon->hpDamageWork[0]] + weapon->hpDamageWork[1];
    if (result < 0) {
        result = 0;
    }
    return result;
}

/**
 * Halves the target's HP, rounding the damage up:
 * the Poison Shroom reel of the battle Bingo slot (weapon_doku_kinoko).
 */
u32 weaponGetPowerHPHalf(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    s32 hp = unit2->currentHp;

    if ((hp & 1) == 1) {
        return hp / 2 + 1;
    }
    return hp / 2;
}

/** Halves the target's HP, rounding the damage down: the Poison Shroom item (used on the user's own side). */
u32 weaponGetPowerHPHalf2(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    return unit2->currentHp / 2;
}

/** FP version of weaponGetPowerHPHalf: halves the target's FP, rounding up, for the Bingo slot's Poison Shroom reel. */
u32 weaponGetFPPowerFPHalf(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    s32 fp = unit2->currentFp;

    if ((fp & 1) == 1) {
        return fp / 2 + 1;
    }
    return fp / 2;
}

/**
 * Damage of Tornado Jump (tatsumaki = tornado):
 * hpDamageWork[0] plus hpDamageWork[1] per equipped Tornado Jump badge.
 */
u32 weaponGetPowerTatsumaki(BattleUnit* unit1, BattleWeapon* weapon, BattleUnit* unit2, BattleUnitPart* part) {
    u32 power;

    BattleGetMarioPtr(_battleWorkPointer);
    power = weapon->hpDamageWork[0];
    power += weapon->hpDamageWork[1] * pouchEquipCheckBadge(ITEM_TORNADO_JUMP);
    return power;
}

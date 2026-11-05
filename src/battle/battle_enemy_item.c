// Enemy item use: decides which healing, support or attack item an enemy unit should use.

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "data/item_data.h"
#include "system.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_database_common.h"
#include "battle/battle_detect.h"
#include "mario/mario_pouch.h"
#include "battle/battle_enemy_item.h"


static void* _check_status_recover_item(BattleUnit* self);
static void* _check_status_support_item(BattleUnit* self);
static void* _check_status_attack_item(BattleUnit* self);
static void* _check_attack_item(BattleUnit* self);
static void* _check_fp_recover_item(BattleUnit* self);
static void* _check_hp_recover_item(BattleUnit* self);

/**
 * Decides whether an enemy should use its held item this turn;
 * returns the item's attack script to run, or NULL to act normally.
 * Called from the btlevtcmd command that enemy AI scripts use.
 * Confused enemies only use items marked usable while confused.
 * Damage items (Thunder Bolt, Fire Flower, POW Block, HP Drain ...) go to _check_attack_item,
 * status items to the status checks, Slow Shroom and Gradual Syrup to the HP and FP checks,
 * and any other item that restores HP or FP is used when a target is missing some.
 */
void* BattleEnemyUseItemCheck(BattleUnit* self) {
    s32 itemId = self->heldItem;
    BattleWeapon* weapon;
    void* r;
    if (itemId == ITEM_NULL) {
        return NULL;
    }
    weapon = itemDataTable[itemId].battle_params;
    if (weapon == NULL) {
        return NULL;
    }
    if ((self->tokenFlags & kConfusedAction) && !(weapon->special & SPECIAL_CONFUSED_ACTION)) {
        return NULL;
    }
    switch (itemId) {
    case ITEM_THUNDER_BOLT:
    case ITEM_THUNDER_RAGE:
    case ITEM_SHOOTING_STAR:
    case ITEM_ICE_STORM:
    case ITEM_FIRE_FLOWER:
    case ITEM_EARTH_QUAKE:
    case ITEM_POW_BLOCK:
    case ITEM_HP_DRAIN_ITEM:
        return _check_attack_item(self);
    case ITEM_RUIN_POWDER:
    case ITEM_SLEEPY_SHEEP:
    case ITEM_STOPWATCH:
    case ITEM_DIZZY_DIAL:
    case ITEM_MINI_MR_MINI:
    case ITEM_MR_SOFTENER:
        return _check_status_attack_item(self);
    case ITEM_BOOS_SHEET:
    case ITEM_VOLT_SHROOM:
    case ITEM_REPEL_CAPE:
    case ITEM_POWER_PUNCH:
    case ITEM_COURAGE_SHELL:
        return _check_status_support_item(self);
    case ITEM_TASTY_TONIC:
        return _check_status_recover_item(self);
    case ITEM_SLOW_SHROOM:
        return _check_hp_recover_item(self);
    case ITEM_GRADUAL_SYRUP:
        return _check_fp_recover_item(self);
    default:
        if (itemDataTable[itemId].hp_restored != 0) {
            r = _check_hp_recover_item(self);
            if (r != NULL) {
                return r;
            }
        }
        if (itemDataTable[itemId].fp_restored != 0) {
            r = _check_fp_recover_item(self);
            if (r != NULL) {
                return r;
            }
        }
        return NULL;
    }
}

/**
 * Returns the item's script when any unit the item can target has less than full HP.
 * Like all checks here, the inactive test reads the user's own attribute flags, not the target's.
 */
static void* _check_hp_recover_item(BattleUnit* self) {
    BattleWork* wp;
    s32 itemId;
    BattleWorkWeaponTargets* targets;
    BattleWeapon* weapon;
    s32 i;
    BattleUnit* unit;

    itemId = self->heldItem;
    wp = _battleWorkPointer;
    targets = &wp->weaponTargets;
    weapon = itemDataTable[itemId].battle_params;
    BattleSamplingEnemy(targets, weapon, self->unitId, BtlUnit_GetEnemyBelong(self),
                        weapon->targetClassFlags, weapon->targetPropertyFlags,
                        (s32)(s8)wp->alliances[self->alliance].attackDirection);
    for (i = 0; i < targets->numTargets; i++) {
        unit = BattleGetUnitPtr(wp, targets->targets[targets->targetIndices[i]].unitIdx);
        if (!(self->attributeFlags & kInactive)) {
            if (unit->currentHp < unit->maxHp) {
                return itemDataTable[itemId].battle_params->attackEvtScript;
            }
        }
    }
    return NULL;
}

/** Returns the item's script when any unit the item can target has less than full FP. */
static void* _check_fp_recover_item(BattleUnit* self) {
    BattleWork* wp;
    s32 itemId;
    BattleWorkWeaponTargets* targets;
    BattleWeapon* weapon;
    s32 i;
    BattleUnit* unit;

    itemId = self->heldItem;
    wp = _battleWorkPointer;
    targets = &wp->weaponTargets;
    weapon = itemDataTable[itemId].battle_params;
    BattleSamplingEnemy(targets, weapon, self->unitId, BtlUnit_GetEnemyBelong(self),
                        weapon->targetClassFlags, weapon->targetPropertyFlags,
                        (s32)(s8)wp->alliances[self->alliance].attackDirection);
    for (i = 0; i < targets->numTargets; i++) {
        s32 max;
        unit = BattleGetUnitPtr(wp, targets->targets[targets->targetIndices[i]].unitIdx);
        if (!(self->attributeFlags & kInactive)) {
            max = BtlUnit_GetMaxFp(unit);
            if (BtlUnit_GetFp(unit) < max) {
                return itemDataTable[itemId].battle_params->attackEvtScript;
            }
        }
    }
    return NULL;
}

/**
 * Damage items: Earthquake and POW Block are used at once if Koops is among the targets and cannot act;
 * otherwise the chance to use the item is 20% per elapsed turn (turnCount * 20 out of 100).
 */
static void* _check_attack_item(BattleUnit* self) {
    BattleWork* wp;
    s32 itemId;
    BattleWorkWeaponTargets* targets;
    BattleWeapon* weapon;
    s32 i;
    BattleUnit* unit;

    itemId = self->heldItem;
    wp = _battleWorkPointer;
    targets = &wp->weaponTargets;
    weapon = itemDataTable[itemId].battle_params;
    BattleSamplingEnemy(targets, weapon, self->unitId, BtlUnit_GetEnemyBelong(self),
                        weapon->targetClassFlags, weapon->targetPropertyFlags,
                        (s32)(s8)wp->alliances[self->alliance].attackDirection);
    if (targets->numTargets <= 0) {
        return NULL;
    }
    if (itemId == ITEM_POW_BLOCK || itemId == ITEM_EARTH_QUAKE) {
        for (i = 0; i < targets->numTargets; i++) {
            unit = BattleGetUnitPtr(wp, targets->targets[targets->targetIndices[i]].unitIdx);
            if (unit->currentKind == UNIT_KOOPS && !(self->attributeFlags & kInactive) && !BtlUnit_CanActStatus(unit)) {
                return itemDataTable[itemId].battle_params->attackEvtScript;
            }
        }
    }
    if (irand(100) >= wp->turnCount * 20) {
        return NULL;
    }
    return itemDataTable[itemId].battle_params->attackEvtScript;
}

/**
 * Status attack items: Sleepy Sheep and Stopwatch are used when some target can still act;
 * Mini Mr. Mini, Mr. Softener, Ruin Powder and Dizzy Dial when some target can act
 * and does not already have that status.
 */
static void* _check_status_attack_item(BattleUnit* self) {
    BattleWork* wp;
    s32 itemId;
    BattleWorkWeaponTargets* targets;
    BattleWeapon* weapon;
    s32 i;
    BattleUnit* unit;

    itemId = self->heldItem;
    wp = _battleWorkPointer;
    targets = &wp->weaponTargets;
    weapon = itemDataTable[itemId].battle_params;
    BattleSamplingEnemy(targets, weapon, self->unitId, BtlUnit_GetEnemyBelong(self),
                        weapon->targetClassFlags, weapon->targetPropertyFlags,
                        (s32)(s8)wp->alliances[self->alliance].attackDirection);
    if (targets->numTargets <= 0) {
        return NULL;
    }
    for (i = 0; i < targets->numTargets; i++) {
        unit = BattleGetUnitPtr(wp, targets->targets[targets->targetIndices[i]].unitIdx);
        if (!(self->attributeFlags & kInactive)) {
            switch (itemId) {
            case ITEM_SLEEPY_SHEEP:
            case ITEM_STOPWATCH:
                if (BtlUnit_CanActStatus(unit)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            case ITEM_MINI_MR_MINI:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_TINY)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            case ITEM_MR_SOFTENER:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_DEFENSE_DOWN)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            case ITEM_RUIN_POWDER:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_CONFUSE)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            case ITEM_DIZZY_DIAL:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_DIZZY)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            }
        }
    }
    return NULL;
}

/**
 * Support items: Power Punch, Courage Shell, Boo's Sheet,
 * Volt Shroom and Repel Cape are used when some ally lacks the status they give (Power Punch also needs the ally
 * able to act).
 */
static void* _check_status_support_item(BattleUnit* self) {
    BattleWork* wp;
    s32 itemId;
    BattleWorkWeaponTargets* targets;
    BattleWeapon* weapon;
    s32 i;
    BattleUnit* unit;

    itemId = self->heldItem;
    wp = _battleWorkPointer;
    targets = &wp->weaponTargets;
    weapon = itemDataTable[itemId].battle_params;
    BattleSamplingEnemy(targets, weapon, self->unitId, BtlUnit_GetEnemyBelong(self),
                        weapon->targetClassFlags, weapon->targetPropertyFlags,
                        (s32)(s8)wp->alliances[self->alliance].attackDirection);
    if (targets->numTargets <= 0) {
        return NULL;
    }
    for (i = 0; i < targets->numTargets; i++) {
        unit = BattleGetUnitPtr(wp, targets->targets[targets->targetIndices[i]].unitIdx);
        if (!(self->attributeFlags & kInactive)) {
            switch (itemId) {
            case ITEM_POWER_PUNCH:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_HUGE)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            case ITEM_COURAGE_SHELL:
                if (!BtlUnit_CheckStatus(unit, STATUS_DEFENSE_UP)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            case ITEM_BOOS_SHEET:
                if (!BtlUnit_CheckStatus(unit, STATUS_INVISIBLE)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            case ITEM_VOLT_SHROOM:
                if (!BtlUnit_CheckStatus(unit, STATUS_ELECTRIC)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            case ITEM_REPEL_CAPE:
                if (!BtlUnit_CheckStatus(unit, STATUS_DODGY)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            }
        }
    }
    return NULL;
}

/**
 * Tasty Tonic: used when some ally has a status it cures (sleep, stop, dizzy, poison, confusion, electric, burn,
 * freeze, tiny, defense down or slow).
 */
static void* _check_status_recover_item(BattleUnit* self) {
    BattleWork* wp;
    s32 itemId;
    BattleWorkWeaponTargets* targets;
    BattleWeapon* weapon;
    s32 i;
    BattleUnit* unit;

    itemId = self->heldItem;
    wp = _battleWorkPointer;
    targets = &wp->weaponTargets;
    weapon = itemDataTable[itemId].battle_params;
    BattleSamplingEnemy(targets, weapon, self->unitId, BtlUnit_GetEnemyBelong(self),
                        weapon->targetClassFlags, weapon->targetPropertyFlags,
                        (s32)(s8)wp->alliances[self->alliance].attackDirection);
    if (targets->numTargets <= 0) {
        return NULL;
    }
    for (i = 0; i < targets->numTargets; i++) {
        unit = BattleGetUnitPtr(wp, targets->targets[targets->targetIndices[i]].unitIdx);
        if (!(self->attributeFlags & kInactive)) {
            switch (itemId) {
            case ITEM_TASTY_TONIC:
                if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) ||
                    BtlUnit_CheckStatus(unit, STATUS_STOP) ||
                    BtlUnit_CheckStatus(unit, STATUS_DIZZY) ||
                    BtlUnit_CheckStatus(unit, STATUS_POISON) ||
                    BtlUnit_CheckStatus(unit, STATUS_CONFUSE) ||
                    BtlUnit_CheckStatus(unit, STATUS_ELECTRIC) ||
                    BtlUnit_CheckStatus(unit, STATUS_BURN) ||
                    BtlUnit_CheckStatus(unit, STATUS_FREEZE) ||
                    BtlUnit_CheckStatus(unit, STATUS_TINY) ||
                    BtlUnit_CheckStatus(unit, STATUS_DEFENSE_DOWN) ||
                    BtlUnit_CheckStatus(unit, STATUS_SLOW)) {
                    return itemDataTable[itemId].battle_params->attackEvtScript;
                }
                break;
            }
        }
    }
    return NULL;
}


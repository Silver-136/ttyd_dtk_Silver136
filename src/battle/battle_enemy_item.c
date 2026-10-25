#pragma dont_inline on
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "data/item_data.h"
#include "system.h"
#include "battle/battle_status_effect.h"

extern ItemData itemDataTable[];
void BattleSamplingEnemy(BattleWorkWeaponTargets* targets, BattleWeapon* weapon, s32 unitId, s8 belong, u32 classFlags, u32 propFlags, s32 direction);

void* _check_status_recover_item(BattleUnit* self);
void* _check_status_support_item(BattleUnit* self);
void* _check_status_attack_item(BattleUnit* self);
void* _check_attack_item(BattleUnit* self);
void* _check_fp_recover_item(BattleUnit* self);
void* _check_hp_recover_item(BattleUnit* self);



#define SETUP()                                                                      \
    BattleWork* wp;                                                                  \
    s32 itemId;                                                                      \
    BattleWorkWeaponTargets* targets;                                                \
    BattleWeapon* weapon;                                                            \
    s32 i;                                                                           \
    BattleUnit* unit;                                                                \
    itemId = self->heldItem;                                                         \
    wp = _battleWorkPointer;                                                         \
    targets = &wp->weaponTargets;                                                    \
    weapon = itemDataTable[itemId].battle_params;                                    \
    BattleSamplingEnemy(targets, weapon, self->unitId, BtlUnit_GetEnemyBelong(self), \
    weapon->targetClassFlags, weapon->targetPropertyFlags,                           \
    (s32)(s8)wp->alliances[self->mAlliance].attackDirection);

#define GETUNIT() unit = BattleGetUnitPtr(wp, targets->targets[targets->targetIndices[i]].unitIdx)
#define RET() return itemDataTable[itemId].battle_params->mAttackEvtScript
#define NOFLAG (self->mAttributeFlags & 0x20000)

void* BattleEnemyUseItemCheck(BattleUnit* self) {
    s32 itemId = self->heldItem;
    BattleWeapon* weapon;
    void* r;
    if (itemId == 0) {
        return NULL;
    }
    weapon = itemDataTable[itemId].battle_params;
    if (weapon == NULL) {
        return NULL;
    }
    if ((self->mTokenFlags & 0x10) && !(weapon->special & 0x10000)) {
        return NULL;
    }
    switch (itemId) {
    case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x8B: case 0x90:
        return _check_attack_item(self);
    case 0x89: case 0x8A: case 0x8C: case 0x8D: case 0x92: case 0x93:
        return _check_status_attack_item(self);
    case 0x86: case 0x87: case 0x88: case 0x8E: case 0x8F:
        return _check_status_support_item(self);
    case 0x99:
        return _check_status_recover_item(self);
    case 0x9D:
        return _check_hp_recover_item(self);
    case 0x9E:
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

void* _check_hp_recover_item(BattleUnit* self) {
    SETUP()
    for (i = 0; i < targets->numTargets; i++) {
        GETUNIT();
        if (!NOFLAG) {
            if (unit->currentHp < unit->maxHp) {
                RET();
            }
        }
    }
    return NULL;
}

void* _check_fp_recover_item(BattleUnit* self) {
    SETUP()
    for (i = 0; i < targets->numTargets; i++) {
        s32 max;
        GETUNIT();
        if (!NOFLAG) {
            max = BtlUnit_GetMaxFp(unit);
            if (BtlUnit_GetFp(unit) < max) {
                RET();
            }
        }
    }
    return NULL;
}

void* _check_attack_item(BattleUnit* self) {
    SETUP()
    if (targets->numTargets <= 0) {
        return NULL;
    }
    if (itemId == 0x8B || itemId == 0x85) {
        for (i = 0; i < targets->numTargets; i++) {
            GETUNIT();
            if (unit->currentKind == UNIT_KOOPS && !NOFLAG && !BtlUnit_CanActStatus(unit)) {
                RET();
            }
        }
    }
    if (irand(100) >= wp->turnCount * 20) {
        return NULL;
    }
    RET();
}

void* _check_status_attack_item(BattleUnit* self) {
    SETUP()
    if (targets->numTargets <= 0) {
        return NULL;
    }
    for (i = 0; i < targets->numTargets; i++) {
        GETUNIT();
        if (!NOFLAG) {
            switch (itemId) {
            case 0x8A: case 0x8C:
                if (BtlUnit_CanActStatus(unit)) {
                    RET();
                }
                break;
            case 0x92:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_TINY)) {
                    RET();
                }
                break;
            case 0x93:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_DEFENSE_DOWN)) {
                    RET();
                }
                break;
            case 0x89:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_CONFUSE)) {
                    RET();
                }
                break;
            case 0x8D:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_DIZZY)) {
                    RET();
                }
                break;
            }
        }
    }
    return NULL;
}

void* _check_status_support_item(BattleUnit* self) {
    SETUP()
    if (targets->numTargets <= 0) {
        return NULL;
    }
    for (i = 0; i < targets->numTargets; i++) {
        GETUNIT();
        if (!NOFLAG) {
            switch (itemId) {
            case 0x8E:
                if (BtlUnit_CanActStatus(unit) && !BtlUnit_CheckStatus(unit, STATUS_HUGE)) {
                    RET();
                }
                break;
            case 0x8F:
                if (!BtlUnit_CheckStatus(unit, STATUS_DEFENSE_UP)) {
                    RET();
                }
                break;
            case 0x86:
                if (!BtlUnit_CheckStatus(unit, STATUS_INVISIBLE)) {
                    RET();
                }
                break;
            case 0x87:
                if (!BtlUnit_CheckStatus(unit, STATUS_ELECTRIC)) {
                    RET();
                }
                break;
            case 0x88:
                if (!BtlUnit_CheckStatus(unit, STATUS_DODGY)) {
                    RET();
                }
                break;
            }
        }
    }
    return NULL;
}

void* _check_status_recover_item(BattleUnit* self) {
    SETUP()
    if (targets->numTargets <= 0) {
        return NULL;
    }
    for (i = 0; i < targets->numTargets; i++) {
        GETUNIT();
        if (!NOFLAG) {
            switch (itemId) {
            case 0x99:
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
                    RET();
                }
                break;
            }
        }
    }
    return NULL;
}


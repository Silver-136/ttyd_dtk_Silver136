#include "battle/battle_detect.h"

void BattleSamplingEnemyUpdate(void) {
    extern void _btlSamplingEnemy(void);

    _btlSamplingEnemy();
}


void _btlSamplingEnemy(void* targetWork) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    extern void* BtlUnit_GetPartsPtr(void*, s32);
    extern s32 BtlUnit_CheckStatus(void*, s32);
    s32 unitIdx;
    s32 partIdx;
    s32 count;
    u8* target;
    void* unit;
    void* part;

    count = 0;
    target = (u8*)((s32)targetWork + 4);
    if ((*(u32*)((s32)targetWork + 0xAC0) & 0x80000000) == 0) {
        for (unitIdx = 0; unitIdx < 64; unitIdx++) {
            unit = BattleGetUnitPtr(_battleWorkPointer, unitIdx);
            if (unit == 0 || BtlUnit_CheckStatus(unit, 0x19) != 0 ||
                (*(u32*)((s32)unit + 0x104) & 0x40) != 0) {
                continue;
            }
            for (partIdx = 1; partIdx <= *(u8*)(*(s32*)((s32)unit + 0x10) + 1); partIdx++) {
                part = BtlUnit_GetPartsPtr(unit, partIdx);
                if (part == 0 || (*(u32*)((s32)part + 0x1AC) & 0x100) != 0) {
                    continue;
                }
                *(s16*)(target + count * 0x24) = unitIdx;
                *(s16*)(target + count * 0x24 + 2) = partIdx;
                *((s8*)((s32)targetWork + 0xA70) + count) = count;
                count++;
            }
        }
    }
    *(u8*)((s32)targetWork + 0xAC9) = count;
    *(u8*)((s32)targetWork + 0xACA) = 0;
}

void BattleChoiceSamplingEnemy(void* targetWork, u32 weighting, int* unitIdx, int* partIdx) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    u32 weights[74];
    s8* order;
    u8 count;
    s32 i;
    s32 index;
    s32 bestWeight;

    *unitIdx = -1;
    *partIdx = 0;
    count = *(u8*)((s32)targetWork + 0xAC9);
    if (count == 0) {
        return;
    }
    order = (s8*)((s32)targetWork + 0xA70);
    for (i = 0; i < 74; i++) {
        weights[i] = 0;
    }
    bestWeight = -1;
    for (i = 0; i < count; i++) {
        void* unit;
        index = order[i];
        weights[index] += 100;
        unit = BattleGetUnitPtr(_battleWorkPointer,
                                *(s16*)((s32)targetWork + 4 + index * 0x24));
        if ((weighting & 1) != 0 && *(s32*)((s32)unit + 8) == 0) {
            weights[index] = weights[index] * 3 / 2;
        }
        if ((weighting & 0x40) != 0) {
            weights[index] += (count - i) * 10;
        }
        if ((s32)weights[index] > bestWeight) {
            bestWeight = weights[index];
            *unitIdx = *(s16*)((s32)targetWork + 4 + index * 0x24);
            *partIdx = *(s16*)((s32)targetWork + 6 + index * 0x24);
        }
    }
}

s32 BattleSamplingEnemy(void* targetWork, void* weapon, int attackerIdx, int enemyBelong, u32 targetClassFlags, u32 targetPropertyFlags, u8 param_7) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void*, s32);
    s32 changed;
    u32 flags;

    changed = 0;
    *(void**)targetWork = weapon;
    *(s32*)((s32)targetWork + 0xAB8) = attackerIdx;
    *(u8*)((s32)targetWork + 0xABC) = enemyBelong;
    *(u32*)((s32)targetWork + 0xAC0) = targetClassFlags;
    *(u32*)((s32)targetWork + 0xAC4) = targetPropertyFlags;
    *(u8*)((s32)targetWork + 0xAC8) = param_7;

    if ((*(u32*)((s32)BattleGetUnitPtr(_battleWorkPointer, attackerIdx) + 0x27C) & 0x10) != 0) {
        flags = *(u32*)((s32)targetWork + 0xAC0);
        if (((flags & 0x100) != 0) && ((flags & 0x200) == 0)) {
            *(u32*)((s32)targetWork + 0xAC0) = (flags & ~0x100);
            *(u32*)((s32)targetWork + 0xAC0) |= 0x200;
            changed = 1;
        } else if (((flags & 0x100) == 0) && ((flags & 0x200) != 0)) {
            *(u32*)((s32)targetWork + 0xAC0) |= 0x100;
            *(u32*)((s32)targetWork + 0xAC0) &= ~0x200;
            changed = 1;
        }
    }

    ((void (*)(void*))_btlSamplingEnemy)(targetWork);
    return changed;
}

void BattleGetFirstAttackUnit(void* battleWork, void** part, void** unit) {
    extern void* BattleGetUnitPtr(void*, s32);
    extern void* BattleGetMarioPtr(void*);
    extern void* BattleGetPartnerPtr(void*, void*);
    void* foundUnit;
    void* otherUnit;
    s32 i;
    s32 mode;

    otherUnit = NULL;
    for (i = 0; i < 64; i++) {
        foundUnit = BattleGetUnitPtr(battleWork, i);
        if (foundUnit != NULL && *(s8*)((s32)foundUnit + 0xC) == 1) {
            break;
        }
    }

    mode = *(s32*)(*(s32*)((s32)battleWork + 0x2738) + 8);
    switch (mode) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            otherUnit = foundUnit;
            foundUnit = BattleGetMarioPtr(battleWork);
            break;
        case 7:
        case 8:
            otherUnit = foundUnit;
            foundUnit = BattleGetPartnerPtr(battleWork, BattleGetMarioPtr(battleWork));
            break;
        default:
            otherUnit = BattleGetMarioPtr(battleWork);
            break;
    }
    *part = foundUnit;
    *unit = otherUnit;
}
